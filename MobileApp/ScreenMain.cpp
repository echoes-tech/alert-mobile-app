/*
 * ScreenMain.cpp
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */
#include "ScreenMain.h"

/**
 * Constructor.
 */
ScreenMain::ScreenMain() :
		TabScreen() {

	uiCreated = false;
	LANGUAGE = getSystemLanguage();
	lprintfln("test mainscreen %s", getLocalPath().c_str());

	//Rappel CreateUI() est appelé dans l'objet Authentication. lorsque l'utilisateur est bien authentifié.

	// Create child screens.

}

/**
 * Destructor.
 */
ScreenMain::~ScreenMain() {
}

void ScreenMain::createUI(String loginToken, long long idMobile) {
	maScreenSetOrientation(MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN);
	MAExtent size = maGetScrSize();
	int mScreenWidth = EXTENT_X(size);
	int mScreenHeight = EXTENT_Y(size);
	eScreenResolution screenResolution = LARGE_SCREEN_RESOLUTION;
	lprintfln("resolution x %d, y %d",mScreenWidth , mScreenHeight);
	if (mScreenWidth <= SMALL_RESOLUTION) {
		screenResolution = SMALL_SCREEN_RESOLUTION;
		lprintfln("resolution small");
	}


	alertTab = new AlertTab(LANGUAGE, loginToken, screenResolution);
	trackingTab = new TrackingTab(LANGUAGE, loginToken, screenResolution, idMobile);
//	homeTab = new HomeTab(LANGUAGE, loginToken, screenResolution);
	optionTab = new OptionTab(LANGUAGE, loginToken, screenResolution);

//	 Add them as tabs.
//	this->addTab(homeTab); //tab index 0;
	this->addTab(trackingTab); //tab index 0;
	this->addTab(alertTab); //tab index 1;
	this->addTab(optionTab); //tab index 2;

	alertTab->addScreenListener(this);
	trackingTab->addScreenListener(this);
//	homeTab->addScreenListener(this);
	optionTab->addScreenListener(this);

	this->show();
	uiCreated = true;
	// iOS and Windows Phone.
	maScreenSetSupportedOrientations(
			MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT
					| MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT
					| MA_SCREEN_ORIENTATION_PORTRAIT
					| MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN);
}

void ScreenMain::pullRequest() {
	if (uiCreated) {
		trackingTab->runTimerEvent();
	}
}

/**
 * Handle keypresses.
 */
void ScreenMain::handleKeyPress(int keyCode) {
	lprintfln("Tab Active : %d", this->getActiveTabIndex());
	maAlertPrintFirstTime = true;
	if (keyCode == MAK_BACK) {
		if (this->getActiveTabIndex() == 1) {
			alertTab->handleKeyPress(keyCode);
		} else {
			maAlert("", "",
					Convert::tr(Screen_Main_Button_close_app + LANGUAGE),
					Convert::tr(Screen_Main_Button_home + LANGUAGE),
					Convert::tr(Screen_Main_Button_cancel + LANGUAGE));
		}
	}
	else if(keyCode == MAK_MENU || keyCode == MAK_HOME)
	{
		maAlert("", "", Convert::tr(Screen_Main_Button_close_app + LANGUAGE),
				Convert::tr(Screen_Main_Button_home + LANGUAGE),
				Convert::tr(Screen_Main_Button_cancel + LANGUAGE));
	}
}

/**
 * Called for events that have no predefined
 * event handling method.
 */
void ScreenMain::customEvent(const MAEvent& event) {
	if (EVENT_TYPE_ALERT == event.type) {
		if (1 == event.alertButtonIndex) {
			maPanic(1, Convert::tr(Screen_Main_close_app + LANGUAGE));
		} else if (2 == event.alertButtonIndex) {

			maSendToBackground();
		} else if (3 == event.alertButtonIndex) {
		}
	} else if (EVENT_TYPE_ALERT_DISMISSED == event.type
			&& maAlertPrintFirstTime) { //lorsqu'on appuit sur le boutton BACK trop longtemps cet event est envoyer et supprime le choix.
		maAlertPrintFirstTime = false;
		maAlert("", "", Convert::tr(Screen_Main_Button_close_app + LANGUAGE),
				Convert::tr(Screen_Main_Button_home + LANGUAGE),
				Convert::tr(Screen_Main_Button_cancel + LANGUAGE));
	}
}

/**
 * Handle pointer presses.
 */
void ScreenMain::handlePointerPressed(MAPoint2d point) {
//	lprintfln("test mainScreen 2");
}

/**
 * Handle pointer moves.
 */
void ScreenMain::handlePointerMoved(MAPoint2d point) {
//	lprintfln("test mainScreen 3");
}

/**
 * Handle pointer releases.
 */
void ScreenMain::handlePointerReleased(MAPoint2d point) {
//	lprintfln("test mainScreen 4");
}


void ScreenMain::orientationChanged(Screen* screen, int screenOrientation)
{

	lprintfln("screenOrientation : %d", screenOrientation);
	if (screen == alertTab) {
//		lprintfln("screenOrientation 1");
		alertTab->orientationChange(screenOrientation);
//		lprintfln("screenOrientation 1");
	} else if (screen == optionTab) {
//		lprintfln("screenOrientation 2");
		optionTab->orientationChange(screenOrientation);
//		lprintfln("screenOrientation 2");
	} else if (screen == trackingTab) {
//		lprintfln("screenOrientation 3");
		trackingTab->orientationChange(screenOrientation);
//		lprintfln("screenOrientation 3");
	}
//	else if (screen == homeTab) {
//		homeTab->orientationChange(screenOrientation);
//		}
}

