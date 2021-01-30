#include "XMLReader.h"
using namespace FeatherFAST;
void XMLReader::_processTemplate(TiXmlElement* element)
{
	vector<FIELD> temp;
	for (TiXmlElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		string val(child->Value());
		int new_field;
		bool flag = false;
		if (val == "uInt32") {
			new_field = _processuInt32(child); flag = true;
		};
		if (val == "uInt64") {
			new_field = _processuInt64(child); flag = true;
		};
		if (val == "int32") {
			new_field = _processInt32(child); flag = true;
		};
		if (val == "int64") {
			new_field = _processInt64(child); flag = true;
		};
		if (val == "string") {
			new_field = _processString(child); flag = true;
		};
		if (val == "sequence") {
			new_field = _processSequence(child, temp); flag = true;
		};
		if (val == "decimal") {
			new_field = _processDecimal(child); flag = true;
		};
		if (val == "byteVector") {
			new_field = _processByteVector(child); flag = true;
		};
		if (flag == false) {
			throw XMLUndefinedType;
		}
		else { 
			temp.push_back(new_field); 
			for (TiXmlAttribute* attr = child->FirstAttribute(); attr; attr = attr->Next()) {
				if ((string)attr->Name() == "id")
					buffer_field_ids.push_back(stoi(attr->Value()));
			};
		};
	}
	for (TiXmlAttribute* attr = element->FirstAttribute(); attr; attr = attr->Next())
	{
		if (string(attr->Name()) == "id")
		{
			int key = stoi(attr->Value());
			_templates.insert(pair<int, vector<FIELD>>(key, temp));
			_defaults.insert(pair<int, vector<string>>(key, buffer_defaults));
			_constants.insert(pair<int, vector<string>>(key, buffer_constants));
			_field_ids.insert(pair<int, vector<int>>(key, buffer_field_ids));
			buffer_defaults.clear();
			buffer_constants.clear();
			buffer_field_ids.clear();
		}
	}
}

 void XMLReader::_processDefaultOp(TiXmlElement* elem)
{
	bool flag = false;
	for (TiXmlAttribute* attr = elem->FirstAttribute(); attr; attr = attr->Next())
		if ((string)attr->Name() == "value")
		{
			buffer_defaults.push_back((string)attr->Value());
			flag = true;
		};
	if (flag == false)
		throw XMLprocessDefaultOp;

}

void XMLReader::_processConstantOp(TiXmlElement* elem)
{
	bool flag = false;
	for (TiXmlAttribute* attr = elem->FirstAttribute(); attr; attr = attr->Next())
		if ((string)attr->Name() == "value")
		{
			buffer_constants.push_back((string)attr->Value());
			flag = true;
		};
	if (flag == false)
		throw XMLprocessConstantOp;
}

FIELD XMLReader::_processString(TiXmlElement* element)
{
	FIELD flags = STRING;
	bool flag = false;
	for (TiXmlElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		if ((string)child->Value() == "delta") {
			flags += DELTA;
			flag = true;
		};
		if ((string)child->Value() == "copy") {
			flags += COPY;
			flag = true;
		};
		if ((string)child->Value() == "default") {
			_processDefaultOp(child);
			flags += DEFAULT;
			flag = true;
		};
		if ((string)child->Value() == "increment") {
			flags += INCREMENT;
			flag = true;
		};
		if ((string)child->Value() == "tail")
		{
			flags += TAIL;
			flag = true;
		};
		if ((string)child->Value() == "constant") {
			_processConstantOp(child);
			flags += CONSTANT;
			flag = true;
		};
		if (flag == false)
		{
			throw XMLUndefinedOperator;
		}
	}
	for (TiXmlAttribute* child = element->FirstAttribute(); child; child = child->Next())
	{
		if (((string)child->Name() == "presence")&&((string)child->Value() == "optional")) flags += OPT;
	}
	return flags;
}

