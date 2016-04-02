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

#pragma once

#include <iostream>
#include <cctype>
#include <string>
#include <sstream>
#include <iomanip>
#include <cassert>

namespace str { /// Main namespace

	namespace imp { /// Implementation namespace
		
		/// Print line info for debugging
		inline void _printDebug(const int _line_, const char *_file_) {
			if (!(_line_ == -1 || _file_ == nullptr)) 
				std::cerr << "Line: " << _line_ << " File: '" << _file_ << '\'' << std::endl;
		};

		/// Check if string contains specifier
		inline bool _containsChar(const char specifier, const std::string &str) {
			return std::find(str.begin(), str.end(), specifier) != str.end();
		};

		/// Escape char for debugging
		inline std::string _escape(const char specifier) {
			switch (specifier) {
			case '\n':
				return "\\n";
			case '\r':
				return "\\r";
			case '\t':
				return "\\t";
			case '\0':
				return "\\0";
			}
			return std::string(1, specifier);
		};

		/// Get the valid specifiers for a given type
		inline std::string _specString(int &val) {
			return "d, i, o, x, X";
		};
		inline std::string _specString(unsigned int &val) {
			return "u, o, x, X";
		};
		inline std::string _specString(float &val) {
			return "f, F, e, E, g, G, a, A";
		};
		inline std::string _specString(double &val) {
			return "f, F, e, E, g, G, a, A";
		};
		inline std::string _specString(long double &val) {
			return "f, F, e, E, g, G, a, A";
		};
		inline std::string _specString(std::string &val) {
			return "s";
		};
		template<typename T>
		inline std::string _specString(T &val) {
			return "s";
		};
		inline std::string _specString(char &val) {
			return "c";
		};
		template<typename T>
		inline std::string _specString(T *&val) {
			return "p";
		};
		inline std::string _specString(bool &val) {
			return "b, B";
		};

		/// Check if the given type matches the given specifier
		inline bool _checkVal(const char specifier, int &val) {
			return _containsChar(specifier, "dioxX");
		};
		inline bool _checkVal(const char specifier, unsigned int &val) {
			return _containsChar(specifier, "uoxX");
		};
		inline bool _checkVal(const char specifier, float &val) {
			return _containsChar(specifier, "fFeEgGaA");
		};
		inline bool _checkVal(const char specifier, double &val) {
			return _containsChar(specifier, "fFeEgGaA");
		};
		inline bool _checkVal(const char specifier, long double &val) {
			return _containsChar(specifier, "fFeEgGaA");
		};
		inline bool _checkVal(const char specifier, std::string &val) {
			return specifier == 's';
		};
		template<class T>
		inline bool _checkVal(const char specifier, T &val) {
			return specifier == 's';
		};
		inline bool _checkVal(const char specifier, char &val) {
			return specifier == 'c';
		};
		template<typename T>
		inline bool _checkVal(const char specifier, T *&val) {
			return specifier == 'p';
		};
		inline bool _checkVal(const char specifier, bool &val) {
			return _containsChar(specifier, "bB");
		};

		/// Structure to hold formatting info
		struct _Format {
			char specifier;
			bool leftJustify;
			bool forceSign;
			bool forceLong;
			bool padZeros;
			unsigned int width;
			unsigned int precision;
			_Format() : specifier(0),
				leftJustify(false), forceSign(false),
				forceLong(false), padZeros(false),
				width(0), precision(0) {};
		};

