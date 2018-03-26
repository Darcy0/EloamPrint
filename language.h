/*
   Author:     GZM
   Date:       2013-5-13
   describe:   class about language in excel and xml file



*/



#pragma once;

//#include "stdafx.h"
#include <stdio.h>
#include <vector>
#include ".\tinyxml.h"
#include <string.h>

class CItem
{
public:
	char ID[200];
	char lang[200];
	char abbr[200];
	char describe[256];
	//string ID;
	//string lang;
	//string abbr;
	//string describe;
	

	CItem();
	CItem(char* ID, char* lang, char* abbr,char* describe);
	CItem(const CItem& item);
	~CItem();

};


class CLanguage
{
public:
	char langID[200];
	char langName[200];
	char describe[256];
	::std::vector<CItem> Item;

    CLanguage();
	CLanguage( char* langID, char* langName, char* describe, std::vector<CItem>& item );
	CLanguage(const CLanguage& lang);
	~CLanguage();

};

extern CLanguage g_language;
extern std::vector<CLanguage> g_vecLangs;
extern void  readXml(const char* path);
extern int  getCountryLanguage(char *cid);
extern char * getLanguageItem(char* Lid );
extern char *getLanguageFname(char*lid);
