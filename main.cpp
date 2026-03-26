#include "texteditorcore.hpp"
#include<fstream>
#include<iostream>

#include "position.hpp"


/*---------------------------------------------------------------------------*/


void 
inserter( TextEditorCore& _t, const position& _from,  const std::string& _text) {
	static size_t counter{};
	++counter;
	std::cout << "Test #" << counter << std::endl;
	_t.setCursor( _from );
	_t.insert( _text );
	std::cout << _t.getCursorPosition();
	std::cout << " ==========================================================" << std::endl;

} // inserter


/*---------------------------------------------------------------------------*/


void 
deleter(TextEditorCore& _t, const position& _from, const position& _to ) {
	static size_t counter{1};
	++counter;
	std::cout << "Test #" << counter << std::endl;
	_t.setCursor( _from ).startSelection().setCursor( _to );
	_t.removeSelectedText();
	std::cout << _t.getCursorPosition();
	std::cout << " ==========================================================" << std::endl;

} // deleter


/*---------------------------------------------------------------------------*/

int main() {
	TextEditorCore t;
	const std::string text = "Just some text";
	inserter( t, t.getCursorPosition(), text);
	deleter( t, {0,0}, {2,1} );
}