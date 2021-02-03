#pragma once
#include <iostream>
#include "FIX.h"
using namespace std;
void FIX44::Listening()
{
	char buffer[RLEN];
	while (true)
	{
		int TCPResult = TCP.Receive((PCSTR)buffer, InputContainer);
		if (TCPResult == -1) {
			isConnected = false;
			throw 9;
			break;
		};
		if (InputContainer == ending_comb) break;
		InputContainer = buffer;
		int i = 0;
		char* pointer = buffer;
		while (strlen(buffer)!=0)
		{
			char* end = strstr(&pointer[1], "8=FIX.4.4"); 
			if (end != NULL)
			{
				end[0] = '\0';
				InputContainer = pointer;
				end[0] = '8';
				pointer = end;
			}
			else
			{
				InputContainer = pointer;
				buffer[0] = '\0';
			}
			if (SysVars.logflag == true) SysVars.LOGGING << "IN: " << InputContainer << endl;
			cout << InputContainer << endl;
			In.Receive(InputContainer);
		}
	}
	return;
}

bool FIX44::is_connected()
{
	return isConnected;
}

short FIX44::Logon(string Password, string HeartBtInt = "2", string ResetSeqNum = "Y", string NewPassword = "", string LanguageID = "E")
{
	InputContainer = "";
	Out.Logon(Password, HeartBtInt, ResetSeqNum, NewPassword, LanguageID);
	isConnected = true;
	thread listener(&FIX44::Listening, this);
	listener.detach();
	return  0;
}

short FIX44::Logout(string Text = NOL)
{
	InputContainer = ending_comb;
	Out.Logout(Text);
	return 0;
}

short FIX44::ResendRequest(ResendReq* Arguments)
{
	Out.ResendRequest(Arguments);
	return 0;
}

short FIX44::NewOrder_Single(NewOrder_sngl* Arguments)
{
	Out.NewOrder_Single(Arguments); 
	return 0;
}

short FIX44::OrderCancelRequest(OrderCancel* Arguments)
{
	Out.OrderCancelRequest(Arguments);
	return 0;
}

vector<ExecutionReportTable>* FIX44::Get_OrdMan_History(string ClOrdID)
{
	return OrdMan.Get_History(ClOrdID);
}

OrderTable FIX44::LastChangedOrder()
{
	return OrdMan.GetChanged();
}
RejectedRequest FIX44::LastRejectedCancel()
{
	return OrdMan.GetReject();
}

void FIX44::FIX44Init(string senderCompID, string targetCompID, string Address, string Port)
{
	isConnected = false;
	if (SysVars.logflag == true) SysVars.LOGGING.open(SysVars.LOGNAME, ios_base::out);
	TCP.initial((PCSTR)& Address, (PCSTR)& Port);
	InputContainer = "";
	Formatter.MessageFormatterInit(senderCompID, targetCompID);
	Out.OutputProcessorInit(&Formatter, &TCP);
	In.InputProcessorInit(&Out, &OrdMan);
}

void FIX44::Reconnect(string address, string port)
{
	TCP.initial((PCSTR)& address, (PCSTR)& port);
}


string MessageFormatter::Linking(list<string>* BodyOrder = 0)
{
	string Message = "";
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

	Heads["52"] = Time;
	Heads["52"] += (char)0x01;
	string Header_priority[] = { "35","49","56","34","43","97","52","122" };
	for (string a : Header_priority)
	{
		if (Heads[a] == NOL) continue;
		Message += a + "=" + Heads[a];
		Heads[a] = NOL;
	}

	if (BodyOrder != 0) {
		for (string a : *BodyOrder)
		{
			if (Heads[a] == NOL) continue;
			Message += a + "=" + Heads[a];
			Heads[a] = NOL;
		}
	}

	for (map<string, string>::iterator iter = Heads.begin(); iter != Heads.end(); ++iter) {
		if (iter->second == NOL) continue;
		Message += iter->first + "=" + iter->second;

	}
	Heads["8"] = "FIX.4.4";
	Heads["8"] += (char)0x01;
	Heads["9"] = to_string(Message.length());
	Heads["9"] += (char)0x01;
	
	Message = "8" + (string)"=" + Heads["8"] + "9" + "=" + Heads["9"] + Message;

	int check = 0;
	for (int i = 0; i < Message.length(); i++) {
		check += (int)Message[i];
	}
	check = check % 256;
	Heads["10"] = to_string(check) + (char)0x01;
	if (check < 10) {
		Heads["10"] = "00" + Heads["10"];
	}
	else if (check < 100) {
		Heads["10"] = "0" + Heads["10"];
	}
	Message += "10" + (string)"=" + Heads["10"];
	
	return Message;
}

