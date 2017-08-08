#include "cursor.hpp"

/*---------------------------------------------------------------------------*/

void 
Cursor::cancelReverseSelection()
{ 
	if ( 
		   ( m_selectedText.from().m_row > m_selectedText.to().m_row )
		|| ( m_selectedText.from().m_row == m_selectedText.to().m_row )  
		&& ( m_selectedText.from().m_col > m_selectedText.to().m_col ) )
		std::swap( m_selectedText.from(), m_selectedText.to() ); //swap from with to, if to < from

} // Cursor::cancelReverseSelection()

/*---------------------------------------------------------------------------*/

void
Cursor::addLastRowFromMultilineSelection( 
		std::string& _selectedText
	,	const MyContainer& _container ) noexcept
{
	_selectedText
		.append( _container[ m_selectedText.to().m_row ]
			.substr( constants::LINE_BEGIN, m_selectedText.to().m_col ) );

} // Cursor::addLastRowFromMultilineSelection

/*---------------------------------------------------------------------------*/

void
Cursor::addSingleRow( 
		std::string& _selectedText
	,	const MyContainer & _container ) noexcept
{
	unsigned countCharacters { m_selectedText.to().m_col - m_selectedText.from().m_col };
	_selectedText
		.assign( _container[ m_selectedText.from().m_row ]
			.substr( m_selectedText.from().m_col, countCharacters ) );

} // Cursor::addSingleRow

/*---------------------------------------------------------------------------*/

void
Cursor::addFirstRowFromMiltilineSelection( 
		std::string& _selectedText
	,	const MyContainer & _container ) noexcept
{
	unsigned countCharacters{ _container[m_selectedText.from().m_row].length() - m_selectedText.from().m_col };
	_selectedText
		.append( _container[ m_selectedText.from().m_row ]
			.substr( m_selectedText.from().m_col, countCharacters )
				.append(constants::END_OF_LINE));

} //  Cursor::addFirstRowFromMiltilineSelection

/*---------------------------------------------------------------------------*/

void
Cursor::multilineRowSelection( 
		std::string& _selectedText
	,	const MyContainer & _container ) noexcept
{
	auto begin{ _container.begin() + (m_selectedText.from().m_row +1 ) },
		 end{ _container.begin() + m_selectedText.to().m_row};
	auto add_row = [ &_selectedText ]( const std::string& _currStr )
	{ _selectedText.append( _currStr ).append( constants:: END_OF_LINE ); };
	 
	std::for_each( begin, end, add_row );

} // Cursor::multilineRowSelection

/*---------------------------------------------------------------------------*/

Cursor::Cursor() 
	:	Cursor( constants::LINE_BEGIN, constants::LINE_BEGIN ) 
{
} // Cursor::Cursor() 

/*---------------------------------------------------------------------------*/

Cursor::Cursor( unsigned row, unsigned col ) 
	:	m_cursor{ row,col }, m_currentMode{ mode::Edit }, m_selectedText{} 
{
} // Cursor::Cursor

/*---------------------------------------------------------------------------*/

void 
Cursor::cursorLeft( const MyContainer& _container )
{
	auto& cursor{ getPositionObject() };

	if ( cursor.m_col > constants::LINE_BEGIN )
		--cursor.m_col;
	else if ( cursor.m_row > constants::LINE_BEGIN )
	{
		cursorUp( _container );
		cursor.m_col = _container[ cursor.m_row ].length();
	}

} // Cursor::cursorLeft

/*---------------------------------------------------------------------------*/

void
Cursor::cursorRight(const MyContainer& container)
{
	auto& cursor{ getPositionObject() };

	if ( cursor.m_col < container[ cursor.m_row ].length() )
		++cursor.m_col;
	else if ( cursor.m_row < container.size() - 1 )
	{
		cursorDown( container );
		cursor.m_col = constants::LINE_BEGIN;
	}
} //  Cursor::cursorRight

/*---------------------------------------------------------------------------*/

