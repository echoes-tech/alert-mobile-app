/*
 * TrackingTab.h
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */

#ifndef TRACKINGTAB_H_
#define TRACKINGTAB_H_

//#include <ma.h>
//#include <MAUtil/String.h>		// C++ String class
//#include <MAUtil/Moblet.h>		// Moblet class

//#include <conprint.h>			// lprintfln for logging

#include <NativeUI/Widgets.h>// Include all widgets
#include <MAP/DateTime.h>

#include "conf.h"
#include "Util.h"
#include "notification/LocalNotification.h"
#include "notification/NotificationManager.h"
#include "Notification/LocalNotificationListener.h"
//#include <IX_CELLID.h>

#include "resource/Convert.h"
#include "MAHeaders.h"
#include "IncludeFile.h"
#include "Page.h"
#include "ActivityPage.h"

#include "Connections.h"

//#include <MTXml/MTXml.h>

//#define BUFSIZE 1024

//using namespace Wormhole;
//using namespace MAUtil;
using namespace NativeUI;
using namespace MAPUtil;
using namespace Notification;

using Notification::NotificationManager;
using Notification::LocalNotification;

class TrackingTab : public NativeUI::Screen/*, public HighLevelHttpConnection*/, public LocalNotificationListener, public Connections
{
public:

//	enum eTrakingTab {
//			TRACKING_LIST = 0,
//		};

	/**
	 * Constructor.
	 */
	TrackingTab(int language, String loginToken, eScreenResolution screenResolution, long long idMobile);

	/**
	 * Destructor.
	 */
	virtual ~TrackingTab();

	void parseJSONTrackingAlert(MAUtil::YAJLDom::Value* root);

//	void connectUrl(String url, eTrakingTab fct);
//	void dataDownloaded(MAHandle , int );

	virtual void connectUrl1();
	virtual void dataDownload1(MAUtil::YAJLDom::Value*, int result, eFonction fonction);

	virtual void didReceiveLocalNotification(Notification::LocalNotification& mNotif){lprintfln("LOCALE notif"); LocalNotification test = mNotif;maNotificationLocalUnschedule(test.getHandle()); maNotificationLocalDestroy(test.getHandle());};

	void createUI();

	void orientationChange(int screenOrientation);

	void runTimerEvent();

private:
	ActivityPage* mActivityPage;

	MAHandle notificationHandle; //test
	String _LOGINTOKEN;
	long long _IDMOBILE;
	int LANGUAGE;
//	int connERR;

//	eFonction fonction;
//	char mBuffer[BUFSIZE];
//	bool mIsConnected;
	bool bCreateUI;

	int count;
	String contentString;

	Page* mainLayout;
	int nbNewAlert;
//	Label *lTrackingTitle;
	Label *lListNoAlert;
	ListView *lValert;
	MAPUtil::DateTime dateLastSend;

	 MAUtil::Map<int, MAUtil::String> mapTrackingAlertDesc;
	 MAUtil::Map<int, String> mapTrackingAlertDate;

	 MAUtil::Map<int,HorizontalLayout*> mapHLTA;
	 MAUtil::Map<int,ListViewItem*> mapLVITA;
	 MAUtil::Map<int,Label*> mapLTADesc;
	 MAUtil::Map<int,Label*> mapLTAHeure;

	STime lastSendAlert;

	LocalNotification* notification;
};

class TrackingStackScreen : public NativeUI::StackScreen, public NativeUI::StackScreenListener
{
public:
	TrackingStackScreen(int language, String loginToken, eScreenResolution screenResolution, int idMobile):StackScreen(), LANGUAGE(language){
		addStackScreenListener(this);
		if(getPlatform() == IOS){
			setTitle(Convert::tr(TRACKING_ALERT_TAB_EN + LANGUAGE));
		}
		setPushTransition(MAW_TRANSITION_TYPE_NONE,0);
		setIcon(ICON_TRACKING + screenResolution);
		mTrackingTab = new TrackingTab(language, loginToken, screenResolution, idMobile);
		push(mTrackingTab);
	};

	void runTimerEvent(){
		mTrackingTab->runTimerEvent();
	}

	virtual void stackScreenScreenPopped( StackScreen* stackScreen, Screen* fromScreen, Screen* toScreen){
	};

	void orientationChange(int screenOrientation){
		if(getPlatform() != IOS){
			if (screenOrientation == MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) {
		//		lprintfln("Orientation paysage");
				Screen::setTitle(Convert::tr(TRACKING_ALERT_TAB_EN + LANGUAGE));
			} else // Portrait
			{
		//		lprintfln("Orientation Portrait");
				Screen::setTitle("");
			}
			mTrackingTab->orientationChange(screenOrientation);
		}
	};

private:
	int LANGUAGE;
	TrackingTab *mTrackingTab;
};

#endif /* TRACKINGTAB_H_ */
