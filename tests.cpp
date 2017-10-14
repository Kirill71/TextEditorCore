#include "testslib.hpp"
#include "texteditorcore.hpp"
#include<fstream>
#include <cassert>
#include <sstream>


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST( emptyConstructor ) {
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
		std::string str = t1.getSelectedText();
	assert(!str.compare(std::string{ "\n\n" }));

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
	
} // emptyConstructor


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST( istreamConstructor )
{

//	setlocale(LC_ALL, "Russian");
	TextEditorCore t1{ std::ifstream("test.txt") };
	assert( t1.getCursorPosition() == position{} );//initial position is  0,0

}; // instreamConstractor


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST( getCursorPositionAndCursorMoving )
{
	TextEditorCore t1{};
	assert(t1.getCursorPosition() == position{});

	TextEditorCore t2{ std::ifstream{ "test.txt" } };

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


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST( insertion ) 
{
	TextEditorCore t1{};

	position t1_p = t1.getCursorPosition();
	t1.insert( "0123456789");

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

}; // insertion


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST( keys )
{
	TextEditorCore t1{ std::ifstream("test.txt") };

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

}; // keys


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST(write)
{

	TextEditorCore t{ std::ifstream{ "file.txt" }};

	std::ofstream ofile("new.txt");

	ofile << t;
	ofile.close();

	std::ifstream ifile1("file.txt");
	std::ifstream ifile2("new.txt");
	ifile1.close();
	ifile2.close();
	std::stringstream first,second;

	first << ifile1.rdbuf();
	second << ifile2.rdbuf();
	assert( !first.str().compare(second.str()));
	std::remove("new.txt");

}; // write


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST( selection ) {
	std::ifstream  infile("test.txt");
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

	got = t1.CtrlHomeKeyPressed()
			.cursorRight()
			.startSelection()
			.cursorDown()
			.cursorDown()
			.cursorRight()
			.cursorDown()
			.finishSelection()
			.getSelectedText();
	assert( !got.compare("12345678901234567890123456789\n012345678901234567890123456789\n012\n01"));

	t1.CtrlEndKeyPressed();
		t1.startSelection();
		t1.cursorUp();
		t1.cursorUp();
		t1.cursorUp();
		t1.cursorUp();
	t1.finishSelection();
	got = t1.getSelectedText();

	assert(!got.compare("01234\n\n\n\n012345678901234567890123456789"));

}; // selection


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST( delete_selected ) {
	std::ifstream  infile("test.txt");
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

	assert( !got.compare( "012\n01234\n" ) );

}; // delete_selected


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST( find ) {
	std::ifstream  infile("test.txt");
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
}; // find


/*---------------------------------------------------------------------------*/


DECLARE_OOP_TEST( replace ) 
{
	TextEditorCore t1(std::ifstream("test.txt"));

	t1.replaceAll("0123456789", "*");
	t1.replaceAll("01234", "");
	t1.replaceAll("012", "");
	t1.replaceAll("01", "");
	t1.replaceAll("*", "");

	assert(t1.replace("0","") == false);
	std::string str = t1.CtrlHomeKeyPressed().startSelection().CtrlEndKeyPressed().finishSelection(). getSelectedText();
	assert(! str.compare( "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" ));

}; // replace


/*---------------------------------------------------------------------------*/