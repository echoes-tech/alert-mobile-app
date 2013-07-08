/*
 * AlertTab.cpp
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */

#include "AlertTab.h"
#include <MAUI/Image.h>

#include "Page.h"
/**
 * Constructor.
 */
AlertTab::AlertTab(int language, String loginToken, eScreenResolution screenResolution) :
		Screen(), LANGUAGE(language), _LOGINTOKEN(loginToken) {

	mActivityPage = new ActivityPage();
//	mIsConnected = false;
	setIcon(ICON_ALERT + screenResolution);

	selectedAsset = -1;
	selectedPlugin = -1;
	selectedInformation = -1;
	selectedUnit = -1;
	selectedOperator = -1;
	selectedUser = -1;
	selectedMedia = -1;
	selectedMediaValue = -1;
	posOptionAlert = -1;
	createUI();
}

/**
 * Destructor.
 */
AlertTab::~AlertTab() {

}

void AlertTab::handleKeyPress(int keyCode) {
//	lprintfln("handleKeyPress : %d", keyCode);
	if (keyCode == MAK_BACK) {
		if (activeMainLayout == mainLayoutAssetChoice) {
			activeMainLayout = mainLayoutAlertChoice;
		} else if (activeMainLayout == mainLayoutPluginChoice) {
			activeMainLayout = mainLayoutAssetChoice;
		} else if (activeMainLayout == mainLayoutInfoChoice) {
			activeMainLayout = mainLayoutPluginChoice;
		} else if (activeMainLayout == mainLayoutOptionChoice) {
			activeMainLayout = mainLayoutInfoChoice;
		} else if (activeMainLayout == mainLayoutOperatorChoice
				|| activeMainLayout == mainLayoutUnitChoice) {
			activeMainLayout = mainLayoutOptionChoice;
		} else if (activeMainLayout == mainLayoutMediaChoice) {
			activeMainLayout = mainLayoutUserChoice;
		} else if (activeMainLayout == mainLayoutMediaValueChoice) {
			activeMainLayout = mainLayoutMediaChoice;
		} else if (activeMainLayout == mainLayoutSnoozeChoice) {
			activeMainLayout = mainLayoutMediaValueChoice;
		} else if (activeMainLayout == mainLayoutAlertDetailChoice) {
			activeMainLayout = mainLayoutAlertChoice;
		} else if (activeMainLayout == mainLayoutUserChoice) {
			if (mapLListDestName.size() > 0) {
				activeMainLayout = mainLayoutListDestChoice;
			} else {
				activeMainLayout = mainLayoutOptionChoice;
			}
		} else if (activeMainLayout == mainLayoutAlertChoice) {
			maAlert("", "",
					Convert::tr(Screen_Main_Button_close_app + LANGUAGE),
					Convert::tr(Screen_Main_Button_home + LANGUAGE),
					Convert::tr(Screen_Main_Button_cancel + LANGUAGE));
		}

//		activeMainLayout->setHeight(30000);
		Screen::setMainWidget(activeMainLayout);
		activeMainLayout->fillSpaceVertically();
		activeMainLayout->fillSpaceHorizontally();

	}
}
void AlertTab::connectUrl1(){
	Screen::setMainWidget(mActivityPage);
}

void AlertTab::dataDownload1(MAUtil::YAJLDom::Value* root, int result, eFonction fonction)
{
	lprintfln("AlertTab DataDownload1");
	if (result == RES_OK || result == 204) {
			switch (fonction) {
			case PLUGIN_LIST:
				parseJSONPlugin(root);
				break;
			case INFORMATION_LIST:
				parseJSONInformation(root);
				break;
			case ALERT_LIST:
				parseJSONAlert(root);
				break;
			case ASSET_LIST:
				parseJSONAsset(root);
				break;
			case OPERATOR_LIST:
				parseJSONOperator(root);
				break;
			case UNIT_INFO:
				parseJSONUnitInfo(root);
				break;
			case SUB_UNIT_INFO:
				parseJSONSubUnitInfo(root);
				break;
			case USER_LIST:
				parseJSONUserForOrganisation(root);
				break;
			case MEDIA_TYPE:
				parseJSONMediaType(root);
				break;
			case MEDIA_VALUE_LIST:
				parseJSONMediaValue(root);
				break;
			case POST_MEDIA_SPE:
				parseJSONPostMediaSpe(root);
				break;
			case POST_ALERT:
				parseJSONPostAlert(root);
				break;
			case SEARCH_INFO:
				parseJSONSearchInfo(root);
				break;
			case ALERT_INFO:
				parseJSONAlertInfo(root);
				break;
			case ALERT_RECIPIENT_LIST:
				parseJSONAlertRecipientList(root);
				break;
			case UNITS_TYPES:
				parseJSONUnitsTypes(root);
				break;
			case DELETE_ALERT_LIST:
				parseJSONDeleteAlert(root);
				break;
			default:
				break;

				delete root;
			}
		} else if (result == CONNERR_DNS) {
			lprintfln("AlertTab DataDownload result = %d", result);
			lprintfln("DNS resolution error.");
		} else if (result == 404 && fonction == ALERT_LIST) {
	//		lListNoAlert->setText(Convert::tr(alert_list_Label_title_no_alert + LANGUAGE));
			lListNoAlert->setHeight(EXTENT_Y(maGetTextSize(""))*4);
	//		lListTitle->setText(
	//				Convert::tr(alert_list_Label_title_no_alert + LANGUAGE));
		}  else if (result == 503 && fonction == POST_ALERT) {
			maMessageBox(Convert::tr(alert_message_box_title_Error + LANGUAGE), Convert::tr(alert_message_box_create_alert_error + LANGUAGE));

			activeMainLayout = mainLayoutAlertChoice;
		} else if (result != RES_OK && (fonction == POST_ALERT || fonction == ALERT_LIST) ){
			String urlTmp = HOST;
			urlTmp += "/alerts/";
			urlTmp += _LOGINTOKEN;
			connectUrl(urlTmp, ALERT_LIST);
		}
		else if (result == 404 && fonction == MEDIA_TYPE){
			maToast(Convert::tr(alert_toast_no_media_found + LANGUAGE),MA_TOAST_DURATION_SHORT);
		}else if (result != 404 && fonction == DELETE_ALERT_LIST){
			maMessageBox("ERREUR", "ALERT not deleted");
		}else {
			lprintfln("AlertTab DataDownload result = %d", result);
		}
		if (result != RES_OK && result != 204) {
			activeMainLayout->setHeight(30000);
			Screen::setMainWidget(activeMainLayout);
			activeMainLayout->fillSpaceVertically();
			activeMainLayout->fillSpaceHorizontally();
		}
}

//void AlertTab::connectUrl(String url, eAlertTab fct, int verb,
//		String jsonMessage) {
//	Screen::setMainWidget(mActivityPage);
//	lprintfln("connectUrl");
//
//	if (mIsConnected == false) {
//		mIsConnected = true;
//		fonction = fct;
//		lprintfln(url.c_str());
//		if (verb == GET) {
// //			lprintfln("GET");
//			int tmp = this->get(url.c_str());
//			lprintfln("GET send = %d", tmp);
//		} else if (verb == POST) {
// //			lprintfln("POST");
//			lprintfln(jsonMessage.c_str());
//			int tmp = this->postJsonRequest(url.c_str(), jsonMessage.c_str());
//			//			Vector<String> test; test.clear();
//			//			int tmp = this->postRequest(url.c_str(), test, jsonMessage.c_str(), jsonMessage.size());
//			lprintfln("POST send = %d", tmp);
//		}
//	} else {
//		lprintfln("Déjà connecté: %d", fonction);
//	}
//}

