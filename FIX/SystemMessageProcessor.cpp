#include "SystemMessageProcessor.h"

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
	if (!strcmp(input, "2")) { return onResendRequest(); };
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
