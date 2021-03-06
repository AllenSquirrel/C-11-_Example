

#include "pch.h"
#include <iostream>
#include<string>
#include<list>
#include<thread>
#include<mutex>
using namespace std;

//创建一个游戏服务器类，包含两个线程（1）接受用户发送来的命令请求，存入list（类似于vector，顺序插入/删除）  
								  //（2）获取用户请求，命令解析执行




//********************************************死锁解决方案************************要求两把锁（两个互斥量）在各个线程中调用顺序保持一致即可解决

//std::lock(互斥量1，互斥量2，，，，)同时锁住多个互斥量，如果有一个没锁住，则释放锁住那一个，导致最终结果为，都没锁住，要么多个互斥量都锁住********不常用！！！

class game
{
private:
	list<int>msgrecqueue;
	mutex mymutex1;
	mutex mymutex2;
public:
	void inmsgrecqueue()
	{
		for (int i = 0; i < 100000; i++)
		{
			cout << "命令写入" << i << endl;
			//mymutex1.lock();
			//mymutex2.lock();

			//*******************************************************************结合使用******************8
			lock(mymutex1, mymutex2);//调用lock类同时锁住两个互斥量

			lock_guard<mutex>sbguard1(mymutex1, adopt_lock);//避免忘记unlock，同时参数加入adopt_lock解除lock_guard中构造函数lock功能
			lock_guard<mutex>sbguard2(mymutex2, adopt_lock);
			msgrecqueue.push_back(i);
			//mymutex1.unlock();
			//mymutex2.unlock();
		}
		return;
	}
	bool outmsgproc(int &command)
	{
		//lock_guard<mutex> sbguard(mymutex);//lock_guard为类模板，sbguard为类对象，以互斥量作为参数，在该类构造函数里执行lock，在析构函数里执行unlock，通过加大括号（作用域）提前实现unlock
		mymutex1.lock();
		mymutex2.lock();
		//lock(mymutex1, mymutex2);//调用lock类同时锁住两个互斥量

		//lock_guard<mutex>sbguard1(mymutex1, adopt_lock);//避免忘记unlock，同时参数加入adopt_lock解除lock_guard构造函数中lock功能
		//lock_guard<mutex>sbguard2(mymutex2, adopt_lock);
		if (!msgrecqueue.empty())
		{
			int command = msgrecqueue.front();//返回第一个元素，因为有empty判断所以不检查元素是否存在
			msgrecqueue.pop_front();//根据list特性，取出第一个元素后，移除第一个元素，但不返回
			mymutex1.unlock();
			mymutex2.unlock();
			//mymutex.unlock();//***************************************************************条件选择分支结构要注意每一分支结构都要有对应的lock和unlock
			return true;
		}
		mymutex1.unlock();
		mymutex2.unlock();
		//mymutex.unlock();//*******************************************************************
		return false;
	}
	void outmsgrecqueue()
	{
		int command = 0;
		for (int i = 0; i < 100000; i++)
		{
			bool result = outmsgproc(command);
			if (result == true)
			{
				cout << "命令解析" << command << endl;
			}

		}
		return;
	}
};


int main()
{
	game gobj;
	thread myoutgobj(&game::outmsgrecqueue, &gobj);
	thread myingobj(&game::inmsgrecqueue, &gobj);
	myoutgobj.join();
	myingobj.join();

	return 0;
}

