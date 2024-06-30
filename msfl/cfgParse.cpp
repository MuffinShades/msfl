#include "pch.h"
#include "cfgParse.hpp"

//from https://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-a-string-with-another-string
void _stringReplaceAll(std::string& str, std::string replaceToken) {
	size_t pos = 0x00;

	while ((pos = str.find(replaceToken, pos)) != std::string::npos) {
		str.replace(pos, replaceToken.length(), replaceToken);
		pos += replaceToken.length();
	}
}

CfgStructure _createErrStructure(int err) {
	CfgStructure _r;
	_r._setErr(err);
	return _r;
}

CfgStructure CfgParse::ParseFile(std::string src) {
	if (src != "") {
		CfgStructure _res;
		std::fstream _file;

		_file.open(src, std::ios::in);

		CfgHeader _currentHeader;

		_currentHeader.headerLabel = "GLOBAL_HEADER";

		int cLine = 0;

		std::string _line;

		while (std::getline(_file, _line)) {
			//get current file line
			cLine++;

			//remove blank space
			_stringReplaceAll(_line, " ");
			_stringReplaceAll(_line, "\t");

			//ignore comments
			if (_line == "" || _line.at(0) == '#' || _line.at(0) == ';') continue;

			char _fToken = _line.at(0);

			std::cout << _fToken << " token" << std::endl;

			//found header
			if (_fToken == '[') {
				//switch current header
				_res.AddHeader(_currentHeader);

				_currentHeader = CfgHeader();
				
				int hEndIdx = _line.find(']',1);

				if (hEndIdx == std::string::npos) {
					return _createErrStructure(CFGERROR_INVALIDHEADER);
				}

				_currentHeader.headerLabel = _line.substr(1, hEndIdx-1);

				std::cout << "H Label: " << _currentHeader.headerLabel << std::endl;

				continue;
			}

			//no header then parse line
			else {
				int eqPos = _line.find('=');

				std::cout << "EQUAL POS: " << eqPos << std::endl;

				if (eqPos == 0) {
#ifdef PRINTCFGERR
					std::cout << "Error! Invalid = position when parsing line: " << cLine << std::endl;
#endif
					return _createErrStructure(CFGERROR_INVALIDSYNTAX_EQ);
				}

				if (eqPos == std::string::npos) { //if no value then create var with null value
					CfgToken _cToken;

					_cToken.SetLabel(_line);

					_currentHeader.AddToken(_cToken);
				}
				else { //if there is a value create a valid token
					CfgToken _cToken;

					_cToken.SetLabel(_line.substr(0, eqPos));
					_cToken.SetRawValue(_line.substr(eqPos + 1));

					_currentHeader.AddToken(_cToken);
				}
			}
		}

		_res.AddHeader(_currentHeader);

		return _res;
	}
	else {
		return _createErrStructure(CFGERROR_BLANKSRC);
	}
}

CfgHeader NullHeader() {
	CfgHeader _r;
	_r.headerLabel = "null";
	return _r;
}

void _WriteHeaderToFile(std::fstream& file, CfgHeader h) {
	//copy over header
	if (h.headerLabel != "GLOBAL_HEADER") {
		file.write("\n", 1);

		//create characters for header
		char* _hlbl = new char[h.headerLabel.length() + 2];
		*(_hlbl) = '[';
		memcpy(_hlbl + 1, h.headerLabel.c_str(), h.headerLabel.length());
		*(_hlbl + h.headerLabel.length() + 1) = ']';

		//write to file
		file.write(_hlbl, h.headerLabel.length() + 2);
		delete[] _hlbl;
	}

	//separator
	file.write("\n\n", 2);

	//now write idividual values
	for (int i = 0; i < h._tokens.size(); i++) {
		std::string _name = h._tokens[i].GetLabel();
		std::string _value = h._tokens[i].GetRawValue();

		if (_name != "") {
			if (_value != "") {
				file.write(_name.c_str(), _name.length());
				file.write("=", 1);
				file.write(_value.c_str(), _value.length());
			}
			else {
				file.write(_name.c_str(), _name.length());
			}

			file.write("\n", 1);
		}
	}
}

