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
        
        std::string what() const noexcept
        {
            return error_message_;
        }

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

        std::vector<Error> get_errors() const noexcept
        {
            return errors_;
        }
    };
}
