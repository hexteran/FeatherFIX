#pragma once
#include "..//FAST/MessageProcessor.h"
#include "../FAST/FASTDecoder.h"
#define SEPARATOR (char)'\001';
class FIXDecoder
{
private:
	static inline void process_no_temp(char* message, unsigned int length, FeatherFAST::Message &mess);
	static int find_field_in_temp(int *it, int id, FeatherFAST::Template& temp);
	static inline void process_with_temp(char* message, unsigned int length,
										FeatherFAST::Message &mess, FeatherFAST::Template& temp);
public:
	FeatherFAST::Message Process(char* message, unsigned int length, FeatherFAST::Template& temp);
	FeatherFAST::Message Process(char* message, unsigned int length);
};