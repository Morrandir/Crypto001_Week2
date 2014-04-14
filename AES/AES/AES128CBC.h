#pragma once
#include "AESCBC.h"

#define COLUMN_SIZE	4
#define EXPANDED_KEY_LENGTH	176
#define ROUNDS 11

class AES128CBC :
	public AESCBC
{
public:
	AES128CBC();
	~AES128CBC();

	// Take a key text of hex string, tranform it to binary string and store in the class member key;
	void setKey(char* key_hex);


	// Decrypt() decrypts this->CT using this->key, provided that both members have been set correctly.
	void Decrypt();

private:
	BYTE key[4][COLUMN_SIZE];
	BYTE round_key[ROUNDS - 1][4][COLUMN_SIZE];

	// Generate the expanded round keys.
	void keySchedule();

};

void copyBlock(BYTE src[4][COLUMN_SIZE], BYTE dst[4][COLUMN_SIZE]);
