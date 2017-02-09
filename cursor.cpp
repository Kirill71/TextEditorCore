#include "cursor.hpp"

Cursor::Cursor() : Cursor(0,0) {}

Cursor::Cursor(unsigned row, unsigned col) : m_cursor{row,col}, m_currentMode{ mode::Edit }, m_selection{}, m_find{} {}

// проверять индекс через at() смысла нет, 
//так как возвращаемый getPositionObject() курсор, всегда меньше или равен максимально возможной позиции.

void Cursor::cursorLeft(const Container& container){
	auto& cursor{ getPositionObject() };

	if (cursor.m_col > 0)
		--cursor.m_col;
	else if (cursor.m_row - 1 > 0)
	{
		cursorUp(container);
		cursor.m_col = container[cursor.m_row].length();
	}
}

void Cursor::cursorRight(const Container& container)
{
	auto& cursor{ getPositionObject() };
	
	if (cursor.m_col < container[cursor.m_row].length())
		++cursor.m_col;
	else if (cursor.m_row + 1 < container.size())
	{
		cursorDown(container);
		cursor.m_col = 0;
	}
}

//void Cursor::genericCursorUpDown(bool expression, const Container & container, const std::function<void(unsigned& row)>& cursorAction)
//{
//	auto& cursor{ getPositionObject() };
//	if (expression)
//		cursorAction(cursor.m_row);
//	else if (cursor.m_col > container[cursor.m_row].length())
//		cursor.m_col = container[cursor.m_row].length();
//}

void Cursor::cursorDown(const Container& container)
{
	auto& cursor{ getPositionObject() };
	/*bool expression{ cursor.m_row + 1 < container.size() };
	genericCursorUpDown(expression, container, [](unsigned & col) {  ++col; });*/
	if (cursor.m_row + 1 < container.size())
		++cursor.m_row;
	else if (cursor.m_col > container[cursor.m_row].length())
		cursor.m_col = container[cursor.m_row].length();
}

void Cursor::cursorUp(const Container& container)
{
	auto& cursor{ getPositionObject() };
	if (cursor.m_row > 0)
		--cursor.m_row;
	else if (cursor.m_col > container[cursor.m_row].length())
		cursor.m_col = container[cursor.m_row].length();
}

void Cursor::setCursor(unsigned row, unsigned col, const Container& container)
{
	setCursor(position(row, col),  container);
}

void Cursor::setCursor(const position& pos, const Container& container)
{
	auto& cursor{ getPositionObject() };
	if (cursor < maxPosition(container))
		cursor = pos;
	else
		throw std::logic_error(errorMessage::INVALID_POSITION);
}
