#pragma once

/**
 * @file Encoder.hh
 * @date 2017.08.14
 * @author kmc7468
 * @brief UTF-32 인코딩을 위한 함수들을 정의합니다.
 */

#include <cstddef>
#include <string>

#include "Encoding.hh"

namespace Dlink
{
	std::size_t utf32_encode_length(const std::string& string);
	std::size_t utf32_encode_length(const std::string& string, Encoding encoding);
	
	std::size_t utf32_encode_length(const std::u16string& utf16string);

	std::u32string utf32_encode(const std::u16string& utf16string);
}