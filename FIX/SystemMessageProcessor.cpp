#include "SystemMessageProcessor.h"

void FeatherFIX::SystemMessageProcessor::Send_Message(FeatherFAST::Message& msg, string MsgType, UTCTimestamp SendingTime, UTCTimestamp OrigSendingTime, bool PossDupFlag, bool PossResend)
{
	time_t timed = time(NULL);
	string message = _encoder.Process(msg, SeqNum, MsgType, SendingTime, OrigSendingTime, PossDupFlag, PossResend);
	cout << "OUTPUT::" << message << endl << endl;
	_tcpc.Send(message.c_str(), message.length() + 1);
	SeqNum++; //есть смысл сделать прибавление счетчика по выполнению условия
}

FeatherFAST::Message& FeatherFIX::SystemMessageProcessor::Process(FeatherFAST::Message& message)
{
	char* input = message.find(35);
	if (input == nullptr) throw NO_TYPE_FIELD;
	if (!strcmp(input, "A")) { return onLogon(); };
	if (!strcmp(input, "5")) { return onLogout(); };
	if (!strcmp(input, "0")) { return onHeartBeat(); };
	if (!strcmp(input, "1"))
	{
		int* input = (int*)message.find(34);
		if (input == nullptr) throw NO_TYPE_FIELD;
		return onTestRequest(*input);
	};
	if (!strcmp(input, "2")) {
		char* input1 = message.find(7);
		char* input2 = message.find(16);
		if ((input1 == nullptr)||(input2 == nullptr)) throw NO_TYPE_FIELD;
		return onResendRequest(stoi(input1),stoi(input2));
	};
	if (!strcmp(input, "4")) { return onSequenceReset(); };
	if (!strcmp(input, "3")) { return onReject(); };
}


FeatherFAST::Message& FeatherFIX::SystemMessageProcessor::onLogon()
{
	FeatherFAST::Message mess;
	return mess;
}

FeatherFAST::Message& FeatherFIX::SystemMessageProcessor::onLogout()
{
	FeatherFAST::Message mess;
	return mess;
}

FeatherFAST::Message& FeatherFIX::SystemMessageProcessor::onHeartBeat()
{
	FeatherFAST::Message mess;
	return mess;
}

void FeatherFIX::SystemMessageProcessor::onResendRequest(int NoBegin, int NoEnd)
{
	for (int i = NoBegin; i <= NoEnd; i++)
	{
		string message = _smpool.load_message(i);
		//делаем новый заголовок и трейлер
	}
}

FeatherFAST::Message& FeatherFIX::SystemMessageProcessor::onResendRequest()
{
	FeatherFAST::Message mess;
	
	return mess;
}

FeatherFAST::Message& FeatherFIX::SystemMessageProcessor::onSequenceReset()
{
	FeatherFAST::Message mess;
	return mess;
}

FeatherFAST::Message& FeatherFIX::SystemMessageProcessor::onReject()
{
	FeatherFAST::Message mess;
	return mess;
}

FeatherFAST::Message& FeatherFIX::SystemMessageProcessor::onTestRequest(int TestReqId)
{
	FeatherFAST::Message mess;
	mess.add_field((char*)& TestReqId, 112, Int32);
	return mess;
}
string FeatherFIX::SentMessagesPool::load_message(int seqnum)
{
	ifstream File(_filename);
	char current_line[15384];

	while (File.getline(current_line, 15384))
	{
		int i;
		for (i = 0; current_line[i] != ' '; i++)
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
		catch (int e)
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