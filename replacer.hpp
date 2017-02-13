#ifndef REPLACER_HPP
#define REPLACER_HPP
#include"finder.hpp"

class Replacer
	: public Finder
{
	bool m_isSuccessfully;
	void replaceInContainer(const position& current_pos, Container& container, const std::string new_str);
public:
	Replacer() = default; // call Finder ctor on default
	Replacer(const Replacer& rhs) = delete;
	Replacer& operator=(const Replacer&) = delete;
	Replacer(const Replacer&& rhs) = delete;
	Replacer& operator=(const Replacer&&) = delete;
	~Replacer() = default;

	bool replace(const std::string & old_str, const std::string & new_str, const position& max_pos, Container& container);
	bool replaceAll(const std::string& old_str, const std::string& new_str, const position& max_pos, Container& container);
};
#endif // !REPLACER_HPP

