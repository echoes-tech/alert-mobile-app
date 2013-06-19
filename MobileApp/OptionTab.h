/*
 * OptionTab.h
 *
 *  Created on: 11 juin 2013
 *      Author: gdr
 */

#ifndef OPTIONTAB_H_
#define OPTIONTAB_H_

#include <NativeUI/Screen.h>
#include <Wormhole/HighLevelHttpConnection.h>
#include <Wormhole/MessageStreamJSON.h>
#include <mastdlib.h>
#include <NativeUI/Widgets.h>

#include "resource/Convert.h"
#include "MAHeaders.h"
#include "Util.h"

#define BUFSIZE 1024

using namespace Wormhole;
using namespace MAUtil;
using namespace NativeUI;


class OptionTab : public NativeUI::Screen, public HighLevelHttpConnection, public RadioGroupListener
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

	void orientationChange(int screenOrientation);

	void createUI();

private:
	String _LOGINTOKEN;
	int LANGUAGE;

	String _modeAuth;
	long long _idMobile;
	String _tokenConnection;
	String _tokenMobile;
	String _login;


	VerticalLayout* vLOption;

	Label* lAuthenticationMode;
	RadioGroup* rGAuthenticationChoice;
	RadioButton* rBModeCredential;
	RadioButton* rBModeNone;

};


#endif /* OPTIONTAB_H_ */
