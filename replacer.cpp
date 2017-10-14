#include "replacer.hpp"

/*---------------------------------------------------------------------------*/


void
Replacer::replaceInContainer(
		const position & current_pos
	,	MyContainer & container
	,	const std::string new_str )
{
	container[ current_pos.m_row ]
		.replace(
				current_pos.m_col
			,	m_findText.searchString().length()
			,	new_str );

	m_isSuccessfully = true;

} // Replacer::replaceInContainer


/*---------------------------------------------------------------------------*/


bool
Replacer::replace(
		const std::string & _oldStr
	,	const std::string & _newStr
	,	const position& _maxPos
	,	MyContainer& _container )
{
	m_isSuccessfully = false;
	Utils::checkEmptyString( _oldStr, errorMessage::EMPTY_REPLACE_STRING );

	auto & current_pos = find_base( _oldStr, _maxPos, _container );
	if ( current_pos != _maxPos )
		replaceInContainer( current_pos, _container, _newStr );

	return m_isSuccessfully;

} // Replacer::replace


/*---------------------------------------------------------------------------*/


bool
Replacer::replaceAll( 
		const std::string & _oldStr
	,	const std::string & _newStr
	,	const position& _maxPos
	,	MyContainer& _container )
{
	if ( replace( _oldStr, _newStr, _maxPos, _container ) ) 
	{
		position current_pos{};
		// подумать про предидущую позицию с учетом замены.
		while ( ( current_pos = findNext( _maxPos, _container, true ) ) != _maxPos )
			replaceInContainer( current_pos, _container, _newStr );
	}
	return m_isSuccessfully;

} // Replacer::replaceAll


/*---------------------------------------------------------------------------*/