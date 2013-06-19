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

	// Create child screens.

}

/**
 * Destructor.
 */
ScreenMain::~ScreenMain() {
}

void ScreenMain::createUI(String loginToken, long long idMobile) {
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
	this->addTab(alertTab); //tab index 0;
	this->addTab(trackingTab); //tab index 1;

	this->addTab(optionTab); //tab index 2;

	alertTab->addScreenListener(this);
	trackingTab->addScreenListener(this);
//	homeTab->addScreenListener(this);
	optionTab->addScreenListener(this);

////	maScreenSetOrientation(MA_SCREEN_ORIENTATION_DYNAMIC);
//
//	// iOS and Windows Phone.
//	maScreenSetSupportedOrientations(
//			MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT
//					| MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT
//					| MA_SCREEN_ORIENTATION_PORTRAIT
//					| MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN);

	this->show();
	uiCreated = true;
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
	lprintfln("test");
	lprintfln("Tab Active : %d", this->getActiveTabIndex());
	maAlertPrintFirstTime = true;
	if (keyCode == MAK_BACK) {
		if (this->getActiveTabIndex() == 0) {
			alertTab->handleKeyPress(keyCode);
		} else {
			maAlert("", "",
					Convert::tr(Screen_Main_Button_close_app + LANGUAGE),
					Convert::tr(Screen_Main_Button_home + LANGUAGE),
					Convert::tr(Screen_Main_Button_cancel + LANGUAGE));
		}
	}
}

/**
 * Called for events that have no predefined
 * event handling method.
 */
void ScreenMain::customEvent(const MAEvent& event) {
	lprintfln("TTTTTTTT test event1 %d", event.type);
	if (EVENT_TYPE_ALERT == event.type) {
		if (1 == event.alertButtonIndex) {
			maPanic(1, Convert::tr(Screen_Main_close_app + LANGUAGE));
		} else if (2 == event.alertButtonIndex) {

			maSendToBackground();
		} else if (3 == event.alertButtonIndex) {
		}
	}
	else if (EVENT_TYPE_ALERT_DISMISSED == event.type && maAlertPrintFirstTime){ //lorsque l'on appuit sur le boutton BACK trop longtemps cet event est envoyer et supprime le chois.
		maAlertPrintFirstTime = false;
		maAlert("", "",Convert::tr(Screen_Main_Button_close_app + LANGUAGE),
									Convert::tr(Screen_Main_Button_home + LANGUAGE),
									Convert::tr(Screen_Main_Button_cancel + LANGUAGE));
			}
//	else if (event.type == 28) {
//		if (uiCreated) {
//			lprintfln("ORIENTATION CHANGE");
//			int width = Screen::getWidth();
//			int height = Screen::getHeight();
//			lprintfln("width %d", width);
//			lprintfln("height %d", height);
//			bool landscapeTmp;
//			if (width > height) {
//				landscapeTmp = true;
//
//			} else {
//				landscapeTmp = false;
//			}
//			if (landscapeTmp != landscape) {
//				landscape = landscapeTmp;
////				alertTab->orientationChange(landscape);
////				trackingTab->orientationChange(landscape);
//				homeTab->orientationDidChange();
////				optionTab->orientationChange(landscape);
//			}
//		}
//	}
}

/**
 * Handle pointer presses.
 */
void ScreenMain::handlePointerPressed(MAPoint2d point) {
	lprintfln("test mainScreen 2");
}

/**
 * Handle pointer moves.
 */
void ScreenMain::handlePointerMoved(MAPoint2d point) {
	lprintfln("test mainScreen 3");
}

/**
 * Handle pointer releases.
 */
void ScreenMain::handlePointerReleased(MAPoint2d point) {
	lprintfln("test mainScreen 4");
}


void ScreenMain::orientationChanged(Screen* screen, int screenOrientation)
{
	lprintfln("screenOrientation : %d", screenOrientation);
	if (screen == alertTab) {
		alertTab->orientationChange(screenOrientation);
	} else if (screen == optionTab) {
		optionTab->orientationChange(screenOrientation);
	} else if (screen == trackingTab) {
		trackingTab->orientationChange(screenOrientation);
	}
//	else if (screen == homeTab) {
//		homeTab->orientationChange(screenOrientation);
//		}
}

