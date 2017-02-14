#include<iostream>
#include<fstream>
#include"texteditorcore.hpp"

int main() {

	TextEditorCore t(std::ifstream("file.txt"));
	//t.write(std::cout);
	t.setCursor(1, 0);
	std::string str{ "Pussy You little pussy" };
	t.insert(str);
	t.write(std::cout);
	std::cout << t.getCoursorPos();
	system("pause");
}