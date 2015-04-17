/*************************************************************************
	> File Name: pool_threads.h
	> Author: Chen
	> Mail: baiyccy@126.com 
	> Created Time: Thu 09 Apr 2015 07:51:53 PM CST
 ************************************************************************/

#ifndef __THREADS_H__
#define __THREADS_H__
#include"pool_common.h"
#include"pool_que.h"
typedef	void* (*handler)(void*); 

typedef struct {
	pthread_mutex_t lock;
	pthread_cond_t pro;
	pthread_cond_t con;
	que myque;
	pthread_t *arr;
	handler myhd;
}thd, *pthd;

void th_init(pthd pd, int quenum, int thdnum, handler hd, void* arg);

elem_t th_pop(pthd pd);

void th_push(pthd pd, elem_t val);

void th_destory(pthd pd, int thdnum);
#endif 
