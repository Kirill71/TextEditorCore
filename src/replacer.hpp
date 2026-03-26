#pragma once
#include "finder.hpp"

/*---------------------------------------------------------------------------*/

class Replacer
	: public Finder
{

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

	bool m_isSuccessfully;

	void replaceInContainer( const position& _currentPos, const std::string& _newStr );

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	explicit Replacer( const TextEditorCoreBase& _container );

	Replacer( const Replacer& _rhs ) = delete;

	Replacer& operator= ( const Replacer& ) = delete;

	Replacer( Replacer&& ) = delete;

	Replacer& operator= ( Replacer&& ) = delete;

	~Replacer() override = default;

	bool replace( const std::string & _oldStr, const std::string & _newStr );

	bool replaceAll( const std::string& _oldStr, const std::string& _newStr );

/*---------------------------------------------------------------------------*/

}; // class Replacer
