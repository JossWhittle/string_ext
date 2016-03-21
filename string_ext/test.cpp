#include <iostream>
#include "string_ext.h"

struct Test {
	int a;
	double d;
};

std::ostream& operator<<(std::ostream& os, const Test &v) {
	os << format("{%d, %d}", v.a, v.d);
	return os;
};

int main() {

	Test obj{5, 3.14};
	std::cout << format("Hello %s %s\n", obj, "Hello world") << std::endl;
	
	std::system("PAUSE");
	return 0;
};