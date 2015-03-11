#pragma once

#include <map>
#include <string>
#include <vector>

class flags {
private:
	std::map<std::string, bool> flag_list;
	std::vector<std::string> files;

	void help();
	void about();
	void no_file();

public:
	flags(int argc, char** argv);

	bool check_flag(const std::string& key);
	std::vector<std::string>& file_names();
};