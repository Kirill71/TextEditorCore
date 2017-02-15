#include<iostream>
#include<fstream>
#include"texteditorcore.hpp"

int main() {

	TextEditorCore t(std::ifstream("file.txt"));
	//t.write(std::cout);
	std::string str{ "Pussy You\n little pussy\n" };
	std::cout<<t.insert(str).find("my name");
	t.write(std::cout);
	system("pause");
}