//void AlertTab::dataDownloaded(MAHandle data, int result) {
//	this->close();
//	lprintfln("dataDownloaded");
//	mIsConnected = false;
//	String sMessage = "";
//	if (result == RES_OK) {
//		connERR = 0;
//		char * jsonData = new char[maGetDataSize(data) + 1];
//		maReadData(data, jsonData, 0, maGetDataSize(data));
//		String jsonTmp = jsonData;
//		Convert::formatJSONBeforeParse(jsonTmp);
//
//		MAUtil::YAJLDom::Value *root = YAJLDom::parse(
//				(const unsigned char*) jsonTmp.c_str(), maGetDataSize(data));
//		switch (fonction) {
//		case PLUGIN_LIST:
//			parseJSONPlugin(root);
//			break;
//		case INFORMATION_LIST:
//			parseJSONInformation(root);
//			break;
//		case ALERT_LIST:
//			parseJSONAlert(root);
//			break;
//		case ASSET_LIST:
//			parseJSONAsset(root);
//			break;
//		case OPERATOR_LIST:
//			parseJSONOperator(root);
//			break;
//		case UNIT_INFO:
//			parseJSONUnitInfo(root);
//			break;
//		case SUB_UNIT_INFO:
//			parseJSONSubUnitInfo(root);
//			break;
//		case USER_LIST:
//			parseJSONUserForOrganisation(root);
//			break;
//		case MEDIA_TYPE:
//			parseJSONMediaType(root);
//			break;
//		case MEDIA_VALUE_LIST:
//			parseJSONMediaValue(root);
//			break;
//		case POST_MEDIA_SPE:
//			parseJSONPostMediaSpe(root);
//			break;
//		case POST_ALERT:
//			parseJSONPostAlert(root);
//			break;
//		case SEARCH_INFO:
//			parseJSONSearchInfo(root);
//			break;
//		case ALERT_INFO:
//			parseJSONAlertInfo(root);
//			break;
//		case ALERT_RECIPIENT_LIST:
//			parseJSONAlertRecipientList(root);
//			break;
//		case UNITS_TYPES:
//			parseJSONUnitsTypes(root);
//		default:
//			break;
//
//			delete jsonData;
//			delete root;
//		}
//	} else if (result == CONNERR_DNS) {
//		connERR++;
//		lprintfln("AlertTab DataDownload result = %d", result);
//		lprintfln("DNS resolution error.");
//	} else if (result == 404 && fonction == ALERT_LIST) {
// //		lListNoAlert->setText(Convert::tr(alert_list_Label_title_no_alert + LANGUAGE));
//		lListNoAlert->setHeight(EXTENT_Y(maGetTextSize(""))*4);
// //		lListTitle->setText(
// //				Convert::tr(alert_list_Label_title_no_alert + LANGUAGE));
//	}  else if (result == 503 && fonction == POST_ALERT) {
//		maMessageBox(Convert::tr(alert_message_box_title_Error + LANGUAGE), Convert::tr(alert_message_box_create_alert_error + LANGUAGE));
//
//		activeMainLayout = mainLayoutAlertChoice;
//	} else if (result != RES_OK && (fonction == POST_ALERT || fonction == ALERT_LIST) ){
//		String urlTmp = HOST;
//		urlTmp += "/alerts/";
//		urlTmp += _LOGINTOKEN;
//		connectUrl(urlTmp, ALERT_LIST);
//	}
//	else if (result == 404 && fonction == MEDIA_TYPE){
//		maToast(Convert::tr(alert_toast_no_media_found + LANGUAGE),MA_TOAST_DURATION_SHORT);
//	}else {
//		connERR++;
//		lprintfln("AlertTab DataDownload result = %d", result);
//	}
//	if (connERR >= 3) {
//		if (result != 404) {
//			sMessage = "Connection Error. ERREUR :";
//			sMessage += Convert::toString(result);
//			maMessageBox("Connection Error", sMessage.c_str());
//		}
//		getSystemConnection();
//	}
//	if (result != RES_OK) {
//		activeMainLayout->setHeight(30000);
//		Screen::setMainWidget(activeMainLayout);
//		activeMainLayout->fillSpaceVertically();
//		activeMainLayout->fillSpaceHorizontally();
//	}
//}

void AlertTab::parseJSONDeleteAlert(MAUtil::YAJLDom::Value* root){
	lprintfln("parseJSONDeleteAlert");
	String urlTmp = HOST;
				urlTmp += "/alerts/";
				urlTmp += _LOGINTOKEN;
				connectUrl(urlTmp, ALERT_LIST);
				for (int idx0 = 0; idx0 < mapAlertId.size(); idx0++) {
					lValert->removeChild(mapLVIAlert[idx0]);
				}
				mapAlertId.clear();
}

void AlertTab::parseJSONAlertRecipientList(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONAlertRecipientList");
	int tmpSize = mapLVIRecipients.size();
	for (int idx1 = 0; idx1 < tmpSize; idx1++) {
		lVRecipients->removeChild(mapLVIRecipients[idx1]);
		mapLVIRecipients[idx1]->removeChild(mapLRecipients[idx1]);
		delete mapLRecipients[idx1];
		delete mapLVIRecipients[idx1];
	}
	mapLRecipients.clear();
	mapLVIRecipients.clear();

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
			MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
			MAUtil::YAJLDom::Value* valueTmp1 = valueTmp->getValueForKey(
					"media_value");

			mapLVIRecipients[idx] = new ListViewItem();
			lVRecipients->addChild(mapLVIRecipients[idx]);
			mapLRecipients[idx] = new Label(
					valueTmp1->getValueForKey("value")->toString());
			mapLVIRecipients[idx]->addChild(mapLRecipients[idx]);
		}

		Screen::setMainWidget(mainLayoutAlertDetailChoice);
		activeMainLayout = mainLayoutAlertDetailChoice;
	}
}

void AlertTab::parseJSONUnitsTypes(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONUnitsTypes");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY == root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		//		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(0);
		//		MAUtil::YAJLDom::Value* valueTmp1 = root->getValueForKey("id");
		if (root->getValueForKey("name")->toString() == "number") {
			eBValue->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
		} else {
			eBValue->setInputMode(EDIT_BOX_INPUT_MODE_ANY);
		}
		Screen::setMainWidget(mainLayoutOptionChoice);
		activeMainLayout = mainLayoutOptionChoice;
	}
}

