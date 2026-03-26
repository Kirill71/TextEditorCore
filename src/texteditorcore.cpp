#include "texteditorcore.hpp"
#include "customiterator.hpp"


/*---------------------------------------------------------------------------*/


TextEditorCore::TextEditorCore() noexcept
	:	m_container{ constants::DEFAULT_DOCUMENT_SIZE, constants::SPACE }
	,	m_cursor{ std::make_unique< Cursor >( m_container ) }
	,	m_finderReplacer{ std::make_unique< Replacer >( m_container ) }
{
} // TextEditorCore::TextEditorCore


/*---------------------------------------------------------------------------*/


TextEditorCore::TextEditorCore( std::istream & _stream ) noexcept
	:	TextEditorCore()
{
	_stream >> *this;

} // TextEditorCore::TextEditorCore


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::cursorLeft()
{
	m_cursor->cursorLeft();
	return *this;

} // TextEditorCore::cursorLeft


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::cursorRight()
{
	m_cursor->cursorRight();
	return *this;

} // TextEditorCore::cursorRight


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::cursorDown()
{
	m_cursor->cursorDown();
	return *this;

} // TextEditorCore::cursorDown


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::cursorUp()
{
	m_cursor->cursorUp();
	return *this;

} // TextEditorCore::cursorUp 


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::setCursor( const size_t _row, const size_t _col )
{
	m_cursor->setCursor( _row, _col );
	return *this;

} // TextEditorCore::setCursor


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::setCursor( const position & _pos )
{
	m_cursor->setCursor( _pos );
	return *this;
} // TextEditorCore::setCursor


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::insert( const char _character )
{
	std::string text{ _character };
	insertText( 
			const_cast< position& >( m_cursor->getCursorPosition() )
		,	text );

	return *this;

} // TextEditorCore::insert


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::insert( const std::string & _str )
{
	insertText(
			const_cast< position& >( m_cursor->getCursorPosition() )
		,	const_cast< std::string& >( _str ) );

	return *this;

} // TextEditorCore::insert


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::removeSelectedText()
{
	deleteText( m_cursor->m_selectedText.from(), m_cursor->m_selectedText.to() );
	m_cursor->m_selectedText.to() = m_cursor->m_selectedText.from();

	return *this;

} // TextEditorCore::removeSelectedText


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::startSelection() noexcept
{
	m_cursor->startSelection();
	return *this;

} // TextEditorCore::startSelection


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::finishSelection() noexcept
{
	m_cursor->finishSelection();
	return *this;

} // TextEditorCore::finishSelection()


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::continueSelection()
{
	m_cursor->continueSelection();
	return *this;

} // TextEditorCore::continueSelection


/*---------------------------------------------------------------------------*/


TextEditorCore& TextEditorCore::resetSelection() noexcept
{
	m_cursor->resetSelection();
	return *this;

} // TextEditorCore::resetSelection


/*---------------------------------------------------------------------------*/


std::string TextEditorCore::getSelectedText() const noexcept
{
	return m_cursor->getSelectedText();

} // TextEditorCore::getSelectedText


/*---------------------------------------------------------------------------*/


const position& TextEditorCore::find( const std::string & _str )
{
	return m_finderReplacer->find( _str );

} // TextEditorCore::find


/*---------------------------------------------------------------------------*/


const position& TextEditorCore::findNext()
{
	return m_finderReplacer->findNext();

} // TextEditorCore::findNext


/*---------------------------------------------------------------------------*/


bool TextEditorCore::replace( const std::string & _oldStr, const std::string & _newStr )
{
	return m_finderReplacer->replace( _oldStr, _newStr);

} // TextEditorCore::replace


/*---------------------------------------------------------------------------*/


bool TextEditorCore::replaceAll( const std::string & _oldStr, const std::string & _newStr )
{
	return m_finderReplacer->replaceAll( _oldStr, _newStr );

} // TextEditorCore::replaceAll


/*---------------------------------------------------------------------------*/


void TextEditorCore::insertText( const position & _pos, const std::string& _text )  noexcept
{
	if ( _text.empty() ) 
		return;

	std::string endOfCurrentString{}, textCopy{ _text };
	if ( _text == constants::END_OF_LINE )
	{
		newLineInsert( _text, endOfCurrentString, _pos );
		return;
	}

	bool	isNewLineNeed{ _text.back() == constants::END_OF_LINE_CHAR }
		,	centralInsertion{}; // this variable keeps two value 0 or 1
	getEndPartOfChangeString( _text, endOfCurrentString, _pos ); // get end part of current change string
	InsertIterator begin{ textCopy };
	// if insertion in string from index not zero

	if ( _pos.m_col > 0 )
	{
		m_container[ _pos.m_row ].insert( _pos.m_col, *begin++ );//insert first row
		centralInsertion = true;
	}

	m_container.insert( 
			m_container.begin() + static_cast<long>(_pos.m_row) + centralInsertion /*implicit cast for uint*/
		,	begin, InsertIterator() );
	//count insertion string
	auto count =
		std::distance( InsertIterator{ textCopy }, InsertIterator() );

	//  calculate new current row( and remember about c-style(numeration from zero))(--count)
	--count;
	auto row{ _pos.m_row + count};
	const auto col{ m_container[ row ].length() };
	m_cursor->setCursor(row, col);
	// this method or add new last row on new line or append  string to end last row; 
	addLastRow( isNewLineNeed, _pos, endOfCurrentString );

	if ( isNewLineNeed )
	{
		m_cursor->setCursor(++row, constants::LINE_BEGIN );
	}

} // TextEditorCore::insertText


