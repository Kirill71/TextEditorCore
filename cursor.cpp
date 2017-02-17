#include "cursor.hpp"
void Cursor::cancelReverseSelection()
{ 
	if (m_selectedText.from().m_row > m_selectedText.to().m_row ||
		(m_selectedText.from().m_row == m_selectedText.to().m_row  && m_selectedText.from().m_col > m_selectedText.to().m_col))
		std::swap(m_selectedText.from(), m_selectedText.to());//swap from with to
}

void Cursor::addLastRowToSelectText(std::string& selectedText, const Container& container) noexcept
{
	selectedText
		.append(container[m_selectedText.to().m_row].substr(constants::LINE_BEGIN, m_selectedText.to().m_col))
		.append(constants::END_OF_LINE);
}

void Cursor::addSingleRow(std::string& selectedText, const Container & container) noexcept
{
	unsigned count_characters { m_selectedText.to().m_col - m_selectedText.from().m_col };
	selectedText
		.assign(container[m_selectedText.from().m_row]
			.substr(m_selectedText.from().m_col, count_characters));
}

void Cursor::addFirstRowFromMiltilineSelection(std::string& selectedText, const Container & container) noexcept
{
	unsigned count_characters{ container[m_selectedText.from().m_row].length() - m_selectedText.from().m_col };
	selectedText
		.append(container[m_selectedText.from().m_row]
			.substr(m_selectedText.from().m_col,
				count_characters)
			.append(constants::END_OF_LINE));
}

void Cursor::multilineRowSelection(std::string& selectedText, const Container & container) noexcept
{
	auto begin{ container.begin() + (m_selectedText.from().m_row + 1) },
		 end{ begin + m_selectedText.to().m_row };
	/*for (auto row = (m_selectedText.from().m_row + 1); row < m_selectedText.to().m_row; ++row)
		selectedText.append(container[row]).append(END_OF_LINE);*/
	auto add_row = [&selectedText](const std::string& curr_str)
	{selectedText.append(curr_str).append(constants:: END_OF_LINE); };

	std::for_each(begin, end,add_row);
	// если завершающий столбец == 0,то нет смысла добовлять завершитель иначе добавляется последняя строка и завершающий перенос каретки.
	if (m_selectedText.to().m_col != constants::LINE_BEGIN)
		addLastRowToSelectText(selectedText, container);
}

Cursor::Cursor() : Cursor(constants::LINE_BEGIN, constants::LINE_BEGIN) {}

Cursor::Cursor(unsigned row, unsigned col) : m_cursor{ row,col }, m_currentMode{ mode::Edit }, m_selectedText{} {}

// проверять индекс через at() смысла нет, 
//так как возвращаемый getPositionObject() курсор, всегда меньше или равен максимально возможной позиции.

void Cursor::cursorLeft(const Container& container){
	auto& cursor{ getPositionObject() };

	if (cursor.m_col > constants::LINE_BEGIN)
		--cursor.m_col;
	else if (cursor.m_row > constants::LINE_BEGIN)
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
	else if (cursor.m_row < container.size() - 1)
	{
		cursorDown(container);
		cursor.m_col = constants::LINE_BEGIN;
	}
}

void Cursor::cursorDown(const Container& container)
{
	auto& cursor{ getPositionObject() };
	if (cursor.m_row < container.size() - 1 && /*check next row  cursor column position*/cursor.m_col > container[++cursor.m_row].length()) {
		cursor.m_col = container[cursor.m_row].length();
	}
}

void Cursor::cursorUp(const Container& container)
{
	auto& cursor{ getPositionObject() };
	if (cursor.m_row > constants::LINE_BEGIN && /*check prev row  cursor column position*/ cursor.m_col > container[--cursor.m_row].length()) {
		cursor.m_col = container[cursor.m_row].length();
	}
}

void Cursor::setCursor(unsigned row, unsigned col, const Container& container)
{
	setCursor(position(row, col),  container);
}

void Cursor::setCursor(const position& pos, const Container& container)
{
	auto& cursor{ getPositionObject() };
	if (pos <= maxPosition(container) && /* check current row lenght*/pos.m_col <= currentRowMaxCol(pos.m_row, container))
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
	cancelReverseSelection();
}

void Cursor::continueSelection()
{
	// если курсор в конце и режим не равен режиму выделения, значит мы закончили выделение и остались на той же позиции(тогда продолжаем выделять)
	if (m_cursor == m_selectedText.to() && m_currentMode == mode::Edit)
		m_currentMode = mode::Select;
	else
		throw std::logic_error(errorMessage::BAD_CONTINUE_SELECTION);
}

void Cursor::resetSelection() noexcept{
	// если выбран режим выделения значит, сбрасываем его и возращаем курсор в начало выделения, иначе ничего 
	if (m_currentMode == mode::Select) {
		m_currentMode = mode::Edit;
		m_cursor = m_selectedText.from(); // при наличии режима выделения эта строка не к чему, 
										  //однако я хочу в будущем от него отказаться, так как думаю,
										  //что смогу реализовать эквивалентный функционал и без него и там она пригодится.
	}
}

std::string Cursor::getSelectedText(const Container& container)
{
	//выделение текста лежит в следующих границах [from, to) 
	if (m_currentMode == mode::Select)
		throw std::logic_error(errorMessage::SELECTING_NOT_FINISHED);

	cancelReverseSelection();
	std::string selectedText{};
	// if selected one row, but no one columns, get this substr
	if ((m_selectedText.from().m_row == m_selectedText.to().m_row) && (m_selectedText.from() != m_selectedText.to())) {
		addSingleRow(selectedText, container);
	}
	else {
		if (m_selectedText.from().m_row != m_selectedText.to().m_row) {
			addFirstRowFromMiltilineSelection(selectedText, container);
		}

		if ((m_selectedText.to() - m_selectedText.from()) >= 1) {
			multilineRowSelection(selectedText, container);
		};
	}
	//Return value optimization
	return selectedText;
}