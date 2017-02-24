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
	void addLastRowFromMultilineSelection(std::string& selectedText, const MyContainer& container) noexcept;
	void addSingleRow(std::string& selectedText, const MyContainer& container) noexcept;
	void addFirstRowFromMiltilineSelection(std::string& selectedText, const MyContainer& container) noexcept;
	void multilineRowSelection(std::string& selectedText, const MyContainer& container) noexcept;
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
	 position maxPosition(const MyContainer& container) const noexcept{
		return position(container.size() - 1, container.back().length());
	}
	 unsigned currentRowMaxCol(unsigned row,const MyContainer& container) const noexcept {
		 return container.at(row).length();
	}
	void cursorLeft(const MyContainer& container);
	void cursorRight(const MyContainer& container);
	void cursorDown(const MyContainer& container);
	void cursorUp(const MyContainer& container);
	void setCursor(unsigned row, unsigned col, const MyContainer& container);
	void setCursor(const position& pos, const MyContainer& container);

	// selection
	void startSelection() noexcept;
	void finishSelection() noexcept;
	void continueSelection();
	void resetSelection() noexcept;
	std::string getSelectedText(const MyContainer& container) noexcept;
};
#endif // !CURSOR_HPP