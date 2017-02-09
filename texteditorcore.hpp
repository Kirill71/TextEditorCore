
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
	const position& maxPosition() const noexcept 
	{
		m_cursor->maxPosition(m_container);
	}
	TextEditorCore& cursorLeft();
	TextEditorCore& cursorRight();
	TextEditorCore& cursorDown();
	TextEditorCore& cursorUp();
	TextEditorCore& setCursor(unsigned row, unsigned col);
	TextEditorCore& setCursor(const position& pos);
	TextEditorCore& write(std::ostream& stream);

	// insertion 
	TextEditorCore& insert(char character);
	TextEditorCore& insert(const char* c_str);
	TextEditorCore& insert( std::string& str);
	TextEditorCore& removeSelectedText();
	// TODO: range insertion and removing

	// some specified keys
	TextEditorCore& HomeKeyPressed();
	TextEditorCore& EndKeyPressed();
	TextEditorCore& CtrlHomeKeyPressed();
	TextEditorCore& CtrlEndKeyPressed();

	// selectedText
	TextEditorCore& startSelection() noexcept;
	TextEditorCore& finishSelection() noexcept;
	TextEditorCore& resetSelection() noexcept;
	std::string getSelectedText() noexcept;

private:
	std::unique_ptr<Cursor> m_cursor;
	Container m_container;

	//private methods
	void insertText(const position& pos, std::string& text);
	void deleteText(const position& from, const position& to);
	void deleteRow(unsigned row) noexcept; 
	void deleteRowTextFragment(const position& from);
	void deleteColTextFragment(const position& to);
	void getEndPartOfChangeString(std::string& text, std::string& end_of_current_string, const position& pos);
	void addFirstLineOfNewText(std::string & text, const position& pos);
};

#endif // !TEXT_EDITOR_CORE_HPP