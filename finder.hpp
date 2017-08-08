#ifndef FINDER_HPP
#define FINDER_HPP
#include<string>
#include<functional>
#include<algorithm>
#include"utils.hpp"
#include"message.hpp"

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


		position&
		lastPosition() noexcept
		{
			return m_find.first;
		}

/*---------------------------------------------------------------------------*/


		std::string& 
		searchString()  noexcept
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

	const position& find_base(		const std::string & _str
								,	const position & _maxPos
								,	const MyContainer& _container
								,	const position & _pos = { 0,0 } );

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	Finder() = default;

	Finder( const Finder& _rhs ) = delete;

	Finder& operator= ( const Finder& ) = delete;

	Finder( const Finder&& _rhs ) = delete;

	Finder& operator= ( const Finder&& ) = delete;

	virtual ~Finder() = default;

	const position& find(		const std::string& _str
							,	const position& _maxPos
							,	const MyContainer& _container );

	const position& findNext(		const position& _maxPos
								,	const MyContainer& _container
								,	bool _isReplace = false );

/*---------------------------------------------------------------------------*/

}; // class Finder

#endif