#include "pch.h"
#include "FileWriter.h"
#include "balloon.hpp"

arr_container<byte> FileWriter::ReadBytesFromBinFile(std::string path) {
	arr_container<byte> res;

	if (path != "") {
		std::ifstream _f(path, std::ios::binary);

		if (_f.good() && _f) {
			_f.seekg(0, std::ios::end);

			long len = _f.tellg();
			res.sz = len;

			_f.seekg(0, std::ios::beg);

			res.dat = new unsigned char[len];
			_f.read((char*)res.dat, len);
		}

		_f.close();
	}

	return res;
}

bool FileWriter::WriteToBinFile(std::string path, unsigned char* bytes, int len) {
	if (path != "") {
		std::ofstream _f(path, std::ios::binary);
		_f.write((char*) bytes, len);

		if (!_f.good()) {
			return false;
		}

		_f.close();
	}
	else {
		return false;
	}

	return true;
}

arr_container<byte> FileWriter::ReadBytesFromBinFileLPCWSTR(LPCWSTR path) {
	arr_container<byte> res;

	if (path != L"") {
		std::ifstream _f(path, std::ios::binary);

		if (_f.good() && _f) {
			_f.seekg(0, std::ios::end);

			long len = _f.tellg();
			res.sz = len;

			_f.seekg(0, std::ios::beg);

			res.dat = new unsigned char[len];
			_f.read((char*)res.dat, len);
		}

		_f.close();
	}

	return res;
}

bool FileWriter::WriteToBinFileLPCWSTR(LPCWSTR path, unsigned char* bytes, int len) {
	if (path != L"") {
		std::ofstream _f(path, std::ios::binary);
		_f.write((char*)bytes, len);

		if (!_f.good()) {
			return false;
		}

		_f.close();
	}
	else {
		return false;
	}

	return true;
}