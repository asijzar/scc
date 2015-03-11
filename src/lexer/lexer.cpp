#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include "lexer.h"
#include "machine.h"
#include "lexer_error.h"

#define ENDING_STATES_OFFSET 30

using namespace std;

std::set<std::string> keywords = { "break", "char", "const", "continue", "double", "else", "for", "if", "int", "return", "sizeof", "struct", 
	"typedef", "void", "while" 
};

map<states, string> punc_types = { {ST_NOT, "not"}, {ST_NOT_EQ, "not_eq"}, {ST_MOD, "mod"}, {ST_MOD_EQ, "mod_eq"}, {ST_AND_BIN, "and_bin"}, {ST_AND, "and"}, 
	{ST_AND_BIN_EQ, "and_bin_eq"}, {ST_BRACE_L, "left_brace"}, {ST_BRACE_R, "right_brace"}, {ST_ASTERISK, "asterisk"}, {ST_ASTERISK_EQ, "asterisk_eq"}, {ST_PLUS, "plus"},
	{ST_INCREMENT, "increment"}, {ST_PLUS_EQ, "plus_eq"}, {ST_COMMA, "comma"}, {ST_MINUS, "minus"}, {ST_FIELD_PTR, "field_ptr"}, {ST_DECREMENT, "decrement"}, 
	{ST_MINUS_EQ, "minus_eq"}, {ST_FIELD, "field"}, {ST_DIV, "div"}, {ST_DIV_EQ, "div_eq"}, {ST_TERNARY_C, "ternary_c"}, {ST_SEMICOLON, "semicolon"}, 
	{ST_LESS, "less"}, {ST_SHIFT_L, "left_shift"}, {ST_LESS_EQ, "less_eq"}, {ST_SHIFT_L_EQ, "left_shift_eq"}, {ST_ASSIGN, "assign"}, {ST_EQ, "equal"}, {ST_GREATER, "greater"}, 
	{ST_SHIFT_R, "right_shift"}, {ST_GREATER_EQ, "greater_eq"}, {ST_SHIFT_R_EQ, "right_shift_eq"}, {ST_TERNARY_Q, "ternary_q"}, {ST_SQUAREBRACE_L, "squarebrace_left"}, 
	{ST_SQUAREBRACE_R, "right_squarebrace"}, {ST_XOR, "xor"}, {ST_XOR_EQ, "xor_eq"}, {ST_CURLYBRACE_L, "left_curlybrace"}, {ST_OR_BIN, "or_bin"}, {ST_OR, "or"}, 
	{ST_OR_BIN_EQ, "or_bin_eq"}, {ST_CURLYBRACE_R, "right_curlybrace"}, {ST_NOT_BIN, "not_bin"}
};

lexer::lexer(string file_name): curr_pos(1, 1), fi(file_name, ios::in), fo(cout) {}
lexer::lexer(string in_file_name, string out_file_name): curr_pos(1, 1), fi(in_file_name, ios::in), fo(*new ofstream(out_file_name, ios::out)) {}

token lexer::get_last_token() const {
	return last_token;
}

bool lexer::is_ending_state(unsigned int state) const {
	return state < machine.size() ? 0 : 1;
}

token  lexer::make_token(const pos& start_pos, unsigned int state, string& token_text) {
	switch (state) {
		case (ST_IDENTIFIER):
			if (keywords.find(token_text) != keywords.end())
				return token(start_pos, LX_KEYWORD, token_text, token_text);
			return token(start_pos, LX_IDENTIFIER, token_text, token_text);
		case (ST_OCTAL): return token(start_pos, LX_INTEGER, token_text, stoi(token_text, 0, 8));
		case (ST_HEX): return token(start_pos, LX_INTEGER, token_text, stoi(token_text, 0, 16));
		case (ST_DECIMAL): return token(start_pos, LX_INTEGER, token_text, stod(token_text));
		case (ST_FLOAT): return token(start_pos, LX_REAL, token_text, stod(token_text));
		case (ST_STRING): return scan_string(start_pos, token_text);
		case (ST_CHAR): return scan_char(start_pos, token_text);
	}
	return token(start_pos, LX_PUNCTUATOR, token_text, punc_types[(states)state]);
}

