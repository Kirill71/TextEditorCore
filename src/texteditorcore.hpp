#pragma once

#include "message.hpp"
#include "cursor.hpp"
#include "replacer.hpp"
#include "export.hpp"

#include<memory>

/*---------------------------------------------------------------------------*/

class API TextEditorCore final
{

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	TextEditorCore() noexcept;

	explicit TextEditorCore( std::istream& _stream ) noexcept;

	TextEditorCore( const TextEditorCore& ) = delete;

	TextEditorCore& operator= ( const TextEditorCore& ) = delete;

	TextEditorCore( TextEditorCore&& ) = delete;

	TextEditorCore& operator= ( TextEditorCore&& ) = delete;

	~TextEditorCore() = default;

/*---------------------------------------------------------------------------*/


	const position& getCursorPosition() const noexcept
	{
		return m_cursor->getCursorPosition();
	}

/*---------------------------------------------------------------------------*/


	 position maxPosition() const noexcept
	 {
		return m_cursor->maxPosition();

	 }

/*---------------------------------------------------------------------------*/

	TextEditorCore& cursorLeft();

	TextEditorCore& cursorRight();

	TextEditorCore& cursorDown();

	TextEditorCore& cursorUp();

	TextEditorCore& setCursor( size_t _row, size_t _col);

	TextEditorCore& setCursor( const position& _pos );

	TextEditorCore& insert( char _character );

	TextEditorCore& insert( const std::string& _str );

	TextEditorCore& removeSelectedText();

/*---------------------------------------------------------------------------*/


	TextEditorCore& homeKeyPressed() noexcept
	{
		m_cursor->getPositionObject().m_col = constants::LINE_BEGIN;
		return *this;
	}

/*---------------------------------------------------------------------------*/


	TextEditorCore&
	endKeyPressed() noexcept
	{
		m_cursor->getPositionObject().m_col 
			= m_container[ m_cursor->getPositionObject().m_row ].length();

		return *this;
	}

/*---------------------------------------------------------------------------*/


	TextEditorCore&
	ctrlHomeKeyPressed() noexcept
	{
		m_cursor->getPositionObject().m_row 
			= m_cursor->getPositionObject().m_col 
				= constants::LINE_BEGIN;

		return *this;
	}

/*---------------------------------------------------------------------------*/


	TextEditorCore&
	ctrlEndKeyPressed() noexcept
	{
		m_cursor->getPositionObject() = maxPosition();
		return *this;
	}

/*---------------------------------------------------------------------------*/

	TextEditorCore& startSelection() noexcept;

	TextEditorCore& finishSelection() noexcept;

	TextEditorCore& continueSelection();

	TextEditorCore& resetSelection() noexcept;

	std::string getSelectedText() const noexcept;

	const position& find( const std::string& _str );

	const position& findNext();

	bool replace( const std::string& _oldStr, const std::string& _newStr );

	bool replaceAll( const std::string& _oldStr, const std::string& _newStr );

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

	API friend std::ostream& operator<< ( std::ostream& _lhs, const TextEditorCore& _rhs ) noexcept;

	API friend std::istream& operator>> ( std::istream& _lhs, TextEditorCore& _rhs ) noexcept;

/*---------------------------------------------------------------------------*/

	TextEditorCoreBase m_container;

	std::unique_ptr< Cursor > m_cursor;

	std::unique_ptr< Replacer > m_finderReplacer;

/*---------------------------------------------------------------------------*/

	void insertText( const position& _pos, const std::string& _text) noexcept;

	void deleteText( const position& _from, const position& _to );

	void deleteRow( size_t _row ) noexcept;

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
						,	const std::string& _text
				   ) noexcept;

	void newLineInsert( 
							const std::string& _text
						,	std::string& _endOfString
						,	const position& _pos
					  ) noexcept;

/*---------------------------------------------------------------------------*/

}; // class TextEditorCore
