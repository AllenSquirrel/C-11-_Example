// thread6_text.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//多线程执行，实现全局变量，数据读取，发现虽然线程执行顺序乱，但是程序整体无错误
#include "pch.h"
#include <iostream>
#include<string>
#include<thread>
#include<list>
#include<vector>
using namespace std;

vector <int> a = { 1,2,3 };
void myprint(int num)
{
	cout <<"线程id=" << std::this_thread::get_id() <<" "<<a[0]<<a[1]<<a[2]<< endl;
	
	return;
}

int main()
{
	vector <thread> mythreads;//创建线程容器，方便管理
	for (int i = 0; i < 10; i++)//创建10个线程
	{
		mythreads.push_back(thread(myprint, i));//创建并开始执行线程
	}
	//join等待
	for (auto iter = mythreads.begin(); iter != mythreads.end(); iter++)//构造容器迭代器完成主线程等待操作、、、、、、、、、、、重点！！！！！以后常用！
	{
		iter->join();
	}
	cout << "主线程执行结束" << endl;
	return 0;
}