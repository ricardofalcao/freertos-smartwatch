#include "lang/lang.h"

const Language_t * Lang::getLanguage() {
    return active_language;
}

void Lang::setLanguage(const Language_t * language) {
    active_language = language;
}

const char * Lang::get(Message_t message) {
    return active_language->messages[message];
}