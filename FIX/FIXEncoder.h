#pragma once
#include "..//FAST/FASTDecoder.h"
#include <cmath>
#include <ctime>
typedef long long UTCTimestamp;
class FIXEncoder
{
	string _targetCompID, _senderCompID;
public:
	FIXEncoder(string TargetCompID, string SenderCompID) : _targetCompID(TargetCompID), _senderCompID(SenderCompID) {};
	string Process(FeatherFAST::Message& msg, int MsgSeqNum, string MsgType, UTCTimestamp SendingTime,
				   UTCTimestamp OrigSendingTime, bool PossDupFlag, bool PossResend);
};