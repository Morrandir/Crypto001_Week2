// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AES128CBC.h"
#include "AES128CTR.h"
#include <iostream>
#include <string>

#undef max

using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
	AES* AESCipher = NULL;
	string msg;
	string key;
	char* temp_msg_hex;
	char* temp_key_hex;
	int mode = -1;

	while (mode != 0 && mode != 1) {
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

		cin.clear();
		cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
	}

	if (NULL == AESCipher) {
		cout << "Error allocating memory for cipher engine, program exiting...\n";
		return 0;
	}

	cout << "please provide the key: ";
	cin >> key;

	cout << "please provide the message text: ";
	cin >> msg;
	
	temp_key_hex = new char[key.length() + 1];
	for (int i = 0; i < key.length(); i++) {
		temp_key_hex[i] = key.c_str()[i];
	}
	temp_key_hex[key.length()] = 0;

	temp_msg_hex = new char[msg.length() + 1];
	for (int i = 0; i < msg.length(); i++) {
		temp_msg_hex[i] = msg.c_str()[i];
	}
	temp_msg_hex[msg.length()] = 0;

	AESCipher->setKey(temp_key_hex);

	mode = -1;
	while (/* mode != 0 && */ mode != 1) {
		cout << "Please choose whether you want to encrypt or decrypt (1 for decrypt, encrypt is not ready): ";
		cin >> mode;

		switch (mode) {
		case 0:
			AESCipher->setPT(temp_msg_hex);
			AESCipher->Encrypt();
			break;
		case 1:
			AESCipher->setCT(temp_msg_hex);
			AESCipher->Decrypt();
			break;
		}

		cin.clear();
		cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
	}


	return 0;
}

