

#include "pch.h"
#include <iostream>
#include<string>
#include<list>
#include<thread>
#include<mutex>
#include"windows.h"

using namespace std;

//创建一个游戏服务器类，包含两个线程（1）接受用户发送来的命令请求，存入list（类似于vector，顺序插入/删除）  
								  //（2）获取用户请求，命令解析执行


//*************************************************************************************************************8


//#define __WINDOWSJQ_  //宏定义一个临界区开关


//此类用于windows下自动释放临界区，（类似于std::lock_guard<mutex>功能），防止忘记LeaveCriticalSection导致死锁的情况发生
class cwinlock
{
public:
	cwinlock(CRITICAL_SECTION *m_pcrtmp)
	{
		m_pcrtical = m_pcrtmp;
		EnterCriticalSection(m_pcrtical);
	}
	~cwinlock()
	{
		LeaveCriticalSection(m_pcrtical);
	}
private:
	CRITICAL_SECTION *m_pcrtical;
};


class game
{
private:
	list<int>msgrecqueue;
	//mutex mymutex;
	recursive_mutex mymutex;
	//*********************************************

#ifdef  __WINDOWSJQ_
		CRITICAL_SECTION my_winsec;//windows的临界区，类似于c++标准里的mutex互斥量
#endif




public:

	//*********************************************

	game()
	{
		#ifdef  __WINDOWSJQ_
		InitializeCriticalSection(&my_winsec);//不同于mutex，mutex对象拿过来在直接用，而windows临界区需要先初始化
		#endif

	}

	void inmsgrecqueue()
	{
		/*std::chrono::milliseconds dura(1500);
		std::this_thread::sleep_for(dura);*/
		for (int i = 0; i < 100000; i++)
		{
			cout << "玩家命令写入" << i << endl;
			//lock_guard<mutex> sbguard(mymutex);
#ifdef  __WINDOWSJQ_

			cwinlock w1(&my_winsec);
			//EnterCriticalSection(&my_winsec);//类似获取锁
			msgrecqueue.push_back(i);
			//LeaveCriticalSection(&my_winsec);//类似解锁
#else
			//mymutex.lock();
			lock_guard<recursive_mutex> sbguard(mymutex);
			//fun2();//加三次锁
			msgrecqueue.push_back(i);
			//mymutex.unlock();

#endif //  __WINDOWSJQ_
		}
		return;
	}
	bool outmsgproc(int &command)
	{
#ifdef __WINDOWSJQ_
		//winlock w2(&my_winsec);
		EnterCriticalSection(&my_winsec);//类似获取锁
		if (!msgrecqueue.empty())
		{
			int command = msgrecqueue.front();//返回第一个元素，因为有empty判断所以不检查元素是否存在
			msgrecqueue.pop_front();//根据list特性，取出第一个元素后，移除第一个元素，但不返回
			LeaveCriticalSection(&my_winsec);//类似解锁
			//mymutex.unlock();//***************************************************************条件选择分支结构要注意每一分支结构都要有对应的lock和unlock
			return true;
		}
		LeaveCriticalSection(&my_winsec);//类似解锁
#else

		//lock_guard<mutex> sbguard(mymutex);//lock_guard为类模板，sbguard为类对象，以互斥量作为参数，在该类构造函数里执行lock，在析构函数里执行unlock，通过加大括号（作用域）提前实现unlock
		mymutex.lock();
		if (!msgrecqueue.empty())
		{
			int command = msgrecqueue.front();//返回第一个元素，因为有empty判断所以不检查元素是否存在
			msgrecqueue.pop_front();//根据list特性，取出第一个元素后，移除第一个元素，但不返回
			mymutex.unlock();//***************************************************************条件选择分支结构要注意每一分支结构都要有对应的lock和unlock
			return true;
		}
		mymutex.unlock();//*******************************************************************
		return false;
#endif // __WINDOWSJQ_
		
	}
	void outmsgrecqueue()
	{
		int command = 0;
		for (int i = 0; i < 100000; i++)
		{
			bool result = outmsgproc(command);
			if (result == true)
			{
				cout << "			取出命令，命令解析" << command << endl;
			}
			else
			{
				cout << "取出命令为空，无法完成命令解析" << i << endl;
			}
		}
		return;
	}
	//void fun1()
	//{
	//	lock_guard<recursive_mutex> sbguard1(mymutex);
	//	//...........
	//	fun2();
	//}
	//void fun2()
	//{
	//	lock_guard<recursive_mutex> sbguard2(mymutex);
	//	//............
	//}
};


int main()
{
	//(一)windows临界区：(等价于mutex)
//#ifdef  __WINDOWSJQ_
//
//	EnterCriticalSection(&my_winsec);//类似获取锁
//	msgrecqueue.push_back(i);
//	LeaveCriticalSection(&my_winsec);//类似解锁
//#else
//	mymutex.lock();
//	msgrecqueue.push_back(i);
//	mymutex.unlock();
//
//#endif // 




	//(二)多次进入临界区实验
	//在”同一个线程“（不同线程就会卡住等待）可以多次进入临界区且不会等待，windows中“相同临界区变量”代表的临界区的进入EnterCriticalSection可以被多次调用
	//调用EnterCriticalSection几次，就得调用离开临界区LeaveCriticalSection几次
	//而在c++中，不允许同一个线程中lock同一个互斥量多次，否则报异常


	//(三)自动析构技术
	//c++中引入std::lock_guard<mutex> sbguard(my_mutex);
	//windows下类似功能实现：
	//此类用于windows下自动释放临界区，（类似于std::lock_guard<mutex>功能），防止忘记LeaveCriticalSection导致死锁的情况发生
	/*class cwinlock
	{
	public:
		cwinlock(CRITICAL_SECTION *m_pcrtmp)
		{
			m_pcrtical = m_pcrtmp;
			EnterCriticalSection(m_pcrtical);
		}
		~cwinlock()
		{
			LeaveCriticalSection(m_pcrtical);
		}
	private:
		CRITICAL_SECTION *m_pcrtical;
	};*/






	//(四)recursive_mutex递归的独占互斥量：允许同一个线程里同一互斥量多次被lock



	//(五)std::timed_mutex:带超时功能的独占互斥量：
	//try_lock_for()::等待一段时间，如果拿到了锁，或等待超时没拿到锁，继续走下来，不卡住等待
	//try_lock_until()::参数是一个未来时间点，在这个未来的时间没到的时间内，如果拿到了锁，那么就走下来，如果时间到了，没拿到锁，仍然走下来


	//std::timed_recursive_mutex带超时功能的递归式独占互斥量



	game gobj;
	thread myoutgobj(&game::outmsgrecqueue, &gobj);
	thread myingobj(&game::inmsgrecqueue, &gobj);
	myoutgobj.join();
	myingobj.join();

	return 0;
}