void
Cursor::cursorDown( const MyContainer& _container )
{
	auto& cursor{ getPositionObject() };

	if (		cursor.m_row < _container.size() - 1 
			&& /*check next row  cursor column position*/cursor.m_col > _container[ ++cursor.m_row ].length() ) 
	{
		cursor.m_col = _container[ cursor.m_row ].length();
	}

} // Cursor::cursorDown

/*---------------------------------------------------------------------------*/

void
Cursor::cursorUp( const MyContainer& _container ) 
{
	auto& cursor{ getPositionObject() };

	if (		cursor.m_row > constants::LINE_BEGIN 
			&& /*check prev row  cursor column position*/ cursor.m_col > _container[ --cursor.m_row ].length() ) 
	{
		cursor.m_col = _container[ cursor.m_row ].length();
	}

} // Cursor::cursorUp

/*---------------------------------------------------------------------------*/

void
Cursor::setCursor(
		unsigned _row
	,	unsigned _col
	,	const MyContainer& _container ) 
{
	setCursor( position( _row, _col),  _container );

} // Cursor::setCursor

/*---------------------------------------------------------------------------*/

void
Cursor::setCursor(const position& pos, const MyContainer& container)
{
	auto& cursor{ getPositionObject() };
	if (pos <= maxPosition(container) && /* check current row lenght*/pos.m_col <= currentRowMaxCol(pos.m_row, container))
		cursor = pos;
	else
		throw std::logic_error(errorMessage::INVALID_POSITION);

} // Cursor::setCursor

/*---------------------------------------------------------------------------*/

void
Cursor::startSelection() noexcept
{
	m_currentMode = mode::Select;
	// selection begin == selection end and equils current cursor position;
	m_selectedText.from() = m_selectedText.to() = m_cursor;

} // Cursor::startSelection

/*---------------------------------------------------------------------------*/

void
Cursor::finishSelection() noexcept 
{
	m_currentMode = mode::Edit;
	m_cursor = m_selectedText.to();
	cancelReverseSelection();

} // Cursor::finishSelection

/*---------------------------------------------------------------------------*/

void 
Cursor::continueSelection()
{
	// если курсор в конце и режим не равен режиму выделения, значит мы закончили выделение и остались на той же позиции(тогда продолжаем выделять)
	if (		m_cursor == m_selectedText.to()
			&&	m_currentMode == mode::Edit )
		m_currentMode = mode::Select;
	else
		throw std::logic_error( errorMessage::BAD_CONTINUE_SELECTION );

} // Cursor::continueSelection

/*---------------------------------------------------------------------------*/

void 
Cursor::resetSelection() noexcept
{
	// если выбран режим выделения значит, сбрасываем его и возращаем курсор в начало выделения, иначе ничего 
	if ( m_currentMode == mode::Select ) 
	{
		m_currentMode = mode::Edit;
		m_cursor = m_selectedText.from(); // при наличии режима выделения эта строка не к чему, 
										  //однако я хочу в будущем от него отказаться, так как думаю,
										  //что смогу реализовать эквивалентный функционал и без него и там она пригодится.
	}

} // Cursor::resetSelection

/*---------------------------------------------------------------------------*/

std::string 
Cursor::getSelectedText( const MyContainer& _container ) noexcept
{
	if ( m_currentMode == mode::Select )
		finishSelection();

	std::string selectedText{};
	// if from == to return.
	if ( !( m_selectedText.from() == m_selectedText.to() ) )
	{
		if ( (m_selectedText.from().m_row == m_selectedText.to().m_row ) ) 
		{
			addSingleRow(selectedText, _container);
		}
		else 
		{
			addFirstRowFromMiltilineSelection( selectedText, _container );
			if ( (m_selectedText.to() - m_selectedText.from()) > 1 )
				multilineRowSelection( selectedText, _container );
			addLastRowFromMultilineSelection( selectedText, _container );
		}
	}
	//Return value optimization
	return selectedText;

} // Cursor::getSelectedText

/*---------------------------------------------------------------------------*/