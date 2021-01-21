#include "global_func.h"

namespace global_func
{
	vector<string> tokenizer(char *str, const char *separator)
	{
		char *token;
		vector<string> vec;
		int len;

		len = strlen(str);
		if (str[len - 1] == '\n')
			str[len - 1] = 0;


		token = strtok(str, separator);
		while (token != 0)
		{
			vec.push_back(token);
			token = strtok(0, separator);
		}

		return vec;
	}

	bool is_subs(string stack, string needle)
	{
		bool is_pass = false;

		size_t posisi = stack.find(needle);
		is_pass = posisi != string::npos;

		return is_pass;
	}

	bool isNumber(string token)
	{
		return std::regex_match(token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
	}
}