
#include "texteditorcore.hpp"
#include<fstream>
#include <sstream>
#include <gtest/gtest.h>


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, emptyConstructor ) {
	TextEditorCore t1{};
	EXPECT_TRUE(t1.getCursorPosition() == position(0, 0));

	t1
		.cursorDown()
		.cursorRight();

	EXPECT_TRUE(t1.getCursorPosition() == position(1, 1));

	t1
		.cursorUp()
		.cursorLeft();
	EXPECT_TRUE( t1.getCursorPosition() == position(0, 0));

	try {
		t1.setCursor(position{ 100, 100 });
	}
	catch (std::logic_error& err) {
		EXPECT_TRUE(errorMessage::INVALID_POSITION == err.what());
	};
	t1.startSelection();
	t1.cursorDown();
		t1.cursorDown();
		t1.finishSelection();
	const std::string str = t1.getSelectedText();
	EXPECT_FALSE(str == "\n\n" );
	EXPECT_TRUE(str == " \n \n" );

	try {
		t1.find("Hello");
	}
	catch (std::logic_error& err) {
		EXPECT_TRUE(errorMessage::TEXT_NOT_FOUND == err.what());
	};
	try {
		EXPECT_TRUE(t1.replaceAll("", "Hello"));
	}
	catch (std::logic_error& err) {
		EXPECT_TRUE(errorMessage::EMPTY_REPLACE_STRING == err.what());
	}
	
} // emptyConstructor


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, istreamConstructor )
{

	std::ifstream stream{"test.txt"};

	TextEditorCore t1{ stream };
	EXPECT_TRUE( t1.getCursorPosition() == position{} );//initial position is  0,0

}; // instreamConstractor


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, getCursorPositionAndCursorMoving )
{
	TextEditorCore t1{};
	EXPECT_TRUE(t1.getCursorPosition() == position{});

	std::ifstream stream{"test.txt"};
	TextEditorCore t2{ stream };

	EXPECT_TRUE(t2.getCursorPosition() == position{});
	constexpr size_t size{ 50 };

	for (size_t i = 0; i <= size; ++i) {
		for (size_t j = 0; j <= size; ++j) {
			try {
				t2.setCursor(position{ i, j });
				EXPECT_TRUE(t2.getCursorPosition() == position(i, j));
			}
			catch (std::logic_error& err) {
				EXPECT_TRUE(errorMessage::INVALID_POSITION == err.what());
			}
		}
	}

	EXPECT_TRUE(t2.getCursorPosition() == position(18, 30));
	for (size_t i{0}; i <= size; ++i) {
		for (size_t j{0}; j <= size; ++j) {
				t2.cursorLeft();
		}
	}

	EXPECT_TRUE(t2.getCursorPosition() == position(0, 0));

	for (size_t i = 0; i <= size; ++i) {
		for (size_t j = 0; j <= size; ++j) {
			t2.cursorRight();
		}
	}
	
	EXPECT_TRUE(t2.getCursorPosition() == position( 18, 30));


	t2.setCursor(position(0, 0));

	for (size_t i{0}; i <= size; ++i) {
			t2.cursorDown();
	}
	
	EXPECT_TRUE(t2.getCursorPosition() == position(18, 0));

	for (size_t i{0}; i <= size; ++i) {
		t2.cursorUp();
	}

	EXPECT_TRUE(t2.getCursorPosition() == position(0, 0));


	t2.setCursor( position{ 1,30 } );
	t2.cursorDown();

	EXPECT_TRUE(t2.getCursorPosition() == position(2, 3));



	t2.setCursor(position{ 5,30 });
	t2.cursorUp();

	EXPECT_TRUE(t2.getCursorPosition() == position(4, 2));


	t2.setCursor(position{ 0,30 });
	t2.cursorRight();

	EXPECT_TRUE(t2.getCursorPosition() == position(1, 0));

	t2.cursorLeft();
	EXPECT_TRUE(t2.getCursorPosition() == position(0, 30));
};


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, insertion )
{
	TextEditorCore t1{};

	position t1_p = t1.getCursorPosition();
	t1.insert( "0123456789");

	try {
		
		t1.setCursor(position{ 0, 30 });
	} 
	catch ( std::logic_error& err ) {
		EXPECT_TRUE(errorMessage::INVALID_POSITION == err.what());
	};

	t1.insert('\n');

	t1.setCursor(position{ 1, 0 });

	t1.insert(std::string{ "\n0123456789\n0123456789\n" });

	t1.setCursor(position{ 4, 0 });

	EXPECT_TRUE( t1.getCursorPosition() == position( 4, 0 ) );

}; // insertion


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, keys )
{
	std::ifstream stream{"test.txt"};
	TextEditorCore t1{ stream };

	t1.endKeyPressed();
	EXPECT_TRUE(t1.getCursorPosition()  == position(0, 30));
	
	t1.ctrlEndKeyPressed();
	EXPECT_TRUE(t1.getCursorPosition() == position(18, 30));
	
	t1.homeKeyPressed();
	EXPECT_TRUE(t1.getCursorPosition() == position(18, 0));

	t1.ctrlHomeKeyPressed();
	EXPECT_TRUE(t1.getCursorPosition() == position(0, 0));
	
	TextEditorCore t2{};

	t2.endKeyPressed();
	EXPECT_TRUE(t2.getCursorPosition() == position(0, 1));

	t2.ctrlEndKeyPressed();
	EXPECT_TRUE(t2.getCursorPosition() == position(t2.maxPosition()));

	t2.homeKeyPressed();
	EXPECT_TRUE(t2.getCursorPosition() == position(t2.maxPosition().m_row,0));

	t2.ctrlHomeKeyPressed();
	EXPECT_TRUE(t2.getCursorPosition() == position(0, 0));

} // keys


