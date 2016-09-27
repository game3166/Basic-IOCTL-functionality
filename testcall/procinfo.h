#ifndef _LINUX_PROCINFO_H
#define _LINUX_PROCINFO_H

#include <linux/types.h>

/*#ifdef __KERNEL__
# include <linux/cache.h>
# include <linux/seqlock.h>
# include <linux/math64.h>
#endif
*/
#ifndef _STRUCT_PROCINFO
#define _STRUCT_PROCINFO
struct procinfo {
	pid_t pid; /* Process ID */
	pid_t ppid; /* Parent process ID */
	struct timespec start_time; /* Process start time */
	int num_sib; /* Number of siblings */
};
#endif

#endif
