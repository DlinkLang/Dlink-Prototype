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
	 * @brief ���ڿ��� UTF-32�� ���ڵ� ���� ���� ���̸� ����մϴ�.
	 * @param string UTF-32�� ���ڵ� �� ���ڿ��Դϴ�.
	 * @return ���ڿ��� UTF-32�� ���ڵ� ���� ���� ���̸� ��ȯ�մϴ�.
	 */
	std::size_t utf32_encode_length(const std::string& string)
	{
		return utf32_encode_length(string, check_encoding(string));
	}
	/**
	 * @brief ���ڿ��� UTF-32�� ���ڵ� ���� ���� ���̸� ����մϴ�.
	 * @param string UTF-32�� ���ڵ� �� ���ڿ��Դϴ�.
	 * @param encoding string�� ���ڵ��Դϴ�.
	 * @return ���ڿ��� UTF-32�� ���ڵ� ���� ���� ���̸� ��ȯ�մϴ�.
	 */
	std::size_t utf32_encode_length(const std::string& string, Encoding encoding)
	{
		// TODO: ���� ���

		return std::size_t();
	}

	/**
	 * @brief UTF-16 ���ڿ��� UTF-32�� ���ڵ� ���� ���� ���̸� ����մϴ�.
	 * @param string UTF-16���� ���ڵ� �� UTF-32�� ���ڵ� �� ���ڿ��Դϴ�.
	 * @return ���ڿ��� UTF-32�� ���ڵ� ���� ���� ���̸� ��ȯ�մϴ�.
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
	 * @brief UTF-16 ���ڿ��� UTF-32�� ���ڵ��մϴ�.
	 * @param utf16string UTF-16���� ���ڵ� �� UTF-32�� ���ڵ� �� ���ڿ��մϴ�.
	 * @return UTF-32�� ���ڵ� �� ���ڿ��� ��ȯ�մϴ�.
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
		
		// TODO: BOM�� ���� ��� ���ڵ� ó��
		return Encoding::None;
	}
}