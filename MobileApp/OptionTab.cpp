/*
 * OptionTab.cpp
 *
 *  Created on: 11 juin 2013
 *      Author: gdr
 */

#include "OptionTab.h"
//#include <MAUtil/Moblet.h>


	/**
	 * Constructor.
	 */
	OptionTab::OptionTab(int language, String loginToken, eScreenResolution screenResolution): Screen(), LANGUAGE(language), _LOGINTOKEN(loginToken)
	{
		if(getPlatform() == IOS){
					Screen::setTitle(Convert::tr(OPTION_TAB_EN + LANGUAGE));
				}
		setIcon(ICON_OPTION + screenResolution);
//		setTitle(Convert::tr(OPTION_TAB_EN + LANGUAGE));
		createUI();
	}

	/**
	 * Destructor.
	 */
	OptionTab::~OptionTab()
	{

	}

	void OptionTab::dataDownloaded(MAHandle data , int result)
		{
			lprintfln("dataDownloaded HomeTab");

		//this->close();
		}

void OptionTab::createUI() {
//	icreateUI = 0;
	String config;
	eFile eFileTmp = tryToRead(config);
	if (eFileTmp == FILE_NOT_EXIST) {
		maMessageBox("ERROR FILE STRORAGE", "Can not read config file.");
	} else if (eFileTmp == FILE_OPEN_ERROR) {
		maMessageBox("ERROR FILE STRORAGE", "Can not read config file");
	} else {
		Convert::formatJSONBeforeParse(config);
		MAUtil::YAJLDom::Value* root = YAJLDom::parse(
				(const unsigned char*) config.c_str(), config.size());
		_modeAuth = root->getValueForKey("authentication_mode")->toString();
		_idMobile = root->getValueForKey("id_media_value")->toInt();
		_tokenConnection = root->getValueForKey("token_authent")->toString();
		_tokenMobile = root->getValueForKey("token_mobile")->toString();
		_login = root->getValueForKey("login")->toString();
		_vibrate = root->getValueForKey("vibrate")->toBoolean();
		_notification = root->getValueForKey("notification")->toBoolean();

		vLOption = new Page("Gestion du compte");

		lAuthenticationMode = new Label(
				Convert::tr(authentication_mode_page_title + LANGUAGE));
		vLOption->addChild(lAuthenticationMode);



		if(getPlatform() != IOS){
			rGAuthenticationChoice = new RadioGroup();

					vLOption->addChild(rGAuthenticationChoice);
		rBModeCredential = new RadioButton();
//		HorizontalLayout *hlCredential = new HorizontalLayout();
//		Label *lCredential = new Label(Convert::tr(authentication_mode_credential + LANGUAGE));
//		hlCredential->addChild(rBModeCredential);
		rBModeCredential->setText(Convert::tr(authentication_mode_credential + LANGUAGE));
		rBModeCredential->setTextColor(0xC0C0C0);
		rGAuthenticationChoice->addView(rBModeCredential);
		rBModeNone = new RadioButton();
		rBModeNone->setText(Convert::tr(authentication_mode_none + LANGUAGE));
		rBModeNone->setTextColor(0xC0C0C0);
		rGAuthenticationChoice->addView(rBModeNone);
		if (_modeAuth == "none") {
			rGAuthenticationChoice->setChecked(rBModeNone);
		} else {
			rGAuthenticationChoice->setChecked(rBModeCredential);
		}
		rGAuthenticationChoice->addRadioGroupListener(this);
//		rGAuthenticationChoice->fillSpaceVertically();
	}
		else
				{
					HorizontalLayout *hLIOSModeAuthent = new HorizontalLayout();
					hLIOSModeAuthent->wrapContentVertically();
					Label *lIOSModeAuthent = new Label(Convert::tr(authentication_mode_none + LANGUAGE));
					cbAuthenticationChoice = new CheckBox();
					hLIOSModeAuthent->addChild(cbAuthenticationChoice);
					hLIOSModeAuthent->addChild(lIOSModeAuthent);
					vLOption->addChild(hLIOSModeAuthent);
					if(_modeAuth == "none"){
						cbAuthenticationChoice->setState(true);
					}else{
						cbAuthenticationChoice->setState(false);
					}
					cbAuthenticationChoice->addCheckBoxListener(this);

				}

		vLOption->setBreakLine();

		lNotificationOption = new Label(Convert::tr(alert_notification_option_sub_title + LANGUAGE));
		vLOption->addChild(lNotificationOption);

		hlSetNotification = new HorizontalLayout();
		hlSetNotification->wrapContentVertically();
		lSetNotification = new Label(Convert::tr(alert_notification_display + LANGUAGE));
		cBSetNotification = new CheckBox();
		cBSetNotification->setState(_notification);
		cBSetNotification->addCheckBoxListener(this);
		hlSetNotification->addChild(cBSetNotification);
		hlSetNotification->addChild(lSetNotification);

		vLOption->addChild(hlSetNotification);

		hlSetVibration = new HorizontalLayout();
		hlSetVibration->wrapContentVertically();
		lSetVibration = new Label(Convert::tr(alert_notification_option_vibrate + LANGUAGE));
		cBSetVibration = new CheckBox();
		cBSetVibration->setState(_vibrate);
		cBSetVibration->addCheckBoxListener(this);
		hlSetVibration->addChild(cBSetVibration);
		hlSetVibration->addChild(lSetVibration);
		vLOption->addChild(hlSetVibration);

		vLOption->setBreakLine();


		Screen::setMainWidget(vLOption);
	}
}


