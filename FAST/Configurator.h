
//добавить в Configurator Little Endian или Big Endian

#pragma once
#include "tinyxml/tinyxml.h"
#include <string>

#define NumofAddrs 16 
#define NumofThreads 8

#define ConfiguratorFileError 201
#define ConfiguratorUndefinedElement 202
#define ConfiguratorLogfileError 203
#define ConfiguratorThreadsError 204
#define ConfiguratorTemplatesError 205

using namespace std;
namespace FeatherFAST {
	class __declspec(dllexport) Configurator
	{
	private:
		string _logfile;
		string _logfile_regime;
		string _ips[NumofAddrs][NumofThreads];
		string _srcs[NumofAddrs][NumofThreads];
		string _ports[NumofAddrs][NumofThreads];
		string _interfaces[NumofAddrs][NumofThreads];
		string _interface;
		string _templatesfile;
		void _process_threads(TiXmlElement* elem);
		void _process_logfile(TiXmlElement* elem);
		void _process_templates(TiXmlElement* elem);

	public:
		Configurator(string configfile);
		string GetTemplatesFile();
		string GetLogfile(string& regime);
		string* GetIPs(int thread);
		string* GetSrcs(int thread);
		string* GetPorts(int thread);
		string* GetInterface(int thread);
	};
};