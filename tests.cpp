#include "testslib.hpp"
#include "texteditorcore.hpp"
#include<fstream>
#include <cassert>
#include <sstream>



//note: if: c_strings are equal strcmp => ret 0
//		if: strings are equal std::basic_string::compare => ret 0

DECLARE_OOP_TEST(empty_constructor) {
	TextEditorCore t1{};
	position t1_p = t1.getCursorPosition();

	assert(t1_p == position(0, 0));

	t1
		.cursorDown()
		.cursorRight();

	assert(t1_p == position(0, 0));

	t1
		.cursorUp()
		.cursorLeft();
	assert(t1_p == position(0, 0));

	try {
		t1.setCursor(position{ 100, 100 });
	}
	catch (std::logic_error err) {
		assert(errorMessage::INVALID_POSITION == err.what());
	};
	t1.startSelection();
	t1.cursorDown();
		t1.cursorDown();
		t1.finishSelection();
		
	assert(!t1.getSelectedText().compare(std::string{ "\n\n\n" }));

	try {
		t1.find("Hello");
	}
	catch (std::logic_error err) {
		// думаю исправить.
		assert(errorMessage::TEXT_NOT_FOUND == err.what());
	};
	try {
		assert(t1.replaceAll("", "Hello"));
	}
	catch (std::logic_error err) {
		assert(errorMessage::EMPTY_REPLACE_STRING == err.what());
	}
	
}




DECLARE_OOP_TEST(istream_constructor) {

//	setlocale(LC_ALL, "Russian");
	TextEditorCore t1{ std::ifstream("loremIO.txt") };
	assert(t1.getCursorPosition() == position{});//initial position is  0,0
};

DECLARE_OOP_TEST(getCursorPosition_and_cursormoving) {
	TextEditorCore t1{};
	assert(t1.getCursorPosition() == position{});

	TextEditorCore t2{ std::ifstream{ "loremIO.txt" } };

	assert(t2.getCursorPosition() == position{});
	const unsigned SIZE{ 50 };

	for (unsigned i ={0}; i <=SIZE; ++i) {
		for (unsigned j{ 0 }; j<= SIZE; ++j) {
			try {
				t2.setCursor(position{ i, j });
				assert(t2.getCursorPosition() == position(i, j));
			}
			catch (std::logic_error err) {
				assert(errorMessage::INVALID_POSITION == err.what());
			}
		}
	}

	assert(t2.getCursorPosition() == position(18, 30));
	for (unsigned i{0}; i <= SIZE; ++i) {
		for (unsigned j{0}; j <= SIZE; ++j) {
				t2.cursorLeft();
		}
	}

	assert(t2.getCursorPosition() == position(0, 0));

	for (unsigned i = 0; i <= SIZE; ++i) {
		for (unsigned j = 0; j <= SIZE; ++j) {
			t2.cursorRight();
		}
	}
	
	assert(t2.getCursorPosition() == position( 18, 30));


	t2.setCursor(position(0, 0));

	for (unsigned i{0}; i <= SIZE; ++i) {
			t2.cursorDown();
	}
	
	assert(t2.getCursorPosition() == position(18, 0));

	for (unsigned i{0}; i <= SIZE; ++i) {
		t2.cursorUp();
	}

	assert(t2.getCursorPosition() == position(0, 0));


	t2.setCursor( position{ 1,30 } );
	t2.cursorDown();

	assert(t2.getCursorPosition() == position(2, 3));



	t2.setCursor(position{ 5,30 });
	t2.cursorUp();

	assert(t2.getCursorPosition() == position(4, 2));


	t2.setCursor(position{ 0,30 });
	t2.cursorRight();

	assert(t2.getCursorPosition() == position(1, 0));

	t2.cursorLeft();
	assert(t2.getCursorPosition() == position(0, 30));
};

DECLARE_OOP_TEST(insertion) {
	TextEditorCore t1{};

	position t1_p = t1.getCursorPosition();
	t1.insert(std::string{ "0123456789" });

	try {
		
		t1.setCursor(position{ 0, 30 });
	} 
	catch ( std::logic_error err ) {
		assert(errorMessage::INVALID_POSITION == err.what());
	};

	t1.insert('\n');

	t1.setCursor(position{ 1, 0 });

	t1.insert(std::string{ "\n0123456789\n0123456789\n" });

	t1.setCursor(position{ 4, 0 });

	assert( t1.getCursorPosition() == position( 4, 0 ) );
};


