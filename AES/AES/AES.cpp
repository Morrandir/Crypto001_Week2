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

// Take a cipher text of hex string, transform it to binary string and assign to class member CT;
void AES::setCT(char* CT_hex) {

	this->CT = hex2bin(CT_hex);

}
