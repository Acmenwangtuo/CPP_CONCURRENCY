#include <map>
#include <string>
#include <thread>
#include <iostream>
#include <list>
#include <vector>
#include <mutex>
using namespace std;

//vector<int> g_v = {1, 2, 3}; // 只读数据是安全的

void myprint(int inum) {
//	cout << "线程开始执行了线程编号=" << inum << endl;
//	cout << "线程结束执行了，线程编号" << inum << endl;
//	g_v[0] = 5;
//	cout << "id为" << std::this_thread::get_id() << "的线程打印g_v值" << g_v[0] << g_v[1] << g_v[2] << endl;
//	return;
}

class A {
	public:
		void inMsgRecvQueue() {
			for (int i = 0; i < 100000; i++) {
				cout << "inMsgRecvQueue()，插入一个元素" << i << endl;
//				my_mutex.lock();
//				my_mutex2.lock();// 顺序相反会造成死锁
				std::unique_lock<std::mutex> sbguard1(my_mutex, std::try_to_lock); //这里也会被卡住拿不到锁
				if (sbguard1.owns_lock()) { //条件成立表示拿到了锁
					msgRecvQueue.push_back(i);
				} else {
					//没拿到锁
					cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
				}
//				my_mutex2.unlock();
//				my_mutex.unlock();
			}
		}
		void outMsgRecvQueue() {
			int command = 0;
			for (int i = 0; i < 100000; i++) {
				bool result = outMsgLULProc(command);
				if (result)
					cout << "outMsgRecvQueue()，删除一个元素" << command << endl;
				else
					cout << "outMsgRecvQueue()执行，但目前消息队列中为空" << endl;
//				if (!msgRecvQueue.empty()) {
//					int command = msgRecvQueue.front();
//					msgRecvQueue.pop_front();
//					cout << "outMsgRecvQueue()，删除一个元素" << command << endl;
//				} else {
//					cout << "outMsgRecvQueue()执行，但目前消息队列中为空" << endl;
//				}
			}
		}
		bool outMsgLULProc(int &command) {
			//std::lock_guard<std::mutex> sbguard(my_mutex);//构造函数里封装了lock，析构函数里封装了unlock，但是只有超出作用域的时候会自己析构
//			my_mutex2.lock();
//			my_mutex.lock();
			//std::lock(my_mutex2,my_mutex);
			//std::lock_guard<std::mutex> sbguard1(my_mutex,std::adopt_lock); // 总结：使用std::adopt_lock的前提是开发者需要先把互斥量lock上
			//std::lock_guard<std::mutex> sbguard2(my_mutex2,std::adopt_lock); //已经锁上需要用这个标志位
			std::unique_lock<std::mutex> sbguard1(my_mutex);//unique_lock更占内存
			std::chrono::milliseconds dura(200);//卡在这里20s
			std::this_thread::sleep_for(dura);
			if (!msgRecvQueue.empty()) {
				command = msgRecvQueue.front();
				msgRecvQueue.pop_front();
				//my_mutex.unlock();
				//my_mutex2.unlock();
				return true;
			}
//			my_mutex.unlock();
//			my_mutex2.unlock();
			return false;
		}
	private:
		list<int> msgRecvQueue;
		std::mutex my_mutex, my_mutex2;
};

int main() {
//	vector<thread> mythread;
//	for (int i = 0; i < 10; i++) {
//		mythread.push_back(thread(myprint, i));
//	}
//	for (auto &iter : mythread)
//		iter.join();
//	cout << "I love China" << endl;
//	return 0;
	A myobj;
	std::thread myOutMsgObj(&A::outMsgRecvQueue, &myobj);
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobj);
	myOutMsgObj.join();
	myInMsgObj.join();
	return 0;
}
