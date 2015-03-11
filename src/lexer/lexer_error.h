#pragma once

#include "../token.h"

enum lex_err_type { LET_TOKEN, LET_OCT, LET_INT, LET_EXP };
class lexer_error {
private:
	token error_token;
	lex_err_type err_type;

public:
	lexer_error();
	lexer_error(unsigned int _err_type, pos _position, std::string _text);

	friend std::ostream& operator<<(std::ostream& os, lexer_error& rlexer_error);
};