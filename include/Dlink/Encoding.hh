#pragma once

/**
 * @file Encoding.hh
 * @date 2017.08.14
 * @author kmc7468
 * @brief Encoding 열거형을 정의합니다.
 */

namespace Dlink
{
	/**
	 * @brief 인코딩 방식입니다.
	 */
	enum class Encoding
	{
		None = 0,

		UTF8,		/**< UTF-8 인코딩 방식입니다. */
		UTF16,		/**< UTF-16 Little Endian 인코딩 방식입니다. */
		UTF32,		/**< UTF-32 Little Endian 인코딩 방식입니다. */
		UTF16Big,	/**< UTF-16 Big Endian 인코딩 방식입니다. */
		UTF32Big,	/**< UTF-32 Big Endian 인코딩 방식입니다. */
	};
}