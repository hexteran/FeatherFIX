#include "MessageProcessor.h"

using namespace FeatherFAST;
using namespace std;

Message MessageProcessor::Process(outPut *in)
{
	Container Result;
	inputbuffer = in;
	temp = in->temp;
	int cc = 0; 
	Result.tid = temp->tid;
/*	for (int i = 0; i < inputbuffer->length; i++)
		std::cout << inputbuffer->ids[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < inputbuffer->length; i++)
		std::cout << ((inputbuffer->types[i]) & (SEQUENCE | SEQ_END)) << " ";*/
	int ic = 0;
	int offset = 0;
	int last_ic = ic;
	int second_lvl_last_ic = ic;
	bool is_in_seq = false;
	bool second_in_seq = false;
	for (int i = 0; i < inputbuffer->length; i++) {//может возникать проблема если константы есть на sequence второго уровня
		if (is_in_seq == false) last_ic = ic;
		if (inputbuffer->ids[i] == SEQ_NUM_EXMP)
		{
			std::cout << inputbuffer->ids[i] << " = " << (int)inputbuffer->values[cc] << endl;
			Result.ids[i] = inputbuffer->ids[i];
			Result.types[i] = inputbuffer->types[i];
			*(int*)& Result.values[cc+offset] = (int)inputbuffer->values[cc];
			cc += sizeof(int);
			continue;
		}
		if (inputbuffer->ids[i] == SEQUENCE_ID)
		{
			if (is_in_seq == true)
			{
				second_in_seq = true;
				second_lvl_last_ic = ic;
			}
			else last_ic = ic;
			cout << "SEQBEG" << endl;
			Result.ids[i] = inputbuffer->ids[i];
			Result.types[i] = inputbuffer->types[i];
			is_in_seq = true;
			continue;
		}
		if ((inputbuffer->ids[i] == SEQ_END_ID))
		{
			if (second_in_seq == true)
			{
				second_in_seq = false;
				ic = second_lvl_last_ic;
			}
			else 
			{
				is_in_seq = false;
				ic = last_ic;
			}
			cout << "SEQEND" << endl;
			Result.ids[i] = inputbuffer->ids[i];
			Result.types[i] = inputbuffer->types[i];
			continue;
		}
		if ((inputbuffer->types[i] & CONSTANT))
		{
			Result.ids[i] = inputbuffer->ids[i];
			Result.types[i] = inputbuffer->types[i];
			strcpy(&Result.values[cc + offset], temp->constants[ic]);
			cout << Result.ids[i] << " = " << &Result.values[cc + offset] <<endl;
			offset += strlen(temp->constants[ic])+1;
			ic++;
			continue;
		}
		if ((inputbuffer->types[i] & FAST_DECIMAL))
		{
			Result.ids[i] = inputbuffer->ids[i];
			Result.types[i] = inputbuffer->types[i];
			*(decimal_value*)& Result.values[cc + offset] = *(decimal_value*)&inputbuffer->values[cc];
			cout << Result.ids[i] << " = " << ((decimal_value*)& Result.values[cc + offset])->exponent <<", "<< ((decimal_value*)& Result.values[cc + offset])->mantissa<<endl;
			cc += sizeof(decimal_value);
			continue;
		}
		if ((inputbuffer->types[i] & (uInt32 | Int32)))
		{
			Result.ids[i] = inputbuffer->ids[i];
			Result.types[i] = inputbuffer->types[i];
			*(unsigned int*)& Result.values[cc + offset] = *(unsigned int*)&(inputbuffer->values[cc]);
			cout << Result.ids[i] << " = " << *(unsigned int*) & (inputbuffer->values[cc])<<endl;
			cc += 4;
		}
		if ((inputbuffer->types[i] & (uInt64 | Int64)))
		{
			Result.ids[i] = inputbuffer->ids[i];
			Result.types[i] = inputbuffer->types[i];
			*(long long*)& Result.values[cc + offset] = *(long long*)&inputbuffer->values[cc];
			cout << Result.ids[i] << " = " << *(long long*) & (inputbuffer->values[cc])<<endl;
			cc += 8;
		}
		if ((inputbuffer->types[i] & (STRING)))
		{
			Result.ids[i] = inputbuffer->ids[i];
			Result.types[i] = inputbuffer->types[i];
			strcpy(&Result.values[cc + offset], &(inputbuffer->values[cc]));
			cout << Result.ids[i] << " = " << &Result.values[cc + offset]<<endl;
			cc += strlen(&inputbuffer->values[cc]) + 1;
		}
	};
	Result.val_length = cc;
	Result.length = inputbuffer->length;
	Result.seqnum = in->seqnum;
	Message ultimate(Result);
	cout << endl;
	while (ultimate.is_end() != true)
	{
		if (ultimate.type() == SEQ_NUM_EXMP)
		{
			std::cout << ultimate.field_id() << " = " << *(int*)ultimate.value() << endl;
			if(ultimate.next()==false) break;
			continue;
		}
		if (ultimate.field_id() == SEQUENCE_ID)
		{
			std::cout << "SEQBEG" << endl;
			if (ultimate.next() == false) break;
			continue;
		}
		if ((ultimate.field_id() == SEQ_END_ID))
		{
			cout << "SEQEND" << endl;
			if (ultimate.next() == false) break;
			continue;
		}
		if ((ultimate.type() & CONSTANT))
		{
			std::cout << ultimate.field_id() <<"="<< ultimate.value()<<endl;
			if (ultimate.next() == false) break;
			continue;
		}
		if ((ultimate.type() & FAST_DECIMAL))
		{
			cout << ultimate.field_id() << " = " << ((decimal_value*)ultimate.value())->exponent << "," << ((decimal_value*)ultimate.value())->mantissa << endl;
			if (ultimate.next() == false) break;
			continue;
		}
		if ((ultimate.type() & (uInt32 | Int32)))
		{
			cout << ultimate.field_id() << " = " << *(int*)ultimate.value() << endl;
			if (ultimate.next() == false) break;
			continue;
		}
		if ((ultimate.type() & (uInt64 | Int64)))
		{
			cout << ultimate.field_id() << " = " << *(long long*)ultimate.value() << endl;
			if (ultimate.next() == false) break;
			continue;
		}
		if ((ultimate.type() & (STRING)))
		{
			cout << ultimate.field_id() << " = " << ultimate.value() << endl;
			if (ultimate.next() == false) break;
		}
	}
	return ultimate;
}

