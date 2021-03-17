#ifndef WEB_FILE_HANDLE_HPP
#define WEB_FILE_HANDLE_HPP

#include "web.hpp"
#include <fstream>

#define CAST2CHARPTR(x) reinterpret_cast<char*>(&x)

void save(web &myweb);
void save(web *myweb);
web load(const char* filename, function*** p_function_input, function*** p_function_activation, function*** p_function_output);

#endif //WEB_FILE_HANDLE_HPP