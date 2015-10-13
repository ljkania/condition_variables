#define OK    0
#define FAIL -1

#include <minix/callnr.h>
#include <minix/endpoint.h>
#include <sys/cdefs.h>
#include <lib.h>
#include <stdio.h>

int cs_lock(int);
int cs_unlock(int);
int cs_wait(int,int);
int cs_broadcast(int);
