// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

std::string getStringFromPointer(int* str) {
	int len = str[4];

	std::string ret;

	if (len <= 15) {
		/*char* dst = new char[len + 1];

		memcpy((void*)dst, (void*)str, len);
		dst[len] = 0;*/

		ret = (const char*)str;
	}
	else {
		const char* src = *(const char**)str;

		ret = src;
	}

	return ret;
}

void SetStringPointer(int* str, const char* text) {
	int len = strlen(text);

	str[4] = len;
	str[5] = max(len, 15);

	if (len <= 15)
		memcpy((void*)str, text, len + 1);
	else {
		void* newstr = malloc(len + 1);
		memcpy(newstr, text, len + 1);
		*(char**)str = (char*)newstr;
	}
}

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