void AlertTab::parseJSONSearchInfo(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONSearchInfo");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY == root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
//		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(0);
//		MAUtil::YAJLDom::Value* valueTmp1 = root->getValueForKey("id");
		if (root->getValueForKey("pos_key_value")->toInt() == 0) {
//			lprintfln("pasdispo");
			keyValue->setFontColor(0x606060);
			eBKeyValue->setText("N/A");
			eBKeyValue->setEnabled(false);
		} else {
//			lprintfln("dispo");
			keyValue->setFontColor(0xC8C8C8);
			eBKeyValue->setText("");
			eBKeyValue->setEnabled(true);
		}
		String urlTmp = HOST;
		urlTmp += "/units/"
				+ Convert::toString(mapInfoIdUnit[selectedInformation])
				+ "/types/";
		urlTmp += _LOGINTOKEN;
		connectUrl(urlTmp, UNITS_TYPES);
	}
}

void AlertTab::parseJSONPostAlert(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONPostAlert");
	String urlTmp = HOST;
	urlTmp += "/alerts/";
	urlTmp += _LOGINTOKEN;
	connectUrl(urlTmp, ALERT_LIST);

	int index = mapSnoozeList.size();
	for (int idx = 0; idx < index; idx++) {
		lVListDest->removeChild(mapLVIListDest[idx]);
		mapLVIListDest[idx]->removeChild(mapLListDestName[idx]);
		delete mapLListDestName[idx];
		delete mapLVIListDest[idx];
	}
	mapLListDestName.clear();
	mapLVIListDest.clear();
	mapAMSId.clear();
	mapMediaValueListId.clear();
	mapSnoozeList.clear();
	selectedAsset = -1;
	selectedPlugin = -1;
	selectedInformation = -1;
	selectedUnit = -1;
	selectedOperator = -1;
	selectedUser = -1;
	selectedMedia = -1;
	selectedMediaValue = -1;
}

void AlertTab::parseJSONAlertInfo(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONAlertInfo");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY == root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
//		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(0);
		MAUtil::YAJLDom::Value* valueTmp1 = root->getValueForKey("alert");

		String asset = valueTmp1->getValueForKey("name")->toString();
		String plugin = asset;
		String information = asset;
		asset.resize(asset.find(" - ",0));
		plugin.remove(0,asset.size()+3);
		plugin.resize(plugin.find(" - ",0));
		information.remove(0,plugin.size()+asset.size()+6);
		lAlertAsset->setText(Convert::tr(alert_label_asset + LANGUAGE) + asset);
		lAlertPlugin->setText(Convert::tr(alert_label_plugin + LANGUAGE) + plugin);
		lAlertInformation->setText(Convert::tr(alert_label_information + LANGUAGE) + information);
//		lAlertName->setText(
//				Convert::tr(alert_create_String_Alert_Name + LANGUAGE)
//						+ valueTmp1->getValueForKey("name")->toString());
		MAUtil::YAJLDom::Value* valueTmp2 = root->getValueForKey("criteria");
		lAlertOperator->setText(
				Convert::tr(alert_create_String_Operator_Name + LANGUAGE)
						+ valueTmp2->getValueForKey("name")->toString());
		MAUtil::YAJLDom::Value* valueTmp3 = root->getValueForKey("alert_value");
		lAlertValue->setText(
				Convert::tr(alert_create_String_Threshold_Value + LANGUAGE)
						+ valueTmp3->getValueForKey("value")->toString());
		MAUtil::YAJLDom::Value* valueTmp4 = root->getValueForKey(
				"information_unit");
		lAlertUnit->setText(
				Convert::tr(alert_create_String_Unit + LANGUAGE)
						+ valueTmp4->getValueForKey("name")->toString());
		lAlertKeyValue->setText(
				Convert::tr(alert_create_String_Key_Value + LANGUAGE)
						+ valueTmp3->getValueForKey("key_value")->toString());

		String urlTmp = HOST;
		urlTmp += "/alerts/" + Convert::toString(mapAlertId[posOptionAlert])
				+ "/recipients/";
		urlTmp += _LOGINTOKEN;
		connectUrl(urlTmp, ALERT_RECIPIENT_LIST);
	}
}

void AlertTab::parseJSONPostMediaSpe(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONPostMediaSpe");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY == root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("ID AMS = %d", root->getValueForKey("id")->toInt());
		mapAMSId[mapAMSId.size()] = root->getValueForKey("id")->toInt();
	}
	currentAMS++;
//		lprintfln("currentAMS = %d", currentAMS);
//lprintfln("mapMediaValueListId.size() = %d", mapMediaValueListId.size());
	if (currentAMS < mapMediaValueListId.size()) {
		String urlTmp = HOST;
		urlTmp += "/medias/specializations/";
		urlTmp += _LOGINTOKEN;
		String message = "                {\"ams_snooze\": ";
		message += Convert::toString(mapSnoozeList[currentAMS]);
//				lprintfln(" mapSnoozeList[currentAMS] = %d",  mapSnoozeList[currentAMS]);
		message += ",\"mev_id\" : ";
		message += Convert::toString(mapMediaValueListId[currentAMS]);
//				lprintfln(" mapMediaValueListId[currentAMS] = %d",  mapMediaValueListId[currentAMS]);
		message += "}";
		connectUrl(urlTmp, POST_MEDIA_SPE, POST, message);
	} else {
		String urlTmp = HOST;
		urlTmp += "/alerts/";
		urlTmp += _LOGINTOKEN;

		String message = "                        {\"name\": \""
				+ mapLAssetName[selectedAsset]/*->getText()*/ + " - "
				+ mapLPluginName[selectedPlugin]->getText() + " - "
				+ mapLInfoName[selectedInformation]->getText()
				+ "\",\
\"alert_value\": \"" + eBValue->getText()
				+ "\",\
\"thread_sleep\": 0,\
\"key_value\": \""
				+ eBKeyValue->getText() + "\",\
\"ast_id\": "
				+ Convert::toString(mapAssetId[selectedAsset])
				+ ",\
\"sea_id\": "
				+ Convert::toString(mapInfoIdSea[selectedInformation])
				+ ",\
\"src_id\": "
				+ Convert::toString(mapInfoIdSrc[selectedInformation])
				+ ",\
\"plg_id\": "
				+ Convert::toString(mapPluginId[selectedPlugin])
				+ ",\
\"inf_value_num\": "
				+ Convert::toString(mapInfoInfValueNum[selectedInformation])
				+ ",\
\"inu_id\": "
				+ Convert::toString(mapInfoIdUnit[selectedInformation])
				+ ",\
\"acr_id\": "
				+ Convert::toString(mapOperatorId[selectedOperator])
				+ ",\
\"ams_id\": [";
		for (int idx = 0; idx < mapAMSId.size(); idx++) {
			if (idx != 0) {
				message += ", ";
			}
			message += Convert::toString(mapAMSId[idx]);
		}
		message += "]\
				 }";
		printf(message.c_str());
		connectUrl(urlTmp, POST_ALERT, POST, message);
	}
}

