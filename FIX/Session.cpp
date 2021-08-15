#include "Session.h"

void FeatherFIX::Session::Hearing()
{
	char* buffer = destination.buffer();
	int buffer_len = destination.buffer_len();
	while (hearing_stop == false)
	{
		int num_symbols = socket.Receive(buffer, buffer_len);
		if (num_symbols != -1)
		{
			FeatherFAST::Message mes = decoder.Process(buffer, num_symbols);
			if (destination.process(mes) == system_message) sysproc.Process(mes);
			
		};
	}
}

void FeatherFIX::Session::Connect(string Addr, string Port)
{
	socket.Connect(Addr.c_str(), Port.c_str());
}

void FeatherFIX::Session::Send_Message(FeatherFAST::Message& msg, string MsgType, 
	UTCTimestamp SendingTime, UTCTimestamp OrigSendingTime, bool PossDupFlag, bool PossResend)
{
	time_t timed = time(NULL);
	string message = encoder.Process(msg, inner_Seq_Num, MsgType, SendingTime, OrigSendingTime, PossDupFlag, PossResend);
	cout <<"OUTPUT::"<< message<< endl <<endl;
	socket.Send(message.c_str(), message.length()+1);
	_smpool.write_message(inner_Seq_Num, message);
	inner_Seq_Num++; //есть смысл сделать прибавление счетчика по выполнению условия
}

void FeatherFIX::Session::Start_Hearing()
{
	hearing_stop = false;
	thread New_Ear(&Session::Hearing,this);
	New_Ear.detach();
}

void FeatherFIX::Session::Stop_Hearing()
{
	hearing_stop = true;
}

bool FeatherFIX::UserInputProc::process(FeatherFAST::Message& message)
{
	char* A = message.find(35);
	if (A == nullptr) {
		throw NO_TYPE_FIELD;
		return user_message;
	}
	if ((strcmp(A, "5")) || (strcmp(A, "A"))
		|| (strcmp(A, "0")) || (strcmp(A, "1"))
		|| (strcmp(A, "2")) || (strcmp(A, "4"))
		|| (strcmp(A, "3"))) {
		return system_message;
	}
	write_to_users();
	return user_message;
}

FeatherFAST::Message FeatherFIX::BasicUserOutput::Order_New(string OrdID, string OrdType, string Price, string Qty, string Side,
	string Symbol, string Secboard)
{
	FeatherFAST::Message mes;
	mes.add_field((char*)OrdID.c_str(), 11, STRING);
	//mes.add_field((char*)account.c_str(), 1, STRING);
	mes.add_field((char*)"1", 386, STRING);
	//mes.add_field((char*)tradingSessionId.c_str(), 336, STRING);
	mes.add_field((char*)Symbol.c_str(), 55, STRING);
	mes.add_field((char*)Side.c_str(), 54, STRING);
	time_t timed = time(NULL);
	tm* t = localtime(&timed);
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
	mes.add_field((char*)Time.c_str(), 60, STRING);
	mes.add_field((char*)Qty.c_str(), 38, STRING);
	mes.add_field((char*)OrdType.c_str(), 40, STRING);
	mes.add_field((char*)Price.c_str(), 44, STRING);
	session.Send_Message(mes, "D", timed, 0, false, false);
	return mes;
}

FeatherFAST::Message FeatherFIX::BasicUserOutput::Order_Cancel()
{
	// TODO: вставьте здесь оператор return
	FeatherFAST::Message mes;
	return mes;
}

FeatherFAST::Message FeatherFIX::BasicUserOutput::Order_CancelOrReplace()
{
	// TODO: вставьте здесь оператор return
	FeatherFAST::Message mes;
	return mes;
}
/*
string FeatherFIX::SentMessagesPool::load_message(int seqnum)
{
	ifstream File(_filename);
	char current_line[15384];
	
	while (File.getline(current_line, 15384))
	{
		int i; 
		for (i = 0; current_line[i]!=' '; i++)
		{
			if (i >= 15384) {
				throw SentMessagesPool_ERROR;
				break;
			}
		}
		current_line[i] = 0;
		try {
			int number = atoi(current_line);
			string result(&current_line[i + 1]);
			if (number == seqnum) return result;
		}
		catch(int e)
		{
			throw SentMessagesPool_ERROR;
		}

	}
	File.close();
	return string("");
}

void FeatherFIX::SentMessagesPool::remove_pool()
{
	remove(_filename.c_str());
}

void FeatherFIX::SentMessagesPool::write_message(int seqnum, string message)
{
	ofstream File;
	try
	{
		File.open(_filename.c_str(), ios::app);
		string lsd = (to_string(seqnum) + " " + message + "\n");
		File << lsd;
	}
	catch (int e)
	{
		throw SentMessagesPool_ERROR;
	}
	File.close();
}
*/