
/*
 * Torque task list launcher tool.
 *
 * Copyright (c) 2015,2017 Axel Kohlmeyer <akohlmey@gmail.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_SYSLOG
#include <syslog.h>
#endif

#include "task-mgr.h"
#include "node-mgr.h"

/** maximum length of line in joblist file */
#define LINEBUFSZ 2048

/** sleep time in seconds between scheduling/polling */
#define SCHEDULE_INTERVAL 2


#ifdef USE_SYSLOG
const char *logname = "torque-launch";
const char *pbsjobid = "(unknown)";
#endif

static int usage(const char *argv0)
{
    printf("\nUsage:  %s [-f|-r|-m|-c <center task #>] "
           "[-p <checkpoint filename>] <joblist filename>\n"
           "Meaning of flags:\n"
           " -f   : process tasks in forward order (default)\n"
           " -r   : process tasks in reverse order\n"
           " -m   : process tasks starting in the middle\n"
           " -c # : like -mid but start with task #\n"
           " -p name : name of checkpoint file\n",argv0);
    return 1;
}

int main(int argc, char **argv)
{
    FILE *fp;
    task_mgr_t *t;
    node_mgr_t *n;
    int i,opt,reorderflag,center,nlines,nnodes;
    const char *ptr,*checkpoint;
    char linebuf[LINEBUFSZ];

    if ((argc < 2) || (argc > 6))
        return usage(argv[0]);

    reorderflag = REORDER_NOTSET;
    center = -1;
    checkpoint = NULL;

    while ((opt = getopt(argc,argv,"frmc:p:")) != -1) {
        switch (opt) {

          case 'f':
              if (reorderflag != REORDER_NOTSET) return usage(argv[0]);
              reorderflag = REORDER_FORWARD;
              break;

          case 'r':
              if (reorderflag != REORDER_NOTSET) return usage(argv[0]);
              reorderflag = REORDER_REVERSE;
              break;

          case 'm':
              if (reorderflag != REORDER_NOTSET) return usage(argv[0]);
              reorderflag = REORDER_CENTER;
              center = -1;
              break;

          case 'c':
              if (reorderflag != REORDER_NOTSET) return usage(argv[0]);
              reorderflag = REORDER_CENTER;
              center = atoi(optarg);
              break;

          case 'p':
              checkpoint = strdup(optarg);
              break;

          default:
              return usage(argv[0]);
        }
    }

    if (optind >= argc) return usage(argv[0]);
    if (reorderflag == REORDER_NOTSET) reorderflag = REORDER_FORWARD;

    fp = fopen(argv[optind],"r");
    if (!fp) {
        perror("Error opening job list file:");
        return 2;
    }

    /* count number of lines in file */
    nlines = 0;
    while (!feof(fp) && !ferror(fp)) {
        if (fgets(linebuf,LINEBUFSZ,fp) != NULL)
            ++nlines;
    }

    /* initialize task manager */
    t = task_mgr_init(nlines);
    if (t == NULL) {
        printf("Error allocating internal data for %d tasks.\n",nlines);
        return 3;
    }
    rewind(fp);
    for (i = 0; i < nlines; ++i) {
        ptr = fgets(linebuf,LINEBUFSZ,fp);
        if (ptr == NULL) break;
        if (task_mgr_add(t,ptr) != 0) {
            printf("Error adding line %d of %d to task list.\n",i+1,nlines);
            task_mgr_exit(t);
            return 4;
        }
    }
    fclose(fp);
    printf("Found %d tasks in task list file '%s'.\n",
           task_mgr_nall(t),argv[optind]);

    /* determine middle of list, if not already set */
    if ((reorderflag == REORDER_CENTER) && (center < 0))
        center = task_mgr_nall(t)/2;

    if ((reorderflag == REORDER_CENTER) && (center >= task_mgr_nall(t))) {
        printf("Center task %d out of range. "
               "Switching to reverse order\n", center);
        reorderflag = REORDER_REVERSE;
    }

    /* reorder tasks, if requested */
    t = task_mgr_reorder(t,reorderflag,center);

#ifdef USE_SYSLOG
    pbsjobid = getenv("PBS_JOBID");
    openlog(logname,LOG_PID|LOG_ODELAY,LOG_LOCAL2);
    syslog(LOG_INFO,"{\"job_id\": %s, \"event\": \"launch\", "
           "\"num_tasks\": %d}",pbsjobid,task_mgr_nall(t));
#endif

    /* initialize node manager */
    n = node_mgr_init();
    if (n == NULL) {
        printf("Error allocating nodes for task processing.\n");
        task_mgr_exit(t);
        return 5;
    }
    nnodes = node_mgr_nall(n);
    printf("Distributing tasks to %d processors.\n",nnodes);

    /* schedule tasks to node when they become available */
    while (task_mgr_todo(t) > 0) {
        task_mgr_chkpnt(t,checkpoint);

        /* task available, node available -> launch task */
        if ((task_mgr_todo(t) > 0) && (node_mgr_nidle(n) > 0)) {
            if (node_mgr_run(n,task_mgr_next(t)) == TM_ERROR_NODE) {
                printf("Error scheduling pending task. Aborting\n");
                break;
            }
        }
       
        /* process pending events */
        if (node_mgr_schedule(n)) continue;
        sleep(SCHEDULE_INTERVAL);
    }

    /* wait for remaining calculations to complete */
    while (node_mgr_nidle(n) < nnodes) {
        task_mgr_chkpnt(t,checkpoint);
        if (node_mgr_schedule(n)) continue;
        sleep(SCHEDULE_INTERVAL);
    }

    /* shut down and clean up */
    node_mgr_exit(n);
    task_mgr_exit(t);

#ifdef USE_SYSLOG
    syslog(LOG_INFO,"{\"job_id\": %s, \"event\": \"exit\"}",pbsjobid);
    closelog();
#endif
    if (checkpoint != NULL) unlink(checkpoint);

    return 0;
}


/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
