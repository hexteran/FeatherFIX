//#include <iostream>
//#include <ctime>
#pragma once
#include <iostream>
#include "..\\FIX\FIXEncoder.h"
#include "..\\FAST\XMLReader.h"
//#include "..\\FIX\FIX.h"
#include "fix_decoder_test.h"


int main() {
/*	std::cout << m1;
	FIXDecoder fx;
	FeatherFAST::XMLReader reader("C:\\Buffer\\Trading\\FAST\\FAST\\FAST\\tinyxml\\SPECTRA_templates.xml");
	FeatherFAST::TemplateManager tempm(reader);
	FeatherFAST::Template *temp = tempm.pull_template(19);
	FeatherFAST::Message Result = fx.Process(m1, strlen(m1),*temp);
	while (Result.is_end() == false)
	{
		if (Result.field_id() == 5842)
			cout << "a";
		if ((Result.type() & FAST_DECIMAL) == 0)
		{
			if (Result.type() & (STRING | CONSTANT))
				cout << Result.field_id() << "=" << Result.value() << endl;
			if (Result.type() & (Int32 | uInt32))
				cout << Result.field_id() << "=" << *(int*)Result.value() << endl;
			if (Result.type() & (Int64 | uInt64))
				cout << Result.field_id() << "=" << *(long long*)Result.value() << endl;
		}
		else
			cout << Result.field_id() << "=" << (*(FeatherFAST::decimal_value*)Result.value()).mantissa << endl;
		
		Result.next();
	}*/
	FIXEncoder fxenc("AAA", "BBB"); 
	FeatherFAST::Message mess;
	long long current_time = 0;
	mess.add_field((char*)"0", 98, STRING);
	mess.add_field((char*)"PASSWORD", 554, STRING);
	cout<<fxenc.Process(mess, 1, "A", current_time, 0, false, false);
	/*MessageFormatter Form;
	cout<<Form.Logon("AAA", 0, "10", "Y", "", "E");*/
	return 0;
}