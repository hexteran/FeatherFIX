#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <iterator>
#include <thread>
#include <ctime>
#include <fstream> 
#include <mutex>
#include <string.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

#define NOL "BULLNULL"
#define RLEN 65536
#define ending_comb "\x6\x6\x6Stop" 
#define DLLEXPORT  __declspec(dllexport)
#define DLLEXPORTNOT
using namespace std;

struct DLLEXPORT Instrument
{
	string Symbol = NOL;
	string Product = NOL;
	string CFICode = NOL;
	string SecurityType = NOL;
};
struct DLLEXPORT YieldData
{
	string Yield = NOL;
	string NoPartyID = NOL;
	string PartyID = NOL;
	string PartyIDSource = NOL;
	string PartyRole = NOL;

};
struct DLLEXPORT CommissionData
{
	string Commission = NOL;
	string CommType = NOL;
};
struct DLLEXPORT OrderQtyData
{
	string OrderQty = NOL;
	string CashOrderQty = NOL;
};
struct DLLEXPORT TrdRegTimestamps
{
	string NoTrdRegTimestamps = NOL;
	string TrdRegTimestamp = NOL;
	string TrdRegTimestampType = NOL;
};
struct DLLEXPORT FinancialDetails
{
	string StartDate = NOL;
	string EndDate = NOL;
	string DeliveryType = NOL;
};
struct DLLEXPORT UnderlyingInstrument
{
	string UnderlyingSymbol = NOL;
};
struct DLLEXPORT UnderlyingStipulations
{
	string NoUnderlyingStips = NOL;
	string UnderlyingStipType = NOL;
	string UnderlyingStipValue = NOL;
};
struct DLLEXPORT Parties
{
	string NoPartyID = NOL;
	string PartyID = NOL;
	string PartyIDSource = NOL;
	string PartyRole = NOL;
};
struct DLLEXPORT ResendReq
{
	string BeginSecNo = NOL;
	string EndSecNo = NOL;
};
struct DLLEXPORT NewOrder_sngl
{
	string ClOrdID = NOL;
	Parties Party;
	string Account = NOL;
	string MaxFloor = NOL;
	string SecondaryClOrdID = NOL;
	string NoTradingSessions = NOL;
	string TradingSessionID = NOL;
	Instrument Tool;
	string Side = NOL;
	string TransactTime = NOL;
	OrderQtyData OrdQty;
	string OrdType = NOL;
	string PriceType = NOL;
	string Price = NOL;
	string TradeThruTime = NOL;
	string TimeInForce = NOL;
	string EffectiveTime = NOL;
	string OrderCapacity = NOL;
	string OrderRestrictions = NOL;
	string MaxPriceLevels = NOL;
};

struct DLLEXPORT OrderCancel
{
	string OrigClOrdID = NOL;
	string OrderID = NOL;
	string ClOrdID = NOL;
	string Side = NOL;
	string TransactTime = NOL;
};
struct DLLEXPORT OrderTable
{
	string ClOrdID = NOL;
	string Status = NOL;
	string ExecType = NOL;
	string WorkingIndicator = NOL;
	string OrdType = NOL;
	string Side = NOL;
	string Price = NOL;
	string LeavesQty = NOL;
	string CumQty = NOL;
	Instrument Tool;
	string LastQty = NOL;
	string LastPrice = NOL;
	string TradingSessionID = NOL;
	string Time = NOL;
	string SeqNum = NOL;
};

struct DLLEXPORT RejectedRequest
{
	string OrderID = NOL;
	string OrdStatus = NOL;
	string CxlRejResponseTo = NOL;
	string Text = NOL;
	string CxlQty = NOL;
	string Time = NOL;
	string SeqNum = NOL;
	string ClOrdID;
};
using namespace std;

struct ExecutionReportTable
{
	string ClOrdID = NOL;
	string OrigClOrdID = NOL;
	string ExecID = NOL;
	string ExecType = NOL;
	string OrdStatus = NOL;
	string ExecRestatementReason = NOL;
	string Account = NOL;
	string Side = NOL;
	string Price = NOL;
	string LastQty = NOL;
	string LastPx = NOL;
	string LeavesQty = NOL;
	string CumQty = NOL;
	string Text = NOL;
	
	//parties не поддерживаем
};

class DLLEXPORTNOT OrderManager
{
	RejectedRequest LastRejectedCancel;
	OrderTable LastChangedOrder;
	map <string, vector<ExecutionReportTable>> Reports;
public:
	void Order_new(string ClOrdID, ExecutionReportTable Report);//создается при получении Exec. Report типа 0
	void Order_update(string ClOrdID, ExecutionReportTable Exec);//изменяется при получении заявки других типов
	vector<ExecutionReportTable>* Get_History(string ClOrdID); //тут должно возбуждаться исключение если элемент не найден
	//может стоит возвращать указатель на вектор вместо копирования вектора?
	void Process(OrderTable* Arguments);
	void Reject(RejectedRequest* Arguments);
	RejectedRequest GetReject();
	OrderTable GetChanged();
};

class DLLEXPORT SystemVariablesProcessor
{
	bool choose(string a, string b);
	bool parse(char* str);
public:
	bool logflag;
	string LOGNAME;
	ofstream LOGGING;
	SystemVariablesProcessor(string FName);
} SysVars("FIX_vars.inf");

class DLLEXPORTNOT TCPClient
{
	SOCKET sock;
	WSADATA wsDATA;
	int LastError;
public:
	bool initial(PCSTR Addr, PCSTR Port);
	int Send(PCSTR message, int len);
	int Receive(PCSTR output, string& input);
};

