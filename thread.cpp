#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <vector>
#include <semaphore>
#include <latch>
#include <barrier>
#include <syncstream>
#include <algorithm>
#include <numeric>
#include <execution>
#include <random>

void f(int & val){
	std::cout << "Starting new thread\n";
	std::cout << val << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << val << std::endl;
	std::cout << "Exiting thread\n";
}
class Int{
public:
	int x;
	Int(int xa): x{xa}{} 
};

void swap(Int & l1, Int & l2)
{
	auto l3 = l1;
	l1=l2;
	l2=l3;
}
int main(){
	Int value = 5;
	Int value2 = 10; 
	std::cout << value.x << std::endl;
	std::cout << value2.x << std::endl;
	swap(value, value2);
	std::cout << value.x << std::endl;
	std::cout << value2.x << std::endl;

	std::thread t1;
	std::cout << "thread id: " << t1.get_id() << std::endl;
	
	std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;
	
	std::thread t2(f, std::ref(value.x));
	std::cout << "thread id: " << t2.get_id() << std::endl;
	
	// std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

	std::this_thread::sleep_for(std::chrono::nanoseconds(1));

	std::thread t3(std::move(t2));
	std::cout << "thread id: " << t3.get_id() << std::endl;

	std::cout << "---Exiting main---\n";
	t3.join();
	return 0;
}

// int main(){
	
// 	std::jthread t1;

// 	int value = 5;
// 	std::cout << "thread id: " << t1.get_id() << std::endl;

// 	std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;
// 	std::jthread t2(f, std::ref(value));
// 	std::cout << "thread id: " << t2.get_id() << std::endl;
// 	std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

// 	std::this_thread::sleep_for(std::chrono::nanoseconds(1));


// 	std::jthread t3(std::move(t2));
// 	std::cout << "thread id: " << t3.get_id() << std::endl;

// 	return 0;
// }
using ms =  std::chrono::milliseconds;
/*===================================RETURN===========================================*/
// int f(int x)
// {
// 	throw std::exception();
// 	// return x;
// }

// int main()
// {
// 	int x =5;
// 	std::thread t1(f,x);
// 	t1.join();
// 	std::cout << "finishing main thread" << std::endl;
// 	return 0;
// }
/*===================================JOIN DETACH===========================================*/
// void f()
// {
// 	std::this_thread::sleep_for(ms(10));
// 	std::cout << std::this_thread::get_id() << " thread finished" << std::endl;
// }
// void submain(){
// 	std::thread t1(f);
// 	std::cout << "waiting for " << t1.get_id() << " thread" << std::endl;
// 	t1.detach();
// 	std::cout << "finishing submain" << std::endl;
// }
// int main()
// {
// 	submain();
// 	std::cout << "back to main" << std::endl;
// 	std::this_thread::sleep_for(std::chrono::milliseconds(11));
// 	return 0;
// }
// void submain()
// {
// 	std::jthread t1(f);
// 	std::cout << "waiting for " << t1.get_id() << " thread" << std::endl;
// 	std::cout << "finishing submain" << std::endl;
// }
// void submain()
//{
// 	std::thread t1(f);
// 	std::cout << "waiting for " << t1.get_id() << " thread" << std::endl;
// 	t1.detach();
// 	std::cout << "finishing submain" << std::endl;
// }
// void submain()
//{
// 	int x =5;
// 	auto begin = steady_clock::now();
// 	std::jthread t1(f,x, begin);
// 	std::cout << "waiting for " << t1.get_id() << " thread: " << duration_cast<ms>(steady_clock::now() - begin).count() << " ms" << std::endl;
// 	t1.detach();
// 	std::cout << "finishing submain" << std::endl;
// }
/*===================================CANCELATION===========================================*/
// void f(std::stop_token stop, int& x)
// {
// 	while(!stop.stop_requested())
// 	{
// 		x++;
// 		std::this_thread::sleep_for(ms(1));
// 	}
// 	if(stop.stop_requested())
// 		std::cout << "stop requested - exiting thread: " <<std::this_thread::get_id() << std::endl;
// 	std::cout << "loops made: " << x << std::endl;
// }
// int main()
// {
// 	int x = 0;
// 	std::cout << "x = " << x << std::endl;
	
// 	std::jthread t1(f, std::ref(x));
// 	std::this_thread::sleep_for(ms(10));
	
// 	t1.request_stop();
// 	std::cout << "stop requested manually" << std::endl;
// 	std::this_thread::sleep_for(ms(1));

// 	std::cout << "x = " << x << std::endl;
// 	std::cout << "exiting main thread" << std::endl;
// 	return 0;
// }
// int main()
// {
// 	int x = 0;
// 	std::jthread t1(f, std::ref(x));
// 	std::this_thread::sleep_for(ms(10));
// 	std::cout << "exiting main thread" << std::endl;
// 	return 0;
// }
// void Stop(std::stop_source& stop_src) { stop_src.request_stop(); }
// int main()
// {
// 	int x = 0;
// 	std::jthread t1(f, std::ref(x));
// 	std::stop_source stop_src= t1.get_stop_source();
// 	std::this_thread::sleep_for(ms(10));
// 	Stop(stop_src);
// 	std::this_thread::sleep_for(ms(1));
// 	std::cout << "exiting main thread" << std::endl;
// 	return 0;
// }
// int main()
// {
// 	int x = 0;
// 	std::jthread t1(f, std::ref(x));
// 	std::stop_callback callback(t1.get_stop_token(), [](){
// 		std::cout << "Invoking stop_callback object's function requested by thread: " 
// 		<< std::this_thread::get_id() << std::endl;
// 	});
// 	std::this_thread::sleep_for(ms(10));
// 	t1.request_stop();
// 	std::cout << "stop requested manually" << std::endl;
// 	std::this_thread::sleep_for(ms(1));
// 	std::cout << "exiting main thread: " << std::this_thread::get_id() << std::endl;
// 	return 0;
// }