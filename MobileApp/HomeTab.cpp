/*
 * HomeTab.cpp
 *
 *  Created on: 14 mai 2013
 *      Author: gdr
 */


#include "HomeTab.h"
//#include <MAUtil/Moblet.h>


	/**
	 * Constructor.
	 */
	HomeTab::HomeTab(int language, String loginToken, eScreenResolution screenResolution): Screen(), LANGUAGE(language), _LOGINTOKEN(loginToken)
	{

		setIcon(ICON_HOME + screenResolution);
			setBackgroundColor(255,255,255);
			createUI();
	}

	/**
	 * Destructor.
	 */
	HomeTab::~HomeTab()
	{

	}

	void HomeTab::dataDownloaded(MAHandle data , int result)
		{
			lprintfln("dataDownloaded HomeTab");

		//this->close();
		}

	void HomeTab::createUI()
	{
	}


	void HomeTab::orientationChange(int screenOrientation) {

		if (screenOrientation == MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) {
				lprintfln("Orientation paysage");
				Screen::setTitle(Convert::tr(HOME_TAB_EN + LANGUAGE));
			} else // Portrait
			{
				lprintfln("Orientation Portrait");
				Screen::setTitle("");
//				Screen::setIcon(ICON_HOME);
			}
	}
