#include "texteditorcore.hpp"

TextEditorCore::TextEditorCore() 
	: m_cursor{std::make_unique<Cursor>()},
	m_finderReplacer{std::make_unique<Replacer>()},
	m_container{ constants::DEFAULT_DOCUMENT_SIZE }
{}

TextEditorCore::TextEditorCore(std::istream & stream) 
	: TextEditorCore(){
	// противоречит SOLIDу, но да ладно.
	//	P.S. Я бы метод чтения и записи в поток
	//вынес в отдельный класс и сделал его частью ТеxtEditorCore 
	m_container
		.assign((customIterator::LineInputIterator<>{ stream }), 
			customIterator::LineInputIterator<>{});
	
	/* Если загружен пустой файл - я это ошибкой не считаю,
	т.к открытие пустого документа,
	поддерживают все известные мне тектовые редакторы.*/
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
	insertText(const_cast<position&>(m_cursor->getCoursorPos()), std::string{ character });
	return *this;
}

TextEditorCore & TextEditorCore::insert( std::string & str){
	insertText(const_cast<position&>(m_cursor->getCoursorPos()), str);
	return *this;
}

TextEditorCore & TextEditorCore::removeSelectedText()
{
	deleteText(m_cursor->m_selectedText.from(),  m_cursor->m_selectedText.to());
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

TextEditorCore & TextEditorCore::write(std::ostream& stream) {
	std::copy(m_container.cbegin(), m_container.cend(),
		std::ostream_iterator<std::string>(stream, constants::END_OF_LINE.c_str()));
	return *this;
}

// private methods // MUST BE IMPROWED
void TextEditorCore::insertText(position& pos, const std::string& text) {
	if (pos > maxPosition())
		throw std::logic_error(errorMessage::INVALID_POSITION);
	if (text.empty()) // empty input string => return
		return;
	std::string end_of_current_string{};
	bool is_new_line_need{ text.back() == constants::END_OF_LINE_CHAR };
	// this method does logic of method insertText more simple.(as for me.) get end of saving string
	getEndPartOfChangeString(text, end_of_current_string, pos);
	std::string copy_text{ text };
	auto begin{ customIterator::LineInsertIterator<>(copy_text) },
		end{ customIterator::LineInsertIterator<>() };
	m_container.insert(m_container.begin() + pos.m_row, begin, end);
	unsigned distance = std::distance(customIterator::LineInsertIterator<>(std::string{ text }),
		customIterator::LineInsertIterator<>());
	unsigned row{ pos.m_row + --distance }, col{ m_container[row].length() };
	m_cursor->setCursor(position{ row,col },m_container);
	if (is_new_line_need)
		m_container.insert(m_container.begin() + pos.m_row + 1, end_of_current_string);
	else
		m_container[pos.m_row].append(end_of_current_string);
}
// ANALIZED
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
		m_container.erase(m_container.begin() + (from.m_row + 1), m_container.begin() + (to.m_row - 1));
}

void TextEditorCore::deleteRow(unsigned row) noexcept{
	m_container.erase(m_container.cbegin() + row);
}

void TextEditorCore::deleteRowTextFragment(const position& from) {
	unsigned count{ m_container.at(from.m_row).length() - from.m_col};

	if (from.m_col == 0) // col == zero - delete all line.
		deleteRow(from.m_row); 
	else 
		m_container[from.m_row].erase(from.m_col, count);
}

void TextEditorCore::deleteColTextFragment(const position & to){
	if (to.m_col == m_container.at(to.m_row).length()) // delete all line from zero to end
		deleteRow(to.m_row); // removing to end of line(from end position)
	else 
		m_container.at(to.m_row).erase(0, to.m_col);
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