#include "includes.h"

static int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *UNUSED(info))
{
    return OK;
}
 
static void sef_cb_signal_handler(int signo)
{
}

static void sef_local_startup()
{
    sef_setcb_init_fresh(sef_cb_init_fresh);
    sef_setcb_init_restart(sef_cb_init_fresh);
    sef_setcb_signal_handler(sef_cb_signal_handler);
    sef_startup();
} 

int rm(endpoint_t who, int signal)
{
	int t1 = cv_remove(who);
	int t2 = m_remove(who, signal);

	if(signal && (t1==OK || t2==OK))
		return EINTR;
	
	return EDONTREPLY;
}

int abs(int x)
{
	return ((x>=0)?x:(-1)*x);
}

int main(int argc, char *argv[])
{
	env_setargs(argc, argv);
    sef_local_startup();
    
	while(1)
	{
		int status;
		message m;

		if((status = sef_receive(ANY, &m)) != OK)	
			printf("Message recieve failed: %d.\n", status);

		endpoint_t rec = m.m_source;
		int res = EINVAL;

		if(m.m_type == CS_LOCK)
			res = m_lock(m.m1_i1, m.m_source);
		else if(m.m_type == CS_UNLOCK)
			res = m_unlock(m.m1_i1, m.m_source);
		else if(m.m_type == CS_WAIT)
			res = cv_wait(m.m1_i1, m.m1_i2, m.m_source);
		else if(m.m_type == CS_BROADCAST)
			res = cv_broadcast(m.m1_i1);
		else if(m.m_type == CS_SREMOVE)
		{
			rec = m.PM_PROC;
			res = rm(m.PM_PROC, 1);
		}
		else if(m.m_type == CS_PREMOVE)
			res = rm(m.PM_PROC, 0);


		if(abs(res)!=abs(EDONTREPLY))
		{
			message msg;
			msg.m_type = abs(res)*(-1);
			send(rec, &msg);
		}
	}
}