void MessageFormatter::MessageFormatterInit(string senderCompID, string targetCompID)
{
	SenderCompID = senderCompID;
	TargetCompID = targetCompID;

}

string MessageFormatter::Logon(string Password, int SeqNum, string HeartBtInt = "2", string ResetSeqNum = "N", string NewPassword = "", string LanguageID = "E")
{
	Load(SeqNum);
	Heads["35"] = "A";
	Heads["35"] += (char)0x01;
	Heads["98"] = "0";
	Heads["98"] += (char)0x01;
	Heads["108"] = HeartBtInt;
	Heads["108"] += char(0x01);
	Heads["141"] = ResetSeqNum;
	Heads["141"] += char(0x01);
	Heads["554"] = Password;
	Heads["554"] += char(0x01);
	if (NewPassword != "") {
		Heads["925"] = NewPassword;
		Heads["925"] += char(0x01);
	}
	if (LanguageID != "") {
		Heads["6936"] = LanguageID;
		Heads["6936"] += char(0x01);
	}
	return Linking();
}

string MessageFormatter::HeartBeat(int SeqNum, string TestReqID = NOL)
{
	Load(SeqNum);
	Heads["35"] = "0";
	Heads["35"] += (char)0x01;
	Heads["112"] = TestReqID;
	Heads["112"] += (char)0x01;
	return Linking();
}

void MessageFormatter::Load(int SeqNum)
{
	Heads["8"] = NOL;
	Heads["9"] = NOL;
	Heads["35"] = NOL;
	Heads["49"] = SenderCompID + (char)0x01;
	Heads["56"] = TargetCompID + (char)0x01;
	Heads["34"] = to_string(SeqNum) + (char)0x01;
	Heads["43"] = NOL;
	Heads["97"] = NOL;
	Heads["52"] = NOL;
	Heads["122"] = NOL;
	Heads["55"] = NOL;
	Heads["460"] = NOL;
	Heads["461"] = NOL;
	Heads["167"] = NOL;
	Heads["236"] = NOL;
	Heads["453"] = NOL;
	Heads["448"] = NOL;
	Heads["447"] = NOL;
	Heads["452"] = NOL;
	Heads["12"] = NOL;
	Heads["13"] = NOL;
	Heads["38"] = NOL;
	Heads["152"] = NOL;
	Heads["768"] = NOL;
	Heads["769"] = NOL;
	Heads["770"] = NOL;
	Heads["916"] = NOL;
	Heads["917"] = NOL;
	Heads["919"] = NOL;
	Heads["311"] = NOL;
	Heads["887"] = NOL;
	Heads["888"] = NOL;
	Heads["889"] = NOL;
	Heads["98"] = NOL;
	Heads["108"] = NOL;
	Heads["141"] = NOL;
	Heads["554"] = NOL;
	Heads["925"] = NOL;
	Heads["1409"] = NOL;
	Heads["6867"] = NOL;
	Heads["6936"] = NOL;
	Heads["58"] = NOL;
	Heads["112"] = NOL;
	Heads["7"] = NOL;
	Heads["16"] = NOL;
	Heads["123"] = NOL;
	Heads["36"] = NOL;
	Heads["45"] = NOL;
	Heads["371"] = NOL;
	Heads["372"] = NOL;
	Heads["373"] = NOL;
	Heads["11"] = NOL;
	Heads["1"] = NOL;
	Heads["111"] = NOL;
	Heads["526"] = NOL;
	Heads["386"] = NOL;
	Heads["336"] = NOL;
	Heads["54"] = NOL;
	Heads["60"] = NOL;
	Heads["40"] = NOL;
	Heads["423"] = NOL;
	Heads["44"] = NOL;
	Heads["5202"] = NOL;
	Heads["59"] = NOL;
	Heads["168"] = NOL;
	Heads["528"] = NOL;
	Heads["529"] = NOL;
	Heads["1090"] = NOL;
	Heads["10"] = NOL;
}

string MessageFormatter::TestRequest(int SeqNum, string TestReqID)
{
	Load(SeqNum);
	Heads["35"] = "1";
	Heads["35"] += (char)0x01;
	Heads["112"] = TestReqID;
	Heads["112"] += (char)0x01;
	return Linking();
}

string MessageFormatter::ResendRequest(int SeqNum, ResendReq* Arguments)
{
	Load(SeqNum);
	Heads["35"] = "2\x1";
	Heads["7"] = Arguments->BeginSecNo+"\x1";
	Heads["16"] = Arguments->EndSecNo+"\x1";
	return Linking();
}

