#pragma once
#include "FIXDecoder.h"
#include <cmath>
inline void FIXDecoder::process_no_temp(char* message, unsigned int length, FeatherFAST::Message& mess)
{
	int i = 0;
	while (i < length)
	{
		char id_raw[10];
		char content[65536];
		int id;
		int j = 0;
		for (; (i < length) && (message[i] != '='); i++)
		{
			if (j >= 10) throw 1;
			id_raw[j] = message[i];
			j++;
		};
		id_raw[j] = 0;
		if (i >= length) throw 1; //сделать обработку
		i++;
		id = stoi(id_raw);
		j = 0;
		for (;(i < length) && (message[i] != '\1'); i++) //почему при замене на SEPARATOR требует скобку?
		{
			if (j >= 65536) throw 1;
			content[j] = message[i];
			j++;
		};
		content[j] = 0;
		if (i >= length) throw 1; //сделать обработку
		i++;
		mess.add_field(content, id, STRING);
	}
}

int FIXDecoder::find_field_in_temp(int *it, int id, FeatherFAST::Template& temp)
{
	int i = 0;
	for (i = *it; i < temp.temp_size; i++)
	{
		if (temp.fieldids[i] == id) {
			*it = i;
			return temp.temp[i];
		};
	}
	for (i = 0; i < *it; i++)
	{
		if (temp.fieldids[i] == id) {
			*it = i;
			return temp.temp[i];
		};
	}
	return STRING;
}

inline void FIXDecoder::process_with_temp(char* message, unsigned int length, FeatherFAST::Message& mess, FeatherFAST::Template& temp)
{
	int it = 0;
	int i = 0;
	while (i < length)
	{
		char id_raw[10];
		char content[65536];
		int id;
		int j = 0;
		for (; (i < length) && (message[i] != '='); i++)
		{
			if (j >= 10) throw 1;
			id_raw[j] = message[i];
			j++;
		};
		id_raw[j] = 0;
		if (i >= length) throw 1; //сделать обработку
		i++;
		id = stoi(id_raw);
		j = 0;
		for (; (i < length) && (message[i] != '\1'); i++) //почему при замене на SEPARATOR требует скобку?
		{
			if (j >= 65536) throw 1;
			content[j] = message[i];
			j++;
		};
		content[j] = 0;
		if (i >= length) throw 1; //сделать обработку
		i++;
		int type = find_field_in_temp(&it, id, temp);
		if (id == 5842)
			cout << "a";
		if (!(type & FAST_DECIMAL))
		{
			if ((type & (Int32 | uInt32))!=0)
			{
				int l = stoi(content);
				mess.add_field((char*)& l, id, type);
			};
			if ((type & (Int64 | uInt64)) != 0)
			{
				long long l = stoll(content);
				mess.add_field((char*)& l, id, type);
			};
			if ((type & (STRING | CONSTANT)) != 0)
			{
				mess.add_field(content, id, type);
			};
			if ((type & BYTE_VECTOR) != 0)
			{
			//	обработка появится в будущем
			};
		}
		else
		{
			FeatherFAST::decimal_value val;
			char mantissa[10];
			int exponent = 0;
			int len = strlen(content);
			int flag = 0;
			for (int j = 0; j < len; j++)
			{
				if (content[j] == '.')
				{
					flag = 1;
					j++;
				};
				if (flag == 1) exponent--;
				mantissa[j - flag] = content[j];
			};
			mantissa[j-flag] = 0;
			val.mantissa = stoi(mantissa);
			if (exponent >= 0) exponent++;
			val.exponent = exponent;
			mess.add_field((char*)&val, id, type);
		}
	}
}

FeatherFAST::Message FIXDecoder::Process(char* message, unsigned int length, FeatherFAST::Template &temp)
{	
	FeatherFAST::Message Result;
	process_with_temp(message, length, Result, temp);
	return Result;
}

FeatherFAST::Message FIXDecoder::Process(char* message, unsigned int length)
{
	FeatherFAST::Message Result;
	process_no_temp(message, length, Result);
	return Result;
}
