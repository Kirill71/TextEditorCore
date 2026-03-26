#include "utils.hpp"

#include <stdexcept>

/*---------------------------------------------------------------------------*/


void 
Utils::checkEmptyString(
		const std::string& _str
	,	const std::string & _exceptionString)
{
	if (_str.empty())
		throw std::logic_error( _exceptionString );
}


/*---------------------------------------------------------------------------*/