
/*
 * Torque task list launcher tool.
 *
 * Copyright (c) 2015 Axel Kohlmeyer <akohlmey@gmail.com>
 */

/* API for managing the task list */

#ifndef TL_TASK_MGR_H
#define TL_TASK_MGR_H

#include "torque.h"

typedef struct {
    const char *cmd;
    int status;
    int exitval;
    tm_node_id nodeid;
    tm_task_id taskid;
} task_t;

typedef struct {
    int nall;
    int nmax;
    int nlast;
    task_t *task;
} task_mgr_t;

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


#endif

/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