string MessageFormatter::Logout(int SeqNum, string Text = NOL)
{
	Load(SeqNum);
	Heads["35"] = "5\x1";
	Heads["58"] = Text;
	if (Text != NOL) Heads["58"] = (char)0x01;
	return Linking();

}

string MessageFormatter::NewOrder_Single(int SeqNum, NewOrder_sngl* Arguments)
{
	/*
	list<string> BodyOrder = { "11","453","448","447","452","1","111",
		"526","386","336","55","460","461","167","54","60","38",
		"152","40","423","44","5202","59","168","528","529","1090" };
	Load(SeqNum);
	Heads["35"] = "D\x1";
	Heads["11"] = Arguments->ClOrdID;
	Heads["1"] = Arguments->Account;
	Heads["111"] = Arguments->MaxFloor;
	Heads["526"] = Arguments->SecondaryClOrdID;
	Heads["386"] = Arguments->NoTradingSessions;
	Heads["336"] = Arguments->TradingSessionID;
	Heads["54"] = Arguments->Side;
	Heads["40"] = Arguments->OrdType;
	Heads["423"] = Arguments->PriceType;
	Heads["44"] = Arguments->Price;
	Heads["5202"] = Arguments->TradeThruTime;
	Heads["59"] = Arguments->TimeInForce;
	Heads["168"] = Arguments->EffectiveTime;
	Heads["528"] = Arguments->OrderCapacity;
	Heads["529"] = Arguments->OrderRestrictions;
	Heads["1090"] = Arguments->MaxPriceLevels;

	Heads["453"] = Arguments->Party.NoPartyID;
	Heads["448"] = Arguments->Party.PartyID;
	Heads["447"] = Arguments->Party.PartyIDSource;
	Heads["452"] = Arguments->Party.PartyRole;

	Heads["55"] = Arguments->Tool.Symbol;
	Heads["460"] = Arguments->Tool.Product;
	Heads["461"] = Arguments->Tool.CFICode;
	Heads["167"] = Arguments->Tool.SecurityType;

	Heads["38"] = Arguments->OrdQty.OrderQty;
	Heads["152"] = Arguments->OrdQty.CashOrderQty;

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

	Heads["60"] = Time;
	Heads["60"] += (char)0x01;
	
	if (Heads["11"] != NOL) Heads["11"] += "\x1";
	if (Heads["1"] != NOL) Heads["1"] += "\x1";
	if (Heads["111"] != NOL) Heads["111"] += "\x1";
	if (Heads["526"] != NOL) Heads["526"] += "\x1";
	if (Heads["386"] != NOL) Heads["386"] += "\x1";
	if (Heads["336"] != NOL) Heads["336"] += "\x1";
	if (Heads["54"] != NOL) Heads["54"] += "\x1";
	if (Heads["40"] != NOL) Heads["40"] += "\x1";
	if (Heads["423"] != NOL) Heads["423"] += "\x1";
	if (Heads["44"] != NOL) Heads["44"] += "\x1";
	if (Heads["5202"] != NOL) Heads["5202"] += "\x1";
	if (Heads["59"] != NOL) Heads["59"] += "\x1";
	if (Heads["168"] != NOL) Heads["168"] += "\x1";
	if (Heads["528"] != NOL) Heads["528"] += "\x1";
	if (Heads["529"] != NOL) Heads["529"] += "\x1";
	if (Heads["1090"] != NOL) Heads["1090"] += "\x1";

	if (Heads["453"] != NOL) Heads["453"] += "\x1";
	if (Heads["448"] != NOL) Heads["448"] += "\x1";
	if (Heads["447"] != NOL) Heads["447"] += "\x1";
	if (Heads["452"] != NOL) Heads["452"] += "\x1";

	if (Heads["55"] != NOL) Heads["55"] += "\x1";
	if (Heads["460"] != NOL) Heads["460"] += "\x1";
	if (Heads["461"] != NOL) Heads["461"] += "\x1";
	if (Heads["167"] != NOL) Heads["167"] += "\x1";

	if (Heads["38"] != NOL) Heads["38"] += "\x1";
	if (Heads["152"] != NOL) Heads["152"] += "\x1";

	cout<<Linking(&BodyOrder)<<endl<<endl;
	*/
	string SecondHalf = "35=D\x1";
	SecondHalf += "49=" + SenderCompID + (char)0x01;
	SecondHalf += "56=" + TargetCompID + (char)0x01;
	SecondHalf += "34=" + to_string(SeqNum) + (char)0x01;

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
	SecondHalf += "52=" + Time + (char)0x01;
	SecondHalf += "11=" + Arguments->ClOrdID + (char)0x01;
	if (Arguments->Party.NoPartyID != NOL) SecondHalf += "453=" + Arguments->Party.NoPartyID + (char)0x01;
	if (Arguments->Party.PartyID != NOL) SecondHalf += "448=" + Arguments->Party.PartyID + (char)0x01;
	if (Arguments->Party.PartyIDSource != NOL) SecondHalf += "447=" + Arguments->Party.PartyIDSource + (char)0x01;
	if (Arguments->Party.PartyRole != NOL) SecondHalf += "452=" + Arguments->Party.PartyRole + (char)0x01;

	if (Arguments->Account != NOL) SecondHalf += "1=" + Arguments->Account + (char)0x01;
	if (Arguments->MaxFloor != NOL) SecondHalf += "111=" + Arguments->MaxFloor + (char)0x01;
	if (Arguments->SecondaryClOrdID != NOL) SecondHalf += "526=" + Arguments->SecondaryClOrdID + (char)0x01;
	SecondHalf += "386=" + Arguments->NoTradingSessions + (char)0x01;
	SecondHalf += "336=" + Arguments->TradingSessionID + (char)0x01;
	if (Arguments->Tool.Symbol != NOL) SecondHalf += "55=" + Arguments->Tool.Symbol + (char)0x01;
	if (Arguments->Tool.Product != NOL) SecondHalf += "460=" + Arguments->Tool.Product + (char)0x01;
	if (Arguments->Tool.CFICode != NOL) SecondHalf += "461=" + Arguments->Tool.CFICode + (char)0x01;
	if (Arguments->Tool.SecurityType != NOL) SecondHalf += "167=" + Arguments->Tool.SecurityType + (char)0x01;
	SecondHalf += "54=" + Arguments->Side + (char)0x01;
	SecondHalf += "60=" + Time + (char)0x01;
	if (Arguments->OrdQty.OrderQty != NOL) SecondHalf += "38=" + Arguments->OrdQty.OrderQty + (char)0x01;
	if (Arguments->OrdQty.CashOrderQty != NOL) SecondHalf += "152=" + Arguments->OrdQty.CashOrderQty + (char)0x01;
	SecondHalf += "40=" + Arguments->OrdType + (char)0x01;
	if (Arguments->PriceType != NOL) SecondHalf += "423=" + Arguments->PriceType + (char)0x01;
	SecondHalf += "44=" + Arguments->Price + (char)0x01;
	if (Arguments->TradeThruTime != NOL) SecondHalf += "5202=" + Arguments->TradeThruTime + (char)0x01;
	if (Arguments->TimeInForce != NOL) SecondHalf += "59=" + Arguments->TimeInForce + (char)0x01;
	if (Arguments->EffectiveTime != NOL) SecondHalf += "168=" + Arguments->EffectiveTime + (char)0x01;
	if (Arguments->OrderCapacity != NOL) SecondHalf += "528=" + Arguments->OrderCapacity + (char)0x01;
	if (Arguments->OrderRestrictions != NOL) SecondHalf += "529=" + Arguments->OrderRestrictions + (char)0x01;
	if (Arguments->MaxPriceLevels != NOL) SecondHalf += "1090=" + Arguments->MaxPriceLevels + (char)0x01;
	string Message = " 8=FIX.4.4" + (char)0x01;
	Message+="9=" + to_string(SecondHalf.length()) + (char)0x01;
	//Message = Message + "9=" + to_string(SecondHalf.length()) + (char)0x01;
	Message += SecondHalf;

	int check = 0;
	for (int i = 0; i < Message.length(); i++) {
		check += (int)Message[i];
	}
	check = check % 256;

	Message += "10=";
	if (check < 10) {
		Message += "00" + to_string(check) + (char)(0x01);
	}
	else if (check < 100) {
		Message += "0" + to_string(check) + (char)(0x01);
	}
	else Message += to_string(check) + (char)(0x01);
	return Message;
}

