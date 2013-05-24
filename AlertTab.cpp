/*
 * AlertTab.cpp
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */

#include "AlertTab.h"

/**
 * Constructor.
 */
AlertTab::AlertTab() :
		Screen() {
	// Set title and icon of the stack screen.
	setTitle("Create Alert");
	posOptionAlert = -1;
//			setBackgroundColor(255,255,255);
//			httpConnect = new HTTPConnect();
//			httpConnect->connectUrl("https://alert-api.echoes-tech.com/plugins/1/sources/1/searches/?login=thomas.saquet@echoes-tech.com&password=147258369aA", "parseJSONPlugin"/*,this*/);
	createUI();

}

/**
 * Destructor.
 */
AlertTab::~AlertTab() {

}

void AlertTab::connectUrl(String url, eAlertTab fct) {
	lprintfln("connectUrl");
	if (mIsConnected == false) {
		mIsConnected = true;
		fonction = fct;
		lprintfln(url.c_str());
		this->get(url.c_str());
	} else {
		lprintfln("Déjà connecté: %d", fonction);
	}
}

void AlertTab::dataDownloaded(MAHandle data, int result) {
	lprintfln("dataDownloaded");
	mIsConnected = false;
	if (result == RES_OK) {
		switch (fonction) {
		case PLUGIN_LIST:
			parseJSONPlugin(data);
			break;
		case INFORMATION_LIST:
			parseJSONInformation(data);
			break;
		case ALERT_LIST:
			parseJSONAlert(data);
			break;
		case ASSET_LIST:
			parseJSONAsset(data);
			break;
		case OPERATOR_LIST:
			parseJSONOperator(data);
			break;
		case UNIT_INFO:
			parseJSONUnitInfo(data);
			break;
		case SUB_UNIT_INFO:
			parseJSONSubUnitInfo(data);
			break;
		default:
			break;
		}
	} else if (result == CONNERR_DNS) {
		lprintfln("AlertTab DataDownload result = %d", result);
		lprintfln("DNS resolution error.");
	} else {
		lprintfln("AlertTab DataDownload result = %d", result);
	}
	this->close();
}

void AlertTab::parseJSONSubUnitInfo(MAHandle data) {
	lprintfln("parseJSONSubUnitInfo");
	char * jsonData = new char[maGetDataSize(data) + 1];
	maReadData(data, jsonData, 0, maGetDataSize(data));
	MAUtil::YAJLDom::Value* root = YAJLDom::parse(
			(const unsigned char*) jsonData, maGetDataSize(data));

	// Traverse the Json tree and print data.
	// Check that the root is valid.
	// The root type should have type with above data ARRAY.

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
			MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
			mapLUnitName[idx + 1] = new Label();
			mapLUnitName[idx + 1]->setText(
					valueTmp->getValueForKey("name")->toString());
			mapUnitId[idx + 1] = valueTmp->getValueForKey("id")->toInt();
		}
		createUnitPage();
	}

}

void AlertTab::parseJSONUnitInfo(MAHandle data) {
	lprintfln("parseJSONUnitInfo");
	char * jsonData = new char[maGetDataSize(data) + 1];
	maReadData(data, jsonData, 0, maGetDataSize(data));
	MAUtil::YAJLDom::Value* root = YAJLDom::parse(
			(const unsigned char*) jsonData, maGetDataSize(data));

	// Traverse the Json tree and print data.
	// Check that the root is valid.
	// The root type should have type with above data ARRAY.

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY == root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
	}

	mapLUnitName[0] = new Label();
	mapLUnitName[0]->setText(root->getValueForKey("name")->toString());
	String UnitTmp = "Unit : ";
	UnitTmp += mapLUnitName[0]->getText();
	unit->setText(UnitTmp);
	if (root->getValueForKey("information_sub_units")->toInt() != 0) {
		String urlTmp = HOST;
		urlTmp += "/units/";
		urlTmp += Convert::toString(mapUnitId[0]);
		urlTmp += "/subunits/";
		urlTmp += LOGIN;
		connectUrl(urlTmp, SUB_UNIT_INFO);
	}
	else
	{
		createUnitPage();
	}
}

