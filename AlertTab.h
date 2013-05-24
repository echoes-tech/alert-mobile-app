/*
 * AlertTab.h
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */

#ifndef ALERTTAB_H_
#define ALERTTAB_H_

//#include "HTTPConnect.h"
#include <NativeUI/Screen.h>
#include <MAUtil/Map.h>

#include <NativeUI/Widgets.h>// Include all widgets

#include <Wormhole/HighLevelHttpConnection.h>
#include <Wormhole/MessageStreamJSON.h>
#include <mastdlib.h>

#include "conf.h"
#include "resource/Convert.h"

#define BUFSIZE 1024
using namespace Wormhole;
using namespace MAUtil;
using namespace NativeUI;


class AlertTab : public NativeUI::Screen, public ButtonListener/*, public RadioGroupListener*/, public HighLevelHttpConnection, public ListViewListener
{
public:

	enum eAlertTab {
		PLUGIN_LIST = 0,
		INFORMATION_LIST,
		ALERT_LIST,
		ASSET_LIST,
		OPERATOR_LIST,
		UNIT_INFO,
		SUB_UNIT_INFO
	};

	/**
	 * Constructor.
	 */
	AlertTab();

	/**
	 * Destructor.
	 */
	virtual ~AlertTab();

	void connectUrl(String url, eAlertTab fct);
	void dataDownloaded(MAHandle , int );

	virtual void buttonClicked(Widget* button);
//	virtual void radioButtonSelected(NativeUI::RadioGroup*, int, NativeUI::RadioButton*);
	virtual void listViewItemClicked(ListView* listView, ListViewItem* listViewItem);

	void parseJSONPlugin(MAHandle data);
	void parseJSONInformation(MAHandle data);
	void parseJSONAlert(MAHandle data);
	void parseJSONAsset(MAHandle data);
	void parseJSONOperator(MAHandle data);
	void parseJSONUnitInfo(MAHandle data);
	void parseJSONSubUnitInfo(MAHandle data);
	void createUI();
	void createOptionPage();
	void createUnitPage();

private:

	eAlertTab fonction;
		char mBuffer[BUFSIZE];
		 bool mIsConnected;
		 int count;
		 String contentString;

		 int selectedPlugin;

		 int selectedInformation;

		 //View 1
		 ListView *lValert;
		 VerticalLayout* mainLayoutAlertChoice;
		 MAUtil::Map<int,ListViewItem*> mapLVIAlert;
		 MAUtil::Map<int,Label*> mapLAlertName;
		 Button *bAddAlert;
		 Button *bCancel;
		 HorizontalLayout *HLOptionAlert;
		 int posOptionAlert;

		 //View 2
		 Label* assetTitle;
		 ListView *lVAsset;
		 MAUtil::Map<int,long long> mapAssetId;
		 VerticalLayout* mainLayoutAssetChoice;
		 MAUtil::Map<int,ListViewItem*> mapLVIAsset;
		 MAUtil::Map<int,Label*> mapLAssetName;

		 //View 3
		 ListView *lVPlugin;
		 MAUtil::Map<int, long long> mapPluginId;
		 VerticalLayout* mainLayoutPluginChoice;
		 MAUtil::Map<int,ListViewItem*> mapLVIPlugin;
		 MAUtil::Map<int,Label*> mapLPluginName;

		 //View 4
		 ListView *lVInfo;
		 MAUtil::Map<int, long long> mapInfoIdSrc;
		 MAUtil::Map<int, long long> mapInfoIdSea;
		 MAUtil::Map<int, long long> mapInfoIdUnit;
		 VerticalLayout* mainLayoutInfoChoice;
		 MAUtil::Map<int,ListViewItem*> mapLVIInfo;
		 MAUtil::Map<int,Label*> mapLInfoName;

		 //View 5
		 VerticalLayout* mainLayoutOptionChoice;
		 ListView* lVOption;
		 MAUtil::Map<int,ListViewItem*> mapLVIOption;
		 Label* keyValue;
		 Label* criterion;
		 Label* value;
		 EditBox* eBValue;
		 Label* unit;
		 Button* bDest;

		 //view 5.2
		 ListView *lVOperator;
		 MAUtil::Map<int, long long> mapOperatorId;
		 VerticalLayout* mainLayoutOperatorChoice;
		 MAUtil::Map<int,ListViewItem*> mapLVIOperator;
	     MAUtil::Map<int,Label*> mapLOperatorName;

	     //view 5.3
	     ListView *lVUnit;
	     MAUtil::Map<int, long long> mapUnitId;
	     VerticalLayout* mainLayoutUnitChoice;
	     MAUtil::Map<int,ListViewItem*> mapLVIUnit;
	     MAUtil::Map<int,Label*> mapLUnitName;


//HTTPConnect* httpConnect;

};


#endif /* ALERTTAB_H_ */
