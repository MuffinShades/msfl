/**
 *
 * Simple .json file parser
 * 
 * Programmed by muffinshades
 * 
 * Copyright muffinshades 2024 all rights reserved
 * 
 */

#include "pch.h"
#include "jparse.h"
#include <iostream>
#include <vector>
#include <string>

void JStruct::print(std::string tab) {
	if (!this) return;
	switch (this->mode) {
	case _JMODE_OBJ: {
		std::cout << tab << "{" << std::endl;
		tab = tab + "\t";
		//std::cout << "sz: " << this->body.size() << std::endl;
		for (int i = 0; i < this->body.size(); i++) {
			if (this->body[i].body == nullptr) {
				std::cout << tab + this->body[i].label << ": " << this->body[i].rawValue << "," << std::endl;
			}
			else {
				//std::cout << tab + this->body[i].label << "   " << this->body[i].body << "   " << this << "   " << this->body.size() << "   "  << i;
				std::cout << tab + this->body[i].label << ": ";
				this->body[i].body->print(tab);
				//std::cout << tab + this->body[i].label << "   " << this->body[i].body << "   " << this->body[i].body->body[0].label << "  " << this << "   " << this->body.size() << "   " << i;
			}
		}
		std::cout << std::endl << tab.substr(0, tab.length() - 1) << "}" << std::endl;
		break;
	}
	case _JMODE_ARR: {
		std::cout << tab << "[" << std::endl;
		tab = tab + "\t";
		//std::cout << "sz: " << this->body.size() << std::endl;
		for (int i = 0; i < this->body.size(); i++) {
			if (this->body[i].body == nullptr) {
				std::cout << tab + this->body[i].rawValue << "," << std::endl;
			}
			else {
				//std::cout << tab + this->body[i].label << "   " << this->body[i].body << "   " << this << "   " << this->body.size() << "   "  << i;
				this->body[i].body->print(tab);
				//std::cout << tab + this->body[i].label << "   " << this->body[i].body << "   " << this->body[i].body->body[0].label << "  " << this << "   " << this->body.size() << "   " << i;
			}
		}
		std::cout << std::endl << tab.substr(0, tab.length() - 1) << "]" << std::endl;
		break;
	}
	}
}

JValue JStruct::operator[](int idx) {
	if (this->mode == _JMODE_ARR) {
		if (idx > 0 && idx < this->body.size())
			return JValue(this->body[idx]);
		else
			return JValue();
	}
	else {
		return JValue();
	}
}

JValue JStruct::operator[](std::string label) {
	return JValue(this->FindToken(label));
}

void JStruct::free() {
	for (int i = 0; i < this->body.size(); i++) {
		if (this->body[i].body != nullptr) {
			this->body[i].body->free();
		}
		this->body[i].free();
	}
}

JToken JStruct::FindToken(std::string label) {
	for (const JToken tok : body) {
		if (tok.label == label) return tok;
	}

	return JToken();
}

/*template<typename _Ty> _Ty JValue::value() {
	switch ((int)this->type) {
	case JTYPE_STRING: {
		return this->rawValue;
		break;
	}
	case JTYPE_FLOAT: {
		try {
			return std::stof(this->rawValue);
		}
		catch (std::exception e) {
			return 0.0f;
		}
		break;
	}
	case JTYPE_INT: {
		try {
			return std::stoi(this->rawValue);
		}
		catch (std::exception e) {
			return NULL;
		}
		break;
	}
	}

	return NULL;
}*/

JValue::JValue(JToken jt) {
	const char* vRaw = jt.rawValue.c_str();

	auto _gType = [](std::string str) {
		std::string nChars = "0123456789.";
		JType _mode = Int;
		for (const char _C : str) {
			if (nChars.find(_C) == std::string::npos)
				return String;
			if (_C == '.')
				_mode = Float;
		}

		return _mode;
	};

	if (jt.rawValue.length() > 0)
		this->type = _gType(jt.rawValue);
}

void JToken::free() {
	if (this->body != nullptr) delete[] this->body;
}

/*

Not Used so yeah

*/
void copyTokToVec(std::vector<JToken> _v, JToken tok) {
	_v.push_back(tok);
	JToken* vtk = &_v[_v.size() - 1];
	vtk->body = tok.body;
	vtk->label = tok.label;
	vtk->rawValue = tok.rawValue;
}

std::string fetchStr(const char* str, int str_len, int idx, int len) {
	std::string r = "";
	for (int i = idx; i < idx + len; i++) {
		if (i > str_len) return r;
		r += str[i];
	}

	return r;
}

