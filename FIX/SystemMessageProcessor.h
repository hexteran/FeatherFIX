#pragma once
#include "..\FAST\FASTDecoder.h"
#include "FIXEncoder.h"
#include "TCPClient.h"
#include "fstream"
#include "string"
#define NO_TYPE_FIELD 110
#define SentMessagesPool_ERROR 800
namespace FeatherFIX
{
	class SentMessagesPool {
	private:
		string _pool;
		string _filename;
	public:
		SentMessagesPool(string filename) : _filename(filename) {};
		string load_message(int seqnum); //в конфиг
		void remove_pool();
		void write_message(int seqnum, string message);
	};
	class SystemMessageProcessor
	{
	private:
		TCPClient & _tcpc;
		FIXEncoder & _encoder;

		int& SeqNum;
		SentMessagesPool& _smpool;
		FeatherFAST::Message& onLogon();
		FeatherFAST::Message& onLogout();
		FeatherFAST::Message& onHeartBeat();
		void onResendRequest(int NoBegin, int NoEnd);
		FeatherFAST::Message& onTestRequest(int TestReqId);
		FeatherFAST::Message& onSequenceReset();
		FeatherFAST:: Message& onReject();
	public:
		SystemMessageProcessor(int& seqnum, FIXEncoder& encoder, TCPClient& tcp, SentMessagesPool& smpool) :
		SeqNum(seqnum), _encoder(encoder), _tcpc(tcp), _smpool(smpool){};
		void Send_Message(FeatherFAST::Message& msg, string MsgType, //добавить MsgType в Message 
			UTCTimestamp SendingTime, UTCTimestamp OrigSendingTime, bool PossDupFlag, bool PossResend);
		FeatherFAST::Message& Process(FeatherFAST::Message& message);
	};
};