string MessageFormatter::OrderCancelRequest(int SeqNum, OrderCancel* Arguments)
{
	/*
	list<string> BodyOrder = { "41","37","11","54","60" };
	Load(SeqNum);
	Heads["35"] = "F\x1";
	Heads["41"] = Arguments->OrigClOrdID;
	Heads["37"] = Arguments->OrderID;
	Heads["11"] = Arguments->ClOrdID;
	Heads["54"] = Arguments->Side;
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

	Heads["60"] = Time;
	Heads["60"] += (char)0x01;

	if (Heads["41"] != NOL) Heads["41"] += "\x1";
	if (Heads["37"] != NOL) Heads["37"] += "\x1";
	if (Heads["11"] != NOL) Heads["11"] += "\x1";
	if (Heads["54"] != NOL) Heads["54"] += "\x1";
	return Linking(&BodyOrder);*/

	string SecondHalf = "35=F\x1";
	SecondHalf += "49=" + SenderCompID + (char)0x01;
	SecondHalf += "56=" + TargetCompID + (char)0x01;
	SecondHalf += "34=" + to_string(SeqNum) + (char)0x01;

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
	SecondHalf += "52=" + Time + (char)0x01;
	if (Arguments->OrigClOrdID != NOL) SecondHalf += "41=" + Arguments->OrigClOrdID + (char)0x01;
	if (Arguments->OrderID != NOL) SecondHalf += "37=" + Arguments->OrderID + (char)0x01;
	if (Arguments->ClOrdID != NOL) SecondHalf += "11=" + Arguments->ClOrdID + (char)0x01;
	SecondHalf += "54=" + Arguments->Side + (char)0x01;
	SecondHalf += "60=" + Time + (char)0x01;

	string Message = " 8=FIX.4.4" + (char)0x01;
	Message += "9=" + to_string(SecondHalf.length()) + (char)0x01;
	Message += SecondHalf;

	int check = 0;
	for (int i = 0; i < Message.length(); i++) {
		check += (int)Message[i];
	}
	check = check % 256;

	Message += "10=";
	if (check < 10) {
		Message += "00" + to_string(check) + (char)(0x01);
	}
	else if (check < 100) {
		Message += "0" + to_string(check) + (char)(0x01);
	}
	else Message += to_string(check) + (char)(0x01);
	return Message;
}
bool SystemVariablesProcessor::parse(char* str)
{
	string operand = (string)str;
	int k = operand.find("=");
	str[k] = 0;
	string operat = (string)str;
	operand = (string)& str[k+1];
	return choose(operat, operand);
}
bool SystemVariablesProcessor::choose(string a, string b)
{
	if (a == "Log_flag") 
		if (b == "true") { logflag = true; }
	if (a == "Log_path") LOGNAME = b;
	else false;
	return true;
}
SystemVariablesProcessor::SystemVariablesProcessor(string FName)
{
	ifstream file;
	
	file.open(FName);
	LOGNAME = "None";
	logflag = false;
	if (file.fail())
	{
		FName = "None";
		throw 8;
		return;
	}
	else
	{
		char papautei[256];
		int i = 0;
		do {
			file >> papautei[i];
			if (papautei[i] == ';')
			{
				papautei[i] = '\0';
				parse(papautei);
				i = 0;
			}
			else i++;
		} while (!file.eof());
	}
}

