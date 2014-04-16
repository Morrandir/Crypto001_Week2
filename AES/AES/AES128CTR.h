#pragma once
#include "AES128.h"
class AES128CTR :
	public AES128
{
public:
	AES128CTR();
	~AES128CTR();
	void setCT(char* CT_hex);
	void setIV(char* IV_hex);
	void Decrypt();

protected:

	// AES CTR will not encrypt/decrypt the PT/CT directly but the IV,
	// so here after receiving PT/CT we put them in these holders;
	BYTE* CT_holder;
	BYTE* PT_holder;

	BYTE IV[COLUMN_SIZE][4];
};

