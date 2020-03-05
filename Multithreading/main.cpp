#include <iostream>
#include <thread>
#include <mutex>

#include "SimpleTimer.h"

std::mutex m1;

// std::using mutex
void Print1(char ch) {
	m1.lock();

	SimpleTimer h;
	for (size_t i = 0; i < 5; ++i) {
		for (size_t j = 0; j < 10; ++j) {
			std::cout << ch;
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		std::cout << std::endl;
	}
	std::cout << "Function Print1, thread_ID: " << std::this_thread::get_id() << '\n';
	std::cout << std::endl;
	m1.unlock();
}

// using std::lock_guard
void Print2(char ch) {
	std::lock_guard<std::mutex> g(m1);

	SimpleTimer h;
	for (size_t i = 0; i < 5; ++i) {
		for (size_t j = 0; j < 10; ++j) {
			std::cout << ch;
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		std::cout << std::endl;
	}
	std::cout << "Function Print2, thread_ID: " << std::this_thread::get_id() << '\n';
	std::cout << std::endl;
}

// using std::recursive_mutex
std::recursive_mutex rm1;
void Print3(char ch, int a) {
	rm1.lock();
	if (a == 0) {
		rm1.unlock();
		return;
	}

	for (size_t j = 0; j < 10; ++j) {
		std::cout << ch;
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
	std::cout << std::endl;

	a -= 1;
	Print3(ch, a);
	rm1.unlock();

}

// using std::unique_lock
void Print4(char ch) {
	SimpleTimer h;
	std::unique_lock<std::mutex> ugl(m1, std::defer_lock);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	ugl.lock();

	for (size_t i = 0; i < 5; ++i) {
		for (size_t j = 0; j < 10; ++j) {
			std::cout << ch;
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		std::cout << std::endl;
	}
	std::cout << "Function Print2, thread_ID: " << std::this_thread::get_id() << '\n';
	std::cout << std::endl;
	ugl.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

// for using functions and anonymous functions in thread
void Sum() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//return 5 + 6;
	std::cout << "Hello" << std::endl;
}

// for using class method in thread

class MyClass {
public:
	int inc(int a) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return ++a;
	}
};





int main() {
	// std::using mutex
	std::thread t1(Print1, '#');
	std::thread t2(Print1, '@');

	t1.join();
	t2.join();

	// using std::lock_guard
	std::thread t3(Print2, '#');
	std::thread t4(Print2, '@');

	t3.join();
	t4.join();

	std::thread t5(Print3, '#', 5);
	std::thread t6(Print3, '@', 5);

	t5.join();
	t6.join();

	std::cout << std::endl;

	std::thread t7(Print4, '#');
	std::thread t8(Print4, '@');

	t7.join();
	t8.join();

	// for using functions and anonymous functions in thread
	int result;
	int a = 5, b = 6;
	std::thread t9(Sum);
	t9.join();
	//std::cout << result << std::endl;

	// for using functions and anonymous functions in thread
	// for using class method in thread

	// through anonymous function
	/*std::thread t9([]() {
		})*/



	for (size_t i = 0; i < 10; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		std::cout << "Main, thread_ID: " << std::this_thread::get_id() << '\n';
	}








	return 0;
}