#pragma once

/**
 * @file Lexer.hh
 * @date 2017.08.02
 * @author dev_kr
 * @brief 입력을 Token 단위로 분석하고 나누어 반환하는 렉서를 정의합니다.
 */

#include <vector>
#include <map>

#include "Token.hh"

namespace Dlink
{
    class Lexer
    {
    private:
        /**
         * @brief 렉싱의 결과인 토큰 열입니다.
         */
        std::vector<Token> token_seq_;

        /**
         * @brief 키워드 문자열에 TokenType형의 토큰 타입을 매핑한 std::map 입니다.
         */
        std::map<std::string, TokenType> keyword_map_;

    public:
        /**
         * @brief 멤버 필드 keyword_map_를 매핑하고 Lexer를 생성합니다.
         */
        Lexer();
        /**
         * @brief 입력에 대해 렉싱을 수행하고 결과를 멤버 필드 token_seq_에 저장합니다.
         * @param str 렉서에 대한 입력입니다.
         */
        void lex(const std::string& str);
        
        /**
         * @brief 렉싱의 결과를 가져옵니다.
         * @return 렉싱의 결과를 저장해 두는 멤버 필드 token_seq_을 반환합니다.
         */
        const std::vector<Token>& get_token_seq() const;
    };
}
