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
	OptionTab::OptionTab(int language, String loginToken): Screen(), LANGUAGE(language), _LOGINTOKEN(loginToken)
	{
		setTitle(Convert::tr(OPTION_TAB_EN + LANGUAGE));
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

		vLOption = new VerticalLayout();
		vLOption->fillSpaceHorizontally();
		vLOption->fillSpaceVertically();

		lAuthenticationMode = new Label(
				Convert::tr(authentication_mode_page_title + LANGUAGE));
		vLOption->addChild(lAuthenticationMode);

		rGAuthenticationChoice = new RadioGroup();
		rGAuthenticationChoice->addRadioGroupListener(this);
		vLOption->addChild(rGAuthenticationChoice);

		rBModeCredential = new RadioButton();
		rBModeCredential->setText(
				Convert::tr(authentication_mode_credential + LANGUAGE));
		rGAuthenticationChoice->addView(rBModeCredential);
		rBModeNone = new RadioButton();
		rBModeNone->setText(Convert::tr(authentication_mode_none + LANGUAGE));
		rGAuthenticationChoice->addView(rBModeNone);
		if (_modeAuth == "none") {
			rGAuthenticationChoice->setChecked(rBModeNone);
		} else {
			rGAuthenticationChoice->setChecked(rBModeCredential);
		}
		rGAuthenticationChoice->fillSpaceVertically();

		Screen::setMainWidget(vLOption);
	}
}


void OptionTab::radioButtonSelected(NativeUI::RadioGroup* , int, NativeUI::RadioButton* rB)
{
	if(rB == rBModeCredential)
		{
			_modeAuth = "credential";
		}
		else if (rB == rBModeNone)
		{
			_modeAuth = "none";
		}
		tryToWrite(_login, _tokenMobile, _tokenConnection, _modeAuth , _idMobile);
}



//void OptionTab::orientationChange(bool landscape) {
//
//	if (landscape) {
//		lprintfln("Orientation paysage");
//		Screen::setTitle(Convert::tr(OPTION_TAB_EN + LANGUAGE));
//	} else // Portrait
//	{
//		lprintfln("Orientation Portrait");
//		Screen::setTitle("");
//		Screen::setIcon(LOGO4);
//	}
//}

//void OptionTab::orientationDidChange() {
//	int width = Screen::getWidth();
//	int height = Screen::getHeight();
//	lprintfln("width %d", width);
//	lprintfln("height %d", height);
//	if (width < height) {
//		lprintfln("Orientation paysage");
//		Screen::setTitle(Convert::tr(HOME_TAB_EN + LANGUAGE));
//	} else // Portrait
//	{
//		lprintfln("Orientation Portrait");
//		Screen::setTitle("");
//		Screen::setIcon(LOGO1);
//	}
//}
