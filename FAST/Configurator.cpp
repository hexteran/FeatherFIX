#pragma once
#include "Configurator.h"
#include <iostream>

using namespace std;
using namespace FeatherFAST;
void Configurator::_process_threads(TiXmlElement* elem)
{
	int i = 0;
	int j = 0;
	bool flag = false;
	for (TiXmlAttribute* attr = elem->FirstAttribute(); attr; attr = attr->Next())
		if ((string)attr->Name() == "int") {
			flag = true;
			_interface = attr->Value();
		}
	if (flag == false)
		throw ConfiguratorThreadsError;
	for (TiXmlElement* child = elem->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		j = 0;
		for (TiXmlElement* addr = child->FirstChildElement(); addr; addr = addr->NextSiblingElement())
		{
			for (TiXmlAttribute* attr = addr->FirstAttribute(); attr; attr = attr->Next())
			{
				bool flag = false;
				if ((string)attr->Name() == "port")
				{
					_ports[i][j] = attr->Value();
					flag = true;
				}
				if ((string)attr->Name() == "ip")
				{
					_ips[i][j] = attr->Value();
					flag = true;
				}
				if ((string)attr->Name() == "src")
				{
					_srcs[i][j] = attr->Value();
					flag = true;
				}
				if ((string)attr->Name() == "interface")
				{
					_interfaces[i][j] = attr->Value();
					flag = true;
				}
				if ((string)attr->Name() == "name")
				{
					flag = true;
				}
				if (flag == false)
					throw ConfiguratorThreadsError;
			}
			j++;
		}
		i++;
	}
}

void Configurator::_process_logfile(TiXmlElement* elem)
{
	bool flag = false;
	for (TiXmlAttribute* attr = elem->FirstAttribute(); attr; attr = attr->Next())
	{
		flag = false;
		if ((string)attr->Name() == "file")
		{
			_logfile = attr->Value();
			flag = true;
		}
		if ((string)attr->Name() == "regime")
			if (((string)attr->Value() == "add") || ((string)attr->Value() == "new"))
			{
				_logfile_regime = attr->Value();
				flag = true;
			}
			else
			{
				throw ConfiguratorLogfileError;
			}
		if (flag == false)
		{
			throw ConfiguratorLogfileError;
		}
	};
	if (flag == false)
	{
		throw ConfiguratorLogfileError;
	}
}

void Configurator::_process_templates(TiXmlElement* elem)
{
	bool flag = false;
	TiXmlAttribute* attr = elem->FirstAttribute();
	if ((string)attr->Name() == "file")
	{
		_templatesfile = attr->Value();
		flag = true;
	}
	if (flag == false)
		throw ConfiguratorTemplatesError;
}

Configurator::Configurator(string configfile)
{
	_templatesfile = "";
	for (int i = 0; i<NumofAddrs;i++)
		for (int j = 0; j < NumofThreads; j++)
		{
			_ips[i][j] = "";
			_srcs[i][j] = "";
		}
	TiXmlDocument doc(configfile.c_str());
	if (!(doc.LoadFile())) {
		throw ConfiguratorFileError;
	}
	TiXmlElement *config = doc.FirstChildElement();
	for (TiXmlElement* child = config->FirstChildElement(); child; child = child->NextSiblingElement())
	{
		bool flag = false;
		if ((string)child->Value() == "logfile") {
			_process_logfile(child);
			flag = true;
		}
		if ((string)child->Value() == "templates") {
			_process_templates(child);
			flag = true;
		}
		if ((string)child->Value() == "threads") {
			_process_threads(child);
			flag = true;
		}
		if ((string)child->Value() == "threads") {
			_process_threads(child);
			flag = true;
		}
		if (flag == false)
			throw ConfiguratorUndefinedElement;
	}
}

string Configurator::GetTemplatesFile()
{
	return _templatesfile;
}

string Configurator::GetLogfile(string& regime)
{
	regime = _logfile_regime;
	return _logfile;
}

string* Configurator::GetIPs(int thread)
{
	return _ips[thread];
}

string* Configurator::GetSrcs(int thread)
{
	return _srcs[thread];
}

string* Configurator::GetPorts(int thread)
{
	return _ports[thread];
}

string* Configurator::GetInterface(int thread)
{
	return _interfaces[thread];
}
