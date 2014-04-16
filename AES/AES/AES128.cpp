#include "stdafx.h"
#include "AES128.h"


AES128::AES128()
{
}


AES128::~AES128()
{
}



// Take a key text of hex string, tranform it to binary string and store in the class member key;
void AES128::setKey(char* key_hex) {
	
	BYTE* temp_key_bin;

	if (strlen(key_hex) != BLOCK_SIZE * 2) {
		printf("AES-128: Key is not 16 bytes long, please provide the correct key...\n");
		return;
	}

	temp_key_bin = hex2bin(key_hex);

	stream2block(temp_key_bin, this->key, COLUMN_SIZE);


	if (temp_key_bin != NULL) {
		delete[] temp_key_bin;
		temp_key_bin = NULL;
	}
}

// Decrypt() decrypts this->CT using this->key, provided that both members have been set correctly.
void AES128::Decrypt() {

	BYTE* plaintext;
	if (this->CT == NULL) {
		printf("please provide cipher text...\n");
		return;
	}
	if (this->size_CT % BLOCK_SIZE != 0) {
		printf("cipher text size if not n*16 bytes for AES128...\n");
		return;
	}
	if (this->key == NULL) {
		printf("please provide key...\n");
		return;
	}

	plaintext = new BYTE[this->size_CT];
	if (plaintext != NULL) {
		if (this->PT != NULL) {
			delete[] this->PT;
			this->PT = NULL;
		}
		this->PT = plaintext;
	}
	else {
		printf("error allocating memory for plain text...\n");
		return;
	}

	this->keySchedule();

	for (int i = 0; i < this->size_CT; i += BLOCK_SIZE) {

		BYTE ct_block[COLUMN_SIZE][4];
		stream2block(this->CT + i, ct_block, COLUMN_SIZE);

		for (int iRound = ROUNDS; iRound > 0; iRound--) {
			AESround_inv(ct_block, this->round_key[iRound], COLUMN_SIZE, iRound, ROUNDS);
		}

		blockXOR(this->round_key[0], ct_block, COLUMN_SIZE);
		block2stream(ct_block, plaintext, COLUMN_SIZE);

		plaintext += BLOCK_SIZE;
	}

}

// Encrypt() encrypts this->PT using this->key, privided that both members have been set correctly.
void AES128::Encrypt() {

	BYTE* ciphertext;

	if (this->PT == NULL) {
		printf("please provide plain text...\n");
		return;
	}
	if (this->size_PT % BLOCK_SIZE != 0) {
		printf("plain text block is not n*16 bytes for AES128...\n");
		return;
	}

	if (this->key == NULL) {
		printf("please provide key...\n");
		return;
	}

	//// padding is for AES CBC, not here;
	//plaintext_padding = new BYTE[this->size_PT + BLOCK_SIZE - this->size_PT % BLOCK_SIZE];
	//for (int i = 0; i < this->size_PT; i++) {
	//	plaintext_padding[i] = this->PT[i];
	//}
	//for (int i = this->size_PT; i < this->size_PT + BLOCK_SIZE - this->size_PT % BLOCK_SIZE; i++) {
	//	plaintext_padding[i] = BLOCK_SIZE - this->size_PT % BLOCK_SIZE;
	//}
	//this->size_PT = this->size_PT + BLOCK_SIZE - this->size_PT % BLOCK_SIZE;


	ciphertext = new BYTE[this->size_PT]; // ciphertext should have same size of PT;
	if (ciphertext != NULL) {
		if (this->CT != NULL) {
			delete[] this->CT;
			this->CT = NULL;
		}
		this->CT = ciphertext;
	}
	else {
		printf("error allocating memory for cipher text...\n");
		return;
	}

	this->keySchedule();

	for (int i = 0; i < this->size_PT; i += BLOCK_SIZE) {

		BYTE pt_block[COLUMN_SIZE][4];
		stream2block(this->PT + i, pt_block, COLUMN_SIZE);

		blockXOR(this->round_key[0], pt_block, COLUMN_SIZE);
		for (int iRound = 1; iRound <= ROUNDS; iRound++) {
			AESround(pt_block, this->round_key[iRound], COLUMN_SIZE, iRound, ROUNDS);
		}


		block2stream(pt_block, ciphertext, COLUMN_SIZE);

		ciphertext += BLOCK_SIZE;
	}

}


// Generate the expanded round keys.
void AES128::keySchedule() {

	copyBlock(this->key, this->round_key[0], COLUMN_SIZE);

	for (int iRcon = 1; iRcon <= ROUNDS; iRcon++) {
		BYTE temp[1][4];
		
		// generate the first column for each round_key block:
		copyBlock(this->round_key[iRcon - 1] + 3, temp, 1);
		rotate(temp[0], 1);
		substitute(temp, 1);
		rcon(temp[0], iRcon);
		sectionXOR(this->round_key[iRcon - 1][0], temp[0]);
		copyBlock(temp, this->round_key[iRcon], 1);

		// generate the remaining 3 columns for each round_key block:
		for (int i = 1; i < 4; i++) {
			copyBlock(this->round_key[iRcon] + i - 1, temp, 1);
			sectionXOR(this->round_key[iRcon - 1][i], temp[0]);
			copyBlock(temp, this->round_key[iRcon] + i, 1);
		}
	}

}