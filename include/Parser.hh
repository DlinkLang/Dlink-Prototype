#pragma once

/**
 * @file Parser.hh
 * @date 2017.08.08
 * @author dev_kr
 * @brief 렉서가 반환한 토큰열을 입력받아 AST를 반환하는 Parser 클래스를 정의합니다.
 */

#include <iostream>
#include <vector>
#include <set>

#include "Token.hh"
#include "ParseStruct.hh"
#include "Error.hh"

namespace Dlink
{
    class Parser
    {
    private:
        /**
         * @brief 입력으로 들어온 토큰열입니다.
         */
        TokenSeq input_;
        /**
         * @brief 현재 파싱중인 토큰을 가르키는 반복자입니다.
         */
        TokenSeq::const_iterator token_iter_; 
        /**
         * @brief 파싱 결과 트리입니다.
         */
        ExpressionPtr output_;
        
        /**
         * @brief 파싱중 생긴 에러들을 담아둡니다.
         */
        Errors errors_;
        
        Token current_token()
        {
            return (*token_iter_);
        }

        Token previous_token()
        {
            return (*(token_iter_ - 1));
        }

        Token next_token()
        {
            return (*(token_iter_ + 1));
        }

        /**
         * @brief 다음 토큰이 주어진 토큰 타입과 맞는지 확인하고 받아들입니다.
         * @param token_type 받아들일 토큰 타입입니다.
         * @return 다음 토큰이 주어진 토큰 타입과 일치하면 true, 아니면 false를 반환합니다.
         */
        bool accept(TokenType token_type); 

        bool expr(ExpressionPtr& out);
        bool addsub(ExpressionPtr& out);
        bool muldiv(ExpressionPtr& out);
        bool number(ExpressionPtr& out);
    public:
        Parser(const TokenSeq& input);

        /**
         * @brief 받아들인 입력에 대해 파싱을 합니다.
         * @param output 결과 AST를 이 AST 레퍼런스에 대입합니다.
         * @return 파싱에 성공하면 true, 아니면 false를 반환합니다.
         */
        bool parse(ExpressionPtr& output);

        /**
         * @brief 파싱중 생긴 에러들을 가져옵니다.
         * @return 파싱중 생긴 에러들을 Error 클래스의 std::vector 형식으로 반환합니다.
         */
        std::vector<Error> get_errors() const noexcept;
    };
}
