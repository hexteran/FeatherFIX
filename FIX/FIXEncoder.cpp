#include "FIXEncoder.h"

string FIXEncoder::Process(FeatherFAST::Message& msg, int MsgSeqNum, string MsgType, UTCTimestamp SendingTime, UTCTimestamp OrigSendingTime, bool PossDupFlag, bool PossResend)
{
	
	string body = "";
	body += "35=" + MsgType + '\001';
	body += "49=" + _senderCompID + '\001';
	body += "56=" + _targetCompID + '\001';
	body += "34=" + to_string(MsgSeqNum) + '\001';

	if (PossDupFlag == true) body += "43=" + 'Y' + '\001';
	if (PossResend == true) body += "97=" + 'Y' + '\001';

	tm* t = localtime(&SendingTime);
	string Time = to_string(t->tm_year + 1900);
	if (t->tm_mon < 10)
		Time += "0";
	Time += to_string(t->tm_mon);
	if (t->tm_mday < 10)
		Time += "0";
	Time += to_string(t->tm_mday);
	Time += '-';
	if (t->tm_hour < 10)
		Time += "0";
	Time += to_string(t->tm_hour);
	if (t->tm_min < 10)
		Time += "0";
	Time += to_string(t->tm_min);
	if (t->tm_sec < 10)
		Time += "0";
	Time += to_string(t->tm_sec);
	Time += ".000000";
	body += "52=" + Time + "\001";
	if (OrigSendingTime != 0)
	{
		tm* t = localtime(&OrigSendingTime);
		string Time = to_string(t->tm_year + 1900);
		if (t->tm_mon < 10)
			Time += "0";
		Time += to_string(t->tm_mon);
		if (t->tm_mday < 10)
			Time += "0";
		Time += to_string(t->tm_mday);
		Time += '-';
		if (t->tm_hour < 10)
			Time += "0";
		Time += to_string(t->tm_hour);
		if (t->tm_min < 10)
			Time += "0";
		Time += to_string(t->tm_min);
		if (t->tm_sec < 10)
			Time += "0";
		Time += to_string(t->tm_sec);
		Time += ".000000";
		body += "112=" + Time + "\001";
	}
	while (msg.is_end()!=true)
	{
		body += to_string(msg.field_id()) + "=";
		if (msg.type() & Int32)
		{
			body += to_string(*(int*)msg.value())+"\001";
			msg.next();
			continue;
		}
		if (msg.type() & uInt32)
		{
			body += to_string(*(unsigned int*)msg.value()) + "\001";
			msg.next();
			continue;
		}
		if (msg.type() & uInt64)
		{
			body += to_string(*(unsigned long long*)msg.value()) + "\001";
			msg.next();
			continue;
		}
		if (msg.type() & Int64)
		{
			body += to_string(*(long long*)msg.value()) + "\001";
			msg.next();
			continue;
		}
		if (msg.type() & (STRING|CONSTANT))
		{
			body += msg.value();
			body += "\001";
			msg.next();
			continue;
		}
		if (msg.type() & BYTE_VECTOR) msg.next(); //добавить обработку
	}
	string Result = "8=FIX.4.4\0019="+to_string(body.length())+'\001'+body;

	const char* str = Result.c_str();
	int len = Result.length();
	int sum = 0;
	for (int i = 0; i < len; i++)
	{
		sum += (int)str[i];
	}
	sum = sum % 256;
	if (sum > 99) Result += "10=" + to_string(sum)+'\001';
	if (sum < 10) Result += "10=00" + to_string(sum) + '\001';
	if ((sum>=10)&&(sum <= 99)) Result += "10=0" + to_string(sum) + '\001';
	return Result;
}
