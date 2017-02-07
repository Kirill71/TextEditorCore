#ifndef TEXT_EDITOR_CORE_HPP
#define TEXT_EDITOR_CORE_HPP
#include<istream>
#include<vector>
#include<string>
#include<utility>

class TextEditorCore {
	// I think about insertion in middle of sequense and I think about list<T>
	using Container = std::vector<std::string>;
public:
	// coursor description
	struct position
	{
		unsigned m_row, m_col;

		position() : m_row{ 0 }, m_col{ 0 } {}

		position(unsigned row_, unsigned col_)
			: m_row{ validate(row_) },
			m_col{ validate(col_) } {}

		unsigned validate(unsigned param) const
		{
			return param > 0 ? param : 0;
		}

		bool operator==(const position& rhs) const
		{
			return (this->m_row == rhs.m_row) && (this->m_col == rhs.m_col);
		}

		bool operator!=(const position& rhs) const
		{
			return !(*this == rhs);
		}
	};



	TextEditorCore();
	explicit TextEditorCore(std::istream& stream);
	// copy operations
	TextEditorCore(const TextEditorCore&) = delete;
	TextEditorCore& operator=(const TextEditorCore&) = delete;
	// move operations
	TextEditorCore(const TextEditorCore&&) = delete;
	TextEditorCore& operator=(const TextEditorCore&&) = delete;
	~TextEditorCore() = default;

	// coursor method
	const position& getCoursorPos() const noexcept
	{
		return coursor;
	}
	TextEditorCore& coursorLeft();
	TextEditorCore& coursorRight();
	TextEditorCore& coursorDown();
	TextEditorCore& coursorUp();
	TextEditorCore& setCoursor(unsigned row, unsigned col);
	TextEditorCore& setCoursor(const position& pos);

	const position& maxPosition() const 
	{
		return position(container.size() - 1, container.back().length());
	}
	
private:
	enum class mode : bool
	{
		Edit = true,
		Select = false
	};

	// fields
	mode currentMode;
	position coursor;
	// first == from, second == to
	std::pair<position, position> selection;
	//first == pos, second == searchString
	std::pair<position , std::string> find;
	Container container;
	// methods

	// if mode == Edit return coursor, if Select return first position of selection;
	position const & getPositionObject() const noexcept
	{
		return (currentMode == mode::Edit) ? coursor : selection.first;
	}
};

#endif // !TEXT_EDITOR_CORE_HPP

