// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AES128CBC.h"
#include "AES128CTR.h"
#include <iostream>
#include <string>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	AES128* AESCipher;
	string msg;
	string key;
	char* temp_CT_hex;
	char* temp_key_hex;
	int mode = -1;

	while (mode != 0 || mode != 1) {
		cout << "Please choose the AES mode (0 for CBC, 1 for CTR): ";
		cin >> mode;
		
		switch (mode) {
		case 0:
			AESCipher = new AES128CBC();
			break;
		case 1:
			AESCipher = new AES128CTR();
			break;
		}
	}

	if (NULL == AESCipher) {
		cout << "Error allocating memory for cipher engine, program exiting...\n";
		return 0;
	}

	cout << "please provide the key: ";
	cin >> key;

	cout << "please provide the message text: ";
	cin >> msg;
	
	temp_key_hex = new char[key.length];
	//key.

	AESCipher->setKey(temp_key_hex);

	mode = -1;
	while (mode != 0 || mode != 1) {
		cout << "Please choose whether you want to encrypt or decrypt (0 for encrypt, 1 for decrypt): ";
		cin >> mode;

		switch (mode) {
		case 0:
			AESCipher->Encrypt();
			break;
		case 1:
			AESCipher->setCT(temp_CT_hex);
			AESCipher->Decrypt();
			break;
		}
	}


	return 0;
}

