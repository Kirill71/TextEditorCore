#include "texteditorcore.hpp"
#include<iterator>
#include<sstream>
#include<list>
TextEditorCore::TextEditorCore() 
	: m_container{ DEFAULT_DOCUMENT_SIZE },
	m_cursor{std::make_unique<Cursor>()}
{}

TextEditorCore::TextEditorCore(std::istream & stream) 
	: TextEditorCore(){
	// ������������ SOLID�, �� �� �����.
	//	P.S. � �� ����� ������ � ������ � �����
	//����� � ��������� ����� � ������ ��� ������ ��xtEditorCore 
	m_container
		.assign(( customIterator::LineInputIterator<>{ stream }), 
			customIterator::LineInputIterator<>{});
	
	/* ���� �������� ������ ���� - � ��� ������� �� ������,
	�.� �������� ������� ���������,
	������������ ��� ��������� ��� �������� ���������.*/
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

TextEditorCore & TextEditorCore::write(std::ostream& stream){  
	std::copy(m_container.cbegin(), m_container.cend(),
		std::ostream_iterator<std::string>(stream, END_OF_LINE));
	return *this;
}

// insertion methods
TextEditorCore & TextEditorCore::insert(char character){
	insertText(m_cursor->getCoursorPos(), std::string{ character });
	return *this;
}

TextEditorCore & TextEditorCore::insert(const char * c_str){
	insertText(m_cursor->getCoursorPos(), std::string{ c_str });
	return *this;
}

TextEditorCore & TextEditorCore::insert( std::string & str){
	insertText(m_cursor->getCoursorPos(), str);
	return *this;
}

TextEditorCore & TextEditorCore::removeSelectedText()
{
	deleteText(/*from*/m_cursor->m_selection.first, /*to*/ m_cursor->m_selection.second);
	m_cursor->m_selection.second = m_cursor->m_selection.first; // to = from after deleting;
	return *this;
}

TextEditorCore & TextEditorCore::HomeKeyPressed()
{
	setCursor(position(m_cursor->getCoursorPos().m_row, 0));
	return *this;
}

TextEditorCore & TextEditorCore::EndKeyPressed()
{
	unsigned row{ m_cursor->getCoursorPos().m_row };
	setCursor(position(row,m_container.at(row).length()));
	return *this;
}

TextEditorCore & TextEditorCore::CtrlHomeKeyPressed()
{
	setCursor(position{});
	return *this;
}

TextEditorCore & TextEditorCore::CtrlEndKeyPressed()
{
	unsigned lenght{ m_container.size() - 1 };
	setCursor(position(lenght, m_container.back().length()));
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

TextEditorCore & TextEditorCore::resetSelection() noexcept
{
	m_cursor->resetSelection();
	return *this;
}

std::string TextEditorCore::getSelectedText() noexcept
{
	return m_cursor->getSelectedText(m_container);
}




// private methods
void TextEditorCore::insertText(const position& pos,  std::string& text) {
	if (pos > maxPosition())
		throw std::logic_error(errorMessage::INVALID_POSITION);
	if (text.empty()) // empty input string => return
		return;
	std::string end_of_current_string{};
	// this method does logic of method insertText more simple.(as for me.) get end of saving string
	getEndPartOfChangeString(text, end_of_current_string, pos);
	// add first line of new text in container and delete this line in variable text;
	addFirstLineOfNewText(text, pos);

	bool is_new_line_need{ text.back() == END_OF_LINE_CHAR };
	auto begin{ customIterator::LineInsertIterator<>(std::stringstream{ text },is_new_line_need) },
		end{ customIterator::LineInsertIterator<>() };

	m_container.insert(m_container.cbegin() + pos.m_row, begin, end);
	// row position after insertion input text

	unsigned number_of_added_rows{
		std::distance(customIterator::LineInsertIterator<>(std::stringstream{ text},is_new_line_need),
		customIterator::LineInsertIterator<>()) }, current_row{ pos.m_row + number_of_added_rows };
	// end string append to

	m_container.at(current_row).append(end_of_current_string);
	setCursor(position(current_row,m_container.at(current_row).length()));
}

void TextEditorCore::deleteText(const position & from, const position & to) {
	if (from > maxPosition() || to > maxPosition())
		throw std::logic_error(errorMessage::INVALID_POSITION);

	if (from == to) // nothing delete
		return;

	if (from.m_row == to.m_row) {
		m_container.at(from.m_row).erase(from.m_col, to.m_col - from.m_col);
	}
	else {
		deleteRowTextFragment(from);
		deleteColTextFragment(to);
	}
	if ((to - from) > 1)   
		m_container.erase(m_container.begin() + from.m_row + 1, m_container.begin() + to.m_row - 1);
}

void TextEditorCore::deleteRow(unsigned row) noexcept{
	m_container.erase(m_container.cbegin() + row);
}

void TextEditorCore::deleteRowTextFragment(const position& from) {
	unsigned from_row_len{ m_container.at(from.m_row).length() };

	if (from.m_col == 0) // col == zero - delete all line.
		deleteRow(from.m_row); 
	else 
		m_container.at(from.m_row).erase(from.m_col, from_row_len - from.m_col);
}

void TextEditorCore::deleteColTextFragment(const position & to){
	if (to.m_col == m_container.at(to.m_row).length()) // delete all line from zero to end
		deleteRow(to.m_row); // removing to end of line(from end position)
	else 
		m_container.at(to.m_row).erase(0, to.m_col);
}

void TextEditorCore::getEndPartOfChangeString(std::string & text, std::string & end_of_current_string, const position& pos){
	//  remember end of string
	  end_of_current_string.assign( m_container.at(pos.m_row).substr(pos.m_col, m_container.at(pos.m_row).length() - pos.m_col));
	// delete end of string from container
	m_container.at(pos.m_row).erase(m_container.at(pos.m_row).begin() + pos.m_col, m_container.at(pos.m_row).end());
}

void TextEditorCore::addFirstLineOfNewText(std::string & text, const position & pos){
	//get  first line from input text
	std::string firstLine = text.substr(FIRST_INDEX, text.find_first_of(END_OF_LINE, FIRST_INDEX) - FIRST_INDEX);
	// add this line in container on end(variable) position
	m_container.at(pos.m_row).append(firstLine);
	// delete first line from  input text
	text.erase(FIRST_INDEX, firstLine.length() + 1);
}