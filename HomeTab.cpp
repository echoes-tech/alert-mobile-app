/*
 * HomeTab.cpp
 *
 *  Created on: 14 mai 2013
 *      Author: gdr
 */




#include "HomeTab.h"

	/**
	 * Constructor.
	 */
	HomeTab::HomeTab(): Screen()
	{
		// Set title and icon of the stack screen.
			setTitle("Home");
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
