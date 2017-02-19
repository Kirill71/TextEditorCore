#include "texteditorcore.hpp"

TextEditorCore::TextEditorCore() noexcept
	: m_cursor{std::make_unique<Cursor>()},
	m_finderReplacer{std::make_unique<Replacer>()},
	m_container{ constants::DEFAULT_DOCUMENT_SIZE, ""}
{}

TextEditorCore::TextEditorCore(std::istream & stream)  noexcept
	: TextEditorCore() {
	stream >> *this;
}
// cursor methods
TextEditorCore& TextEditorCore::cursorLeft(){
	m_cursor->cursorLeft(m_container);
	return *this;
}

TextEditorCore& TextEditorCore::cursorRight(){
	m_cursor->cursorRight(m_container);
	return *this;
}

TextEditorCore& TextEditorCore::cursorDown(){
	m_cursor->cursorDown(m_container);
	return *this;
}

TextEditorCore& TextEditorCore::cursorUp(){
	m_cursor->cursorUp(m_container);
	return *this;
}

TextEditorCore& TextEditorCore::setCursor(unsigned row, unsigned col){
	m_cursor->setCursor(row, col, m_container);
	return *this;
}

TextEditorCore& TextEditorCore::setCursor(const position & pos){
	m_cursor->setCursor(pos, m_container);
	return *this;
}

// insertion methods
TextEditorCore & TextEditorCore::insert(char character){
	insertText(const_cast<position&>(m_cursor->getCursorPosition()), std::string{ character });
	return *this;
}

TextEditorCore & TextEditorCore::insert( const std::string & str){
	insertText(const_cast<position&>(m_cursor->getCursorPosition()), const_cast<std::string&>(str));
	return *this;
}

TextEditorCore & TextEditorCore::removeSelectedText()
{
	deleteText(m_cursor->m_selectedText.from(), m_cursor->m_selectedText.to());
	m_cursor->m_selectedText.to() = m_cursor->m_selectedText.from(); 
	return *this;
}

TextEditorCore & TextEditorCore::startSelection() noexcept
{
	m_cursor->startSelection();
	return *this;
}

TextEditorCore & TextEditorCore::finishSelection() noexcept
{
	m_cursor->finishSelection();
	return *this;
}

TextEditorCore & TextEditorCore::continueSelection()
{
	m_cursor->continueSelection();
	return *this;
}

TextEditorCore & TextEditorCore::resetSelection() noexcept
{
	m_cursor->resetSelection();
	return *this;
}

std::string TextEditorCore::getSelectedText() noexcept
{
	return m_cursor->getSelectedText(m_container);
}

const position & TextEditorCore::find(const std::string & str)
{
	return m_finderReplacer->find(str, maxPosition(), m_container);
}

const position & TextEditorCore::findNext()
{
	return m_finderReplacer->findNext(maxPosition(), m_container);
}

bool TextEditorCore::replace(const std::string & old_str, const std::string & new_str)
{
	return  m_finderReplacer->replace(old_str, new_str, maxPosition(), m_container);
}

bool TextEditorCore::replaceAll(const std::string & old_str, const std::string & new_str)
{
	return m_finderReplacer->replaceAll(old_str, new_str, maxPosition(), m_container);
}

// private methods 
void TextEditorCore::insertText(position& pos,  std::string& text)  noexcept {
	if (text.empty()) // empty input string => return
		return;

	std::string end_of_current_string{}, copy_text{ text };
	if (text == constants::END_OF_LINE) {
		newLineInsert(text, end_of_current_string, pos);
		return;
	}
	bool is_new_line_need{ text.back() == constants::END_OF_LINE_CHAR }, central_insertion{}; // this variable keeps two value 0 or 1																		  // get end part of current change string
	getEndPartOfChangeString(text, end_of_current_string, pos);
	auto begin{ InsertIterator{ copy_text } };
	// if insertion in string from index not zero

	if (pos.m_col > 0) {
		m_container[pos.m_row].insert(pos.m_col, *begin++);//insert first row
		central_insertion = true;
	}

	m_container.insert(m_container.begin() + pos.m_row +/*implicit cast for uint*/ central_insertion, begin, InsertIterator());
	//count insertion string
	unsigned count = std::distance(InsertIterator{ std::string{ text } }, InsertIterator());
	//  calculate new current row( and remeber about c-style(numeration from zero))(--count)
	unsigned row{ pos.m_row + --count }, col{ m_container[row].length() };
	m_cursor->setCursor(position{ row,col }, m_container);
	// this method or add new last row on new line or append  string to end last row; 
	addLastRow(is_new_line_need, pos, end_of_current_string);

	if (is_new_line_need)
		m_cursor->setCursor(position{ ++row, constants::LINE_BEGIN }, m_container);
}

