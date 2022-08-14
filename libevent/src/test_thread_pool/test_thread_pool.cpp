/*
	2022年6月7日20:35:05
	基于libevent搭建的C++跨平台线程池
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
#include "XFtpServerCMD.h"
using namespace std;
#define SPORT 5001	// win:192.168.17.1	  linux:192.168.220.148

void listen_cb(struct evconnlistener* e, evutil_socket_t s, struct sockaddr* a, int socklen, void* arg)
{
	cout << "listen_cb" << endl;
	XTask* task = new XFtpServerCMD();
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
	//忽略管道信号，发送数据给已关闭的socket
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return 1;
#endif
	//1 初始化线程池
	XThreadPool::Get()->Init(10);

	std::cout << "test thread pool!\n";
	//创建libevent的上下文
	event_base* base = event_base_new();
	if (base)
	{
		cout << "event_base_new success!" << endl;
	}
	//监听端口
	//socket ，bind，listen 绑定事件
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SPORT);

	evconnlistener* ev = evconnlistener_new_bind(base, listen_cb, base,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 10, (sockaddr*)&sin, sizeof(sin));
	// 事件分发处理
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
