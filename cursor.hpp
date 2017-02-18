#ifndef CURSOR_HPP
#define CURSOR_HPP
#include<utility>
#include<string>
#include<algorithm>
#include<functional>
#include"position.hpp"
#include"message.hpp"

class Cursor{
	friend class TextEditorCore;
	position m_cursor;
	enum class mode : bool{
		Edit = true,
		Select = false
	};
	// fields
	mode m_currentMode;
	struct selectedText{
	private:
		std::pair<position /*from*/, position /*to*/> m_selection;
	public:
		selectedText() : m_selection{} {};
		position& from() noexcept {
			return m_selection.first;
		}
		position& to() noexcept {
			return m_selection.second;
		}
	} m_selectedText;
	
	//tested
	// methods
	// if mode == Edit return coursor, if Select return first position of selection;
	position& getPositionObject() noexcept{
		return (m_currentMode == mode::Edit) ? m_cursor : m_selectedText.to();
	}
	void cancelReverseSelection();
	void addLastRowFromMultilineSelection(std::string& selectedText, const Container& container) noexcept;
	void addSingleRow(std::string& selectedText, const Container& container) noexcept;
	void addFirstRowFromMiltilineSelection(std::string& selectedText, const Container& container) noexcept;
	//WARNING NOT TESTED
	void multilineRowSelection(std::string& selectedText, const Container& container) noexcept;
public:
	Cursor();
	Cursor(unsigned row, unsigned col);
	Cursor(const Cursor& csr) = delete;
	Cursor(const Cursor&& csr) = delete;
	Cursor& operator=(const Cursor& csr) = delete;
	Cursor& operator=(const Cursor&& csr) = delete;
	~Cursor() = default;
	// tested
	const position& getCursorPosition() const noexcept{
		return m_cursor;
	}
	//RVO //tested
	 position maxPosition(const Container& container) const noexcept{
		return position(container.size() - 1, container.back().length());
	}
	 unsigned currentRowMaxCol(unsigned row,const Container& container) const noexcept {
		 return container.at(row).length();
	}
	//tested
	void cursorLeft(const Container& container);
	//tested
	void cursorRight(const Container& container);
	//tested
	void cursorDown(const Container& container);
	//tested
	void cursorUp(const Container& container);
	// tested  think to int int.
	void setCursor(unsigned row, unsigned col, const Container& container);
	//tested
	void setCursor(const position& pos, const Container& container);

	void currentLineBegin() noexcept 
	{
		getPositionObject().m_col = constants::LINE_BEGIN;
	};

	void currentLineEnd(const Container & container) noexcept
	{
		getPositionObject().m_col = container[m_cursor.m_row].length();
	}

	void documentBegin() noexcept 
	{
		getPositionObject().m_row = getPositionObject().m_col = constants::LINE_BEGIN;
	}
	void documentEnd(const Container & container) noexcept
	{
		getPositionObject() = maxPosition(container);
	}

	// selection
	//tested
	void startSelection() noexcept;
	//tested
	void finishSelection() noexcept;
	//tested
	void continueSelection();
	//tested
	void resetSelection() noexcept;
	//tested
	std::string getSelectedText(const Container& container) noexcept;
};
#endif // !CURSOR_HPP