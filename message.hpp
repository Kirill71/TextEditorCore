#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include<vector>
#include<string>
// I think about insertion in middle of sequense and I think about list<T>
using Container = std::vector<std::string>;

/*������� ������ ���������,
����� �� ������ ������ ������������� ������, ������������ ������ � 1000 ���������.*/
const unsigned DEFAULT_DOCUMENT_SIZE = 1000;
const char* END_OF_LINE = "\n";
namespace errorMessage  
{
	const char* INVALID_POSITION = "Invalid position of cursor";
};
 
#endif // !MESSAGE_HPP

