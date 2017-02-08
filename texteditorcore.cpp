#include "texteditorcore.hpp"
#include<iterator>
#include<sstream>
#include<list>
TextEditorCore::TextEditorCore() 
	: m_container{ DEFAULT_DOCUMENT_SIZE },
	m_cursor{std::make_unique<Cursor>()}
{}

TextEditorCore::TextEditorCore(std::istream & stream) 
	: TextEditorCore()
{
	// противоречит SOLIDу, но да ладно.
	//	P.S. Я бы метод чтения и записи в поток
	//вынес в отдельный класс и сделал его частью ТеxtEditorCore 
	m_container
		.assign(( customIterator::LineInputIterator<>{ stream }), 
			customIterator::LineInputIterator<>{});
	
	/* Если загружен пустой файл - я это ошибкой не считаю,
	т.к открытие пустого документа,
	поддерживают все известные мне тектовые редакторы.*/
}
// cursor methods
TextEditorCore& TextEditorCore::cursorLeft()
{
	m_cursor->cursorLeft(m_container);
	return *this;
}

TextEditorCore& TextEditorCore::cursorRight()
{
	m_cursor->cursorRight(m_container);
	return *this;
}

TextEditorCore& TextEditorCore::cursorDown()
{
	m_cursor->cursorDown(m_container);
	return *this;
}

TextEditorCore& TextEditorCore::cursorUp()
{
	m_cursor->cursorUp(m_container);
	return *this;
}

TextEditorCore& TextEditorCore::setCursor(unsigned row, unsigned col)
{
	m_cursor->setCursor(row, col, m_container);
	return *this;
}

TextEditorCore& TextEditorCore::setCursor(const position & pos)
{
	m_cursor->setCursor(pos, m_container);
	return *this;
}

TextEditorCore & TextEditorCore::write(std::ostream& stream)
{  
	std::copy(m_container.cbegin(), m_container.cend(),
		std::ostream_iterator<std::string>(stream, END_OF_LINE));
	return *this;
}

// insertion methods
TextEditorCore & TextEditorCore::insert(char)
{
	// TODO: вставьте здесь оператор return
}

TextEditorCore & TextEditorCore::insert(const char *)
{
	// TODO: вставьте здесь оператор return
}


// private methods
void TextEditorCore::insertText(const position& pos,  std::string& text) noexcept
{
	if (text.empty()) // empty input string => return
		return;

	//  remember end of string
	std::string  end_of_current_string{ m_container.at(pos.m_row).substr(pos.m_col, m_container.at(pos.m_row).length() - pos.m_col) };
	// delete end of string from container
	m_container.at(pos.m_row).erase(m_container.at(pos.m_row).begin() + pos.m_col, m_container.at(pos.m_row).end());
	//get  first line from input text
	std::string firstLine = text.substr(FIRST_INDEX, text.find_first_of(END_OF_LINE, FIRST_INDEX) - FIRST_INDEX);
	// add this line in container on end(variable) position
	m_container.at(pos.m_row).append(firstLine);
	// delete first line from  input text
	text.erase(FIRST_INDEX, firstLine.length());


	bool is_new_line_need = text.back() == END_OF_LINE_CHAR;
	auto begin{ customIterator::LineInsertIterator<>(std::stringstream{ text },is_new_line_need) },
		end{ customIterator::LineInsertIterator<>() };


	m_container.insert(m_container.cbegin() + pos.m_row, begin, end);
	// row position after insertion input text

	unsigned added_rows{
		std::distance(customIterator::LineInsertIterator<>(std::stringstream{ text},is_new_line_need),
		customIterator::LineInsertIterator<>()) }, current_row{ pos.m_row + added_rows };
	// end string append to


	m_container.at(current_row).append(end_of_current_string);
	setCursor(position(current_row,m_container.at(current_row).length()));
}
