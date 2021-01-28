#ifndef CURSOR_HPP
#define CURSOR_HPP

#include<utility>
#include<string>
#include<algorithm>
#include<functional>
#include"position.hpp"
#include"message.hpp"

/*---------------------------------------------------------------------------*/

class Cursor{

/*---------------------------------------------------------------------------*/

	friend class TextEditorCore;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	explicit Cursor( const MyContainer& _container );

	Cursor( unsigned _row, unsigned _col, const MyContainer& _container );

	Cursor( const Cursor& _csr ) = delete;

	Cursor( Cursor&& _csr ) = delete;

	Cursor& operator=( const Cursor& _csr ) = delete;

	Cursor& operator=( Cursor&& _csr ) = delete;

	~Cursor() = default;

/*---------------------------------------------------------------------------*/


	const position& getCursorPosition() const noexcept 
	{
		return m_cursor;
	}

/*---------------------------------------------------------------------------*/


	position maxPosition() const noexcept
	{
		return position( m_container.size() - 1, m_container.back().length() );
	}

/*---------------------------------------------------------------------------*/


	unsigned currentRowMaxCol( unsigned _row  ) const noexcept 
	{
		return m_container.at( _row ).length();
	}

/*---------------------------------------------------------------------------*/

	void cursorLeft();

	void cursorRight();

	void cursorDown();

	void cursorUp();

	void setCursor( unsigned _row , unsigned _col );

	void setCursor( const position& _pos );

	void startSelection() noexcept;

	void finishSelection() noexcept;

	void continueSelection();

	void resetSelection() noexcept;

	std::string getSelectedText() noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

	position& getPositionObject() noexcept
	{
		return  m_currentMode == mode::Edit ? m_cursor : m_selectedText.to();
	}

/*---------------------------------------------------------------------------*/

	void cancelReverseSelection();

	void addLastRowFromMultilineSelection( std::string& _selectedText ) noexcept;

	void addSingleRow( std::string& _selectedText ) noexcept;

	void addFirstRowFromMiltilineSelection( std::string& _selectedText ) noexcept;

	void multilineRowSelection( std::string& _selectedText ) noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

	position m_cursor;

	enum class mode : bool
	{
		Edit = true,
		Select = false
	} 
	m_currentMode;

	struct selectedText 
	{

	/*---------------------------------------------------------------------------*/

	private:

	/*---------------------------------------------------------------------------*/

		std::pair< position /*from*/, position /*to*/ > m_selection;

	/*---------------------------------------------------------------------------*/

	public:

	/*---------------------------------------------------------------------------*/

		selectedText() 
			: m_selection {} {};

	/*---------------------------------------------------------------------------*/


		position& from() noexcept 
		{
			return m_selection.first;
		}

	/*---------------------------------------------------------------------------*/


		position& to() noexcept 
		{
			return m_selection.second;
		}

	/*---------------------------------------------------------------------------*/

	} //  struct selectedText

	m_selectedText;

	const MyContainer& m_container;

/*---------------------------------------------------------------------------*/

}; // class Cursor

#endif // !CURSOR_HPP
