#ifndef REPLACER_HPP
#define REPLACER_HPP
#include"finder.hpp"

/*---------------------------------------------------------------------------*/

class Replacer
	: public Finder
{

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

	bool m_isSuccessfully;

	void replaceInContainer( 
									const position& _currentPos
								,	MyContainer& _container
								,	const std::string _newStr );

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	Replacer() = default;

	Replacer( const Replacer& _rhs ) = delete;

	Replacer& operator= ( const Replacer& ) = delete;

	Replacer( const Replacer&& ) = delete;

	Replacer& operator= ( const Replacer&& ) = delete;

	~Replacer() = default;

	bool replace(	
							const std::string & _oldStr
					,		const std::string & _newStr 
					,		const position& _maxPos
					,		 MyContainer& _container );

	bool replaceAll(
								const std::string& oldStr
						,		const std::string& newStr
						,		const position& maxPos
						,		MyContainer& container );

/*---------------------------------------------------------------------------*/

}; // class Replacer

#endif // !REPLACER_HPP