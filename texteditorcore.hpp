#ifndef TEXT_EDITOR_CORE_HPP
#define TEXT_EDITOR_CORE_HPP

#include<istream>
#include<memory>
#include<iterator>
#include<sstream>
#include"message.hpp"
#include"cursor.hpp"
#include"replacer.hpp"
#include"lineinputiterator.hpp"

class TextEditorCore {
public:
	//tested
	TextEditorCore();
	//tested
	explicit TextEditorCore(std::istream& stream);
	// copy operations
	TextEditorCore(const TextEditorCore&) = delete;
	TextEditorCore& operator=(const TextEditorCore&) = delete;
	// move operations
	TextEditorCore(const TextEditorCore&&) = delete;
	TextEditorCore& operator=(const TextEditorCore&&) = delete;
	~TextEditorCore() = default;

	// coursor methods
	const position& getCursorPosition() const noexcept
	{
		return m_cursor->getCursorPosition();
	}
	//  rvo //tested
	 position maxPosition() const noexcept 
	{
		return m_cursor->maxPosition(m_container);
	}
	 //tested
	TextEditorCore& cursorLeft();
	//tested
	TextEditorCore& cursorRight();
	//tested
	TextEditorCore& cursorDown();
	//tested
	TextEditorCore& cursorUp();
	//tested
	TextEditorCore& setCursor(unsigned row, unsigned col);
	//tested
	TextEditorCore& setCursor(const position& pos);

	// insertion 
 /*deprecated*/	TextEditorCore& insert(char character);

	TextEditorCore& insert( std::string& str);
	TextEditorCore& removeSelectedText();

	// some specified keys
	//tested
	TextEditorCore& HomeKeyPressed() noexcept 
	{
		m_cursor->currentLineBegin();
		return *this;
	}

	//tested
	TextEditorCore& EndKeyPressed() noexcept 
	{
		m_cursor->currentLineEnd(m_container);
		return *this;
	}

	//tested
	TextEditorCore& CtrlHomeKeyPressed() noexcept 
	{
		m_cursor->documentBegin();
		return *this;
	}

	//tested
	TextEditorCore& CtrlEndKeyPressed() noexcept 
	{
		m_cursor->documentEnd(m_container);
		return *this;
	}

	// selectedText
	//tested
	TextEditorCore& startSelection() noexcept;
	//tested
	TextEditorCore& finishSelection() noexcept;
	//tested
	TextEditorCore& continueSelection();
	//tested
	TextEditorCore& resetSelection() noexcept;
	//tested
	std::string getSelectedText() noexcept;

	// find Methods
	//tested
	const position& find(const std::string& str);
	//tested
	const position& findNext();

	//  replace methods
	//tested
	bool replace(const std::string& old_str, const std::string& new_str);
	//tested
	bool replaceAll(const std::string& old_str, const std::string& new_str);
	// write to stream
	//tested
	TextEditorCore& write(std::ostream& stream);
private:
	std::unique_ptr<Cursor> m_cursor;
	std::unique_ptr<Replacer> m_finderReplacer;
	Container m_container;

	//private methods
	void insertText( position& pos, const std::string& text) noexcept;
	void deleteText(const position& from, position& to);
	void deleteRow(unsigned row) noexcept;
	void deleteRowTextFragment(const position& from) noexcept;
	void deleteColTextFragment( const position& to) noexcept;
	void getEndPartOfChangeString(const std::string& text, std::string& end_of_current_string, const position& pos);
	// this method or add last row on new line or append last row; 
	void addLastRow(bool is_new_line_need,const position& pos, const std::string& last_row) noexcept;
};
#endif // !TEXT_EDITOR_CORE_HPP