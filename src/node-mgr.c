
/*
 * Torque task list launcher tool.
 *
 * Copyright (c) 2015 Axel Kohlmeyer <akohlmey@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tm.h>

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
    printf("Task %d  Parent Task %d  Nodes %d\n",
           n->roots.tm_me, n->roots.tm_parent, n->roots.tm_nnodes);
    tm_nodeinfo(&(n->nodeid),&(n->nall));
    for (i = 0; i < n->nall; ++i) {
        printf("Node %d/%d: id %d\n",i+1,n->nall,n->nodeid[i]);
    }

    n->node = (node_t *)calloc(n->nall,sizeof(node_t));
    if (n->node == NULL) {
        tm_finalize();
        free((void *)n);
        return NULL;
    }
    return n;
}



/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
