# string_ext
## A C++11, header-only implementation of `sprintf` with typechecking and error handling.

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

	specifier : d, i, u, o, x, X, n    (short/int/long/long long & unsigned variants)
				f, e, E, g, G, a, A    (float / double / long double)
				s                      (string / any type with an ostream<< operator), 
				c                      (char), 
				p                      (ptr)
	            b, B                   (bool)


## Examples

Everything is just like it is with printf! But now you get error handling, typechecking, security, and dynamic memory which means no more buffer overruns!

	/// Writing to stdout
	printf("Hello world.\n");
	// vs.
	std::cout << format_str("Hello world.\n");

---

	/// Writing to stderr
	fprintf(stderr, "Hello world.\n");
	// vs.
	std::cerr << format_str("Hello world.\n");

---

	/// Writing to a string buffer
	char buf[256];
	sprintf(buf, "a = %d b = %d c = %d\n", 1, 2, 3);
	// vs.
	std::string buf = format_str("a = %d b = %d c = %d\n", 1, 2, 3);

---

	/// Print a struct or class object that has an ostream<< operator
	struct SomeObject {
		int a;
		double d;

		/// The ostream operator can itself call format_str !
		friend std::ostream& operator<<(std::ostream& os, const Test &v) {
			os << format_str("SomeObject{ %i, %f }", v.a, v.d);
			return os;
		};
	};
	
	SomeObject obj{ 10, 3.14 };
	std::cout << format_str("Here is an object: %s\n", obj);

## Error Handling

	std::cout << format_str("Cause an error: %m", 0);
	
	// Line: 100 File: 'test.cpp'
	// String Format | Undefined format specifier : 'm'

	std::cout << format_str("Cause an error: %i", 0.f);
	
	// Line: 105 File: 'test.cpp'
	// String Format | Incorrect format specifier for type (float): Saw 'i' | Expected 'f, e, E, g, G, a, A'
	
	std::cout << format_str("Cause an error: %");
	
	// Line: 110 File: 'test.cpp'
	// String Format | Incomplete format string : Ended in '%'

	std::cout << format_str("Cause an error: %d");
	
	// Line: 115 File: 'test.cpp'
	// String Format | Not enough arguments

	std::cout << format_str("Cause an error: %*d", 0);
	
	// Line: 251 File: 'test.cpp'
	// String Format | Not enough arguments : Variable width needs '2' arguments. Have '1'
	
	std::cout << format_str("Cause an error: %.*d", 0);
	
	// Line: 256 File: 'test.cpp'
	// String Format | Not enough arguments : Variable precision needs '2' arguments. Have '1'

	std::cout << format_str("Cause an error: %*.*d", 0);
	
	// Line: 251 File: 'test.cpp'
	// String Format | Not enough arguments : Variable width & precision needs '3' arguments. Have '1'

	std::cout << format_str("Cause an error: %*.*d", 0, 0);
	
	// Line: 251 File: 'test.cpp'
	// String Format | Not enough arguments : Variable width & precision needs '3' arguments. Have '2'

	std::cout << format_str("Cause an error: %*d", 'a', 0);
	
	// Line: 271 File: 'test.cpp'
	// String Format | Invalid width argument : (char)

	std::cout << format_str("Cause an error: %.*d", 3.14L, 0);

	// Line: 276 File: 'test.cpp'
	// String Format | Invalid precision argument : (long double)

	std::cout << format_str("Cause an error: ", 1, 2, 3);
	
	// Line: 120 File: 'test.cpp'
	// String Format | Unused arguments : '3'

	std::cout << format_str("Cause an error: %##d", 0);
	
	// Line: 125 File: 'test.cpp'
	// String Format | Format flag already set: 'Force Long'
