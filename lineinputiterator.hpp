#ifndef LINE_INPUT_ITERATOR_HPP
#define LINE_INPUT_ITERATOR_HPP

#include <iterator>
#include <istream>
#include <string>
#include"message.hpp"

namespace customIterator {

	template <class StringT = std::string>
	class LineInputIterator :
		public std::iterator<std::input_iterator_tag, StringT, std::ptrdiff_t, const StringT*, const StringT&>
	{
	public:
		typedef typename StringT::value_type char_type;
		typedef typename StringT::traits_type traits_type;
		typedef std::basic_istream<char_type, traits_type> istream_type;

		LineInputIterator() : is{ nullptr }, value{} {}
		explicit LineInputIterator(istream_type& is) : is{ &is }, value{} { std::getline(*is, value) }
		const StringT& operator*() const { return value; }
		const StringT* operator->() const { return &value; }
		virtual LineInputIterator<StringT>& operator++()
		{
			if (is && !std::getline(*is, value)) {
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
	protected:
		istream_type* is;
		StringT value;
	};

	template <class StringT = std::string>
	class LineInsertIterator :
		public LineInputIterator<StringT> 
	{
	public:
		LineInsertIterator() : LineInputIterator(), new_line{} {}
		explicit LineInsertIterator(istream_type& is, bool new_line_) : LineInputIterator(), is{ &is }, new_line{new_line_} { }
		LineInputIterator<StringT>& operator++() override
		{
			if (is->eof()) {
				is = nullptr;
			}
			if (is && !std::getline(*is, value) && new_line) {
				new_line = false;
				value = END_OF_LINE_CHAR;
			}
			return *this;
		}
	private:
		bool new_line;
	};
} // end ub
#endif