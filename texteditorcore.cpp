#include "texteditorcore.hpp"

/* ������� ������ ���������,
����� �� ������ ������ ������������� ������, ������������ ������ � 1000 ���������.*/
const unsigned DEFAULT_DOCUMENT_SIZE = 1000;
const char END_OF_LINE = '\n';

TextEditorCore::TextEditorCore() : currentMode{ mode::Edit },
coursor{}, selection{}, find{}, container{ DEFAULT_DOCUMENT_SIZE }
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




