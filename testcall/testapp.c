#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>
#include "procinfo.h"

struct ioc_arg 
{
	pid_t ipid;
	struct procinfo pinfo;
}ioc;

#define MAGIC_NO 'k'

int main (int argc, char *argv[])
{
    int fd, rc;
    int MY_IOCTL;
    int user_pid;
    time_t start,end;
    char *nodename = "/dev/getprocinfo";

    /* open device node */

    if (argc > 1)
        nodename = argv[1];
    fd = open (nodename, O_RDWR);
    printf (" I opened the device node, file descriptor = %d\n", fd);

    
    printf("Enter the PID of the process\n");
    scanf("%d",&user_pid);
    ioc.ipid = user_pid;
    printf ("\n Sending from user-space: pid %d \n",ioc.ipid);
    time(&start);
    /* send to the device */
 
    MY_IOCTL = (int)_IOW (MAGIC_NO, 1, struct ioc_arg);
    
    rc = ioctl (fd, MY_IOCTL, &ioc);
    
    /* reading from the device */
    
    MY_IOCTL = (int)_IOR (MAGIC_NO, 1, struct ioc_arg);
    rc = ioctl (fd, MY_IOCTL, &ioc);
    time(&end);
    printf(" PID = %d\n  PPID = %d\n  Process start time %d\n  number of siblings= %d\n", ioc.pinfo.pid, ioc.pinfo.ppid, (int)ioc.pinfo.start_time.tv_sec,ioc.pinfo.num_sib);
    printf("Time taken to execute system call %lf seconds \n",difftime(end,start));

   

    close (fd);
    exit (0);

}

