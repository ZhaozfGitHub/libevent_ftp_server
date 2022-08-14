#pragma once
#include <vector>   // ����ֱ�ӷ����±꣬������ǿ
class XThread;
class XTask;

// �õ���ģʽ
class XThreadPool
{
public:
	static XThreadPool* Get()
	{
		static XThreadPool p;
		return &p;
	}
	// ��ʼ�������̲߳������߳�
	void Init(int threadCount);

	// �ַ��߳�
	void Dispatch(XTask* task);
private:
	// �߳�����
	int threadCount = 0;
	int lastThread = -1;
	// �̳߳��߳�
	std::vector<XThread*> threads;
	XThreadPool() {};
	~XThreadPool() {};
};