DECLARE_OOP_TEST( keys ) {
	TextEditorCore t1{ std::ifstream("loremIO.txt") };

	t1.EndKeyPressed();
	assert(t1.getCursorPosition()  == position(0, 30));
	
	t1.CtrlEndKeyPressed();
	assert(t1.getCursorPosition() == position(18, 30));
	
	t1.HomeKeyPressed();
	assert(t1.getCursorPosition() == position(18, 0));

	t1.CtrlHomeKeyPressed();
	assert(t1.getCursorPosition() == position(0, 0));
	
	TextEditorCore t2{};

	t2.EndKeyPressed();
	assert(t2.getCursorPosition() == position(0, 0));

	t2.CtrlEndKeyPressed();
	assert(t2.getCursorPosition() == position(t2.maxPosition()));

	t2.HomeKeyPressed();
	assert(t2.getCursorPosition() == position(t2.maxPosition().m_row,0));

	t2.CtrlHomeKeyPressed();
	assert(t2.getCursorPosition() == position(0, 0));
};


DECLARE_OOP_TEST(write) {

TextEditorCore t{ std::ifstream{ "loremIO.txt" }};

std::ofstream ofile("newIO.txt");

t.write(std::ofstream{ "newIO.txt" });

// check files are equal

std::ifstream ifile1("loremIO.txt");
std::ifstream ifile2("newIO.txt");

std::stringstream first,second;

first << ifile1.rdbuf();
second << ifile2.rdbuf();

assert( !first.str().compare( second.str()) );
std::remove("newIO.txt");
};

DECLARE_OOP_TEST(selection) {
	std::ifstream  infile("loremIO.txt");
	TextEditorCore t1(infile);
	infile.close();

	std::string got = t1
		.startSelection()
		.setCursor(position(0, 30))
		.finishSelection().getSelectedText();

	
	assert(got ==  "012345678901234567890123456789" );

	
	got = t1
			.startSelection()
			.setCursor(position(0, 30))
			.resetSelection()
			.finishSelection()
			.getSelectedText();

	assert(got == std::string{});



	//got = t1.CtrlHomeKeyPressed()
	//		.cursorRight()
	//		.startSelection()
	//		.cursorDown()
	//		.cursorDown()
	//		.cursorRight()
	//		.cursorDown()
	//		.finishSelection()
	//		.getSelectedText();
	////error
	//assert( !got.compare("12345678901234567890123456789\n012345678901234567890123456789\n012\n01234\n01\n"));


	//got = t1.CtrlEndKeyPressed()
	//		.startSelection()
	//		.cursorUp()
	//		.cursorUp()
	//		.cursorUp()
	//		.cursorUp()
	//		.finishSelection()
	//		.getSelectedText();

	//assert(!got.compare("01234\n\n\n\n012345678901234567890123456789"));


};

DECLARE_OOP_TEST(delete_selected) {
	std::ifstream  infile("loremIO.txt");
	TextEditorCore t1(infile);
	infile.close();

	t1.startSelection()
		.setCursor(position(1, 30))
		.finishSelection()
		.removeSelectedText();


	std::string got = t1.startSelection()
					.cursorDown()
					.cursorDown().finishSelection()
					.getSelectedText();

	assert( got.compare( "012\n01234\n" ) );
};

DECLARE_OOP_TEST(find) {
	std::ifstream  infile("loremIO.txt");
	TextEditorCore t1(infile);
	infile.close();

	position f = t1.find("012345");


	assert(f == position{});



	f = t1.findNext();
	assert( f == position( 0,10 ) );


	f = t1.findNext();

	assert(f == position(0, 20));

	f = t1.findNext();
	assert(f == position(1, 0));


	try {
		f = t1.find("");
	} catch (std::logic_error err) {
		assert(errorMessage::EMPTY_SEARCH_STRING == err.what());
	};

		f = t1.findNext();
		assert(f == position(1,10));
		
		try {
			f = t1.find("Hello");
		}
		catch (std::logic_error err) {
			assert(errorMessage::TEXT_NOT_FOUND == err.what());
		}		
};



DECLARE_OOP_TEST(replace) {//replacing non multiline 
	std::ifstream ("loremIO.txt");
	TextEditorCore t1(std::ifstream("loremIO.txt"));

	t1.replaceAll("0123456789", "*");
	t1.replaceAll("01234", "");
	t1.replaceAll("012", "");
	t1.replaceAll("01", "");
	t1.replaceAll("*", "");

	assert(t1.replace("0","") == false);
	std::string str = t1.CtrlHomeKeyPressed().startSelection().CtrlEndKeyPressed().finishSelection(). getSelectedText();
	assert(! str.compare( "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" ));
};