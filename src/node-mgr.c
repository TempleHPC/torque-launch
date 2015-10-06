
/*
 * Torque task list launcher tool.
 *
 * Copyright (c) 2015 Axel Kohlmeyer <akohlmey@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "torque.h"
#include "node-mgr.h"

#define NODE_IDLE 0
#define NODE_EXEC 1
#define NODE_BUSY 2


static const char *status[] = {
    "idle", "exec", "busy", NULL
};

node_mgr_t *node_mgr_init()
{
    int i;
    node_mgr_t *n = (node_mgr_t *)malloc(sizeof(node_mgr_t));
    if (n == NULL) return NULL;

    if (tm_init(&i,&(n->roots)) != TM_SUCCESS) {
        free((void *)n);
        printf("Error initializing Torque environment");
        return NULL;
    }

    /* make sure the task launcher has no parent task */
    if (n->roots.tm_parent != 0) {
        printf("Nested task scheduling not supported\n");
        free((void *)n);
        return NULL;
    }

    tm_nodeinfo(&(n->nodeid),&(n->nall));
    n->nrun = 0;
    n->node = (node_t *)calloc(n->nall,sizeof(node_t));
    if (n->node == NULL) {
        tm_finalize();
        free((void *)n->nodeid);
        free((void *)n);
        return NULL;
    }
    return n;
}


void node_mgr_exit(node_mgr_t *n)
{
    if (n == NULL) return;
    free((void *)n->node);
    tm_finalize();
    free((void *)n->nodeid);
    free((void *)n);
}


tm_node_id node_mgr_run(node_mgr_t *n, task_t *t)
{
    int i, rv;
    tm_node_id id;
    char *job[3];

    if ((n == NULL) || (t == NULL)) return TM_ERROR_NODE;

    job[0] = (char *)"/bin/sh";
    job[1] = (char *)"-c";
    job[2] = (char *)t->cmd;

    for (i = 0; i < n->nall; ++i)
        if (n->node[i].status == NODE_IDLE)
            break;

    id = n->nodeid[i];
    t->nodeid = id;
    n->nrun++;
    n->node[i].status = NODE_EXEC;
    n->node[i].task = t;
    rv = tm_spawn(3,job,NULL,id,&(t->taskid),&(n->node[i].event));

    if (rv == TM_SUCCESS)
        return id;
    else return TM_ERROR_NODE;
}


int node_mgr_schedule(node_mgr_t *n)
{
    int i,rv,event;

    if (n == NULL) return 0;

    rv = tm_poll(TM_NULL_EVENT,&event,0,&i);

    if (rv != TM_SUCCESS) return 0;
    if (event == TM_NULL_EVENT) return 0;

    /* look for node matching the reported event */
    for (i = 0; i < n->nall; ++i) {

        if (n->node[i].event == event) {
            task_t *t = n->node[i].task;

            switch (n->node[i].status) {

            case NODE_EXEC:     /* tm_spawn completed. */
                n->node[i].status = NODE_BUSY;
                tm_obit(t->taskid,&(t->exitval),&(n->node[i].event));
                break;

            case NODE_BUSY:     /* task completed */
                n->node[i].status = NODE_IDLE;
                n->nrun--;
                task_done(t);
                break;

            default:
                printf("Unexpected event %d for node %d  status %d\n",
                       event,n->nodeid[i],n->node[i].status);
            }
            return 1;
        }
    }
    return 0;
}


int node_mgr_nall(node_mgr_t *n)
{
    if (n == NULL) return 0;
    return n->nall;
}


int node_mgr_nidle(node_mgr_t *n)
{
    if (n == NULL) return 0;
    return (n->nall - n->nrun);
}


void node_mgr_print(node_mgr_t *n)
{
    int i;
    if (n == NULL) return;
    printf("Task=%d  Parent=%d  Nodes=%d\n",
           n->roots.tm_me, n->roots.tm_parent, n->roots.tm_nnodes);
    for (i = 0; i < n->nall; ++i) {
        if (n->node[i].status == NODE_IDLE) {
            printf("Node %03d/%03d|%s: (none)\n",i+1, n->nall,
                   status[n->node[i].status]);
        } else {
            printf("Node %03d/%03d|%s: %s",i+1, n->nall,
                   status[n->node[i].status],n->node[i].task->cmd);
        }
    }
}
/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
