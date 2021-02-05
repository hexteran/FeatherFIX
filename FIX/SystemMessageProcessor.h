#pragma once
#include "..\FAST\FASTDecoder.h"
#define NO_TYPE_FIELD 110
namespace FeatherFIX
{
	class SystemMessageProcessor
	{
	private:
		int& SeqNum;
		FeatherFAST::Message& onLogon();
		FeatherFAST::Message& onLogout();
		FeatherFAST::Message& onHeartBeat();
		FeatherFAST::Message& onResendRequest();
		FeatherFAST::Message& onTestRequest(int TestReqId);
		FeatherFAST::Message& onSequenceReset();
		FeatherFAST::Message& onReject();
	public:
		SystemMessageProcessor(int& seqnum) : SeqNum(seqnum) {};
		FeatherFAST::Message& Process(FeatherFAST::Message& message);
	};
};