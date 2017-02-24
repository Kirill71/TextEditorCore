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
	
	//cursor methods
	// if mode == Edit return coursor, if Select return first position of selection;
	position& getPositionObject() noexcept{
		return (m_currentMode == mode::Edit) ? m_cursor : m_selectedText.to();
	}
	void cancelReverseSelection();
	void addLastRowFromMultilineSelection(std::string& selectedText, const Container& container) noexcept;
	void addSingleRow(std::string& selectedText, const Container& container) noexcept;
	void addFirstRowFromMiltilineSelection(std::string& selectedText, const Container& container) noexcept;
	void multilineRowSelection(std::string& selectedText, const Container& container) noexcept;
public:
	Cursor();
	Cursor(unsigned row, unsigned col);
	Cursor(const Cursor& csr) = delete;
	Cursor(const Cursor&& csr) = delete;
	Cursor& operator=(const Cursor& csr) = delete;
	Cursor& operator=(const Cursor&& csr) = delete;
	~Cursor() = default;
	const position& getCursorPosition() const noexcept {
		return m_cursor;
	}
	//RVO or move semantic
	 position maxPosition(const Container& container) const noexcept{
		return position(container.size() - 1, container.back().length());
	}
	 unsigned currentRowMaxCol(unsigned row,const Container& container) const noexcept {
		 return container.at(row).length();
	}
	void cursorLeft(const Container& container);
	void cursorRight(const Container& container);
	void cursorDown(const Container& container);
	void cursorUp(const Container& container);
	void setCursor(unsigned row, unsigned col, const Container& container);
	void setCursor(const position& pos, const Container& container);

	// selection
	void startSelection() noexcept;
	void finishSelection() noexcept;
	void continueSelection();
	void resetSelection() noexcept;
	std::string getSelectedText(const Container& container) noexcept;
};
#endif // !CURSOR_HPP