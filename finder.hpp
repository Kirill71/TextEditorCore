#ifndef FINDER_HPP
#define FINDER_HPP
#include<string>
#include"position.hpp"
#include"message.hpp"
class Finder
{
	struct findText {
		findText() : m_find{} {}
		position& lastPosition() {
			return m_find.first;
		}

		std::string& searchString() {
			return m_find.second;
		}

	private:
		std::pair<position, std::string> m_find;
	}m_findText;
	const position& find_base(const std::string & str, const position & max_pos, const Container& container, const position & pos = { 0,0 });
public:
	Finder();
	virtual ~Finder();

	const position& find(const char * c_str, const position& max_pos, const Container& container);
	const position& find(const std::string& str, const position& max_pos, const Container& container, const position& pos = { 0,0 });
	const position& findNext(const position& max_pos, const Container& container);
};
#endif

