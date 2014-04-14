#pragma once
#include "Windows.h"

class AES
{
public:
	AES();
	~AES();
protected:
	BYTE* PT;
	BYTE* CT;
public:
	// Take a cipher text of hex string, transform it to binary string and assign to class member CT;
	void setCT(char* CT_hex);

};



BYTE* hex2bin(char* string_hex);