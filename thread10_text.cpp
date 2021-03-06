

#include "pch.h"
#include <iostream>
#include<string>
#include<list>
#include<thread>
#include<mutex>
using namespace std;

//创建一个游戏服务器类，包含两个线程（1）接受用户发送来的命令请求，存入list（类似于vector，顺序插入/删除）  
								  //（2）获取用户请求，命令解析执行




//********************************************   unique_lock使用    *****************************************
//unique_lock类模板类似lock_guard
//unique_lock成员函数：
//(1)lock,实际上自己可以不用手动加unlock，但加了之后就有点画蛇添足，但不会报错
//(2)unlock
//(3)try_lock();bool类型，是否解锁ture拿到锁，or false没有拿到锁；
//(4)release(),返回她所管理的mutex对象指针，并释放所有权，也就是说，这个unique_lock和mutex不再有联系



class game
{
private:
	list<int>msgrecqueue;
	mutex mymutex1;
	//mutex mymutex2;
public:
	void inmsgrecqueue()
	{
		for (int i = 0; i < 100000; i++)
		{
			cout << "命令写入" << i << endl;
			//mymutex1.lock();
			//mymutex2.lock();

			//lock(mymutex1, mymutex2);//调用lock类同时锁住两个互斥量

			//lock_guard<mutex>sbguard1(mymutex1, adopt_lock);//避免忘记unlock，同时参数加入adopt_lock解除lock_guard中构造函数lock功能
			unique_lock<mutex>sbguard1(mymutex1, try_to_lock);//************* try_to_lock目的是尝试解锁
			if (sbguard1.owns_lock())//判断是否有拿到锁
			{
				msgrecqueue.push_back(i);
			}
			else
			{
				cout << "正在等待lock...，此时执行其他任务命令" << i << endl;
			}
			//lock_guard<mutex>sbguard2(mymutex2,adopt_lock);
			//msgrecqueue.push_back(i);
			//mymutex1.unlock();
			//mymutex2.unlock();
		}
		return;
	}
	bool outmsgproc(int &command)
	{
		//lock_guard<mutex> sbguard(mymutex);//lock_guard为类模板，sbguard为类对象，以互斥量作为参数，在该类构造函数里执行lock，在析构函数里执行unlock，通过加大括号（作用域）提前实现unlock
		//mymutex1.lock();
		//mymutex2.lock();
		//lock(mymutex1, mymutex2);//调用lock类同时锁住两个互斥量
		
		unique_lock<mutex>sbguard1(mymutex1);
		chrono::milliseconds dts(2000);
		this_thread::sleep_for(dts);


		//lock_guard<mutex>sbguard1(mymutex1, adopt_lock);//避免忘记unlock，同时参数加入adopt_lock解除lock_guard构造函数中lock功能
		//lock_guard<mutex>sbguard2(mymutex2, adopt_lock);
		if (!msgrecqueue.empty())
		{
			int command = msgrecqueue.front();//返回第一个元素，因为有empty判断所以不检查元素是否存在
			msgrecqueue.pop_front();//根据list特性，取出第一个元素后，移除第一个元素，但不返回
			//mymutex1.unlock();
			//mymutex2.unlock();
			//mymutex.unlock();//***************************************************************条件选择分支结构要注意每一分支结构都要有对应的lock和unlock
			return true;
		}
		//mymutex1.unlock();
		//mymutex2.unlock();
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