FIELD XMLReader::_processuInt32(TiXmlElement* element)
{
	FIELD flags = uInt32;
	bool flag = false;
	for (TiXmlElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		if ((string)child->Value() == "delta") {
			flags += DELTA;
			flag = true;
		};
		if ((string)child->Value() == "copy") {
			flags += COPY;
			flag = true;
		};
		if ((string)child->Value() == "default") {
			_processDefaultOp(child);
			flags += DEFAULT;
			flag = true;
		};
		if ((string)child->Value() == "increment") {
			flags += INCREMENT;
			flag = true;
		};
		if ((string)child->Value() == "tail")
		{
			flags += TAIL;
			flag = true;
		};
		if ((string)child->Value() == "constant") {
			_processConstantOp(child);
			flags += CONSTANT;
			flag = true;
		};
		if (flag == false)
		{
			throw XMLUndefinedOperator;
		}
	};
	for (TiXmlAttribute* child = element->FirstAttribute(); child; child = child->Next())
	{
		if (((string)child->Name() == "presence") && ((string)child->Value() == "optional")) flags += OPT;
	}
	return flags;
}

FIELD XMLReader::_processuInt64(TiXmlElement* element)
{
	FIELD flags = uInt64;
	bool flag = false;
	for (TiXmlElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		if ((string)child->Value() == "delta") {
			flags += DELTA;
			flag = true;
		};
		if ((string)child->Value() == "copy") {
			flags += COPY;
			flag = true;
		};
		if ((string)child->Value() == "default") {
			_processDefaultOp(child);
			flags += DEFAULT;
			flag = true;
		};
		if ((string)child->Value() == "increment") {
			flags += INCREMENT;
			flag = true;
		};
		if ((string)child->Value() == "tail")
		{
			flags += TAIL;
			flag = true;
		};
		if ((string)child->Value() == "constant") {
			_processConstantOp(child);
			flags += CONSTANT;
			flag = true;
		};
		if (flag == false)
		{
			throw XMLUndefinedOperator;
		}
	}
	for (TiXmlAttribute* child = element->FirstAttribute(); child; child = child->Next())
	{
		if (((string)child->Name() == "presence") && ((string)child->Value() == "optional")) flags += OPT;
	}
	return flags;
}

FIELD XMLReader::_processInt32(TiXmlElement* element)
{
	FIELD flags = Int32;
	bool flag = false;
	for (TiXmlElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		if ((string)child->Value() == "delta") {
			flags += DELTA;
			flag = true;
		};
		if ((string)child->Value() == "copy") {
			flags += COPY;
			flag = true;
		};
		if ((string)child->Value() == "default") {
			_processDefaultOp(child);
			flags += DEFAULT;
			flag = true;
		};
		if ((string)child->Value() == "increment") {
			flags += INCREMENT;
			flag = true;
		};
		if ((string)child->Value() == "tail")
		{
			flags += TAIL;
			flag = true;
		};
		if ((string)child->Value() == "constant") {
			_processConstantOp(child);
			flags += CONSTANT;
			flag = true;
		};
		if (flag == false)
		{
			throw XMLUndefinedOperator;
		}
	}
	for (TiXmlAttribute* child = element->FirstAttribute(); child; child = child->Next())
	{
		if (((string)child->Name() == "presence") && ((string)child->Value() == "optional")) flags += OPT;
	}
	return flags;
}

FIELD XMLReader::_processInt64(TiXmlElement* element)
{
	FIELD flags = Int64;
	bool flag = false;
	for (TiXmlElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		if ((string)child->Value() == "delta") {
			flags += DELTA;
			flag = true;
		};
		if ((string)child->Value() == "copy") {
			flags += COPY;
			flag = true;
		};
		if ((string)child->Value() == "default") {
			_processDefaultOp(child);
			flags += DEFAULT;
			flag = true;
		};
		if ((string)child->Value() == "increment") {
			flags += INCREMENT;
			flag = true;
		};
		if ((string)child->Value() == "tail")
		{
			flags += TAIL;
			flag = true;
		};
		if ((string)child->Value() == "constant") {
			_processConstantOp(child);
			flags += CONSTANT;
			flag = true;
		};
		if (flag == false)
		{
			throw XMLUndefinedOperator;
		}
	}
	for (TiXmlAttribute* child = element->FirstAttribute(); child; child = child->Next())
	{
		if (((string)child->Name() == "presence") && ((string)child->Value() == "optional")) flags += OPT;
	}
	return flags;
}

