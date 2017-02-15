#ifndef UTILS_HPP
#define UTILS_HPP
#include<string>
#include"position.hpp"
struct Utils {
	static void  checkEmptyString(const std::string str,
		const std::string& exception_string);

	static bool  searchPredicate(const std::string& current_string,
		const std::string& search_text, position* find_pos) noexcept;
};
#endif // !UTILS_HPP
