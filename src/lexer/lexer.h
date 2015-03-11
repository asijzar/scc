#pragma once

#include <fstream>
#include "../token.h"

class lexer {
private:
	std::ifstream fi;
	std::ostream& fo;
	pos curr_pos;
	token last_token;

	bool is_ending_state(unsigned int state) const;

	char scan_escape(std::string& token_text);
	token scan_char(const pos& start_pos, std::string& token_text);
	token scan_string(const pos& start_pos, std::string& token_text);

	bool pass_comment(unsigned int state);

	token make_token(const pos& cur_pos, unsigned int state, std::string& token_text);

public:
	lexer(std::string file_name);
	lexer(std::string in_file_name, std::string out_file_name);

	token get_last_token() const;

	token get();
	bool next(token& _token);
};
