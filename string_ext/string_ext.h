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
		inline std::string _specString(int &val)						
		{ return "d, i, o, x, X"; };
		inline std::string _specString(short int &val)					
		{ return "d, i, o, x, X"; };
		inline std::string _specString(long int &val)					
		{ return "d, i, o, x, X"; };
		inline std::string _specString(long long int &val)				
		{ return "d, i, o, x, X"; };
		inline std::string _specString(unsigned int &val)				
		{ return "u, o, x, X"; };
		inline std::string _specString(unsigned short int &val)			
		{ return "u, o, x, X"; };
		inline std::string _specString(unsigned long int &val)			
		{ return "u, o, x, X"; };
		inline std::string _specString(unsigned long long int &val)		
		{ return "u, o, x, X"; };
		inline std::string _specString(float &val)						
		{ return "f, F, e, E, g, G, a, A"; };
		inline std::string _specString(double &val)						
		{ return "f, F, e, E, g, G, a, A"; };
		inline std::string _specString(long double &val)				
		{ return "f, F, e, E, g, G, a, A"; };
		inline std::string _specString(std::string &val)				
		{ return "s"; };
		template<typename T>
		inline std::string _specString(T &val)							
		{ return "s"; };
		inline std::string _specString(char &val)						
		{ return "c"; };
		inline std::string _specString(int *&val)						
		{ return "p, n"; };
		inline std::string _specString(unsigned int *&val)				
		{ return "p, n"; };
		template<typename T>
		inline std::string _specString(T *&val)							
		{ return "p"; };
		inline std::string _specString(bool &val)						
		{ return "b, B"; };

		/// Check if the given type matches the given specifier
		inline bool _checkVal(const char specifier, int &val)						
		{ return _containsChar(specifier, "dioxX"); };
		inline bool _checkVal(const char specifier, short int &val)					
		{ return _containsChar(specifier, "dioxX"); };
		inline bool _checkVal(const char specifier, long int &val)					
		{ return _containsChar(specifier, "dioxX"); };
		inline bool _checkVal(const char specifier, long long int &val)				
		{ return _containsChar(specifier, "dioxX"); };
		inline bool _checkVal(const char specifier, unsigned int &val)				
		{ return _containsChar(specifier, "uoxX"); };
		inline bool _checkVal(const char specifier, unsigned short int &val)		
		{ return _containsChar(specifier, "uoxX"); };
		inline bool _checkVal(const char specifier, unsigned long int &val)			
		{ return _containsChar(specifier, "uoxX"); };
		inline bool _checkVal(const char specifier, unsigned long long int &val)	
		{ return _containsChar(specifier, "uoxX"); };
		inline bool _checkVal(const char specifier, float &val)						
		{ return _containsChar(specifier, "fFeEgGaA"); };
		inline bool _checkVal(const char specifier, double &val)					
		{ return _containsChar(specifier, "fFeEgGaA"); };
		inline bool _checkVal(const char specifier, long double &val)				
		{ return _containsChar(specifier, "fFeEgGaA"); };
		inline bool _checkVal(const char specifier, std::string &val)				
		{ return specifier == 's'; };
		template<class T>
		inline bool _checkVal(const char specifier, T &val)							
		{ return specifier == 's'; };
		inline bool _checkVal(const char specifier, char &val)						
		{ return specifier == 'c'; };
		inline bool _checkVal(const char specifier, int *&val)						
		{ return _containsChar(specifier, "pn"); };
		inline bool _checkVal(const char specifier, unsigned int *&val)				
		{ return _containsChar(specifier, "pn"); };
		template<typename T>
		inline bool _checkVal(const char specifier, T *&val)						
		{ return specifier == 'p'; };
		inline bool _checkVal(const char specifier, bool &val)						
		{ return _containsChar(specifier, "bB"); };

		/// Structure to hold formatting info
		struct _Format {
			char specifier;
			bool leftJustify, forceSign, forceLong, padZeros;
			unsigned int width, precision;
			_Format() : specifier(0),
				leftJustify(false), forceSign(false),
				forceLong(false), padZeros(false),
				width(0), precision(0) {};
		};

		/**
		* Munches a format declaration starting at pos, assuming *pos == '%'
		* @param _line_		Pass along the debug macro __LINE__ from the call site
		* @param _file_		Pass along the debug macro __FILE__ from the call site
		* @param pos		Iterator to the current position in the format string
		* @param fmtE		Iterator to the end of the format string
		* @return			Returns a _Format object representing the munched format declaration
		*/
		inline _Format _parseFormat(const int _line_, const char *_file_,
			std::string::const_iterator &pos,
			std::string::const_iterator &fmtE) {

			_Format fmt;
			/// Assume pos starts on '%'
			int mode = 0;
			while ((++pos) != fmtE) {
				if (mode == 0) {
					/// Flags
					auto fmtErr = [&]() {
						_printDebug(_line_, _file_);
						std::cerr << "String Format | Format flag already set: '" << *pos << '\'' << std::endl << std::endl;
						std::exit(EXIT_FAILURE);
					};

					switch (*pos) {
					case '-':
						if (fmt.leftJustify) fmtErr();
						fmt.leftJustify = true;
						break;
					case '+':
						if (fmt.forceSign) fmtErr();
						fmt.forceSign = true;
						break;
					case '0':
						if (fmt.padZeros) fmtErr();
						fmt.padZeros = true;
						break;
					case '#':
						if (fmt.forceLong) fmtErr();
						fmt.forceLong = true;
						break;
					default:
						pos--;
						mode++;
					}
					continue;
				}
				else if (mode == 1) {
					/// Width - Munch number
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
					/// Precision
					if (*pos == '.') {
						/// Munch number
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
					/// Specifier
					fmt.specifier = *pos;
					pos++;
					break;
				}
			}

			/// Check for invalid specifier
			if (!_containsChar(fmt.specifier, "diuoxXfFeEgGaAscpnbB")) {
				_printDebug(_line_, _file_);
				std::cerr << "Undefined format specifier: '"
					<< _escape(fmt.specifier) << '\'' << std::endl << std::endl;
				std::exit(EXIT_FAILURE);
			}
			return fmt;
		};

		/**
		* Formats the current argument. From pos munch the format declaration and insert formatted val into the stream
		* @param _line_		Pass along the debug macro __LINE__ from the call site
		* @param _file_		Pass along the debug macro __FILE__ from the call site
		* @param ret		The ostream to write output to
		* @param pos		Iterator to the current position in the format string
		* @param fmtE		Iterator to the end of the format string
		* @param val		The next value to insert into fmt
		*/
		template<typename T>
		inline void _formatVal(const int _line_, const char *_file_,
			std::ostringstream &ret,
			std::string::const_iterator &pos,
			std::string::const_iterator &fmtE,
			T &val) {

			/// Modifies pos as it munches the formatting declaration!
			_Format f = _parseFormat(_line_, _file_, pos, fmtE);

			/// Provide typechecking on formatting declaration because we know the type of val
			if (!_checkVal(f.specifier, val)) {
				_printDebug(_line_, _file_);
				std::cerr << "String Format | Incorrect format specifier: Saw '" << f.specifier
					<< "' | Expected '" << _specString(val) << '\'' << std::endl << std::endl;
				std::exit(EXIT_FAILURE);
			}

			/// Cache stream state before formatting
			std::ios state(NULL);
			state.copyfmt(ret);

			/// Apply flags/width/precision
			if (f.forceSign)				ret << std::showpos;
			if (f.forceLong)				ret << std::showpoint << std::showbase;
			if (f.padZeros)					ret << std::setfill('0');
			if (f.leftJustify)				ret << std::left;
			if (f.width > 0)				ret << std::setw(f.width);
			if (f.precision > 0)			ret << std::setprecision(f.precision);
			if (std::isupper(f.specifier))	ret << std::uppercase;

			/// Specifier specific formatting
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
				{ /// Special case: Need to reinterpret val to apply logic
					bool *ptr = reinterpret_cast<bool*>(&val);
					if (f.forceLong) {
						ret << (*ptr ? "true" : "false");
					}
					else {
						ret << (*ptr ? '1' : '0');
					}
					ret.copyfmt(state); /// Reset stream state to before _formatVal
					return;
				}
			case 'B':
				{ /// Special case: Need to reinterpret val to apply logic
					bool *ptr = reinterpret_cast<bool*>(&val);
					if (f.forceLong) {
						ret << (*ptr ? "TRUE" : "FALSE");
					}
					else {
						ret << (*ptr ? 'T' : 'F');
					}
					ret.copyfmt(state); /// Reset stream state to before _formatVal
					return;
				}
			case 'p':
				{ /// Special case: Need to reinterpret val to format as uint:hex
					ret << std::showbase << std::hex;
					unsigned int *ptr = reinterpret_cast<unsigned int*>(&val);
					ret << *ptr;
					ret.copyfmt(state); /// Reset stream state to before _formatVal
					return;
				}
			case 's':
				{ /// Special case: Use intermediate stream so ostream operators respect formatting
					std::ostringstream ss;
					ss << val;
					const std::string s = ss.str();
					if (f.precision == 0 || f.precision >= s.size()) {
						ret << s;
					}
					else {
						ret << s.substr(0, f.precision);
					}
					ret.copyfmt(state); /// Reset stream state to before _formatVal
					return;
				}
			case 'n':
				{ /// Special case: Need to reinterpret val to insert current string length
					unsigned int **ptr = reinterpret_cast<unsigned int**>(&val);
					**ptr = (unsigned int) ret.str().size();
					ret.copyfmt(state); /// Reset stream state to before _formatVal
					return;
				}
			}

			ret << val;
			ret.copyfmt(state); /// Reset stream state to before _formatVal
		};

		/**
		* Formats the current argument. Special case, treat char* as std::string
		* @param _line_		Pass along the debug macro __LINE__ from the call site
		* @param _file_		Pass along the debug macro __FILE__ from the call site
		* @param ret		The ostream to write output to
		* @param pos		Iterator to the current position in the format string
		* @param fmtE		Iterator to the end of the format string
		* @param val		The next value to insert into fmt
		*/
		inline void _formatVal(const int _line_, const char *_file_,
			std::ostringstream &ret,
			std::string::const_iterator &pos,
			std::string::const_iterator &fmtE,
			const char *val) {

			/// Transparently convert c-string to std::string
			_formatVal(_line_, _file_, ret, pos, fmtE, std::string(val));
		};

		/**
		* Recursively munches through the format string when no arguments are left/present
		* @param _line_		Pass along the debug macro __LINE__ from the call site
		* @param _file_		Pass along the debug macro __FILE__ from the call site
		* @param ret		The ostream to write output to
		* @param fmtS		Iterator to the start of the format string
		* @param fmtE		Iterator to the end of the format string
		*/
		inline void _format(const int _line_, const char *_file_,
			std::ostringstream &ret,
			std::string::const_iterator &fmtS,
			std::string::const_iterator &fmtE) {

			auto pos = std::find(fmtS, fmtE, '%');
			if (pos != fmtE) {
				/// Grab fmt before delimiter
				ret << std::string(fmtS, pos);

				const auto next = pos + 1;
				if (next == fmtE) {
					/// If fmt ends then there was an incomplete format declaration
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
					/// If this is actually a format declaration then we don't have any args to insert 
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

		/// Count the number of varadic template arguments
		inline unsigned int _numArgs() { return 0u; };
		template<typename T, typename ...Args>
		inline unsigned int _numArgs(T &val, Args ...args) { return 1u + _numArgs(args...); };

		/**
		* Recursively munches through the format string inserting the set of arguments
		* @param _line_		Pass along the debug macro __LINE__ from the call site
		* @param _file_		Pass along the debug macro __FILE__ from the call site
		* @param ret		The ostream to write output to
		* @param fmtS		Iterator to the start of the format string
		* @param fmtE		Iterator to the end of the format string
		* @param val		The next value to insert into fmt
		* @param ...args	The set of remaining arguments to insert into fmt
		*/
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

				_printDebug(_line_, _file_);
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

	}; /// imp namespace

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

}; /// str namespace