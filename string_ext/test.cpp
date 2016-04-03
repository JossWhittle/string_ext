/*

Copyright (C) 2016, Joss Whittle

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.


*/

#include "string_ext.h"

struct Test {
	int a;
	double d;

	/// Use format specifier '%s' for types with an ostream operator 
	friend std::ostream& operator<<(std::ostream& os, const Test &v) {
		os << 10 << format_str(" Test{%i, %f}", v.a, v.d);
		return os;
	};
};

int main() {
	Test obj{5, 3.14};
	
	/// Call to get a formatted string 
	// str::format("...", args...)
	
	/// Alternatively, call macro to pass along __LINE__ and __FILE__ for debugging
	// format_str("...", args...)

	/// Formats follow the form:
	// %[flags][width][.precision]specifier 
	//
	// Use %% for a percent sign
	//
	// flags    : - (left align)
	//            + (show sign)
	//            0 (pad with 0's)
	//            # (show base or decimal) 
	//
	// specifier: d, i, u, o, x, X       (int)
	//            f, F, e, E, g, G, a, A (float / double / long double)
	//            s                      (string / ostream<<), 
	//            c                      (char), 
	//            p                      (ptr)
	//            b, B                   (bool)
	
	std::cout << format_str("Base and Point:\n%#x\n%#X\n%#f\n\n", 0xc, 0xf, 42.1f);

	std::cout << format_str("Precision:\n%f\n%.1f\n%.2f\n%.3f\n"
							"%.4f\n%.5f\n%.6f\n%.7f\n\n", 
							1234.56789, 1234.56789, 1234.56789, 1234.56789, 
							1234.56789, 1234.56789, 1234.56789, 1234.56789);

	std::cout << format_str("Padding:\n%d\n%1d\n%2d\n%4d\n%8d\n%16d\n\n", 0, 1, 2, 4, 8, 16);

	std::cout << format_str("Alignment:\n%-10d Left Aligned\n%10d Right Aligned\n\n", -10, 10);

	std::cout << format_str("Types:\nbool:\n"
		"%b, %B, %#b, %#B, \n\n"
		"\nint:\n"
		"%d, %i, %u, %#o, %#x, %#X, \n\n"
		"float:\n"
		"%f, %F, %e, %E, \n%g, %G, %a, %A, \n\n"
		"double:\n"
		"%f, %F, %e, %E, \n%g, %G, %a, %A, \n\n"
		"long double:\n"
		"%f, %F, %e, %E, \n%g, %G, %a, %A, \n\n"
		"string, char, ptr, ostream:\n"
		"\"%-20s\", '%c', %p, %s\n\n",
		true, false, true, false,
		-1, -2, 3u, 4, 12, 15,
		1.1f, 2.2f, 3.3f, 4.4f, 
		5.5f, 6.6f, 7.f, 8.f,
		1.1, 2.2, 3.3, 4.4, 
		5.5, 6.6, 7., 8.,
		1.1L, 2.2L, 3.3L, 4.4L, 
		5.5L, 6.6L, 7.L, 8.L,
		"Hello world.", '&', &obj, obj);

	std::cout << format_str("ostream formatting:\n|%20.6s|\n|%-20s|\n\n", obj, obj);

	unsigned int a, b = 0;
	std::cout << format_str("Hello%n world.%n | ", &a, &b);
	std::cout << format_str("%#x %#x\n\n", a, b);

	//std::cout << format_str("Cause an error: %m", 0);
	//
	// Line: 100 File: 'test.cpp'
	// String Format | Undefined format specifier : 'm'

	//std::cout << format_str("Cause an error: %i", 0.f);
	//
	// Line: 105 File: 'test.cpp'
	// String Format | Incorrect format specifier : Saw 'i' | Expected 'f, F, e, E, g, G, a, A'
	
	//std::cout << format_str("Cause an error: %");
	//
	// Line: 110 File: 'test.cpp'
	// String Format | Incomplete format string : Ended in '%'

	//std::cout << format_str("Cause an error: %d");
	//
	// Line: 115 File: 'test.cpp'
	// String Format | Not enough arguments
	
	//std::cout << format_str("Cause an error: ", 1, 2, 3);
	//
	// Line: 120 File: 'test.cpp'
	// String Format | Unused arguments : '3'

	//std::cout << format_str("Cause an error: %##d", 0);
	//
	// Line: 125 File: 'test.cpp'
	// String Format | Format flag already set : '#'

	return EXIT_SUCCESS;
};