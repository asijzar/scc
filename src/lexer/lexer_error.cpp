#include <iostream>
#include "lexer_error.h"

#define ERROR_OFFSET 54

using namespace std;

lexer_error::lexer_error() : error_token() {}

lexer_error::lexer_error(unsigned int curr_state, pos _position, std::string _text) : error_token(_position, LX_ERROR, _text, 0) {
	err_type = lex_err_type(curr_state - ERROR_OFFSET);
}

ostream& operator<<(ostream& os, lexer_error& rlexer_error) {
	os << rlexer_error.error_token.get_pos().line << ":" << rlexer_error.error_token.get_pos().col << ":" << " error:";
	switch (rlexer_error.err_type) {
		case (LET_TOKEN):
			os << " unrecognized token '";
			break;
		case (LET_OCT):
			os << " invalid digit in octal constant '";
			break;
		case (LET_INT):
			os << " invalid suffix on integer constant '";
			break;
		case (LET_EXP):
			os << " exponent has no digits '";
	}

	os << rlexer_error.error_token.get_text() << "'" << endl;
	return os;
}