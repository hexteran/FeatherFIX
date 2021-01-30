#include "FASTDecoder.h"
#include <iostream>
using namespace FeatherFAST;
FASTDecoder::FASTDecoder(map<int, vector<int>>& fields, map<int, vector<int>>& ids,
	map<int, vector<string>>& defaults, map<int, vector<string>>& constants):
	template_Mngr(fields, ids, defaults, constants), fst_prc()
{
	
}
FeatherFAST::FASTDecoder::FASTDecoder(XMLReader& xml) :
	_fields(xml.GetTemplates()), _ids(xml.GetFieldIds()), _defaults(xml.GetDefaults()),
	_constants(xml.GetConstants()), template_Mngr(_fields, _ids, _defaults, _constants), fst_prc()
{
};

Message FASTDecoder::Process(char* message,unsigned int length)
{
	seqnum = *(int*)message;
	pmap = &message[4];
	pmap_len = get_pmap_length(pmap);
	mposition = pmap_len + 4;

	int field_len;
	int template_number = to_uInt32(&message[mposition],&field_len);
	cout << "seqnum: " << seqnum;
	cout << "\ntemplate: " << template_number << "\n";
	//cout << "\n-------" << template_number << "-------\n";
	inPut in_fst_prcsr;
	outPut out_fst_prcsr;
	out_fst_prcsr.seqnum = seqnum;
	in_fst_prcsr.message = &message[mposition + field_len];
	in_fst_prcsr.pmap = pmap;
	in_fst_prcsr.temp = template_Mngr.pull_template(template_number);
	in_fst_prcsr.msg_length = length;
	out_fst_prcsr.temp = in_fst_prcsr.temp;
	fst_prc.Process(&in_fst_prcsr, &out_fst_prcsr);
	cout << "A\n";
	return MsgProc.Process(&out_fst_prcsr);
}

unsigned int FeatherFAST::to_uInt32(char* raw, int* length)
{
	char buffer[BUFFSIZE];
	int size = 16;
	int i = -1;
	int j = size;
	buffer[size - 1] = raw[0];
	for (int k = size; k < BUFFSIZE; k++)
	{
		buffer[k] = 0;
	}
	if ((raw[0] & 128) != 128)
	{
		do
		{
			i += 1;
			j -= 1;
			buffer[j - 1] = raw[i + 1];
			buffer[j - 1] = buffer[j - 1] & 127;
			buffer[j - 1] = buffer[j - 1] << 1;
			unsigned int* l = (unsigned int*)& buffer[j - 1];
			*l = *l >> 1;
			if (buffer[j + 3] & 1) {
				buffer[j + 2] += 128;
				buffer[j + 3] = 0;
};
		} while ((raw[i + 1] & 128) != 128);
		*length = i + 2;
	}
	else
	{
	buffer[size - 1] = buffer[size - 1] & 127;
	*length = 1;
	};
	unsigned int* l = (unsigned int*)& buffer[j - 1];
	return *l;
}

unsigned long long int FeatherFAST::to_uInt64(char* raw, int* length)
{
	char buffer[BUFFSIZE];
	int size = 16;
	int i = -1;
	int j = size;
	buffer[size - 1] = raw[0];
	for (int k = size; k < BUFFSIZE; k++)
	{
		buffer[k] = 0;
	}
	if ((raw[0] & 128) != 128)
	{
		do
		{
			i += 1;
			j -= 1;
			buffer[j - 1] = raw[i + 1];
			buffer[j - 1] = buffer[j - 1] & 127;
			buffer[j - 1] = buffer[j - 1] << 1;
			unsigned long long int* l = (unsigned long long int*) & buffer[j - 1];
			*l = *l >> 1;
			if (buffer[j + 7] & 1) {
				buffer[j + 6] += 128;
				buffer[j + 7] = 0;
			};
		} while ((raw[i + 1] & 128) != 128);
		*length = i + 2;
	}
	else
	{
		buffer[size - 1] = buffer[size - 1] & 127;
		*length = 1;
	};
	unsigned long long int* l = (unsigned long long int*) & buffer[j - 1];
	return *l;
}