void AlertTab::parseJSONMediaValue(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONMediaValue");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");

		if (mainLayoutMediaValueChoice == NULL) {
			mainLayoutMediaValueChoice = new Page(Convert::tr(alert_create_Label_media_value_choice + LANGUAGE));

			lVMediaValue = new ListView();
			lVMediaValue->addListViewListener(this);
			mainLayoutMediaValueChoice->addChild(lVMediaValue);
		} else {
			int index = mapLVIMediaValue.size();
			for (int idx = 0; idx < index; idx++) {
				lVMediaValue->removeChild(mapLVIMediaValue[idx]);
				mapLVIMediaValue[idx]->removeChild(mapLMediaValueName[idx]);
				delete mapLMediaValueName[idx];
				delete mapLVIMediaValue[idx];
			}
			mapLMediaValueName.clear();
			mapLVIMediaValue.clear();
		}

		for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
			MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
			mapMediaValueId[idx] = valueTmp->getValueForKey("id")->toInt();
			mapLMediaValueName[idx] = new Label();
			mapLMediaValueName[idx]->setText(
					(valueTmp->getValueForKey("value")->toString().c_str()));
			mapLVIMediaValue[idx] = new ListViewItem();
			mapLVIMediaValue[idx]->addChild(mapLMediaValueName[idx]);
			lVMediaValue->addChild(mapLVIMediaValue[idx]);

		}
	}
	Screen::setMainWidget(mainLayoutMediaValueChoice);
	activeMainLayout = mainLayoutMediaValueChoice;
}

void AlertTab::parseJSONMediaType(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONMediaType");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		if (mainLayoutMediaChoice == NULL) {
			mainLayoutMediaChoice = new Page(Convert::tr(alert_create_Label_media_choice + LANGUAGE));

			lVMedia = new ListView();
			lVMedia->addListViewListener(this);
			mainLayoutMediaChoice->addChild(lVMedia);
		} else {
			int index = mapLVIMedia.size();
			for (int idx = 0; idx < index; idx++) {
				lVMedia->removeChild(mapLVIMedia[idx]);
				mapLVIMedia[idx]->removeChild(mapLMediaName[idx]);
				delete mapLMediaName[idx];
				delete mapLVIMedia[idx];
			}
			mapLMediaName.clear();
			mapLVIMedia.clear();
		}
		for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
			MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
			mapMediaId[idx] = valueTmp->getValueForKey("id")->toInt();
			mapLMediaName[idx] = new Label();
			mapLMediaName[idx]->setText(
					(valueTmp->getValueForKey("name")->toString().c_str()));
			mapLVIMedia[idx] = new ListViewItem();
			mapLVIMedia[idx]->addChild(mapLMediaName[idx]);
			lVMedia->addChild(mapLVIMedia[idx]);

		}
	}

	Screen::setMainWidget(mainLayoutMediaChoice);
	activeMainLayout = mainLayoutMediaChoice;
}

void AlertTab::parseJSONUserForOrganisation(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONSubUnitInfo");

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		if (mainLayoutUserChoice == NULL) {
			mainLayoutUserChoice = new Page(Convert::tr(alert_create_Label_user_choice + LANGUAGE));

			lVUser = new ListView();
			lVUser->addListViewListener(this);
			mainLayoutUserChoice->addChild(lVUser);
		} else {
			int index = mapLVIUser.size();
			for (int idx = 0; idx < index; idx++) {
				lVUser->removeChild(mapLVIUser[idx]);
				mapLVIUser[idx]->removeChild(mapLUserName[idx]);
				delete mapLUserName[idx];
				delete mapLVIUser[idx];
			}
			mapLUserName.clear();
			mapLVIUser.clear();
		}

		for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
			MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
			mapUserId[idx] = valueTmp->getValueForKey("id")->toInt();
			mapLUserName[idx] = new Label();
			String space = " ";
			mapLUserName[idx]->setText(
					(valueTmp->getValueForKey("first_name")->toString() + space
							+ valueTmp->getValueForKey("last_name")->toString()).c_str());
			mapLVIUser[idx] = new ListViewItem();
			mapLVIUser[idx]->addChild(mapLUserName[idx]);
			lVUser->addChild(mapLVIUser[idx]);

		}
	}
	Screen::setMainWidget(mainLayoutUserChoice);
	activeMainLayout = mainLayoutUserChoice;
}

void AlertTab::parseJSONSubUnitInfo(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONSubUnitInfo");

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");

		int index = mapLVIUser.size();

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

void AlertTab::parseJSONUnitInfo(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONUnitInfo");

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY == root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");

		int index = mapLVIUnit.size();
		for (int idx = 0; idx < index; idx++) {
			lVUnit->removeChild(mapLVIUnit[idx]);
			mapLVIUnit[idx]->removeChild(mapLUnitName[idx]);
			delete mapLUnitName[idx];
			delete mapLVIUnit[idx];
		}
		mapLUnitName.clear();
		mapLVIUnit.clear();

		mapLUnitName[0] = new Label();
		mapLUnitName[0]->setText(root->getValueForKey("name")->toString());
		String UnitTmp = Convert::tr(alert_create_String_Unit + LANGUAGE);
		UnitTmp += mapLUnitName[0]->getText();
		unit->setText(UnitTmp);
		nbOfSubUnits = root->getValueForKey("information_sub_units")->toInt();


		String urlTmp = HOST;
		urlTmp += "/plugins/" + Convert::toString(mapPluginId[selectedPlugin])
				+ "/sources/"
				+ Convert::toString(mapInfoIdSrc[selectedInformation])
				+ "/searches/"
				+ Convert::toString(mapInfoIdSea[selectedInformation]);
		urlTmp += _LOGINTOKEN;
		connectUrl(urlTmp, SEARCH_INFO);
	}
}

void AlertTab::parseJSONOperator(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONOperator");

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		if (mainLayoutOperatorChoice == NULL) {
			mainLayoutOperatorChoice = new Page(Convert::tr(alert_create_Label_operator_choice + LANGUAGE));

			lVOperator = new ListView();
			lVOperator->addListViewListener(this);
			mainLayoutOperatorChoice->addChild(lVOperator);
		} else {
			int index = mapLVIOperator.size();
			for (int idx = 0; idx < index; idx++) {
				lVOperator->removeChild(mapLVIOperator[idx]);
				mapLVIOperator[idx]->removeChild(mapLOperatorName[idx]);
				delete mapLOperatorName[idx];
				delete mapLVIOperator[idx];
			}
			mapLOperatorName.clear();
			mapLVIOperator.clear();
		}
	}
	for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
		mapOperatorId[idx] = valueTmp->getValueForKey("id")->toInt();
		mapLOperatorName[idx] = new Label();
		mapLOperatorName[idx]->setText(
				valueTmp->getValueForKey("name")->toString().c_str());
		mapLVIOperator[idx] = new ListViewItem();
		mapLVIOperator[idx]->addChild(mapLOperatorName[idx]);
		lVOperator->addChild(mapLVIOperator[idx]);

	}
	Screen::setMainWidget(mainLayoutOperatorChoice);
	activeMainLayout = mainLayoutOperatorChoice;

}