bool TCPClient::initial(PCSTR Addr, PCSTR Port) 
{
	int Result = WSAStartup(MAKEWORD(2, 2), &wsDATA);
	if (Result != 0) {
		if (SysVars.logflag == true) SysVars.LOGGING<<"TCPClient::initial::WSAStartup, Error Code: "<<WSAGetLastError()<<endl;
		throw 1;
		return -1;
	}
	ADDRINFOA hint, * resp;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	Result = getaddrinfo(Addr, Port, &hint, &resp);
	if (Result != 0) {
		if (SysVars.logflag == true) SysVars.LOGGING << "TCPClient::initial::getaddrinfo, Error Code: " << WSAGetLastError()<<endl;
		throw 2;
		return -1;
	}
	sock = INVALID_SOCKET;
	sock = socket(resp->ai_family, resp->ai_socktype, resp->ai_protocol);
	if (sock == INVALID_SOCKET) {
		if (SysVars.logflag == true) SysVars.LOGGING << "TCPClient::initial::socket, Error Code: " << WSAGetLastError() << endl;
		throw 3;
		return -1;
	}
	int timeout = 10000; //in milliseconds. this is 10 seconds вот ето вот константа нужно менять
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)& timeout, sizeof(int)); //setting the receive timeout

	Result = connect(sock, resp->ai_addr, (int)resp->ai_addrlen);
	if (Result != 0) {
		if (SysVars.logflag == true) SysVars.LOGGING << "TCPClient::initial::connect, Error Code: " << WSAGetLastError() << endl;
		throw 4;
		return -1;
	}
	return 0;
};
int TCPClient::Send(PCSTR message, int len) {
	int Result = send(sock, message, len, 0);
	LastError = WSAGetLastError();
	if (LastError != 0) {
		if (SysVars.logflag == true) SysVars.LOGGING << "TCPClient::Send::send, Error Code: " << LastError << endl;
		throw 5;
		return -1;
	}
	if (SysVars.logflag == true) SysVars.LOGGING << "OUT: " << message << endl;
	return Result;
}
int TCPClient::Receive(PCSTR output, string& input) {
	const int rlen = 101553;
	char received[rlen];
	int Result = 0;
	if (input!=ending_comb)
		Result = recv(sock, received, rlen, 0);
	if (input == ending_comb)
	{
		strcpy((char*)output, input.c_str());
		return 1;
	}
	LastError = WSAGetLastError();
	if (LastError != 0) {
		if (SysVars.logflag == true) SysVars.LOGGING << "TCPClient::Receive::recv, Error Code: " << LastError << endl;
		throw 6;
		return -1;
	}
	received[Result] = 0;
	//cout << received<<endl<<endl;
	strcpy((char*)output, received);
	return Result;
}

