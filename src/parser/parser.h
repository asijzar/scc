#include "../lexer/lexer.h"

class parser {
private:
	expr_node* parse_tree;

public:
	parser();

	expr_node* parse_expr();
};

class expr_node {
private:
	term_node left;
	op_node op;

public:
	expr_node();

	expr_node* parse_term();
};

class term_node {
private:
	


};