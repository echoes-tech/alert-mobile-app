/*
 * TrackingTab.cpp
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */

#include "TrackingTab.h"
#include <ma.h>
/**
 * Constructor.
 */
TrackingTab::TrackingTab(int language, String loginToken, eScreenResolution screenResolution, long long idMobile) :
		Screen(), LANGUAGE(language), _LOGINTOKEN(loginToken), _IDMOBILE(idMobile) {

	// Set title and icon of the stack screen.
	setIcon(ICON_TRACKING + screenResolution);
//	setTitle(Convert::tr(TRACKING_ALERT_TAB_EN + LANGUAGE));
//			setBackgroundColor(255,255,255);
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
	urlTmp += "/alerts/recipients/" + Convert::toString(_IDMOBILE) + "/trackings/";
	//		urlTmp += "/plugins/1/informations";
	urlTmp += _LOGINTOKEN;
	lprintfln(urlTmp.c_str());
	connectUrl(urlTmp, TRACKING_LIST);

	lprintfln("timer");
}

void TrackingTab::dataDownloaded(MAHandle data, int result) {
	lprintfln("dataDownloaded trackingTab");
	mIsConnected = false;
	this->close();
	if (result == RES_OK) {
		connERR = 0;
		char * jsonData = new char[maGetDataSize(data) + 1];
								maReadData(data, jsonData, 0, maGetDataSize(data));
				String jsonTmp = jsonData;
				Convert::formatJSONBeforeParse(jsonTmp);

				MAUtil::YAJLDom::Value* root = YAJLDom::parse((const unsigned char*) jsonTmp.c_str(), maGetDataSize(data));


		parseJSONTrackingAlert(root);
		delete jsonData;
		delete root;
		//this->close();
	} else if (result == CONNERR_DNS) {
		connERR++;
		lprintfln("AlertTab DataDownload result = %d", result);
		lprintfln("DNS resolution error.");
	}  else if (result != 404) {
		connERR++;
		lprintfln("TrackingTab DataDownload result = %d", result);
	}
	if(connERR >= 3)
	{
		getSystemConnection();
		String sMessage = "Connection ERROR. ERREUR:";
		sMessage += Convert::toString(result);
		maMessageBox("Connection Error", sMessage.c_str() );
	}
}

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

	STime lastSendAlertTmp;
	if(root->getNumChildValues() > 0)
	{
		MAUtil::YAJLDom::Value* valueTmp0 = root->getValueByIndex(0);
		String dateTmp = valueTmp0->getValueForKey("send_date")->toString();
		lastSendAlertTmp = Convert::toSTime(dateTmp);
	}


	if (lastSendAlertTmp > lastSendAlert) {
//		Screen::setMainWidget(new ActivityIndicator());
		Screen::removeChild(mainLayout);

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

	for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
		lTrackingTitle->setText(Convert::tr(traking_list_Label_title_alert + LANGUAGE));
		mapTrackingAlertDate[idx] =
				valueTmp->getValueForKey("send_date")->toString();
		lprintfln(mapTrackingAlertDate[idx].c_str());
		String convert = valueTmp->getValueForKey("content")->toString();
		Convert::HTMLdecode(convert);

		mapLTADesc[idx] = new Label(convert);
		mapLTADesc[idx]->wrapContentHorizontally ();
		mapLTADesc[idx]->wrapContentVertically ();
//		mapLTADesc[idx]->setMaxNumberOfLines(10);
//		mapLTADesc[idx] = new MAUI::Label();
//		mapLTADesc[idx]->setEnabled(false);
//		mapLTADesc[idx]->setText(convert);

		if(!bCreateUI)
		{
			String dateTmp1 = valueTmp->getValueForKey("send_date")->toString();
			STime lastSendAlertTmp1 = Convert::toSTime(dateTmp1);
//			lprintfln("%d %d %d %d %d %d", lastSendAlertTmp1.year,lastSendAlertTmp1.mon,lastSendAlertTmp1.day,lastSendAlertTmp1.hour,lastSendAlertTmp1.min,lastSendAlertTmp1.sec);
			if(lastSendAlertTmp1>lastSendAlert)
			{
//				lprintfln("%d %d %d %d %d %d", lastSendAlertTmp1.year,lastSendAlertTmp1.mon,lastSendAlertTmp1.day,lastSendAlertTmp1.hour,lastSendAlertTmp1.min,lastSendAlertTmp1.sec);
//				lprintfln("%d %d %d %d %d %d", lastSendAlert.year,lastSendAlert.mon,lastSendAlert.day,lastSendAlert.hour,lastSendAlert.min,lastSendAlert.sec);
				notification = new LocalNotification();
				notification->setContentBody(mapLTADesc[idx]->getText());
//				lprintfln(valueTmp->getValueForKey("content")->toString().c_str());
					// Set some platform specific values.

					if (getPlatform() == ANDROID) {
						// Set the vibration duration to 5seconds when an alert is displayed.
						notification->setVibrate(true);
						notification->setVibrateDuration(1);
						notification->setPlaySound(true);
						// Set notification title and ticker.
	//				    notification->setContentTitle("My message title");
						notification->setTickerText("Nouvelle alerte");
					} else {
						// Set a badge number.
						notification->setBadgeNumber(6);
						// Set the title of the action button or slider.
						notification->setAlertAction("ok");
					}
					NotificationManager::getInstance()->unscheduleLocalNotification(notification);
					NotificationManager::getInstance()->scheduleLocalNotification(notification);
			}
		}

			mapHLTA[idx] = new HorizontalLayout();
			mapLTAHeure[idx] = new Label((mapTrackingAlertDate[idx]));
			mapLTAHeure[idx]->setFontSize(18);

			mapLTAHeure[idx]->setBackgroundColor(0x666666);
			mapLTAHeure[idx]->setWidth(110);
			mapHLTA[idx]->addChild(mapLTAHeure[idx]);

			mapHLTA[idx]->addChild(mapLTADesc[idx]);
			mapLVITA[idx] = new ListViewItem();
			mapLVITA[idx]->addChild(mapHLTA[idx]);

			lValert->addChild(mapLVITA[idx]);
		}

		lastSendAlert = lastSendAlertTmp;
		if (bCreateUI) {
			bCreateUI = false;
		}
		Screen::setMainWidget(mainLayout);
	}
	}
}

