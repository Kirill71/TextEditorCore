#include "finder.hpp"

bool  searchPredicate(const std::string& current_string,
	const std::string& search_text, position* find_pos) noexcept
{
	++find_pos->m_row;
	find_pos->m_col = current_string.find(search_text, (find_pos->m_col != std::string::npos) ? find_pos->m_col : 0);
	return  find_pos->m_col != std::string::npos;
};

const position & Finder::find_base(const std::string & str, const position & max_pos, const Container& container, const position & pos)
{
	Utils::checkEmptyString(str, errorMessage::EMPTY_SEARCH_STRING);
	m_findText.setParams(pos, str);

	auto it = std::find_if(container.begin() + pos.m_row, container.end(),
		std::bind(&searchPredicate, std::placeholders::_1,
			str, &m_findText.lastPosition()));

	--m_findText.lastPosition().m_row; // return to c-style numeration from zero

	if (it == container.end())
		m_findText.lastPosition() = max_pos;

	return m_findText.lastPosition();
}

const position & Finder::find(const std::string & str, const position & max_pos, const Container & container)
{
	position const &  find_pos{ find_base(str, max_pos, container) };
	return  (find_pos != max_pos) ? find_pos : throw std::logic_error(errorMessage::TEXT_NOT_FOUND);
}

const position & Finder::findNext(const position& max_pos, const Container& container)
{
	return find_base(m_findText.searchString(), max_pos, container,
		position{ m_findText.lastPosition().m_row,
		m_findText.lastPosition().m_col + m_findText.searchString().length()});
}