/*************************************************************************
	> File Name: pool_que.h
	> Author: Chen
	> Mail: baiyccy@126.com 
	> Created Time: Thu 09 Apr 2015 02:07:14 PM CST
 ************************************************************************/

#ifndef __QUE_H__
#define __QUE_H__
#include"pool_common.h"
typedef struct {
	int head;
	int tail;
	int len;
	elem_t *arr;
}que, *pque;

void que_init(pque pq, int len);

void que_push(pque pq, elem_t val);

void que_pop(pque pq);

elem_t que_top(pque pq);

int que_empty(pque pq);

int que_full(pque pq);

void que_destory(pque pq);

#endif
