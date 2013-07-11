/*
 * TrackingTab.cpp
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */

#include "TrackingTab.h"

/**
 * Constructor.
 */
TrackingTab::TrackingTab(int language, String loginToken,
		eScreenResolution screenResolution, long long idMobile) :
		Screen(), LANGUAGE(language), _LOGINTOKEN(loginToken), _IDMOBILE(
				idMobile) {

	// Set icon of the stack screen.
	setIcon(ICON_TRACKING + screenResolution);

//	mIsConnected = false;
	createUI();
}

/**
 * Destructor.
 */
TrackingTab::~TrackingTab() {

}

void TrackingTab::runTimerEvent() {
	bCreateUI = false;
	String urlTmp = HOST;
	urlTmp += "/alerts/recipients/" + Convert::toString(_IDMOBILE)
			+ "/trackings/";
	urlTmp += _LOGINTOKEN;
	lprintfln(urlTmp.c_str());
	connectUrl(urlTmp, TRACKING_LIST);

	lprintfln("timer");
}

void TrackingTab::connectUrl1(){
//	Screen::setMainWidget(mActivityPage);
}

void TrackingTab::dataDownload1(MAUtil::YAJLDom::Value* root, int result, eFonction fonction)
{
	if (result == RES_OK) {
			parseJSONTrackingAlert(root);
			delete root;
		} else if (result == CONNERR_DNS) {
			lprintfln("AlertTab DataDownload result = %d", result);
			lprintfln("DNS resolution error.");
		} else if (result != 404) {
			lprintfln("TrackingTab DataDownload result = %d", result);
	}
}
//void TrackingTab::dataDownloaded(MAHandle data, int result) {
//	lprintfln("dataDownloaded trackingTab");
//	mIsConnected = false;
//	this->close();
//	if (result == RES_OK) {
//		connERR = 0;
//		char * jsonData = new char[maGetDataSize(data) + 1];
//		maReadData(data, jsonData, 0, maGetDataSize(data));
//		String jsonTmp = jsonData;
//		Convert::formatJSONBeforeParse(jsonTmp);
//
//		MAUtil::YAJLDom::Value* root = YAJLDom::parse(
//				(const unsigned char*) jsonTmp.c_str(), maGetDataSize(data));
//
//		parseJSONTrackingAlert(root);
//		delete jsonData;
//		delete root;
//		//this->close();
//	} else if (result == CONNERR_DNS) {
//		connERR++;
//		lprintfln("AlertTab DataDownload result = %d", result);
//		lprintfln("DNS resolution error.");
//	} else if (result != 404) {
//		connERR++;
//		lprintfln("TrackingTab DataDownload result = %d", result);
//	}
//	if (connERR >= 3) {
//		getSystemConnection();
//		String sMessage = "Connection ERROR. ERREUR:";
//		sMessage += Convert::toString(result);
//		maMessageBox("Connection Error", sMessage.c_str());
//	}
//}

