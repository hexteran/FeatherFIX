//#include <iostream>
//#include <ctime>
#pragma once
#include <fstream>
#include <C:\Buffer\\Trading\\FAST\\FAST\\FAST\\XMLReader.h>
#include <C:\Buffer\\Trading\\FAST\\FAST\\FAST\\UDPClient.h>
#include <C:\Buffer\\Trading\\FAST\\FAST\\FAST\\Workflow.h>
#include "decoder_test.h"
//#include <C:\Buffer\\Trading\\FAST\\FAST\\FAST\\FASTDecoder.h>
//#include <cmath>
//#include <windows.h>
#include <iostream>

using namespace std;
using namespace FeatherFAST;
int main() {
/*
	XMLReader doc("C:\\Buffer\\Trading\\FAST\\FAST\\FAST\\tinyxml\\SPECTRA_templates.xml");
	FASTDecoder decoder(doc);
	char bts[] = { (char)0x14, (char)0x29, (char)0xb0 }; 
	int length;
	unsigned int start_time = clock(); // начальное время
	decoder.Process(pkt43, 10000);*/
	/*for (int i = 0; i < 100000; i++)
			decoder.Process(pkt57, 432);
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время
	std::cout << (float)search_time/ 10000;
	Sleep(3000);
	fstream flt("C:\\Buffer\\batch11.log", ios::in | ios::out | ios::binary);
	flt.write((char*)& pkt40[4], 346);*/
	//	decoder.Process(pkt2, 96);

	//	while (i<2)
	//	{
		//	step_pmap();
	//	}
	/*	int length;
		char raw[] = { 0xfe, 0x9, 0xd2 };
		//decimal_value k = to_double(raw, &length, true);
	/*	float a = 1;
		float b = 0.1;
		char* buffer = (char*)&a;/*==*/
	XMLReader doc("C:\\Buffer\\Trading\\FAST\\FAST\\FAST\\tinyxml\\SPECTRA_templates.xml");
	map<int, vector<FIELD>> all_fields = doc.GetTemplates();
	map<int, vector<int>> all_ids = doc.GetFieldIds();
	map<int, vector<string>> all_defaults = doc.GetDefaults();
	map<int, vector<string>> all_constants = doc.GetConstants();
	FASTDecoder decoder(doc);
	UDPClient clnt;
	clnt.Init((char*)"1.1.7.143",(char*)"239.195.140.12", (char*)"91.203.255.242", (char*)"49012");

	for (int i = 0; i < 1000000; i++)
	{
		clnt.Read(decoder);
		Sleep(5);
	}
	cout << endl;
	
	//WorkFlow bgx((char*)"C:\\Buffer\\Trading\\FAST\\FAST\\FAST\\tinyxml\\configurator.xml", (char*)"C:\\Buffer\\Trading\\FAST\\FAST\\FAST\\tinyxml\\FIX50SP2-2020-Nov.xml"); 
	return 0;
}