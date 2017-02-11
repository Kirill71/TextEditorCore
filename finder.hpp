#ifndef FINDER_HPP
#define FINDER_HPP
#include<string>
#include<functional>
#include<algorithm>
#include"position.hpp"
#include"message.hpp"

class Finder
{
protected:
	struct findText {
		findText() = default;
		position& lastPosition() {
			return m_find.first;
		}

		std::string& searchString() {
			return m_find.second;
		}
	private:
		std::pair<position, std::string> m_find;
	} m_findText;
	const position& find_base(const std::string & str, const position & max_pos, const Container& container, const position & pos = { 0,0 });
public:
	Finder() = default;
	Finder(const Finder& rhs) = delete;
	Finder& operator=(const Finder&) = delete;
	Finder(const Finder&& rhs) = delete;
	Finder& operator=(const Finder&&) = delete;
	virtual ~Finder() = default;

	const position& find(const std::string& str, const position& max_pos, const Container& container);
	const position& findNext(const position& max_pos, const Container& container);
};
#endif

