#include "finder.hpp"

const position & Finder::find_base(const std::string & str, const position & max_pos, const MyContainer& container, const position & pos){
	Utils::checkEmptyString(str, errorMessage::EMPTY_SEARCH_STRING);
	m_findText.setParams(pos, str);

	auto it = std::find_if(container.begin() + pos.m_row, container.end(),
		std::bind([](const std::string & current_string, const std::string & search_text, position * find_pos) {
		++find_pos->m_row;
		find_pos->m_col = current_string.find(search_text, (find_pos->m_col != std::string::npos) ? find_pos->m_col : 0);
		return  find_pos->m_col != std::string::npos;
	} , std::placeholders::_1,
			str, &m_findText.lastPosition()));

	--m_findText.lastPosition().m_row; // return to c-style numeration from zero

	if (it == container.end())
		m_findText.lastPosition() = max_pos;

	return m_findText.lastPosition();
}

const position & Finder::find(const std::string & str, const position & max_pos, const MyContainer & container){
	return  (find_base(str, max_pos, container) != max_pos) ? m_findText.lastPosition() : max_pos;
}

const position & Finder::findNext(const position& max_pos, const MyContainer& container, bool is_replace){
	auto length{ (is_replace) ? 0 : m_findText.searchString().length() };
	return find_base(m_findText.searchString(), max_pos, container,
		position{ m_findText.lastPosition().m_row,
		m_findText.lastPosition().m_col + length});
}