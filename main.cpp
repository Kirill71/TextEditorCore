#include"texteditorcore.hpp"
#include<fstream>
#include<iostream>

/*---------------------------------------------------------------------------*/

void 
inserter( TextEditorCore& _t, const position& _from,  std::string& _text) {
	static unsigned counter{};
	++counter;
	std::cout << "Test #" << counter << std::endl;
	_t.setCursor( _from );
	_t.insert( _text );
	std::cout << _t;
	std::cout << _t.getCursorPosition();
	std::cout << " ==========================================================" << std::endl;

} // inserter

/*---------------------------------------------------------------------------*/

void 
deleter(TextEditorCore& _t, const position& _from, const position& to ) {
	static unsigned counter{};
	++counter;
	std::cout << "Test #" << counter << std::endl;
	_t.setCursor( _from ).startSelection().setCursor( 2,1 );
	_t.removeSelectedText();
	std::cout << _t;
	std::cout << _t.getCursorPosition();
	std::cout << " ==========================================================" << std::endl;

} // deleter

/*---------------------------------------------------------------------------*/
