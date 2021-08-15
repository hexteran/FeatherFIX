//#include <iostream>
//#include <ctime>
#pragma once
#include <iostream>
#include "..\\FIX\FIXDecoder.h"
#include "..\\FIX\FIXEncoder.h"
#include "..\\FAST\XMLReader.h"
#include "..\\FIX\TCPClient.h"
#include "..\\FIX\Session.h"
#include "..\\FIX\SystemMessageProcessor.h"
//#include "..\\FIX\FIX.h"
#include "fix_decoder_test.h"
#include <ctime>
#include <chrono>

class ssd : public FeatherFIX::UserInputProc
{
private:
	int k = 10;
	char buf[1024] = {0};
	int len = 1024;
public:
	virtual void write_to_users()
	{
		cout << buf << endl;
	};
};
int main() {
	ssd kjk;
	FeatherFIX::Session one("IFIX-EQ-UAT", "MU9043800002", kjk,8, "C:\\Buffer\\Dest.txt");
	one.Connect("91.208.232.200", "9130");
	FeatherFAST::Message msg;
	msg.add_field((char*)"0",98,STRING);
	msg.add_field((char*)"3", 108, STRING);
	msg.add_field((char*)"4068", 554, STRING);

	one.Start_Hearing();
	time_t timed = time(NULL);
	one.Send_Message(msg,"A", timed,timed, false, false);
	Sleep(5000);
	FeatherFIX::Session& s = one;
	FeatherFIX::BasicUserOutput out(one);
	out.Order_New("1", (char*)"1", (char*)"0", (char*)"1", (char*)"1", (char*)"SBRP", (char*)"TQBR");
	Sleep(5000);
	one.Stop_Hearing();
	cout << " DONE";
//	Sleep(5000);
	//FeatherFAST::Message logon;
	//FeatherFIX::SentMessagesPool hdm("C:\\Buffer\\distro1.txt");
	//hdm.remove_pool();
	//hdm.write_message(15,"aaa");
	//cout << hdm.load_message(15);

}