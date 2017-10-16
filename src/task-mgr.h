
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

/* task list sort flags */
#define SORT_NOTSET  0
/** process tasks in the order of the job list file (default, no-op) */
#define SORT_FORWARD 1
/** process tasks in reverse order of the job list file */
#define SORT_REVERSE 2
/** process tasks centered around task # given.
    use middle of list unless a different task # is given */
#define SORT_CENTER  3

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

/*! Change status of completed task
 * \param t task list element
 */
void task_done(task_t *t);

/*! Sort list of tasks in one of several ways
 * \param t task list struct allocated by task_mgr_init
 * \param s sort flag, determines sort order (forward, reverse, or center)
 * \param c center task for center sort order
 * \return sorted task list struct
 */
task_mgr_t *task_mgr_sort(task_mgr_t *t, const int s, const int c);

#endif

/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
