#include "texteditorcore.hpp"

/* ������� ������ ���������,
����� �� ������ ������ ������������� ������, ������������ ������ � 1000 ���������.*/
const unsigned DEFAULT_DOCUMENT_SIZE = 1000;

TextEditorCore::TextEditorCore() : currentMode{ mode::Edit },
coursor{}, selection{}, find{}, container{ DEFAULT_DOCUMENT_SIZE }
{}


