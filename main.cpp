#include"texteditorcore.hpp"
#include<fstream>
#include<iostream>
void inserter(TextEditorCore& t, const position& from,  std::string& text) {
	static unsigned counter{};
	++counter;
	std::cout << "Test #" << counter << std::endl;
	t.setCursor(from);
	t.insert(text);
	std::cout << t;
	std::cout << t.getCursorPosition();
	std::cout << " ==========================================================" << std::endl;
}

void deleter(TextEditorCore& t, const position& from, const position& to) {
	static unsigned counter{};
	++counter;
	std::cout << "Test #" << counter << std::endl;
	t.setCursor(from).startSelection().setCursor(2,1);
	t.removeSelectedText();
	std::cout << t;
	std::cout << t.getCursorPosition();
	std::cout << " ==========================================================" << std::endl;
}
int main() {
	TextEditorCore t(std::ifstream{ "file.txt" });
	TextEditorCore t1(std::ifstream{ "file.txt" });
	TextEditorCore t2(std::ifstream{ "file.txt" });
	TextEditorCore t3(std::ifstream{ "file.txt" });
	TextEditorCore t4(std::ifstream{ "file.txt" });
	TextEditorCore t5(std::ifstream{ "file.txt" });
	TextEditorCore t6(std::ifstream{ "file.txt" });
	TextEditorCore t7(std::ifstream{ "file.txt" });
	TextEditorCore t8(std::ifstream{ "file.txt" });
	TextEditorCore t9(std::ifstream{ "file.txt" });
	TextEditorCore t10(std::ifstream{ "file.txt" });
	inserter(t,position{ 0,0 }, std::string{ "Jully\n My little Jully\n" });
	deleter(t, position{ 0,0 }, position{1,16});
//	t.insert(std::string{ "bla-bla-bla" });
//	inserter(t1, position{ 1,0 }, std::string{ "Jully little Jully\n" });
//	inserter(t2, position{ 1,4 }, std::string{ "Jully little Jully\n" });
//	inserter(t3, position{ 0,0 }, std::string{ "Jully little Jully" });
//	inserter(t4, position{ 0,4 }, std::string{ "Jully little Jully\n" });
//	inserter(t5, position{ 0,0 }, std::string{ "Jully little\n Jully\n" });
//	inserter(t6, position{ 0,5 }, std::string{ "Jully little\n Jully\n" });
//	inserter(t7, position{ 1,3 }, std::string{});
//	inserter(t8, position{ 0,0 }, std::string{ constants::END_OF_LINE });
//	inserter(t9, position{ 0,4 }, std::string{ constants::END_OF_LINE });
//	inserter(t10, position{ 0,21 }, std::string{ constants::END_OF_LINE });
//	std::cout << t;
system("pause");
}