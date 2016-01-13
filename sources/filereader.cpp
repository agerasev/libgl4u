#include <gl/filereader.hpp>

#include <cstdio>

using namespace gl;

FileReader::FileReader(const std::string &filename) throw(FileNotFoundException) {
	file = fopen(filename.c_str(), "r");
	if(file == nullptr)
		throw FileNotFoundException(filename);
	
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	data = new char[size + 1];
	long chars_read = fread(data, 1, size, file);
	data[chars_read] = '\0';
}

FileReader::~FileReader() {
	if(data != nullptr)
		delete[] data;
	if(file != nullptr)
		fclose(file);
}

char *FileReader::getData() {
	return data;
}

long FileReader::getSize() const {
	return size;
}
