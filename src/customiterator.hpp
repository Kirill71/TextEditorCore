#pragma once

#include<iterator>
#include<string>
#include<cstring>
#include "message.hpp"

/*---------------------------------------------------------------------------*/

namespace customIterator {

	template < class StringT = std::string >
	class LineInputIterator
	{

/*---------------------------------------------------------------------------*/

	public:

/*---------------------------------------------------------------------------*/

		using iterator_category = std::input_iterator_tag;
		using value_type        = StringT;
		using difference_type   = std::ptrdiff_t;
		using pointer           = const StringT*;
		using reference         = const StringT&;

		using char_type         = typename StringT::value_type;
		using traits_type       = typename StringT::traits_type;
		using istream_type      = std::basic_istream< char_type, traits_type >;

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


		LineInputIterator& operator++ (){
			if ( is && !std::getline( *is, value ) ) 
				is = nullptr;
			return *this;
		}

/*---------------------------------------------------------------------------*/


		LineInputIterator operator++ ( int )
		{
			LineInputIterator prev( *this );
			++*this;
			return prev;
		}

/*---------------------------------------------------------------------------*/


		bool operator!= ( const LineInputIterator& other ) const noexcept
		{
			return is != other.is;
		}

/*---------------------------------------------------------------------------*/


		bool operator== ( const LineInputIterator& other ) const noexcept{
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
	class LineInsertIterator
	{

/*---------------------------------------------------------------------------*/

	public:

/*---------------------------------------------------------------------------*/

		using iterator_category = std::input_iterator_tag;
		using value_type        = StringT;
		using difference_type   = std::ptrdiff_t;
		using pointer           = const StringT*;
		using reference         = const StringT&;

/*---------------------------------------------------------------------------*/


		LineInsertIterator() 
			:	line { nullptr }
			,	nextToken { nullptr }
			,	value {} {};

/*---------------------------------------------------------------------------*/


		explicit LineInsertIterator( std::string& _text )
			: LineInsertIterator() 
					{ 
						line = std::strtok(
								const_cast< char* >( _text.c_str() ) 
							,	constants::END_OF_LINE.c_str() );
						value = line; 
					}

/*---------------------------------------------------------------------------*/


		LineInsertIterator& operator++ ()
		{
			if ( (line = std::strtok(
				      nullptr
				      ,	constants::END_OF_LINE.c_str() ) ) )
				value = line;

			return *this;
		}

/*---------------------------------------------------------------------------*/


		LineInsertIterator operator++ ( int )
		{
			LineInsertIterator prev( *this );
			++*this;

			return prev;
		}

/*---------------------------------------------------------------------------*/


		bool operator!= ( const LineInsertIterator& _other ) const noexcept
		{
			return line != _other.line;
		}

/*---------------------------------------------------------------------------*/


		bool operator== ( const LineInsertIterator& _other ) const noexcept
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