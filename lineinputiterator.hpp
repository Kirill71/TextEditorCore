#ifndef LINEINPUT_ITERATOR_HPP
#define LINEINPUT_ITERATOR_HPP

#include <iterator>
#include <istream>
#include <string>

namespace customIterator {

	template <class StringT = std::string>
	class LineInputIterator :
		public std::iterator<std::input_iterator_tag, StringT, std::ptrdiff_t, const StringT*, const StringT&>
	{
	public:
		typedef typename StringT::value_type char_type;
		typedef typename StringT::traits_type traits_type;
		typedef std::basic_istream<char_type, traits_type> istream_type;

		LineInputIterator() : is{ nullptr } {}
		LineInputIterator(istream_type& is) : is{ &is } {}
		const StringT& operator*() const { return value; }
		const StringT* operator->() const { return &value; }
		LineInputIterator<StringT>& operator++()
		{
			if (is && !getline(*is, value)) {
				is = nullptr;
			}
			return *this;
		}
		LineInputIterator<StringT> operator++(int)
		{
			LineInputIterator<StringT> prev(*this);
			++*this;
			return prev;
		}
		bool operator!=(const LineInputIterator<StringT>& other) const noexcept
		{
			return is != other.is;
		}
		bool operator==(const LineInputIterator<StringT>& other) const noexcept
		{
			return !(*this != other);
		}
	private:
		istream_type* is;
		StringT value;
	};
} // end ub
#endif