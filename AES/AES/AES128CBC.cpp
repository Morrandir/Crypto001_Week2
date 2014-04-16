#include "stdafx.h"
#include "AES128CBC.h"


AES128CBC::AES128CBC()
{
}


AES128CBC::~AES128CBC()
{
}


void AES128CBC::Decrypt() {
	
	BYTE* plaintext;
	if (this->CT == NULL) {
		printf("please provide cipher text...\n");
		return;
	}
	if (this->size_CT % BLOCK_SIZE != 0) {
		printf("cipher text size if not n*16 bytes, make sure you provided the correct cipher text...\n");
		return;
	}
	if (this->key == NULL) {
		printf("please provide key...\n");
		return;
	}

	plaintext = new BYTE[this->size_CT];
	if (plaintext != NULL) {
		this->PT = plaintext;
	}
	else {
		printf("error allocating memory for plain text...\n");
		return;
	}

	this->keySchedule();

	for (int i = 0; i < this->size_CT; i += BLOCK_SIZE) {

		BYTE ct_block[COLUMN_SIZE][4];
		BYTE IV_block[COLUMN_SIZE][4];
		stream2block(this->CT + i, ct_block, COLUMN_SIZE);
		copyBlock(this->IV, IV_block, COLUMN_SIZE);
		this->setIV(ct_block);

		for (int iRound = ROUNDS; iRound > 0; iRound--) {
			AESround_inv(ct_block, this->round_key[iRound], COLUMN_SIZE, iRound, ROUNDS);
		}

		blockXOR(this->round_key[0], ct_block, COLUMN_SIZE);
		blockXOR(IV_block, ct_block, COLUMN_SIZE);
		block2stream(ct_block, plaintext, COLUMN_SIZE);

		plaintext += BLOCK_SIZE;
	}

	// plaintext[-1] read out the padding number and then we use it as index to add an end of string the the plaintext.
	plaintext[-plaintext[-1]] = 0;

	this->getPT();
}

void AES128CBC::setCT(char* CT_hex) {
	char IV_hex[BLOCK_SIZE * 2 + 1];
	char* CT_hex_noIV;
	for (int i = 0; i < BLOCK_SIZE * 2; i++) {
		IV_hex[i] = CT_hex[i];
	}
	IV_hex[BLOCK_SIZE * 2] = 0;
	this->setIV(IV_hex);

	CT_hex_noIV = new char[strlen(CT_hex) - BLOCK_SIZE * 2 + 1];
	for (int i = 0; i < strlen(CT_hex) - BLOCK_SIZE * 2; i++) {
		CT_hex_noIV[i] = CT_hex[BLOCK_SIZE * 2 + i];
	}
	CT_hex_noIV[strlen(CT_hex) - BLOCK_SIZE * 2] = 0;

	AES::setCT(CT_hex_noIV);

	if (CT_hex_noIV != NULL) {
		delete[] CT_hex_noIV;
		CT_hex_noIV = NULL;
	}
}

void AES128CBC::setIV(char* IV_hex) {
	stream2block(hex2bin(IV_hex), this->IV, COLUMN_SIZE);
}

void AES128CBC::setIV(BYTE IV_block[COLUMN_SIZE][4]) {
	copyBlock(IV_block, this->IV, COLUMN_SIZE);
}