map <string, string> InputProcessor::Parsing()
{
	map <string, string> output;
	for (int i = 0; i < InputString.length(); i++)
	{
		string num = "";
		string data = "";
		while (InputString[i] != '=') {
			num += InputString[i];
			i++;
		}
		i++;
		while (InputString[i] != (char)0x01) {
			data += InputString[i];
			i++;
		}
		output[num] = data;
	}
	return output;
}


short InputProcessor::Distributing()
{
	map <string, string> Heads = Parsing();
	if (Heads.find("35") == Heads.end()) return -1;
	string MsgType = Heads["35"];
	if ((MsgType == "0") || (MsgType == "1") || (MsgType == "A") || (MsgType == "5") || (MsgType == "3"))
	{
		SysInProc.Process(Heads);
	}
	else
	{
		ClInProc.Process(Heads);
	}
	return 0;
}

void InputProcessor::Receive(string& Input)
{
	InputString = Input;
	Distributing();
}

void InputProcessor::InputProcessorInit(OutputProcessor* Out, OrderManager* Ordman)
{
	SysInProc.LoadOutputProcessor(Out);
	ClInProc.LoadOutputProcessor(Out, Ordman);
}

void ClientInputProcessor::ExecutionReport()
{
	ExecutionReportTable Report;
	if (Heads.count("1") != 0) {
		Report.Account = Heads["1"];
	}
	if (Heads.count("41") != 0) {
		Report.OrigClOrdID = Heads["41"];
	}
	Report.ClOrdID = Heads["11"];
	Report.ExecID = Heads["17"];
	Report.ExecType = Heads["150"];
	Report.OrdStatus = Heads["39"];
	if (Heads.count("378") != 0) {
		Report.ExecRestatementReason = Heads["378"];
	}
	if (Heads.count("54") != 0) {
		Report.Side = Heads["54"];
	}
	if (Heads.count("44") != 0) {
		Report.Price = Heads["44"];
	}
	if (Heads.count("32") != 0) {
		Report.LastQty = Heads["32"];
	}
	if (Heads.count("31") != 0) {
		Report.LastPx = Heads["31"];
	}
	if (Heads.count("151") != 0) {
		Report.LeavesQty = Heads["151"];
	}
	if (Heads.count("14") != 0) {
		Report.CumQty = Heads["14"];
	}
	if (Heads.count("58") != 0) {
		Report.Text = Heads["58"];
	}
	if (Heads["39"] == "0" || Heads["39"] == "8") {
		OrdMan->Order_new(Report.ClOrdID,Report);
		//cout << "Add"<<endl<<endl;
	}
	else // тут может быть проблема,т.к. может вызываться неподходящий метод
	{
		if (Report.OrigClOrdID != NOL)
		{
		//	cout << "Alter" << endl << endl;
			OrdMan->Order_update(Report.OrigClOrdID, Report);
		}
		else
		{
		//	cout << "Trade" << endl << endl;
			OrdMan->Order_update(Report.ClOrdID, Report);
		}
	}
}	


void ClientInputProcessor::OrderCancelReject()
{
	RejectedRequest Arguments;
	Arguments.OrderID = Heads["37"];
	Arguments.OrdStatus = Heads["39"];
	if (Heads.find("58") != Heads.end()) Arguments.Text = Heads["58"];
	if (Heads.find("84") != Heads.end()) Arguments.CxlQty = Heads["84"];
	if (Heads.find("102") != Heads.end()) Arguments.CxlQty = Heads["102"];
	Arguments.SeqNum = Heads["34"];
	Arguments.ClOrdID = Heads["11"];
	Arguments.Time = Heads["52"];
	OrdMan->Reject(&Arguments);

}

void ClientInputProcessor::Process(map <string, string> heads)
{
	Heads = heads;
	if (Heads["35"] == "8") {
		ExecutionReport();
	}
	if (Heads["35"] == "9") {
		OrderCancelReject();
	}

};

