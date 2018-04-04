#include "replacer.hpp"

/*---------------------------------------------------------------------------*/


void
Replacer::replaceInContainer( const position & current_pos,	const std::string new_str )
{
	const_cast< MyContainer& >( m_container )[ current_pos.m_row ]
		.replace(
				current_pos.m_col
			,	m_findText.searchString().length()
			,	new_str );

	m_isSuccessfully = true;

} // Replacer::replaceInContainer


/*---------------------------------------------------------------------------*/


Replacer::Replacer( const MyContainer & _container )
	: Finder( _container )
{
} // Replacer::Replacer


/*---------------------------------------------------------------------------*/


bool
Replacer::replace( const std::string & _oldStr , const std::string & _newStr )
{
	m_isSuccessfully = false;
	Utils::checkEmptyString( _oldStr, errorMessage::EMPTY_REPLACE_STRING );

	auto & current_pos = find_base( _oldStr);
	if ( current_pos !=  maxPosition() )
		replaceInContainer( current_pos, _newStr );

	return m_isSuccessfully;

} // Replacer::replace


/*---------------------------------------------------------------------------*/


bool
Replacer::replaceAll( const std::string & _oldStr, const std::string & _newStr )
{
	if ( replace( _oldStr, _newStr ) ) 
	{
		position current_pos{};
		auto & maxPos{ maxPosition() };
		// подумать про предидущую позицию с учетом замены.
		while ( ( current_pos = findNext( true ) ) != maxPos )
			replaceInContainer( current_pos,  _newStr );
	}
	return m_isSuccessfully;

} // Replacer::replaceAll


/*---------------------------------------------------------------------------*/