#include <cv.h>

int cs_lock(int mutex_id)
{
	int cv;
	minix_rs_lookup("cv", &cv);

	message m;
	m.m1_i1 = mutex_id;

	int result;

	while((result=_syscall(cv, CS_LOCK, &m)) == FAIL)
		if(errno==EINTR)
			m.m1_i1 = mutex_id;
		else
			return FAIL;

	return result;
}

int cs_unlock(int mutex_id)
{
	int cv;
    minix_rs_lookup("cv", &cv);

    message m;
    m.m1_i1 = mutex_id;

    return _syscall(cv, CS_UNLOCK, &m);
}

int cs_wait(int cond_var_id, int mutex_id)
{
	int cv;
    minix_rs_lookup("cv", &cv);

    message m;
    m.m1_i1 = cond_var_id;
    m.m1_i2 = mutex_id;

    int result;

    if(((result=_syscall(cv, CS_WAIT, &m)) == FAIL) && errno==EINTR)
        return cs_lock(mutex_id)==FAIL ? FAIL : OK;

    return result;
}

int cs_broadcast(int cond_var_id)
{
	int cv;
    minix_rs_lookup("cv", &cv);

    message m;
    m.m1_i1 = cond_var_id;

    return _syscall(cv, CS_BROADCAST, &m);
}