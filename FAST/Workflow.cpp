#include "Workflow.h"
using namespace FeatherFAST;
void WorkThread::_initUDP(char* Addr, char* Group, char* Source, char* Port)
{
	_udpClient.Init(Addr, Group, Source, Port);
}

void WorkThread::Run()
{

}

void WorkThread::Shutdown()
{
	_isrunning = false;
}

WorkThread::WorkThread(XMLReader& xmlr, char* Addr, char* Group, char* Source, char* Port):
all_fields(xmlr.GetTemplates()), all_ids(xmlr.GetFieldIds()),
all_defaults(xmlr.GetDefaults()), all_constants(xmlr.GetConstants()),
_fastDecoder(all_fields, all_ids, all_defaults, all_constants)
{
	_initUDP(Addr, Group, Source, Port);
	_isrunning = true;
	Run();
/*	thread worker(&WorkThread::Run,this);
	worker.detach();
	Sleep(10000);*/
}

void WorkFlow::Run()
{
	num_of_threads = 0;
	string* ips;
	string* sources;
	string* ports;
	string* interfaces;
	do
	{
		ips = _config.GetIPs(num_of_threads);
		num_of_threads++;
	} while ((num_of_threads <= NumofThreads) && (ips[0] != ""));
	num_of_threads--;
	for (int i = 0; i < num_of_threads; i++)
	{
		//cout << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOONE";
		ips = _config.GetIPs(i);
		sources = _config.GetSrcs(i);
		ports = _config.GetPorts(i);
		interfaces = _config.GetInterface(i);
		_threads[i] = new WorkThread(_xmlRead,(char*)interfaces[0].c_str(), (char*)ips[0].c_str(),
									(char*)sources[0].c_str(), (char*)ports[0].c_str());
	}
}

WorkFlow::WorkFlow(string configurator, string template_file) : 
	_config(configurator), _xmlRead(template_file)
{
	threads_iterator = 0;
	Run();

}
