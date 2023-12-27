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
	std::vector<int> data1 = {7, 8, 9};
	std::vector<int> data2 = {1, 2, 3};
	auto shuffle = [&](){
		std::osyncstream(std::cout) 
		<< "----shuffling data sets----" << std::endl << std::endl;
		std::swap(data1, data2);
	};
	auto print = [](auto& data, auto& out){
		for(auto& value : data)
			out << "value = " << value << std::endl;
	};
	std::barrier processed{ 2, shuffle };

	auto quadratic = [&processed, print](std::vector<int> & data){
		for(auto& value : data)
			value *=10;
		{
			std::osyncstream out(std::cout);
			out << "values before synchronising " 
			<< std::this_thread::get_id() << std::endl;
			print(data, out);
		}
		auto token = processed.arrive();
		std::osyncstream(std::cout) << "calculated" << std::endl;
		processed.wait(std::move(token));
		{
			std::osyncstream out(std::cout);
			out << "values after synchronising " 
			<< std::this_thread::get_id() << std::endl;
			print(data, out);
		}
		processed.arrive_and_wait();
		std::osyncstream (std::cout) << "finishing execution" << std::endl;
	};
	std::jthread t1(quadratic, std::ref(data1));
	std::jthread t2(quadratic, std::ref(data2));
	return 0;
}