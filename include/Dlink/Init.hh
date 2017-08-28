#pragma once

/**
 * @file Init.hh
 * @author dev_kr
 * @brief Dlink 컴파일 작업의 초기 설정과 관련된 기능들의 집합입니다.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "CommandLine.hh"
#include "CodeGen.hh"

namespace Dlink
{
	/**
	 * @brief 명령줄 데이터 처리 함수의 반환 타입입니다.
	 */
	using ProcessedType = std::string;

	ProcessedType ProcessCommandLine(int argc, char** argv);
}
