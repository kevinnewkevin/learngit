/*************************************************************************
	> File Name: pool_que.c
	> Author: Chen
	> Mail: baiyccy@126.com 
	> Created Time: Thu 09 Apr 2015 08:38:28 PM CST
 ************************************************************************/

#include"pool_que.h"

void que_init(pque pq, int len)
{
	pq->arr = (elem_t*)calloc(len, sizeof(elem_t));
	if(pq->arr == NULL)
	{
		printf(" calloc error !\n");
		exit(1);
	}
	pq->tail = 0;
	pq->head = 0;
	pq->len = len;
}

int que_empty(pque pq)
{
	if(pq->tail == pq->head)
		return 1;
	else
		return 0;
}

int que_full(pque pq)
{
	if((pq->tail + 1) % pq->len == pq->head)
		return 1;
	else 
		return 0;
}

void que_push(pque pq, elem_t val)
{
	pq->arr[pq->tail] = val;
	pq->tail = (pq->tail + 1) % pq->len;
}

void que_pop(pque pq)
{
	pq->head = (pq->head + 1) % pq->len;
}

elem_t que_top(pque pq)
{
	return pq->arr[pq->head];
}

void que_destory(pque pq)
{
	free(pq->arr);
}