std::string jp_raw_val(std::string val) {
	const char* _Vstr = val.c_str();

	if (_Vstr[0] == '"') {
		std::cout << _Vstr << "   " << _Vstr[0] << "   vstr" << std::endl;
		if (_Vstr[val.length() - 1] == '"') {
			return val.substr(1, val.length() - 2);
		}
		else {
			throw JERR_INVALID_TOK;
		}
	}
	else {
		return val;
	}
}

void jparse::genTok(const char* rStr, JToken& tok) {
	if (rStr[0] == '{') {
		//parse as JStruct
		JStruct _r = jparse::parseStr(rStr);
		tok.body = new JStruct();

		//memcpy(tok.body, &_r, sizeof(JStruct));
		tok.body->body = _r.body;
		tok.body->mode = _r.mode;

		//std::cout << "Tok: " << tok.label << "   " << tok.rawValue << std::endl;
	}
	else if (rStr[0] == '[') {
		//parse as JStruct
		JStruct _r = jparse::parseStr(rStr, false);
		tok.body = new JStruct();

		//memcpy(tok.body, &_r, sizeof(JStruct));
		tok.body->body = _r.body;
		tok.body->mode = _r.mode;

		//std::cout << "STUFF: " << rStr << std::endl;
	}
	else {
		tok.rawValue = jp_raw_val(rStr);

		//std::cout << "Tok: " << tok.label << "   " << tok.rawValue << std::endl;
	}
}

std::string jparse::GenerateString(JStruct json, bool min, std::string tab) {
	auto _gType = [](std::string str) {
		std::string nChars = "0123456789.";
		JType _mode = Int;
		for (const char _C : str) {
			if (nChars.find(_C) == std::string::npos)
				return String;
			if (_C == '.')
				_mode = Float;
		}

		return _mode;
	};

	std::string rStr = "";
	switch (json.mode) {
	case _JMODE_OBJ: {
		rStr += "{";
		for (int i = 0; i < json.body.size(); i++) {
			if (json.body[i].body == nullptr) {
				std::string val = json.body[i].rawValue;
				if (_gType(json.body[i].rawValue) == String)
					val = "\"" + val + "\"";
				rStr += "\"" + json.body[i].label + "\":" + val + (i < json.body.size() - 1 ? "," : "");
			}
			else {
				rStr += "\"" + json.body[i].label + "\":";
				rStr += jparse::GenerateString(*json.body[i].body, min, "") + (i < json.body.size() - 1 ? "," : "");
			}
		}
		rStr += "}";
		break;
	}
	case _JMODE_ARR: {
		rStr += "[";
		for (int i = 0; i < json.body.size(); i++) {
			if (json.body[i].body == nullptr) {
				std::string val = json.body[i].rawValue;
				if (_gType(json.body[i].rawValue) == String)
					val = "\"" + val + "\"";
				rStr += val + (i < json.body.size() - 1 ? "," : "");
			}
			else {
				rStr += jparse::GenerateString(*json.body[i].body, min, "") + (i < json.body.size() - 1 ? "," : "");
			}
		}
		rStr += "]";
		break;
	}
	}

	return rStr;
}

/*

Funciton to remove whitespace and unnecesary characters from string

*/

std::string removeJunk(const char* code) {
	int len = strlen(code);

	//create a string cause its easier to work with
	std::string codeStrRes = "", res = "";

	//first remove comments
	bool inComment = false;
	bool inLineComment = false;

	int line = 0;

	std::string lineCollector = "";

	bool lineBegin = true;

	bool inStr = false;

	auto _charEscaped = [](const char* code, int idx) {
		int bCount = 0, cIdx = idx - 1, len = strlen(code);

		while (cIdx >= 0 && code[cIdx] == '\\') {
			bCount++;
			cIdx--;
		}

		return (bCount % 2) == 0 ? false : true;
	};

	for (int i = 0; i < len; i++) {
		if (code[i] == '\t' || code[i] == '\v' || code[i] == '\f' || code[i] == '\r' || (code[i] == ' ' && !inStr)) continue;
		if (code[i] == '\n') {
			line++;
			if (inLineComment && !inComment) inLineComment = false;
			continue;
		}

		if (code[i] == '"') {
			bool isBackSlashed = _charEscaped(code, i);

			if (!isBackSlashed) {
				inStr = !inStr;
			}
		}

		//
		if (fetchStr(code, len, i, 2) == "/*") {
			//std::cout << "In comment! " << "   " << inComment << std::endl;
			inComment = true;
			i++;
			continue;
		}

		if (fetchStr(code, len, i, 2) == "//" && !inComment) {
			i++;
			inLineComment = true;
			continue;
		}

		//std::cout << fetchStr(code, len, i, 2) << std::endl;

		if (fetchStr(code, len, i, 2) == "*/") {
			if (!inComment)
				return "";

			//std::cout << "End of comment! " << "   " << inComment << std::endl;
			inComment = false;
			i++;
			continue;
		}

		if (!inComment && !inLineComment) {
			res += code[i];
		}

		lineBegin = false;
	}
	if (inComment) {
		return "";
	}

	//
	//std::cout << res << std::endl;
	return res;
}

