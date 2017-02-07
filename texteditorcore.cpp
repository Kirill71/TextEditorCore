#include "texteditorcore.hpp"

/* Базовый размер документа,
чтобы не делать частые перевыделения памяти, устанавливаю размер в 1000 элементов.*/
const unsigned DEFAULT_DOCUMENT_SIZE = 1000;

TextEditorCore::TextEditorCore() : currentMode{ mode::Edit },
coursor{}, selection{}, find{}, container{ DEFAULT_DOCUMENT_SIZE }
{}


