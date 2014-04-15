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