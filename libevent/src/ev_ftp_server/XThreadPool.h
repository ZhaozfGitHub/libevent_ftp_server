#pragma once
#include <vector>   // 可以直接访问下标，比链表强
class XThread;
class XTask;

// 用单例模式
class XThreadPool
{
public:
	static XThreadPool* Get()
	{
		static XThreadPool p;
		return &p;
	}
	// 初始化所有线程并启动线程
	void Init(int threadCount);

	// 分发线程
	void Dispatch(XTask* task);
private:
	// 线程数量
	int threadCount = 0;
	int lastThread = -1;
	// 线程池线程
	std::vector<XThread*> threads;
	XThreadPool() {};
	~XThreadPool() {};
};

