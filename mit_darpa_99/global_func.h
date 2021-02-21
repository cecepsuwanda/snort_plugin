
#include <vector>
#include <regex>
#include <string>

using namespace std;

#ifndef Included_global_func_H
#define Included_global_func_H

namespace global_func
{
	vector<string> tokenizer(char *str, const char *separator);

	bool is_subs(string stack, string needle);

	bool isNumber(string token);

	void remove_cr(string& tmp_str);
}
#endif
