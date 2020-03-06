#include <iostream>
#include <thread>
#include <mutex>

#include "SimpleTimer.h"

std::mutex m1;
std::mutex m2;

// using std::mutex
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
	std::lock_guard<std::mutex> g(m2);

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
	std::cout << "Function Print4, thread_ID: " << std::this_thread::get_id() << '\n';
	std::cout << std::endl;
	ugl.unlock();
}

// for using functions and anonymous functions in thread
void Sum1(int&result, int a, int b) {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	result = a + b;
}

int Sum2(int a, int b) {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	return a + b;
}

// for using class method in thread
class MyClass {
public:
	void show(int var) {
		std::cout << "Variable: " << var << std::endl;
	}

	int inc(int& var) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return ++var;
	}
};

int main() {
	// using std::mutex
	std::thread t1(Print1, '#');
	std::thread t2(Print1, '@');

	t1.join();
	t2.join();

	// using std::lock_guard
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::thread t3(Print2, '#');
	std::thread t4(Print2, '@');

	t3.join();
	t4.join();

	// using std::recursive_mutex
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::thread t5(Print3, '#', 5);
	std::thread t6(Print3, '@', 5);

	t5.join();
	t6.join();

	// using std::unique_lock
	std::cout << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
	std::thread t7(Print4, '#');
	std::thread t8(Print4, '@');

	t7.join();
	t8.join();

	// using function in thread
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	int result;
	int a = 4, b = 6;

	std::thread t9(Sum1, std::ref(result), a, b);
	t9.join();

	std::cout << "Result: " << result << std::endl;

	// using anonymous function in thread
	a = 4, b = 7;

	std::thread t10([&result, a, b]() {
		result = Sum2(a, b);
		});
	t10.join();
	std::cout << "Result: " << result << std::endl;

	// using class method in thread
	MyClass mc;
	int variable = 12;

		//if method returns nothing
	std::thread t11(&MyClass::show, &mc, variable);
	t11.detach();

		//if method returns value
	std::thread t12([&mc, &variable]() {
		variable = mc.inc(variable);
		});
	t12.join();

	std::cout << "Variable: " << variable << std::endl;

	//main thread
	for (size_t i = 0; i < 10; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		std::cout << "Main, thread_ID: " << std::this_thread::get_id() << '\n';
	}
	   	  
	return 0;
}