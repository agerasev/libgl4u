#pragma once

#include <string>

#include <gl/exception.hpp>

namespace gl {
class FileReader {
private:
	FILE *file = nullptr;
	char *data = nullptr;
	long size = 0;
	
public:
	FileReader(const std::string &filename) throw(FileNotFoundException);
	~FileReader();
	
	char *getData();
	long getSize() const;
};
}
