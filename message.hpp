#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include<vector>
#include<string>
#include<stdexcept>

/*---------------------------------------------------------------------------*/

using MyContainer = std::vector< std::string >;

/*---------------------------------------------------------------------------*/

namespace constants 
{

	const unsigned	  DEFAULT_DOCUMENT_SIZE{ 1000 };

	const unsigned    LINE_BEGIN{ 0 };

	const std::string END_OF_LINE{ "\n" };

	const char		  END_OF_LINE_CHAR = { '\n' };

	const std::string SPACE{ " " };

}; // constants

/*---------------------------------------------------------------------------*/

namespace errorMessage 
{

	const  std::string INVALID_POSITION { "Invalid position of cursor" };

	const  std::string EMPTY_SEARCH_STRING { "Empty search string" };

	const  std::string EMPTY_REPLACE_STRING { "Empty replace string" };

	const  std::string TEXT_NOT_FOUND { "Text not found" };

	const  std::string BAD_CONTINUE_SELECTION{ "Bad continue selection" };

}; // errorMessage

/*---------------------------------------------------------------------------*/

#endif