		inline _Format _parseFormat(const int _line_, const char *_file_,
			std::string::const_iterator &pos,
			std::string::const_iterator &fmtE) {
			_Format fmt;
			/// Assume pos starts on '%'
			int mode = 0;
			while ((++pos) != fmtE) {
				if (mode == 0) {
					/// Flags
					switch (*pos) {
					case '-':
						if (fmt.leftJustify) {
							_printDebug(_line_, _file_);
							std::cerr << "String Format | Format flag already set: '" << *pos << '\'' << std::endl << std::endl;
							std::exit(EXIT_FAILURE);
						}
						fmt.leftJustify = true;
						break;
					case '+':
						if (fmt.forceSign) {
							_printDebug(_line_, _file_);
							std::cerr << "String Format | Format flag already set: '" << *pos << '\'' << std::endl << std::endl;
							std::exit(EXIT_FAILURE);
						}
						fmt.forceSign = true;
						break;
					case '0':
						if (fmt.padZeros) {
							_printDebug(_line_, _file_);
							std::cerr << "String Format | Format flag already set: '" << *pos << '\'' << std::endl << std::endl;
							std::exit(EXIT_FAILURE);
						}
						fmt.padZeros = true;
						break;
					case '#':
						if (fmt.forceLong) {
							_printDebug(_line_, _file_);
							std::cerr << "String Format | Format flag already set: '" << *pos << '\'' << std::endl << std::endl;
							std::exit(EXIT_FAILURE);
						}
						fmt.forceLong = true;
						break;
					default:
						pos--;
						mode++;
					}
					continue;
				}
				else if (mode == 1) {
					// Width
					auto n = pos;
					while (n != fmtE && std::isdigit(*n)) n++;
					if (n != pos) {
						fmt.width = std::stoi(std::string(pos, n));
						pos = n - 1;
					}
					else {
						pos--;
					}
					mode++;
					continue;
				}
				else if (mode == 2) {
					// Precision
					if (*pos == '.') {
						pos++;
						auto n = pos;
						while (n != fmtE && std::isdigit(*n)) n++;
						if (n != pos) {
							fmt.precision = std::stoi(std::string(pos, n));
							pos = n - 1;
						}
						else {
							pos--;
						}
						mode++;
					}
					else {
						pos--;
						mode++;
					}
					continue;
				}
				else if (mode == 3) {
					// Specifier
					fmt.specifier = *pos;
					pos++;
					break;
				}
			}

			if (!_containsChar(fmt.specifier, "diuoxXfFeEgGaAscpbB")) {
				_printDebug(_line_, _file_);
				std::cerr << "Undefined format specifier: '"
					<< _escape(fmt.specifier) << '\'' << std::endl << std::endl;
				std::exit(EXIT_FAILURE);
			}
			return fmt;
		};

		template<typename T>
		inline void _formatVal(const int _line_, const char *_file_,
			std::ostringstream &ret,
			std::string::const_iterator &pos,
			std::string::const_iterator &fmtE,
			T &val) {

			/// Modifies pos as it munches the formatting declaration!
			/// Provides typechecking on formatting declaration because we know the type of val
			_Format f = _parseFormat(_line_, _file_, pos, fmtE);

			if (!_checkVal(f.specifier, val)) {
				_printDebug(_line_, _file_);
				std::cerr << "String Format | Incorrect format specifier: Saw '" << f.specifier
					<< "' | Expected '" << _specString(val) << '\'' << std::endl << std::endl;
				std::exit(EXIT_FAILURE);
			}

			std::ios state(NULL);
			state.copyfmt(ret);

			if (f.forceSign)				ret << std::showpos;
			if (f.forceLong)				ret << std::showpoint << std::showbase;
			if (f.padZeros)					ret << std::setfill('0');
			if (f.leftJustify)				ret << std::left;
			if (f.width > 0)				ret << std::setw(f.width);
			if (f.precision > 0)			ret << std::setprecision(f.precision);
			if (std::isupper(f.specifier))	ret << std::uppercase;

			switch (f.specifier) {
			case 'o':
				ret << std::oct;
				break;
			case 'x':
			case 'X':
				ret << std::hex;
				break;
			case 'a':
			case 'A':
				ret << std::hexfloat;
				break;
			case 'e':
			case 'E':
				ret << std::scientific;
				break;
			case 'b':
			{
				bool *ptr = reinterpret_cast<bool*>(&val);
				if (f.forceLong) {
					ret << (*ptr ? "true" : "false");
				}
				else {
					ret << (*ptr ? "1" : "0");
				}
				ret.copyfmt(state);
				return;
			}
			case 'B':
			{
				bool *ptr = reinterpret_cast<bool*>(&val);
				if (f.forceLong) {
					ret << (*ptr ? "TRUE" : "FALSE");
				}
				else {
					ret << (*ptr ? "T" : "F");
				}
				ret.copyfmt(state);
				return;
			}
			case 'p':
			{
				ret << std::showbase << std::hex;
				unsigned int *ptr = reinterpret_cast<unsigned int*>(&val);
				ret << *ptr;
				ret.copyfmt(state);
				return;
			}
			}
			ret << val;
			ret.copyfmt(state);
		};