int CfgParse::WriteToFile(std::string src, CfgStructure data) {
	if (src != "") {
		std::fstream _file;
		_file.open(src, std::ios::out);

		CfgHeader _glbHeader = NullHeader();

		//search for global header
		for (int i = 0; i < data._headers.size(); i++) {
			if (data._headers[i].headerLabel == "GLOBAL_HEADER") {
				_glbHeader = data._headers[i];
				data._headers.erase(data._headers.begin()+i);
				break;
			}
		}

		//if there is a global header write it to our file
		if (_glbHeader.headerLabel != "null") {
			_WriteHeaderToFile(_file, _glbHeader);
		}

		//now write all the other headers
		for (int i = 0; i < data._headers.size(); i++) {
			_WriteHeaderToFile(_file, data._headers[i]);
		}
	}

	return 0x01;
}

void CfgToken::SetRawValue(std::string val) {
	this->_rawValue = val;
}

void CfgToken::SetLabel(std::string label) {
	this->_tokenLabel = label;
}

std::string  CfgToken::GetLabel() {
	return this->_tokenLabel;
}

void CfgHeader::AddToken(CfgToken token) {
	this->_tokens.push_back(token);
}

void CfgStructure::AddHeader(CfgHeader h) {
	this->_headers.push_back(h);
}

std::string CfgToken::GetRawValue() {
	return this->_rawValue;
}

int CfgHeader::GetStatus() {
	return this->returnStatus;
}

bool strEq(const char* str1, const char* str2, int tLen) {
	if (strlen(str1) < tLen) tLen = strlen(str1);
	if (strlen(str2) < tLen) tLen = strlen(str2);

	for (int i = 0; i < tLen; i++) {
		if (str1[i] != str2[i]) return false;
	}

	return true;
}

CfgToken CfgHeader::ExtractToken(std::string name) {
	for (int i = 0; i < this->_tokens.size(); i++) {
		if (strEq(this->_tokens[i].GetLabel().c_str(), name.c_str(), name.length())) {
			return this->_tokens[i];
		}
	}

	CfgToken _t;
	_t.SetRawValue("null");
	return _t;
}

CfgHeader CfgParse::CreateGlobalHeader() {
	CfgHeader _res;
	_res.headerLabel = "GLOBAL_HEADER";
	return _res;
}

CfgHeader CfgParse::ExtractHeader(std::string src, std::string headerName) {
	if (src != "") {
		CfgHeader _rHeader = NullHeader();
		
		std::fstream _file;

		_file.open(src, std::ios::in);

		int cLine = 0;

		while (!_file.eof()) {
			//get current file line
			cLine++;
			std::string _line;
			std::getline(_file, _line);

			//ignore comments
			if (_line == "" || _line.at(0) == '#' || _line.at(0) == ';') continue;

			//remove blank space
			_stringReplaceAll(_line, " ");
			_stringReplaceAll(_line, "\t");

			char _fToken = _line.at(0);

			std::string _labl = "";

			//found header
			if (_fToken == '[') {
				//switch current header
				int hEndIdx = _line.find(']', 1);

				if (hEndIdx == std::string::npos) {
					return NullHeader();
				}

				std::string _labl = _line.substr(1, hEndIdx - 1);

				if (_labl == headerName) {
					_rHeader.headerLabel = _labl;
				}

				continue;
			}

			//no header then parse line
			else if (headerName == "GLOBAL_HEADER" && _rHeader.headerLabel == "null") {
				_rHeader = CfgParse::CreateGlobalHeader();
			}
			else if (_rHeader.headerLabel != "null") {
				int eqPos = _line.find('=');

				if (eqPos == 0) {
#ifdef PRINTCFGERR
					std::cout << "Error! Invalid = position when parsing line: " << cLine << std::endl;
#endif
					return NullHeader();
				}

				if (eqPos == std::string::npos) { //if no value then create var with null value
					CfgToken _cToken;

					_cToken.SetLabel(_line);

					_rHeader.AddToken(_cToken);
				}
				else { //if there is a value create a valid token
					CfgToken _cToken;

					_cToken.SetLabel(_line.substr(0, eqPos));
					_cToken.SetRawValue(_line.substr(eqPos + 1, _line.length()));

					_rHeader.AddToken(_cToken);
				}
			}
		}

		return _rHeader;
	}
	else {
		return NullHeader();
	}
}

#include <typeinfo>

template<typename _T> _T _stringToT(std::string value) {
	std::string _tName = typeid(_T).name();

	if (_tName == "int") {
		try {
			return std::stoi(value);
		}
		catch (std::exception e) {
			return NULL;
		}
	}

	if (_tName == "float") {
		try {
			return std::stof(value);
		}
		catch (std::exception e) {
			return NULL;
		}
	}

	if (_tName == "long") {
		try {
			return std::stol(value);
		}
		catch (std::exception e) {
			return NULL;
		}
	}

	if (_tName == "double") {
		try {
			return std::stod(value);
		}
		catch (std::exception e) {
			return NULL;
		}
	}

	if (_tName == "short") {
		try {
			return (short)std::stoi(value);
		}
		catch (std::exception e) {
			return NULL;
		}
	}

	if (_tName == "char") {
		return (char)value[0];
	}

	return NULL;
}

