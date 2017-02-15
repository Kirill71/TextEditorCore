#include<iostream>
#include<fstream>
#include"texteditorcore.hpp"

int main() {

	TextEditorCore t(std::ifstream("file.txt"));
	//t.write(std::cout);
	std::string str{ "Pussy You\n little pussy\n" };
	t.insert(str);
	t.startSelection()
		.setCursor(0, 0)
		.finishSelection();
		t.removeSelectedText();
	t.write(std::cout);
	system("pause");
}