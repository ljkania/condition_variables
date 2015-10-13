#define _POSIX_SOURCE 1
#define _MINIX 1
#define _SYSTEM 1

#define MAX 1024

#include<minix/callnr.h>
#include<minix/com.h>
#include<minix/config.h>
#include<minix/const.h>
#include<minix/endpoint.h>
#include<minix/ipc.h>
#include<minix/syslib.h>
#include<minix/sysutil.h>
#include<minix/type.h>

#include<machine/vm.h>
#include<machine/vmparam.h>

#include<sys/errno.h>
#include<sys/ipc.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/vm.h>

#include<errno.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

int m_lock(int, endpoint_t);
int m_unlock(int, endpoint_t);
int m_remove(endpoint_t, int);

int cv_wait(int cond_var_id, int mutex_id, endpoint_t who);
int cv_broadcast(int cond_var_id);
int cv_remove(endpoint_t who);
