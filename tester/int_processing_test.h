int int_encode_8bytes(long long int number, char* buffer)
{
	char* bytes = (char*)& number;
	int offset = 0;
	buffer[9] = (bytes[0] & 127) + 128;
	buffer[8] = (bytes[1] & 63) * 2 + (bytes[0] & 128) / 128;
	buffer[7] = (bytes[2] & 31) * 4 + (bytes[1] & 192) / 64;
	buffer[6] = (bytes[3] & 15) * 8 + (bytes[2] & 224) / 32;
	buffer[5] = (bytes[4] & 7) * 16 + (bytes[3] & 240) / 16;
	buffer[4] = (bytes[5] & 3) * 32 + (bytes[4] & 248) / 8;
	buffer[3] = (bytes[6] & 1) * 64 + (bytes[5] & 252) / 4;
	buffer[2] = (bytes[6] & 254) / 2;
	buffer[1] = (bytes[7] & 127);
	buffer[0] = (bytes[7] & 128) / 128;
	if (buffer[0] == 0)
		offset = 1;
	return offset;
}
int int_encode_7bytes(long long int number, char* buffer)
{
	char* bytes = (char*)& number;
	int offset = 0;
	buffer[7] = (bytes[0] & 127) + 128;
	buffer[6] = (bytes[1] & 63) * 2 + (bytes[0] & 128) / 128;
	buffer[5] = (bytes[2] & 31) * 4 + (bytes[1] & 192) / 64;
	buffer[4] = (bytes[3] & 15) * 8 + (bytes[2] & 224) / 32;
	buffer[3] = (bytes[4] & 7) * 16 + (bytes[3] & 240) / 16;
	buffer[2] = (bytes[5] & 3) * 32 + (bytes[4] & 248) / 8;
	buffer[1] = (bytes[6] & 1) * 64 + (bytes[5] & 252) / 4;
	buffer[0] = (bytes[6] & 254) / 2;
	if (buffer[0] == 0)
		offset = 1;
	return offset;
}

int int_encode_6bytes(long long int number, char* buffer)
{
	char* bytes = (char*)& number;
	int offset = 0;
	buffer[6] = (bytes[0] & 127) + 128;
	buffer[5] = (bytes[1] & 63) * 2 + (bytes[0] & 128) / 128;
	buffer[4] = (bytes[2] & 31) * 4 + (bytes[1] & 192) / 64;
	buffer[3] = (bytes[3] & 15) * 8 + (bytes[2] & 224) / 32;
	buffer[2] = (bytes[4] & 7) * 16 + (bytes[3] & 240) / 16;
	buffer[1] = (bytes[5] & 3) * 32 + (bytes[4] & 248) / 8;
	buffer[0] = (bytes[5] & 252) / 4;
	if (buffer[0] == 0)
		offset = 1;
	return offset;
}

int int_encode_5bytes(long long int number, char* buffer)
{
	char* bytes = (char*)& number;
	int offset = 0;
	buffer[5] = (bytes[0] & 127) + 128;
	buffer[4] = (bytes[1] & 63) * 2 + (bytes[0] & 128) / 128;
	buffer[3] = (bytes[2] & 31) * 4 + (bytes[1] & 192) / 64;
	buffer[2] = (bytes[3] & 15) * 8 + (bytes[2] & 224) / 32;
	buffer[1] = (bytes[4] & 7) * 16 + (bytes[3] & 240) / 16;
	buffer[0] = (bytes[4] & 248) / 8;
	if (buffer[0] == 0)
		offset = 1;
	return offset;
}

int int_encode_4bytes(int number, char* buffer)
{
	char* bytes = (char*)& number;
	int offset = 0;
	buffer[4] = (bytes[0] & 127) + 128;
	buffer[3] = (bytes[1] & 63) * 2 + (bytes[0] & 128) / 128;
	buffer[2] = (bytes[2] & 31) * 4 + (bytes[1] & 192) / 64;
	buffer[1] = (bytes[3] & 15) * 8 + (bytes[2] & 224) / 32;
	buffer[0] = (bytes[3] & 240) / 16;
	if (buffer[0] == 0)
		offset = 1;
	return offset;
}
int int_encode_3bytes(int number, char* buffer)
{
	char* bytes = (char*)& number;
	int offset = 0;
	buffer[3] = (bytes[0] & 127) + 128;
	buffer[2] = (bytes[1] & 63) * 2 + (bytes[0] & 128) / 128;
	buffer[1] = (bytes[2] & 31) * 4 + (bytes[1] & 192) / 64;
	buffer[0] = (bytes[2] & 224) / 32;
	if (buffer[0] == 0)
		offset = 1;
	return offset;
}
int int_encode_2bytes(int number, char* buffer)
{
	char* bytes = (char*)& number;
	int offset = 0;
	buffer[2] = (bytes[0] & 127) + 128;
	buffer[1] = (bytes[1] & 63) * 2 + (bytes[0] & 128) / 128;
	buffer[0] = (bytes[1] & 192) / 64;
	if (buffer[0] == 0)
		offset = 1;
	return offset;
}
int int_encode_1byte(int number, char* buffer)
{
	char* bytes = (char*)& number;
	int offset = 0;
	buffer[1] = (bytes[0] & 127) + 128;
	buffer[0] = (bytes[0] & 128) / 128;
	if (buffer[0] == 0)
		offset = 1;
	return offset;
}