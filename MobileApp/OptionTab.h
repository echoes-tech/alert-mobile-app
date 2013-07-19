/*
 * OptionTab.h
 *
 *  Created on: 11 juin 2013
 *      Author: gdr
 */

#ifndef OPTIONTAB_H_
#define OPTIONTAB_H_

//#include <Wormhole/HighLevelHttpConnection.h>
//#include <Wormhole/MessageStreamJSON.h>
//#include <mastdlib.h>
//#include <NativeUI/Widgets.h>

#include "IncludeFile.h"
#include "resource/Convert.h"
#include "MAHeaders.h"
#include "Util.h"
#include "Page.h"

//#define BUFSIZE 1024

using namespace Wormhole;
//using namespace MAUtil;
using namespace NativeUI;


class OptionTab : public NativeUI::Screen, public HighLevelHttpConnection, public RadioGroupListener, public CheckBoxListener
{
public:
	/**
	 * Constructor.
	 */
	OptionTab(int language, String loginToken, eScreenResolution screenResolution);

	/**
	 * Destructor.
	 */
	virtual ~OptionTab();

	void dataDownloaded(MAHandle , int );
	virtual void radioButtonSelected(NativeUI::RadioGroup*, int, NativeUI::RadioButton*);
	virtual void checkBoxStateChanged(NativeUI::CheckBox*, bool);


	void orientationChange(int screenOrientation);

	void createUI();

private:
	String _LOGINTOKEN;
	int LANGUAGE;

//	int icreateUI;
	String _modeAuth;
	long long _idMobile;
	String _tokenConnection;
	String _tokenMobile;
	String _login;
	bool _vibrate;
	bool _notification;

	Page* vLOption;
//android
	Label* lAuthenticationMode;
	RadioGroup* rGAuthenticationChoice;
	RadioButton* rBModeCredential;
	RadioButton* rBModeNone;
//IOS
	CheckBox* cbAuthenticationChoice;

	Label* lNotificationOption;
	HorizontalLayout* hlSetNotification;
	Label* lSetNotification;
	CheckBox* cBSetNotification;
	HorizontalLayout* hlSetVibration;
	Label* lSetVibration;
	CheckBox* cBSetVibration;

};

class OptionStackScreen : public NativeUI::StackScreen, public NativeUI::StackScreenListener
{
public:
	OptionStackScreen(int language, String loginToken, eScreenResolution screenResolution):StackScreen(), LANGUAGE(language){
		addStackScreenListener(this);
		if(getPlatform() == IOS){
			Screen::setTitle(Convert::tr(OPTION_TAB_EN + LANGUAGE));
		}
		setPushTransition(MAW_TRANSITION_TYPE_NONE,0);
		setIcon(ICON_OPTION + screenResolution);
		mOptionTab = new OptionTab(language, loginToken, screenResolution);
		push(mOptionTab);
	};

	virtual void stackScreenScreenPopped( StackScreen* stackScreen, Screen* fromScreen, Screen* toScreen){
//		lprintfln("stackScreenScreenPopped");
//		mAlertTab->handleKeyPress(MAK_BACK);
//		push(mAlertTab);
//		mAlertTab->fillSpaceHorizontally();
//		mAlertTab->fillSpaceVertically();
//	};

//	void handleKeyPress(int keyCode){
//		mOptionTab->handleKeyPress(keyCode);
	};

	void orientationChange(int screenOrientation){
		if(getPlatform() != IOS){
			if (screenOrientation == MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) {
		//		lprintfln("Orientation paysage");
				Screen::setTitle(Convert::tr(OPTION_TAB_EN + LANGUAGE));
			} else // Portrait
			{
		//		lprintfln("Orientation Portrait");
				Screen::setTitle("");
			}
			mOptionTab->orientationChange(screenOrientation);
		}
	};

//	 virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData){
//		 lprintfln("widget event %d",  widgetEventData->eventType);
//
//		 if( widgetEventData->eventType == MAW_EVENT_STACK_SCREEN_POPPED)
//		 {
//			 mAlertTab->handleKeyPress(MAK_BACK);
//			 push(mAlertTab);
//			 mAlertTab->fillSpaceHorizontally();
//			 mAlertTab->fillSpaceVertically();
//		 }

//	 };
private:
	int LANGUAGE;
	OptionTab *mOptionTab;
};


#endif /* OPTIONTAB_H_ */
