#pragma once

/**
 * @file Token.hh
 * @author dev_kr
 * @brief Token 클래스를 정의합니다.
 */

#include <string>
#include <map>
#include <utility>
#include <vector>

namespace Dlink
{
	/**
	 * @brief 토큰의 타입입니다.
	 */
    enum class TokenType
    {
        none,               /**< 알 수 없는 토큰입니다. */
        eof,                /**< 파일의 끝입니다. */

        identifier,         /**< 식별자입니다. */

        bin_integer,        /**< 2진 정수 리터럴입니다. */
        oct_integer,        /**< 8진 정수 리터럴입니다. */
        dec_integer,        /**< 10진 정수 리터럴입니다. */
        hex_integer,        /**< 16진 정수 리터럴입니다. */

        floating,           /**< 실수 리터럴입니다. */

        character,          /**< 문자 리터럴입니다. */
        string,             /**< 문자열 리터럴입니다. */
        
        plus,               /**< 더하기 연산자 '+' 입니다. */
        increment,          /**< 증가 연산자 '++' 입니다. */
        plus_assign,        /**< 더하기 대입 연산자 '+=' 입니다. */

        minus,              /**< 빼기 연산자 '-' 입니다. */
        decrement,          /**< 감소 연산자 '--' 입니다. */
        minus_assign,       /**< 빼기 대입 연산자 '-=' 입니다. */
        
        multiply,           /**< 곱하기 연산자 '*' 입니다. */
        multiply_assign,    /**< 곱하기 대입 연산자 '*=' 입니다. */

        divide,             /**< 나누기 연산자 '/' 입니다. */ 
        divide_assign,      /**< 나누기 대입 연산자 '/=' 입니다. */

        modulo,             /**< 나머지 연산자 '%' 입니다. */
        modulo_assign,      /**< 나머지 대입 연산자 '%=' 입니다. */

        assign,             /**< 대입 연산자 '=' 입니다. */
        equal,              /**< 비교 연산자 '==' 입니다. */
        noteq,              /**< 대조 연산자 '!=' 입니다. */
        greater,            /**< 초과 연산자 '>' 입니다. */
        eqgreater,          /**< 이상 연산자 '>=' 입니다. */
        less,               /**< 미만 연산자 '<' 입니다. */
        eqless,             /**< 이하 연산자 '<=' 입니다. */

        logic_and,          /**< 논리 AND 연산자 '&&' 입니다. */
        logic_or,           /**< 논리 OR 연산자 '||' 입니다. */

        bit_not,            /**< 비트 NOT 연산자 '~' 입니다. */
        bit_and,            /**< 비트 AND 연산자 '&' 입니다. */
        bit_and_assign,     /**< 비트 AND 대입 연산자 '&=' 입니다. */
        bit_or,             /**< 비트 OR 연산자 '|' 입니다. */
        bit_or_assign,      /**< 비트 OR 대입 연산자 '|=' 입니다. */
        bit_xor,            /**< 비트 XOR 연산자 '^' 입니다. */
        bit_xor_assign,     /**< 비트 XOR 대입 연산자 '^=' 입니다. */
        bit_lshift,         /**< 비트 좌쉬프트 연산자 '<<' 입니다. */
        bit_lshift_assign,  /**< 비트 좌쉬프트 대입 연산자 '<<=' 입니다. */
        bit_rshift,         /**< 비트 우쉬프트 연산자 '>>' 입니다. */
        bit_rshift_assign,  /**< 비트 우쉬프트 대입 연산자 '>>=' 입니다. */

        lbrace,             /**< 여는 중괄호 '{' 입니다. */
        rbrace,             /**< 닫는 중괄호 '}' 입니다. */
        lparen,             /**< 여는 소괄호 '(' 입니다. */ 
        rparen,             /**< 닫는 소괄호 ')' 입니다. */
        lbparen,            /**< 여는 대괄호 '[' 입니다. */
        rbparen,            /**< 닫는 대괄호 ']' 입니다. */

        dot,                /**< 온점 '.' 입니다. */
        comma,              /**< 쉼표 ',' 입니다. */
        apostrophe,         /**< 어포스트로피 '\'' 입니다. */
        semicolon,          /**< 세미콜론 ';' 입니다. */
        colon,              /**< 콜론 ':' 입니다. */

        exclamation,        /**< 느낌표 '!' 입니다. */
        question,           /**< 물음표 '?' 입니다. */

		unsafe,				/**< 키워드 'unsafe' 입니다. */
        _if,                /**< 키워드 'if' 입니다. */
        _else,              /**< 키워드 'else' 입니다. */
        _switch,            /**< 키워드 'switch' 입니다. */
        _case,              /**< 키워드 'case' 입니다. */
        _for,               /**< 키워드 'for' 입니다. */
        _while,             /**< 키워드 'while' 입니다. */
        _do,                /**< 키워드 'do' 입니다. */
        _break,             /**< 키워드 'break' 입니다. */
        _continue,          /**< 키워드 'continue' 입니다. */
        _return,            /**< 키워드 'return' 입니다. */
        _null,              /**< 키워드 'null' 입니다. */
        _const,             /**< 키워드 'const' 입니다. */
		literal,			/**< 키워드 'literal' 입니다. */

		_unsigned,			/**< 키워드 'unsigned' 입니다. */
		_signed,			/**< 키워드 'signed' 입니다. */
		_char,				/**< 키워드 'char' 입니다. */
		byte,				/**< 키워드 'byte' 입니다. */
		_short,				/**< 키워드 'short' 입니다. */
		_int,				/**< 키워드 'int' 입니다. */
		_long,				/**< 키워드 'long' 입니다. */
		_void,				/**< 키워드 'void' 입니다. */
		half,				/**< 키워드 'half' 입니다. */
		single,				/**< 키워드 'single' 입니다. */
		_double,			/**< 키워드 'double' 입니다. */
    };

	/**
	 * @brief Dlink 코드를 구성하는 최소한의 단위입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 * @see Dlink::Lexer
	 */
    struct Token final
    {
		Token() = default;
		Token(const std::string& data_, TokenType type_, const std::size_t line_, const std::size_t end_col_);
    
		static const Token empty;

		/** 토큰의 원본 문장입니다. */
		std::string data;
		/** TokenType 형식의 토큰의 타입입니다. */
		TokenType type;

		/** 토큰의 줄 번호입니다. */
		std::size_t line;
		/** 토큰이 끝나는 위치의 세로단 번호입니다. */
		std::size_t end_col;
		/** 토큰이 시작되는 위치의 세로단 번호입니다. */
		std::size_t col;
	};

	/** Token에 대한 std::vector 타입입니다. */
    using TokenSeq = std::vector<Token>;

    extern const std::map<TokenType, std::string> token_map;
}
