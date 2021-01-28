#include "cursor.hpp"

/*---------------------------------------------------------------------------*/

void 
Cursor::cancelReverseSelection()
{ 
	if ( ( m_selectedText.from().m_row > m_selectedText.to().m_row )
		|| ( m_selectedText.from().m_row == m_selectedText.to().m_row )  
		&& ( m_selectedText.from().m_col > m_selectedText.to().m_col ) )
		std::swap( m_selectedText.from(), m_selectedText.to() ); //swap from with to, if to < from

} // Cursor::cancelReverseSelection()

/*---------------------------------------------------------------------------*/

void
Cursor::addLastRowFromMultilineSelection( std::string& _selectedText ) noexcept
{
	_selectedText
		.append( m_container[ m_selectedText.to().m_row ]
			.substr( constants::LINE_BEGIN, m_selectedText.to().m_col ) );

} // Cursor::addLastRowFromMultilineSelection

/*---------------------------------------------------------------------------*/

void
Cursor::addSingleRow( std::string& _selectedText ) noexcept
{
	auto countCharacters { m_selectedText.to().m_col - m_selectedText.from().m_col };
	_selectedText
		.assign( m_container[ m_selectedText.from().m_row ]
			.substr( m_selectedText.from().m_col, countCharacters ) );

} // Cursor::addSingleRow

/*---------------------------------------------------------------------------*/

void
Cursor::addFirstRowFromMiltilineSelection( std::string& _selectedText ) noexcept
{
	auto countCharacters{ m_container[m_selectedText.from().m_row].length() - m_selectedText.from().m_col };
	_selectedText
		.append( m_container[ m_selectedText.from().m_row ]
			.substr( m_selectedText.from().m_col, countCharacters )
				.append(constants::END_OF_LINE));

} //  Cursor::addFirstRowFromMiltilineSelection

/*---------------------------------------------------------------------------*/

void
Cursor::multilineRowSelection( std::string& _selectedText ) noexcept
{
	auto begin{ m_container.begin() + ( m_selectedText.from().m_row + 1 ) },
	end{ m_container.begin() + m_selectedText.to().m_row};
	
	auto add_row = [ &_selectedText ]( const std::string& _currStr )
	{ 
		_selectedText.append( _currStr ).append( constants:: END_OF_LINE ); 
	};
	 
	std::for_each( begin, end, add_row );

} // Cursor::multilineRowSelection

/*---------------------------------------------------------------------------*/

Cursor::Cursor( const MyContainer& _container )
	:	Cursor( constants::LINE_BEGIN, constants::LINE_BEGIN, _container )
{
} // Cursor::Cursor() 

/*---------------------------------------------------------------------------*/

Cursor::Cursor( unsigned _row, unsigned _col, const MyContainer& _container ) 
	:	m_cursor{ _row, _col }
	,	m_currentMode{ mode::Edit }
	,	m_selectedText{}
	,	m_container{ _container }
{
} // Cursor::Cursor

/*---------------------------------------------------------------------------*/

void 
Cursor::cursorLeft()
{
	auto& cursor{ getPositionObject() };

	if ( cursor.m_col > constants::LINE_BEGIN )
	{
		--cursor.m_col;
	}
	else if ( cursor.m_row > constants::LINE_BEGIN )
	{
		cursorUp();
		cursor.m_col = m_container[ cursor.m_row ].length();
	}

} // Cursor::cursorLeft

/*---------------------------------------------------------------------------*/

void
Cursor::cursorRight()
{
	auto& cursor{ getPositionObject() };

	if ( cursor.m_col < m_container[ cursor.m_row ].length() )
	{
		++cursor.m_col;
	}
	else if ( cursor.m_row < m_container.size() - 1 )
	{
		cursorDown();
		cursor.m_col = constants::LINE_BEGIN;
	}
} //  Cursor::cursorRight

/*---------------------------------------------------------------------------*/

