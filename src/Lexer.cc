#include "Lexer.hh"

#include <cctype>

namespace Dlink
{
	/**
	 * @brief 새 Lexer 인스턴스를 만듭니다.
	 */
	Lexer::Lexer()
	{
		keyword_map_["if"] = TokenType::_if;
		keyword_map_["else"] = TokenType::_else;
		keyword_map_["switch"] = TokenType::_switch;
		keyword_map_["case"] = TokenType::_case;
		keyword_map_["for"] = TokenType::_for;
		keyword_map_["while"] = TokenType::_while;
		keyword_map_["do"] = TokenType::_do;
		keyword_map_["break"] = TokenType::_break;
		keyword_map_["continue"] = TokenType::_continue;
		keyword_map_["return"] = TokenType::_return;
		keyword_map_["null"] = TokenType::_null;
		keyword_map_["const"] = TokenType::_const;
		
		keyword_map_["unsigned"] = TokenType::_unsigned;
		keyword_map_["signed"] = TokenType::_signed;
		keyword_map_["char"] = TokenType::_char;
		keyword_map_["short"] = TokenType::_short;
		keyword_map_["int"] = TokenType::_int;
		keyword_map_["long"] = TokenType::_long;
	}

	/**
	 * @brief Dlink 코드에 대해 렉싱 작업을 수행합니다.
	 * @param code 렉싱 작업을 수행할 Dlink 코드입니다.
	 */
	void Lexer::lex(const std::string& code)
	{
		std::size_t line = 1, i = 0;

		std::stringstream input_stream(code);
		std::string cur_line;

		while (std::getline(input_stream, cur_line, '\n'))
		{
			for (i = 0; i < cur_line.length(); i++)
			{
				char ch = cur_line[i];

				if (std::isalpha(ch))
				{
					std::string temp = "";
					do
					{
						temp += cur_line[i];
						i++;
					} while (std::isalpha(cur_line[i]) || std::isdigit(cur_line[i]) || cur_line[i] == '_');

					i--;

					bool is_keyword = (keyword_map_.find(temp) != keyword_map_.end());

					if (is_keyword)
					{
						Token keyword_token(temp, keyword_map_[temp], line, i + 1);
						token_seq_.push_back(keyword_token);
					}
					else
					{
						Token identifier_token(temp, TokenType::identifier, line, i + 1);
						token_seq_.push_back(identifier_token);
					}
				}
				else if (std::isdigit(ch))
				{
					std::string temp = "";

					if (ch == '0' && i < cur_line.length() - 2)
					{
						char next_ch = cur_line[i + 1];
						switch (next_ch)
						{
						case 'b':
						case 'B':
							temp += '0';
							temp += next_ch;
							i += 2;

							do
							{
								temp += cur_line[i];
								i++;
							} while (cur_line[i] == '0' || cur_line[i] == '1');
							i--;

							token_seq_.push_back(Token(temp, TokenType::bin_integer, line, i + 1));
							break;

						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
							temp += '0';
							i++;

							do
							{
								temp += cur_line[i];
								i++;
							} while (('0' <= cur_line[i]) && (cur_line[i] <= '7'));
							i--;

							token_seq_.push_back(Token(temp, TokenType::oct_integer, line, i + 1));
							break;

						case 'x':
						case 'X':
							temp += '0';
							temp += next_ch;
							i += 2;

							do
							{
								temp += cur_line[i];
								i++;
							} while (i < cur_line.length() && (std::isdigit(cur_line[i]) ||
								(('a' <= cur_line[i]) && (cur_line[i] <= 'f')) ||
								(('A' <= cur_line[i]) && (cur_line[i] <= 'F'))));
							i--;

							token_seq_.push_back(Token(temp, TokenType::hex_integer, line, i + 1));
							break;

						default:
							token_seq_.push_back(Token("0", TokenType::dec_integer, line, i + 1));
							break;
						}
					}
					else
					{
						do
						{
							temp += cur_line[i];
							i++;
						} while (i < cur_line.length() && std::isdigit(cur_line[i]));

						if (cur_line[i] == '.')
						{
							do
							{
								temp += cur_line[i];
								i++;
							} while (i < cur_line.length() && std::isdigit(cur_line[i]));

							if (cur_line[i] == 'e' && (cur_line[i + 1] == '-' || cur_line[i + 1] == '+'))
							{
								temp += 'e';
								temp += cur_line[i + 1];
								i += 2;
								do
								{
									temp += cur_line[i];
									i++;
								} while (i < cur_line.length() && std::isdigit(cur_line[i]));
							}

							token_seq_.push_back(Token(temp, TokenType::floating, line, i));
						}
						else
						{
							token_seq_.push_back(Token(temp, TokenType::dec_integer, line, i));
						}
						i--;
					}
				}
				else if (ch == '\"')
				{
					std::string temp = "";
					i++;

					do
					{
						if (cur_line[i] == '\\')
						{
							if (i < cur_line.length() - 2 && cur_line[i + 1] == '\"')
							{
								temp += '\"';
								i += 2;
								continue;
							}
						}

						temp += cur_line[i];
						i++;
					} while (cur_line[i] != '\"');

					token_seq_.push_back(Token(temp, TokenType::string, line, i - 1));
				}
				else
				{
					switch (ch)
					{
					case '+':
						if (cur_line[i + 1] == '+')
						{
							i++;
							token_seq_.push_back(Token("++", TokenType::increment, line, i + 1));
						}
						else if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token("+=", TokenType::plus_assign, line, i + 1));
						}
						else
						{
							token_seq_.push_back(Token("+", TokenType::plus, line, i + 1));
						}
						break;
					case '-':
						if (cur_line[i + 1] == '-')
						{
							i++;
							token_seq_.push_back(Token("--", TokenType::decrement, line, i + 1));
						}
						else if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token("-=", TokenType::minus_assign, line, i + 1));
						}
						else
						{
							token_seq_.push_back(Token("-", TokenType::minus, line, i + 1));
						}
						break;
					case '*':
						if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token("*=", TokenType::multiply_assign, line, i + 1));
						}
						else
						{
							token_seq_.push_back(Token("*", TokenType::multiply, line, i + 1));
						}
						break;
					case '/':
						if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token("/=", TokenType::divide_assign, line, i + 1));
						}
						else
						{
							token_seq_.push_back(Token("/", TokenType::divide, line, i + 1));
						}
						break;
					case '%':
						if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token("%=", TokenType::modulo_assign, line, i + 1));
						}
						else
						{
							token_seq_.push_back(Token("%", TokenType::modulo, line, i + 1));
						}
						break;
					case '=':
						if (cur_line[i + 1] != '=')
						{
							token_seq_.push_back(Token("=", TokenType::assign, line, i + 1));
						}
						else
						{
							i++;
							token_seq_.push_back(Token("==", TokenType::equal, line, i + 1));
						}
						break;
					case '>':
						if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token(">=", TokenType::eqless, line, i + 1));
						}
						else if (cur_line[i + 1] == '>')
						{
							i++;
							if (cur_line[i + 2] == '=')
							{
								i++;
								token_seq_.push_back(Token(">>=", TokenType::bit_lshift_assign, line, i + 1));
							}
							else
							{
								token_seq_.push_back(Token(">>", TokenType::bit_lshift, line, i + 1));
							}
						}
						else
						{
							token_seq_.push_back(Token(">", TokenType::less, line, i + 1));
						}
						break;
					case '<':
						if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token("<=", TokenType::eqless, line, i + 1));
						}
						else if (cur_line[i + 1] == '<')
						{
							i++;
							if (cur_line[i + 2] == '=')
							{
								i++;
								token_seq_.push_back(Token("<<=", TokenType::bit_rshift_assign, line, i + 1));
							}
							else
							{
								token_seq_.push_back(Token("<<", TokenType::bit_rshift, line, i + 1));
							}
						}
						else
						{
							token_seq_.push_back(Token("<", TokenType::less, line, i + 1));
						}
						break;
					case '&':
						if (cur_line[i + 1] == '&')
						{
							i++;
							token_seq_.push_back(Token("&&", TokenType::logic_and, line, i + 1));
						}
						else if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token("&=", TokenType::bit_and_assign, line, i + 1));
						}
						else
						{
							token_seq_.push_back(Token("&", TokenType::bit_and, line, i + 1));
						}
						break;
					case '|':
						if (cur_line[i + 1] == '|')
						{
							i++;
							token_seq_.push_back(Token("||", TokenType::logic_or, line, i + 1));
						}
						else if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token("|=", TokenType::bit_or_assign, line, i + 1));
						}
						else
						{
							token_seq_.push_back(Token("|", TokenType::bit_or, line, i + 1));
						}
						break;
					case '~':
						token_seq_.push_back(Token("~", TokenType::bit_not, line, i + 1));
						break;
					case '^':
						if (cur_line[i + 1] == '=')
						{
							i++;
							token_seq_.push_back(Token("^=", TokenType::bit_xor_assign, line, i + 1));
						}
						else
						{
							token_seq_.push_back(Token("^", TokenType::bit_xor, line, i + 1));
						}
						break;
					case '{':
						token_seq_.push_back(Token("{", TokenType::lbrace, line, i + 1));
						break;
					case '}':
						token_seq_.push_back(Token("}", TokenType::rbrace, line, i + 1));
						break;
					case '(':
						token_seq_.push_back(Token("(", TokenType::lparen, line, i + 1));
						break;
					case ')':
						token_seq_.push_back(Token(")", TokenType::rparen, line, i + 1));
						break;
					case '[':
						token_seq_.push_back(Token("[", TokenType::lbparen, line, i + 1));
						break;
					case ']':
						token_seq_.push_back(Token("]", TokenType::rbparen, line, i + 1));
						break;
					case '.':
						token_seq_.push_back(Token(".", TokenType::dot, line, i + 1));
						break;
					case ',':
						token_seq_.push_back(Token(",", TokenType::comma, line, i + 1));
						break;
					case '\'':
						token_seq_.push_back(Token("\'", TokenType::apostrophe, line, i + 1));
						break;
					case ';':
						token_seq_.push_back(Token(";", TokenType::semicolon, line, i + 1));
						break;
					case ':':
						if (cur_line[i + 1] != ':')
						{
							token_seq_.push_back(Token(":", TokenType::colon, line, i + 1));
						}
						break;
					case '!':
						if (cur_line[i + 1] != '=')
						{
							token_seq_.push_back(Token("!", TokenType::exclamation, line, i + 1));
						}
						else
						{
							i++;
							token_seq_.push_back(Token("!=", TokenType::noteq, line, i + 1));
						}
						break;
					case '?':
						token_seq_.push_back(Token("?", TokenType::question, line, i + 1));
						break;
					}
				}
			}

			line++;
		}

		token_seq_.push_back(Token("", TokenType::eof, line, i));
	}
	/**
	 * @brief 렉싱 작업을 수행한 결과를 가져옵니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 렉싱 작업의 결과를 반환합니다.
	 */
	const std::vector<Token>& Lexer::get_token_seq() const noexcept
	{
		return token_seq_;
	}

	/**
	 * @brief 표준 입출력 스트림을 통해 렉싱의 결과를 시각화합니다.
	 */
	void Lexer::dump() const
	{
		dump(std::cout);
	}
	/**
	 * @brief 입출력 스트림을 통해 렉싱의 결과를 시각화합니다.
	 */
	void Lexer::dump(std::ostream& out) const
	{
		for (const Token& token : token_seq_)
		{
			out << "Line " << token.line << " Col " << token.col
				<< " -> " << token_map.at(token.type) << "(" << token.data << ")\n";
		}
	}
}
