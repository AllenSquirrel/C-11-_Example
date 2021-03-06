#include "pch.h"
#include <iostream>
#include<string>
#include<list>
#include<thread>
#include<mutex>

#include<future>

using namespace std;



//原子操作：是在多线程中不会被打断的程序执行片段；不需要用到互斥量加锁的多线程编程技术
//【注意】原子操作针对一个变量，而不是代码段，互斥量针对代码段
//std::atomic类模板，用来封装某个类型的值
//**********************************************************************************************************************
//经典原子操作例程：
//
//std::atomic<bool>g_value = false;
//void mythread()//线程入口函数
//{
//	std::chrono::milliseconds dura(1000);
//	while (g_value == false)
//	{
//		cout << "thread_id=" << this_thread::get_id() << "执行中。。。" << endl;
//		cout << g_value << endl;
//		std::this_thread::sleep_for(dura);
//	}
//	cout << "thread_id=" << this_thread::get_id() << "执行结束" << endl;
//	return;
//}
//int main()
//{
//
//	std::thread myobj1(mythread);
//	std::thread myobj2(mythread);
//	std::chrono::milliseconds dura(5000);
//	std::this_thread::sleep_for(dura);
//	g_value = true;//原子操作
//	myobj1.join();
//	myobj2.join();
//	cout << g_value << endl;
//	return 0;
//}

//*********************************************************************************************************************




std::atomic<int>g_value = 0;//g_value由原来简单int到类对象，但使用时为了方便理解依然认为是int
void mythread()//线程入口函数
{
	for (int i = 0; i < 10000000; i++)
	{

		g_value++;//对应原子操作，不会被打断
		//g_value=g_value+1;
		//*****************************************【注意】原子操作对++，--，+=，|=，&=是支持，其他等价形式可能不支持
		//
	}

	return;
}
int main()
{

	std::thread myobj1(mythread);
	std::thread myobj2(mythread);
	myobj1.join();
	myobj2.join();
	cout << g_value << endl;
	return 0;
}







