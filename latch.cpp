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
int main()
{
	std::vector<int> data = {10, 15, 20};
	std::latch processed{ 3 };

	auto quadratic = [&processed](int & value){
		value *= value;
		std::this_thread::sleep_for(ms(value));
		std::osyncstream(std::cout) << "calculated" << std::endl;
		processed.arrive_and_wait();
		std::osyncstream(std::cout) << "value = " << value << std::endl;
	};
	std::vector<std::jthread> t_pool;
	for(int i = 0; i < 3; i++)
		t_pool.emplace_back(quadratic, std::ref(data[i]));
	return 0;
}
