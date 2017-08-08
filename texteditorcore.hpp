#ifndef TEXT_EDITOR_CORE_HPP
#define TEXT_EDITOR_CORE_HPP

#include<istream>
#include<memory>
#include<iterator>
#include<sstream>
#include"message.hpp"
#include"cursor.hpp"
#include"replacer.hpp"
#include"customIterator.hpp"

/*---------------------------------------------------------------------------*/

class TextEditorCore
{

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	TextEditorCore() noexcept;

	explicit TextEditorCore( std::istream& _stream ) noexcept;

	TextEditorCore( const TextEditorCore& ) = delete;

	TextEditorCore& operator= ( const TextEditorCore& ) = delete;

	TextEditorCore( const TextEditorCore&& ) = delete;

	TextEditorCore& operator= ( const TextEditorCore&& ) = delete;

	~TextEditorCore() = default;

/*---------------------------------------------------------------------------*/


	const position& 
	getCursorPosition() const noexcept
	{
		return m_cursor->getCursorPosition();
	}

/*---------------------------------------------------------------------------*/


	 position 
	 maxPosition() const noexcept 
	 {
		return m_cursor->maxPosition( m_container );

	 }

/*---------------------------------------------------------------------------*/

	TextEditorCore& cursorLeft();

	TextEditorCore& cursorRight();

	TextEditorCore& cursorDown();

	TextEditorCore& cursorUp();

	TextEditorCore& setCursor( unsigned _row, unsigned _col);

	TextEditorCore& setCursor( const position& _pos );

	TextEditorCore& insert( char _character );

	TextEditorCore& insert( const std::string& _str );

	TextEditorCore& removeSelectedText();

/*---------------------------------------------------------------------------*/


	TextEditorCore& 
	HomeKeyPressed() noexcept
	{
		m_cursor->getPositionObject().m_col = constants::LINE_BEGIN;
		return *this;
	}

/*---------------------------------------------------------------------------*/


	TextEditorCore&
	EndKeyPressed() noexcept 
	{
		m_cursor->getPositionObject().m_col 
			= m_container[ m_cursor->getPositionObject().m_row ].length();

		return *this;
	}

/*---------------------------------------------------------------------------*/


	TextEditorCore&
	CtrlHomeKeyPressed() noexcept 
	{
		m_cursor->getPositionObject().m_row 
			= m_cursor->getPositionObject().m_col 
				= constants::LINE_BEGIN;

		return *this;
	}

/*---------------------------------------------------------------------------*/


	TextEditorCore&
	CtrlEndKeyPressed() noexcept 
	{
		m_cursor->getPositionObject() = maxPosition();
		return *this;
	}

/*---------------------------------------------------------------------------*/

	TextEditorCore& startSelection() noexcept;

	TextEditorCore& finishSelection() noexcept;

	TextEditorCore& continueSelection();

	TextEditorCore& resetSelection() noexcept;

	std::string getSelectedText() noexcept;

	const position& find( const std::string& _str );

	const position& findNext();

	bool replace( const std::string& _oldStr, const std::string& _newStr );

	bool replaceAll( const std::string& _oldStr, const std::string& _newStr );

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

	friend  std::ostream& operator<< ( std::ostream& _lhs, TextEditorCore& _rhs ) noexcept;

	friend  std::istream& operator>> ( std::istream& _lhs, TextEditorCore& _rhs ) noexcept;

/*---------------------------------------------------------------------------*/

	std::unique_ptr< Cursor > m_cursor;

	std::unique_ptr< Replacer > m_finderReplacer;

	MyContainer m_container;


/*---------------------------------------------------------------------------*/

	void insertText( position& _pos, std::string& _text) noexcept;

	void deleteText( const position& _from, position& _to );

	void deleteRow( unsigned _row ) noexcept;

	void deleteRowTextFragment( const position& _from ) noexcept;

	void deleteColTextFragment( const position& _to ) noexcept;

	void getEndPartOfChangeString( 
											const std::string& _text
										,	std::string& _endOfCurrentString
										,	const position& _pos 
								 ); 
	void addLastRow(	 
							bool _isNewLineNeed
						,	const position& _pos
						,	const std::string& lastRow 
				   ) noexcept;

	void newLineInsert( 
							const std::string& _text
						,	std::string& _endOfString
						,	position& _pos 
					  ) noexcept;

/*---------------------------------------------------------------------------*/

}; // class TextEditorCore

#endif // !TEXT_EDITOR_CORE_HPP