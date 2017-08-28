#include "Init.hh"

namespace Dlink
{
	/**
	 * @brief 커맨드 라인에서 파싱된 데이터를 기반으로 파일 로드 등의 작업을 수행합니다.
	 * @param argc 명령줄의 개수입니다.
	 * @param argv 명령줄 배열입니다.
	 * @return 로드된 파일의 내용입니다.
	 */
	ProcessedType ProcessCommandLine(int argc, char** argv)
	{
		auto cmd_line = Dlink::ParseCommandLine(argc, argv);
		auto cmd_line_err = Dlink::CheckError_ParsedCommandLine(cmd_line);
		
		if (cmd_line_err.first != Dlink::ParsedCommandLine::Error::Done)
		{
			throw cmd_line_err.first;
		}

		std::string code_filename;
		long long opt_level = 0;

		for (auto cmd : cmd_line)
		{
			if (cmd.type == Dlink::ParsedCommandLine::Type::Input)
			{
				code_filename = *reinterpret_cast<std::string*>(cmd.x);
			}
			else if(cmd.type == Dlink::ParsedCommandLine::Type::Optimize)
			{
				opt_level = cmd.x;
			}
		}

		Dlink::LLVM::function_pm = std::make_unique<llvm::legacy::FunctionPassManager>(Dlink::LLVM::module.get());

		if (opt_level > 0)
		{
			Dlink::LLVM::function_pm->add(llvm::createInstructionCombiningPass());
			Dlink::LLVM::function_pm->add(llvm::createReassociatePass());
			Dlink::LLVM::function_pm->add(llvm::createGVNPass());
			Dlink::LLVM::function_pm->add(llvm::createCFGSimplificationPass());
		}

		Dlink::LLVM::function_pm->doInitialization();

		std::ifstream code_file(code_filename);
		std::string code((std::istreambuf_iterator<char>(code_file)), std::istreambuf_iterator<char>());

		return code;
	}
}
