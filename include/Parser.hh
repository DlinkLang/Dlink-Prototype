#pragma once

/**
 * @file Parser.hh
 * @date 2017.07.27
 * @author dev_kr
 * @brief 문법의 정의 및 파서 객체의 생성을 관리합니다.
 */

#include <memory>

#include "peglib.hh"

#include "ParseStruct.hh"

namespace Dlink
{

    class Parser : private peg::parser
    {
    public:

        /**
         * @brief 정의된 문법을 통한 파서의 초기화 및 파싱 액션 설정을 합니다.
         */
        Parser();

        static const char grammar[];

        using peg::parser::operator bool;
        using peg::parser::parse_n;
        using peg::parser::parse;
        using peg::parser::search;
        using peg::parser::operator[];  
        using peg::parser::enable_packrat_parsing;
        using peg::parser::enable_ast;
        using peg::parser::enable_trace;
    };
};
