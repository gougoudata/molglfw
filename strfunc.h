#ifndef _STRFUNC_H
#define _STRFUNC_H

#include<string>
#include<vector>

/* strpos */
size_t strpos(const std::string &haystack, const std::string &needle);

/* strreplace */
std::string& str_replace(const std::string &search, const std::string 
&replace, std::string &subject);

/* substring (two variants) */
std::string substr(const std::string &str, int start);
std::string substr(const std::string &str, int start, int length);

/* strtrim */
std::string& strtrim(std::string &subject);

/* explode */
std::vector<std::string> explode( const std::string &delimiter, const std::string &explodeme);

std::vector<std::string> grep(std::vector<std::string> &lines, std::string pattern);
bool is_match(const std::string& text, const std::string& pattern);

#endif // _STRFUNC_H
