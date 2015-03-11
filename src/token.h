#pragma once

#include <vector>
#include <string>
#include <iostream>

struct pos {
	int line;
	int col;
	
	pos();
	pos(int _line, int _col);

	bool operator!=(pos rpos) const;
};

enum lex_type { LX_KEYWORD, LX_IDENTIFIER, LX_PUNCTUATOR, LX_INTEGER, LX_REAL, LX_CHAR, LX_STRING, LX_ERROR, LX_END };

enum un_type { UN_STR, UN_CHAR, UN_INT, UN_DOUBLE };
class val_type {
private:
	union curr_type {
		char __char;
		char* __str;
		int __int;
		double __double;
	};
	un_type type;
	curr_type value;

public:
	val_type();
	val_type(const val_type& rval_type);
	val_type(int _int);
	val_type(double _double);
	val_type(char _char);
	val_type(std::string _str);
	
	friend std::ostream& operator<<(std::ostream& os, const val_type& rval_type);
};

class token {
private:
	pos position;
	lex_type type;	
	std::string text;
	val_type value;

public:
	token();
	token(const token& rtoken);
	token(pos _position, lex_type _type, std::string _text, val_type _value);

	pos& get_pos();
	std::string& get_text();

	bool is_end() const;

	friend std::ostream& operator<<(std::ostream& os, const token& rtoken);
};