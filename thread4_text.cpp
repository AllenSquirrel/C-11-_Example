// thread4_text.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<thread>
#include<string>
using namespace std;

class A
{
public:
	int m_i;
	A(int i) :m_i(i)
	{
		cout << "A::A(int)构造函数" << this << "thread id=" << std::this_thread::get_id() << endl;
	}
	A(const A &a) :m_i(a.m_i)
	{
		cout << "A::A(const)拷贝构造函数" << this << "thread id=" << std::this_thread::get_id() << endl;
	}
	~A()
	{
		cout << "A::~A()析构函数" << this << "thread id=" << std::this_thread::get_id() << endl;
	}
	void myprint(int m)//pmybuf为类对象引用来接传进来的参数
	{
		//cout << i << endl;
		cout << this << "thread id=" << std::this_thread::get_id() << endl;
		return;
	}
	void operator()(int n)//定义了调用操作符（）的类对象
	{
		cout << this << "thread id=" << std::this_thread::get_id() << endl;
		return;
	}
};
int main()
{
	//cout << "main_thread id=" << std::this_thread::get_id() << endl;
	/*int myseconder = 12;
	thread mytobj(myprint, A(myseconder));*/
	//mytobj.detach();
	A obj(10);
	//**************************************************

	//thread mytobj(&A::myprint, obj, 15);//此时参数传递，实际上调用拷贝构造函数，即子线程所使用的对象内存地址与主线程中不一致，在主线程就完成拷贝构造，所以即便主线程不等待子线程先执行完也是允许的
	//thread mytobj(&A::myprint, std::ref(obj), 15);//强制说明传递参数为引用，此时不调用拷贝构造函数，子线程的对象内存地址和主线程一致，此时必须使用join，否则主线程先执行完，释放内存，导致子线程还没执行完就丢失对象，发生异常
	
	//**************************************************
	thread mytobj(obj, 15);//operator成员函数
	mytobj.join();
	//mytobj.detach();
	//cout << "Hello World!" << endl;
	return 0;
}