// thread1_text.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//创建线程，熟悉join和deatch用法
#include "pch.h"
#include <iostream>

#include<thread>

using namespace std;


void myprint()
{
	cout << "线程开始。。。" << endl;
	//...
	//
	//...
	cout << "线程结束1。。。" << endl;
	cout << "线程结束2。。。" << endl;
	cout << "线程结束3。。。" << endl;
	cout << "线程结束4。。。" << endl;
	cout << "线程结束5。。。" << endl;
	cout << "线程结束6。。。" << endl;
	cout << "线程结束7。。。" << endl;
	cout << "线程结束8。。。" << endl;
}

class TA
{
	int m_i;
public:
	TA(int i) :m_i(i) { cout << "构造函数" << endl; }
	TA(const TA &ta) :m_i(ta.m_i) { cout << "拷贝构造函数" << endl; }
	~TA() { cout << "析构函数" << endl; }
	//TA(int &i) :m_i(i) {}
	void operator()()
	{
		/*cout << "线程开始，，，"<<endl;


		cout << "线程结束，，，" << endl;*/
		cout << "m_i1值为：" << m_i << endl;
		cout << "m_i2值为：" << m_i << endl;
		cout << "m_i3值为：" << m_i << endl;
		cout << "m_i4值为：" << m_i << endl;
		cout << "m_i5值为：" << m_i << endl;
		cout << "m_i6值为：" << m_i << endl;


	}

};
int main()
{
	int mi=6;
	//thread mytobj(myprint);//实参为可调用对象：函数
	//mytobj.join();
	TA ta(mi);
	thread mytobj(ta);//实参为可调用对象：类
	//auto mylamthread = [] {
	//	cout<<"1"<<endl;
	//	cout << "2" << endl;
	//	cout << "3" << endl;
	//	cout << "4" << endl;
	//};
	//thread mytobj(mylamthread);
	if (mytobj.joinable())
	{
		//mytobj.join();
		mytobj.detach();
	}
	//mytobj.detach();
	cout << "主线程" << endl;
	cout << "主线程" << endl;
	cout << "主线程" << endl;
	cout << "主线程" << endl;
	return 0;
}