/*---------------------------------------------------------------------------*/


TEST(TextEditorCoreSuit, write)
{
	std::ifstream stream{"test.txt"};
	TextEditorCore t{ stream };

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
	EXPECT_TRUE( first.str() == second.str() );
	std::remove("new.txt");

} // write


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, selection ) {
	std::ifstream stream("test.txt");
	TextEditorCore t1 {stream};

	std::string selectedText = t1
		.startSelection()
		.setCursor(position(0, 30))
		.finishSelection().getSelectedText();

	EXPECT_TRUE(selectedText == "012345678901234567890123456789" );
	selectedText = t1
			.startSelection()
			.setCursor(position(0, 30))
			.resetSelection()
			.finishSelection()
			.getSelectedText();

	EXPECT_TRUE(selectedText.empty());

	selectedText = t1.ctrlHomeKeyPressed()
			.cursorRight()
			.startSelection()
			.cursorDown()
			.cursorDown()
			.cursorRight()
			.cursorDown()
			.finishSelection()
			.getSelectedText();
	EXPECT_TRUE( selectedText == "12345678901234567890123456789\n012345678901234567890123456789\n012\n01" );

	t1.ctrlEndKeyPressed();
		t1.startSelection();
		t1.cursorUp();
		t1.cursorUp();
		t1.cursorUp();
		t1.cursorUp();
	t1.finishSelection();
	selectedText = t1.getSelectedText();

	EXPECT_TRUE(selectedText == "01234\n\n\n\n012345678901234567890123456789");

} // selection


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, delete_selected ) {
	std::ifstream stream("test.txt");
	TextEditorCore t1{stream};

	t1.startSelection()
		.setCursor(position(1, 30))
		.finishSelection()
		.removeSelectedText();


	const std::string selectedText = t1.startSelection()
					.cursorDown()
					.cursorDown().finishSelection()
					.getSelectedText();

	EXPECT_TRUE( selectedText == "012\n01234\n" );

} // delete_selected


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, find ) {
	std::ifstream stream("test.txt");
	TextEditorCore t1(stream);

	position f = t1.find("012345");

	EXPECT_TRUE(f == position( 0, 0));

	f = t1.findNext();
	EXPECT_TRUE( f == position( 0,10 ) );

	f = t1.findNext();

	EXPECT_TRUE(f == position(0, 20));

	f = t1.findNext();
	EXPECT_TRUE(f == position(1, 0));

	try {
		f = t1.find("");
	} catch (std::logic_error& err) {
		EXPECT_TRUE(errorMessage::EMPTY_SEARCH_STRING == err.what());
	};
		f = t1.findNext();
		EXPECT_TRUE(f == position(1,10));
		
		try {
			f = t1.find("Hello");
		}
		catch (std::logic_error& err) {
			EXPECT_TRUE(errorMessage::TEXT_NOT_FOUND == err.what());
		}		
} // find


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, replace ) {
	std::ifstream stream{ "test.txt" };
	TextEditorCore t1{ stream };

	t1.replaceAll( "0123456789", "*" );
	t1.replaceAll( "01234", "" );
	t1.replaceAll( "012", "" );
	t1.replaceAll( "01", "" );
	t1.replaceAll( "*", "" );

	EXPECT_FALSE( t1.replace("0","") );
	std::string str = t1.ctrlHomeKeyPressed().startSelection().ctrlEndKeyPressed().finishSelection(). getSelectedText();
	EXPECT_TRUE( str == "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" );

} // replace


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, replaceNotFound ) {
	std::ifstream stream("test.txt");
	TextEditorCore t1(stream);

	EXPECT_FALSE( t1.replace("NoSuchTextHere", "x") );
	EXPECT_FALSE( t1.replaceAll("NoSuchTextHere", "x") );

} // replaceNotFound


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, findNotFoundReturnsMaxPosition ) {
	std::ifstream stream("test.txt");
	TextEditorCore t1(stream);

	const position result = t1.find("NoSuchTextHere");
	EXPECT_TRUE( result == t1.maxPosition() );

	const position nextResult = t1.findNext();
	EXPECT_TRUE( nextResult == t1.maxPosition() );

} // findNotFoundReturnsMaxPosition


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, emptyFileConstructor ) {
	std::ifstream stream{ "empty.txt" };
	TextEditorCore t1{ stream };

	EXPECT_TRUE( t1.getCursorPosition() == position{} );
	EXPECT_TRUE( t1.maxPosition() == position{} );

	std::ostringstream out;
	out << t1;
	EXPECT_TRUE( out.str() == "\n" );

} // emptyFileConstructor


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, insertAtDocumentEnd ) {
	std::ifstream stream("test.txt");
	TextEditorCore t1{ stream };

	t1.ctrlEndKeyPressed();
	EXPECT_TRUE( t1.getCursorPosition() == position(18, 30) );

	t1.insert( std::string{ "END" } );
	EXPECT_TRUE( t1.getCursorPosition() == position(18, 33) );

	const std::string lastRow = t1.homeKeyPressed()
		.startSelection()
		.endKeyPressed()
		.finishSelection()
		.getSelectedText();

	EXPECT_TRUE( lastRow == "012345678901234567890123456789END" );

} // insertAtDocumentEnd


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, deleteMultipleFullRows ) {
	std::ifstream stream("test.txt");
	TextEditorCore t1{ stream };

	// select from row 2 through row 5, spanning two full rows(3,4) in between
	t1.setCursor(position(2, 0))
		.startSelection()
		.setCursor(position(5, 0))
		.finishSelection()
		.removeSelectedText();

	EXPECT_TRUE( t1.getCursorPosition() == position(2, 0) );

	const std::string remainingRow = t1.startSelection()
		.endKeyPressed()
		.finishSelection()
		.getSelectedText();

	EXPECT_TRUE( remainingRow == "012345678901234567890123456789" );

} // deleteMultipleFullRows