/*---------------------------------------------------------------------------*/


void TextEditorCore::deleteText( const position & _from, const position & _to )
{
	if ( _from >= maxPosition() || _to >= maxPosition() )
		throw std::logic_error(errorMessage::INVALID_POSITION);

	if ( m_cursor->m_currentMode == Cursor::mode::Select )
	{
		finishSelection();
	}

	if ( _from == _to ) // nothing delete
	{
		return;
	}

	m_cursor->setCursor( _from );
	if ( _from.m_row == _to.m_row ) 
	{
		m_container.at( _from.m_row ).erase( _from.m_col, _to.m_col - _from.m_col );
		if ( m_container[ _from.m_row ].empty() )
			deleteRow( _from.m_row );
	}
	else
	{
		deleteColTextFragment( _to );
		if ( (_to.m_row - _from.m_row) > 1 )
			m_container.erase(
					m_container.begin() + static_cast<long>(_from.m_row) + 1
				,	m_container.begin() + static_cast<long>(_to.m_row) );
		deleteRowTextFragment( _from );
	}

} // TextEditorCore::deleteText


/*---------------------------------------------------------------------------*/

void TextEditorCore::deleteRow(const size_t _row ) noexcept
{
	m_container.erase( m_container.cbegin() + static_cast<long>(_row) );

} // TextEditorCore::deleteRow


/*---------------------------------------------------------------------------*/


void TextEditorCore::deleteRowTextFragment( const position& _from ) noexcept
{
	const auto count{ m_container.at( _from.m_row ).length() - _from.m_col };
	if ( _from.m_col == 0 )
	{
		deleteRow( _from.m_row );
	}
	else 
	{
		m_container[ _from.m_row ].erase( _from.m_col, count );
	}

} // TextEditorCore::deleteRowTextFragment


/*---------------------------------------------------------------------------*/


void TextEditorCore::deleteColTextFragment( const position & _to ) noexcept
{
	if (_to.m_col == m_container.at( _to.m_row ).length()) 
	{
		deleteRow( _to.m_row );
	}
	else 
	{
		m_container.at( _to.m_row ).erase( constants::LINE_BEGIN, _to.m_col );
	}

} // TextEditorCore::deleteColTextFragment


/*---------------------------------------------------------------------------*/


void TextEditorCore::getEndPartOfChangeString(
		const std::string & _text
	,	std::string & _endOfCurrentString
	,	const position& _pos )
{
	const auto count{ m_container.at( _pos.m_row ).length() - _pos.m_col };
	//  remember end of string
	  _endOfCurrentString
		  .assign( m_container.at( _pos.m_row )
			  .substr( _pos.m_col, count )
		  );
	// delete end of string from container
	m_container.at( _pos.m_row ).erase(
			m_container.at( _pos.m_row ).begin() + static_cast<long>(_pos.m_col)
		,	m_container.at( _pos.m_row ).end() );

	if ( _pos.m_col == constants::LINE_BEGIN ) // if get all string.
	{
		m_container.erase( m_container.begin() + static_cast<long>(_pos.m_row) );
	}

} // TextEditorCore::getEndPartOfChangeString


/*---------------------------------------------------------------------------*/


void TextEditorCore::addLastRow(
		const bool _isNewLineNeed
	,	const position & _pos
	,	const std::string & _text ) noexcept
{
	if ( _isNewLineNeed )
	{
		m_container.insert( m_container.begin() + static_cast<long>(_pos.m_row + 1), _text );
	}
	else
	{
		m_container[ _pos.m_row ].append( _text );
	}
}


/*---------------------------------------------------------------------------*/


void TextEditorCore::newLineInsert(
		const std::string & _text
	,	std::string & _endOfString
	,	const position & _pos ) noexcept
{
	if ( _pos.m_col == constants::LINE_BEGIN ) 
	{
		m_container.insert( m_container.begin() + static_cast<long>(_pos.m_row), constants::SPACE );
	}
	else
	{
		getEndPartOfChangeString( _text, _endOfString, _pos );
		m_container.insert( m_container.begin() + static_cast<long>(_pos.m_row + 1), _endOfString );
	}

	m_cursor->setCursor( position{ _pos.m_row + 1, constants::LINE_BEGIN } );

}  //TextEditorCore::newLineInsert

/*---------------------------------------------------------------------------*/


std::ostream& operator<<( std::ostream & _lhs, const TextEditorCore & _rhs ) noexcept
{
	std::copy( _rhs.m_container.cbegin(), _rhs.m_container.cend(),
		std::ostream_iterator< std::string >(
				_lhs
			,	constants::END_OF_LINE.c_str() 
			) );

	return _lhs;

} // operator<<


/*---------------------------------------------------------------------------*/


std::istream& operator >> ( std::istream & _lhs, TextEditorCore & _rhs ) noexcept
{
	const InputIterator begin{ _lhs };
	const InputIterator end{};
	_rhs.m_container.assign( begin, end );

	return _lhs;

} // operator>>


/*---------------------------------------------------------------------------*/
