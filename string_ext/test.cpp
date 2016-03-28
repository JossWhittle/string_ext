#include "string_ext.h"

struct Test {
	int a;
	double d;

	friend std::ostream& operator<<(std::ostream& os, const Test &v) {
		os << format_str("Test{%i, %f}", v.a, v.d);
		return os;
	};
};

int main() {
	Test obj{5, 3.14};
	
	std::cout << format_str("int:\n"
		"%d, %i, %u, %#o, %#x, %#X, \n\n"
		"float:\n"
		"%f, %F, %e, %E, \n%g, %G, %a, %A, \n\n"
		"double:\n"
		"%f, %F, %e, %E, \n%g, %G, %a, %A, \n\n"
		"long double:\n"
		"%f, %F, %e, %E, \n%g, %G, %a, %A, \n\n"
		"string, char, ptr, ostream:\n"
		"\"%s\", '%c', %p, %s\n",
		-1, -2, 3u, 4, 12, 15,
		1.1f, 2.2f, 3.3f, 4.4f, 
		5.5f, 6.6f, 7.f, 8.f,
		1.1, 2.2, 3.3, 4.4, 
		5.5, 6.6, 7., 8.,
		1.1L, 2.2L, 3.3L, 4.4L, 
		5.5L, 6.6L, 7.L, 8.L,
		"Hello world.", '&', &obj, obj) << std::endl;
	
	return EXIT_SUCCESS;
};