void TrackingTab::parseJSONTrackingAlert(MAUtil::YAJLDom::Value* root) {
	// Traverse the Json tree and print data.
	// Check that the root is valid.
	// The root type should have type with above data ARRAY.

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");

	} else {
		lprintfln("Root node is valid :) \n");
		lprintfln("%d\n", root->getNumChildValues());
		lListNoAlert->setHeight(0);

		STime lastSendAlertTmp;
		if (root->getNumChildValues() > 0) {
			MAUtil::YAJLDom::Value* valueTmp0 = root->getValueByIndex(0);
			String dateTmp = valueTmp0->getValueForKey("send_date")->toString();
			lastSendAlertTmp = Convert::toSTime(dateTmp);
		}

		if (lastSendAlertTmp > lastSendAlert) {

			Screen::setMainWidget(mActivityPage);
			//////clean la memoire
			int index = mapLVITA.size();
			for (int idx1 = 0; idx1 < index; idx1++) {
				mapHLTA[idx1]->removeChild(mapLTAHeure[idx1]);
				mapHLTA[idx1]->removeChild(mapLTADesc[idx1]);
				delete mapLTAHeure[idx1];
				delete mapLTADesc[idx1];
				mapLVITA[idx1]->removeChild(mapHLTA[idx1]);
				delete mapHLTA[idx1];
				lValert->removeChild(mapLVITA[idx1]);
				delete mapLVITA[idx1];
			}
			mapHLTA.clear();
			mapLTAHeure.clear();
			mapLTADesc.clear();
			mapLVITA.clear();
			//////
			Screen::setMainWidget(mainLayout);
			for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
				MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);

				mapTrackingAlertDate[idx] = valueTmp->getValueForKey(
						"send_date")->toString();
				lprintfln(mapTrackingAlertDate[idx].c_str());
				String convert =
						valueTmp->getValueForKey("content")->toString();
				Convert::HTMLdecode(convert);

				mapLTADesc[idx] = new Label(convert);
				mapLTADesc[idx]->wrapContentHorizontally();
				mapLTADesc[idx]->wrapContentVertically();

				if (!bCreateUI) {
					String dateTmp1 =
							valueTmp->getValueForKey("send_date")->toString();
					STime lastSendAlertTmp1 = Convert::toSTime(dateTmp1);

					if (lastSendAlertTmp1 > lastSendAlert) {
						String config;
						tryToRead(config);
						Convert::formatJSONBeforeParse(config);
						MAUtil::YAJLDom::Value* root = YAJLDom::parse((const unsigned char*) config.c_str(),config.size());
						bool _vibrate = root->getValueForKey("vibrate")->toBoolean();
						bool _notification = root->getValueForKey("notification")->toBoolean();
						if(_notification)
						{
							lprintfln("_notification");
//							maToast("_notification", MA_TOAST_DURATION_SHORT);
						}
						else
						{
							lprintfln("!_notification");
//							maToast("!_notification", MA_TOAST_DURATION_SHORT);
						}
						if(_vibrate)
						{
							lprintfln("_vibrate");
//							maToast("_vibrate", MA_TOAST_DURATION_SHORT);
						} else {
							lprintfln("!_vibrate");
//							maToast("!_vibrate", MA_TOAST_DURATION_SHORT);
						}
						if (_notification) {
							//TODO gdr : http://jira.mosync.com/browse/MOSYNC-3332 bug pour les notifications elles ne sont pas supprimées de la bar des
			//					notifications correction lors de la sortie de la vesion 3.3.1
//				lprintfln("%d %d %d %d %d %d", lastSendAlertTmp1.year,lastSendAlertTmp1.mon,lastSendAlertTmp1.day,lastSendAlertTmp1.hour,lastSendAlertTmp1.min,lastSendAlertTmp1.sec);
//				lprintfln("%d %d %d %d %d %d", lastSendAlert.year,lastSendAlert.mon,lastSendAlert.day,lastSendAlert.hour,lastSendAlert.min,lastSendAlert.sec);

//						lprintfln("maNotificationLocalDestroy(notification->getHandle() %d" , maNotificationLocalDestroy(notification->getHandle()));
						maNotificationLocalUnschedule(notification->getHandle()); lprintfln("maNotificationLocalUnschedule(notification->getHandle() : %d" , maNotificationLocalUnschedule(notification->getHandle()));
//						if (notification == NULL) {
//						if(maNotificationLocalDestroy(notification->getHandle()) == -1)
//						{
//							nbNewAlert = 0;
//						}
//						if(notification != NULL)
//						{
//							delete notification;
//							lprintfln("notif delete");
//						}


//							notification->setFlag(Notification::NOTIFICATION_FLAG_AUTO_CANCEL);
//							notification->setDisplayFlag(NOTIFICATION_DISPLAY_DEFAULT);

//						}
//						nbNewAlert ++;
//						notification->setContentBody("Vous avez " + Convert::toString(nbNewAlert) + " nouvelle(s) alerte(s)");
							int res;
//							res = maNotificationRemove(150260);
//							lprintfln("Destroy1 %d", res);
//							if (/*toto != 0*/toto == 0) {
//								toto = 1;
//
//							maNotificationLocalDestroy(toto);
//							lprintfln("Destroy %d", toto);
//							}
//							else{
//								toto ++;
//								maNotificationAdd(NOTIFICATION_TYPE_APPLICATION_LAUNCHER, 150260, "test Notificvation", Convert::toString(toto).c_str());
							notificationHandle = maNotificationLocalCreate();
							lprintfln("%d", notificationHandle);
//							}
							res = maNotificationLocalSetProperty(notificationHandle, MA_NOTIFICATION_LOCAL_CONTENT_BODY, valueTmp->getValueForKey("content")->toString().c_str());
							lprintfln("%d", res);
//						notification->setContentBody(valueTmp->getValueForKey("content")->toString().c_str());//// test
//				lprintfln(valueTmp->getValueForKey("content")->toString().c_str());
						// Set some platform specific values.

						if (getPlatform() == ANDROID) {

//							notification->setVibrate(_vibrate);//// test

							if(_vibrate)
							{
								res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_VIBRATE,"true");lprintfln("%d", res);
								res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_VIBRATE_DURATION, "1000");lprintfln("%d", res);
//							notification->setVibrateDuration(1);//// test
							}
							else
							{
								res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_VIBRATE, "false");lprintfln("%d", res);
								res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_VIBRATE_DURATION, "0");lprintfln("%d", res);
//							notification->setVibrateDuration(0);//// test
							}
							// Set notification title and ticker.
							res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_FLAG, "MA_NOTIFICATION_FLAG_AUTO_CANCEL");lprintfln("%d", res);
