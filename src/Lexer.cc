/**
 * @file Lexer.cc
 * @date 2017.08.02
 * @author dev_kr
 * @brief 입력을 Token 단위로 분석하고 나누어 반환하는 렉서를 정의합니다.
 */

#include "Lexer.hh"

namespace Dlink
{
    Lexer::Lexer()
    {
        keyword_map_["if"] = TokenType::_if;
		keyword_map_["else"] = TokenType::_else;
		keyword_map_["switch"] = TokenType::_switch;
		keyword_map_["for"] = TokenType::_for;
		keyword_map_["while"] = TokenType::_while;
		keyword_map_["do"] = TokenType::_do;
		keyword_map_["break"] = TokenType::_break;
		keyword_map_["continue"] = TokenType::_continue;
		keyword_map_["return"] = TokenType::_return;
		keyword_map_["const"] = TokenType::_const;
    }

    void Lexer::lex(const std::string& str)
	{
		std::size_t line = 1, i = 0, col_temp = 0;

        auto get_col = [&]() -> std::size_t
        {
            return i - col_temp;
        };

		for (;i < str.length(); i++)
		{
			char ch = str[i];

			if (std::isalpha(ch))
			{
				std::string temp = "";
				do
				{
					temp += str[i];
					i++;
				} while (std::isalpha(str[i]) || std::isdigit(str[i]) || str[i] == '_');

				i--;

				bool is_keyword = (keyword_map_.find(temp) != keyword_map_.end());

				if (is_keyword)
				{
					Token keyword_token(temp, keyword_map_[temp], line, get_col());
					token_seq_.push_back(keyword_token);
				}
				else
				{
					Token identifier_token(temp, TokenType::identifier, line, get_col());
					token_seq_.push_back(identifier_token);
				}
			}
			else if (std::isdigit(ch))
			{
				std::string temp = "";

				if (ch == '0' && i < str.length() - 2) 
				{
					char next_ch = str[i + 1];
					switch (next_ch)
					{
					case 'b':
					case 'B': 
						temp += '0';
						temp += next_ch;
						i += 2;

						do
						{
							temp += str[i];
							i++;
						} while (str[i] == '0' || str[i] == '1');
						i--;

						token_seq_.push_back(Token(temp, TokenType::bin_integer, line, get_col()));
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
							temp += str[i];
							i++;
						} while (('0' <= str[i]) && (str[i] <= '7'));
						i--;

						token_seq_.push_back(Token(temp, TokenType::oct_integer, line, get_col()));
						break;

					case 'x':
					case 'X': 
						temp += '0';
						temp += next_ch;
						i += 2;

						do
						{
							temp += str[i];
							i++;
						} while (i < str.length() && (std::isdigit(str[i]) ||
							(('a' <= str[i]) && (str[i] <= 'f')) ||
													  (('A' <= str[i]) && (str[i] <= 'F'))));
						i--;

						token_seq_.push_back(Token(temp, TokenType::hex_integer, line, get_col()));
						break;

					default:
						token_seq_.push_back(Token("0", TokenType::dec_integer, line, get_col()));
						break;
					}
				}
				else
				{
					do
					{
						temp += str[i];
						i++;
					} while (i < str.length() && std::isdigit(str[i]));

					if (str[i] == '.') 
					{
						do
						{
							temp += str[i];
							i++;
						} while (i < str.length() && std::isdigit(str[i]));

						if (str[i] == 'e' && (str[i + 1] == '-' || str[i + 1] == '+')) 
						{
							temp += 'e';
							temp += str[i + 1];
							i += 2;
							do
							{
								temp += str[i];
								i++;
							} while (i < str.length() && std::isdigit(str[i]));
						}

						token_seq_.push_back(Token(temp, TokenType::floating, line, get_col()));
					}
					else 
					{
						token_seq_.push_back(Token(temp, TokenType::dec_integer, line, get_col()));
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
					if (str[i] == '\\')
					{
						if (i < str.length() - 2 && str[i + 1] == '\"')
						{
							temp += '\"';
							i += 2;
							continue;
						}
					}

					temp += str[i];
					i++;
				} while (str[i] != '\"');

				token_seq_.push_back(Token(temp, TokenType::string, line, get_col()));
			}
			else 
			{
				switch (ch)
				{
				case '+':
					if (str[i + 1] == '+') 
					{
						i++;
						token_seq_.push_back(Token("++", TokenType::increment, line, get_col()));
					}
					else if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token("+=", TokenType::plus_assign, line, get_col()));
					}
					else 
					{
						token_seq_.push_back(Token("+", TokenType::plus, line, get_col()));
					}
					break;
				case '-':
					if (str[i + 1] == '-') 
					{
						i++;
						token_seq_.push_back(Token("--", TokenType::decrement, line, get_col()));
					}
					else if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token("-=", TokenType::minus_assign, line, get_col()));
					}
					else 
					{
						token_seq_.push_back(Token("-", TokenType::minus, line, get_col()));
					}
					break;
				case '*':
					if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token("*=", TokenType::multiply_assign, line, get_col()));
					}
					else 
					{
						token_seq_.push_back(Token("*", TokenType::multiply, line, get_col()));
					}
					break;
				case '/':
					if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token("/=", TokenType::divide_assign, line, get_col()));
					}
					else 
					{
						token_seq_.push_back(Token("/", TokenType::divide, line, get_col()));
					}
					break;
				case '%':
					if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token("%=", TokenType::modulo_assign, line, get_col()));
					}
					else 
					{
						token_seq_.push_back(Token("%", TokenType::modulo, line, get_col()));
					}
					break;
				case '=':
					if (str[i + 1] != '=') 
					{
						token_seq_.push_back(Token("=", TokenType::equal, line, get_col()));
					}
					else 
					{
						i++;
						token_seq_.push_back(Token("==", TokenType::equal, line, get_col()));
					}
					break;
				case '>':
					if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token(">=", TokenType::eqless, line, get_col()));
					}
					else if (str[i + 1] == '>') 
					{
						i++;
						if (str[i + 2] == '=') 
						{
							i++;
							token_seq_.push_back(Token(">>=", TokenType::bit_lshift_assign, line, get_col()));
						}
						else 
						{
							token_seq_.push_back(Token(">>", TokenType::bit_lshift, line, get_col()));
						}
					}
					else 
					{
						token_seq_.push_back(Token(">", TokenType::less, line, get_col()));
					}
					break;
				case '<':
					if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token("<=", TokenType::eqless, line, get_col()));
					}
					else if (str[i + 1] == '<') 
					{
						i++;
						if (str[i + 2] == '=') 
						{
							i++;
							token_seq_.push_back(Token("<<=", TokenType::bit_rshift_assign, line, get_col()));
						}
						else 
						{
							token_seq_.push_back(Token("<<", TokenType::bit_rshift, line, get_col()));
						}
					}
					else 
					{
						token_seq_.push_back(Token("<", TokenType::less, line, get_col()));
					}
					break;
				case '&':
					if (str[i + 1] == '&') 
					{
						i++;
						token_seq_.push_back(Token("&&", TokenType::logic_and, line, get_col()));
					}
					else if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token("&=", TokenType::bit_and_assign, line, get_col()));
					}
					else 
					{
						token_seq_.push_back(Token("&", TokenType::bit_and, line, get_col()));
					}
					break;
				case '|':
					if (str[i + 1] == '|') 
					{
						i++;
						token_seq_.push_back(Token("||", TokenType::logic_or, line, get_col()));
					}
					else if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token("|=", TokenType::bit_or_assign, line, get_col()));
					}
					else 
					{
						token_seq_.push_back(Token("|", TokenType::bit_or, line, get_col()));
					}
					break;
				case '~':
					token_seq_.push_back(Token("~", TokenType::bit_not, line, get_col()));
					break;
				case '^':
					if (str[i + 1] == '=') 
					{
						i++;
						token_seq_.push_back(Token("^=", TokenType::bit_xor_assign, line, get_col()));
					}
					else 
					{
						token_seq_.push_back(Token("^", TokenType::bit_xor, line, get_col()));
					}
					break;
				case '{':
					token_seq_.push_back(Token("{", TokenType::lbrace, line, get_col()));
					break;
				case '}':
					token_seq_.push_back(Token("}", TokenType::rbrace, line, get_col()));
					break;
				case '(':
					token_seq_.push_back(Token("(", TokenType::lparen, line, get_col()));
					break;
				case ')':
					token_seq_.push_back(Token(")", TokenType::rparen, line, get_col()));
					break;
				case '[':
					token_seq_.push_back(Token("[", TokenType::lbparen, line, get_col()));
					break;
				case ']':
					token_seq_.push_back(Token("]", TokenType::rbparen, line, get_col()));
					break;
				case '.':
					token_seq_.push_back(Token(".", TokenType::dot, line, get_col()));
					break;
				case ',':
					token_seq_.push_back(Token(",", TokenType::comma, line, get_col()));
					break;
				case '\'':
					token_seq_.push_back(Token("\'", TokenType::apostrophe, line, get_col()));
					break;
				case ';':
					token_seq_.push_back(Token(";", TokenType::semicolon, line, get_col()));
					break;
				case ':':
					if (str[i + 1] != ':') 
					{
						token_seq_.push_back(Token(":", TokenType::colon, line, get_col()));
					}
					break;
				case '!': 
					if (str[i + 1] != '=') 
					{
						token_seq_.push_back(Token("!", TokenType::exclamation, line, get_col()));
					}
					else 
					{
						i++;
						token_seq_.push_back(Token("!=", TokenType::noteq, line, get_col()));
					}
					break;
				case '?':
					token_seq_.push_back(Token("?", TokenType::question, line, get_col()));
					break;
				case '\n':
					line++;
                    col_temp = i - 1;
					break;
				}
			}
		}
	}

    const std::vector<Token>& Lexer::get_token_seq() const
	{
		return token_seq_;
	}

}
