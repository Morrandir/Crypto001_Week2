#include "stdafx.h"
#include "AES128CTR.h"


AES128CTR::AES128CTR() {
	if (this->PT_holder != NULL) {
		delete[] this->PT_holder;
		this->PT_holder = NULL;
	}
	if (this->CT_holder != NULL) {
		delete[] this->CT_holder;
		this->CT_holder = NULL;
	}
}


AES128CTR::~AES128CTR()
{
}

void AES128CTR::setCT(char* CT_hex) {
	char IV_hex[BLOCK_SIZE * 2 + 1];
	char* CT_hex_noIV;
	for (int i = 0; i < BLOCK_SIZE * 2; i++) {
		IV_hex[i] = CT_hex[i];
	}
	IV_hex[BLOCK_SIZE * 2] = 0;
	this->setIV(IV_hex);

	// for AES CTR decryption, we need to encrypt the IV, so here we actually
	// set the plain text as IV.
	AES::setPT(IV_hex);

	CT_hex_noIV = new char[strlen(CT_hex) - BLOCK_SIZE * 2 + 1];
	for (unsigned int i = 0; i < strlen(CT_hex) - BLOCK_SIZE * 2; i++) {
		CT_hex_noIV[i] = CT_hex[BLOCK_SIZE * 2 + i];
	}
	CT_hex_noIV[strlen(CT_hex) - BLOCK_SIZE * 2] = 0;

	// the real cipher text will be stored in CT_holder.
	this->CT_holder = hex2bin(CT_hex_noIV);
	this->size_CT = strlen(CT_hex_noIV) / 2;

	if (CT_hex_noIV != NULL) {
		delete[] CT_hex_noIV;
		CT_hex_noIV = NULL;
	}
}

void AES128CTR::setIV(char* IV_hex) {
	stream2block(hex2bin(IV_hex), this->IV, COLUMN_SIZE);
}

void AES128CTR::Decrypt() {

	int size_CT_holder = this->size_CT;
	int nBlocks = (size_CT_holder % BLOCK_SIZE) ? size_CT_holder / BLOCK_SIZE + 1 : size_CT_holder / BLOCK_SIZE;

	this->PT_holder = new BYTE[nBlocks * BLOCK_SIZE + 1];

	for (int i = 0; i < nBlocks; i++) {
		AES128::Encrypt();

		bool no_carry;

		for (int j = this->size_PT - 1; j >= 0; j--) {
			if (no_carry = ++this->PT[j]) {
				break;
			}
		}


		for (int j = 0; j < BLOCK_SIZE; j++) {
			this->PT_holder[i * BLOCK_SIZE + j] = this->CT[j];
		}
	}

	for (int i = 0; i < size_CT_holder; i++) {
		this->PT_holder[i] = this->PT_holder[i] ^ this->CT_holder[i];
	}
	this->PT_holder[size_CT_holder] = 0;

	if (this->PT != NULL) {
		delete[] this->PT;
	}
	this->PT = this->PT_holder;

	this->getPT();
}