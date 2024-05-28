#include<iostream>
#include<thread>
#include<vector>
#include<queue>
#include<stdexcept>
#include<condition_variable>
#include<memory>
#include<assert.h>
using namespace std;
const int MAX_THREADS = 1000;
//线程安全队列
template<typename T>
class ThreadSafeQue {
public:
	bool empty() {
		unique_lock<mutex>lk(queue_mutex);
		return _queue.empty;
	}
	bool pop(T &ret) {
		unique_lock<mutex>lk(queue_mutex);
		if (_queue.empty()) {
			return false;
		}
		ret=_queue.front();
		_queue.pop();
		return true;
	}
	void push(T &data) {
		unique_lock<mutex>lk(queue_mutex);
		_queue.push(data);
		lk.unlock();
		condition.notify_all();
	}
	ThreadSafeQue(){};
	~ThreadSafeQue() {};
	mutex queue_mutex;
private:
	condition_variable condition;
	queue<T>_queue;
};
template<typename T>
class threadPool {
public:
	threadPool(int number = 1);
	~threadPool();
	bool appand(T* request);//添加任务函数
private:
	mutex pool_mutex;//线程池的锁
	bool stop;//停止标志
	
	static void* worker(void* arg);//每个线程都要执行的函数
	void run();

	ThreadSafeQue<T*>task_queue;//任务队列，提供缓冲
	vector<thread>work_threads;//存放多个工作线程
};
template<typename T>
threadPool<T>::threadPool(int number) :stop(false) {
	//数量小于零或大于最大值
	if (number <= 0 || number > MAX_THREADS) {
		throw exception();
	}
	//创建几个线程存到线程1池
	cout << "threadPool create " << number << " threads" << endl;
	for (int i = 0; i < number; ++i) {
		unique_lock<mutex> lk(pool_mutex);
		work_threads.emplace_back(worker, this);
	}
}
template<typename T>
threadPool<T>::~threadPool() {
	//对stop操作，要上锁
	unique_lock<mutex> lk(pool_mutex);
	stop = true;
	//等待所有线程结束
	for (auto& wt : work_threads) {
		wt.join();
	}
	cout << "ThreadPool is already closed" << endl;
}
template<typename T>
bool threadPool<T>::appand(T* request) {
	task_queue.push(request);
	return true;
}
template<typename T>
void* threadPool<T>::worker(void* arg) {
	//线程运行
	threadPool* pool = (threadPool*)arg;
	pool->run();
	return pool;
}
template<typename T>
void threadPool<T>::run() {
	//还未停止则继续取任务
	while (!stop) {
		T* request;
		if (this->task_queue.pop(request)) {
			cout << "thread" << this_thread::get_id << ":" << endl;
			request->process();
		}
	}
}
class Request {
public:
	Request(char c) :ch(c) {};
	void process() {
		for (int i = 0; i < 10; ++i) {
			cout << ch << i << endl;
		}
	}
private:
	char ch;
};
int main() {
	Request re1('#'), re2('A');
	threadPool<Request> pool(3);
	pool.appand(&re1);
	pool.appand(&re2);
	this_thread::sleep_for(std::chrono::seconds(3));
	return 0;
}