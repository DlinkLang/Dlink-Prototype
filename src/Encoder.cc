#include "Encoder.hh"

namespace Dlink
{
	/**
	 * @brief 문자열을 UTF-32로 인코딩 했을 때의 길이를 계산합니다.
	 * @param string UTF-32로 인코딩 할 문자열입니다.
	 * @return 문자열을 UTF-32로 인코딩 했을 때의 길이를 반환합니다.
	 */
	std::size_t utf32_encode_length(const std::string& string)
	{
		Encoding encoding;

		// TODO: 인코딩 추론

		return utf32_encode_length(string, encoding);
	}

	/**
	 * @brief 문자열을 UTF-32로 인코딩 했을 때의 길이를 계산합니다.
	 * @param string UTF-32로 인코딩 할 문자열입니다.
	 * @param encoding string의 인코딩입니다.
	 */
	std::size_t utf32_encode_length(const std::string& string, Encoding encoding)
	{
		// TODO: 길이 계산

		return std::size_t();
	}
}