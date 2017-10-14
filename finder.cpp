#include "finder.hpp"


/*---------------------------------------------------------------------------*/


const position &
Finder::find_base( 
		const std::string & _str
	,	const position & _maxPos
	,	const MyContainer& _container
	,	const position & _pos )
{
	Utils::checkEmptyString( _str, errorMessage::EMPTY_SEARCH_STRING );
	m_findText.setParams( _pos, _str );

	auto it = std::find_if( _container.begin() + _pos.m_row, _container.end(),
		std::bind( [] ( 
				const std::string & _currentString
			,	const std::string & _searchText
			,	position* _findPos )
	{
		++_findPos->m_row;
		_findPos->m_col =
			_currentString.find( 
				_searchText
			,	( _findPos->m_col != std::string::npos ) 
				?	_findPos->m_col 
				:	0 );

		return  _findPos->m_col != std::string::npos;

	} , std::placeholders::_1,
			_str, &m_findText.lastPosition() ) );

	--m_findText.lastPosition().m_row; // return to c-style numeration from zero

	if ( it == _container.end() )
		m_findText.lastPosition() = _maxPos;

	return m_findText.lastPosition();

} // Finder::find_base


/*---------------------------------------------------------------------------*/


const position &
Finder::find( 
		const std::string & _str
	,	const position & _maxPos
	,	const MyContainer & _container )
{
	return find_base( _str, _maxPos, _container) != _maxPos
		?	m_findText.lastPosition() 
		:	_maxPos;

} // Finder::find

/*---------------------------------------------------------------------------*/

const position & 
Finder::findNext(
		const position& _maxPos
	,	const MyContainer& _container
	,	bool _isReplace )
{
	auto length{ _isReplace ? 0 : m_findText.searchString().length() };

	return find_base( 
				m_findText.searchString()
			,	_maxPos
			,	_container
			,	position{ 
							m_findText.lastPosition().m_row
						,	m_findText.lastPosition().m_col + length }
	);

} //  Finder::findNext


/*---------------------------------------------------------------------------*/