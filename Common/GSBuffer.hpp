#ifndef _RSS_COMMON_GSBUFFER_HPP_
#define _RSS_COMMON_GSBUFFER_HPP_

#include <string>

extern int gamespy_buffer_validate(std::string &buffer);
extern std::string gamespy_buffer_get(std::string &buffer, std::string what);

#endif
