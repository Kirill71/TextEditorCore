#include "utils.hpp"


/*---------------------------------------------------------------------------*/


void 
Utils::checkEmptyString(
		const std::string _strForCheck
	,	const std::string & _exceptionString)
{
	if (_strForCheck.empty())
		throw std::logic_error( _exceptionString );
}


/*---------------------------------------------------------------------------*/