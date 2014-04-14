// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AES128CBC.h"


int _tmain(int argc, _TCHAR* argv[])
{
	AES128CBC AESCipher;
	char* temp_CT_hex =	"4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81";
	char* temp_key_hex = "140b41b22a29beb4061bda66b6747e14";

	AESCipher.setCT(temp_CT_hex);
	AESCipher.setKey(temp_key_hex);
	AESCipher.Decrypt();

	return 0;
}

