#include "replacer.hpp"

void Replacer::replaceInContainer(const position & current_pos, Container & container, const std::string new_str)
{
	container[current_pos.m_row]
		.replace(current_pos.m_col, m_findText.searchString().length(), new_str);
}

bool Replacer::replace(const std::string & old_str, const std::string & new_str, const position& max_pos,  Container& container)
{
	m_isSuccessfully = false ;
	checkEmptyString(old_str, errorMessage::EMPTY_REPLACE_STRING);
	position const & current_pos = find_base(old_str, max_pos, container);
	if (current_pos != max_pos) {
		replaceInContainer(current_pos, container, new_str);
		m_isSuccessfully = true;
	}
	return m_isSuccessfully;
}

bool Replacer::replaceAll(const std::string & old_str, const std::string & new_str, const position& max_pos, Container& container)
{
	if (replace(old_str, new_str, max_pos, container)) {
		 position&  current_pos = const_cast<position&>(max_pos);

		while ((current_pos = findNext(max_pos, container)) != max_pos) {
			replaceInContainer(current_pos, container, new_str);
		}
	}
	return m_isSuccessfully;
}