		inline void _formatVal(const int _line_, const char *_file_,
			std::ostringstream &ret,
			std::string::const_iterator &pos,
			std::string::const_iterator &fmtE,
			const char *val) {

			/// Transparently convert c-string to std::string
			_formatVal(_line_, _file_, ret, pos, fmtE, std::string(val));
		};

		/// Count the number of varadic template arguments
		inline unsigned int _numArgs() { return 0u; };
		template<typename T, typename ...Args>
		inline unsigned int _numArgs(T &val, Args ...args) { return 1u + _numArgs(args...); };

		inline void _format(const int _line_, const char *_file_,
			std::ostringstream &ret,
			std::string::const_iterator &fmtS,
			std::string::const_iterator &fmtE) {

			auto pos = std::find(fmtS, fmtE, '%');
			if (pos != fmtE) {
				/// If delimiter is found then there were not enough arguments
				const auto next = pos + 1;
				if (next == fmtE) {
					_printDebug(_line_, _file_);
					std::cerr << "String Format | Incomplete format string: Ended in '%'" << std::endl << std::endl;
					std::exit(EXIT_FAILURE);
				}
				else if (*next == '%') {
					/// Special case for % sign
					ret << '%';
					pos += 2;
					/// Formate the remaining string
					_format(_line_, _file_, ret, pos, fmtE);
				}
				else {
					_printDebug(_line_, _file_);
					std::cerr << "String Format | Not enough arguments" << std::endl << std::endl;
					std::exit(EXIT_FAILURE);
				}
			}
			else {
				/// No args, print remaining fmt string
				ret << std::string(fmtS, fmtE);
			}
		};

		template<typename T, typename ...Args>
		inline void _format(const int _line_, const char *_file_,
			std::ostringstream &ret,
			std::string::const_iterator &fmtS,
			std::string::const_iterator &fmtE,
			T &val, Args ...args) {

			/// Find the next format delimiter
			auto pos = std::find(fmtS, fmtE, '%');
			if (pos == fmtE) {
				/// If no delimiter is found print the rest of the fmt string blindly
				//_format(_line_, _file_, ret, fmtS, fmtE); //  remaining args... are discarded

				if (!(_line_ == -1 || _file_ == nullptr)) {
					std::cerr << "Line: " << _line_ << " File: '" << _file_ << '\'' << std::endl;
				}
				std::cerr << "String Format | Unused arguments: '"
					<< (1u + _numArgs(args...)) << '\'' << std::endl << std::endl;
				std::exit(EXIT_FAILURE);
			}
			else {
				/// Grab fmt before delimiter
				ret << std::string(fmtS, pos);

				const auto next = pos + 1;
				if (next == fmtE) {
					_printDebug(_line_, _file_);
					std::cerr << "String Format | Incomplete format string: Ended in '%'" << std::endl << std::endl;
					std::exit(EXIT_FAILURE);
				}
				else if (*next == '%') {
					/// Special case for % sign
					ret << '%';
					pos += 2;
					/// Formate the remaining string
					_format(_line_, _file_, ret, pos, fmtE, val, args...);
				}
				else {
					/// Format current argument
					_formatVal(_line_, _file_, ret, pos, fmtE, val);
					/// Formate the remaining string
					_format(_line_, _file_, ret, pos, fmtE, args...);
				}
			}
		};

	}; // imp namespace

	/// Public interface
	#define format_str(...) str::format(__LINE__, __FILE__, __VA_ARGS__)

	/**
	 * Formats a string using the set of provided varadic template arguments
	 * @param _line_	Pass along the debug macro __LINE__ from the call site
	 * @param _file_	Pass along the debug macro __FILE__ from the call site
	 * @param fmt		The format string to use
	 * @param ...args	The set of arguments to insert into fmt
	 * @return			The string fmt with args formatted and inserted where specified
	 */
	template<typename ...Args>
	inline std::string format(const int _line_, const char *_file_, const std::string &fmt, Args ...args) {
		std::ostringstream ret;
		str::imp::_format(_line_, _file_, ret, fmt.begin(), fmt.end(), args...);
		return ret.str();
	};

	/**
	* Formats a string using the set of provided varadic template arguments (without call site debug info)
	* @param fmt		The format string to use
	* @param ...args	The set of arguments to insert into fmt
	* @return			The string fmt with args formatted and inserted where specified
	*/
	template<typename ...Args>
	inline std::string format(const std::string &fmt, Args ...args) {
		return str::format(-1, nullptr, fmt, args...);
	};

}; // str namespace