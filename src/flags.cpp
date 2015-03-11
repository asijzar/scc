#include <iostream>
#include "flags.h"

using namespace std;

std::vector<std::string> flag_str = { "-h", "-about", "-l" };

flags::flags(int argc, char** argv) {
	if (argc == 1) help();
	for (int i = 0; i < flag_str.size(); i++)
		flag_list.insert(pair<string, bool>(flag_str[i], 0));

	for (int i = 1; i < argc; i++) {
		flag_list[argv[i]] = 1;	
		if (strcmp(argv[i], "-h") == 0) help();
		if (strcmp(argv[i], "-about") == 0) about();
		if (strcmp(argv[i], "-l") == 0) 
			if (i + 1 < argc) files.push_back(argv[++i]);
			else no_file();
	}
}

void flags::help() {
	cout << "Command line arguments:" << endl;
	cout << "-h - help window" << endl;
	cout << "-l - lexical analyzer" << endl;
	cout << "-about - about program" << endl;
}

void flags::about() {
	cout << "C Compiler" << endl;
	cout << "Kostin Dmitry, 2014" << endl;
}

void flags::no_file() {
	cout << "File name not found" << endl;
}

bool flags::check_flag(const string& key) {
	return flag_list[key];
}

std::vector<std::string>& flags::file_names() {
	return files;
}