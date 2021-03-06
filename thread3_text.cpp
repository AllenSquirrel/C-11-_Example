// thread3_text.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//线程id获取
//#include "pch.h"
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
		cout << "A::A(int)构造函数" <<this<<"thread id="<<std::this_thread::get_id()<< endl; 
	}
	A(const A &a) :m_i(a.m_i) 
	{ 
		cout << "A::A(const)拷贝构造函数" << this << "thread id=" << std::this_thread::get_id() << endl;
	}
	~A() 
	{ 
		cout << "A::~A()析构函数" << this << "thread id=" << std::this_thread::get_id() << endl;
	}
};
void myprint( const A &pmybuf)//pmybuf为类对象引用来接传进来的参数
{
	//cout << i << endl;
	cout << &pmybuf << "thread id=" << std::this_thread::get_id() << endl;
	return;
}
int main()
{
	cout << "main_thread id=" << std::this_thread::get_id() << endl;
	int myseconder = 12;
	thread mytobj(myprint,A(myseconder));
	//mytobj.detach();
	mytobj.join();
	//cout << "Hello World!" << endl;
	return 0;
}