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

//std::launch::deferred,延迟调用（线程入口函数调用延迟到std::future对象的wait或get（）函数调用时才执行），且根本没有创建新线程，是在主线程中调用的线程入口函数
//std::launch::async,在调用async函数时就开始创建线程,此参数作为async（）函数默认参数

//(2)std::packaged_task:打包任务，把任务包装起来
//是一个类模板，它的模板参数是各种可调用对象，通过std：：packaged_task把各种可调用对象包装起来，方便将来作为线程入口函数调用



//(3)std::promise,类模板
//能够在某个线程中给它赋值，然后在其他线程中取值










//class A
//{
//public:
//	int mythread(int mypar)//线程入口函数
//	{
//		cout << mypar << endl;
//		cout << "mythread()开始执行" << "thread_id=" << this_thread::get_id() << endl;
//		std::chrono::milliseconds dura(5000);
//		std::this_thread::sleep_for(dura);
//		cout << "mythread()执行结束" << "thread_id=" << this_thread::get_id() << endl;
//		return 5;
//	}
//};


//int mythread(int mypar)//线程入口函数
//{
//	cout << mypar << endl;
//	cout << "mythread()开始执行" << "thread_id=" << this_thread::get_id() << endl;
//	std::chrono::milliseconds dura(5000);
//	std::this_thread::sleep_for(dura);
//	cout << "mythread()执行结束" << "thread_id=" << this_thread::get_id() << endl;
//	return 5;
//}



//创建一个容器：
//vector<std::packaged_task<int(int)>>mytasks;


void mythread(std::promise<int>&tmp, int num)
{
	num++;
	num *= 10;
	cout << "mythread()开始执行" << "thread_id=" << this_thread::get_id() << endl;
	std::chrono::milliseconds dura(5000);
	std::this_thread::sleep_for(dura);
	cout << "mythread()执行结束" << "thread_id=" << this_thread::get_id() << endl;
	int result = num;
	tmp.set_value(result);//创建std::promise类对象引用，在该线程中给其赋值
	return;
}

void mythread2(std::future<int>&tmpf)
{
	cout << tmpf.get() << endl;
	return;
}


int main()
{
	cout << "main" << "thread_id=" << this_thread::get_id() << endl;
	//A a;
	//int temp = 1;
	//cout << "main" << "thread_id=" << this_thread::get_id() << endl;
	//std::future<int>result = std::async(std::launch::async,&A::mythread,&a,temp);//创建一个线程开始后台执行,线程为类成员函数是，用第二个参数是对象引用，才能保证线程里用的是同一个对象，而不是拷贝
	//cout << "continue...." << endl;
	//cout << "continue...." << endl;
	//cout << "continue...." << endl;
	//cout << "continue...." << endl;
	//int def;
	//def = 0;
	//cout << result.get() << endl;//卡在这里等mythread执行完，仅允许调用一次，不可调用多次
	//cout << "成功获取后台线程返回值" << endl;


	//******************************************************************************************************

	//std::packaged_task<int(int)>mypt([](int mypar){
	//	cout << mypar << endl;
	//	cout << "mythread()开始执行" << "thread_id=" << this_thread::get_id() << endl;
	//	std::chrono::milliseconds dura(5000);
	//	std::this_thread::sleep_for(dura);
	//	cout << "mythread()执行结束" << "thread_id=" << this_thread::get_id() << endl;
	//	return 5;
	//});//以lambda表达式形式创建packaged_task对象



	//cout << "main" << "thread_id=" << this_thread::get_id() << endl;
	//std::packaged_task<int(int)>mypt(mythread);//函数mythread通过packaged_task包装
	//std::thread myobj(std::ref(mypt), 1);//线程直接开始执行，第二个参数作为线程入口函数的参数
	//myobj.join();
	//mypt(1);
	//std::future<int>result = mypt.get_future();//std::future对象result通过借助packaged_task类的对象mypt来保存线程入口函数返回值（future类和packaged_task类绑定）
	//cout << result.get() << endl;
	//cout << "成功获取后台线程返回值" << endl;


	//******************************************************************************************************

	//mytasks.push_back(std::move(mypt));//用move移动语义，将mypt对象入容器
	//std::packaged_task<int(int)>mypt2;
	//auto iter = mytasks.begin();//创建迭代器保存容器里第一个对象mypt；
	//mypt2 = std::move(*iter); //用move移动语义，将迭代器指针指向的所保存对象mypt
	//mytasks.erase(iter);//删除第一个元素，迭代器立即失效，后续不可再调用迭代器
	//mypt2(1);
	//std::future<int>result = mypt2.get_future();
	//cout << result.get() << endl;
	//cout << "成功获取后台线程返回值" << endl;




	std::promise<int>myprom;//声明一个promise对象
	std::thread t1(mythread, std::ref(myprom), 1);//第二个参数是对象引用（std::ref）
	t1.join();
	std::future<int>result = myprom.get_future();//std::future对象result通过借助promise类的对象myprom来保存线程入口函数返回值（future类和promise类绑定）


	std::thread t2(mythread2,std::ref(result));//将future类对象引用给了线程2，在线程2里获取值
	t2.join();
	/*cout << result.get() << endl;*/
	cout << "成功获取后台线程返回值" << endl;
	return 0;
}