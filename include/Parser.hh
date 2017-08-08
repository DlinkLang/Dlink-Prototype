#pragma once

/**
 * @file Lexer.hh
 * @date 2017.08.08
 * @author dev_kr
 * @brief 렉서가 반환한 토큰열을 입력받아 AST를 반환하는 Parser 클래스를 정의합니다.
 */

#include <iostream>
#include <vector>

#include "Token.hh"
#include "ParseStruct.hh"

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
        TokenSeq::const_iterator current_token_; 
        /**
         * @brief 파싱 결과 트리입니다.
         */
        ExpressionPtr output_;

        /**
         * @brief 다음 토큰이 주어진 토큰 타입과 맞는지 확인하고 받아들입니다.
         * @param 받아들일 토큰 타입입니다.
         * @return 다음 토큰이 주어진 토큰 타입과 일치하면 true, 아니면 false를 반환합니다.
         */
        bool accept(TokenType token_type); 

        bool expr(ExpressionPtr& out);
        bool addsub(ExpressionPtr& out);
        bool muldiv(ExpressionPtr& out);
        bool number(ExpressionPtr& out);
    public:
        Parser(const TokenSeq& input);

        bool parse(ExpressionPtr& output);
    };
}
