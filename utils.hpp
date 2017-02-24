#ifndef UTILS_HPP
#define UTILS_HPP
#include<string>
#include"position.hpp"
struct Utils {
	static void  checkEmptyString(const std::string str,
		const std::string& exception_string);
};
#endif // !UTILS_HPP