void AlertTab::parseJSONAsset(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONAsset");

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		if (mainLayoutAssetChoice == NULL) {
			mainLayoutAssetChoice = new Page(Convert::tr(alert_create_Label_asset_choice + LANGUAGE));

			lVAsset = new ListView();
			lVAsset->addListViewListener(this);
			mainLayoutAssetChoice->addChild(lVAsset);
		} else {
			int index = mapLVIAsset.size();
			for (int idx = 0; idx < index; idx++) {
				lVAsset->removeChild(mapLVIAsset[idx]);
//				mapLVIAsset[idx]->removeChild(mapLAssetName[idx]);
//				delete mapLAssetName[idx];
				delete mapLVIAsset[idx];
			}
			mapLAssetName.clear();
			mapLVIAsset.clear();
		}
	}
	for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {

		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);

		mapAssetId[idx] = valueTmp->getValueForKey("id")->toInt();
		mapLAssetName[idx] = valueTmp->getValueForKey("name")->toString().c_str();
		mapLVIAsset[idx] = new ListViewItem();
		mapLVIAsset[idx]->setIcon(ICON_SERVER_SMALL);
		mapLVIAsset[idx]->setText(mapLAssetName[idx]);

		lVAsset->addChild(mapLVIAsset[idx]);

	}
	mainLayoutAssetChoice->fillSpaceVertically();
	Screen::setMainWidget(mainLayoutAssetChoice);
	activeMainLayout = mainLayoutAssetChoice;
}

void AlertTab::parseJSONAlert(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONAlert");
	posOptionAlert = -1;

	for (int idx0 = 0; idx0 < mapAlertId.size(); idx0++) {
//		mTest->removeItem(mapLVIAlert[idx0]);
		lValert->removeChild(mapLVIAlert[idx0]);
	}
	mapAlertId.clear();

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");

	} else {
		lprintfln("Root node is valid :) \n");
	}
	lListNoAlert->setHeight(0);
//	lListTitle->setText(Convert::tr(alert_list_Label_title + LANGUAGE));
	Screen::setMainWidget(mainLayoutAlertChoice);
	activeMainLayout = mainLayoutAlertChoice;
	for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
		MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
		MAUtil::YAJLDom::Value* valueTmp1 = valueTmp->getValueForKey("alert");

		mapAlertId[idx] = valueTmp1->getValueForKey("id")->toInt();
		mapLAlertName[idx] = new Label();
		mapLAlertName[idx]->fillSpaceHorizontally();
		mapLAlertName[idx]->setTextHorizontalAlignment(MAW_ALIGNMENT_LEFT);
		mapLAlertName[idx]->setText(
				valueTmp1->getValueForKey("name")->toString().c_str());

		mapLVIAlert[idx] = new ListViewItem();
		mapLVIAlert[idx]->addChild(mapLAlertName[idx]);

		lValert->addChild(mapLVIAlert[idx]);
//		mTest->addItem(mapLVIAlert[idx]);
	}

}

void AlertTab::parseJSONInformation(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONInformation");

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");

	} else {
		lprintfln("Root node is valid :) \n");
		if (mainLayoutInfoChoice == NULL) {
			mainLayoutInfoChoice = new Page(Convert::tr(alert_create_Label_info_choice + LANGUAGE), "");

			lVInfo = new ListView();
			lVInfo->addListViewListener(this);
			mainLayoutInfoChoice->addChild(lVInfo);
		} else {
			int index = mapLVIInfo.size();
			for (int idx = 0; idx < index; idx++) {
				lVInfo->removeChild(mapLVIInfo[idx]);
				mapLVIInfo[idx]->removeChild(mapLInfoName[idx]);
				delete mapLInfoName[idx];
				delete mapLVIInfo[idx];
			}
			mapLInfoName.clear();
			mapLVIInfo.clear();
		}

		mainLayoutInfoChoice->setMemo(mainLayoutPluginChoice->getMemo() + mapLPluginName[selectedPlugin]->getText() + "/");
//		followAlertInformation->setText(mainLayoutPluginChoice->getMemo() + mapLPluginName[selectedPlugin]->getText() + "/");

		lprintfln("%d\n", root->getNumChildValues());
		int idx1 = 0;
		for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {

			MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);
			if (valueTmp->getValueForKey("display")->toString() == "true") {
				MAUtil::YAJLDom::Value* valueTmp1 = valueTmp->getValueForKey(
						"id");
				mapLInfoName[idx1] = new Label();
				mapLInfoName[idx1]->setText(
						valueTmp->getValueForKey("name")->toString().c_str());
				mapInfoIdSea[idx1] =
						valueTmp1->getValueForKey("search_id")->toInt();
				mapInfoIdSrc[idx1] =
						valueTmp1->getValueForKey("source_id")->toInt();
				mapInfoIdUnit[idx1] =
						valueTmp1->getValueForKey("unit_id")->toInt();
				mapInfoInfValueNum[idx1] = valueTmp1->getValueForKey(
						"sub_search_number")->toInt();
				mapLVIInfo[idx1] = new ListViewItem();
				mapLVIInfo[idx1]->addChild(mapLInfoName[idx1]);
				lVInfo->addChild(mapLVIInfo[idx1]);
				idx1 ++;
			}
		}
		Screen::setMainWidget(mainLayoutInfoChoice);
		activeMainLayout = mainLayoutInfoChoice;
	}
}

void AlertTab::parseJSONPlugin(MAUtil::YAJLDom::Value* root) {

	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");

	} else {
		lprintfln("Root node is valid :) \n");
		if (mainLayoutPluginChoice == NULL) {
			mainLayoutPluginChoice = new Page(Convert::tr(alert_create_Label_plugin_choice + LANGUAGE), "");

			lVPlugin = new ListView();
			lVPlugin->addListViewListener(this);
			mainLayoutPluginChoice->addChild(lVPlugin);
		} else {
			int index = mapLVIPlugin.size();
			for (int idx = 0; idx < index; idx++) {
				lVPlugin->removeChild(mapLVIPlugin[idx]);
				mapLVIPlugin[idx]->removeChild(mapLPluginName[idx]);
				delete mapLPluginName[idx];
				delete mapLVIPlugin[idx];
			}
			mapLPluginName.clear();
			mapLVIPlugin.clear();
		}
		mainLayoutPluginChoice->setMemo( "Alerte : " + mapLAssetName[selectedAsset]/*->getText()*/ + "/");
//		followAlertPlugin->setText( "Alerte : " + mapLAssetName[selectedAsset]/*->getText()*/ + "/");

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
		Screen::setMainWidget(mainLayoutPluginChoice);
		activeMainLayout = mainLayoutPluginChoice;
	}
}

