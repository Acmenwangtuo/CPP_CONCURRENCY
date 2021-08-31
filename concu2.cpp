#include <map>
#include <string>
#include <thread>
#include <iostream>
#include <list>
#include <vector>
#include <mutex>
using namespace std;

//vector<int> g_v = {1, 2, 3}; // ֻ�������ǰ�ȫ��

void myprint(int inum) {
//	cout << "�߳̿�ʼִ�����̱߳��=" << inum << endl;
//	cout << "�߳̽���ִ���ˣ��̱߳��" << inum << endl;
//	g_v[0] = 5;
//	cout << "idΪ" << std::this_thread::get_id() << "���̴߳�ӡg_vֵ" << g_v[0] << g_v[1] << g_v[2] << endl;
//	return;
}

class A {
	public:
		void inMsgRecvQueue() {
			for (int i = 0; i < 100000; i++) {
				cout << "inMsgRecvQueue()������һ��Ԫ��" << i << endl;
//				my_mutex.lock();
//				my_mutex2.lock();// ˳���෴���������
				std::unique_lock<std::mutex> sbguard1(my_mutex, std::try_to_lock); //����Ҳ�ᱻ��ס�ò�����
				if (sbguard1.owns_lock()) { //����������ʾ�õ�����
					msgRecvQueue.push_back(i);
				} else {
					//û�õ���
					cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;
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
					cout << "outMsgRecvQueue()��ɾ��һ��Ԫ��" << command << endl;
				else
					cout << "outMsgRecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << endl;
//				if (!msgRecvQueue.empty()) {
//					int command = msgRecvQueue.front();
//					msgRecvQueue.pop_front();
//					cout << "outMsgRecvQueue()��ɾ��һ��Ԫ��" << command << endl;
//				} else {
//					cout << "outMsgRecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << endl;
//				}
			}
		}
		bool outMsgLULProc(int &command) {
			//std::lock_guard<std::mutex> sbguard(my_mutex);//���캯�����װ��lock�������������װ��unlock������ֻ�г����������ʱ����Լ�����
//			my_mutex2.lock();
//			my_mutex.lock();
			//std::lock(my_mutex2,my_mutex);
			//std::lock_guard<std::mutex> sbguard1(my_mutex,std::adopt_lock); // �ܽ᣺ʹ��std::adopt_lock��ǰ���ǿ�������Ҫ�Ȱѻ�����lock��
			//std::lock_guard<std::mutex> sbguard2(my_mutex2,std::adopt_lock); //�Ѿ�������Ҫ�������־λ
			std::unique_lock<std::mutex> sbguard1(my_mutex);//unique_lock��ռ�ڴ�
			std::chrono::milliseconds dura(200);//��������20s
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
