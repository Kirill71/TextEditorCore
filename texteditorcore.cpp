#include "texteditorcore.hpp"
/* ������� ������ ���������,
����� �� ������ ������ ������������� ������, ������������ ������ � 1000 ���������.*/
const unsigned DEFAULT_DOCUMENT_SIZE = 1000;
const char END_OF_LINE = '\n';

using namespace  errorMessage;

TextEditorCore::TextEditorCore() : container{ DEFAULT_DOCUMENT_SIZE }, cursor{std::make_unique<Cursor>()}
{}

TextEditorCore::TextEditorCore( std::istream & stream): TextEditorCore()
{
	//TODO: �������� ��� ����������� ���������� getline � ������� �����.
	std::string tempLine;
	while( !stream.eof() )
	{
		std::getline(stream, tempLine, END_OF_LINE);
		container.push_back(tempLine);
		tempLine.clear();
	}
	/* ���� �������� ������ ���� - � ��� ������� �� ������,
	�.� �������� ������� ���������,
	������������ ��� ��������� ��� �������� ���������.*/
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
