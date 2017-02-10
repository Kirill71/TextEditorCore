#include "finder.hpp"

const position & Finder::find_base(const std::string & str, const position & max_pos, const Container& container, const position & pos = { 0,0 })
{
	if (str.empty())
		throw std::logic_error(errorMessage::EMPTY_SEARCH_STRING);

	m_findText.searchString() = str;
	m_findText.lastPosition() = pos;
	// predicate for algorithm find_if (lambda expression) 
	auto search_predicate = [](const std::string& current_string,
		const std::string& search_text, position* find_pos)
	{
		++find_pos->m_row;
		find_pos->m_col = current_string.find(search_text,(find_pos->m_col != std::string::npos)?find_pos->m_col:0);
		return  find_pos->m_col != std::string::npos;
	};

	auto it = std::find_if(container.begin() + pos.m_row , container.end(),
		std::bind(&search_predicate, std::placeholders::_1,
			str,&m_findText.lastPosition()));

	if (it == container.end()) 
		m_findText.lastPosition() = max_pos;

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

