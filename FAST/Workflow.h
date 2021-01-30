#pragma once
#include <vector>
#include <map>
#include <string>
#include <thread>
#include "Configurator.h"
#include "XMLReader.h"
#include "UDPClient.h"
#include "Logger.h"
#include "ErrorHandler.h"
#include "FASTDecoder.h"
#include "MessageProcessor.h"

#define NUM_OF_CLIENTS_PER_THREAD 16 //сберегу константу на будущее
//#define NumofThreads 8
#define BUFFERSIZE 3200
namespace  FeatherFAST {

	typedef int FIELD;

	class WorkThread
	{
	private:
		string _threadname;
		int _numofclients;
		bool _isrunning;
		map<int, vector<FIELD>> all_fields;
		map<int, vector<int>> all_ids;
		map<int, vector<string>> all_defaults;
		map<int, vector<string>> all_constants;
		FASTDecoder _fastDecoder;
		UDPClient _udpClient;
		ErrorHandler _errorHandler;
		MessageProcessor _messageProcessor;
		std::map <int, std::vector<FIELD>> _templates;

		void _initUDP(char* Addr, char* Group, char* Source, char* Port);

	public:
		void Run();
		void Shutdown();
		WorkThread(XMLReader& xmlr, char* Addr, char* Group, char* Source, char* Port);
	};

	class WorkFlow
	{
	private:
		int threads_iterator;
		int num_of_threads;
		XMLReader _xmlRead;
		Configurator _config;
		WorkThread* _threads[NumofThreads];
	public:
		void Run();
		WorkFlow(string configurator, string template_file);
	};
};