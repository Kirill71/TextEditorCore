#ifndef TEXT_EDITOR_CORE_HPP
#define TEXT_EDITOR_CORE_HPP

#include<istream>
#include<memory>
#include<iterator>
#include<sstream>
#include"message.hpp"
#include"cursor.hpp"
#include"replacer.hpp"
#include"customIterator.hpp"

class TextEditorCore{
public:
	TextEditorCore() noexcept;
	explicit TextEditorCore(std::istream& stream) noexcept;
	// copy operations
	TextEditorCore(const TextEditorCore&) = delete;
	TextEditorCore& operator=(const TextEditorCore&) = delete;
	// move operations
	TextEditorCore(const TextEditorCore&&) = delete;
	TextEditorCore& operator=(const TextEditorCore&&) = delete;
	~TextEditorCore() = default;

	// coursor methods
	const position& getCursorPosition() const noexcept{
		return m_cursor->getCursorPosition();
	}
	//  rvo 
	 position maxPosition() const noexcept {
		return m_cursor->maxPosition(m_container);
	}
	TextEditorCore& cursorLeft();
	TextEditorCore& cursorRight();
	TextEditorCore& cursorDown();
	TextEditorCore& cursorUp();
	TextEditorCore& setCursor(unsigned row, unsigned col);
	TextEditorCore& setCursor(const position& pos);

	// insertion 
	TextEditorCore& insert(char character);

	TextEditorCore& insert(const std::string& str);
	TextEditorCore& removeSelectedText();

	// some specified keys
	TextEditorCore& HomeKeyPressed() noexcept {
		m_cursor->getPositionObject().m_col = constants::LINE_BEGIN;
		return *this;
	}

	TextEditorCore& EndKeyPressed() noexcept {
		m_cursor->getPositionObject().m_col = m_container[m_cursor->getPositionObject().m_row].length();
		return *this;
	}

	TextEditorCore& CtrlHomeKeyPressed() noexcept {
		m_cursor->getPositionObject().m_row = m_cursor->getPositionObject().m_col = constants::LINE_BEGIN;
		return *this;
	}

	TextEditorCore& CtrlEndKeyPressed() noexcept {
		m_cursor->getPositionObject() = maxPosition();
		return *this;
	}

	// selectedText
	TextEditorCore& startSelection() noexcept;
	TextEditorCore& finishSelection() noexcept;
	TextEditorCore& continueSelection();
	TextEditorCore& resetSelection() noexcept;
	std::string getSelectedText() noexcept;

	// find Methods
	const position& find(const std::string& str);
	const position& findNext();

	//  replace methods
	bool replace(const std::string& old_str, const std::string& new_str);
	bool replaceAll(const std::string& old_str, const std::string& new_str);
private:
	friend  std::ostream& operator<< (std::ostream& lhs, TextEditorCore& rhs) noexcept;
	friend  std::istream& operator>> (std::istream& lhs, TextEditorCore& rhs) noexcept;
	std::unique_ptr<Cursor> m_cursor;
	std::unique_ptr<Replacer> m_finderReplacer;
	MyContainer m_container;

	//private methods
	void insertText( position& pos, std::string& text) noexcept;
	void deleteText(const position& from, position& to);
	void deleteRow(unsigned row) noexcept;
	void deleteRowTextFragment(const position& from) noexcept;
	void deleteColTextFragment( const position& to) noexcept;
	void getEndPartOfChangeString(const std::string& text, std::string& end_of_current_string, const position& pos);
	// this method or add last row on new line or append last row; 
	void addLastRow(bool is_new_line_need,const position& pos, const std::string& last_row) noexcept;
	void newLineInsert(const std::string& text,  std::string& end_of_string, position& pos ) noexcept;
};
#endif // !TEXT_EDITOR_CORE_HPP