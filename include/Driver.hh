#pragma once

/**
 * @file Driver.hh
 * @date 2017.7.24
 * @author dev_kr
 * @brief 파서와 스캐너를 관리하는 Driver 클래스를 정의합니다.
 */

#include <istream>
#include <memory>

#include "Lexer.hh"
#include "parser.tab.hh"

namespace Dlink
{
	
	/**
	 * @brief 파서와 스캐너를 생성하고 파싱과 추상 문법 트리의 생성을 관리합니다.
	 */
	class Driver
	{
	public:
		Driver() = default;
		
		/**
		 * @brief 입력 스트림을 받아 입력을 받을 수 있는지 확인하고 스캐너와 파서를 초기화하고 파싱을 한뒤 성공 여부를 반환합니다.
		 * @param stream 입력 스트림입니다.
		 * @return 파싱에 성공하면 0, 실패하면 0 이외의 수를 반환합니다.
		 */
		int parse(std::istream& stream);
	private:
		std::unique_ptr<Dlink::Parser>  parser  = nullptr;
		std::unique_ptr<Dlink::Lexer> lexer = nullptr;
	};
}

