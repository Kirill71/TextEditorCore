
#ifndef TEXT_EDITOR_CORE_HPP
#define TEXT_EDITOR_CORE_HPP

#include<istream>
#include<memory>
#include"cursor.hpp"
#include"lineinputiterator.hpp"

class TextEditorCore {

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

	// coursor methods


	const position& getCoursorPos() const noexcept
	{
		return m_cursor->getCoursorPos();
	}
	TextEditorCore& cursorLeft();
	TextEditorCore& cursorRight();
	TextEditorCore& cursorDown();
	TextEditorCore& cursorUp();
	TextEditorCore& setCursor(unsigned row, unsigned col);
	TextEditorCore& setCursor(const position& pos);
	TextEditorCore& write(std::ostream& stream);

	// insertion 
	TextEditorCore& insert(char);
	TextEditorCore& insert(const char*);
private:
	std::unique_ptr<Cursor> m_cursor;
	Container m_container;

	//private methods
	void insertText(const position& pos, std::string& text) noexcept;
};

#endif // !TEXT_EDITOR_CORE_HPP