char lexer::scan_escape(string& token_text) {
	curr_pos.col++;
	char esc_char = fi.get();
	token_text += esc_char;
	switch (esc_char) {
		case ('\''): return '\'';
		case ('"'): return '\"';
		case ('?'): return '\?';
		case ('\\'): return '\\';
		case ('0'): return '\0';
		case ('a'): return '\a';
		case ('b'): return '\b';
		case ('f'): return '\f';
		case ('n'): return '\n';
		case ('r'): return '\r';
		case ('t'): return '\t';
		case ('v'): return '\v';
	}
	return '\\';
}

token lexer::scan_char(const pos& start_pos, string& token_text) {
	char curr_char = fi.get(); 
	token_text += curr_char;
	if (curr_char == '\\') curr_char = scan_escape(token_text);
	token_text += fi.get(); //read closing ' char
	curr_pos.col += 2;
	
	return token(start_pos, LX_CHAR, token_text, curr_char);
}

token lexer::scan_string(const pos& start_pos, string& token_text) {
	char curr_char;
	string curr_str;

	while (fi.get(curr_char)) {
		token_text += curr_char;
		curr_pos.col++;
		if (curr_char == '\\') curr_char = scan_escape(token_text);
		if (curr_char == '"') break;
		curr_str += curr_char;
	}

	return token(start_pos, LX_STRING, token_text, curr_str);
}

bool lexer::pass_comment(unsigned int state) {
	char curr_char;

	if (state - 30 == ST_ONE_LINE_COMMENT) {
		while (fi.get(curr_char)) {
			curr_pos.col++;
			if (curr_char == '\n') { 
				curr_pos.line++;
				curr_pos.col = 1;
				return 1;
			}
		}
		return 1;
	}

	if (state - 30 == ST_MULTI_LINE_COMMENT) {
		int end_comm_flag = 0;
		while (fi.get(curr_char)) {
			curr_pos.col++;
			if (curr_char == '\n') {
				curr_pos.line++;
				curr_pos.col = 1;
			}
			if (curr_char == '*') {
				end_comm_flag = 1;
				continue;
			}
			if (curr_char == '/' && end_comm_flag) {
				curr_pos.col++;
				return 1;
			}
			end_comm_flag = 0;
		}
		return 1;
	}
	return 0;
}

token lexer::get() {
	token curr_token;

	unsigned int curr_state = 0;
	char curr_char;
	string token_text;
	bool is_start = 0;
	pos start_pos = curr_pos;
	int prev_state = 0;
	int was_error_state = 0;
	bool read_last = 0;
	bool stop_read = 0;

	while (1) {
		if (is_ending_state(curr_state))
			if (curr_state - ENDING_STATES_OFFSET > ST_ERROR_TOKEN) {
				if (!was_error_state) {
					read_last = 1;
					was_error_state = curr_state;
				}
				curr_state = prev_state;
			} else break;

		if (fi.get(curr_char)) {
			prev_state = curr_state;
			curr_state = machine[curr_state][curr_char];
			
			if (pass_comment(curr_state)) { 
				curr_state = 0;
				is_start = 0;
				token_text.clear();
				continue;
			}

			if (curr_state != 0) {
				if (!is_start) { 
					start_pos = curr_pos;
					is_start = 1;
				}
				if (!stop_read) token_text += curr_char;
				if (read_last) stop_read = 1;
			}
			
			if (!curr_state && curr_char == '\n') {
				curr_pos.col = 1;
				curr_pos.line++;
			} else curr_pos.col += curr_char == '\t' ? 4 : 1;
		} else break;
	}
	
	if (was_error_state) curr_state = was_error_state;

	bool was_ending_state = is_ending_state(curr_state);
	if (!was_ending_state) curr_state = machine[curr_state]['\0']; //if curr_char is last symbol in file
	if (curr_state == 0) return token(pos(-1, -1), LX_END, "", val_type());

	if (unget_states.find(curr_state) != unget_states.end() && was_ending_state) {
		fi.unget();
		curr_pos.col--;
		token_text.pop_back();
	}

	curr_state -= ENDING_STATES_OFFSET;

	try {
		if (curr_state >= ST_ERROR_TOKEN) throw lexer_error(curr_state, start_pos, token_text);
		curr_token = make_token(start_pos, curr_state, token_text);
	} catch(lexer_error lex_err) {
		fo << lex_err;
		curr_token = token(start_pos, LX_ERROR, token_text, 0);
	}

	last_token = curr_token;
	return curr_token;
}

bool lexer::next(token& _token) {
	_token = get();
	if (!_token.is_end()) {
		fo << _token;	
		return 1;
	}
	return 0;
}