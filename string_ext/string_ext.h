#pragma once

#include <cstdio>
#include <cstdarg>
#include <cctype>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>
#include <utility>
#include <type_traits>
#include <cassert>

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

inline _Format _parseFormat(std::string::const_iterator &pos, std::string::const_iterator &fmtE) {
	_Format fmt;
	/// Assume pos starts on '%'
	int mode = 0;
	while ((++pos) != fmtE) {
		if (mode == 0) {
			/// Flags
			if (*pos == '-') {
				fmt.leftJustify = true;
				//std::cout << "LEFTJUSTIFY: " << std::endl;
			}
			else if (*pos == '+') {
				fmt.forceSign = true;
				//std::cout << "FORCESIGN: " << std::endl;
			}
			else if (*pos == '0') {
				fmt.padZeros = true;
				//std::cout << "PADZEROS: " << std::endl;
			}
			else if (*pos == '#') {
				fmt.forceLong = true;
				//std::cout << "FORCELONG: " << std::endl;
			}
			else {
				pos--;
				mode++;
				continue;
			}
		}
		else if (mode == 1) {
			// Width
			auto n = pos;
			while (n != fmtE && std::isdigit(*n)) n++; 
			if (n != pos) {
				fmt.width = std::stoi(std::string(pos, n));
				//std::cout << "WIDTH: " << fmt.width << std::endl;
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
					//std::cout << "PRECISION: " << fmt.precision << std::endl;
					pos = n - 1;
				}
				else {
					pos--;
				}
				mode++;
				continue;
			}
			else {
				pos--;
				mode++;
				continue;
			}
		}
		else if (mode == 3) {
			// Specifier
			fmt.specifier = *pos;
			//std::cout << "SPECIFIER: " << fmt.specifier << std::endl;
			pos++;
			break;
		}
	}
	return fmt;
};

inline bool _checkVal(char specifier, int &val) {
	return specifier == 'd' || specifier == 'i' || specifier == 'o' || specifier == 'x' || specifier == 'X';
};
inline bool _checkVal(char specifier, unsigned int &val) {
	return specifier == 'u' || specifier == 'o' || specifier == 'x' || specifier == 'X';
};
inline bool _checkVal(char specifier, float &val) {
	return specifier == 'f' || specifier == 'F' || specifier == 'e' || specifier == 'E' || specifier == 'g' || specifier == 'G' || specifier == 'a' || specifier == 'A';
};
inline bool _checkVal(char specifier, double &val) {
	return specifier == 'f' || specifier == 'F' || specifier == 'e' || specifier == 'E' || specifier == 'g' || specifier == 'G' || specifier == 'a' || specifier == 'A';
};
inline bool _checkVal(char specifier, long double &val) {
	return specifier == 'f' || specifier == 'F' || specifier == 'e' || specifier == 'E' || specifier == 'g' || specifier == 'G' || specifier == 'a' || specifier == 'A';
};
inline bool _checkVal(char specifier, std::string &val) {
	return specifier == 's';
};

template<class T>
inline bool _checkVal(char specifier, T &val) {
	return specifier == 's';
};

inline bool _checkVal(char specifier, char &val) {
	return specifier == 'c';
};
template<typename T>
inline bool _checkVal(char specifier, T *&val) {
	return specifier == 'p';
};

template<typename T>
inline void _formatVal(std::stringstream &ret, std::string::const_iterator &pos, std::string::const_iterator &fmtE, T &val) {
	/// Modifies pos as it munches the formatting declaration!
	/// Provides typechecking on formatting declaration because we know the type of val
	_Format f = _parseFormat(pos, fmtE);
	
	assert(_checkVal(f.specifier, val) && "Invalid specifier for type!");

	std::ios state(NULL);
	state.copyfmt(ret);

	ret << std::boolalpha;
	if (f.forceSign) ret << std::showpos;
	if (f.forceLong) ret << std::showpoint << std::showbase;
	if (f.padZeros)  ret << std::setfill('0'); 
	if (f.leftJustify)  ret << std::left;
	if (f.width > 0) ret << std::setw(f.width);
	if (f.precision > 0) ret << std::setprecision(f.precision);

	switch (f.specifier) {
	case 'o': 
		ret << std::oct;
		if (f.forceLong) ret << 0;
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
	case 'p':
		ret << std::hex;
		ret << val;
		ret.copyfmt(state);
		return;
	}
	ret << val;
	ret.copyfmt(state);
};

inline void _formatVal(std::stringstream &ret, std::string::const_iterator &pos, std::string::const_iterator &fmtE, const char *val) {
	/// Transparently convert c-string to std::string
	_formatVal(ret, pos, fmtE, std::string(val));
};

inline void _format(std::stringstream &ret, std::string::const_iterator &fmtS, std::string::const_iterator &fmtE) {
	/// No args, print remaining fmt string
	ret << std::string(fmtS, fmtE);
};

template<typename T, typename ...Args>
inline void _format(std::stringstream &ret, std::string::const_iterator &fmtS, std::string::const_iterator &fmtE, T &val, Args ...args) {
	/// Find the next format delimiter
	auto pos = std::find(fmtS, fmtE, '%');
	if (pos == fmtE) {
		/// If no delimiter is found print the rest of the fmt string blindly
		_format(ret, fmtS, fmtE); //  remaining args... are discarded
	}
	else {
		/// Grab fmt before delimiter
		ret << std::string(fmtS, pos);

		const auto next = pos + 1;
		if ((next != fmtE) && (*next == '%')) {
			ret << '%';
			pos += 2;
			/// Formate the remaining string
			_format(ret, pos, fmtE, val, args...);
		}	
		else {
			/// Format current argument
			_formatVal(ret, pos, fmtE, val);
			/// Formate the remaining string
			_format(ret, pos, fmtE, args...);
		}
	}
};

template<typename ...Args>
inline std::string format(const std::string &fmt, Args ...args) {
	/// This is the public interface to the formatter
	std::stringstream ret;
	_format(ret, fmt.begin(), fmt.end(), args...);
	return ret.str();
};