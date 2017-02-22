#include "utils.hpp"

void Utils::checkEmptyString(const std::string str, const std::string & exception_string){
	if (str.empty())
		throw std::logic_error(exception_string);
}