int FeatherFAST::to_Int32(char* raw, int* length)
{
	char buffer[BUFFSIZE];
	int size = 16;
	int i = -1;
	int j = size;
	buffer[size - 1] = raw[0];
	for (int k = size; k < BUFFSIZE; k++)
	{
		buffer[k] = 0;
	}
	if ((raw[0] & 128) != 128)
	{
		do
		{
			i += 1;
			j -= 1;
			buffer[j - 1] = raw[i + 1];
			buffer[j - 1] = buffer[j - 1] & 127;
			buffer[j - 1] = buffer[j - 1] << 1;
			unsigned int* l = (unsigned int*)& buffer[j - 1];
			*l = *l >> 1;
			if (buffer[j + 3] & 1) {
				buffer[j + 2] += 128;
				buffer[j + 3] = 0;
			};
		} while ((raw[i + 1] & 128) != 128);
		*length = i + 2;
	}
	else
	{
		buffer[size - 1] = buffer[size - 1] & 127;
		*length = 1;
	};
	unsigned int* l = (unsigned int*)& buffer[j - 1];
	if (raw[0] & 64) {
		for (int i = 3; i >= 0; i--)
		{
			if (((char*)l)[i] == 0) ((char*)l)[i] = 0xff;
			else break;
		}
		i++;
		if (i >= 0)
		{
			char j = 128;
			while (((((char*)l)[i]) & j) == 0)
			{
				((char*)l)[i] += j;
				j = j / 2;
			}
		}
	}
	return (signed int)*l;
}

long long int FeatherFAST::to_Int64(char* raw, int* length)
{
	char buffer[BUFFSIZE];
	int size = 16;
	int i = -1;
	int j = size;
	buffer[size - 1] = raw[0];
	for (int k = size; k < BUFFSIZE; k++)
	{
		buffer[k] = 0;
	}
	if ((raw[0] & 128) != 128)
	{
		do
		{
			i += 1;
			j -= 1;
			buffer[j - 1] = raw[i + 1];
			buffer[j - 1] = buffer[j - 1] & 127;
			buffer[j - 1] = buffer[j - 1] << 1;
			unsigned long long int* l = (unsigned long long int*) & buffer[j - 1];
			*l = *l >> 1;
			if (buffer[j + 7] & 1) {
				buffer[j + 6] += 128;
				buffer[j + 7] = 0;
			};
		} while ((raw[i + 1] & 128) != 128);
		*length = i + 2;
	}
	else
	{
		buffer[size - 1] = buffer[size - 1] & 127;
		*length = 1;
	};
	long long int* l = (long long int*) & buffer[j - 1];
	if (raw[0] & 64) {
		for (int i = 7; i >= 0; i--)
		{
			if (((char*)l)[i] == 0) ((char*)l)[i] = 0xff;
			else break;
		}
		i++;
		if (i >= 0)
		{
			char j = 128;
			while (((((char*)l)[i]) & j) == 0)
			{
				((char*)l)[i] += j;
				j = j / 2;
			}
		}
	}
	return (signed long long)*l;
}

void FeatherFAST::to_string(char* message, char* buffer, int* length)
{
	int i = -1;
	do
	{
		i += 1;
		buffer[i] = message[i] & 127;
	} while ((message[i] & 128) == 0);
	buffer[i + 1] = 0;
	*length = i + 1;
};

decimal_value FeatherFAST::to_double(char* raw, int* length, bool is_two_byted)
{
	signed int exp = 0;
	signed long long mant = 0;
	int explen = 0;
	int mantlen = 0;
	if (is_two_byted == true)
	{
		exp = to_Int32(raw, &explen);
		raw += explen;
		if ((exp >= 64) || exp <= -64) throw TYPE_CONVERTION_ERROR;
	}
	
	mant = to_Int64(raw, &mantlen);
	decimal_value result;
	result.mantissa = mant;
	result.exponent = exp;
	*length = explen + mantlen;		
	return result;
};

int FASTDecoder::get_pmap_length(char* pmap)
{ 
	int last_byte = 0;
	while ((pmap[last_byte]&128)!=128)
		last_byte++;
	return last_byte+1;
}

int FASTGroup::get_field_length(char* field)
{
	int last_byte = 0;
	while ((field [last_byte] & 128) != 128)
		last_byte++;
	return last_byte + 1;
}