void AlertTab::parseJSONOperator(MAHandle data) {
	lprintfln("parseJSONOperator");
	char * jsonData = new char[maGetDataSize(data) + 1];
	maReadData(data, jsonData, 0, maGetDataSize(data));
	MAUtil::YAJLDom::Value* root = YAJLDom::parse(
			(const unsigned char*) jsonData, maGetDataSize(data));
	// Traverse the Json tree and print data.
	// Check that the root is valid.
	// The root type should have type with above data ARRAY.
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		mainLayoutOperatorChoice = new VerticalLayout();
		// Make the layout fill the entire screen.
		mainLayoutOperatorChoice->fillSpaceHorizontally();
		mainLayoutOperatorChoice->fillSpaceVertically();
		Screen::setMainWidget(mainLayoutOperatorChoice);
		lVOperator = new ListView();
		lVOperator->addListViewListener(this);
		mainLayoutOperatorChoice->addChild(lVOperator);
	}
	for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
		//				lprintfln("dans le for Asset");
		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
		//				mapAssetName[idx] = valueTmp->getValueForKey("name")->toString();
		mapOperatorId[idx] = valueTmp->getValueForKey("id")->toInt();
		//				lprintfln(mapAssetName[idx].c_str());
		mapLOperatorName[idx] = new Label();
		mapLOperatorName[idx]->setText(
				valueTmp->getValueForKey("name")->toString().c_str());
		//				mapLAssetName[idx]->setText(mapAssetName[idx].c_str());
		mapLVIOperator[idx] = new ListViewItem();
		mapLVIOperator[idx]->addChild(mapLOperatorName[idx]);
		lVOperator->addChild(mapLVIOperator[idx]);

	}

}

void AlertTab::parseJSONAsset(MAHandle data) {
	lprintfln("parseJSONAsset");
	char * jsonData = new char[maGetDataSize(data) + 1];
	maReadData(data, jsonData, 0, maGetDataSize(data));
	MAUtil::YAJLDom::Value* root = YAJLDom::parse(
			(const unsigned char*) jsonData, maGetDataSize(data));
	// Traverse the Json tree and print data.
	// Check that the root is valid.
	// The root type should have type with above data ARRAY.
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		mainLayoutAssetChoice = new VerticalLayout();
		// Make the layout fill the entire screen.
		mainLayoutAssetChoice->fillSpaceHorizontally();
		mainLayoutAssetChoice->fillSpaceVertically();
		Screen::setMainWidget(mainLayoutAssetChoice);
		assetTitle = new Label();
		assetTitle->setText("Choix des server");
		mainLayoutAssetChoice->addChild(assetTitle);
		lVAsset = new ListView();
		lVAsset->addListViewListener(this);
		mainLayoutAssetChoice->addChild(lVAsset);
	}
	for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
//				lprintfln("dans le for Asset");
		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
//				mapAssetName[idx] = valueTmp->getValueForKey("name")->toString();
		mapAssetId[idx] = valueTmp->getValueForKey("id")->toInt();
//				lprintfln(mapAssetName[idx].c_str());
		mapLAssetName[idx] = new Label();
		mapLAssetName[idx]->setText(
				valueTmp->getValueForKey("name")->toString().c_str());
//				mapLAssetName[idx]->setText(mapAssetName[idx].c_str());
		mapLVIAsset[idx] = new ListViewItem();
		mapLVIAsset[idx]->addChild(mapLAssetName[idx]);
		lVAsset->addChild(mapLVIAsset[idx]);

	}
}

