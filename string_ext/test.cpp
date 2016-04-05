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
		os << "(" << 10 << format_str(" Test{%i, %f}", v.a, v.d) << ")";
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
	// specifier: d, i, u, o, x, X, n    (int)
	//            f, e, E, g, G, a, A    (float / double / long double)
	//            s                      (string / ostream<<), 
	//            c                      (char), 
	//            p                      (ptr)
	//            b, B                   (bool)

	/// No args
	printf("Hello world.\n");
	std::cout << format_str("Hello world.\n") << std::endl;

	/// int
	printf("int %d %i %u %o %x %X\n", -1, -2, 3u, 4, 5u, -6);
	std::cout << format_str("int %d %i %u %o %x %X\n", -1, -2, 3u, 4, 5u, -6) << std::endl;
	/// int - Base Zeros
	printf("int %o %x %X %#o %#x %#X\n", 0, 0, 0, 0, 0, 0);
	std::cout << format_str("int %o %x %X %#o %#x %#X\n", 0, 0, 0, 0, 0, 0) << std::endl;
	/// int - Types
	printf("int %hd %ld %lld %hu %lu %llu\n", (short) -1, -2l, -3ll, (unsigned short) 4, 5ul, 6ull); // Need to use h, l, ll modifiers for types
	std::cout << format_str("int %d %d %d %u %u %u\n", (short) -1, -2l, -3ll, (unsigned short) 4u, 5ul, 6ull) << std::endl; // Types determined automatically!
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
	int b;
	std::cout << format_str("int: %n%d\n", b, b) << std::endl; // Works!
	int c;
	std::cout << format_str("int%n|\n%*s|%d\n", c, c, "", c) << std::endl; // Works!

	/// float
	printf("float %f\n", 3.14f);
	std::cout << format_str("float %f\n", 3.14f) << std::endl;
	/// float - Types
	printf("float %.16f %.16f %.16Lf\n", 1.23456789f, 1.23456789, 1.23456789L); // Need to use L modifier for long double
	std::cout << format_str("float %.16f %.16f %.16f\n", 1.23456789f, 1.23456789, 1.23456789L) << std::endl; // Types determined automatically!
	/// float - Base
	printf("float %#f %f %#f %f %#f\n", 3.14, 2., 2., 2.0, 2.0); 
	std::cout << format_str("float %#f %f %#f %f %#f\n", 3.14, 2., 2., 2.0, 2.0) << std::endl; 
	/// float - Width
	printf("float |%1f|%2f|%4f|%8f|%16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789); 
	std::cout << format_str("float |%1f|%2f|%4f|%8f|%16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float - Precision
	printf("float |%.1f|%.2f|%.4f|%.8f|%.16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float |%.1f|%.2f|%.4f|%.8f|%.16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float - Width / Precision
	printf("float |%10.1f|%10.2f|%10.4f|%10.8f|%10.16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float |%10.1f|%10.2f|%10.4f|%10.8f|%10.16f|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float - Sign
	printf("float '%+f' '%+f' '%f' '%f'\n", -1.f, 2.f, -1.f, 2.f);
	std::cout << format_str("float '%+f' '%+f' '%f' '%f'\n", -1.f, 2.f, -1.f, 2.f) << std::endl;

	/// float-e
	printf("float-e %e %E\n", 3.14f, 3.14f);
	std::cout << format_str("float-e %e %E\n", 3.14f, 3.14f) << std::endl;
	/// float-e - Types
	printf("float-e %.16e %.16e %.16Le\n", 1.23456789f, 1.23456789, 1.23456789L); // Need to use L modifier for long double
	std::cout << format_str("float-e %.16e %.16e %.16e\n", 1.23456789f, 1.23456789, 1.23456789L) << std::endl; // Types determined automatically!
	/// float-e - Base
	printf("float-e %#e %e %#e %e %#e\n", 3.14, 2., 2., 2.0, 2.0);
	std::cout << format_str("float-e %#e %e %#e %e %#e\n", 3.14, 2., 2., 2.0, 2.0) << std::endl;
	/// float-e - Width
	printf("float-e |%1e|%2e|%4e|%8e|%16e|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float-e |%1e|%2e|%4e|%8e|%16e|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float-e - Precision
	printf("float-e |%.1e|%.2e|%.4e|%.8e|%.16e|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float-e |%.1e|%.2e|%.4e|%.8e|%.16e|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float-e - Width / Precision
	printf("float-e |%10.1e|%10.2e|%10.4e|%10.8e|%10.16e|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float-e |%10.1e|%10.2e|%10.4e|%10.8e|%10.16e|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float-e - Sign
	printf("float-e '%+e' '%+e' '%e' '%e'\n", -1.f, 2.f, -1.f, 2.f);
	std::cout << format_str("float-e '%+e' '%+e' '%e' '%e'\n", -1.f, 2.f, -1.f, 2.f) << std::endl;

	/// float-g
	printf("float-g %g %G\n", 3.14f, 3.14f);
	std::cout << format_str("float-g %g %G\n", 3.14f, 3.14f) << std::endl;
	/// float-g - Types
	printf("float-g %.16g %.16g %.16Lg\n", 1.23456789f, 1.23456789, 1.23456789L); // Need to use L modifier for long double
	std::cout << format_str("float-g %.16g %.16g %.16g\n", 1.23456789f, 1.23456789, 1.23456789L) << std::endl; // Types determined automatically!
	/// float-g - Base
	printf("float-g %#g %g %#g %g %#g\n", 3.14, 2., 2., 2.0, 2.0); 
	std::cout << format_str("float-g %#g %g %#g %g %#g\n", 3.14, 2., 2., 2.0, 2.0) << std::endl;
	/// float-g - Width
	printf("float-g |%1g|%2g|%4g|%8g|%16g|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float-g |%1g|%2g|%4g|%8g|%16g|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float-g - Precision
	printf("float-g |%.1g|%.2g|%.4g|%.8g|%.16g|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float-g |%.1g|%.2g|%.4g|%.8g|%.16g|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float-g - Width / Precision
	printf("float-g |%10.1g|%10.2g|%10.4g|%10.8g|%10.16g|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float-g |%10.1g|%10.2g|%10.4g|%10.8g|%10.16g|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float-g - Sign
	printf("float-g '%+g' '%+g' '%g' '%g'\n", -1.f, 2.f, -1.f, 2.f);
	std::cout << format_str("float-g '%+g' '%+g' '%g' '%g'\n", -1.f, 2.f, -1.f, 2.f) << std::endl;

	/// float-a
	printf("float-a %a %A\n", 3.14f, 3.14f);
	std::cout << format_str("float-a %a %A\n", 3.14f, 3.14f) << std::endl;
	/// float-a - Types
	printf("float-a %.16a %.16a %.16La\n", 1.23456789f, 1.23456789, 1.23456789L); // Need to use L modifier for long double
	std::cout << format_str("float-a %.16a %.16a %.16a\n", 1.23456789f, 1.23456789, 1.23456789L) << std::endl; // Types determined automatically!
	/// float-a - Base
	printf("float-a %#a %a %#a %a %#a\n", 3.14, 2., 2., 2.0, 2.0); 
	std::cout << format_str("float-a %#a %a %#a %a %#a\n", 3.14, 2., 2., 2.0, 2.0) << std::endl;
	/// float-a - Width
	printf("float-a |%1a|%2a|%4a|%8a|%16a|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float-a |%1a|%2a|%4a|%8a|%16a|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float-a - Precision
	printf("float-a |%.1a|%.2a|%.4a|%.8a|%.16a|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float-a |%.1a|%.2a|%.4a|%.8a|%.16a|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float-a - Width / Precision
	printf("float-a |%10.1a|%10.2a|%10.4a|%10.8a|%10.16a|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789);
	std::cout << format_str("float-a |%10.1a|%10.2a|%10.4a|%10.8a|%10.16a|\n", 12345.6789, 12345.6789, 12345.6789, 12345.6789, 12345.6789) << std::endl;
	/// float-a - Sign
	printf("float-a '%+a' '%+a' '%a' '%a'\n", -1.f, 2.f, -1.f, 2.f);
	std::cout << format_str("float-a '%+a' '%+a' '%a' '%a'\n", -1.f, 2.f, -1.f, 2.f) << std::endl;

	/// string
	printf("string '%s'\n", "Hello world");
	std::cout << format_str("string '%s'\n", "Hello world") << std::endl;
	/// string - ostream<<
	//printf("string '%s' '%s'\n", "Hello world", obj); // No way of directly converting type to string
	std::cout << format_str("string '%s' '%s'\n", "Hello world", obj) << std::endl;
	/// string - Width
	printf("string '%5s' '%10s' '%s'\n", "ABC", "ABCDEFGHIJKLMN", "ABCDEFGHIJKLMN");
	std::cout << format_str("string '%5s' '%10s' '%s'\n", "ABC", "ABCDEFGHIJKLMN", "ABCDEFGHIJKLMN") << std::endl;
	/// string - Precision
	printf("string '%.5s' '%.10s' '%s'\n", "ABCDEFGHIJKLMN", "ABCDEFGHIJKLMN", "ABCDEFGHIJKLMN");
	std::cout << format_str("string '%.5s' '%.10s' '%s'\n", "ABCDEFGHIJKLMN", "ABCDEFGHIJKLMN", "ABCDEFGHIJKLMN") << std::endl;

	/// char
	printf("char '%c' '%c'\n", (unsigned char) '&', '&');
	std::cout << format_str("char '%c' '%c'\n", (unsigned char) '&', '&') << std::endl;
	/// char - Width
	printf("char '%-4c' '%4c'\n", (unsigned char) '&', '&');
	std::cout << format_str("char '%-4c' '%4c'\n", (unsigned char) '&', '&') << std::endl;
	/// char - Precision
	printf("char '%.4c' '%.4c'\n", (unsigned char) '&', '&');
	std::cout << format_str("char '%.4c' '%.4c'\n", (unsigned char) '&', '&') << std::endl;

	/// ptr
	printf("ptr %#p %#p\n", (void*) nullptr, &obj);
	std::cout << format_str("ptr %#p %#p\n", (void*) nullptr, &obj) << std::endl;

	/// bool
	//printf("bool '%b' '%B' '%#b' '%#B'\n", true, false, false, true); // Does not handle bool
	std::cout << format_str("bool '%b' '%B' '%#b' '%#B'\n", true, false, false, true) << std::endl;
	/// bool - Width
	std::cout << format_str("bool '%10b' '%-10B' '%#10b' '%#-10B'\n", true, false, false, true) << std::endl;

	/// tabular
	int c0, c1, c2, c3;
	std::cout << format_str("| id %n",			c0)
			  << format_str("| name    %n",		c1)
			  << format_str("| rank %n",		c2)
			  << format_str("| desc.   %n|\n",	c3);
	
	const std::string rowFmt = "| %*d | %-*s | %-*d | %-.*s |\n";
	std::cout << format_str(rowFmt, c0 - 3, 0, c1 - 3, "Tom",	c2 - 3, 99, c3 - 3, "Lorem...")
			  << format_str(rowFmt, c0 - 3, 1, c1 - 3, "Dick",	c2 - 3, 50, c3 - 3, "Ipsum...")
			  << format_str(rowFmt, c0 - 3, 2, c1 - 3, "Harry", c2 - 3, 20, c3 - 3, "Lorem...");

	//std::cout << format_str("Cause an error: %m", 0);
	//
	// Line: 100 File: 'test.cpp'
	// String Format | Undefined format specifier : 'm'

	//std::cout << format_str("Cause an error: %i", 0.f);
	//
	// Line: 105 File: 'test.cpp'
	// String Format | Incorrect format specifier for type (float): Saw 'i' | Expected 'f, e, E, g, G, a, A'
	
	//std::cout << format_str("Cause an error: %");
	//
	// Line: 110 File: 'test.cpp'
	// String Format | Incomplete format string : Ended in '%'

	//std::cout << format_str("Cause an error: %d");
	//
	// Line: 115 File: 'test.cpp'
	// String Format | Not enough arguments

	//std::cout << format_str("Cause an error: %*d", 0);
	//
	// Line: 251 File: 'test.cpp'
	// String Format | Not enough arguments : Variable width needs '2' arguments. Have '1'
	
	//std::cout << format_str("Cause an error: %.*d", 0);
	//
	// Line: 256 File: 'test.cpp'
	// String Format | Not enough arguments : Variable precision needs '2' arguments. Have '1'

	//std::cout << format_str("Cause an error: %*.*d", 0);
	//
	// Line: 251 File: 'test.cpp'
	// String Format | Not enough arguments : Variable width & precision needs '3' arguments. Have '1'

	//std::cout << format_str("Cause an error: %*.*d", 0, 0);
	//
	// Line: 251 File: 'test.cpp'
	// String Format | Not enough arguments : Variable width & precision needs '3' arguments. Have '2'

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