/*template<typename _T> _T CfgToken::GetValueAs() {
	if (this->_rawValue != "") {
		return _stringToT<_T>(this->_rawValue);
	}
	else {
		return NULL;
	}
}

template char CfgToken::GetValueAs<char>();
template short CfgToken::GetValueAs<short>();
template int CfgToken::GetValueAs<int>();
template long CfgToken::GetValueAs<long>();
template float CfgToken::GetValueAs<float>();
template double CfgToken::GetValueAs<double>();*/

std::vector<std::string> SplitString(std::string str, char delim) {
	int splitIdx = 0;

	std::vector<std::string> _tkns;

	while (splitIdx != std::string::npos) {
		splitIdx = str.find(delim);

		if (splitIdx != std::string::npos) {
			_tkns.push_back(str.substr(0,splitIdx));
			str = str.substr(splitIdx+1);
		}
	}

	_tkns.push_back(str);

	return _tkns;
}

CfgHeader CfgStructure::ExtractHeader(std::string label) {
	if (label != "") {
		for (int i = 0; i < this->_headers.size(); i++) {
			if (this->_headers[i].headerLabel == label) {
				return this->_headers[i];
			}
		}
	}

	CfgHeader nullHeader;
	return nullHeader;
}

/*template<typename _T> std::vector<_T> CfgToken::GetValueVectorAs() {
	if (this->_rawValue != "") {
		int len = 0;

		std::vector<std::string> _rawVals = SplitString(this->_rawValue, ',');
		std::vector<_T> rv = std::vector<_T>(_rawVals.size());

		for (size_t i = 0; i < rv.size(); i++) {
			rv[i] = _stringToT<_T>(_rawVals[i]);
		}

		return rv;
	}
	else {
		std::vector<_T> _nullRes(0);
		return _nullRes;
	}
}

template std::vector<char> CfgToken::GetValueVectorAs<char>();
template std::vector<short> CfgToken::GetValueVectorAs<short>();
template std::vector<int> CfgToken::GetValueVectorAs<int>();
template std::vector<long> CfgToken::GetValueVectorAs<long>();
template std::vector<float> CfgToken::GetValueVectorAs<float>();
template std::vector<double> CfgToken::GetValueVectorAs<double>();*/


CfgStructure CfgParse::ParseStr(const char* dat, int len) {
	CfgStructure _res;

	CfgHeader _currentHeader;

	_currentHeader.headerLabel = "GLOBAL_HEADER";

	int cLine = 0;
	std::vector<std::string> lines = SplitString(std::string(dat), '\n');

	std::string _line;

	while (cLine < lines.size()) {
		_line = lines[cLine];
		//get current file line
		cLine++;

		//remove blank space
		_stringReplaceAll(_line, " ");
		_stringReplaceAll(_line, "\t");

		//ignore comments
		if (_line == "" || _line.at(0) == '#' || _line.at(0) == ';') continue;

		char _fToken = _line.at(0);

		//found header
		if (_fToken == '[') {
			//switch current header
			_res.AddHeader(_currentHeader);

			_currentHeader = CfgHeader();

			int hEndIdx = _line.find(']', 1);

			if (hEndIdx == std::string::npos) {
				return _createErrStructure(CFGERROR_INVALIDHEADER);
			}

			_currentHeader.headerLabel = _line.substr(1, hEndIdx - 1);

			continue;
		}

		//no header then parse line
		else {
			int eqPos = _line.find('=');

			if (eqPos == 0) {
#ifdef PRINTCFGERR
				std::cout << "Error! Invalid = position when parsing line: " << cLine << std::endl;
#endif
				return _createErrStructure(CFGERROR_INVALIDSYNTAX_EQ);
			}

			if (eqPos == std::string::npos) { //if no value then create var with null value
				CfgToken _cToken;

				_cToken.SetLabel(_line);

				_currentHeader.AddToken(_cToken);
			}
			else { //if there is a value create a valid token
				CfgToken _cToken;

				_cToken.SetLabel(_line.substr(0, eqPos));
				_cToken.SetRawValue(_line.substr(eqPos + 1));

				_currentHeader.AddToken(_cToken);
			}
		}
	}

	_res.AddHeader(_currentHeader);

	return _res;
}