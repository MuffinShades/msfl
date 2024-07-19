#pragma once
#pragma warning(disable: 4660)
/*

	Cfg parser

	Written in C++ by James Weigand

	This utility is used to parse .cfg, .ini, and .inf files

*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

#define CFGERROR_BLANKSRC 0xef0
#define CFGERROR_INVALIDHEADER 0xef01
#define CFGERROR_INVALIDSYNTAX_EQ 0xef02

#ifdef MSFL_EXPORTS
#define MSFL_EXP __declspec(dllexport)
#else
#define MSFL_EXP
#endif

//#define PRINTCFGERR //define if you want errors printed
extern "C" {

	class CfgToken {
	private:
		std::string _rawValue = "";
		std::string _tokenLabel = "";
	public:
		MSFL_EXP std::string GetRawValue();
		//NOTE only supports standard types!
		//template<typename _T> MSFL_EXP _T GetValueAs();
		//template<typename _T> MSFL_EXP std::vector<_T> GetValueVectorAs();
		MSFL_EXP void SetRawValue(std::string val);
		MSFL_EXP void SetLabel(std::string label);
		MSFL_EXP std::string GetLabel();
	};

	class CfgHeader {
	private:
		int returnStatus = 0x00;
	public:
		std::string headerLabel = "";
		std::vector<CfgToken> _tokens;
		MSFL_EXP int GetStatus();
		MSFL_EXP CfgToken ExtractToken(std::string name);
		MSFL_EXP void AddToken(CfgToken token);
	};

	class CfgStructure {
	private:
		int returnStatus = 0x00;
	public:
		std::vector<CfgHeader> _headers;
		MSFL_EXP CfgHeader ExtractHeader(std::string label);
		MSFL_EXP void AddHeader(CfgHeader h);
		MSFL_EXP int GetStatus() { return this->returnStatus; };
		MSFL_EXP void _setErr(int err) {
			this->returnStatus = err;
		};
	};

	class CfgParse
	{
	public:
		MSFL_EXP static CfgStructure ParseFile(std::string src);
		MSFL_EXP static CfgStructure ParseStr(const char* dat, int len);
		MSFL_EXP static CfgHeader ExtractHeader(std::string src, std::string headerName);
		MSFL_EXP static int WriteToFile(std::string src, CfgStructure data);
		MSFL_EXP static CfgHeader CreateGlobalHeader();
	};

}