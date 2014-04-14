#include "stdafx.h"
#include "AES128CBC.h"


AES128CBC::AES128CBC()
{
}


AES128CBC::~AES128CBC()
{
}



// Take a key text of hex string, tranform it to binary string and store in the class member key;
void AES128CBC::setKey(char* key_hex) {

	BYTE* temp_key_bin = hex2bin(key_hex);
	for (int i = 0; i < COLUMN_SIZE; i++) {
		for (int j = 0; j < 4; j++) {
			this->key[j][i] = temp_key_bin[4 * i + j];
		}
	}

	if (temp_key_bin != NULL) {
		delete[] temp_key_bin;
		temp_key_bin = NULL;
	}
}

// Decrypt() decrypts this->CT using this->key, provided that both members have been set correctly.
void AES128CBC::Decrypt() {
	if (this->CT == NULL) {
		printf("please provide cipher text...\n");
		return;
	}
	if (this->key == NULL) {
		printf("please provide key...\n");
		return;
	}
	this->keySchedule();
}

// Generate the expanded round keys.
void AES128CBC::keySchedule() {

	copyBlock(this->key, this->round_key[0]);

	for (int iRcon = 1; iRcon < ROUNDS; iRcon++) {
		BYTE temp[4][1];
		
		for (int i = 0; i < 4; i++) {
			temp[i][0] = this->round_key[iRcon - 1][i][3];
		}
	}
}

void copyBlock(BYTE src[4][COLUMN_SIZE], BYTE dst[4][COLUMN_SIZE]) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < COLUMN_SIZE; j++) {
			dst[i][j] = src[i][j];
		}
	}
}