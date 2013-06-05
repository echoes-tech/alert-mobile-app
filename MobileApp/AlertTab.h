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

#include "Util.h"
#include "conf.h"
#include "resource/Convert.h"
#include "MAHeaders.h"

#define BUFSIZE 1024
using namespace Wormhole;
using namespace MAUtil;
using namespace NativeUI;


class AlertTab : public NativeUI::Screen, public ButtonListener, public HighLevelHttpConnection, public ListViewListener, public EditBoxListener
{
public:

	enum eAlertTab {
		PLUGIN_LIST = 0,
		INFORMATION_LIST,
		ALERT_LIST,
		ASSET_LIST,
		OPERATOR_LIST,
		UNIT_INFO,
		SUB_UNIT_INFO,
		USER_LIST,
		MEDIA_VALUE_LIST,
		MEDIA_TYPE,
		POST_MEDIA_SPE,
		POST_ALERT,
		SEARCH_INFO,
		ALERT_INFO
	};

	/**
	 * Constructor.
	 */
	AlertTab(int language, String loginToken);

	/**
	 * Destructor.
	 */
	virtual ~AlertTab();

	void handleKeyPress(int keyCode);

	void connectUrl(String url, eAlertTab fct, int verb = GET, String jsonMessage = "");
	void dataDownloaded(MAHandle , int );

	virtual void buttonClicked(Widget* button);
//	virtual void radioButtonSelected(NativeUI::RadioGroup*, int, NativeUI::RadioButton*);
	virtual void listViewItemClicked(ListView* listView, ListViewItem* listViewItem);
	virtual void editBoxReturn(EditBox* editBox){};

	void parseJSONPlugin(MAUtil::YAJLDom::Value* root);
	void parseJSONInformation(MAUtil::YAJLDom::Value* root);
	void parseJSONAlert(MAUtil::YAJLDom::Value* root);
	void parseJSONAsset(MAUtil::YAJLDom::Value* root);
	void parseJSONOperator(MAUtil::YAJLDom::Value* root);
	void parseJSONUnitInfo(MAUtil::YAJLDom::Value* root);
	void parseJSONSubUnitInfo(MAUtil::YAJLDom::Value* root);
	void parseJSONUserForOrganisation(MAUtil::YAJLDom::Value* root);
	void parseJSONUserInfo(MAUtil::YAJLDom::Value* root);
	void parseJSONMediaType(MAUtil::YAJLDom::Value* root);
	void parseJSONMediaValue(MAUtil::YAJLDom::Value* root);
	void parseJSONPostMediaSpe(MAUtil::YAJLDom::Value* root);
	void parseJSONPostAlert(MAUtil::YAJLDom::Value* root);
	void parseJSONSearchInfo(MAUtil::YAJLDom::Value* root);
	void parseJSONAlertInfo(MAUtil::YAJLDom::Value* root);

	void createUI();
	void createOptionPage();
	void createUnitPage();
	void createSnoozePage();
	void createDestListPage();
	void createDetailAlertPage();

	bool optionPageValid();
	bool snoozePageValid();

//	void drawChangeVerticalHorizontal(int width, int height);
private:
	String _LOGINTOKEN;
	int LANGUAGE;
	int connERR;

	eAlertTab fonction;
	char mBuffer[BUFSIZE];
	bool mIsConnected;
	int count;
	String contentString;
	VerticalLayout* activeMainLayout;

	int selectedAsset;
	int selectedPlugin;
	int selectedInformation;
	int selectedUnit;
	int selectedOperator;
	int selectedUser;
	int selectedMedia;
	int selectedMediaValue;

	//View 1
	ListView *lValert;
	VerticalLayout* mainLayoutAlertChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIAlert;
	MAUtil::Map<int, Label*> mapLAlertName;
	MAUtil::Map<int, long long> mapAlertId;
	Button *bAddAlert;
	Button *bDelete;
	Button *bCancel;
	Button *bView;
	HorizontalLayout *HLOptionAlert;
	int posOptionAlert;

