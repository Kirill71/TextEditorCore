#ifndef CURSOR_HPP
#define CURSOR_HPP
#include<utility>
#include<string>
#include<functional>
#include"message.hpp"

struct position
{
	position() : position(0,0){}

	position(unsigned row_, unsigned col_) : m_row{ validate(row_) }, m_col{ validate(col_) } {}
	// copy operations
	position(const position& pos) = default;
	position& operator=(const position& pos) = default;
	position(const position&& pos) : m_row{ std::move(pos.m_row) }, m_col{ std::move(pos.m_col) } {}
	bool operator==(const position& rhs) const noexcept
	{
		return (this->m_row == rhs.m_row) && (this->m_col == rhs.m_col);
	}

	bool operator!=(const position& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	bool operator>(const position& rhs) const  noexcept
	{
		return (this->m_row > rhs.m_row) && (this->m_col > rhs.m_col);
	}

	bool operator<(const position& rhs) const  noexcept
	{
		return  !(*this > rhs);
	}

	unsigned m_row, m_col;

private:
	unsigned validate(unsigned param) const noexcept
	{
		return param >= 0 ? param : 0;
	}
};

class Cursor
{
	friend class TextEditorCore;
	position m_cursor;
	enum class mode : bool
	{
		Edit = true,
		Select = false
	};
	// fields
	mode m_currentMode;
	// first == from, second == to
	std::pair<position, position> m_selection;
	//first == pos, second == searchString
	std::pair<position, std::string> m_find;
	// methods

	// if mode == Edit return coursor, if Select return first position of selection;
	position& getPositionObject() noexcept
	{
		return (m_currentMode == mode::Edit) ? m_cursor : m_selection.first;
	}
	//void genericCursorUpDown(bool expressionFirst, const Container& container, const std::function<void(unsigned& row)>& cursorAction);
public:
	Cursor();
	Cursor(unsigned row, unsigned col);
	Cursor(const Cursor& csr) = delete;
	Cursor(const Cursor&& csr) = delete;
	Cursor& operator=(const Cursor& csr) = delete;
	Cursor& operator=(const Cursor&& csr) = delete;
	~Cursor() = default;

	const position& getCoursorPos() const noexcept
	{
		return m_cursor;
	}
	const position& maxPosition(const Container& container) const noexcept
	{
		return position(container.size() - 1, container.back().length());
	}
	void cursorLeft(const Container& container);
	void cursorRight(const Container& container);
	void cursorDown(const Container& container);
	void cursorUp(const Container& container);
	void setCursor(unsigned row, unsigned col, const Container& container);
	void setCursor(const position& pos, const Container& container);
};
#endif // !CURSOR_HPP