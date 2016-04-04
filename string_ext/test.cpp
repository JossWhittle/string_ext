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
		int p;
		os << 10 << format_str(" Test{%i, %f}%n %d", v.a, v.d, &p, p);
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
	//            f, e, E, g, G, a, A    (float / double / long double)
	//            s                      (string / ostream<<), 
	//            c                      (char), 
	//            p, n                   (ptr / int* for string length)
	//            b, B                   (bool)
	
	/// No args
	printf("Hello world.\n");
	std::cout << format_str("Hello world.\n") << std::endl;

	/// int
	printf("int %d %i %u %o %x %X\n", -1, -2, 3u, 4, 5u, -6);
	std::cout << format_str("int %d %i %u %o %x %X\n", -1, -2, 3u, 4, 5u, -6) << std::endl;
	/// int - Base
	printf("int %#o %#x %#X\n", 4, 5, -6);
	std::cout << format_str("int %#o %#x %#X\n", 4, 5, -6) << std::endl;
	/// int - Width / Alignment
	printf("int |%-10d|%10d|\n", 4, 5);
	std::cout << format_str("int |%-10d|%10d|\n", 4, 5) << std::endl;
	/// int - Width / Pad with Zero
	printf("int %08d %08d\n", 44, 5555);
	std::cout << format_str("int %08d %08d\n", 44, 5555) << std::endl;
	/// int - Sign
	printf("int %+d %+d\n", -1, 2);
	std::cout << format_str("int %+d %+d\n", -1, 2) << std::endl;
	/// int - Char Count
	//int a = 0;
	//printf("int %n %d\n", &a, a); // Compiler error MSVC'2013, %n deprecated
	//int b;
	//std::cout << format_str("int %n%d\n", &b, b) << std::endl; // Works!

	/// float
	printf("float %f\n", 3.14f);
	std::cout << format_str("float %f\n", 3.14) << std::endl;
	/// float - Base
	printf("float %#f %f %#f %f %#f\n", 3.14, 2., 2., 2.0, 2.0); // MSVC'2013 printf not respecting padding
	std::cout << format_str("float %#f %f %#f %f %#f\n", 3.14, 2., 2., 2.0, 2.0) << std::endl; 
	/// float - Width
	printf("float |%1f|%2f|%4f|%8f|%16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789); 
	std::cout << format_str("float |%1f|%2f|%4f|%8f|%16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float - Width
	printf("float |%.1f|%.2f|%.4f|%.8f|%.16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float |%.1f|%.2f|%.4f|%.8f|%.16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;


	//std::cout << format_str("Cause an error: %m", 0);
	//
	// Line: 100 File: 'test.cpp'
	// String Format | Undefined format specifier : 'm'

	//std::cout << format_str("Cause an error: %i", 0.f);
	//
	// Line: 105 File: 'test.cpp'
	// String Format | Incorrect format specifier for type (float): Saw 'i' | Expected 'f, F, e, E, g, G, a, A'
	
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