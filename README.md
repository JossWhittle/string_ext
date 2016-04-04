# string_ext
## A C++11 implementation of `sprintf` with typechecking and error handling.

Implements `printf` http://www.cplusplus.com/reference/cstdio/printf/ style string formatting using C++ stringstreams and ios manipulators, returning the formatted result as a std::string. 

## Usage

Call to get a formatted string 
	
	str::format("...", args...)
	
Alternatively, call macro to pass along `__LINE__` and `__FILE__` for debugging
	
	format_str("...", args...)

	Equivalent to...
	str::format(__LINE__, __FILE__, "...", args...)

Formats follow the form:
	
	%[flags][width][.precision]specifier 

	Use %% for a percent sign

	flags	  : - (left align)
				+ (show sign)
				0 (pad with 0's)
				# (show base or decimal) 

	specifier : d, i, u, o, x, X       (short/int/long/long long & unsigned variants)
				f, F, e, E, g, G, a, A (float / double / long double)
				s                      (string / any type with an ostream<< operator), 
				c                      (char), 
				p, n                   (ptr / int* for string length)
	            b, B                   (bool)

## Error Handling

	std::cout << format_str("Cause an error: %m", 0);

	// Line: 100 File: 'test.cpp'
	// String Format | Undefined format specifier : 'm'

	std::cout << format_str("Cause an error: %i", 0.f);

	// Line: 105 File: 'test.cpp'
	// String Format | Incorrect format specifier for type (float): Saw 'i' | Expected 'f, F, e, E, g, G, a, A'
	
	std::cout << format_str("Cause an error: %");

	// Line: 110 File: 'test.cpp'
	// String Format | Incomplete format string : Ended in '%'

	std::cout << format_str("Cause an error: %d");

	// Line: 115 File: 'test.cpp'
	// String Format | Not enough arguments
	
	std::cout << format_str("Cause an error: ", 1, 2, 3);

	// Line: 120 File: 'test.cpp'
	// String Format | Unused arguments : '3'

	std::cout << format_str("Cause an error: %##d", 0);

	// Line: 125 File: 'test.cpp'
	// String Format | Format flag already set : '#'
