// thread7_text.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//加入互斥量实现读写,保护共享数据
//互斥量（是一个类对象）【mutex】，使用lock（）上锁，操作共享数据，unlock（）解锁，
//互斥量的使用要注意，保护数据过多，影响效率，过少起不到保护效果
//lock()与unlock()个数一定要匹配使用，对称存在
#include "pch.h"
#include <iostream>
#include<string>
#include<list>
#include<thread>
#include<mutex>
using namespace std;

//创建一个游戏服务器类，包含两个线程（1）接受用户发送来的命令请求，存入list（类似于vector，顺序插入/删除）  
								  //（2）获取用户请求，命令解析执行




//********************************************使用lock_guard类模板代替lock和unlock************************

class game
{
private:
	list<int>msgrecqueue;
	mutex mymutex;
public:
	void inmsgrecqueue()
	{
		for (int i = 0; i < 100000; i++)
		{
			cout << "命令写入" << i << endl;
			//lock_guard<mutex> sbguard(mymutex);
			mymutex.lock();
			msgrecqueue.push_back(i);
			mymutex.unlock();
		}
		return;
	}
	bool outmsgproc(int &command)
	{
		lock_guard<mutex> sbguard(mymutex);//lock_guard为类模板，sbguard为类对象，以互斥量作为参数，在该类构造函数里执行lock，在析构函数里执行unlock，通过加大括号（作用域）提前实现unlock
		//mymutex.lock();
		if (!msgrecqueue.empty())
		{
			int command = msgrecqueue.front();//返回第一个元素，因为有empty判断所以不检查元素是否存在
			msgrecqueue.pop_front();//根据list特性，取出第一个元素后，移除第一个元素，但不返回
			//mymutex.unlock();//***************************************************************条件选择分支结构要注意每一分支结构都要有对应的lock和unlock
			return true;
		}
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
				cout << "命令解析" << command<< endl;
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

