#include "language.h"
#include <windows.h>
#include <QtGui>
#include "qmessagebox.h"
//#include <afxwin.h>


CLanguage g_language;
std::vector<CLanguage> g_vecLangs;
CItem::CItem()
{
	memset(this->ID,0,20*sizeof(char));
    memset(this->lang,0,200*sizeof(char));
	memset(this->abbr,0,200*sizeof(char));
	memset(this->describe,0,256*sizeof(char));
}
CItem::CItem(char* ID, char* lang, char* abbr, char* describe)
{
	strcpy(this->ID , ID);
	strcpy(this->lang ,lang);
	strcpy(this->abbr , abbr);
	strcpy(this->describe , describe);
}

CItem::CItem(const CItem &item)
{
	strcpy(this->ID , item.ID);
	strcpy(this->lang , item.lang);
	strcpy(this->abbr , item.abbr);
	strcpy(this->describe , item.describe);
}

CItem::~CItem()
{

}

CLanguage::CLanguage()
{

}

CLanguage::CLanguage(char* langID, char* langName, char* describe, std::vector<CItem>& item)
{
	strcpy(this->langID , langID);
	strcpy(this->langName , langName);
	strcpy(this->describe , describe);
	this->Item = item;

}

CLanguage::CLanguage(const CLanguage &lang)
{
	strcpy(this->langID , lang.langID);
	strcpy(this->langName , lang.langName);
	strcpy(this->describe , lang.describe);
	this->Item = lang.Item;

}

CLanguage::~CLanguage()
{
}


void  readXml(const char * xmlfile)
{	
	//const char* xmlfile1 = ".\\副本新高拍仪字符表-翻译2013.5.24pm-R1.xml";			
	TiXmlDocument doc;

    if ( !doc.LoadFile(xmlfile) )
	{
		QMessageBox::information(0,QString::fromLocal8Bit("tishi"),QString::fromLocal8Bit("打开文档失败"),0);
		//AfxMessageBox(L"写入Xml成功");
		//QMessageBox::information( QApplication::desktop(), ("提示"), QObject::tr("打开文档失败！") );
		//QMessageBox::information( QApplication::desktop(), ("提示1"), xmlfile );
		return;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* langpackElement;
    TiXmlElement* langsElement;
	//TiXmlAttribute* pAttrib;                   //属性
	TiXmlElement*   ItemElement;
	TiXmlElement*   IDElement;
	

	langpackElement = hDoc.FirstChildElement("Language_package").Element();
	if (!langpackElement)
		return;
    for ( langsElement = langpackElement->FirstChildElement();
		  langsElement;
		  langsElement = langsElement->NextSiblingElement()
		)
	{ //遍历各国语言
	     char chid[20];
	     char chlang[200];
		 memset(chid,0,20*sizeof(char));
		 memset(chlang,0,200*sizeof(char));
		 if (langsElement->Attribute("id"))
	         strcpy( chid , langsElement->Attribute("id"));
		 if (langsElement->Attribute("name"))
             strcpy( chlang , langsElement->Attribute("name"));


		 std::vector<CItem> m_vecItem;

	     for ( ItemElement = langsElement->FirstChildElement();
		       ItemElement;
		       ItemElement = ItemElement->NextSiblingElement()
		      )
	     { //遍历各个语言的Item
			
			  CItem item;
			  

			  char chtemp[256];

		      for (  IDElement = ItemElement->FirstChildElement();
			         IDElement;
			         IDElement = IDElement->NextSiblingElement()
			  )
		     { //遍历各个Item的子元素
			     const char* ch =  IDElement->GetText();
			     if ( ch )
					 strcpy(  chtemp, ch);
			     else
				     strcpy(  chtemp,"");

				 if ( 0 == strcmp("ID",IDElement->Value()) )
					 strcpy (item.ID ,chtemp);
				 else if ( 0 == strcmp("Lang",IDElement->Value()) )
					 strcpy (item.lang ,chtemp);
				 else if ( 0 == strcmp("Abbr",IDElement->Value()) )
					 strcpy(item.abbr, chtemp);
				 else if ( 0 == strcmp("Describe",IDElement->Value()) )
					 strcpy(item.describe, chtemp);
				
			  }

			  m_vecItem.push_back(item);

	     }

		 CLanguage lang(chid, chlang, "", m_vecItem);
 		 g_vecLangs.push_back(lang);
	}
	;
}

int  getCountryLanguage(char *cid)
{
	int i;
	for(i=0;i<g_vecLangs.size();i++)
	{
		if(0== strcmp(cid,g_vecLangs[i].langID))				
			{
				memset(g_language.langID,0,200);
				memset(g_language.describe,0,256);
				memset(g_language.langName,0,200);
				g_language.Item.clear();
				strcpy(g_language.langID,g_vecLangs.at(i).langID);
				strcpy(g_language.describe,g_vecLangs.at(i).describe);
				strcpy(g_language.langName,g_vecLangs.at(i).langName);				
				g_language.Item=g_vecLangs.at(i).Item;	
				return 1;
			}
		
	}
		return -1;   //传入的id不正确；

}

char * getLanguageItem( char* lid)
{	
	int j;
	char strr[200];
	for(j=0;j<g_language.Item.size();j++)
	{
		if(0 == strcmp(lid,g_language.Item.at(j).ID))
		//if(0== ::_stricmp(lid,g_language.Item.at(j).ID))
		{
			if(strlen(g_language.Item.at(j).abbr)==0)
				//strcpy(strr, g_language.Item.at(j).lang);
			::strcpy(strr, g_language.Item.at(j).lang);		
			else
				::strcpy(strr, g_language.Item.at(j).abbr);		
			return strr;
		}		
	}	
	return NULL;
}

char *getLanguageFname(char *Lid)
{
	int j;
	char getName[200];
	for(j=0;j<g_language.Item.size();j++)
	{
		if(0 ==strcmp(Lid,g_language.Item.at(j).ID))
		{
			::strcpy(getName,g_language.Item.at(j).lang);
			return getName;
		}
	}
	return NULL;
}

	/*int main()
	{
		readXml();
		char test[200];
		char cid[50] = "0";
		char lid[50] ="STRNG_LIANGTIAN_V4.0";	
		getCountryLanguage(cid);
		if(getLanguageItem(lid))
		strcpy(test, getLanguageItem(lid));
		return 0;
	}*/