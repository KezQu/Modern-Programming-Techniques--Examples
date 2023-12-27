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
std::binary_semaphore bsemaphore(0);
void f(std::vector<int>& data)
{
	std::cout << "waiting for data on thread: " << 
	std::this_thread::get_id() << std::endl;
	bsemaphore.acquire();
	std::cout << "data acquired" << std::endl;
	for(auto& value : data)
		value += 10;
	std::cout << "data processed" << std::endl;
	bsemaphore.release();
}
int main()
{
	std::vector<int> data;
	std::jthread t1(f, std::ref(data));
	std::this_thread::sleep_for(ms(1));
	std::cout << "preparing data on thread: " << 
	std::this_thread::get_id() << std::endl;
	for(int i = 0; i < 10; ++i)
		data.push_back(i);
	for(auto& value : data)
		std::cout << value << ", ";
	std::cout << std::endl << "data prepared" << std::endl;
	bsemaphore.release();
	std::cout << "waiting for processing..." << std::endl;
	std::this_thread::sleep_for(ms(1));
	bsemaphore.acquire();
	for(auto& value : data)
		std::cout << value << ", ";
	return 0;
}