#ifndef POSITION_HPP
#define POSITION_HPP

/*---------------------------------------------------------------------------*/

struct position {

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	position() 
		:	position(0, 0) 
	{
	}

	position( unsigned _row, unsigned _col ) 
		:	m_row{ validate( _row ) }
		,	m_col{ validate( _col ) } 
	{
	}

/*---------------------------------------------------------------------------*/

	position( const position& _pos ) = default;

	position& operator= ( const position& _pos ) = default;

	position( const position&& _pos )	
		:	m_row{ std::move( _pos.m_row ) }
		,	m_col{ std::move( _pos.m_col ) } 
	{
	}

/*---------------------------------------------------------------------------*/


	bool 
	operator== ( const position& _rhs ) const noexcept 
	{
		return ( this->m_row == _rhs.m_row ) && ( this->m_col == _rhs.m_col );
	}

/*---------------------------------------------------------------------------*/


	bool 
	operator!= ( const position& _rhs ) const noexcept 
	{
		return !( *this == _rhs );
	}

/*---------------------------------------------------------------------------*/


	bool
	operator<= ( const position& _rhs ) const  noexcept
	{
		return ( this->m_row <= _rhs.m_row );
	}

/*---------------------------------------------------------------------------*/


	bool
	operator>= ( const position& _rhs ) const  noexcept 
	{
		return  !( *this <= _rhs );
	}

/*---------------------------------------------------------------------------*/


	bool 
	operator< ( const position& _rhs ) const  noexcept 
	{
		return ( this->m_row < _rhs.m_row );
	}

/*---------------------------------------------------------------------------*/

	bool 
	operator>( const position& _rhs ) const  noexcept
	{
		return  !( *this < _rhs );
	}

/*---------------------------------------------------------------------------*/


	int 
	operator- ( const position& _rhs ) const noexcept
	{
		return static_cast< int >( this->m_row - _rhs.m_row );
	}

/*---------------------------------------------------------------------------*/


	friend std::ostream&
	operator << ( std::ostream& _lhs, const position& _rhs )
	{
		_lhs << _rhs.m_row << " - " << _rhs.m_col << std::endl;
		return _lhs;
	}

/*---------------------------------------------------------------------------*/

	unsigned m_row, m_col;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/


	unsigned 
	validate( unsigned _param ) const noexcept 
	{
		return _param >= 0 
			?	_param 
			:	0;
	}

/*---------------------------------------------------------------------------*/

}; // position

#endif