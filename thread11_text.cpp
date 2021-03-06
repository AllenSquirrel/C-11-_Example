#include "pch.h"
#include <iostream>
#include<string>
#include<list>
#include<thread>
#include<mutex>
using namespace std;

//设计模式：根据项目开发经验，模块划分结构，程序灵活，晦涩难读，可读性和移植性差

//单例设计模式：整个项目，有某个或某些特殊类，属于该类的对象，只创建一个



//引入互斥量，实现访问共享
std::mutex resource_mutex;

//系统定义标记once_flag
std::once_flag myflag;

//单例类：
class MyCAS 
{
private:
	MyCAS() {}//私有化构造函数，此时在mian函数直接创建该类对象报错异常，那该怎么办（此时如何创建）？？？
	static MyCAS *m_instance;//私有成员变量：：静态成员变量
public:
	static void creatinstance()//只调用1次
	{
		std::chrono::milliseconds dura(20000);
		std::this_thread::sleep_for(dura);
		cout << "creatinstance被执行" << endl;


		m_instance = new MyCAS;
    	static clears cl;
	}
	static MyCAS *get_instance()//公有成员函数，可调用私有成员变量创建对象
	{
		//if (m_instance == NULL)//双重锁定，双重检查，                提高效率：：只有在首次初始化的时候会进入该条件，后续不会进入，也就意味着后续也不会调用互斥量加锁
		//	//1.如果if (m_instance ！= NULL)条件成立，表示m_instance已经被new过
		//	//2.如果if (m_instance == NULL)条件成立，不代表m_instance一定没被new过，在多线程中，线程1通过条件判断正准备new的时候发生上下文切换，线程2也可以通过条件判断，进而new一下，再切回来线程1再new一下
		//{
		//	//*********************************************

		//	std::unique_lock<std::mutex>mymutex(resource_mutex);//自动加锁，出互斥量作用域自动解锁

		//	//*********************************************
		//	if (m_instance == NULL)
		//	{
		//		m_instance = new MyCAS;

		//		static clears cl;
		//	}
		//}

		call_once(myflag, creatinstance);//两个线程同时执行到这里，其中一个线程要等另外一个线程执行完毕creatinstance
		cout << "call_once执行完毕" << endl;
		return m_instance;
	}
	void function()
	{
		cout << "测试" << endl;
	}
	class clears //类中套类，在类的析构函数里用delete释放对象
	{
	public:
		~clears()
		{
			if (MyCAS::m_instance)
			{
				delete MyCAS::m_instance;
				MyCAS::m_instance = NULL;

			}
		}
	};
};
//需要对静态成员变量初始化：
MyCAS *MyCAS::m_instance = NULL;

void mythread()
{
	cout << "我的线程开始执行。。。" << endl;
	MyCAS *p1 = MyCAS::get_instance();
	cout << "我的线程执行结束。。。" << endl;
}

int main()
{
	//MyCAS a;
	//单例设计模式：：
	//***********************************************************************************************************************
	MyCAS *p1 = MyCAS::get_instance();//创建一个对象，返回该类对象的指针
	MyCAS *p2 = MyCAS::get_instance();//结果，内存地址查看发现，p1，p2为同一个对象的指针
	p1->function();
	p2->function();
	MyCAS::get_instance()->function();//直接通过get_instance返回创建的类对象调用成员函数，将该装载的数据装载，不再通过指向对象的指针来调用成员函数
	//***********************************************************************************************************************




	//单例设计模式共享数据问题分析，解决：：
	//面临问题：需要我们自己创建的线程（非主线程）中创建MyCAS类，这种线程不止一个，我们可能面临get_instance函数互斥问题
	//解决如下：
	//if if(m_instance == NULL)//双重锁定，双重检查，                提高效率：：只有在首次初始化的时候会进入该条件，后续不会进入，也就意味着后续也不会调用互斥量加锁
	//		//1.如果if (m_instance ！= NULL)条件成立，表示m_instance已经被new过
	//		//2.如果if (m_instance == NULL)条件成立，不代表m_instance一定没被new过，在多线程中，线程1通过条件判断正准备new的时候发生上下文切换，线程2也可以通过条件判断，进而new一下，再切回来线程1再new一下
	//{
	//	//*********************************************

	//	std::unique_lock<std::mutex>mymutex(resource_mutex);//自动加锁，出互斥量作用域自动解锁

	//	//*********************************************
	//	if (m_instance == NULL)
	//	{
	//		m_instance = new MyCAS;

	//		static clears cl;
	//	}
	//}

	std::thread myobj1(mythread);
	std::thread myobj2(mythread);
	myobj1.join();
	myobj2.join();


	//std::call_once()   ::c++引入函数，该函数第二个参数为一个函数a（）；
	//call_once()功能是保证函数a（）只调用一次
	//call_once()具备互斥量这种能力，而且效率上，比互斥量消耗的资源更少；
	//call_once()需要与一个标记结合使用，std::once_flag(一个结构)，通过标记来决定对应函数a（）是否执行，
	//调用call_once（）成功后，call_once把这个标记设置为一种已调用状态，后续再次调用call_once（），只要once_flag被设置为“已调用”状态，那么对应函数a（）不在被执行
	return 0;
}