void ClientInputProcessor::LoadOutputProcessor(OutputProcessor* Out, OrderManager* OrderMan)
{
	Output = Out;
	OrdMan = OrderMan;
}

short SystemInputProcessor::HeartBeat()
{
	return 0;
}

short SystemInputProcessor::TestRequest()
{
	if (Heads.find("112") == Heads.end()) Output->HeartBeat(NOL);
	else Output->HeartBeat(Heads["112"]);
	return 0;
}

short SystemInputProcessor::Logon()
{
	if (Heads["34"] == "1")
		return 0;
	ifstream logfile;
	string str;
	string seqnum = "0";
	string preseqnum = "0";
	logfile.open(SysVars.LOGNAME);
	if (logfile.fail()) {
		Output->Logout(NOL);
		throw 7;
		return 0;
	}
	while (getline(logfile, str)) {
		if (str[0] == 'I' && str[1] == 'N') {
			string M = char(0x1) + "34=";
			int pointer = str.find(M);
			int i;
			for (i = pointer + 2; str[i] != '\x1'; i += 1);
			preseqnum = seqnum;
			seqnum = str.substr(pointer + 2, i - pointer - 2);
		}
	}
	if (stoi(Heads["34"]) - stoi(preseqnum) > 1) {
		ResendReq Rr;
		Rr.BeginSecNo = to_string(stoi(preseqnum) + 1);
		Rr.EndSecNo = to_string(stoi(Heads["34"]) - 1);
		//cout << endl << endl << Rr.BeginSecNo << "-" << Rr.EndSecNo << endl << endl;
		Output->ResendRequest(&Rr);
	};
	logfile.close();
	return 0;
}
short SystemInputProcessor::Logout()
{
	return 0;
}
short SystemInputProcessor::Reject()
{
	return 0;
}

void SystemInputProcessor::Process(map <string, string> heads)
{
	Heads = heads;
	if (Heads["35"] == "1") {
		TestRequest();
	}
	if (Heads["35"] == "0") {
		HeartBeat();
	}
	if (Heads["35"] == "A") {
		Logon();
	}
	if (Heads["35"] == "5") {
		Logout();
	}
	if (Heads["35"] == "3") {
		Reject();
	}
}

void SystemInputProcessor::LoadOutputProcessor(OutputProcessor* Out)
{
	Output = Out;
}


short OutputProcessor::Send(string Message)
{
	int Result = TCP->Send((PCSTR)Message.c_str(), Message.length());
	if (Result == -1)
	{
		return -1;
	};
	SeqNum++;
	return 0;
}

short OutputProcessor::HeartBeat(string TestReqID = NOL)
{
	string Message = LocalFormatter->HeartBeat(SeqNum, TestReqID);
	Send(Message);
	return 0;
}

short OutputProcessor::Logon(string Password, string HeartBtInt = "2", string ResetSeqNum = "Y", string NewPassword = "", string LanguageID = "E")
{
	string Message = LocalFormatter->Logon(Password, SeqNum, HeartBtInt, ResetSeqNum, NewPassword, LanguageID);
	Send(Message);
	return 0;
}

short OutputProcessor::Logout(string Text = NOL)
{
	string Message = LocalFormatter->Logout(SeqNum, Text);
	Send(Message);
	return 0;
}

short OutputProcessor::NewOrder_Single(NewOrder_sngl* Arguments)
{
	string Message = LocalFormatter->NewOrder_Single(SeqNum, Arguments);
	//cout << endl << endl << float(clock() - T)/float(CLOCKS_PER_SEC);
	Send(Message);
	return 0;
}

short OutputProcessor::OrderCancelRequest(OrderCancel* Arguments)
{
	string Message = LocalFormatter->OrderCancelRequest(SeqNum, Arguments);
	Send(Message);
	return 0;
}

short OutputProcessor::ResendRequest(ResendReq* Arguments)
{
	string Message = LocalFormatter->ResendRequest(SeqNum, Arguments);
	Send(Message);
	return 0;
}

void OutputProcessor::OutputProcessorInit(MessageFormatter* localFormatter, TCPClient* tcp)
{
	TCP = tcp;
	LocalFormatter = localFormatter;
	SeqNum = 1;
}

void OrderManager::Order_new(string ClOrdID, ExecutionReportTable Report)
{
	vector<ExecutionReportTable> v;
	v.push_back(Report);
	Reports.insert(pair<string,vector<ExecutionReportTable>>(ClOrdID, v));
}

