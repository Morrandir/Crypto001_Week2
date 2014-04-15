#include "stdafx.h"
#include "AES.h"


AES::AES() {
}


AES::~AES()
{
	if (PT != NULL) {
		delete[] PT;
		PT = NULL;
	}
	if (CT != NULL) {
		delete[] CT;
		CT = NULL;
	}
}

// Take a cipher text of hex string, transform it to binary string and assign to class member CT;
void AES::setCT(char* CT_hex) {

	this->CT = hex2bin(CT_hex);
	this->size_CT = strlen(CT_hex) / 2;

}

// Take a stream of hex string and output a steam of binary string;
BYTE* hex2bin(char* string_hex) {
	
	int hex_length = strlen(string_hex);
	BYTE* temp_CT_bin = new BYTE[hex_length / 2];

	for (int i = 0; i < hex_length / 2; i++) {
		char placeholder[2];
		placeholder[0] = string_hex[2 * i];
		placeholder[1] = string_hex[2 * i + 1];
		temp_CT_bin[i] = strtoul(placeholder, 0, 16);
	}

	return temp_CT_bin;
}

void stream2block(BYTE* stream, BYTE block[][4], int column_size) {
	
	for (int i = 0; i < column_size; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = stream[4 * i + j];
		}
	}

}

void block2stream(BYTE block[][4], BYTE* stream, int column_size) {

	for (int i = 0; i < column_size; i++) {
		for (int j = 0; j < 4; j++) {
			stream[4 * i + j] = block[i][j];
		}
	}
}

void copyBlock(BYTE src[][4], BYTE dst[][4], int column_size) {

	for (int i = 0; i < column_size; i++) {
		for (int j = 0; j < 4; j++) {
			dst[i][j] = src[i][j];
		}
	}
}

void revertBlock(BYTE block[][4], int column_size) {
	BYTE* temp = new BYTE[column_size * 4];
	if (NULL == temp) {
		printf("error allocating memory during reverting a block...\n");
		return;
	}

	block2stream(block, temp, column_size);

	for (int i = 0; i < column_size; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = temp[column_size * j + i];
		}
	}


	if (temp != NULL) {
		delete[] temp;
		temp = NULL;
	}
}

void rotate(BYTE section[4], int step) {
	BYTE temp[4] = {0, 0, 0, 0};

	for (int i = 0; i < step; i++) {
		temp[i] = section[i];
	}
		
	for (int i = 0; i < 4; i++) {
		section[i] = ((i + step) % 4 < step) ? temp[(i + step) % 4] : section[i + step];
	}

}

void substitute(BYTE block[][4], int column_size) {
	for (int i = 0; i < column_size; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = s[block[i][j]];
		}
	}
}

void substitute_inv(BYTE block[][4], int column_size) {
	for (int i = 0; i < column_size; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = inv_s[block[i][j]];
		}
	}
}

void rcon(BYTE section[4], int round) {
	BYTE RCON[4] = { 0, 0, 0, 0 };

	RCON[0] = Rcon[round];

	sectionXOR(RCON, section);

}

void sectionXOR(BYTE src[4], BYTE dst[4]) {

	for (int i = 0; i < 4; i++) {
		dst[i] = dst[i] ^ src[i];
	}

}

void blockXOR(BYTE src[][4], BYTE dst[][4], int column_size) {

	for (int i = 0; i < column_size; i++) {
		sectionXOR(src[i], dst[i]);
	}
}


void AESround_inv(BYTE ct_block[][4], BYTE round_key[][4], int column_size, int iRound, int total_rounds) {
	
	blockXOR(round_key, ct_block, column_size);

	if (iRound != total_rounds) {
		for (int i = 0; i < column_size; i++) {
			mixColumn_inv(ct_block[i]);
		}
	}

	shiftRow_inv(ct_block, column_size);

	substitute_inv(ct_block, column_size);
}

void mixColumn_inv(BYTE ct_column[4]) {
	BYTE temp[4];

	for (int i = 0; i < 4; i++) {
		temp[i] = ct_column[i];
	}

	ct_column[0] = mc14[temp[0]] ^ mc11[temp[1]] ^ mc13[temp[2]] ^ mc9[temp[3]];
	ct_column[1] = mc9[temp[0]] ^ mc14[temp[1]] ^ mc11[temp[2]] ^ mc13[temp[3]];
	ct_column[2] = mc13[temp[0]] ^ mc9[temp[1]] ^ mc14[temp[2]] ^ mc11[temp[3]];
	ct_column[3] = mc11[temp[0]] ^ mc13[temp[1]] ^ mc9[temp[2]] ^ mc14[temp[3]];
}

void shiftRow_inv(BYTE ct_block[][4], int column_size) {
	revertBlock(ct_block, column_size);

	for (int i = 1; i < column_size; i++) {
		rotate(ct_block[i], 4 - i);
	}

	revertBlock(ct_block, column_size);
}