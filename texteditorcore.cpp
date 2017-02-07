#include "texteditorcore.hpp"
/* Ѕазовый размер документа,
чтобы не делать частые перевыделени€ пам€ти, устанавливаю размер в 1000 элементов.*/
const unsigned DEFAULT_DOCUMENT_SIZE = 1000;
const char END_OF_LINE = '\n';

using namespace  errorMessage;

TextEditorCore::TextEditorCore() : container{ DEFAULT_DOCUMENT_SIZE }, cursor{std::make_unique<Cursor>()}
{}

TextEditorCore::TextEditorCore( std::istream & stream): TextEditorCore()
{
	//TODO: подумать про возможность добавление getline в условие цикла.
	std::string tempLine;
	while( !stream.eof() )
	{
		std::getline(stream, tempLine, END_OF_LINE);
		container.push_back(tempLine);
		tempLine.clear();
	}
	/* ≈сли загружен пустой файл - € это ошибкой не считаю,
	т.к открытие пустого документа,
	поддерживают все известные мне тектовые редакторы.*/
}

TextEditorCore& TextEditorCore::cursorLeft()
{
	cursor->cursorLeft(container);
	return *this;
}

TextEditorCore & TextEditorCore::cursorRight()
{
	cursor->cursorRight(container);
	return *this;
}

TextEditorCore & TextEditorCore::cursorDown()
{
	cursor->cursorDown(container);
	return *this;
}

TextEditorCore & TextEditorCore::cursorUp()
{
	cursor->cursorUp(container);
	return *this;
}

TextEditorCore & TextEditorCore::setCursor(unsigned row, unsigned col)
{
	cursor->setCursor(row, col, container);
	return *this;
}

TextEditorCore & TextEditorCore::setCursor(const position & pos)
{
	cursor->setCursor(pos, container);
	return *this;
}
