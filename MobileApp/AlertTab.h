/*
 * AlertTab.h
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */

#ifndef ALERTTAB_H_
#define ALERTTAB_H_

//#include "HTTPConnect.h"
//#include <NativeUI/Screen.h>
//#include <MAUtil/Map.h>
//
//#include <NativeUI/Widgets.h>// Include all widgets
//
//#include <Wormhole/HighLevelHttpConnection.h>
//#include <Wormhole/MessageStreamJSON.h>
//#include <mastdlib.h>

#include "IncludeFile.h"
#include "Util.h"
#include "conf.h"
#include "resource/Convert.h"
#include "MAHeaders.h"
#include "Connections.h"

#include "Page.h"

#include "ActivityPage.h"

//#define BUFSIZE 1024
//using namespace Wormhole;
using namespace MAUtil;
using namespace NativeUI;


class AlertTab : public NativeUI::Screen, public ButtonListener/*, public HighLevelHttpConnection*/, public ListViewListener, public EditBoxListener, public Connections
{
public:

//	enum eAlertTab {
//		PLUGIN_LIST = 0,
//		INFORMATION_LIST,
//		ALERT_LIST,
//		ASSET_LIST,
//		OPERATOR_LIST,
//		UNIT_INFO,
//		SUB_UNIT_INFO,
//		USER_LIST,
//		MEDIA_VALUE_LIST,
//		MEDIA_TYPE,
//		POST_MEDIA_SPE,
//		POST_ALERT,
//		SEARCH_INFO,
//		ALERT_INFO,
//		ALERT_RECIPIENT_LIST,
//		UNITS_TYPES
//	};

	/**
	 * Constructor.
	 */
	AlertTab(int language, String loginToken, eScreenResolution screenResolution);

	/**
	 * Destructor.
	 */
	virtual ~AlertTab();

	void handleKeyPress(int keyCode);

//	void connectUrl(String url, eAlertTab fct, int verb = GET, String jsonMessage = "");
//	void dataDownloaded(MAHandle , int );

	virtual void connectUrl1();
	virtual void dataDownload1(MAUtil::YAJLDom::Value*, int result, eFonction fonction);

	virtual void buttonClicked(Widget* button);
	virtual void listViewItemClicked(ListView* listView, ListViewItem* listViewItem);

	virtual void editBoxEditingDidEnd(EditBox* editBox) {}
	virtual void editBoxReturn(EditBox* editBox){maWidgetSetProperty(editBox->getWidgetHandle(), MAW_EDIT_BOX_SHOW_KEYBOARD, "false");}
	virtual void editBoxEditingDidBegin(EditBox* editBox) {maWidgetSetProperty(editBox->getWidgetHandle(), MAW_EDIT_BOX_SHOW_KEYBOARD, "true");}


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
	void parseJSONAlertRecipientList(MAUtil::YAJLDom::Value* root);
	void parseJSONUnitsTypes(MAUtil::YAJLDom::Value* root);
	void parseJSONDeleteAlert(MAUtil::YAJLDom::Value* root);


	void createUI();
	void createOptionPage();
	void createUnitPage();
	void createSnoozePage();
	void createDestListPage();
	void createDetailAlertPage();

	void orientationChange(int screenOrientation);

	bool optionPageValid();
	bool snoozePageValid();

private:
	int activateKeyBoard;
	ActivityPage* mActivityPage;

	String _LOGINTOKEN;
	int LANGUAGE;
//	int connERR;

//	eAlertTab fonction;
//	char mBuffer[BUFSIZE];
//	bool mIsConnected;
//	int count;
//	String contentString;
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

	Page* mainLayoutAlertChoice;
	Label *lListNoAlert;
	ListView *lValert;
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
	Page* mainLayoutAlertDetailChoice;
	Label* lAlertAsset;
	Label* lAlertPlugin;
	Label* lAlertInformation;
	Label* lAlertOperator;
	Label* lAlertValue;
	Label* lAlertUnit;
	Label* lAlertKeyValue;
	Label* lAlertMedia;
	Label* lAlertSnooze;
//
	ListView* lVRecipients;
	Label* lAlertRecipient;
	MAUtil::Map<int, Label*> mapLRecipients;
	MAUtil::Map<int, ListViewItem*> mapLVIRecipients;



	//View 2
	Page* mainLayoutAssetChoice;
	ListView *lVAsset;
	MAUtil::Map<int, long long> mapAssetId;
	MAUtil::Map<int, ListViewItem*> mapLVIAsset;
	MAUtil::Map<int, String> mapLAssetName;

	//View 3
	Page* mainLayoutPluginChoice;
	ListView *lVPlugin;
	MAUtil::Map<int, long long> mapPluginId;
	MAUtil::Map<int, ListViewItem*> mapLVIPlugin;
	MAUtil::Map<int, Label*> mapLPluginName;

	//View 4
	Page* mainLayoutInfoChoice;
	ListView *lVInfo;
	MAUtil::Map<int, long long> mapInfoIdSrc;
	MAUtil::Map<int, long long> mapInfoIdSea;
	MAUtil::Map<int, long long> mapInfoIdUnit;
	MAUtil::Map<int, long long> mapInfoInfValueNum;
	MAUtil::Map<int, ListViewItem*> mapLVIInfo;
	MAUtil::Map<int, Label*> mapLInfoName;

	//View 5
	Page* mainLayoutOptionChoice;
	ListView* lVOption;
	ListView* lVCriteria;
	MAUtil::Map<int, ListViewItem*> mapLVIOption;
	Label* keyValue;
	EditBox* eBKeyValue;

	Label* criterion;
	Label* value;
	EditBox* eBValue;
	Label* unit;
	Button* bDest;

	//view 5.2
	Page* mainLayoutOperatorChoice;
	ListView *lVOperator;
	MAUtil::Map<int, long long> mapOperatorId;
	MAUtil::Map<int, ListViewItem*> mapLVIOperator;
	MAUtil::Map<int, Label*> mapLOperatorName;

	//view 5.3
	Page* mainLayoutUnitChoice;
	int nbOfSubUnits; //si > 0 alors on va chercher les sous unités.
	ListView *lVUnit;
	MAUtil::Map<int, long long> mapUnitId;
	MAUtil::Map<int, ListViewItem*> mapLVIUnit;
	MAUtil::Map<int, Label*> mapLUnitName;

	//View 6
	Page* mainLayoutUserChoice;
	ListView *lVUser;
	MAUtil::Map<int, long long> mapUserId;
	MAUtil::Map<int, ListViewItem*> mapLVIUser;
	MAUtil::Map<int, Label*> mapLUserName;

	//View 7
	Page* mainLayoutMediaChoice;
	ListView *lVMedia;
	MAUtil::Map<int, long long> mapMediaId;
	MAUtil::Map<int, ListViewItem*> mapLVIMedia;
	MAUtil::Map<int, Label*> mapLMediaName;

	//View 8
	Page* mainLayoutMediaValueChoice;
	ListView *lVMediaValue;
	MAUtil::Map<int, long long> mapMediaValueId;
	MAUtil::Map<int, ListViewItem*> mapLVIMediaValue;
	MAUtil::Map<int, Label*> mapLMediaValueName;

	//View 9
	Page* mainLayoutSnoozeChoice;;
	Label* lSnooze;
	EditBox* eBSnooze;
	Label* lSecond;
	Button* bAddToList;

	//View 10
	Page* mainLayoutListDestChoice;
	ListView *lVListDest;
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

};


#endif /* ALERTTAB_H_ */
