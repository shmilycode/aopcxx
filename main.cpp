#include "aopcxx.h"
#include <iostream>

class Log{
public:
	void before(const char* str) {
		std::cout << "Log before " << str << std::endl;
	}

	void after(const char* str) {
		std::cout << "Log after " << str << std::endl;
	}
};

class Clock{
public:
	void before(const char* str) {
		std::cout << "Clock before " << str << std::endl;
	}

	void after(const char* str) {
		std::cout << "Clock after " << str << std::endl;
	}
};

void foo(const char* str) {
	std::cout << str << std::endl;
}

int main() {
	make_aspect<Log, Clock>(&foo, "foo function");
	return 0;
}