class DLLEXPORT MessageFormatter
{
private:
	map<string, string> Heads;
	string SenderCompID, TargetCompID;
	void Load(int SeqNum);
	string Linking(list<string>* BodyOrder);
public:
	void MessageFormatterInit(string senderCompID, string targetCompID);
	string Logon(string, int, string, string, string, string);
	string HeartBeat(int, string);
	string TestRequest(int SeqNum, string TestReqID);
	string ResendRequest(int SeqNum, ResendReq* Arguments);
	string Logout(int, string);
	string NewOrder_Single(int SeqNum, NewOrder_sngl* Arguments);
	string OrderCancelRequest(int SeqNum, OrderCancel* Arguments);
};

class DLLEXPORTNOT OutputProcessor
{
private:
	TCPClient* TCP;
	int SeqNum;
	MessageFormatter* LocalFormatter;
	short Send(string Message);
public:
	short HeartBeat(string);
	short Logon(string, string, string, string, string);
	short Logout(string);
	short NewOrder_Single(NewOrder_sngl* Arguments);
	short OrderCancelRequest(OrderCancel* Arguments);
	short ResendRequest(ResendReq* Arguments);
	void OutputProcessorInit(MessageFormatter* localFormatter, TCPClient* tcp);
};

class DLLEXPORTNOT SystemInputProcessor
{
	OutputProcessor* Output;
	map <string, string> Heads;
	short HeartBeat();
	short TestRequest();
	short Logon();//нужно исключение если не найден файл
	short Logout();
	short Reject(); //возбуждать исключение
public:
	void Process(map <string, string> heads);
	void LoadOutputProcessor(OutputProcessor* Out);
};

class DLLEXPORTNOT ClientInputProcessor {
	OutputProcessor* Output;
	OrderManager* OrdMan;
	map <string, string> Heads;
	void ExecutionReport();
	void OrderCancelReject();
public:
	void Process(map <string, string> heads);
	void LoadOutputProcessor(OutputProcessor* Out, OrderManager* OrderMan);
};


class DLLEXPORTNOT InputProcessor
{
private:
	string InputString;
	SystemInputProcessor SysInProc;
	ClientInputProcessor ClInProc;
	map <string, string> Parsing();
	short Distributing();
public:
	void Receive(string& Input);
	void InputProcessorInit(OutputProcessor* Out, OrderManager* Ordman);
};

class DLLEXPORT FIX44
{
private:
//	string LSeqNum_before_disconnect;
//	string LastINSeqNum;  //при таком запоминании последнего номера полученного сообщения может быть проблема с тем,
	//что прямо перед разрывом соединения может быть вызвана ResendRequest и пришлются сообщения с номерами меньшими чем требуется 
	bool isConnected;
	TCPClient TCP;
	MessageFormatter Formatter;
	OutputProcessor Out;
	InputProcessor In;
	thread Listener;
	string InputContainer;
	void Listening();
	OrderManager OrdMan;
public:
	bool is_connected();
	short Logon(string Password, string HeartBtInt, string ResetSeqNum, string NewPassword, string LanguageID);
	short Logout(string Text);
	short ResendRequest(ResendReq* Arguments);
	short NewOrder_Single(NewOrder_sngl* Arguments);
	short OrderCancelRequest(OrderCancel* Arguments);
	vector<ExecutionReportTable>* Get_OrdMan_History(string ClOrdID);
	void FIX44Init(string senderCompID, string targetCompID, string address, string port); //исключение, если не подключено
	void Reconnect(string address, string port);
	OrderTable LastChangedOrder();
	RejectedRequest LastRejectedCancel();
};

class DLLEXPORT command_container //не является потокобезопасным
{

public:
	vector<string> ID;
	vector<string> SECCODE;
	vector<string> SECBOARD;
	vector<string> Side;
	vector<string> Qty;
	vector<string> OrdType;
	vector<string> Price;
	vector<string> Type;
	vector<string> OrigClOrdID;
	mutex Mutex;
	command_container()
	{
		ID.push_back(NOL);
		SECCODE.push_back(NOL);
		SECBOARD.push_back(NOL);
		Side.push_back(NOL);
		Qty.push_back(NOL);
		OrdType.push_back(NOL);
		Price.push_back(NOL);
		Type.push_back(NOL);
		OrigClOrdID.push_back(NOL);
	}
	void push(string ID, string SECCODE, string SECBOARD, string Side, string Qty, string OrdType, string Price, string Type, string origclordid);
	void pop(string* buffer);
};

class DLLEXPORT FIXinterface 
{
	bool isConnected;
	string ScomID, TcomID, Addr, Port, Pass, Account, NewPass, LangID;
	command_container cc;
	FIX44 client;
	bool Initialize();//сделать исключение
	void WorkingThread();
public:
	FIXinterface(string senderCompID, string targetCompID, string address, string port, string password, string account, string NewPassword = "", string LanguageID = "E")
	{
		isConnected = false;
		ScomID = senderCompID;
		TcomID = targetCompID;
		Addr = address;
		Port = port;
		Pass = password;
		Account = account;
		NewPass = NewPassword;
		LangID = LanguageID;
		thread worker(&FIXinterface::WorkingThread, this);
		worker.detach();
	}
	void Resume_session();
	void End_session();
	int order_place(string ID, string SECCODE, string SECBOARD, string Side, string Qty, Parties* parties, string OrdType = "1", string Price = "0");
	int order_place(string ID, string SECCODE, string SECBOARD, string Side, string Qty, string OrdType = "1", string Price = "0");
	int order_cancel(string ID, string OrigID);
};
bool DLLEXPORT SessionIDs[1024];
int DLLEXPORT FIX_start(char* senderCompID, char* targetCompID, char* address, char* port);

