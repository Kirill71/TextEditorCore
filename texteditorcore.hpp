
#ifndef TEXT_EDITOR_CORE_HPP
#define TEXT_EDITOR_CORE_HPP
#include<istream>
#include<memory>
#include"cursor.hpp"


class TextEditorCore {
	// I think about insertion in middle of sequense and I think about list<T>
	
public:
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
		return cursor->getCoursorPos();
	}
	TextEditorCore& cursorLeft();
	TextEditorCore& cursorRight();
	TextEditorCore& cursorDown();
	TextEditorCore& cursorUp();
	TextEditorCore& setCursor(unsigned row, unsigned col);
	TextEditorCore& setCursor(const position& pos);

	
	
private:
	std::unique_ptr<Cursor> cursor;
	Container container;
};

#endif // !TEXT_EDITOR_CORE_HPP