void AlertTab::parseJSONAlert(MAHandle data) {
	lprintfln("parseJSONAlert");
	char * jsonData = new char[maGetDataSize(data) + 1];
	maReadData(data, jsonData, 0, maGetDataSize(data));
	MAUtil::YAJLDom::Value* root = YAJLDom::parse(
			(const unsigned char*) jsonData, maGetDataSize(data));
	// Traverse the Json tree and print data.
	// Check that the root is valid.
	// The root type should have type with above data ARRAY.
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");

	} else {
		lprintfln("Root node is valid :) \n");
	}

	for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
		MAUtil::YAJLDom::Value* valueTmp1 = valueTmp->getValueForKey("alert");

		String test1 = valueTmp1->getValueForKey("name")->toString();
//				lprintfln("dans le for");
//				mapAlertName[idx] = valueTmp1->getValueForKey("name")->toString();
//				mapInformationName[idx] = valueTmp->getValueForKey("id")->toInt();
//				int test = valueTmp->getValueForKey("id")->toInt();
//				lprintfln(mapAlertName[idx].c_str());
//				lprintfln("%d", test);

		mapLAlertName[idx] = new Label();
		mapLAlertName[idx]->setText(
				valueTmp1->getValueForKey("name")->toString().c_str());

		mapLVIAlert[idx] = new ListViewItem();
		mapLVIAlert[idx]->addChild(mapLAlertName[idx]);

		lValert->addChild(mapLVIAlert[idx]);
	}
}

void AlertTab::parseJSONInformation(MAHandle data) {
	char * jsonData = new char[maGetDataSize(data) + 1];
	maReadData(data, jsonData, 0, maGetDataSize(data));
	MAUtil::YAJLDom::Value* root = YAJLDom::parse(
			(const unsigned char*) jsonData, maGetDataSize(data));
	// Traverse the Json tree and print data.
	// Check that the root is valid.
	// The root type should have type with above data ARRAY.
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");

	} else {
		lprintfln("Root node is valid :) \n");
		mainLayoutInfoChoice = new VerticalLayout();
		// Make the layout fill the entire screen.
		mainLayoutInfoChoice->fillSpaceHorizontally();
		mainLayoutInfoChoice->fillSpaceVertically();
		Screen::setMainWidget(mainLayoutInfoChoice);
		lVInfo = new ListView();
		lVInfo->addListViewListener(this);
		mainLayoutInfoChoice->addChild(lVInfo);
	}

	lprintfln("%d\n", root->getNumChildValues());
	for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
		MAUtil::YAJLDom::Value* valueTmp1 = valueTmp->getValueForKey("id");
		mapLInfoName[idx] = new Label();
		mapLInfoName[idx]->setText(
				valueTmp->getValueForKey("name")->toString().c_str());
		mapInfoIdSea[idx] = valueTmp1->getValueForKey("search_id")->toInt();
		mapInfoIdSrc[idx] = valueTmp1->getValueForKey("source_id")->toInt();
		mapInfoIdUnit[idx] = valueTmp1->getValueForKey("unit_id")->toInt();
		mapLVIInfo[idx] = new ListViewItem();
		mapLVIInfo[idx]->addChild(mapLInfoName[idx]);
		lVInfo->addChild(mapLVIInfo[idx]);
	}
}

void AlertTab::parseJSONPlugin(MAHandle data) {
	char * jsonData = new char[maGetDataSize(data) + 1];
	maReadData(data, jsonData, 0, maGetDataSize(data));
	MAUtil::YAJLDom::Value* root = YAJLDom::parse(
			(const unsigned char*) jsonData, maGetDataSize(data));
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");

	} else {
		lprintfln("Root node is valid :) \n");
		mainLayoutPluginChoice = new VerticalLayout();
		mainLayoutPluginChoice->fillSpaceHorizontally();
		mainLayoutPluginChoice->fillSpaceVertically();
		Screen::setMainWidget(mainLayoutPluginChoice);
		lVPlugin = new ListView();
		lVPlugin->addListViewListener(this);
		mainLayoutPluginChoice->addChild(lVPlugin);
	}

	lprintfln("%d\n", root->getNumChildValues());
	for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);

		String test1 = valueTmp->getValueForKey("name")->toString();
