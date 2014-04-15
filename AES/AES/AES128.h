#pragma once
#include "AES.h"

#define BLOCK_SIZE	16
#define COLUMN_SIZE	(BLOCK_SIZE / 4)
#define EXPANDED_KEY_LENGTH	176
#define ROUNDS 10

class AES128 :
	public AES
{
public:
	AES128();
	~AES128();

	// Take a key text of hex string, tranform it to binary string and store in the class member key;
	void setKey(char* key_hex);


	// Decrypt() decrypts this->CT using this->key, provided that both members have been set correctly.
	void Decrypt();

protected:
	BYTE key[COLUMN_SIZE][4];
	BYTE round_key[ROUNDS + 1][COLUMN_SIZE][4];

	// Generate the expanded round keys.
	void keySchedule();

};

