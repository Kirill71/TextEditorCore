#include "cursor.hpp"

void Cursor::cancelReverseSelection()
{ 
	if (m_selectedText.from().m_row > m_selectedText.to().m_row ||
		(m_selectedText.from().m_row == m_selectedText.to().m_row  && m_selectedText.from().m_col > m_selectedText.to().m_col))
		std::swap(m_selectedText.from(),m_selectedText.to());//swap from with to
}

Cursor::Cursor() : Cursor(0,0) {}

Cursor::Cursor(unsigned row, unsigned col) : m_cursor{ row,col }, m_currentMode{ mode::Edit }, m_selectedText{}, m_find{} {}

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

void Cursor::startSelection() noexcept {
	m_currentMode = mode::Select;
	// selection begin == selection end and equils current cursor position;
	m_selectedText.from() = m_selectedText.to() = m_cursor;
}

void Cursor::finishSelection() noexcept {
	m_currentMode = mode::Edit;
	m_cursor = m_selectedText.to();
}
void Cursor::resetSelection(){
	startSelection();
}

std::string Cursor::getSelectedText(const Container& container) noexcept
{
	//	TODO: not beautiful!!!!! I want improve it in future. 
	cancelReverseSelection();
	std::string selectedText{};
	   // if selected one row, but no one columns, get this substr
	if ((m_selectedText.from().m_row == m_selectedText.to().m_row )&& (m_selectedText.from() != m_selectedText.to())) 
			selectedText
				.assign(container[m_selectedText.from().m_row]
					.substr(m_selectedText.from().m_col, m_selectedText.to().m_col - m_selectedText.from().m_col));
	else {
		if (m_selectedText.from().m_row != m_selectedText.to().m_row) {
			selectedText
				.append(container[m_selectedText.from().m_row]
					.substr(m_selectedText.from().m_col,
						container[m_selectedText.from().m_row].length() - m_selectedText.from().m_col)
							.append(END_OF_LINE));
		}

		if ((m_selectedText.to() - m_selectedText.from()) > 1) {
			for (auto row = (m_selectedText.from().m_row + 1); row < m_selectedText.to().m_row; ++row)
				selectedText.append(container[row]).append(END_OF_LINE);
		};
	}
	return selectedText;
}
