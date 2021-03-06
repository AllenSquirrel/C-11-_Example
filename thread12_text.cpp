#include "pch.h"
#include <iostream>
#include<string>
#include<list>
#include<thread>
#include<mutex>

using namespace std;


// 线程A：等待一个条件满足（等待消息队列中要处理的消息）
//线程B：专门往消息队列中扔消息（数据）
//条件变量的作用：线程B触发线程A，使得线程A继续往下执行
//条件变量std::condition_variable实际上是一个类，是一个和条件相关的类，说白了就是等待一个条件达成，这个类需要和互斥量配合工作，用的时候我们要生成该类对象

//条件变量std::condition_variable这是一个类；
//wait   ::void wait(unique_lock<mutex>& _Lck, _Predicate _Pred)第一个参数是互斥量对象锁，
//第二个参数是lambda表达式返回true，wait()将直接返回
//第二个参数是lambda表达式返回false，wait()将解锁互斥量，并堵塞到本行，直至其他某个线程调用notify_one（）成员函数为止
//       ::void wait(unique_lock<mutex>& _Lck)无第二个参数，wait（)将解锁互斥量，并堵塞到本行，直至其他某个线程调用notify_one（）成员函数为止
//notify_one唤醒wait()后尽快解开该进程的互斥锁，此时另外一线程wait（）不断尝试重新获取互斥锁，如果获取失败，继续堵塞；获取成功（wait所在线程上锁），继续通过第二个参数判断（是否再次堵塞or向下执行），若第二个参数不存在，无条件唤醒（相当于判断默认为true，向下执行）

//存在缺陷：
//        1> notify_one唤醒wait()后解锁后又迅速再次获取锁，此时另外一线程wait（）不断尝试重新获取互斥锁，获取多次失败，意味着此时已经消息队列已经写入多条数据，而我们wait获取一次成功，只能取出一次数据
//        2>假如outmsgrecqueue（）正在处理一个任务（玩家命令解析任务），此过程需要一段时间，而提前使用unique_lock的unlock解锁，使得写入线程获取互斥锁，此时执行notify_one唤醒wait()将无效






//notify_all





//创建一个游戏服务器类，包含两个线程（1）接受用户发送来的命令请求，存入list（类似于vector，顺序插入/删除）  
								  //（2）获取用户请求，命令解析执行




//********************************************使用lock_guard类模板代替lock和unlock************************

class game
{
private:
	list<int>msgrecqueue;
	mutex mymutex;
	condition_variable my_cond;
public:
	void inmsgrecqueue()//把数据写入消息队列的线程
	{

		for (int i = 0; i < 100000; i++)
		{
			cout << "命令写入一个元素" << i << endl;
			//lock_guard<mutex> sbguard(mymutex);
			//mymutex.lock();
			std::unique_lock<mutex> sbguard(mymutex);
			msgrecqueue.push_back(i);


			my_cond.notify_one();//写入数据后，消息队列不为空，此时可以读，唤醒正在堵塞的wait（），使其向下执行
			my_cond.notify_all();//对于两个从消息队列获取数据的线程，我们使用notify_all可以唤醒多个wait
			//mymutex.unlock();
		}
		return;
	}
	



	void outmsgrecqueue()//把数据从消息队列取出的线程
	{
		int command = 0;
		while (true)
		{
			std::unique_lock<mutex> sbguard(mymutex);
			my_cond.wait(sbguard,[this]{//一个lambda表达式就是一个可调用对象（函数）
				if (!msgrecqueue.empty())
					return true;
				else
					return false;
			});

			//执行到这已经说明wait被唤醒，且判断消息队列不为空，此时该线程已经获得互斥锁。


			int command = msgrecqueue.front();
			msgrecqueue.pop_front();
			sbguard.unlock();//利用unique_lock灵活特点，随时解锁，以免锁住时间太长，此处取出后立即解锁，使得写入线程能够立即获取锁

			cout << "outmsgrecqueue执行，取出一个元素" << command <<"thread_id"<<this_thread::get_id<< endl;
		}
		
		return;
	}
};


int main()
{
	game gobj;
	thread myoutgobj(&game::outmsgrecqueue, &gobj);
	thread myoutgobj1(&game::outmsgrecqueue, &gobj);
	thread myingobj(&game::inmsgrecqueue, &gobj);
	myoutgobj.join();
	myoutgobj1.join();
	myingobj.join();

	return 0;
}