void TextEditorCore::deleteText(const position & from, position & to) {
	if (from >= maxPosition() || to >= maxPosition())
		throw std::logic_error(errorMessage::INVALID_POSITION);
	if (m_cursor->m_currentMode == Cursor::mode::Select)
		finishSelection();
	if (from == to) // nothing delete
		return;
	m_cursor->setCursor(from, m_container);
	if (from.m_row == to.m_row) 
	{
		m_container.at(from.m_row).erase(from.m_col, to.m_col - from.m_col);
		if (m_container[from.m_row].empty())
			deleteRow(from.m_row);
	}
	else
	{
		deleteColTextFragment(to);
		if ((to.m_row - from.m_row) > 1)
			m_container.erase(m_container.begin() + from.m_row + 1, m_container.begin() + to.m_row);
		deleteRowTextFragment(from);		
	}
}

void TextEditorCore::deleteRow(unsigned row) noexcept{
	m_container.erase(m_container.cbegin() + row);
}

void TextEditorCore::deleteRowTextFragment(const position& from) noexcept{
	unsigned count{ m_container.at(from.m_row).length() - from.m_col};
	if (from.m_col == 0) {
		deleteRow(from.m_row);
	}
	else 
		m_container[from.m_row].erase(from.m_col, count);
}

void TextEditorCore::deleteColTextFragment(const position & to) noexcept{
	if (to.m_col == m_container.at(to.m_row).length()) 
		deleteRow(to.m_row);
	else 
		m_container.at(to.m_row).erase(constants::LINE_BEGIN, to.m_col);
}

void TextEditorCore::getEndPartOfChangeString( const std::string & text, std::string & end_of_current_string, const position& pos){
	unsigned count{ m_container.at(pos.m_row).length() - pos.m_col };
	//  remember end of string
	  end_of_current_string
		  .assign( m_container.at(pos.m_row)
			  .substr(pos.m_col, count)
		  );
	// delete end of string from container
	m_container.at(pos.m_row).erase(m_container.at(pos.m_row).begin() + pos.m_col, m_container.at(pos.m_row).end());
	if (pos.m_col == constants::LINE_BEGIN) // if get all string. 
		m_container.erase(m_container.begin() + pos.m_row);
}

void TextEditorCore::addLastRow(bool is_new_line_need, const position & pos, const std::string & part_of_string) noexcept
{
	if (is_new_line_need)
		m_container.insert(m_container.begin() + pos.m_row + 1, part_of_string);
	else
		m_container[pos.m_row].append(part_of_string);
}

void TextEditorCore::newLineInsert(const std::string & text, std::string & end_of_string, position & pos) noexcept
{
	if (pos.m_col == 0) 
		m_container.insert(m_container.begin() + pos.m_row, constants::SPACE);
	else
	{
		getEndPartOfChangeString(text, end_of_string, pos);
		m_container.insert(m_container.begin() + pos.m_row + 1, end_of_string);
	}
	m_cursor->setCursor(position{ pos.m_row + 1, constants::LINE_BEGIN }, m_container);
}

std::ostream & operator<<(std::ostream & lhs,  TextEditorCore & rhs) noexcept
{
	std::copy(rhs.m_container.cbegin(), rhs.m_container.cend(),
		std::ostream_iterator<std::string>(lhs, constants::END_OF_LINE.c_str()));
	return lhs;
}

std::istream & operator >> (std::istream & lhs, TextEditorCore & rhs) noexcept
{
	rhs.m_container
		.assign((InputIterator{ lhs }),
			InputIterator{});
	return lhs;
}
