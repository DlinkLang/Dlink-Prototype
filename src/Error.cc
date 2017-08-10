#include "Error.hh"

#include <utility>

namespace Dlink
{
	/**
	 * @brief �� Error �ν��Ͻ��� ����ϴ�.
	 * @param error_token ������ �߻��� �κ��� ��ū�Դϴ�.
	 * @param error_message ���� �޼����Դϴ�.
	 */
	Error::Error(const Token& error_token, const std::string& error_message)
		: error_token_(error_token), error_message_(error_message)
	{}
	/**
	 * @brief ���� �ν��Ͻ��� ������ �� Error �ν��Ͻ��� ����ϴ�.
	 * @param error ������ ���� �ν��Ͻ��Դϴ�.
	 */
	Error::Error(const Error& error)
		: error_token_(error.error_token_), error_message_(error.error_message_)
	{}
	/**
	 * @brief ���� �ν��Ͻ��� �ʵ带 �̵��� �� Error �ν��Ͻ��� ����ϴ�.
	 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
	 * @param error �̵��� ���� �ν��Ͻ��Դϴ�.
	 */
	Error::Error(Error&& error) noexcept
		: error_token_(std::move(error.error_token_)), error_message_(std::move(error.error_message_))
	{}

	/**
	 * @brief ���� �޼����� �����ɴϴ�.
	 * @return ���� �޼����� std::string Ÿ������ ��ȯ�մϴ�.
	 */
	std::string Error::what() const
	{
		return error_message_;
	}
	/**
	 * @brief ������ �߻��� �κ��� ��ū�� �����ɴϴ�.
	 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
	 * @return ������ �߻��� �κ��� ��ū�� ��ȯ�մϴ�.
	 */
	const Token& Error::error_token() const noexcept
	{
		return error_token_;
	}

	/**
	 * @brief ������ �߰��մϴ�.
	 * @details �̹� �߰��� ������� �߰����� �ʽ��ϴ�.
	 * @param error �߰��� �����Դϴ�.
	 */
	void Errors::add_error(const Error& error)
	{
		const Token& error_token = error.error_token();

		for (auto old_error : errors_)
		{
			const Token& old_token = old_error.error_token();

			if (error.what() == old_error.what() &&
				error_token.line == old_token.line &&
				error_token.col == old_token.col)
			{
				return;
			}
		}

		errors_.push_back(error);
	}
	/**
	 * @brief �߰��� ���� ������ �����ɴϴ�.
	 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
	 * @return �߰��� ���� �����Դϴ�.
	 */
	const std::vector<Error>& Errors::get_errors() const noexcept
	{
		return errors_;
	}
}