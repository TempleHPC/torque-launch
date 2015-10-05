
/*
 * Torque task list launcher tool.
 *
 * Copyright (c) 2015 Axel Kohlmeyer <akohlmey@gmail.com>
 */

#include <stdio.h>

#include "task-mgr.h"
#include "node-mgr.h"

#define LINEBUFSZ 2048

int main(int argc, char **argv)
{
    FILE *fp;
    task_mgr_t *t;
    int i,nlines;
    const char *ptr;
    char linebuf[LINEBUFSZ];

    if (argc != 2) {
        printf("\nUsage:  %s <joblist filename>\n\n",argv[0]);
        return 1;
    }

    fp = fopen(argv[1],"r");
    if (!fp) {
        perror("Error: could not open job list file");
        return 2;
    }

    /* count number of lines in file */
    nlines = 0;
    while (!feof(fp) && !ferror(fp)) {
        if (fgets(linebuf,LINEBUFSZ,fp) != NULL)
            ++nlines;
    }

    t = task_mgr_init(nlines);
    if (t == NULL) {
        printf("Error allocating internal data for %d tasks\n",nlines);
        return 3;
    }

    rewind(fp);
    for (i = 0; i < nlines; ++i) {
        ptr = fgets(linebuf,LINEBUFSZ,fp);
        if (ptr == NULL) break;
        if (task_mgr_add(t,ptr) != 0) {
            printf("Error adding line %d of %d to task list\n",i+1,nlines);
            task_mgr_exit(t);
            return 4;
        }
    }
    fclose(fp);

    printf("Found %d tasks in task list file '%s'\n",
           task_mgr_nall(t),argv[1]);
    /* task_mgr_print(t); */
    return 0;
}


/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
