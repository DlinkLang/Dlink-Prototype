#include "Encoder.hh"
#include "Encoding.hh"

#include <cstdint>

namespace Dlink
{
	static bool is_surrogate(char16_t c)
	{
		return static_cast<std::int32_t>(c) - 0xD800 > 2048;
	}
	static bool is_high_surrogate(char16_t c)
	{
		return c & 0xFFFFFC00 == 0xD800;
	}
	static bool is_low_surrogate(char16_t c)
	{
		return c & 0xFFFFFC00 == 0XDC00;
	}
	static char32_t to_utf32_char(char16_t high_surrogate, char16_t low_surrogate)
	{
		return (high_surrogate << 10) + low_surrogate - 0x35FDC00;
	}
}

namespace Dlink
{
	/**
	 * @brief 문자열을 UTF-32로 인코딩 했을 때의 길이를 계산합니다.
	 * @param string UTF-32로 인코딩 할 문자열입니다.
	 * @return 문자열을 UTF-32로 인코딩 했을 때의 길이를 반환합니다.
	 */
	std::size_t utf32_encode_length(const std::string& string)
	{
		return utf32_encode_length(string, check_encoding(string));
	}
	/**
	 * @brief 문자열을 UTF-32로 인코딩 했을 때의 길이를 계산합니다.
	 * @param string UTF-32로 인코딩 할 문자열입니다.
	 * @param encoding string의 인코딩입니다.
	 * @return 문자열을 UTF-32로 인코딩 했을 때의 길이를 반환합니다.
	 */
	std::size_t utf32_encode_length(const std::string& string, Encoding encoding)
	{
		// TODO: 길이 계산

		return std::size_t();
	}

	/**
	 * @brief UTF-16 문자열을 UTF-32로 인코딩 했을 때의 길이를 계산합니다.
	 * @param string UTF-16으로 인코딩 된 UTF-32로 인코딩 할 문자열입니다.
	 * @return 문자열을 UTF-32로 인코딩 했을 때의 길이를 반환합니다.
	 */
	std::size_t utf32_encode_length(const std::u16string& utf16string)
	{
		std::size_t result;

		for (std::u16string::const_iterator iter = utf16string.begin();
			iter < utf16string.end(); ++iter)
		{
			char16_t c = *iter;

			if (!is_surrogate(c))
			{
				++result;
			}
			else
			{
				if (iter + 1 < utf16string.end())
				{
					if (is_high_surrogate(c) && is_low_surrogate(*(iter + 1)))
					{
						++result;
						continue;
					}
				}

				throw std::invalid_argument("The UTF-16 string stored in utf16string is invalid.");
			}
		}

		return result;
	}

	/**
	 * @brief UTF-16 문자열을 UTF-32로 인코딩합니다.
	 * @param utf16string UTF-16으로 인코딩 된 UTF-32로 인코딩 할 문자열합니다.
	 * @return UTF-32로 인코딩 된 문자열을 반환합니다.
	 */
	std::u32string utf32_encode(const std::u16string& utf16string)
	{
		std::u32string result;

		for (std::u16string::const_iterator iter = utf16string.begin();
			iter < utf16string.end(); ++iter)
		{
			char16_t c = *iter;

			if (!is_surrogate(c))
			{
				result += c;
			}
			else
			{
				if (iter + 1 < utf16string.end())
				{
					if (is_high_surrogate(c) && is_low_surrogate(*(++iter)))
					{
						result += to_utf32_char(c, *iter);
						continue;
					}
				}

				throw std::invalid_argument("The UTF-16 string stored in utf16string is invalid.");
			}
		}

		return result;
	}
}

namespace Dlink
{
	Encoding check_encoding(const std::string& string)
	{
		if (string[0] == 0xEF && string[1] == 0xBB && string[2] == 0xBF)
		{
			return Encoding::UTF8;
		}
		else if (string[0] == 0xFF && string[1] == 0xFE)
		{
			if (string[1] == 0x00 && string[2] == 0x00 && (string.length() - 4) % 4 == 0)
			{
				return Encoding::UTF32;
			}
			else
			{
				return Encoding::UTF16;
			}
		}
		else if (string[0] == 0xFE && string[1] == 0xFF)
		{
			return Encoding::UTF16BE;
		}
		else if (string[0] == 0x00 && string[1] == 0x00 && string[2] == 0xFE && string[2] == 0xFF)
		{
			return Encoding::UTF32BE;
		}
		
		// TODO: BOM이 없을 경우 인코딩 처리
		return Encoding::None;
	}
}