//		mapPluginName[idx] = valueTmp->getValueForKey("name")->toString();
		mapPluginId[idx] = valueTmp->getValueForKey("id")->toInt();
		mapLPluginName[idx] = new Label();
		mapLPluginName[idx]->setText(
				valueTmp->getValueForKey("name")->toString().c_str());
		mapLVIPlugin[idx] = new ListViewItem();
		mapLVIPlugin[idx]->addChild(mapLPluginName[idx]);
		lVPlugin->addChild(mapLVIPlugin[idx]);
	}
}

void AlertTab::listViewItemClicked(ListView* listView,
		ListViewItem* listViewItem) {
	lprintfln("Clicked");
	if (listView == lValert) {
		if (posOptionAlert != -1)//savoir si on a ouvert les options (supprimer/annuler/view), d'une alerte dans la View 1
				{
			mapLVIAlert[posOptionAlert]->removeChild(HLOptionAlert);
			mapLVIAlert[posOptionAlert]->addChild(
					mapLAlertName[posOptionAlert]);
			posOptionAlert = -1;
		}
		for (int i = 0; i < mapLVIAlert.size(); i++) {
			if (mapLVIAlert[i] == listViewItem) {
				posOptionAlert = i;
				mapLVIAlert[i]->removeChild(mapLAlertName[i]);
				HLOptionAlert = new HorizontalLayout();

				Button *bDelete = new Button();
				bDelete->setText("supprimer");
				HLOptionAlert->addChild(bDelete);
				bCancel = new Button();
				bCancel->setText("annuler");
				bCancel->addButtonListener(this);
				HLOptionAlert->addChild(bCancel);
				Button *bView = new Button();
				bView->setText("detail");
				HLOptionAlert->addChild(bView);
				mapLVIAlert[i]->addChild(HLOptionAlert);
			}
		}
	} else if (listView == lVAsset) {
		for (int i = 0; i < mapLVIAsset.size(); i++) {
			if (mapLVIAsset[i] == listViewItem) {
				String urlTmp = HOST;
				urlTmp += "/assets/";
				urlTmp += Convert::toString(mapAssetId[i]);
				urlTmp += "/plugins/";
				urlTmp += LOGIN;
				lprintfln(urlTmp.c_str());
				connectUrl(urlTmp, PLUGIN_LIST);
			}
		}
	} else if (listView == lVPlugin) {
		for (int i = 0; i < mapLVIPlugin.size(); i++) {
			if (mapLVIPlugin[i] == listViewItem) {
				String urlTmp = HOST;
				urlTmp += "/plugins/";
				urlTmp += Convert::toString(mapPluginId[i]);
				urlTmp += "/informations/";
				urlTmp += LOGIN;
				lprintfln(urlTmp.c_str());
				connectUrl(urlTmp, INFORMATION_LIST);
			}
		}
	} else if (listView == lVInfo) {

		for(int idx = 0; idx<mapInfoIdUnit.size(); idx++)
		{
			if(listViewItem == mapLVIInfo[idx])
			{
				mapUnitId[0] = mapInfoIdUnit[idx];
			}
		}
		String urlTmp = HOST;
		urlTmp += "/units/";
		urlTmp += Convert::toString(mapUnitId[0]);
		urlTmp += LOGIN;
		connectUrl(urlTmp, UNIT_INFO);
		createOptionPage();

	} else if (listViewItem == mapLVIOption[1]) {
		String urlTmp = HOST;
		urlTmp += "/criteria/";
		urlTmp += LOGIN;
		connectUrl(urlTmp, OPERATOR_LIST);
	} else if (listView == lVOperator) {
		String criterionTmp = "operateur : ";
		for (int idx = 0; idx < mapLVIOperator.size(); idx++) {
			if (mapLVIOperator[idx] == listViewItem) {
				criterionTmp += mapLOperatorName[idx]->getText();
			}
		}
		criterion->setText(criterionTmp);
		Screen::setMainWidget(mainLayoutOptionChoice);
	} else if (listViewItem == mapLVIOption[3]) {
		Screen::setMainWidget(mainLayoutUnitChoice);
	} else if (listView == lVUnit) {
		Screen::setMainWidget(mainLayoutOptionChoice);
	}

}