void
Cursor::cursorDown()
{
	auto& cursor{ getPositionObject() };
	/*check next row  cursor column position*/
	if ( cursor.m_row < m_container.size() - 1 && cursor.m_col > m_container[ ++cursor.m_row ].length() ) 
	{
		cursor.m_col = m_container[ cursor.m_row ].length();
	}

} // Cursor::cursorDown

/*---------------------------------------------------------------------------*/

void
Cursor::cursorUp() 
{
	auto& cursor{ getPositionObject() };
	/*check prev row  cursor column position*/
	if ( cursor.m_row > constants::LINE_BEGIN && cursor.m_col > m_container[ --cursor.m_row ].length() ) 
	{
		cursor.m_col = m_container[ cursor.m_row ].length();
	}

} // Cursor::cursorUp

/*---------------------------------------------------------------------------*/

void
Cursor::setCursor( unsigned _row, unsigned _col ) 
{
	setCursor( position( _row, _col) );

} // Cursor::setCursor

/*---------------------------------------------------------------------------*/

void
Cursor::setCursor( const position& pos )
{
	auto& cursor{ getPositionObject() };
	/* check current row lenght*/
	if (pos <= maxPosition() && pos.m_col <= currentRowMaxCol( pos.m_row ) )
	{
		cursor = pos;
	}
	else
	{
		throw std::logic_error( errorMessage::INVALID_POSITION );
	}

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
	// åñëè êóðñîð â êîíöå è ðåæèì íå ðàâåí ðåæèìó âûäåëåíèÿ, çíà÷èò ìû çàêîí÷èëè âûäåëåíèå è îñòàëèñü íà òîé æå ïîçèöèè(òîãäà ïðîäîëæàåì âûäåëÿòü)
	if ( m_cursor == m_selectedText.to() &&	m_currentMode == mode::Edit )
	{
		m_currentMode = mode::Select;
	}
	else
	{
		throw std::logic_error( errorMessage::BAD_CONTINUE_SELECTION );
	}

} // Cursor::continueSelection

/*---------------------------------------------------------------------------*/

void 
Cursor::resetSelection() noexcept
{
	// åñëè âûáðàí ðåæèì âûäåëåíèÿ çíà÷èò, ñáðàñûâàåì åãî è âîçðàùàåì êóðñîð â íà÷àëî âûäåëåíèÿ, èíà÷å íè÷åãî 
	if ( m_currentMode == mode::Select ) 
	{
		m_currentMode = mode::Edit;
		m_cursor = m_selectedText.from(); // ïðè íàëè÷èè ðåæèìà âûäåëåíèÿ ýòà ñòðîêà íå ê ÷åìó, 
										  //îäíàêî ÿ õî÷ó â áóäóùåì îò íåãî îòêàçàòüñÿ, òàê êàê äóìàþ,
										  //÷òî ñìîãó ðåàëèçîâàòü ýêâèâàëåíòíûé ôóíêöèîíàë è áåç íåãî è òàì îíà ïðèãîäèòñÿ.
	}

} // Cursor::resetSelection

/*---------------------------------------------------------------------------*/

std::string 
Cursor::getSelectedText() noexcept
{
	if ( m_currentMode == mode::Select )
	{
		finishSelection();
	}

	std::string selectedText{};
	// if from == to return.
	if ( !( m_selectedText.from() == m_selectedText.to() ) )
	{
		if ( ( m_selectedText.from().m_row == m_selectedText.to().m_row ) ) 
		{
			addSingleRow( selectedText );
		}
		else 
		{
			addFirstRowFromMiltilineSelection( selectedText );
			if ( (m_selectedText.to() - m_selectedText.from()) > 1 )
			{
				multilineRowSelection( selectedText );
			}
			addLastRowFromMultilineSelection( selectedText );
		}
	}
	//Return value optimization
	return selectedText;

} // Cursor::getSelectedText

/*---------------------------------------------------------------------------*/
