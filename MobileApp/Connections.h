/*
 * Connection.h
 *
 *  Created on: 5 juil. 2013
 *      Author: gdr
 */

#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

#include <Wormhole/HighLevelHttpConnection.h>
#include <Wormhole/MessageStreamJSON.h>

#include "IncludeFile.h"
#include "Util.h"

//#define BUFSIZE 1024
using namespace Wormhole;
//using namespace MAUtil;
//using namespace NativeUI;


class Connections : public HighLevelHttpConnection
{
public:

	enum eFonction {
		//trackingTab
		TRACKING_LIST = 0,
		//alertTab
		PLUGIN_LIST,
		INFORMATION_LIST,
		ALERT_LIST,
		ASSET_LIST,
		OPERATOR_LIST,
		UNIT_INFO,
		SUB_UNIT_INFO,
		USER_LIST,
		MEDIA_VALUE_LIST,
		MEDIA_TYPE,
		POST_MEDIA_SPE,
		POST_ALERT,
		SEARCH_INFO,
		ALERT_INFO,
		ALERT_RECIPIENT_LIST,
		UNITS_TYPES,
		DELETE_ALERT_LIST,
		//authenticationTab
		USER_TOKEN,
		MEDIAS_LIST,
		POST_MEDIA_VALUE,
		POST_MEDIA_VALUE_VALIDATION,
		AUTHENTICATION_VALIDATION
	};


	/**
	 * Constructor.
	 */
	Connections();

	/**
	 * Destructor.
	 *
	 */
	virtual ~Connections();

	void connectUrl(String url, eFonction fct, int verb = GET, String jsonMessage = "");
	virtual void connectUrl1() = 0;
	void dataDownloaded(MAHandle, int);
	virtual void dataDownload1(MAUtil::YAJLDom::Value* root, int result, eFonction fonction) = 0;
	int deleteJsonRequest(const char* url);

private:
	eFonction fonction;
	bool mIsConnected;
	int connERR;
};

#endif /* CONNECTIONS_H_ */