void OptionTab::radioButtonSelected(NativeUI::RadioGroup*, int,
		NativeUI::RadioButton* rB) {
	if(getPlatform() != IOS){
	lprintfln("radioButtonSelected");
	if (rB == rBModeCredential) {
		_modeAuth = "credential";
	} else if (rB == rBModeNone) {
		_modeAuth = "none";
	}

//	if (icreateUI > 1) {
		if (tryToWrite(_login, _tokenMobile, _tokenConnection, _modeAuth,_idMobile, _vibrate, _notification) != FILE_CLOSE) {
			maToast(Convert::tr(alert_change_mode_auth_no_saved + LANGUAGE),MA_TOAST_DURATION_SHORT);
		}
//	} else {
//		icreateUI++;
//	}
	}
}

void OptionTab::checkBoxStateChanged(NativeUI::CheckBox* cB, bool b)
{

	lprintfln("bool %s",b);
	if(cB == cBSetNotification)
	{
		_notification = cBSetNotification->isChecked();
		if(_notification)
		{
		lprintfln("_notification %s", _notification);
		}
	}
	else if (cB == cBSetVibration)
	{
		_vibrate = cBSetVibration->isChecked();
		if(_vibrate)
				{
				lprintfln("_vibrate %s", _vibrate);
				}
	}
	if(getPlatform() == IOS){
		if(cB == cbAuthenticationChoice)
		{
				bool modeAuth = cbAuthenticationChoice->isChecked();
				if(modeAuth)
				{
					_modeAuth = "none";

				}else{
					_modeAuth = "credential";
				}
				lprintfln("_modeAuth %s", _modeAuth.c_str());
		}
	}
//	tryToWrite(_login, _tokenMobile, _tokenConnection, _modeAuth,_idMobile, _vibrate, _notification);
	if (tryToWrite(_login, _tokenMobile, _tokenConnection, _modeAuth,_idMobile, _vibrate, _notification) != FILE_CLOSE) {
				maToast(Convert::tr(alert_change_mode_auth_no_saved + LANGUAGE),MA_TOAST_DURATION_SHORT);
			}
//	if (icreateUI > 1) {
//			if (tryToWrite(_login, _tokenMobile, _tokenConnection, _modeAuth,_idMobile, _vibrate, _notification) == FILE_CLOSE) {
//				maToast(Convert::tr(alert_change_mode_auth_saved + LANGUAGE),MA_TOAST_DURATION_SHORT);
//			}
//	}
}

void OptionTab::orientationChange(int screenOrientation) {

	if (screenOrientation == MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) {
//		lprintfln("Orientation paysage");
		Screen::setTitle(Convert::tr(OPTION_TAB_EN + LANGUAGE));
	} else // Portrait
	{
//		lprintfln("Orientation Portrait");
		Screen::setTitle("");
//		Screen::setIcon(ICON_OPTION);
	}
}