JStruct jparse::parseStr(const char* jData, bool _clean) {
	char* jsonData;
	if (_clean) {
		std::string rStr = removeJunk(jData);
		const char* c_str = rStr.c_str();
		jsonData = new char[strlen(c_str)];
		memcpy(jsonData, c_str, strlen(c_str));
	}
	else {
		jsonData = (char*)jData;
	}
	//std::cout << "pStr: " << jsonData << std::endl;
	JStruct rStruct;

	int mode; //0 obj, 1 array

	//mode check
	char bChar = jsonData[0];

	switch (bChar) {
	case '[': {
		mode = _JMODE_ARR;
		//std::cout << "ARR MODE" << std::endl;
		break;
	}
	case '{': {
		mode = _JMODE_OBJ;
		break;
	}
	default: {
		return JStruct();
	}
	}

	//std::cout << "MODE:   " << bChar << "   " << mode << std::endl;

	//split
	int cChar = 1, len = strlen(jsonData), indentsC = 0, indentsA = 0;

	std::vector<std::string> chunks;

	std::string charCollect;

	bool cBreak = false, fBreak = false;

	while (cChar < len) {

		switch (jsonData[cChar]) {
		case ',': {
			if (indentsC == 0 && indentsA == 0) {
				cBreak = true;

				chunks.push_back(charCollect);
				charCollect = "";
			}
			break;
		}
		case '{': {
			indentsC++;
			break;
		}
		case '[': {
			indentsA++;
			break;
		}
		case '}': {
			indentsC--;
			if (indentsC < 0) fBreak = true;
			break;
		}
		case ']': {
			indentsA--;
			if (indentsA < 0) fBreak = true;
			break;
		}
		}

		if (fBreak) break;

		if (!cBreak)
			charCollect += jsonData[cChar];
		else
			cBreak = false;

		cChar++;
	}

	chunks.push_back(charCollect);

	//do some parsing stuff, ya know, the usual
	auto _split = [](const char* str, char delim) {
		std::vector<std::string> res;
		std::string current;
		int rb, ab;
		rb = ab = 0;
		for (int i = 0; i < strlen(str); i++) {
			switch (str[i]) {
			case '{': {
				rb++;
				break;
			}
			case '}': {
				rb--;
				break;
			}
			case '[': {
				ab++;
				break;
			}
			case ']': {
				ab--;
				break;
			}
			}
			if (str[i] == delim && rb <= 0 && ab <= 0) {
				res.push_back(current);
				current = "";
				continue;
			}

			current += str[i];
		}

		res.push_back(current);
		return res;
	};

	if (mode == _JMODE_OBJ) {
		for (std::string c : chunks) {
			//std::cout << "P Chunk " << c << std::endl;
			std::vector<std::string> lSplit = _split(c.c_str(), ':');
			//for (std::string s : lSplit)
				//std::cout << s << " t" << std::endl;
			if (lSplit.size() > 2) return JStruct();

			JToken tok;

			const char* lStr = lSplit[0].c_str();
			//label str check
			if (lStr[0] != '"' || lStr[lSplit[0].length() - 1] != '"') return JStruct();

			tok.label = lSplit[0].substr(1, lSplit[0].length() - 2); //left

			//value
			const char* rStr = lSplit[1].c_str();
			//std::cout << "Right Parse: " << rStr << std::endl;
			genTok(rStr, tok);

			rStruct.body.push_back(tok);
		}
	}
	else {
		//parse as an array
		for (std::string c : chunks) {
			JToken _tok;
			genTok(c.c_str(), _tok);
			rStruct.body.push_back(_tok);
		}
	}

	rStruct.mode = mode;

	return rStruct;
}