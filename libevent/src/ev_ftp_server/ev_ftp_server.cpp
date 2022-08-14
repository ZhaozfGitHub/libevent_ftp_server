/*
	2022年6月17日08:32:01
	在libevent实现的线程池基础上搭建的FTP服务器
	作者：赵志凡
*/
#include <event2/event.h>
#include <event2/listener.h>
#include <string.h>
#include "XThreadPool.h"
#ifndef _WIN32
#include <signal.h>
#endif
#include <iostream>
#include "XFtpFactory.h"
using namespace std;
#define SPORT 21

void listen_cb(struct evconnlistener* e, evutil_socket_t s, struct sockaddr* a, int socklen, void* arg)
{
	cout << "listen_cb" << endl;
	XTask* task = XFtpFactory::Get()->CreateTask();
	task->sock = s;
	XThreadPool::Get()->Dispatch(task);
}
int main()
{
#ifdef _WIN32 
	// 初始化socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return 1;
#endif
	// 1、初始化线程池
	XThreadPool::Get()->Init(10);
	cout << "test thread pool!" << endl;
	event_base* base = event_base_new();
	if (base)
	{
		cout << "event_base_new success!" << endl;
	}
	// 2、监听端口
	// socket ，bind，listen 绑定事件
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SPORT);

	evconnlistener* ev = evconnlistener_new_bind(base, listen_cb, base,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 10, (sockaddr*)&sin, sizeof(sin));
	
	if (base)
		event_base_dispatch(base);
	if (ev)
		evconnlistener_free(ev);
	if (base)
		event_base_free(base);
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}
