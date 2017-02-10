#ifndef POSITION_HPP
#define POSITION_HPP
struct position {

	position() : position(0, 0) {}

	position(unsigned row_, unsigned col_) : m_row{ validate(row_) }, m_col{ validate(col_) } {}
	// copy operations
	position(const position& pos) = default;
	position& operator=(const position& pos) = default;
	position(const position&& pos) : m_row{ std::move(pos.m_row) }, m_col{ std::move(pos.m_col) } {}

	bool operator==(const position& rhs) const noexcept {
		return (this->m_row == rhs.m_row) && (this->m_col == rhs.m_col);
	}

	bool operator!=(const position& rhs) const noexcept {
		return !(*this == rhs);
	}

	bool operator>(const position& rhs) const  noexcept {
		return (this->m_row > rhs.m_row) && (this->m_col > rhs.m_col);
	}

	bool operator<(const position& rhs) const  noexcept {
		return  !(*this > rhs);
	}
	int operator-(const position& rhs) const noexcept {
		return static_cast<int>(this->m_row - rhs.m_row);
	}
	unsigned m_row, m_col;
private:
	unsigned validate(unsigned param) const noexcept {
		return param >= 0 ? param : 0;
	}
};
#endif
