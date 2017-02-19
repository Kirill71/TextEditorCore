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
//int main() {
//	TextEditorCore t(std::ifstream{ "file.txt" });
//	TextEditorCore t1(std::ifstream{ "file.txt" });
//	TextEditorCore t2(std::ifstream{ "file.txt" });
//	TextEditorCore t3(std::ifstream{ "file.txt" });
//	TextEditorCore t4(std::ifstream{ "file.txt" });
//	TextEditorCore t5(std::ifstream{ "file.txt" });
//	TextEditorCore t6(std::ifstream{ "file.txt" });
//	TextEditorCore t7(std::ifstream{ "file.txt" });
//	TextEditorCore t8(std::ifstream{ "file.txt" });
//	TextEditorCore t9(std::ifstream{ "file.txt" });
//	TextEditorCore t10(std::ifstream{ "file.txt" });
//	inserter(t,position{ 0,0 }, std::string{ "Pussy little pussy\n" });
//	t.insert(std::string{ "bla-bla-bla" });
//	inserter(t1, position{ 1,0 }, std::string{ "Pussy little pussy\n" });
//	inserter(t2, position{ 1,4 }, std::string{ "Pussy little pussy\n" });
//	inserter(t3, position{ 0,0 }, std::string{ "Pussy little pussy" });
//	inserter(t4, position{ 0,4 }, std::string{ "Pussy little pussy\n" });
//	inserter(t5, position{ 0,0 }, std::string{ "Pussy little\n pussy\n" });
//	inserter(t6, position{ 0,5 }, std::string{ "Pussy little\n pussy\n" });
//	inserter(t7, position{ 1,3 }, std::string{});
//	inserter(t8, position{ 0,0 }, std::string{ constants::END_OF_LINE });
//	inserter(t9, position{ 0,4 }, std::string{ constants::END_OF_LINE });
//	inserter(t10, position{ 0,21 }, std::string{ constants::END_OF_LINE });
//	std::cout << t;
//	system("pause");
//}