void TrackingTab::createUI() {
	bCreateUI = true;
	String urlTmp = HOST;
	urlTmp += "/alerts/recipients/" + Convert::toString(_IDMOBILE) + "/trackings/";
	urlTmp += _LOGINTOKEN;
	lprintfln(urlTmp.c_str());

	connectUrl(urlTmp, TRACKING_LIST);
	mainLayout = new VerticalLayout();
	// Make the layout fill the entire screen.
	mainLayout->fillSpaceHorizontally();
	mainLayout->fillSpaceVertically();
	lTrackingTitle = new Label(Convert::tr(traking_list_Label_title_no_alert + LANGUAGE));
	lTrackingTitle->fillSpaceHorizontally();
	mainLayout->addChild(lTrackingTitle);
	// Add the layout as the root of the screen's widget tree.
//	Screen::setMainWidget(new ActivityIndicator());
		Screen::setMainWidget(mainLayout);
//	VerticalLayout* vl = new VerticalLayout();
//
//	        vl->addChild(new ActivityIndicator());
//
//
//	        vl->setChildHorizontalAlignment("center");
//	        vl->setChildVerticalAlignment("center");

	lValert = new ListView();
	mainLayout->addChild(lValert);
}

void TrackingTab::connectUrl(String url, eTrakingTab fct) {
	//verifie si on est connecté
//		MACellInfo ci;
//		int res;
//		res = maGetCellInfo(&ci);
//		lprintfln("maGetCellInfo: %i\n", res);
//		lprintfln("MCC: %s\n", ci.mcc);
//		lprintfln("MNC: %s\n", ci.mnc);
//		lprintfln("LAC: %i\n", ci.lac);
//		lprintfln("CellId: %i\n", ci.cellId);
//		lprintfln("connecter ou pas ? %d ", maNetworkStatus());
	lprintfln("connectUrl");
	if (mIsConnected == false) {
		mIsConnected = true;
		fonction = fct;
		lprintfln("GET REQUEST : %d", this->get(url.c_str()));
	} else {
		lprintfln("Déjà connecté: %d", fonction);
	}
}


void TrackingTab::orientationChange(int screenOrientation) {

	if (screenOrientation == MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) {
		lprintfln("Orientation paysage");
		Screen::setTitle(Convert::tr(TRACKING_ALERT_TAB_EN + LANGUAGE));
	} else // Portrait
	{
		lprintfln("Orientation Portrait");
		Screen::setTitle("");
//		Screen::setIcon(ICON_TRACKING);
	}
}
