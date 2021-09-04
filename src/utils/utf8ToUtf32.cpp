#include "utf8ToUtf32.h"

std::u32string utf8ToUtf32(const std::string& utf8_str)
{
	std::u32string utf32_str;

	char32_t utf32_char = 0;
	int bitc = 0;

	auto push = [&](int i)
	{
		if (i > 0)
		{
			if (bitc == 0 && !(utf32_char >= 0xD800 && utf32_char <= 0xDFFF) && utf32_char <= 0x10FFFF)
			{
				utf32_str.push_back(utf32_char);
			}
			else { utf32_str.push_back(U'\xFFFD'); }
		}
	};

	for (int i = 0; i < utf8_str.size(); i++)
	{
		if ((utf8_str[i] & 0b10000000) == 0b00000000)
		{
			push(i);
			bitc = 0;
			utf32_char = utf8_str[i];
		}
		else if ((utf8_str[i] & 0b11000000) == 0b10000000)
		{
			bitc -= 6;
			utf32_char |= (utf8_str[i] & 0b00111111) << bitc;
		}
		else if ((utf8_str[i] & 0b11100000) == 0b11000000)
		{
			push(i);
			bitc = 6;
			utf32_char = (utf8_str[i] & 0b000111111) << bitc;
		}
		else if ((utf8_str[i] & 0b11110000) == 0b11100000)
		{
			push(i);
			bitc = 12;
			utf32_char = (utf8_str[i] & 0b00001111) << bitc;
		}
		else if ((utf8_str[i] & 0b11111000) == 0b11110000)
		{
			push(i);
			bitc = 18;
			utf32_char = (utf8_str[i] & 0b00000111) << bitc;
		}
		else
		{
			push(i);
			bitc = -1;
		}
	}
	if (utf8_str.size() > 0) push(true);

	return utf32_str;
}