bool FeatherFAST::Message::next()
{
	if (is_end()) return false;
	if (mes.ids[it] == (SEQUENCE_ID))
	{
		it++;
		return true;
	}
	if (mes.ids[it] == (SEQ_END_ID))
	{
		it++;
		return true;
	}
	if (mes.ids[it] == (SEQ_NUM_EXMP))
	{
		it++;
		iv += 4;
		return true;
	}
	if (mes.types[it] & (STRING | CONSTANT))
	{
		it++;
		iv += strlen(&mes.values[iv]) + 1;
		return true;
	}
	if (mes.types[it] & (Int32 | uInt32))
	{
		it++;
		iv += 4;
		return true;
	}
	if (mes.types[it] & (Int64 | uInt64))
	{
		it++;
		iv += 8;
		return true;
	}
	if (mes.types[it] & (FAST_DECIMAL))
	{
		it++;
		iv += sizeof(decimal_value);
		return true;
	}
}

int FeatherFAST::Message::type()
{
	return mes.types[it];
}

char* FeatherFAST::Message::value()
{
	if (mes.ids[it] == SEQ_END_ID) return (char*)seqend;
	if (mes.ids[it] == SEQUENCE_ID) return (char*) seqbeg;
	return &mes.values[iv];
}

int FeatherFAST::Message::field_id()
{
	return mes.ids[it];
}

int FeatherFAST::Message::length()
{
	return mes.length;
}

int FeatherFAST::Message::seqnum()
{
	return mes.seqnum;
}

void FeatherFAST::Message::add_field(char* field, int id, int type)
{

	if (mes.length >= OUTPUT_MAX_SIZE) throw MESSAGE_BUFFER_OVERFLOW;
	mes.types[mes.length] = type;
	mes.ids[mes.length] = id;
	if (type & (Int32|uInt32))
	{
		*(int*)&mes.values[i_len] = *(int*) field;
		i_len += 4;
	};
	if (type & (Int64 | uInt64))
	{
		*(long long*)& mes.values[i_len] = *(long long*) field;
		i_len += 8;
	};
	if (type & (STRING | CONSTANT))
	{
		strcpy(&mes.values[i_len], field);
		i_len += strlen(field)+1;
	};
	if (type & BYTE_VECTOR); //добавить поддержку BYTE_VECTOR

}

bool FeatherFAST::Message::delete_field()
{
	if (length() == 0) return false;
	return true;
	/*int space = 0;
	if (mes.ids[it] == (SEQUENCE_ID))
	{
		move_id();
		move_t();
		return true;
	}
	if (mes.ids[it] == (SEQ_END_ID))
	{
		move_id();
		move_t();
		return true;
	}
	if (mes.ids[it] == (SEQ_NUM_EXMP))
	{
		move_id();
		move_t();
		move_v(4);
		return true;
	}
	if (mes.types[it] & (Int32 | uInt32))
	{
		move_id();
		move_t();
		move_v(4);
		return true;
	}
	if (mes.types[it] & (Int64 | uInt64))
	{
		move_id();
		move_t();
		move_v(8);
		return true;
	}
	if (mes.types[it] & (STRING | CONSTANT))
	{
		move_id();
		move_t();
		move_v(strlen(&mes.values[iv])+1);
		return true;
	}
	if (mes.types[it] & (FAST_DECIMAL))
	{
		move_id();
		move_t();
		move_v(sizeof(decimal_value));
		return true;
	}*/
}

bool FeatherFAST::Message::is_end()
{
	return it == mes.length;
}

bool FeatherFAST::Message::is_beg()
{
	return it==0;
}
