// thread2_text.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//避免detach大坑，线程入口函数参数传递问题
#include "pch.h"
#include <iostream>
#include<thread>
#include<string>
using namespace std;

//void myprint(const int &i, char *p)
//{
//	cout << i << endl;//此处虽然参数为引用，但是实际上i和mi是不同地址，是值传递
//	cout << p << endl;//传递的指针参数，p和a同地址，在使用datach时有可能主线程先结束，导致子线程传入参数为空*************不稳定（1）
//}
//void myprint(const int &i, const string &pmybuf)
//{
//	cout << i << endl;
//	cout << pmybuf.c_str()<< endl;
//}
//int main()
//{
//	int mi = 6;
//	//char a[] = "this is a thread!";
//	//thread mytobj(myprint, mi, a);
//	char mybuf[] = "this is a thread!";
//	//thread mytobj(myprint, mi, mybuf);//事实上，系统在主线程（main）执行完成后才会传递，即mybuf转string**************不稳定（2）
//	//修改如下：
//	thread mytobj(myprint, mi, string(mybuf));//直接将mybuf转换为string对象,在创建线程同时构造临时对象的方法传递参数是可行的***********************稳定
//	mytobj.detach();
//
//	cout << "Hello World!"<<endl; 
//}

//****************************************************************验证********************************************************


class A
{
public:
	int m_i;
	A(int i) :m_i(i) { cout << "A::A(int)构造函数" << endl; }
	A(const A &a) :m_i(a.m_i) { cout << "A::A(const)拷贝构造函数" << endl; }
	~A() { cout << "A::~A()析构函数" << endl; }
};
void myprint(const int &i, const A &pmybuf)//pmybuf为类对象引用来接传进来的参数
{
	//cout << i << endl;
	cout << &pmybuf << endl;
	return;
}
int main()
{
	int mi = 6;
	int myseconder = 12;
	thread mytobj(myprint, mi, A(myseconder));//直接将mybuf转换为string对象
	mytobj.detach();

	//cout << "Hello World!" << endl;
	return 0;
}

//总结：
//（1）如果传递参数为简单类型（int等），建议直接值传递，不要使用引用：
//（2）如果传递参数为类对象，避免隐式类型转换。全部在创建线程这一行就创建出临时对象，然后在函数参数里用引用来接。