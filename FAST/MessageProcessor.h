#pragma once
#include <iostream>
#include <string>

#define SEQ_END_ID -1
#define SEQUENCE_ID -2
#define SEQ_NUM_EXMP -3

#define OUTPUT_MAX_SIZE 2048 
#define STRING_MAX_SIZE 64
#define BUFFSIZE 32
#define TEMPLATESIZE 1024
#define EMPTY_TEMPLATE -1

#define OPT 4096
#define DELTA 1
#define COPY 2 
#define TAIL 4
#define DEFAULT 8
#define INCREMENT 16
#define uInt32 32
#define uInt64 64
#define Int32 128
#define Int64 256
#define FAST_DECIMAL 512
#define STRING 1024 
#define BYTE_VECTOR 2048
#define CONSTANT 8192
#define SEQUENCE 16384
#define SEQ_END 32768

#define MESSAGE_BUFFER_OVERFLOW 601

namespace FeatherFAST {
	struct __declspec(dllexport) decimal_value
	{
		int exponent;
		long long mantissa;
	};
	struct __declspec(dllexport) Template
	{
		int tid = EMPTY_TEMPLATE;
		int* temp;
		char* defaults[TEMPLATESIZE];
		char* constants[TEMPLATESIZE];
		int* fieldids;
		int const_size;
		int def_size;
		int mes_size;
		int temp_size;
	};
	struct __declspec(dllexport) outPut
	{
		Template* temp;
		char values[OUTPUT_MAX_SIZE*10];
		int types[OUTPUT_MAX_SIZE];
		int ids[OUTPUT_MAX_SIZE];
		int length = 0;
		int seqnum;
	};
	struct __declspec(dllexport) Container
	{
		char values[OUTPUT_MAX_SIZE*30];
		int types[OUTPUT_MAX_SIZE];
		int ids[OUTPUT_MAX_SIZE];
		int length=0;
		int val_length=0;
		int seqnum = 0;
		int tid = EMPTY_TEMPLATE;
	};
	struct __declspec(dllexport) inPut
	{
		unsigned int msg_length;
		Template* temp;
		char* message;
		char* pmap;
	};
	template <typename T> class Field
	{
	private:
		int _id, _flags;
		T _value;
	public:
		Field(int id, int flags, T value) : _id(id), _flags(flags), _value(value) {};
		Field() {};

		T get_value();
		void set_value(T value);

		int get_flags();
		void set_flags(int flags);

		int get_id();
		void set_id(int id);
	};
	class __declspec(dllexport) Message { //закончить позже
	private:
		const char seqend[7], seqbeg[7];
		Container mes;
		int it, iv, i_len;
		void move_id();
		void move_t();
		void move_v(int space);
	public:
		Message(Container m): mes(m), it(0), iv(0), seqend("SEQEND"), seqbeg("SEQBEG") {};//перетащить в src
		Message() : it(0), iv(0), seqend("SEQEND"), seqbeg("SEQBEG"), i_len(0) {};
		bool is_end();
		bool is_beg();
		int field_id();
		int type();
		char* value();
		void prev();
		bool next();
		int length();
		int seqnum();
		void add_field(char* field, int id, int type);
		bool delete_field(); // закончить
		void delete_field(int id);
	};
	class MessageProcessor
	{
	private:
		int _thread_id;
		Template* temp;
		outPut* inputbuffer;
		void process_default();
		void process_constants();
		void process_tail();
		void process_increment();
	public:
		Message Process(outPut* in);
	};
	template<typename T>
	inline T Field<T>::get_value()
	{
		return _value;
	}
	template<typename T>
	inline void Field<T>::set_value(T value)
	{
		_value = value;
	}
	template<typename T>
	inline int Field<T>::get_flags()
	{
		return _flags;
	}
	template<typename T>
	inline void Field<T>::set_flags(int flags)
	{
		_flags = _flags | flags;
	}
	template<typename T>
	inline int Field<T>::get_id()
	{
		return _id;
	}
	template<typename T>
	inline void Field<T>::set_id(int id)
	{
		_id = id;
	}
};