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
	landscape = false;
	alertTab = new AlertTab(LANGUAGE, loginToken);
	trackingTab = new TrackingTab(LANGUAGE, loginToken, idMobile);
//	homeTab = new HomeTab(LANGUAGE, loginToken);
	optionTab = new OptionTab(LANGUAGE, loginToken);

//	 Add them as tabs.
//	this->addTab(homeTab); //tab index 0;
	this->addTab(trackingTab); //tab index 1;
	this->addTab(alertTab); //tab index 2;
	this->addTab(optionTab); //tab index 3;

	Environment::getEnvironment().addCustomEventListener(this);

//	alertTab->addEventListener(this);
//	trackingTab->addEventListener(this);
//	homeTab->addEventListener(this);
//	optionTab->addEventListener(this);

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

//void ScreenMain::drawChangeVerticalHorizontal(int width, int height)
//{lprintfln("drawChangeVerticalHorizontal");
//	if (this->getActiveTabIndex() >= 0)
//	{
//		lprintfln("drawChangeVerticalHorizontal");
//	if (width > height) // Landscape
//				{
//		lprintfln("drawChangeVerticalHorizontal Landscape");
//		homeTab->setTitle(Convert::tr(HOME_TAB_EN + LANGUAGE));
//		} else // Portrait
//		{
//			lprintfln("drawChangeVerticalHorizontal Portrait");
//			homeTab->setTitle("");
//			homeTab->setIcon(LOGO);
//		}
//	}
////	alertTab->drawChangeVerticalHorizontal(width, height);
////	trackingTab->drawChangeVerticalHorizontal(width, height);
////	homeTab->drawChangeVerticalHorizontal(width, height);
//}

