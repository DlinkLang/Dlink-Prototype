#include "Encoder.hh"

#include <cstdint>

namespace Dlink
{
	static bool is_surrogate(char16_t c)
	{
		return static_cast<std::int32_t>(c) - 0xD800 < 2048;
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
		Encoding encoding;

		// TODO: ���ڵ� �߷�

		return utf32_encode_length(string, encoding);
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