void AlertTab::buttonClicked(Widget* button) {
	lprintfln("bouton click");
	if (button == bAddAlert) {
		String urlTmp = HOST;
		urlTmp += "/assets/";
		urlTmp += LOGIN;
		connectUrl(urlTmp, ASSET_LIST);
	} else if (button == bCancel) {
		mapLVIAlert[posOptionAlert]->removeChild(HLOptionAlert);
		mapLVIAlert[posOptionAlert]->addChild(mapLAlertName[posOptionAlert]);
		posOptionAlert = -1;
	}
}

void AlertTab::createUnitPage()
{
	mainLayoutUnitChoice = new VerticalLayout();
	mainLayoutUnitChoice->fillSpaceHorizontally();
	mainLayoutUnitChoice->fillSpaceVertically();

	lVUnit = new ListView();
	lVUnit->addListViewListener(this);
	mainLayoutUnitChoice->addChild(lVUnit);

	for (int idx = 0; idx <= mapUnitId.size() - 1; idx++) {
			mapLVIUnit[idx] = new ListViewItem();
			mapLVIUnit[idx]->addChild(mapLUnitName[idx]);
			lVUnit->addChild(mapLVIUnit[idx]);
		}
}

void AlertTab::createOptionPage() {
	mainLayoutOptionChoice = new VerticalLayout();
	mainLayoutOptionChoice->fillSpaceHorizontally();
	mainLayoutOptionChoice->fillSpaceVertically();

	lVOption = new ListView();
	lVOption->addListViewListener(this);
	mainLayoutOptionChoice->addChild(lVOption);
	mapLVIOption[0] = new ListViewItem();

	keyValue = new Label();
	keyValue->setText("choix de la valeur clé");
	mapLVIOption[0]->addChild(keyValue);
	//		mapLVIOption[0]->setEnabled(false);
	lVOption->addChild(mapLVIOption[0]);
	mapLVIOption[1] = new ListViewItem();
	criterion = new Label();
	criterion->setText("choix du critére");
	mapLVIOption[1]->addChild(criterion);
	lVOption->addChild(mapLVIOption[1]);

	mapLVIOption[2] = new ListViewItem();
	HorizontalLayout *hLTmp = new HorizontalLayout();
	mapLVIOption[2]->addChild(hLTmp);
	value = new Label();
	value->setText("choix de la valeur : ");
	hLTmp->addChild(value);
	eBValue = new EditBox();
	hLTmp->addChild(eBValue);
	lVOption->addChild(mapLVIOption[2]);

	mapLVIOption[3] = new ListViewItem();
	unit = new Label();
	unit->setText("unit : ");
	mapLVIOption[3]->addChild(unit);
	lVOption->addChild(mapLVIOption[3]);

	bDest = new Button();
	bDest->setText("saisir les destinataires");
	mainLayoutOptionChoice->addChild(bDest);
	Screen::setMainWidget(mainLayoutOptionChoice);
}

void AlertTab::createUI() {

	String urlTmp = HOST;
	urlTmp += "/alerts/";
	urlTmp += LOGIN;
	connectUrl(urlTmp, ALERT_LIST);

	mainLayoutAlertChoice = new VerticalLayout();
	// Make the layout fill the entire screen.
	mainLayoutAlertChoice->fillSpaceHorizontally();
	mainLayoutAlertChoice->fillSpaceVertically();
	Screen::setMainWidget(mainLayoutAlertChoice);
	lValert = new ListView();
	lValert->addListViewListener(this);
	lValert->fillSpaceVertically();
	ListViewItem *lviAddAlert = new ListViewItem();
	bAddAlert = new Button();
	bAddAlert->addButtonListener(this);
	bAddAlert->fillSpaceHorizontally();
	bAddAlert->setText("Add an alert");
	lviAddAlert->addChild(bAddAlert);
	mainLayoutAlertChoice->addChild(lValert);
	mainLayoutAlertChoice->addChild(lviAddAlert);
}
