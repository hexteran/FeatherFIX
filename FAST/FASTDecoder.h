//little-endian order supported only

#pragma once
#include<vector>
#include<map>
#include<string>
#include "MessageProcessor.h"
#include "XMLReader.h"
#define TRUE 1
#define FALSE 0
#define DECFALSE -1
#define GROUPPMAP 65536

#define DECODER_OUT_OF_BOUNDS_ERROR 401
#define TEMPLATEMANAGER_LOAD_ERROR 402
#define FASTprocessor_step_temp_ERROR 403
#define TYPE_CONVERTION_ERROR 404
#define SEQUENCE_CONVERTION_ERROR 405

using namespace std;

namespace FeatherFAST {
	unsigned int to_uInt32(char* raw, int* length);
	unsigned long long int to_uInt64(char* raw, int* length);
	int to_Int32(char* raw, int* length);
	long long int to_Int64(char* raw, int* length);
	decimal_value to_double(char* raw, int* length, bool is_two_byted);
	void to_string(char* message, char* buffer, int* length);

	class __declspec(dllexport) TemplateManager
	{
	public:
	private:
		Template cache[3];
		map<int, vector<int>>& allfields;
		map<int, vector<int>>& allids;
		map<int, vector<string>>& alldefaults;
		map<int, vector<string>>& allconstants;
		int first, second, third;
	public:
		void load_new_temp(int tid);
		TemplateManager(map<int, vector<int>>& fields, map<int, vector<int>>& ids, map<int, vector<string>>& defaults, map<int, vector<string>>& constants);
		TemplateManager(XMLReader& reader);
		void Init(map<int, vector<int>>& fields, map<int, vector<int>>& ids, map<int, vector<string>>& defaults, map<int, vector<string>>& constants);
		Template* pull_template(int tid);
	};
	class __declspec(dllexport) FASTGroup //абстрактный класс - группа в FAST сообщения и методы работы с ним
	{
	protected:
		Template* temp;
		unsigned int length;
		char* pmap;
		int pmap_len;
		char* currentmsg;
		outPut* outputbuffer;
		int mc, pc, tc; //inputcursors - курсоры по pmap, сообщению и шаблону
		int otc, ovc; //outputcoursors - курсоры по массиву типов и массиву значений
		int get_field_length(char* field);

		virtual void process_sequence(inPut* in, outPut* out, int* tco, int* mco, int* otco, int* ovco) = 0;//убрать лишний outPut
		void stepup(inPut* in);
		int step_temp();
		bool step_pmap();
		void step_msg(int, bool, inPut*);
	};

	class __declspec(dllexport) FASTsequence : FASTGroup
	{
	private:
		void process_sequence(inPut* in, outPut* out, int* tco, int* mco, int* otco, int* ovco); //убрать лишний outPut
	public:
		void Process(inPut* in, outPut* out, int* tco, int* mco, int* otco, int* ovco);
	};

	class __declspec(dllexport) FASTprocessor : FASTGroup
	{
	private:
		FASTsequence seqproc;
		virtual void process_sequence(inPut* in, outPut* out, int* tco, int* mco, int* otco, int* ovco);//убрать лишний outPut
	public:
		FASTprocessor();
		void Process(inPut* in, outPut* out);
	};

	class __declspec(dllexport) FASTDecoder
	{
	private:
		map<int, vector<int>> _fields;
		map<int, vector<int>> _ids;
		map<int, vector<string>> _defaults;
		map<int, vector<string>> _constants;

		MessageProcessor MsgProc;
		FASTprocessor fst_prc;
		TemplateManager template_Mngr;

		int seqnum;
		int  mposition, pposition, tposition;

		char* pmap;
		int pmap_len;
		inline int get_pmap_length(char* pmap);

		int length;

	public:
		
		FASTDecoder(map<int, vector<int>>& fields, map<int, vector<int>>& ids,
			map<int, vector<string>>& defaults, map<int, vector<string>>& constants);
		FASTDecoder(XMLReader& xml);
		Message Process(char* message, unsigned int length);
	};
};