void AlertTab::listViewItemClicked(ListView* listView,
		ListViewItem* listViewItem) {
	lprintfln("Clicked");
	if (listView == lValert) {
		if (posOptionAlert != -1) //savoir si on a ouvert les options (supprimer/annuler/view), d'une alerte dans la View 1
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

				bDelete = new Button();
				bDelete->addButtonListener(this);
				bDelete->setText(
						Convert::tr(
								alert_create_Button_Delete_Alert + LANGUAGE));
				bDelete->fillSpaceHorizontally();
				HLOptionAlert->addChild(bDelete);
				bCancel = new Button();
				bCancel->setText(
						Convert::tr(
								alert_create_Button_Cancel_Alert + LANGUAGE));
//			bCancel->fillSpaceHorizontally();
				bCancel->addButtonListener(this);
				HLOptionAlert->addChild(bCancel);
				bView = new Button();
				bView->addButtonListener(this);
				bView->setText(
						Convert::tr(
								alert_create_Button_Detail_Alert + LANGUAGE));
				bView->fillSpaceHorizontally();
				HLOptionAlert->addChild(bView);
				mapLVIAlert[i]->addChild(HLOptionAlert);
			}
		}
	} else if (listView == lVAsset) {
//		Screen::setMainWidget(activityIndicator);
		for (int i = 0; i < mapLVIAsset.size(); i++) {
			if (mapLVIAsset[i] == listViewItem) {
				selectedAsset = i;
				String urlTmp = HOST;
				urlTmp += "/assets/";
				urlTmp += Convert::toString(mapAssetId[i]);
				urlTmp += "/plugins/";
				urlTmp += _LOGINTOKEN;
//				lprintfln(urlTmp.c_str());
				connectUrl(urlTmp, PLUGIN_LIST);
			}
		}
	} else if (listView == lVPlugin) {
//		Screen::setMainWidget(activityIndicator);
		for (int i = 0; i < mapLVIPlugin.size(); i++) {
			if (mapLVIPlugin[i] == listViewItem) {
				selectedPlugin = i;
				String urlTmp = HOST;
				urlTmp += "/plugins/";
				urlTmp += Convert::toString(mapPluginId[i]);
				urlTmp += "/informations/";
				urlTmp += _LOGINTOKEN;
//				lprintfln(urlTmp.c_str());
				connectUrl(urlTmp, INFORMATION_LIST);
			}
		}
	} else if (listView == lVInfo) {
//		Screen::setMainWidget(activityIndicator);
		for (int idx = 0; idx < mapInfoIdUnit.size(); idx++) {
			if (listViewItem == mapLVIInfo[idx]) {
				selectedInformation = idx;
				mapUnitId[0] = mapInfoIdUnit[idx];
			}
		}
		String urlTmp = HOST;
		urlTmp += "/units/";
		urlTmp += Convert::toString(mapUnitId[0]);
		urlTmp += _LOGINTOKEN;
		connectUrl(urlTmp, UNIT_INFO);
		createOptionPage();

	} else if (listViewItem == mapLVIOption[0]) {
//		Screen::setMainWidget(activityIndicator);
		maWidgetSetProperty(eBKeyValue->getWidgetHandle(),
				MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
		maWidgetSetProperty(eBValue->getWidgetHandle(),
				MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
		String urlTmp = HOST;
		urlTmp += "/criteria/";
		urlTmp += _LOGINTOKEN;
		connectUrl(urlTmp, OPERATOR_LIST);
	} else if (listView == lVOperator) {

		String criterionTmp = Convert::tr(
				alert_create_String_Operator_Name + LANGUAGE);
		for (int idx = 0; idx < mapLVIOperator.size(); idx++) {
			if (mapLVIOperator[idx] == listViewItem) {
				selectedOperator = idx;
				criterionTmp += mapLOperatorName[idx]->getText();
			}
		}
		criterion->setText(criterionTmp);
		Screen::setMainWidget(mainLayoutOptionChoice);
		activeMainLayout = mainLayoutOptionChoice;
	} else if (listViewItem == mapLVIOption[1]) {
//		Screen::setMainWidget(activityIndicator);
		maWidgetSetProperty(eBKeyValue->getWidgetHandle(),
				MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
		maWidgetSetProperty(eBValue->getWidgetHandle(),
				MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
		if (nbOfSubUnits != 0 && mapUnitId.size() == 1) {

			String urlTmp1 = HOST;
			urlTmp1 += "/units/";
			urlTmp1 += Convert::toString(mapUnitId[0]);
			urlTmp1 += "/subunits/";
			urlTmp1 += _LOGINTOKEN;
			connectUrl(urlTmp1, SUB_UNIT_INFO);
		} else if (mapUnitId.size() == 1) {
			createUnitPage();
		} else {
			Screen::setMainWidget(mainLayoutUnitChoice);
			activeMainLayout = mainLayoutUnitChoice;
		}
	} else if (listView == lVUnit) {
		String unitTmp = "operateur : ";
		for (int idx = 0; idx < mapLVIUnit.size(); idx++) {
			if (mapLVIUnit[idx] == listViewItem) {
				selectedUnit = idx;
				unitTmp += mapLUnitName[idx]->getText();
			}
		}
		unit->setText(unitTmp);
		Screen::setMainWidget(mainLayoutOptionChoice);
		activeMainLayout = mainLayoutOptionChoice;
	} else if (listView == lVUser) {
//		Screen::setMainWidget(activityIndicator);
		for (int idx = 0; idx < mapLVIUser.size(); idx++) {
			if (mapLVIUser[idx] == listViewItem) {
				selectedUser = idx;
			}
		}
		String urlTmp1 = HOST;
		urlTmp1 += "/organizations/users/";
		urlTmp1 += Convert::toString(mapUserId[selectedUser]);
		urlTmp1 += "/medias/";
		urlTmp1 += _LOGINTOKEN;
		connectUrl(urlTmp1, MEDIA_TYPE);
	} else if (listView == lVMedia) {
//		Screen::setMainWidget(activityIndicator);
		for (int idx = 0; idx < mapLVIMedia.size(); idx++) {
			if (mapLVIMedia[idx] == listViewItem) {
				selectedMedia = idx;
			}
		}
		String urlTmp1 = HOST;
		urlTmp1 += "/organizations/users/";
		urlTmp1 += Convert::toString(mapUserId[selectedUser]);
		urlTmp1 += "/medias/";
		urlTmp1 += Convert::toString(mapMediaId[selectedMedia]);
		urlTmp1 += _LOGINTOKEN;
		connectUrl(urlTmp1, MEDIA_VALUE_LIST);
	} else if (listView == lVMediaValue) {
		for (int idx = 0; idx < mapLVIMediaValue.size(); idx++) {
			if (mapLVIMediaValue[idx] == listViewItem) {
				selectedMediaValue = idx;
			}
		}
		createSnoozePage();
	}
}

void AlertTab::buttonClicked(Widget* button) {
//	lprintfln("bouton click");
	if (button == bAddAlert) {
//		Screen::setMainWidget(activityIndicator);
		String urlTmp = HOST;
		urlTmp += "/assets/";
		urlTmp += _LOGINTOKEN;
		connectUrl(urlTmp, ASSET_LIST);
	} else if (button == bCancel) {
		mapLVIAlert[posOptionAlert]->removeChild(HLOptionAlert);
		mapLVIAlert[posOptionAlert]->addChild(mapLAlertName[posOptionAlert]);
		posOptionAlert = -1;
	} else if (button == bView) {
//		Screen::setMainWidget(activityIndicator);
		String urlTmp = HOST;
		urlTmp += "/alerts/" + Convert::toString(mapAlertId[posOptionAlert]);
		urlTmp += _LOGINTOKEN;
		connectUrl(urlTmp, ALERT_INFO);
	} else if (button == bDelete) {
		String urlTmp = HOST;
		urlTmp += "/alerts/" + Convert::toString(mapAlertId[posOptionAlert])
				+ _LOGINTOKEN;
		connectUrl(urlTmp, DELETE_ALERT_LIST, DELETE);
	} else if (button == bDest) {
		maWidgetSetProperty(eBKeyValue->getWidgetHandle(),
				MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
		maWidgetSetProperty(eBValue->getWidgetHandle(),
				MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
		if (optionPageValid()) {
//			Screen::setMainWidget(activityIndicator);
			String urlTmp = HOST;
			urlTmp += "/organizations/users/";
			urlTmp += _LOGINTOKEN;
			connectUrl(urlTmp, USER_LIST);
		}
	} else if (button == bAddToList) {
		maWidgetSetProperty(eBSnooze->getWidgetHandle(),
				MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
		if (snoozePageValid()) {
			int index = mapSnoozeList.size();
			mapSnoozeList[index] = Convert::toInt(eBSnooze->getText().c_str());
//			lprintfln("YOOOOO : %d", mapSnoozeList[index]);
			Screen::setMainWidget(mainLayoutListDestChoice);
			//pour que les deux boutons soient de la même taille.
			bCreateAlert->setHeight(bAddOtherDest->getHeight());
			bAddOtherDest->setHeight(bCreateAlert->getHeight());

			activeMainLayout = mainLayoutListDestChoice;
			mapMediaValueListId[index] = mapMediaValueId[selectedMediaValue];
			mapLListDestName[index] = new Label();
			mapLListDestName[index]->setText(
					mapLUserName[selectedUser]->getText());
			mapLVIListDest[index] = new ListViewItem();
			mapLVIListDest[index]->addChild(mapLListDestName[index]);
			lVListDest->addChild(mapLVIListDest[index]);
		}
	} else if (button == bAddOtherDest) {
		activeMainLayout->setHeight(30000);
		Screen::setMainWidget(mainLayoutUserChoice);
		activeMainLayout->fillSpaceVertically();
		activeMainLayout->fillSpaceHorizontally();

		activeMainLayout = mainLayoutUserChoice;
	} else if (button == bCreateAlert) {
//		Screen::setMainWidget(activityIndicator);
		mapAMSId.clear();
		currentAMS = 0;
		String urlTmp = HOST;
		urlTmp += "/medias/specializations/";
		urlTmp += _LOGINTOKEN;
		String message = "             {\"ams_snooze\": ";
		message += Convert::toString(mapSnoozeList[0]);
		message += ",\"mev_id\" : ";
		message += Convert::toString(mapMediaValueListId[0]);
		message += "}";
		connectUrl(urlTmp, POST_MEDIA_SPE, POST, message);
	}
}

void AlertTab::createDestListPage() {
	if(mainLayoutListDestChoice == NULL){
	MAExtent screenSize = maGetScrSize();
	int width = EXTENT_X(screenSize);

	mainLayoutListDestChoice = new Page(Convert::tr(alert_create_Label_list_dest_choice + LANGUAGE), "");

	lVListDest = new ListView();
	lVListDest->addListViewListener(this);
	lVListDest->fillSpaceVertically();
	mainLayoutListDestChoice->addChild(lVListDest);

//	hLButtonListDest = new HorizontalLayout();
	lVIButtonListDest = new ListViewItem();

	bAddOtherDest = new Button();
	bAddOtherDest->setWidth(width / 2);
	bAddOtherDest->setText(
			Convert::tr(alert_create_Button_Add_another_recipient + LANGUAGE));
	bAddOtherDest->addButtonListener(this);
	lVIButtonListDest->addChild(bAddOtherDest);
	bCreateAlert = new Button();
	bCreateAlert->setWidth(width / 2);
	bCreateAlert->setLeftPosition(width / 2);
	bCreateAlert->setText(
			Convert::tr(alert_create_Button_Create_alert + LANGUAGE));
	bCreateAlert->addButtonListener(this);
	lVIButtonListDest->addChild(bCreateAlert);
	lVIButtonListDest->wrapContentVertically();
//	bCreateAlert->fillSpaceVertically();

	mainLayoutListDestChoice->addChild(lVIButtonListDest);
//	bCreateAlert->setHeight(bAddOtherDest->getHeight());
	lprintfln("hauteur bouton");
	lprintfln("%d",bAddOtherDest->getHeight());
	lprintfln("%d",bCreateAlert->getHeight());
	}

//	Screen::setMainWidget(mainLayoutListDestChoice); //DEBUG
}

void AlertTab::createSnoozePage() {
	if(mainLayoutSnoozeChoice == NULL)
	{
		mainLayoutSnoozeChoice = new Page(Convert::tr(alert_create_Label_snooze_choice + LANGUAGE));

	lSnooze = new Label();
	lSnooze->setText(Convert::tr(alert_create_Label_Snooze + LANGUAGE));
	mainLayoutSnoozeChoice->addChild(lSnooze);
	HorizontalLayout* hLSnooze = new HorizontalLayout();
	hLSnooze->fillSpaceVertically();
	eBSnooze = new EditBox();
	eBSnooze->addEditBoxListener(this);
	eBSnooze->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	hLSnooze->addChild(eBSnooze);
	lSecond = new Label();
	lSecond->setText(Convert::tr(alert_create_Label_Seconde + LANGUAGE));
	hLSnooze->addChild(lSecond);
	mainLayoutSnoozeChoice->addChild(hLSnooze);
	bAddToList = new Button();
	bAddToList->fillSpaceHorizontally();
	bAddToList->setText(Convert::tr(alert_create_Button_add_list + LANGUAGE));
	bAddToList->addButtonListener(this);
	mainLayoutSnoozeChoice->addChild(bAddToList);
	}
	eBSnooze->setText("");
	Screen::setMainWidget(mainLayoutSnoozeChoice);
	activeMainLayout = mainLayoutSnoozeChoice;
}

void AlertTab::createUnitPage() {
	if (mainLayoutUnitChoice == NULL) {
		mainLayoutUnitChoice = new Page(Convert::tr(alert_create_Label_unit_choice + LANGUAGE));

		lVUnit = new ListView();
		lVUnit->addListViewListener(this);
		mainLayoutUnitChoice->addChild(lVUnit);
	}
	for (int idx = 0; idx <= mapUnitId.size() - 1; idx++) {
		mapLVIUnit[idx] = new ListViewItem();
		mapLVIUnit[idx]->addChild(mapLUnitName[idx]);
		lVUnit->addChild(mapLVIUnit[idx]);
	}
	Screen::setMainWidget(mainLayoutUnitChoice);
	activeMainLayout = mainLayoutUnitChoice;
}

void AlertTab::createOptionPage() {

//	TODO :GDR On ne peut pas utiliser de ListView avec un editBox dedans sinon le clavier ne pointe pas sur le wiget "editbox" explication bug :
//	http://www.mosync.com/content/bug-editbox-lost-focus-after-virtual-keybords-opens

	if(mainLayoutOptionChoice == NULL)
	{

		mainLayoutOptionChoice = new Page(Convert::tr(alert_create_Label_option_choice + LANGUAGE), "");

		lVOption = new ListView();
		lVOption->addListViewListener(this);
		lVOption->fillSpaceVertically();


		keyValue = new Label();
		keyValue->setText(Convert::tr(alert_create_Label_key_value + LANGUAGE));

		eBKeyValue = new EditBox();
		eBKeyValue->fillSpaceHorizontally();
		eBKeyValue->addEditBoxListener(this);

		HorizontalLayout* hlTmp = new HorizontalLayout();
		hlTmp->addChild(keyValue);
		hlTmp->addChild(eBKeyValue);
		hlTmp->wrapContentVertically();

		lVCriteria = new ListView();
		lVCriteria->wrapContentVertically();
		lVCriteria->addListViewListener(this);

		criterion = new Label();
		criterion->setText(Convert::tr(alert_create_Label_operator + LANGUAGE));

		mapLVIOption[0] = new ListViewItem();
		mapLVIOption[0]->addChild(criterion);

		value = new Label();
		value->setText(Convert::tr(alert_create_Label_value + LANGUAGE));

		eBValue = new EditBox();
		eBValue->fillSpaceHorizontally();
		eBValue->addEditBoxListener(this);

		HorizontalLayout *hLTmp1 = new HorizontalLayout();
		hLTmp1->wrapContentVertically();
		hLTmp1->addChild(value);
		hLTmp1->addChild(eBValue);

		mapLVIOption[1] = new ListViewItem();
		unit = new Label();

		mapLVIOption[1]->addChild(unit);

		ListView* lVUnit = new ListView();
		lVUnit->fillSpaceVertically();
		lVUnit->addListViewListener(this);

		mainLayoutOptionChoice->addChild(hlTmp);
		mainLayoutOptionChoice->setBreakLine(1, 0x333333);
		mainLayoutOptionChoice->addChild(lVCriteria);
		mainLayoutOptionChoice->setBreakLine(1, 0x333333);
		lVCriteria->addChild(mapLVIOption[0]);
		mainLayoutOptionChoice->addChild(hLTmp1);
		mainLayoutOptionChoice->setBreakLine(1, 0x333333);

		mainLayoutOptionChoice->addChild(lVUnit);
		lVUnit->addChild(mapLVIOption[1]);

		bDest = new Button();
		bDest->fillSpaceHorizontally();
		bDest->addButtonListener(this);
		bDest->setText(Convert::tr(alert_create_Button_Add_recipient + LANGUAGE));
		mainLayoutOptionChoice->addChild(bDest);
	}

	mainLayoutOptionChoice->setMemo(mainLayoutInfoChoice->getMemo() + mapLInfoName[selectedInformation]->getText());
	mainLayoutListDestChoice->setMemo(mainLayoutOptionChoice->getMemo());

	criterion->setText(Convert::tr(alert_create_Label_operator + LANGUAGE));
	eBValue->setText("");
	selectedOperator = -1;

}

void AlertTab::createDetailAlertPage() {
	mainLayoutAlertDetailChoice = new Page(Convert::tr(alert_detail_Label_title + LANGUAGE));

	lAlertAsset = new Label();
	mainLayoutAlertDetailChoice->addChild(lAlertAsset);
	lAlertPlugin = new Label();
	mainLayoutAlertDetailChoice->addChild(lAlertPlugin);
	lAlertInformation = new Label();
	mainLayoutAlertDetailChoice->addChild(lAlertInformation);

	lAlertOperator = new Label();
	mainLayoutAlertDetailChoice->addChild(lAlertOperator);
	lAlertValue = new Label();
	mainLayoutAlertDetailChoice->addChild(lAlertValue);
	lAlertUnit = new Label();
	mainLayoutAlertDetailChoice->addChild(lAlertUnit);
	lAlertKeyValue = new Label();
	mainLayoutAlertDetailChoice->addChild(lAlertKeyValue);
	lAlertMedia = new Label();
	mainLayoutAlertDetailChoice->addChild(lAlertMedia);
	lAlertSnooze = new Label();
	mainLayoutAlertDetailChoice->addChild(lAlertSnooze);
	lAlertRecipient = new Label(Convert::tr(alert_create_String_list_media_label + LANGUAGE));
	mainLayoutAlertDetailChoice->addChild(lAlertRecipient);
	lVRecipients = new ListView();
	lVRecipients->fillSpaceVertically();
	mainLayoutAlertDetailChoice->addChild(lVRecipients);

}

void AlertTab::createUI() {
	String urlTmp = HOST;
	urlTmp += "/alerts/";
	urlTmp += _LOGINTOKEN;
	connectUrl(urlTmp, ALERT_LIST);

	mainLayoutAlertChoice = new Page(Convert::tr(alert_list_Label_title + LANGUAGE));

	lListNoAlert = new Label(Convert::tr(alert_list_Label_title_no_alert + LANGUAGE));
		mainLayoutAlertChoice->addChild(lListNoAlert);
		lListNoAlert->setHeight(0);

	Screen::setMainWidget(mainLayoutAlertChoice);
	activeMainLayout = mainLayoutAlertChoice;
	lValert = new ListView();
//	lValert->addChild(mTest);
	lValert->addListViewListener(this);
	lValert->fillSpaceVertically();

	ListViewItem *lviAddAlert = new ListViewItem();
	bAddAlert = new Button();
	bAddAlert->addButtonListener(this);
	bAddAlert->fillSpaceHorizontally();
	bAddAlert->setText(Convert::tr(alert_create_Button_add_alert + LANGUAGE));
	lviAddAlert->addChild(bAddAlert);
	mainLayoutAlertChoice->addChild(lValert);
	mainLayoutAlertChoice->addChild(lviAddAlert);
	createDestListPage();
	createDetailAlertPage();
}

bool AlertTab::optionPageValid() {
	if (eBKeyValue->getText() == "") {
		maMessageBox(Convert::tr(alert_message_box_title_Information +LANGUAGE),Convert::tr(alert_message_box_key_value+LANGUAGE));
		return false;
	} else if (selectedOperator == -1) {
		maMessageBox(Convert::tr(alert_message_box_title_Information +LANGUAGE),Convert::tr(alert_message_box_operator+LANGUAGE));
		return false;
	} else if (eBValue->getText() == "") {
		maMessageBox(Convert::tr(alert_message_box_title_Information +LANGUAGE),Convert::tr(alert_message_box_value+LANGUAGE));
		return false;
	}
	return true;
}

bool AlertTab::snoozePageValid() {
	if (eBSnooze->getText() == "") {
		maMessageBox(Convert::tr(alert_message_box_title_Information +LANGUAGE), Convert::tr(alert_message_box_snooze+LANGUAGE));
		return false;
	} else if (Convert::toInt(eBSnooze->getText().c_str()) < 60) {
		maMessageBox(Convert::tr(alert_message_box_title_Information +LANGUAGE), Convert::tr(alert_message_box_snooze_60+LANGUAGE));
		return false;
	}
	return true;
}

void AlertTab::orientationChange(int screenOrientation) {
	MAExtent screenSize = maGetScrSize();
	int width = EXTENT_X(screenSize);
	int height = EXTENT_Y(screenSize);

	if (screenOrientation == MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) {
//		lprintfln("Orientation paysage");
		Screen::setTitle(Convert::tr(CREATE_ALERT_TAB_EN + LANGUAGE));

		bAddOtherDest->setWidth(height / 2);
		bCreateAlert->setWidth(height / 2);
		bCreateAlert->setLeftPosition(height / 2);
	} else // Portrait
	{
		bAddOtherDest->setWidth(width / 2);
		bCreateAlert->setWidth(width / 2);
		bCreateAlert->setLeftPosition(width / 2);
//		lprintfln("Orientation Portrait");
		Screen::setTitle("");
	}
}
