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
	std::cout << t;
std::cout<< t.find("student");

system("pause");
}