/*---------------------------------------------------------------------------*/


TEST( TextEditorCoreSuit, continueSelection ) {
	TextEditorCore t1{};

	t1.startSelection()
		.cursorRight()
		.finishSelection();

	EXPECT_TRUE( t1.getCursorPosition() == position(0, 1) );

	t1.continueSelection(); // cursor is at selection end and mode is Edit, so this is allowed
	t1.cursorRight();
	t1.finishSelection();

	const std::string selectedText = t1.getSelectedText();
	EXPECT_TRUE( selectedText == " \n" );

	t1.setCursor( position( 5, 0 ) ); // moves cursor away from the selection end

	try {
		t1.continueSelection();
		FAIL() << "expected BAD_CONTINUE_SELECTION to be thrown";
	}
	catch ( std::logic_error& err ) {
		EXPECT_TRUE( errorMessage::BAD_CONTINUE_SELECTION == err.what() );
	}

} // continueSelection


/*---------------------------------------------------------------------------*/


TEST( PositionSuit, comparisonOperators ) {
	const position p1{ 1, 5 };
	const position p2{ 1, 50 };
	const position p3{ 2, 0 };

	EXPECT_TRUE( p1 == position( 1, 5 ) );
	EXPECT_TRUE( p1 != p2 );

	// all relational operators only compare m_row; m_col is ignored
	EXPECT_TRUE( p1 <= p2 );
	EXPECT_TRUE( p1 < p3 );
	EXPECT_FALSE( p3 < p1 );

	// operator> and operator>= are defined as negations of operator< and operator<=,
	// so for equal rows operator> reports true while operator>= reports false
	EXPECT_TRUE( p1 > p2 );
	EXPECT_FALSE( p1 >= p2 );
	EXPECT_TRUE( p3 >= p1 );

	EXPECT_TRUE( p3 - p1 == 1 );
	EXPECT_TRUE( p1 - p3 == -1 );

} // comparisonOperators


/*---------------------------------------------------------------------------*/