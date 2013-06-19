/*
 * HomeTab.h
 *
 *  Created on: 14 mai 2013
 *      Author: gdr
 */

#ifndef HOMETAB_H_
#define HOMETAB_H_

#include <NativeUI/Screen.h>
#include <Wormhole/HighLevelHttpConnection.h>
#include <Wormhole/MessageStreamJSON.h>
#include <mastdlib.h>

#include "resource/Convert.h"
#include "MAHeaders.h"
#include "Util.h"

#define BUFSIZE 1024

using namespace Wormhole;
using namespace MAUtil;
using namespace NativeUI;


class HomeTab : public NativeUI::Screen, public HighLevelHttpConnection
{
public:
	/**
	 * Constructor.
	 */
	HomeTab(int language, String loginToken, eScreenResolution screenResolution);

	/**
	 * Destructor.
	 */
	virtual ~HomeTab();
	void dataDownloaded(MAHandle , int );
	void createUI();

	void orientationChange(int screenOrientation);

private:
	String _LOGINTOKEN;
	int LANGUAGE;

};


#endif /* HOMETAB_H_ */
