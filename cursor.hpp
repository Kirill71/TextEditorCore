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
	Cursor();

	Cursor( unsigned _row, unsigned _col );

	Cursor( const Cursor& _csr ) = delete;

	Cursor( const Cursor&& _csr ) = delete;

	Cursor& operator=( const Cursor& _csr ) = delete;

	Cursor& operator=( const Cursor&& _csr ) = delete;

	~Cursor() = default;

/*---------------------------------------------------------------------------*/


	const position& getCursorPosition() const noexcept 
	{
		return m_cursor;
	}

/*---------------------------------------------------------------------------*/


	position maxPosition( const MyContainer& _container ) const noexcept
	{
		return position( _container.size() - 1, _container.back().length() );
	}

/*---------------------------------------------------------------------------*/


	unsigned currentRowMaxCol( unsigned _row , const MyContainer& _container ) const noexcept 
	{
		return _container.at( _row ).length();
	}

/*---------------------------------------------------------------------------*/

	void cursorLeft( const MyContainer& _container );

	void cursorRight( const MyContainer& _container );

	void cursorDown( const MyContainer& _container );

	void cursorUp( const MyContainer& _container );

	void setCursor( unsigned _row , unsigned _col , const MyContainer& _container );

	void setCursor( const position& _pos , const MyContainer& _container );

	void startSelection() noexcept;

	void finishSelection() noexcept;

	void continueSelection();

	void resetSelection() noexcept;

	std::string getSelectedText( const MyContainer& _container ) noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

	position& getPositionObject() noexcept
	{
		return ( m_currentMode == mode::Edit ) ? m_cursor : m_selectedText.to();
	}

/*---------------------------------------------------------------------------*/

	void cancelReverseSelection();

	void addLastRowFromMultilineSelection( std::string& _selectedText , const MyContainer& _container ) noexcept;

	void addSingleRow( std::string& _selectedText , const MyContainer& _container ) noexcept;

	void addFirstRowFromMiltilineSelection( std::string& _selectedText , const MyContainer& _container ) noexcept;

	void multilineRowSelection( std::string& _selectedText , const MyContainer& _container ) noexcept;

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

/*---------------------------------------------------------------------------*/

}; // class Cursor

#endif // !CURSOR_HPP