	//View 1.1
	VerticalLayout* mainLayoutAlertDetailChoice;
	Label* lAlertName;
	Label* lAlertOperator;
	Label* lAlertValue;
	Label* lAlertUnit;
	Label* lAlertKeyValue;
	Label* lAlertMedia;
	Label* lAlertSnooze;

	//View 2
	Label* assetTitle;
	ListView *lVAsset;
	MAUtil::Map<int, long long> mapAssetId;
	VerticalLayout* mainLayoutAssetChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIAsset;
	MAUtil::Map<int, Label*> mapLAssetName;

	//View 3
	ListView *lVPlugin;
	MAUtil::Map<int, long long> mapPluginId;
	VerticalLayout* mainLayoutPluginChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIPlugin;
	MAUtil::Map<int, Label*> mapLPluginName;

	//View 4
	ListView *lVInfo;
	MAUtil::Map<int, long long> mapInfoIdSrc;
	MAUtil::Map<int, long long> mapInfoIdSea;
	MAUtil::Map<int, long long> mapInfoIdUnit;
	MAUtil::Map<int, long long> mapInfoInfValueNum;
	VerticalLayout* mainLayoutInfoChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIInfo;
	MAUtil::Map<int, Label*> mapLInfoName;

	//View 5
	VerticalLayout* mainLayoutOptionChoice;
	ListView* lVOption;
	MAUtil::Map<int, ListViewItem*> mapLVIOption;
	Label* keyValue;
	EditBox* eBKeyValue;

	Label* criterion;
	Label* value;
	EditBox* eBValue;
	Label* unit;
	Button* bDest;

	//view 5.2
	ListView *lVOperator;
	MAUtil::Map<int, long long> mapOperatorId;
	VerticalLayout* mainLayoutOperatorChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIOperator;
	MAUtil::Map<int, Label*> mapLOperatorName;

	//view 5.3
	int nbOfSubUnits; //si > 0 alors on va chercher les sous unités.
	ListView *lVUnit;
	MAUtil::Map<int, long long> mapUnitId;
	VerticalLayout* mainLayoutUnitChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIUnit;
	MAUtil::Map<int, Label*> mapLUnitName;

	//View 6
	ListView *lVUser;
	MAUtil::Map<int, long long> mapUserId;
	VerticalLayout* mainLayoutUserChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIUser;
	MAUtil::Map<int, Label*> mapLUserName;

	//View 7
	ListView *lVMedia;
	MAUtil::Map<int, long long> mapMediaId;
	VerticalLayout* mainLayoutMediaChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIMedia;
	MAUtil::Map<int, Label*> mapLMediaName;

	//View 8
	ListView *lVMediaValue;
	MAUtil::Map<int, long long> mapMediaValueId;
	VerticalLayout* mainLayoutMediaValueChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIMediaValue;
	MAUtil::Map<int, Label*> mapLMediaValueName;

	//View 9
	VerticalLayout* mainLayoutSnoozeChoice;
	Label* lSnooze;
	EditBox* eBSnooze;
	Label* lSecond;
	Button* bAddToList;

	//View 10
	ListView *lVListDest;
//	MAUtil::Map<int, long long> mapMediaValueId;
	VerticalLayout* mainLayoutListDestChoice;
	MAUtil::Map<int, ListViewItem*> mapLVIListDest;
	MAUtil::Map<int, Label*> mapLListDestName;
	Button* bAddOtherDest;
	Button* bCreateAlert;
	HorizontalLayout* hLButtonListDest;
	ListViewItem* lVIButtonListDest;
	MAUtil::Map<int, long long>mapMediaValueListId;
	MAUtil::Map<int, int>mapSnoozeList;

	//Create Alert
	MAUtil::Map<int, long long>mapAMSId;
	int currentAMS;
//HTTPConnect* httpConnect;

};


#endif /* ALERTTAB_H_ */
