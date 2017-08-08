#pragma once

/**
 * @file Token.hh
 * @date 2017.08.02
 * @author dev_kr
 * @brief Dlink의 렉서가 반환하는 토큰 타입을 정의합니다.
 */

#include <string>
#include <map>
#include <utility>
#include <vector>

namespace Dlink
{
    enum class TokenType
    {
        none,               /**< 알 수 없는 토큰 */
        eof,                /**< 파일의 끝 */

        identifier,         /**< 식별자 */

        bin_integer,        /**< 2진 정수 리터럴 */
        oct_integer,        /**< 8진 정수 리터럴 */
        dec_integer,        /**< 10진 정수 리터럴 */
        hex_integer,        /**< 16진 정수 리터럴 */

        floating,           /**< 실수 리터럴 */

        character,          /**< 문자 리터럴 */
        string,             /**< 문자열 리터럴 */
        
        plus,               /**< 더하기 연산자 '+' */
        increment,          /**< 증가 연산자 '++' */
        plus_assign,        /**< 더하기 대입 연산자 '+=' */

        minus,              /**< 빼기 연산자 '-' */
        decrement,          /**< 감소 연산자 '--' */
        minus_assign,       /**< 빼기 대입 연산자 '-=' */
        
        multiply,           /**< 곱하기 연산자 '*'  */
        multiply_assign,    /**< 곱하기 대입 연산자 '*=' */

        divide,             /**< 나누기 연산자 '/' */ 
        divide_assign,      /**< 나누기 대입 연산자 '/=' */

        modulo,             /**< 나머지 연산자 '%' */
        modulo_assign,      /**< 나머지 대입 연산자 '%=' */

        assign,             /**< 대입 연산자 '=' */
        equal,              /**< 비교 연산자 '==' */
        noteq,              /**< 대조 연산자 '!=' */
        greater,            /**< 초과 연산자 '>' */
        eqgreater,          /**< 이상 연산자 '>=' */
        less,               /**< 미만 연산자 '<' */
        eqless,             /**< 이하 연산자 '<=' */

        logic_and,          /**< 논리 AND 연산자 '&&' */
        logic_or,           /**< 논리 OR 연산자 '||' */

        bit_not,            /**< 비트 NOT 연산자 '~' */
        bit_and,            /**< 비트 AND 연산자 '&' */
        bit_and_assign,     /**< 비트 AND 대입 연산자 '&=' */
        bit_or,             /**< 비트 OR 연산자 '|' */
        bit_or_assign,      /**< 비트 OR 대입 연산자 '|=' */
        bit_xor,            /**< 비트 XOR 연산자 '^' */
        bit_xor_assign,     /**< 비트 XOR 대입 연산자 '^=' */
        bit_lshift,         /**< 비트 좌쉬프트 연산자 '<<' */
        bit_lshift_assign,  /**< 비트 좌쉬프트 대입 연산자 '<<=' */
        bit_rshift,         /**< 비트 우쉬프트 연산자 '>>' */
        bit_rshift_assign,  /**< 비트 우쉬프트 대입 연산자 '>>=' */

        lbrace,             /**< 여는 중괄호 '{' */
        rbrace,             /**< 닫는 중괄호 '}' */
        lparen,             /**< 여는 소괄호 '(' */ 
        rparen,             /**< 닫는 소괄호 ')' */
        lbparen,            /**< 여는 대괄호 '[' */
        rbparen,            /**< 닫는 대괄호 ']' */

        dot,                /**< 온점 '.' */
        comma,              /**< 쉼표 ',' */
        apostrophe,         /**< 어포스트로피 '\'' */
        semicolon,          /**< 세미콜론 ';' */
        colon,              /**< 콜론 ':' */

        exclamation,        /**< 느낌표 '!' */
        question,           /**< 물음표 '?' */

        _if,                /**< 키워드 'if' */
        _else,              /**< 키워드 'else' */
        _switch,            /**< 키워드 'switch' */
        _case,              /**< 키워드 'case' */
        _for,               /**< 키워드 'for' */
        _while,             /**< 키워드 'while' */
        _do,                /**< 키워드 'do' */
        _break,             /**< 키워드 'break' */
        _continue,          /**< 키워드 'continue' */
        _return,            /**< 키워드 'return' */
        _null,              /**< 키워드 'null' */
        _const,             /**< 키워드 'const' */
    };

    struct Token
    {
        /**
         * @brief 토큰의 실제 문자열 데이터입니다.
         */
        std::string data;
        /**
         * @brief TokenType 형식의 토큰의 타입입니다.
         */
        TokenType type;

        /**
         * @brief 토큰의 줄 번호입니다.
         */
        std::size_t line;    
        /**
         * @brief 토큰이 끝나는 위치의 세로단 번호입니다.
         */
        std::size_t end_col;
        /**
         * @brief 토큰이 시작되는 위치의 세로단 번호입니다.
         */
        std::size_t col;

        Token(const std::string& data_, TokenType type_, const std::size_t line_, const std::size_t end_col_)
            : data(data_), type(type_), line(line_), end_col(end_col_), col(end_col_ - data_.length())
        {}
    };

    using TokenSeq = std::vector<Token>;

    /**
     * @brief 토큰 타입을 문자열화 시켜 매핑하여 담아두는 전역 상수입니다.
     */
    extern const std::map<TokenType, std::string> token_map;
}
