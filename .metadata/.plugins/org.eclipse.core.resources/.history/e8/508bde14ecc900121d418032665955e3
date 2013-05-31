/*
 * Connection.cpp
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */

#include "HTTPConnect.h"
#include <maheap.h>
#include <conprint.h>
#include <mavsprintf.h>
#include <IX_PIM.h>
#include <maapi.h>

#include <NativeUI/WebView.h>
#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>

//#include "AlertTab.h"

#define EXAMPLE_URL "https://alert-api.echoes-tech.com/plugins/1/sources/1/searches/?login=thomas.saquet@echoes-tech.com&password=147258369aA"

	#define LOG printf;

using namespace std;
using namespace Wormhole;
using namespace NativeUI;

HTTPConnect::HTTPConnect()
{
	mIsConnected = false;
}

void HTTPConnect::connectUrl(String url, String fct, Widget* ptr)
{
//	AlertTab* tmp = ptr;
	lprintfln("connectUrl");
	if(mIsConnected == false)
	{
	mIsConnected = true;
	fonction = fct;
	this->get(url.c_str());
	}
	else
	{
		lprintfln("Déjà connecté: %s", fonction.c_str());
	}
}

void HTTPConnect::dataDownloaded(MAHandle data , int result)
{
	lprintfln("dataDownloaded");


//		    lprintfln("datasize: %d \n Data: %s %d\n ",strlen(jsonData),jsonData);

//switch (fonction) {
//	case "parseJSONPlugin":
//	AlertTab* tmp = pointeur;
//		tmp->parseJSONPlugin(data);
//		break;
//	default:
//		break;
//	}
//this->close();
}


	void HTTPConnect::parsingJSON()
		{
		MAUtil::YAJLDom::Value* root = YAJLDom::parse((const unsigned char*)contentString.c_str(),contentString.size());
						                        // Traverse the Json tree and print data.
						                        // Check that the root is valid.
						                        // The root type should have type with above data ARRAY.
						                        if (NULL == root
						                            || YAJLDom::Value::NUL == root->getType()
						                            || YAJLDom::Value::ARRAY != root->getType())
						                        {
						                            lprintfln("Root node is not valid\n");

						                        }else{
						                            lprintfln("Root node is valid :) \n");
						                        }

			lprintfln("%d",root->getNumChildValues());
			for(int idx = 0; idx<=root->getNumChildValues() ; idx++  )
			{

				MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
				MAUtil::YAJLDom::Value* valueTmp1 = valueTmp->getValueForKey("id");

				String test1 = valueTmp->getValueForKey("is_static")->toString();
				int test = valueTmp1->getValueForKey("search_id")->toInt();
				lprintfln(test1.c_str());
				lprintfln("%d",test);
			}
		}
