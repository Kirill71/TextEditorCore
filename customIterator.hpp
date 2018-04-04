#ifndef LINE_INPUT_ITERATOR_HPP
#define LINE_INPUT_ITERATOR_HPP

#include<iterator>
#include<istream>
#include<string>
#include<cstring>
#include"message.hpp"

/*---------------------------------------------------------------------------*/

namespace customIterator{

	template < class StringT = std::string >
	class LineInputIterator :
		public std::iterator< std::input_iterator_tag ,
		StringT , std::ptrdiff_t , const StringT* , const StringT& >
	{

/*---------------------------------------------------------------------------*/

	public:

/*---------------------------------------------------------------------------*/
		typedef typename StringT::value_type char_type;
		typedef typename StringT::traits_type traits_type;
		typedef std::basic_istream< char_type, traits_type > istream_type;

/*---------------------------------------------------------------------------*/


		LineInputIterator() : is { nullptr }, value {} {}

/*---------------------------------------------------------------------------*/


		explicit LineInputIterator( istream_type& _is ) 
			:	is { &_is }
			,	value {} { std::getline( *is , value , '\n' ); }

/*---------------------------------------------------------------------------*/


		const StringT& operator* () const { return value; }

/*---------------------------------------------------------------------------*/


		const StringT* operator->() const { return &value; }

/*---------------------------------------------------------------------------*/


		LineInputIterator<StringT>& operator++ (){
			if ( is && !std::getline( *is, value ) ) 
				is = nullptr;
			return *this;
		}

/*---------------------------------------------------------------------------*/


		LineInputIterator<StringT> operator++ ( int )
		{
			LineInputIterator<StringT> prev( *this );
			++*this;
			return prev;
		}

/*---------------------------------------------------------------------------*/


		bool operator!= ( const LineInputIterator< StringT >& other ) const noexcept
		{
			return is != other.is;
		}

/*---------------------------------------------------------------------------*/


		bool operator== ( const LineInputIterator< StringT >& other ) const noexcept{
			return !( *this != other );
		}

/*---------------------------------------------------------------------------*/

	private:

/*---------------------------------------------------------------------------*/

		istream_type* is;

		StringT value;

/*---------------------------------------------------------------------------*/

	}; // LineInputIterator


/*---------------------------------------------------------------------------*/


	template < class StringT = std::string >
	class LineInsertIterator :
		public std::iterator< std::input_iterator_tag ,
		StringT , std::ptrdiff_t , const StringT* , const StringT& >
	{

/*---------------------------------------------------------------------------*/

	public:

/*---------------------------------------------------------------------------*/


		LineInsertIterator() 
			:	line { nullptr }
			,	nextToken { nullptr }
			,	value {} {};

/*---------------------------------------------------------------------------*/


		explicit LineInsertIterator( std::string& _text )
			: LineInsertIterator() 
					{ 
						line = strtok_s( 
								const_cast< char* >( _text.c_str() ) 
							,	constants::END_OF_LINE.c_str()
							,	&nextToken );
						value = line; 
					}

/*---------------------------------------------------------------------------*/


		LineInsertIterator< StringT >& operator++ ()
		{
			if ( line = strtok_s(
						nullptr
					,	constants::END_OF_LINE.c_str()
					,	&nextToken ) 
			    ) 
				value = line;

			return *this;
		}

/*---------------------------------------------------------------------------*/


		LineInsertIterator<StringT> operator++ ( int )
		{
			LineInsertIterator< StringT > prev( *this );
			++*this;

			return prev;
		}

/*---------------------------------------------------------------------------*/


		bool operator!= ( const LineInsertIterator< StringT >& _other ) const noexcept
		{
			return line != _other.line;
		}

/*---------------------------------------------------------------------------*/


		bool operator== ( const LineInsertIterator< StringT >& _other ) const noexcept
		{
			return ! ( *this != _other );
		}

/*---------------------------------------------------------------------------*/


		const StringT& operator* () const { return value; }

/*---------------------------------------------------------------------------*/


		const StringT* operator->() const { return &value; }

/*---------------------------------------------------------------------------*/

	private:

/*---------------------------------------------------------------------------*/

		char* line;

		char* nextToken;

		StringT value;

/*---------------------------------------------------------------------------*/

	}; // class LineInsertIterator
} 

/*---------------------------------------------------------------------------*/

using InputIterator = customIterator::LineInputIterator<>;
using InsertIterator = customIterator::LineInsertIterator<>;

/*---------------------------------------------------------------------------*/

#endif