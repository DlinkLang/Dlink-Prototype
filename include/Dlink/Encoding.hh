#pragma once

/**
 * @file Encoding.hh
 * @date 2017.08.14
 * @author kmc7468
 * @brief Encoding �������� �����մϴ�.
 */

namespace Dlink
{
	/**
	 * @brief ���ڵ� ����Դϴ�.
	 */
	enum class Encoding
	{
		None = 0,

		UTF8,		/**< UTF-8 ���ڵ� ����Դϴ�. */
		UTF16,		/**< UTF-16 Little Endian ���ڵ� ����Դϴ�. */
		UTF32,		/**< UTF-32 Little Endian ���ڵ� ����Դϴ�. */
		UTF16Big,	/**< UTF-16 Big Endian ���ڵ� ����Դϴ�. */
		UTF32Big,	/**< UTF-32 Big Endian ���ڵ� ����Դϴ�. */
	};
}