
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task-mgr.h"

#define TASK_PENDING   0
#define TASK_RUNNING   1
#define TASK_COMPLETE  2
#define TASK_FAILED    3

static const char *status[] = {
    "pending", "running", "complete", "failed", NULL
};

task_mgr_t *task_mgr_init(int num)
{
    task_mgr_t *t = (task_mgr_t *)malloc(sizeof(task_mgr_t));
    if (t == NULL) return NULL;
    t->task = (task_t *)calloc(num,sizeof(task_t));
    if (t->task == NULL) {
        free((void *)t);
        return NULL;
    }
    t->nmax = num;
    t->nall = 0;
    t->nlast = 0;
    return t;
}


void task_mgr_exit(task_mgr_t *t)
{
    int i;
    if (t != NULL) {
        if (t->task != NULL) {
            for (i = 0; i < t->nmax; ++i) {
                if (t->task[i].cmd != NULL)
                    free((void *)t->task[i].cmd);
            }
            free((void *)t->task);
        }
        free((void *)t);
    }
}


int task_mgr_add(task_mgr_t *t, const char *cmd)
{
    int n;
    if (t == NULL) return 1;
    if (cmd == NULL) return 2;
    /* skip over whitespace */
    while (isspace(*cmd)) ++cmd;
    /* empty or comment-only line */
    if ((*cmd == '\0') || (*cmd == '#')) return 0;

    n = t->nall;
    /* no more reserved space left */
    if (n == t->nmax) return 3;

    /* copy command and init data structure */
    t->task[n].cmd = strdup(cmd);
    if (t->task[n].cmd == NULL) return 4;
    t->task[n].status = TASK_PENDING;
    t->task[n].nodeid = TM_ERROR_NODE;
    t->task[n].taskid = TM_NULL_TASK;
    t->nall = n+1;
    return 0;
}


int task_mgr_nall(task_mgr_t *t)
{
    if (t == NULL) return 0;
    return t->nall;
}


int task_mgr_todo(task_mgr_t *t)
{
    if (t == NULL) return 0;
    return t->nall - t->nlast;
}


task_t *task_mgr_next(task_mgr_t *t)
{
    if (t == NULL) return NULL;
    if (t->nlast < t->nall) {
        task_t *n = &(t->task[t->nlast]);
        n->status = TASK_RUNNING;
        t->nlast++;
        return n;
    }
    return NULL;
}


void task_mgr_print(task_mgr_t *t)
{
    int i;
    if (t == NULL) return;
    for (i = 0; i < t->nall; ++i) {
        printf("%03d/%03d|[%-8s]: %s",i+1,t->nall,
               status[t->task[i].status],t->task[i].cmd);
    }
}
/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