void TemplateManager::load_new_temp(int tid)
{
	try
	{
		int buf = second;
		second = first;
		first = third;
		third = buf;
		vector<string>& defaults = alldefaults[tid];
		vector<string>& constants = allconstants[tid];
		cache[first].tid = tid;
		cache[first].temp = &allfields[tid][0];
		cache[first].fieldids = &allids[tid][0];
		cache[first].temp_size = allfields[tid].size();
		cache[first].def_size = defaults.size();
		cache[first].const_size = constants.size();
		for (int i = 0; i < cache[first].def_size; i++)
		{
			cache[first].defaults[i] = (char*)defaults[i].c_str();
		}
		for (int i = 0; i < cache[first].const_size; i++)
		{
			cache[first].constants[i] = (char*)constants[i].c_str();
		}
		for (int i = 0; i < cache[first].temp_size; i++)
		{
			if (cache[first].temp[i] & SEQUENCE)
			{
				for (int j = i + 1; (j < cache[first].temp_size) && ((cache[first].temp[j] & SEQ_END) == 0); j++)
				{
					if (((DEFAULT | COPY | INCREMENT | TAIL) & cache[first].temp[j])!=0)
					{
					 cache[first].temp[i] = cache[first].temp[i] | GROUPPMAP;
						break;
					};
				}
			}
		}
	}
	catch (int)
	{
		throw TEMPLATEMANAGER_LOAD_ERROR;
	}
	
}

TemplateManager::TemplateManager(map<int, vector<int>>& fields, map<int, vector<int>>& ids,
	map<int, vector<string>>& defaults, map<int, vector<string>>& constants):
	allfields(fields),
	allids(ids),allconstants(constants), alldefaults(defaults){

	cache[0].tid = EMPTY_TEMPLATE;
	cache[1].tid = EMPTY_TEMPLATE;
	cache[2].tid = EMPTY_TEMPLATE;
	first = 0;
	second = 0;
	third = 0;
}

void TemplateManager::Init(map<int, vector<int>>& fields, map<int, vector<int>>& ids, map<int, vector<string>>& defaults, map<int, vector<string>>& constants)
{

}

Template* TemplateManager::pull_template(int tid)
{
	if (cache[first].tid == tid) return &cache[first];
	if (cache[second].tid == tid) return &cache[second];
	if (cache[third].tid == tid) return &cache[third];
	load_new_temp(tid);
	return &cache[first];
}

FASTprocessor::FASTprocessor()
{
	int k = 0;
}

void FASTprocessor::Process(inPut* in, outPut* out)
{
	otc = 0;
	ovc = 0;
	currentmsg = in->message;
	length = in->msg_length;
	outputbuffer = out;
	temp = in->temp;
	pmap = in->pmap;
	pmap_len = get_field_length(pmap);
	mc = 0;
	pc = 1;
	tc = 0;
	int dc = 0;
	int cc = 0;
	stepup(in);
	outputbuffer->length = otc;
}
void FASTprocessor::process_sequence(inPut* in, outPut* out, int* tco, int* mco, int* otco, int* ovco)
{
	seqproc.Process(in, out, tco, mco, otco, ovco);
}
void FASTGroup::stepup(inPut* in)
{
	while ((mc < length)&&(tc<temp->temp_size))
	{ // переработать условия
		bool isPresent = true;
		int field = step_temp();
		if (((field & DEFAULT) != 0) || ((field & COPY) != 0) || ((field & INCREMENT) != 0) || ((field & TAIL) != 0))
		{
			if ((field & uInt32) || (field & uInt64) || (field & Int32) || (field & Int64) || (field & STRING) || (field & BYTE_VECTOR))
			{
				isPresent = step_pmap();
			}
			if (field & FAST_DECIMAL)
			{
				isPresent = step_pmap();
				if ((currentmsg[mc] == (char)0x80) && (isPresent)) { isPresent = false; mc++; };
			}
		}
		else if (field & OPT)
		{
			if ((field & uInt32) || (field & uInt64) || (field & Int32) || (field & Int64) || (field & STRING) || (field & BYTE_VECTOR))
			{
				isPresent = true;
				if (currentmsg[mc] == (char)0x80) { isPresent = false; mc++; };
			}
			if (field & FAST_DECIMAL)
			{
				if (currentmsg[mc] == (char)0x80) { isPresent = false; mc++; };
			}
			if (field & CONSTANT) isPresent = step_pmap();
			if (field & SEQUENCE) isPresent = step_pmap();
		}
		if ((isPresent >= TRUE) || ((isPresent == DECFALSE)))
		{
			step_msg(field, isPresent, in);
		}
		if (field == 0x8000) break;
	}
}
int FASTGroup::step_temp() 
{
	if (tc < temp->temp_size)
	{
		tc++;
		return temp->temp[tc-1];
	}
	throw FASTprocessor_step_temp_ERROR;
}

bool FASTGroup::step_pmap() //нужен тест
{
	pc++;
	int k = (pc) / 8 + 1;
	char l = (pmap[k - 1] & (1 << (7 - (pc % 8))));
	if (k > pmap_len) return 0;
	if ((pc % (8*k-1) == 0)) pc++; // протестить - это может генерировать ошибки
	return l;
}

