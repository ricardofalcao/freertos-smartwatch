#include "lang/lang.h"

void Lang::setLanguage(const Language_t * language) {
    active_language = language;
}

const char * Lang::get(Message_t message) {
    return active_language->messages[message];
}