/* 
date: 2013 - 2017, 
author : Sergei Zaychenko, NURE, Kharkiv, Ukraine, Aldec-KTC (c)

purpose : Simple Unit-testing lib.

refactored by Kirill Zharenkov, Aldec-KTC(c)

*/

#ifndef _TESTSLIB_HPP_
#define _TESTSLIB_HPP_

/*****************************************************************************/

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include<locale>
#include<functional>

/*****************************************************************************/

using TestProcedure =  std::function< void() >; 

/*****************************************************************************/

class TestsRunner
{

/*-----------------------------------------------------------------*/

public:

/*-----------------------------------------------------------------*/

	void addTest ( std::string const & _tpName, TestProcedure _tp )
	{
		m_testProcedures.push_back( std::make_pair( _tpName, _tp ) );
	}

/*-----------------------------------------------------------------*/

	void runTests ()
	{
		assert( !m_testProcedures.empty() );

		std::cout << "Running " << m_testProcedures.size() << " test(s):\n";

		int counter = 1;
		std::for_each(
			m_testProcedures.begin()
			, m_testProcedures.end()
			, [ & ] ( std::pair< std::string, TestProcedure > const & _test )
		{
			std::cout << "Test #" << counter << " \"" << _test.first << "\" ";
			(_test.second )( );
			std::cout << "\n> Success... " << '\n';
			++counter;
		}
		);

		std::cout << "Finished running tests.\n";
	}

/*-----------------------------------------------------------------*/

private:

/*-----------------------------------------------------------------*/

	using TestProcedureres = 
		std::vector< std::pair< std::string, TestProcedure > >;

/*-----------------------------------------------------------------*/

	 TestProcedureres m_testProcedures;

/*-----------------------------------------------------------------*/

};


/*****************************************************************************/


static TestsRunner testsRunner;


/*****************************************************************************/


class TestProcedureWrapper
{
public:
	TestProcedureWrapper ( std::string const & _tpName, TestProcedure _tp )
	{
		testsRunner.addTest( _tpName, _tp );
	}
};


/*****************************************************************************/


#define DECLARE_OOP_TEST( arg_testProcedureName )                                                                        \
    void arg_testProcedureName ();                                                                                        \
    static TestProcedureWrapper gs_wrapper_##arg_testProcedureName( #arg_testProcedureName, & arg_testProcedureName );    \
    void arg_testProcedureName ()


/*****************************************************************************/


#define ASSERT_THROWS( TESTED_CODE, EXPECTED_MESSAGE )		\
	try                                                     \
	{                                                       \
        { TESTED_CODE; }                                    \
        assert( ! "Exception must have been thrown" );      \
	}                                                       \
	catch ( const std::exception & e )                      \
	{                                                       \
		assert( ! strcmp( e.what(), EXPECTED_MESSAGE ) );   \
	}


/*****************************************************************************/

int main ()
{
	setlocale(LC_ALL, "Russian");
	testsRunner.runTests();
	system("pause");
}

/*****************************************************************************/
#endif //  _TESTSLIB_HPP_