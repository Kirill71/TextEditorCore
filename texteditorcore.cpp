#include "texteditorcore.hpp"
/* ������� ������ ���������,
����� �� ������ ������ ������������� ������, ������������ ������ � 1000 ���������.*/
const unsigned DEFAULT_DOCUMENT_SIZE = 1000;
const char END_OF_LINE = '\n';

using namespace  errorMessage;

TextEditorCore::TextEditorCore() : m_container{ DEFAULT_DOCUMENT_SIZE }, m_cursor{std::make_unique<Cursor>()}
{}

TextEditorCore::TextEditorCore( std::istream & stream): TextEditorCore()
{
	//TODO: �������� ��� ����������� ���������� getline � ������� �����.
	std::string tempLine;
	while( !stream.eof() )
	{
		std::getline(stream, tempLine, END_OF_LINE);
		m_container.push_back(tempLine);
		tempLine.clear();
	}
	/* ���� �������� ������ ���� - � ��� ������� �� ������,
	�.� �������� ������� ���������,
	������������ ��� ��������� ��� �������� ���������.*/
}

TextEditorCore& TextEditorCore::cursorLeft()
{
	m_cursor->cursorLeft(m_container);
	return *this;
}

TextEditorCore & TextEditorCore::cursorRight()
{
	m_cursor->cursorRight(m_container);
	return *this;
}

TextEditorCore & TextEditorCore::cursorDown()
{
	m_cursor->cursorDown(m_container);
	return *this;
}

TextEditorCore & TextEditorCore::cursorUp()
{
	m_cursor->cursorUp(m_container);
	return *this;
}

TextEditorCore & TextEditorCore::setCursor(unsigned row, unsigned col)
{
	m_cursor->setCursor(row, col, m_container);
	return *this;
}

TextEditorCore & TextEditorCore::setCursor(const position & pos)
{
	m_cursor->setCursor(pos, m_container);
	return *this;
}
