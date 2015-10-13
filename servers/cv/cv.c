#include "includes.h"

typedef struct cv
{
	int id;
	endpoint_t who;
	int mutex;
	struct cv *next;
} cv;

struct cv *CV[MAX];


int cv_wait(int cond_var_id, int mutex_id, endpoint_t who)
{
	if(m_unlock(mutex_id, who) == EPERM)
		return EINVAL;
	
	int id = MAX;

	struct cv *temp = malloc(sizeof(cv));
	temp->id = cond_var_id;
	temp->mutex = mutex_id;
	temp->who = who;
	temp->next = NULL;



	for(int i=0; i<MAX; i++)
		if(CV[i] && CV[i]->id == temp->id)
		{
			struct cv *act = CV[i];

			while(act->next)
				act = act->next;

			act->next = temp;

			return EDONTREPLY;
		}
		else if(!CV[i])
			id = (i<id) ? i : id;

	CV[id] = temp;
	return EDONTREPLY;
}

int cv_broadcast(int cond_var_id)
{
	for(int i=0; i<MAX; i++)
	{
		if(!CV[i] || CV[i]->id!=cond_var_id)
			continue;

		while(CV[i])
		{
			if(m_lock(CV[i]->mutex, CV[i]->who) == OK)
			{
				message m;
				m.m_type = OK;
				send(CV[i]->who, &m);
			}

			struct cv *trash = CV[i];
			CV[i]=trash->next;
			free(trash);
		}
		
		return OK;
	}

	return EINVAL;
}

int cv_remove(endpoint_t who)
{
	for(int i=0; i<MAX; i++)
	{
		if(!CV[i])
			continue;

		if(CV[i]->who == who)
		{
			struct cv *trash = CV[i];
			CV[i] = trash->next;
			free(trash);

			return OK;
		}

		struct cv *last = CV[i];
		struct cv *act = last->next;

		while(act)
		{
			if(act->who == who)
			{
				last->next = act->next;
				free(act);
				return OK;
			}

			last = act;
			act = act->next;
		}
	}

	return EINVAL;
}
