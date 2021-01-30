#pragma once
#include "C:\\Buffer\\Trading\\FAST\FAST\\FAST\\tinyxml\\tinyxml.h" // change
#include <map>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

//field properties flags
#define OPT 4096
#define DELTA 1
#define COPY 2 
#define TAIL 4
#define DEFAULT 8
#define INCREMENT 16
#define uInt32 32
#define uInt64 64
#define Int32 128
#define Int64 256
#define FAST_DECIMAL 512
#define STRING 1024 
#define BYTE_VECTOR 2048
#define CONSTANT 8192
#define SEQUENCE 16384
#define SEQ_END 32768

//error codes:
#define XMLFileError 101
#define XMLUndefinedElement 102
#define XMLUndefinedType 103
#define XMLUndefinedOperator 104
#define XMLSequenceUndefinedType 105
#define XMLprocessDefaultOp 106
#define XMLprocessConstantOp 107
namespace FeatherFAST {
	typedef int FIELD;

	class __declspec(dllexport) XMLReader
	{
	private:
		map <int, vector<FIELD>> _templates;
		vector<string> buffer_defaults;
		vector<string> buffer_constants;
		vector<int> buffer_field_ids;
		map<int, vector<string>> _defaults;
		map<int, vector<string>> _constants;
		map<int, vector<int>> _field_ids;

		void _processTemplate(TiXmlElement* element);
		void _processDefaultOp(TiXmlElement* elem);
		void _processConstantOp(TiXmlElement* elem);
		FIELD _processString(TiXmlElement* element);
		FIELD _processuInt32(TiXmlElement* element);
		FIELD _processuInt64(TiXmlElement* element);
		FIELD _processInt32(TiXmlElement* element);
		FIELD _processInt64(TiXmlElement* element);
		FIELD _processDecimal(TiXmlElement* element);
		FIELD _processByteVector(TiXmlElement* element);
		FIELD _processSequence(TiXmlElement* element, vector<FIELD>& temp);

	public:
		XMLReader(string filename);
		map<int, vector<int>> GetTemplates();
		map<int, vector<string>> GetDefaults();
		map<int, vector<string>> GetConstants();
		map<int, vector<int>> GetFieldIds();
	};
};