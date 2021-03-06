#include "pch.h"
#include <iostream>
#include<string>
#include<list>
#include<thread>
#include<mutex>

#include<future>

using namespace std;

//(1)future成员函数：
//future_status::  ready等待
//                 timeout超时
//                 deferred延迟

//(2)shared_future类模板，get（）函数复制数据






//int mythread(int mypar)//线程入口函数
//{
//	cout << mypar << endl;
//	cout << "mythread()开始执行" << "thread_id=" << this_thread::get_id() << endl;
//	std::chrono::milliseconds dura(5000);
//	std::this_thread::sleep_for(dura);
//	cout << "mythread()执行结束" << "thread_id=" << this_thread::get_id() << endl;
//	return 5;
//}
//
//
//
//
//
//int main()
//{
//
//	//int temp = 1;
//	//cout << "main" << "thread_id=" << this_thread::get_id() << endl;
//	//std::future<int>result = std::async(std::launch::deferred,mythread,temp);//std::launch::deferred存在，延迟线程执行，但线程并不会卡在这一行，而是后面调用get（）后卡住
//	//cout << "continue...." << endl;
//	//
//	//std::future_status status = result.wait_for(std::chrono::seconds(6));//希望等待1秒，线程返回
//	//if (status == std::future_status::timeout)
//	//{
//	//	cout << "线程还没执行完，超时！" << endl;
//	//	cout << "获取后台线程返回值失败" << endl;
//	//}
//	//else if (status == std::future_status::ready)
//	//{
//	//	cout << "线程执行完毕！" << endl;
//	//	cout << result.get() << endl;
//	//	cout << "成功获取后台线程返回值" << endl;
//	//}
//	//else if (status == std::future_status::deferred)
//	//{
//	//	cout << "线程延迟执行！" << endl;
//	//	cout << result.get() << endl;//卡在这里等mythread执行完，仅允许调用一次，不可调用多次::主要是因为get（）函数设计是一个移动语义；
//	//	//result作为future类对象，保存线程返回值，调用get后（移动语义，转移数据）result立即变为空，所以二次调用抛出异常
//	//	cout << "成功获取后台线程返回值" << endl;
//	//}
//
//	//**************************************************************************************************
//
//
//	
//
//
//	cout << "main" << "thread_id=" << this_thread::get_id() << endl;
//	std::packaged_task<int(int)>mypt(mythread);//函数mythread通过packaged_task包装
//	std::thread myobj(std::ref(mypt), 1);//线程直接开始执行，第二个参数作为线程入口函数的参数
//	myobj.join();
//	std::future<int>result = mypt.get_future();//std::future对象result通过借助packaged_task类的对象mypt来保存线程入口函数返回值（future类和packaged_task类绑定）
//	bool ifcan = result.valid();
//	std::shared_future<int>result_s(std::move(result));
//	ifcan = result.valid();
//	
//	cout << result_s.get() << endl;//shared_future可以get多次
//	cout << result_s.get() << endl;
//
//
//
//	
//
//	return 0;
//}




//(3)原子操作std::atomic
//有两个线程：一个线程读，一个线程写(互斥量)
//如下：
std::mutex m_g;
int g_value = 0;
void mythread()//线程入口函数
{
	for (int i = 0; i < 10000000; i++)
	{
		m_g.lock();
		g_value++;
		m_g.unlock();
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




//std::atomic<int>g_value = 0;//g_value由原来简单int到类对象，但使用时为了方便理解依然认为是int
//void mythread()//线程入口函数
//{
//	for (int i = 0; i < 10000000; i++)
//	{
//
//		g_value++;
//
//	}
//
//	return;
//}
//int main()
//{
//
//	std::thread myobj1(mythread);
//	std::thread myobj2(mythread);
//	myobj1.join();
//	myobj2.join();
//	cout << g_value << endl;
//	return 0;
//}







