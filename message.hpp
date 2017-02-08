#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include<vector>
#include<string>
// I think about insertion in middle of sequense and I think about list<T>
using Container = std::vector<std::string>;

/*Базовый размер документа,
чтобы не делать частые перевыделения памяти, устанавливаю размер в 1000 элементов.*/
const unsigned DEFAULT_DOCUMENT_SIZE = 1000;
const unsigned  FIRST_INDEX = 0;
const char* END_OF_LINE = "\n";
const char END_OF_LINE_CHAR = '\n';
const char* SPACE = " ";
namespace errorMessage  
{
	const char* INVALID_POSITION = "Invalid position of cursor";
};
 
#endif // !MESSAGE_HPP