void OrderManager::Order_update(string ClOrdID, ExecutionReportTable Exec)
{
	Reports[ClOrdID].push_back(Exec);
}

vector<ExecutionReportTable>* OrderManager::Get_History(string ClOrdID)
{
	if (Reports.count(ClOrdID) == 0)
	{
		//cout << "Exception"<< endl<<endl;
	}
	return &Reports[ClOrdID];
}

void OrderManager::Process(OrderTable* Arguments)
{
	LastChangedOrder = *Arguments;
}

void OrderManager::Reject(RejectedRequest* Arguments)
{
	LastRejectedCancel = *Arguments;
};

RejectedRequest OrderManager::GetReject()
{
	return LastRejectedCancel;
};

OrderTable OrderManager::GetChanged()
{
	return LastChangedOrder;
}

void command_container::push(string id, string seccode, string secboard, string side, string qty, string ordtype, string price, string type, string origclordid)
{
	//Mutex.lock();
	ID.push_back(id);
	SECCODE.push_back(seccode);
	SECBOARD.push_back(secboard);
	Side.push_back(side);
	Qty.push_back(qty);
	OrdType.push_back(ordtype);
	Price.push_back(price);
	Type.push_back(type);
	OrigClOrdID.push_back(origclordid);
	//Mutex.unlock();
}

void command_container::pop(string* v)
{
	//string v[7];
	//Mutex.lock();
	v[0] = NOL;
	v[1] = NOL;
	v[2] = NOL;
	v[3] = NOL;
	v[4] = NOL;
	v[5] = NOL;
	v[6] = NOL;
	v[7] = NOL;
	v[8] = NOL;
	if (Type.back() != NOL)
	{
		int size = ID.size();
		v[0] = ID[size-1];
		ID.pop_back();
		v[1] = SECCODE[size-1];
		SECCODE.pop_back();
		v[2] = SECBOARD[size-1];
		SECBOARD.pop_back();
		v[3] = Side[size-1];
		Side.pop_back();
		v[4] = Qty[size-1];
		Qty.pop_back();
		v[5] = OrdType[size-1];
		OrdType.pop_back();
		v[6] = Price[size-1];
		Price.pop_back();
		v[7] = Type[size - 1];
		Type.pop_back();
		v[8] = OrigClOrdID[size - 1];
		OrigClOrdID.pop_back();
	}
	//Mutex.unlock();
}

bool FIXinterface::Initialize()
{
	client.FIX44Init(ScomID, TcomID, Addr, Port);
	client.Logon(Pass, "2", "Y", NewPass, LangID);
	return true;
}

void FIXinterface::WorkingThread()
{
	if (Initialize()==false)
	{
		return;
	}
	string buffer[9];
	while (true)
	{
		cc.pop(buffer);
		isConnected = client.is_connected();
		if (client.is_connected() == false) {
			//break;
		}
		if (buffer[7] == NOL) continue;
		if (buffer[7] == "P")
		{
			NewOrder_sngl Order;
			Order.ClOrdID = buffer[0];
			Order.Account = Account;
			Order.NoTradingSessions = "1";
			Order.TradingSessionID = buffer[2];
			Instrument Tool;
			Tool.Symbol = buffer[1];
			Order.Tool = Tool;
			Order.Side = buffer[3];
			OrderQtyData Qty;
			Qty.OrderQty = buffer[4];
			Order.OrdQty = Qty;
			Order.OrdType = buffer[5];
			Order.Price = buffer[6];
			client.NewOrder_Single(&Order);
		}
		if (buffer[7] == "C")
		{
			OrderCancel Order;
			Order.ClOrdID = buffer[0];
			Order.OrigClOrdID = buffer[8];
			Order.Side = "1";//по умолчанию, но можно и поменять, наверное, если будут проблемы
			client.OrderCancelRequest(&Order);
		}
		if (buffer[7] == "R")
		{
			//cout << endl<<"R"<<endl;
			client.Reconnect(Addr, Port);
			client.Logon(Pass, "2", "N");
		}
	}

}

void FIXinterface::Resume_session()
{
	cc.push(NOL, NOL, NOL, NOL, NOL, NOL, NOL, "R", NOL);
}

int FIXinterface::order_place(string ID, string SECCODE, string SECBOARD, string Side, string Qty, string OrdType, string Price)
{
	cc.push(ID, SECCODE, SECBOARD, Side, Qty, OrdType, Price, "P", NOL);
	return 0;
}

int FIXinterface::order_cancel(string ID, string OrigID)
{
	cc.push(ID, NOL, NOL, NOL, NOL, NOL, NOL, "C", OrigID);
	return 0;
}
