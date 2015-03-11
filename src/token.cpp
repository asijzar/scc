#define _CRT_SECURE_NO_WARNINGS

#include "token.h"

using namespace std;

vector<string> lex_types_str = { "keyword", "identifier", "punctuator", "integer", "real", "char", "string" };

pos::pos() : line(0), col(0) {}
pos::pos(int _line, int _col) : line(_line), col(_col) {}

bool pos::operator!=(pos rpos) const {
	return line != rpos.line || col != rpos.col;
}

val_type::val_type() : type(UN_INT) {
	value.__int = 0;
}

val_type::val_type(const val_type& rval_type) : type(rval_type.type) {
	switch (rval_type.type) {
		case UN_INT: value.__int = rval_type.value.__int; break;
		case UN_DOUBLE: value.__double = rval_type.value.__double; break;
		case UN_STR: 
			value.__str = (char*)malloc(sizeof(char) * strlen(rval_type.value.__str));
			strcpy(value.__str, rval_type.value.__str);
			break;
		case UN_CHAR: value.__char = rval_type.value.__char; break;
	}	
}

val_type::val_type(int _int) : type(UN_INT) {
	value.__int = _int;
}

val_type::val_type(double _double) : type(UN_DOUBLE) {
	value.__double = _double;
}

val_type::val_type(char _char) : type(UN_CHAR) {
	value.__char = _char;
}

val_type::val_type(string _str) : type(UN_STR) {
	int str_size = sizeof(char) * _str.length();
	value.__str = (char*)malloc(str_size);
	strcpy(value.__str, _str.c_str());
}

ostream& operator<<(std::ostream& os, const val_type& rval_type) {
	switch (rval_type.type) {
		case UN_INT: os << rval_type.value.__int; break;
		case UN_DOUBLE: os << rval_type.value.__double; break;
		case UN_CHAR: os << rval_type.value.__char; break;
		case UN_STR: os << rval_type.value.__str; break;
	}	
	return os;
}

token::token() : position(0, 0), type(), text(), value() {}
token::token(const token& rtoken) : position(rtoken.position), type(rtoken.type), text(rtoken.text), value(rtoken.value) {}
token::token(pos _position, lex_type _type, std::string _text, val_type _value) : position(_position), type(_type), text(_text), value(_value) {}

pos& token::get_pos() {
	return position;
}

std::string& token::get_text() {
	return text;
}

bool token::is_end() const {
	return type == LX_END;
}

ostream& operator<<(ostream& os, const token& rtoken) {
	if (rtoken.type != LX_ERROR)
		os << rtoken.position.line << '\t' << rtoken.position.col
		<< '\t' << lex_types_str[rtoken.type] << '\t' << rtoken.text << '\t' << rtoken.value << endl;

	return os;
}