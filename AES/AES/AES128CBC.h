#pragma once
#include "AES128.h"
class AES128CBC :
	public AES128
{
public:
	AES128CBC();
	~AES128CBC();
	void Decrypt();
	void setCT(char* CT_hex);
	void setIV(char* IV_hex);
	void setIV(BYTE IV_block[COLUMN_SIZE][4]);

protected:
	BYTE IV[COLUMN_SIZE][4];

};

