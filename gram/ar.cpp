expr_node* parse_expr() {
	auto left = parse_term();
	auto op = scan -> get();
	if (op == tok.plus || op == tok.minus) {
		scan -> next();
		
		
expr_node* parse_factor() {
	auto tok = scan -> get();
	
	if (tok == tok.const()) {
		return new const_node(tok -> int_value);
		
		