FIELD XMLReader::_processDecimal(TiXmlElement* element)
{
	FIELD flags = FAST_DECIMAL;
	bool flag = false;
	for (TiXmlElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		if ((string)child->Value() == "delta") {
			flags += DELTA;
			flag = true;
		};
		if ((string)child->Value() == "copy") {
			flags += COPY;
			flag = true;
		};
		if ((string)child->Value() == "default") {
			_processDefaultOp(child);
			flags += DEFAULT;
			flag = true;
		};
		if ((string)child->Value() == "increment") {
			flags += INCREMENT;
			flag = true;
		};
		if ((string)child->Value() == "tail")
		{
			flags += TAIL;
			flag = true;
		};
		if ((string)child->Value() == "constant") {
			_processConstantOp(child);
			flags += CONSTANT;
			flag = true;
		};
		if (flag == false)
		{
			throw XMLUndefinedOperator;
		}
	}
	for (TiXmlAttribute* child = element->FirstAttribute(); child; child = child->Next())
	{
		if (((string)child->Name() == "presence") && ((string)child->Value() == "optional")) flags += OPT;
	}
	return flags;
}

FIELD XMLReader::_processByteVector(TiXmlElement* element)
{
	FIELD flags = BYTE_VECTOR;
	bool flag = false;
	for (TiXmlElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		if ((string)child->Value() == "delta") {
			flags += DELTA;
			flag = true;
		};
		if ((string)child->Value() == "copy") {
			flags += COPY;
			flag = true;
		};
		if ((string)child->Value() == "default") {
			_processDefaultOp(child);
			flags += DEFAULT;
			flag = true;
		};
		if ((string)child->Value() == "increment") {
			flags += INCREMENT;
			flag = true;
		};
		if ((string)child->Value() == "tail")
		{
			flags += TAIL;
			flag = true;
		};
		if ((string)child->Value() == "constant") {
			_processConstantOp(child);
			flags += CONSTANT;
			flag = true;
		};
		if (flag == false)
		{
			throw XMLUndefinedOperator;
		}
	}
	for (TiXmlAttribute* child = element->FirstAttribute(); child; child = child->Next())
	{
		if (((string)child->Name() == "presence") && ((string)child->Value() == "optional")) flags += OPT;
	}
	return flags;
}

FIELD XMLReader::_processSequence(TiXmlElement* element, vector<FIELD>& temp)
{
	FIELD flags = SEQUENCE;
	buffer_field_ids.push_back(SEQUENCE);
	for (TiXmlAttribute* child = element->FirstAttribute(); child; child = child->Next())
	{
		if (((string)child->Name() == "presence") && ((string)child->Value() == "optional")) flags += OPT;
	}
	temp.push_back(flags);
	for (TiXmlElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		string val(child->Value());
		int new_field;
		bool flag = false;
		if ((val == "uInt32") || (val == "length")) {
			new_field = _processuInt32(child); flag = true;
		};
		if (val == "uInt64") {
			new_field = _processuInt64(child); flag = true;
		};
		if (val == "int32") {
			new_field = _processInt32(child); flag = true;
		};
		if (val == "int64") {
			new_field = _processInt64(child); flag = true;
		};
		if (val == "string") {
			new_field = _processString(child); flag = true;
		};
		if (val == "sequence") {
			new_field = _processSequence(child, temp); flag = true;
		};
		if (val == "decimal") {
			new_field = _processDecimal(child); flag = true;
		};
		if (val == "byteVector") {
			new_field = _processByteVector(child); flag = true;
		};
		if (flag == false) {
			throw XMLSequenceUndefinedType;
		}
		else{
			temp.push_back(new_field);
			for (TiXmlAttribute* attr = child->FirstAttribute(); attr; attr = attr->Next()) {
				if ((string)attr->Name() == "id")
					buffer_field_ids.push_back(stoi(attr->Value()));
			};
		}
	}
	buffer_field_ids.push_back(SEQ_END);
	return SEQ_END;
}

XMLReader::XMLReader(string filename)
{
	TiXmlDocument doc(filename.c_str());
	if (!(doc.LoadFile()))
	{
		throw XMLFileError;
		return;
	}	
	for (TiXmlElement* child = doc.FirstChildElement()->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		if ((string)child->Value() == "template")
			_processTemplate(child);
		else {
			throw XMLUndefinedElement;
		}
	}
}

map<int, vector<int>> XMLReader::GetTemplates()
{
	return _templates;
}

map<int, vector<string>> XMLReader::GetDefaults()
{
	return _defaults;
}

map<int, vector<string>> XMLReader::GetConstants()
{
	return _constants;
}

map<int, vector<int>> XMLReader::GetFieldIds()
{
	return _field_ids;
}
