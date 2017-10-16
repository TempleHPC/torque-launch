
/*
 * Torque task list launcher tool.
 *
 * Copyright (c) 2015,2017 Axel Kohlmeyer <akohlmey@gmail.com>
 */

/* API for managing the task list */

#ifndef TL_TASK_MGR_H
#define TL_TASK_MGR_H

#include "torque.h"

typedef struct {
    const char *cmd;
    int status;
    int exitval;
    int tasknum;
    tm_node_id nodeid;
    tm_task_id taskid;
} task_t;

typedef struct {
    int nall;
    int nmax;
    int nlast;
    task_t *task;
} task_mgr_t;

/* task list reorder flags */
#define REORDER_NOTSET  0
/** process tasks in the order of the job list file (default, no-op) */
#define REORDER_FORWARD 1
/** process tasks in reverse order of the job list file */
#define REORDER_REVERSE 2
/** process tasks centered around task # given.
    use middle of list unless a different task # is given */
#define REORDER_CENTER  3

/*! Allocate and initialize a task list struct
 * \param num maximum number of tasks
 * \return allocated task list struct
 */
task_mgr_t *task_mgr_init(int num);

/*! Clean up and free task list struct
 * \param t task list struct allocated by task_mgr_init
 */
void task_mgr_exit(task_mgr_t *t);

/*! Add an entry to task list
 * \param t task list struct allocated by task_mgr_init
 * \param cmd command to execute for this task
 * \return 0 if successful, other if add failed.
 */
int task_mgr_add(task_mgr_t *t, const char *cmd);

/*! Return number of total tasks in task list
 * \param t task list struct allocated by task_mgr_init
 * \return number of tasks
 */
int task_mgr_nall(task_mgr_t *t);

/*! Return number of pending tasks in task list
 * \param t task list struct allocated by task_mgr_init
 * \return number of tasks
 */
int task_mgr_todo(task_mgr_t *t);

/*! Return next pending command in task list
 * \param t task list struct allocated by task_mgr_init
 * \return command string
 */
task_t *task_mgr_next(task_mgr_t *t);

/*! Print task list
 * \param t task list struct allocated by task_mgr_init
 */
void task_mgr_print(task_mgr_t *t);

/*! Write task list checkpoint file
 * \param t task list struct allocated by task_mgr_init
 * \param n name of the checkpoint file
 */
void task_mgr_chkpnt(task_mgr_t *t, const char *n);

/*! Change status of completed task
 * \param t task list element
 */
void task_done(task_t *t);

/*! Reorder list of tasks in one of several ways
 * \param t task list struct allocated by task_mgr_init()
 * \param s reorder flag, determines list order (forward, reverse, or center)
 * \param c center task for reorder around center
 * \return reordered task list struct
 */
task_mgr_t *task_mgr_reorder(task_mgr_t *t, const int s, const int c);

#endif

/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
