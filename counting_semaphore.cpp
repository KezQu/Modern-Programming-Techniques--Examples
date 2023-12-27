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

using ms =  std::chrono::milliseconds;
int main(){
	std::counting_semaphore semaphore(0);
	auto f = [&semaphore](std::stop_token token){
		int waited = 0;
		std::cout << "started thread: " << std::this_thread::get_id() << std::endl;
		while(!semaphore.try_acquire() && !token.stop_requested())
		{
			waited += 1;
			std::this_thread::sleep_for(ms(1));
		}
		if(token.stop_requested())
			std::cout << "Exiting without getting access in thread: " 
			<< std::this_thread::get_id() << std::endl;
		else
			std::cout << "After " << waited << " ms got access in thread: " 
			<< std::this_thread::get_id() << std::endl;
	};
	std::vector<std::jthread> thread_pool;
	for(int i = 0; i < 3; i++)
	{
		thread_pool.push_back(std::jthread(f));
		std::this_thread::sleep_for(ms(1));
	}
	for(auto time : {5, 10})
	{
		std::this_thread::sleep_for(ms(time));
		semaphore.release();
	}
	return 0;
}