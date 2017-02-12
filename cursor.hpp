#ifndef CURSOR_HPP
#define CURSOR_HPP
#include<utility>
#include<string>
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
	// first == from, second == to
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
	void addLastRowToSelectText(std::string& selectedText, const Container& container) noexcept;
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
	// tested
	const position& getCoursorPos() const noexcept{
		return m_cursor;
	}
	//RVO //tested
	 position maxPosition(const Container& container) const noexcept{
		return position(container.size() - 1, container.back().length());
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
	// selection
	void startSelection() noexcept;
	void finishSelection() noexcept;
	void continueSelection();
	void resetSelection() noexcept;
	std::string getSelectedText(const Container& container) noexcept;
};
#endif // !CURSOR_HPP