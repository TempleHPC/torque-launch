
/*
 * Torque task list launcher tool.
 *
 * Copyright (c) 2015,2017 Axel Kohlmeyer <akohlmey@gmail.com>
 */

/* wrapper around tm.h to avoid multiple inclusion */

#ifndef TL_TORQUE_H
#define TL_TORQUE_H

#include <tm.h>

#ifdef USE_SYSLOG
/*! ident string to be used in syslog calls */
extern const char *logname;
extern const char *pbsjobid;
#endif

#endif

/*
 * Local Variables:
 * c-basic-offset: 4
 * auto-revert-buffer: 't
 * compile-command: "make -k -C ../ fedora"
 * End:
 */
