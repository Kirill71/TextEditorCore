#include "finder.hpp"

const position & Finder::find_base(const std::string & str, const position & max_pos, const Container& container, const position & pos = { 0,0 })
{
	if (str.empty())
		throw std::logic_error(errorMessage::EMPTY_SEARCH_STRING);
	unsigned  col{};
	m_findText.searchString() = str;
	m_findText.lastPosition() = position{ max_pos };
	for (unsigned row {pos.m_row}; row < container.size(); ++row)
	{
		 col = container[row].find(str, /* for findNext()*/(row == pos.m_row) ? pos.m_col : 0);
		 if (col != std::string::npos) {
			 m_findText.lastPosition() = position{ row,col };
			 break;
		 }
	}
	return m_findText.lastPosition();
}

Finder::Finder() : m_findText{}{}

Finder::~Finder(){}

const position & Finder::find(const char * c_str, const position& max_pos, const Container& container)
{
	return find(std::string{ c_str }, max_pos, container);
}

const position & Finder::find(const std::string & str, const position & max_pos, const Container & container, const position& pos = {0,0})
{
	position const &  find_pos{ find_base(str, max_pos, container,pos) };
	return  (find_pos != max_pos) ? find_pos : throw std::logic_error(errorMessage::TEXT_NOT_FOUND);
}

const position & Finder::findNext(const position& max_pos, const Container& container)
{
	return find(m_findText.searchString(), max_pos, container,
		position{ m_findText.lastPosition().m_row,
		m_findText.lastPosition().m_col + m_findText.searchString().length()});
}

