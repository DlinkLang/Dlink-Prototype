#pragma once

/**
 * @file Error.hh
 * @date 2017.08.08
 * @author dev_kr
 * @brief 파싱 또는 CodeGen 도중에 발생하는 에러를 추상화한 Error 클래스를 정의합니다.
 */

#include <string>
#include <vector>

#include "Token.hh"

namespace Dlink
{
    class Error
    {
    private:
        Token error_token_;
        std::string error_message_;
    public:
        Error(const Token& error_token, const std::string& error_message)
            : error_token_(error_token), error_message_(error_message)
        {}
        
        /**
         * @brief 에러 메세지를 가져옵니다.
         * @return 에러 메세지를 std::string의 형태로 반환합니다.
         */
        std::string what() const noexcept
        {
            return error_message_;
        }

        /**
         * @brief 에러가 난 토큰을 가져옵니다.
         * @return 에러가 난 토큰을 반환합니다.
         */
        const Token& error_token() const noexcept
        {
            return error_token_;
        }
    };

    class Errors
    {
    private:
        std::vector<Error> errors_;
    public:

        /**
         * @brief 에러를 추가합니다. 중복이 있으면 추가하지 않습니다.
         * @param error 추가할 에러입니다.
         */
        void add_error(Error error)
        {
            for(auto temp_error : errors_)
            {
                Token error_token = error.error_token();
                Token temp_token = temp_error.error_token();

                if(error.what() == temp_error.what() &&
                   error_token.line == temp_token.line &&
                   error_token.col == temp_token.col)
                {
                    return;
                }
            }

            errors_.push_back(error);
        }

        /**
         * @brief 에러들을 가져옵니다.
         * @return 에러들을 Error 클래스의 std::vector 형식으로 반환합니다.
         */
        std::vector<Error> get_errors() const noexcept
        {
            return errors_;
        }
    };
}
