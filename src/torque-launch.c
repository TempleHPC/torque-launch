
/*
 * Torque task list launcher tool.
 *
 * Copyright (c) 2015 Axel Kohlmeyer <akohlmey@gmail.com>
 */

#include <stdio.h>
#include <unistd.h>

#include "task-mgr.h"
#include "node-mgr.h"

#define LINEBUFSZ 2048

int main(int argc, char **argv)
{
    FILE *fp;
    task_mgr_t *t;
    node_mgr_t *n;
    int i,nlines,nnodes;
    const char *ptr;
    char linebuf[LINEBUFSZ];

    if (argc != 2) {
        printf("\nUsage:  %s <joblist filename>\n\n",argv[0]);
        return 1;
    }

    fp = fopen(argv[1],"r");
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
           task_mgr_nall(t),argv[1]);
    /* task_mgr_print(t); */

    /* initialize node manager */
    n = node_mgr_init();
    if (n == NULL) {
        printf("Error allocating nodes for task processing.\n");
        task_mgr_exit(t);
        return 5;
    }
    nnodes = node_mgr_nall(n);
    printf("Distributing tasks to %d processors.\n",nnodes);
    node_mgr_print(n);

    /* schedule tasks */
    while ((task_mgr_todo(t) > 0) && (node_mgr_nidle(n) < nnodes)) {

        /* task available, node available -> launch task */
        if ((task_mgr_todo(t) > 0) && (node_mgr_nidle(n) > 0)) {
            if (node_mgr_run(n,task_mgr_next(t)) == TM_ERROR_NODE) {
                printf("Error scheduling pending task. Aborting\n");
                break;
            }
        }
       
        /* process pending events */
        if (node_mgr_schedule(n)) continue;
        sleep(10);
        node_mgr_print(n);
        task_mgr_print(t);
    }

    /* shut down and clean up */
    node_mgr_exit(n);
    task_mgr_exit(t);
    return 0;
}


/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
