#include "texteditorcore.hpp"
#include<iterator>

TextEditorCore::TextEditorCore() : m_container{ DEFAULT_DOCUMENT_SIZE }, m_cursor{std::make_unique<Cursor>()}
{}

TextEditorCore::TextEditorCore(std::istream & stream) : TextEditorCore()
{
	// противоречит SOLIDу, но да ладно.P.S. Я бы метод чтения и записи в поток
	//вынес в отдельный класс и сделал его частью ТеxtEditorCore 
	m_container
		.assign(( std::istream_iterator<std::string>{ stream }), 
			std::istream_iterator<std::string>{});
	
	/* Если загружен пустой файл - я это ошибкой не считаю,
	т.к открытие пустого документа,
	поддерживают все известные мне тектовые редакторы.*/
}

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
