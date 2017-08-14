#include "Encoder.hh"

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
	 */
	std::size_t utf32_encode_length(const std::string& string, Encoding encoding)
	{
		// TODO: ���� ���

		return std::size_t();
	}
}