#include "texteditorcore.hpp"

/* Ѕазовый размер документа,
чтобы не делать частые перевыделени€ пам€ти, устанавливаю размер в 1000 элементов.*/
const unsigned DEFAULT_DOCUMENT_SIZE = 1000;
const char END_OF_LINE = '\n';

TextEditorCore::TextEditorCore() : currentMode{ mode::Edit },
coursor{}, selection{}, find{}, container{ DEFAULT_DOCUMENT_SIZE }
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




