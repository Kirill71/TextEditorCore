#include "utils.hpp"

void Utils::checkEmptyString(const std::string str, const std::string & exception_string)
{
	if (str.empty())
		throw std::logic_error(exception_string);
}

bool Utils::searchPredicate(const std::string & current_string, const std::string & search_text, position * find_pos) noexcept
{
		++find_pos->m_row;
		find_pos->m_col = current_string.find(search_text, (find_pos->m_col != std::string::npos) ? find_pos->m_col : 0);
		return  find_pos->m_col != std::string::npos;
}
