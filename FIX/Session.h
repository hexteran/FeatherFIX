#pragma once
#include <thread>
#include "FIXDecoder.h"
#include "FIXEncoder.h"
#include "TCPClient.h"
#include "SystemMessageProcessor.h"
#include <iostream>
#include <fstream>

#define system_message true
#define user_message false

namespace FeatherFIX
{
	class UserInputProc //должен быть класс-наследник, который обрабатывает поступившее пользовательское сообщение
	{
	private:
		int len = 10485;
		char buf[10485];
	public:
		bool process(FeatherFAST::Message& message);
		virtual void write_to_users() { cout << "llk"; };
		virtual char* buffer() { return buf; };
		virtual int buffer_len() { return len; };
	};
	/*
	class SentMessagesPool {
	private:
		string _pool;
		string _filename;
	public:
		SentMessagesPool(string filename): _filename(filename) {};
		string load_message(int seqnum); //в конфиг
		void remove_pool();
		void write_message(int seqnum, string message);
	};
	*/
	class Session
	{
	private:
		string _targetCompID;
		string _senderCompID;
		SentMessagesPool _smpool;
		TCPClient socket;
		UserInputProc& destination; //проверить наследование
		FIXDecoder decoder;
		FIXEncoder encoder;
		SystemMessageProcessor sysproc;
		bool hearing_stop;
		void Hearing();
		int inner_Seq_Num, outer_Seq_Num; //проверять
	public:
		Session(string TargetCompID, string SenderCompID, UserInputProc& listener, int beg_seq_num = 1, string smpoolname = "smpool.txt") :
			destination(listener), encoder(TargetCompID, SenderCompID),
			_targetCompID(TargetCompID), _senderCompID(SenderCompID),
			sysproc(inner_Seq_Num, encoder, socket, _smpool), inner_Seq_Num(beg_seq_num),
			_smpool(smpoolname)
		{
			destination.write_to_users();
		};
		void Connect(string Addr, string Port);
		void Send_Message(FeatherFAST::Message& msg, string MsgType, //добавить MsgType в Message 
			UTCTimestamp SendingTime, UTCTimestamp OrigSendingTime, bool PossDupFlag, bool PossResend);
		void Start_Hearing();
		void Stop_Hearing();
	};

	class BasicUserOutput
	{
		Session& session;
		//string& account, tradingSessionId;
	public:
		BasicUserOutput(Session& BasicSession) : session(BasicSession) {};
		virtual FeatherFAST::Message Order_New(string OrdID, string OrdType, string Price, string Side, string Qty, string Symbol, string Secboard);
		FeatherFAST::Message Order_Cancel();
		FeatherFAST::Message Order_CancelOrReplace();
		FeatherFAST::Message Order_MassCancel;
	};
}