
/*
 * Torque task list launcher tool.
 *
 * Copyright (c) 2015 Axel Kohlmeyer <akohlmey@gmail.com>
 */

/* API for managing the compute nodes */

#ifndef TL_NODE_MGR_H
#define TL_NODE_MGR_H

#include "torque.h"
#include "task-mgr.h"

typedef struct {
    task_t *task;
    int status;
    tm_event_t event;
} node_t;

typedef struct {
    int nall;
    int nrun;
    node_t *node;
    struct tm_roots roots;
    tm_node_id *nodeid;
} node_mgr_t;

/*! Allocate and initialize a node list struct
 * \param num number of nodes in reservation
 * \return allocated node list struct
 */
node_mgr_t *node_mgr_init();

/*! Clean up and free node list struct
 * \param n node list struct allocated by node_mgr_init
 */
void node_mgr_exit(node_mgr_t *n);

/*! Launch a task on an idle node
 * \param n node list struct allocated by node_mgr_init
 * \param t task struct to execute
 * \return allocated node id if successful, otherwise TM_ERROR_NODE
 */
tm_node_id node_mgr_run(node_mgr_t *n, task_t *t);

/*! Return number of total nodes in node list
 * \param t node list struct allocated by node_mgr_init
 * \return number of nodes
 */
int node_mgr_nall(node_mgr_t *t);

/*! Return number of idle nodes in node list
 * \param t node list struct allocated by node_mgr_init
 * \return number of nodes
 */
int node_mgr_nidle(node_mgr_t *t);

/*! Process pending Torque events
 * \param t node list struct allocated by node_mgr_init
 * \return 1 if event processed, 0 if no event was pending
 */
void node_mgr_event(node_mgr_t *t);

/*! Print node list
 * \param t node list struct allocated by node_mgr_init
 */
void node_mgr_print(node_mgr_t *t);



#endif

/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