void FASTGroup::step_msg(int field, bool isPresent, inPut* in)
{
	outputbuffer->types[otc] = field;
	otc++;
	int len = 0;
	if (field & CONSTANT)
	{
		*(int*)& (outputbuffer->ids[otc - 1]) = in->temp->fieldids[tc - 1];
		
		return;
	}
	if (field & FAST_DECIMAL)
	{
		decimal_value k;
		if (isPresent == true) k = to_double(&currentmsg[mc], &len, true);
		else k = to_double(&currentmsg[mc], &len, false);
		*(decimal_value*)& (outputbuffer->values[ovc]) = k;
		*(int*)& (outputbuffer->ids[otc - 1]) = in->temp->fieldids[tc - 1];
		ovc += sizeof(decimal_value);
		
		
	}
	if ((field&uInt32))
	{
		unsigned int k = to_uInt32(&currentmsg[mc], &len);
		*(unsigned int*)&(outputbuffer->values[ovc]) = k;
		*(int*)& (outputbuffer->ids[otc - 1]) = in->temp->fieldids[tc - 1];
		ovc += 4;
		

	}
	if ((field & Int32))
	{
		int k = to_Int32(&currentmsg[mc], &len);
		*(int*)&(outputbuffer->values[ovc]) = k;
		*(int*)& (outputbuffer->ids[otc - 1]) = in->temp->fieldids[tc - 1];
		ovc += 4;
		
	}
	if ((field & uInt64))
	{
		unsigned long long int k = to_uInt64(&currentmsg[mc], &len);
		*(unsigned long long int*)&(outputbuffer->values[ovc]) = k;
		*(int*)& (outputbuffer->ids[otc - 1]) = in->temp->fieldids[tc - 1];
		ovc += 8;
		
	}
	if ((field & Int64))
	{
		long long int k = to_Int64(&currentmsg[mc], &len);
		*(long long int*)&(outputbuffer->values[ovc]) = k;
		*(int*)& (outputbuffer->ids[otc - 1]) = in->temp->fieldids[tc - 1];
		ovc += 8;
		
	}
	if ((field & STRING))
	{
		
		to_string(&currentmsg[mc], &(outputbuffer->values[ovc]), &len);
		*(int*)& (outputbuffer->ids[otc - 1]) = in->temp->fieldids[tc - 1];
		ovc += len+1;
		
	}
	if ((field & SEQ_END))
	{
		*(int*)& (outputbuffer->ids[otc - 1]) = SEQ_END_ID;
	}
	if ((field & SEQUENCE))
	{
		int len_num_exmp;
		int num_exmp = to_uInt32(&currentmsg[mc], &len_num_exmp);
		mc += len_num_exmp;
		*(int*)& (outputbuffer->ids[otc - 1]) = SEQ_NUM_EXMP;
		*(int*)& (outputbuffer->types[otc - 1]) = SEQ_NUM_EXMP; 
		otc++;
		*(int*)& (outputbuffer->values[ovc]) = num_exmp;
		ovc += 4;
		int last_tc = tc;
		for (int i = num_exmp; i > 0; i--) {
			tc = last_tc;
			inPut newin;
			newin.message = currentmsg;
			newin.msg_length = length;
			newin.pmap = &currentmsg[mc];
			newin.temp = temp;
			if (field & GROUPPMAP)
				mc += get_field_length(&currentmsg[mc]);
			*(int*)& (outputbuffer->ids[otc - 1]) = SEQUENCE_ID;
			process_sequence(&newin, outputbuffer, &tc, &mc, &otc, &ovc);
			if (i != 1) otc++;
		}
	}
	mc += len;
	
}
void FASTsequence::process_sequence(inPut* in, outPut* out, int* tco, int* mco, int* otco, int* ovco)
{

}
void FASTsequence::Process(inPut* in, outPut* out, int* tco, int* mco, int *otco, int *ovco)
{
	currentmsg = in->message;
	length = in->msg_length;
	outputbuffer = out;
	temp = in->temp;
	pmap = in->pmap;
	pmap_len = get_field_length(pmap);
	mc = *mco; //мы все эти курсоры просто перекидываем из одного класса в другой
	pc = 0; 
	tc = *tco;
	if (temp->temp[tc] == 0x20) tc++;
	otc = *otco;
	ovc = *ovco;
	stepup(in);
	*mco = mc;
	*tco = tc;
	*otco = otc;
	*ovco = ovc;
}
