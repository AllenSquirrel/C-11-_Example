#include "pch.h"
#include <iostream>
#include<string>
#include<list>
#include<thread>
#include<mutex>

#include<future>

using namespace std;

//(1)std::async, std::future创建后台任务并返回值

//std::async,是个函数模板，用来启动一个异步任务（创建一个线程，并执行对应线程入口函数），启动异步任务之后，返回一个std::future对象
//std::future对象来存储线程入口函数返回值（线程返回结果），通过调用std::future对象的成员函数get（）来获取结果                   （注意：该结果不可马上获取，只有当线程执行完毕时，可以取出结果）

//std::launch::deferred,延迟调用（线程入口函数调用延迟到std::future对象的wait或get（）函数调用时才执行，如果不调用get或wait，此线程入口函数将不调用），且根本没有创建新线程，是在主线程中调用的线程入口函数
//std::launch::async,强制在调用async函数时就开始创建新线程
//不加额外参数调用，async（）函数默认参数是std::launch::async|std::launch::deferred，，系统会自行决定是异步运行（创建新任务）还是在主线程里同步运行（不创建新任务），异步任务就运行在执行get（）语句所在线程上；
//std::thread和std::async区别：
//std::thread,如果系统资源紧张，创建线程就会失败，而std::async,有时候不创建线程直接在主线程中运行


//
//std::future<int>result = async(mythread);写法，在系统资源紧张时，会延迟调用，因此，需要判断异步任务到底有没有被推迟执行（std::launch::deferred还是std::launch::async）
//std::future对象的wait_for()







int mythread(int mypar)//线程入口函数
{
	cout << mypar << endl;
	cout << "mythread()开始执行" << "thread_id=" << this_thread::get_id() << endl;
	std::chrono::milliseconds dura(5000);
	std::this_thread::sleep_for(dura);
	cout << "mythread()执行结束" << "thread_id=" << this_thread::get_id() << endl;
	return 5;
}



int main()
{
	int temp = 1;
	cout << "main" << "thread_id=" << this_thread::get_id() << endl;
	std::future<int>result = std::async(mythread,temp);//判断到底是立即创建新线程还是延迟执行
	//cout << "continue...." << endl;
	
	std::future_status status = result.wait_for(std::chrono::seconds(0));
	if (status == std::future_status::deferred)
	{
		cout << "线程延迟执行！系统资源紧张" << endl;
		cout << result.get() << endl;//卡在这里等mythread执行完，仅允许调用一次，不可调用多次::主要是因为get（）函数设计是一个移动语义；
		//result作为future类对象，保存线程返回值，调用get后（移动语义，转移数据）result立即变为空，所以二次调用抛出异常
	}
	else
	{
		if (status == std::future_status::timeout)
		{
			cout << "线程还没执行完，超时等待！" << endl;
			cout << result.get() << endl;
		}
		else if (status == std::future_status::ready)
		{
			cout << "线程执行完毕！" << endl;
			cout << result.get() << endl;
			cout << "成功获取后台线程返回值" << endl;
		}
	}

	return 0;
}