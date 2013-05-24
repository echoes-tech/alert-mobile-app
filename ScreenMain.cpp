/*
 * ScreenMain.cpp
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */
#include "ScreenMain.h"



/**
 * Create the main screen with three tabs.
 */
ScreenMain* ScreenMain::createTabUI()
{

}

/**
 * Constructor.
 */
ScreenMain::ScreenMain() : TabScreen()
{

	// Create child screens.
	alertTab = new AlertTab();
	trackingTab = new TrackingTab();
	homeTab = new HomeTab();

	// Add them as tabs.
	this->addTab(homeTab);
	this->addTab(trackingTab);
	this->addTab(alertTab);
}

/**
 * Destructor.
 */
ScreenMain::~ScreenMain()
{
}


void ScreenMain::pullRequest(){trackingTab->runTimerEvent();}

/**
	 * Handle keypresses.
	 */
	void ScreenMain::handleKeyPress(int keyCode){lprintfln("test");}

	/**
	 * Handle pointer presses.
	 */
	void ScreenMain::handlePointerPressed(MAPoint2d point){}

	/**
	 * Handle pointer moves.
	 */
	void ScreenMain::handlePointerMoved(MAPoint2d point){}

	/**
	 * Handle pointer releases.
	 */
	void ScreenMain::handlePointerReleased(MAPoint2d point){}
