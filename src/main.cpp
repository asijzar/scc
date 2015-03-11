#include "flags.h"
#include "lexer/lexer.h"

using namespace std;

int main(int argc, char** argv) {
	flags args(argc, argv);
	
	if (args.check_flag("-l"))
		for (unsigned int i = 0; i < args.file_names().size(); i++) {
			lexer lex(args.file_names()[i], "../lexer/lex.out");
			token cur_token;
			while (lex.next(cur_token));
		}

	system("pause");
	return 0; 
}