#include "mutex.h"

typedef struct Node
{
	int number;
	endpoint_t who;
	struct Node *next;
} Node;

struct Node *Mutex[MAX];


int m_lock(int number, endpoint_t who)
{
	int id=MAX;
	
	for(int i=0; i<MAX; i++)
		if(Mutex[i] && Mutex[i]->number == number)
		{
			id = i;
			break;
		}
		else if(!Mutex[i])
			id = (i<id) ? i : id;

	if(id == MAX || (Mutex[id] && Mutex[id]->who == who))
		return EINVAL;

	struct Node *temp = malloc(sizeof(Node));
	temp->who = who;
	temp->number = number;
	temp->next = NULL;
	
	if(!Mutex[id])
	{
		Mutex[id] = temp;
		return OK;
	}

	struct Node *act = Mutex[id];

	while(act->next)
		act = act->next;

	act->next = temp;

	return EDONTREPLY;
}

int m_unlock(int number, endpoint_t who)
{
	for(int i=0; i<MAX; i++)
	{
		if(!Mutex[i] || Mutex[i]->number != number || Mutex[i]->who !=who)
			continue;

		struct Node *trash = Mutex[i];
		Mutex[i] = trash->next;
		free(trash);

		if(Mutex[i])
		{
			message m;
			m.m_type = OK;
			send(Mutex[i]->who, &m);
		}

		return OK;
	}

	return EPERM;
}

int m_remove(endpoint_t who, int signal)
{
	for(int i=0; i<MAX; i++)
	{
		if(!Mutex[i])
			continue;

		if(!signal && Mutex[i]->who == who)
		{
			struct Node *trash = Mutex[i];
			Mutex[i] = trash->next;
			free(trash);
			
			if(Mutex[i])
			{
				message m;
				m.m_type = OK;
				send(Mutex[i]->who, &m);
			}

			continue;
		}

		struct Node *last = Mutex[i];
		struct Node *act = last->next;

		while(act)
		{
			if(act->who == who)
			{
				last->next = act->next;
				free(act);

				if(signal)
					return OK;
			}

			last = last->next;

			if(!last)
				act = NULL;
			else
				act = last->next;
		}
	}

	if(signal)
		return EINVAL;

	return OK;
}