//							lprintfln("notification->setFlag(NOTIFICATION_FLAG_AUTO_CANCEL) %d",notification->setFlag(NOTIFICATION_FLAG_AUTO_CANCEL));//// test
							res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_CONTENT_TITLE, "Nouvelle alerte");lprintfln("%d", res);
//							notification->setContentTitle("Nouvelle alerte");//// test
							res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_TICKER_TEXT, "Nouvelle alerte");lprintfln("%d", res);
//							notification->setTickerText("Nouvelle alerte");//// test
						} else {
							// Set a badge number.
							res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_BADGE_NUMBER, "6");lprintfln("%d", res);
//							notification->setBadgeNumber(6);//// test
							// Set the title of the action button or slider.
							res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_ALERT_ACTION, "Ok");lprintfln("%d", res);

//							notification->setAlertAction("ok");//// test
						}
						res = maNotificationLocalSetProperty(notificationHandle,MA_NOTIFICATION_LOCAL_PLAY_SOUND, "true");lprintfln("%d", res);
//						notification->setPlaySound(true);//// test

						res = maNotificationLocalSchedule(notificationHandle);lprintfln("%d", res);



//						NotificationManager::getInstance()->unscheduleLocalNotification(notification);
//						NotificationManager::getInstance()->scheduleLocalNotification(notification);//test
//						lprintfln("maNotificationLocalSchedule(notification->getHandle() %d" , maNotificationLocalSchedule(notification->getHandle()));
//						maNotificationLocalSchedule(notification->getHandle());
//						NotificationManager::getInstance()->scheduleLocalNotification(notification);

					}
					}
				}

				mapHLTA[idx] = new HorizontalLayout();
				mapLTAHeure[idx] = new Label((mapTrackingAlertDate[idx]));

				mapLTAHeure[idx]->setBackgroundColor(0x666666);
				MAExtent size = maGetScrSize();
				int mScreenWidth = EXTENT_X(size);
				mapLTAHeure[idx]->setWidth(mScreenWidth / 4);
				mapHLTA[idx]->addChild(mapLTAHeure[idx]);

				mapHLTA[idx]->addChild(mapLTADesc[idx]);
				mapLVITA[idx] = new ListViewItem();
				mapLVITA[idx]->addChild(mapHLTA[idx]);

				lValert->addChild(mapLVITA[idx]);
			}

			lastSendAlert = lastSendAlertTmp;
		}
//		else
//		{
//			Screen::setMainWidget(mainLayout);
//		}
	}
}

void TrackingTab::createUI() {
	mActivityPage = new ActivityPage();
	notificationHandle = 0;
//	notification = new LocalNotification();
	NotificationManager::getInstance()->addLocalNotificationListener(this);
	bCreateUI = true;
	String urlTmp = HOST;
	urlTmp += "/alerts/recipients/" + Convert::toString(_IDMOBILE)
			+ "/trackings/";
	urlTmp += _LOGINTOKEN;
	lprintfln(urlTmp.c_str());

	connectUrl(urlTmp, TRACKING_LIST);
	mainLayout = new Page(
			Convert::tr(traking_list_Label_title_alert + LANGUAGE));
	lListNoAlert = new Label(
			Convert::tr(traking_list_Label_title_no_alert + LANGUAGE));
	mainLayout->addChild(lListNoAlert);

	Screen::setMainWidget(mainLayout);

	lValert = new ListView();
//	lValert->wrapContentVertically();
	mainLayout->addChild(lValert);
}


void TrackingTab::orientationChange(int screenOrientation) {

	if (screenOrientation == MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) {
//		lprintfln("Orientation paysage");
		Screen::setTitle(Convert::tr(TRACKING_ALERT_TAB_EN + LANGUAGE));
	} else // Portrait
	{
//		lprintfln("Orientation Portrait");
		Screen::setTitle("");
//		Screen::setIcon(ICON_TRACKING);
	}
}
