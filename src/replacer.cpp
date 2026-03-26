#include "replacer.hpp"
#include "utils.hpp"

/*---------------------------------------------------------------------------*/


void
Replacer::replaceInContainer( const position & _currentPos,	const std::string& new_str )
{
	const_cast< TextEditorCoreBase& >( m_container )[ _currentPos.m_row ]
		.replace(
				_currentPos.m_col
			,	m_findText.searchString().length()
			,	new_str );

	m_isSuccessfully = true;

} // Replacer::replaceInContainer


/*---------------------------------------------------------------------------*/


Replacer::Replacer( const TextEditorCoreBase & _container )
	: Finder( _container )
	, m_isSuccessfully{false}
{
} // Replacer::Replacer


/*---------------------------------------------------------------------------*/


bool
Replacer::replace( const std::string & _oldStr , const std::string & _newStr )
{
	m_isSuccessfully = false;
	Utils::checkEmptyString( _oldStr, errorMessage::EMPTY_REPLACE_STRING );

	auto & current_pos = find_base( _oldStr);
	if ( current_pos != maxPosition() )
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
		while ( ( current_pos = findNext( true ) ) != maxPosition() )
			replaceInContainer( current_pos,  _newStr );
	}
	return m_isSuccessfully;

} // Replacer::replaceAll


/*---------------------------------------------------------------------------*/