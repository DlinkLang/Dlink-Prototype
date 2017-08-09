#pragma once

/**
 * @file Lexer.hh
 * @date 2017.08.02
 * @author dev_kr
 * @brief 입력을 Token 단위로 분석하고 나누어 반환하는 렉서를 정의합니다.
 */

#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#include "Token.hh"

namespace Dlink
{
	/**
	 * @brief Dlink 코드를 토큰 단위로 쪼개는 렉서입니다.
	 */
    class Lexer
    {
    private:
        /**
         * @brief 렉싱의 결과인 토큰열입니다.
         */
        TokenSeq token_seq_;

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
        const TokenSeq& get_token_seq() const;
        
        /**
         * @brief 렉싱의 결과의 각 토큰마다 줄 번호, 세로줄 번호, 토큰 타입을 출력합니다.
         */
        void dump() const;
    };    
}         
