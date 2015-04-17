/*************************************************************************
	> File Name: pool_threads.c
	> Author: Chen
	> Mail: baiyccy@126.com 
	> Created Time: Thu 09 Apr 2015 10:06:07 PM CST
 ************************************************************************/

#include "pool_threads.h"

void th_init(pthd pd, int quenum, int thdnum, handler hd, void* arg)
{
	int i;
	que_init(&pd->myque, quenum);

	pd->arr = (pthread_t*)calloc(thdnum, sizeof(pthread_t));
	for(i = 0 ; i < thdnum; i++)
	{
		pthread_create(&pd->arr[i], NULL, hd, arg);
	}
	pd->myhd = hd;
	
	pthread_mutex_init(&pd->lock, NULL);

	pthread_cond_init(&pd->pro, NULL);
	pthread_cond_init(&pd->con, NULL);
}

elem_t th_pop(pthd pd)
{
	elem_t val;
	pthread_mutex_lock(&pd->lock);
	while(que_empty(&pd->myque))
	{
		pthread_cond_signal(&pd->pro);
		pthread_cond_wait(&pd->con, &pd->lock);
	}
	val = que_top(&pd->myque);
	que_pop(&pd->myque);
	pthread_mutex_unlock(&pd->lock);
	return val;
}

void th_push(pthd pd, elem_t val)
{
	pthread_mutex_lock(&pd->lock);
	while(que_full(&pd->myque))
	{
		pthread_cond_signal(&pd->con);
		pthread_cond_wait(&pd->pro, &pd->lock);
	}
	que_push(&pd->myque, val);
	pthread_cond_signal(&pd->con);
	pthread_mutex_unlock(&pd->lock);
}

void th_destory(pthd pd, int thdnum)
{
	int i;
	que_destory(&pd->myque);
	for(i = 0; i < thdnum; i++)
	{
		pthread_join(pd->arr[i], NULL);
	}
	pthread_mutex_destroy(&pd->lock);
	pthread_cond_destroy(&pd->pro);
	pthread_cond_destroy(&pd->con);
}

