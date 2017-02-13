#ifndef FINDER_HPP
#define FINDER_HPP
#include<string>
#include<functional>
#include<algorithm>
#include"position.hpp"
#include"message.hpp"
#include"utils.hpp"

class Finder
{
protected:
	struct findText {
		findText() = default;
		position& lastPosition() noexcept{
			return m_find.first;
		}

		std::string& searchString()  noexcept{
			return m_find.second;
		}
		void setParams(const position& pos, const std::string& find_text) noexcept {
			m_find.first = pos;
			m_find.second = find_text;
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
	//tested
	const position& find(const std::string& str, const position& max_pos, const Container& container);
	//tested
	const position& findNext(const position& max_pos, const Container& container);
};
#endif

