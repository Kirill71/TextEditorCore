#pragma once

#include<string>
#include<algorithm>
#include "position.hpp"
#include "message.hpp"


/*---------------------------------------------------------------------------*/

class Finder
{

/*---------------------------------------------------------------------------*/

protected:

/*---------------------------------------------------------------------------*/

	struct findText
	{

/*---------------------------------------------------------------------------*/

	public:

/*---------------------------------------------------------------------------*/

		findText() = default;

/*---------------------------------------------------------------------------*/


		position& lastPosition() noexcept
		{
			return m_find.first;
		}

/*---------------------------------------------------------------------------*/


		std::string& searchString()  noexcept
		{
			return m_find.second;
		}

/*---------------------------------------------------------------------------*/


		void 
		setParams( const position& _pos, const std::string& _findText ) noexcept 
		{
			m_find.first = _pos;
			m_find.second = _findText;
		}

/*---------------------------------------------------------------------------*/

	private:

/*---------------------------------------------------------------------------*/

		std::pair< position, std::string > m_find;

/*---------------------------------------------------------------------------*/

	} //  struct findText
	m_findText;


	const TextEditorCoreBase& m_container;

	const position& find_base(		const std::string & _str
								,	const position & _pos = { 0,0 } );

	position maxPosition() const noexcept
	{
		return { m_container.size() - 1, m_container.back().length() };
	}

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	explicit Finder( const TextEditorCoreBase& _container );

	Finder( const Finder& _rhs ) = delete;

	Finder& operator= ( const Finder&  _rhs ) = delete;

	Finder( Finder&& _rhs ) = delete;

	Finder& operator= ( Finder&& ) = delete;

	virtual ~Finder() = default;

	const position& find( const std::string& _str );

	const position& findNext( bool _isReplace = false );

/*---------------------------------------------------------------------------*/

}; // class Finder