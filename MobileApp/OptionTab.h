/*
 * OptionTab.h
 *
 *  Created on: 11 juin 2013
 *      Author: gdr
 */

#ifndef OPTIONTAB_H_
#define OPTIONTAB_H_

#include <Wormhole/HighLevelHttpConnection.h>
#include <Wormhole/MessageStreamJSON.h>
#include <mastdlib.h>
#include <NativeUI/Widgets.h>

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

	int icreateUI;
	String _modeAuth;
	long long _idMobile;
	String _tokenConnection;
	String _tokenMobile;
	String _login;
	bool _vibrate;
	bool _notification;

	Page* vLOption;

	Label* lAuthenticationMode;
	RadioGroup* rGAuthenticationChoice;
	RadioButton* rBModeCredential;
	RadioButton* rBModeNone;

	Label* lNotificationOption;
	HorizontalLayout* hlSetNotification;
	Label* lSetNotification;
	CheckBox* cBSetNotification;
	HorizontalLayout* hlSetVibration;
	Label* lSetVibration;
	CheckBox* cBSetVibration;

};


#endif /* OPTIONTAB_H_ */
