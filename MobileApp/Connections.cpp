/*
// * Connections.cpp
// *
// *  Created on: 5 juil. 2013
// *      Author: gdr
// */
//
//#include "Connections.h"
//
//Connections::Connections(){
//
//}
//
// Connections::~Connections(){
//
//	}
//
//	void Connections::connectUrl(String url, /*eAuthenticationTab fct, int verb = GET,*/ String jsonMessage){
////		Screen::setMainWidget(mActivityPage);
////		lprintfln("connectUrl");
////
////		if (mIsConnected == false) {
////			mIsConnected = true;
////			fonction = fct;
////			lprintfln(url.c_str());
////			if (verb == GET) {
////	//			lprintfln("GET");
////				int tmp = this->get(url.c_str());
////				lprintfln("GET send = %d", tmp);
////			} else if (verb == POST) {
////	//			lprintfln("POST");
////				lprintfln(jsonMessage.c_str());
////				int tmp = this->postJsonRequest(url.c_str(), jsonMessage.c_str());
////				//			Vector<String> test; test.clear();
////				//			int tmp = this->postRequest(url.c_str(), test, jsonMessage.c_str(), jsonMessage.size());
////				lprintfln("POST send = %d", tmp);
////			}
////
////		} else {
////			lprintfln("Déjà connecté: %d", fonction);
////		}
//	}
//
//	void Connections::dataDownloaded(MAHandle, int){
////		this->close();
////		lprintfln("dataDownloaded");
////		mIsConnected = false;
////		String sMessage = "";
////		if (result == RES_OK) {
////			connERR = 0;
////			char * jsonData = new char[maGetDataSize(data) + 1];
////			maReadData(data, jsonData, 0, maGetDataSize(data));
////			String jsonTmp = jsonData;
////			Convert::formatJSONBeforeParse(jsonTmp);
////
////			MAUtil::YAJLDom::Value *root = YAJLDom::parse(
////					(const unsigned char*) jsonTmp.c_str(), maGetDataSize(data));
////			switch (fonction) {
////			case PLUGIN_LIST:
////				parseJSONPlugin(root);
////				break;
////			case INFORMATION_LIST:
////				parseJSONInformation(root);
////				break;
////			case ALERT_LIST:
////				parseJSONAlert(root);
////				break;
////			case ASSET_LIST:
////				parseJSONAsset(root);
////				break;
////			case OPERATOR_LIST:
////				parseJSONOperator(root);
////				break;
////			case UNIT_INFO:
////				parseJSONUnitInfo(root);
////				break;
////			case SUB_UNIT_INFO:
////				parseJSONSubUnitInfo(root);
////				break;
////			case USER_LIST:
////				parseJSONUserForOrganisation(root);
////				break;
////			case MEDIA_TYPE:
////				parseJSONMediaType(root);
////				break;
////			case MEDIA_VALUE_LIST:
////				parseJSONMediaValue(root);
////				break;
////			case POST_MEDIA_SPE:
////				parseJSONPostMediaSpe(root);
////				break;
////			case POST_ALERT:
////				parseJSONPostAlert(root);
////				break;
////			case SEARCH_INFO:
////				parseJSONSearchInfo(root);
////				break;
////			case ALERT_INFO:
////				parseJSONAlertInfo(root);
////				break;
////			case ALERT_RECIPIENT_LIST:
////				parseJSONAlertRecipientList(root);
////				break;
////			case UNITS_TYPES:
////				parseJSONUnitsTypes(root);
////			default:
////				break;
////
////				delete jsonData;
////				delete root;
////			}
////		} else if (result == CONNERR_DNS) {
////			connERR++;
////			lprintfln("AlertTab DataDownload result = %d", result);
////			lprintfln("DNS resolution error.");
////		} else if (result == 404 && fonction == ALERT_LIST) {
////	//		lListNoAlert->setText(Convert::tr(alert_list_Label_title_no_alert + LANGUAGE));
////			lListNoAlert->setHeight(EXTENT_Y(maGetTextSize(""))*4);
////	//		lListTitle->setText(
////	//				Convert::tr(alert_list_Label_title_no_alert + LANGUAGE));
////		}  else if (result == 503 && fonction == POST_ALERT) {
////			maMessageBox(Convert::tr(alert_message_box_title_Error + LANGUAGE), Convert::tr(alert_message_box_create_alert_error + LANGUAGE));
////
////			activeMainLayout = mainLayoutAlertChoice;
////		} else if (result != RES_OK && (fonction == POST_ALERT || fonction == ALERT_LIST) ){
////			String urlTmp = HOST;
////			urlTmp += "/alerts/";
////			urlTmp += _LOGINTOKEN;
////			connectUrl(urlTmp, ALERT_LIST);
////		}
////		else if (result == 404 && fonction == MEDIA_TYPE){
////			maToast(Convert::tr(alert_toast_no_media_found + LANGUAGE),MA_TOAST_DURATION_SHORT);
////		}else {
////			connERR++;
////			lprintfln("AlertTab DataDownload result = %d", result);
////		}
////		if (connERR >= 3) {
////			if (result != 404) {
////				sMessage = "Connection Error. ERREUR :";
////				sMessage += Convert::toString(result);
////				maMessageBox("Connection Error", sMessage.c_str());
////			}
////			getSystemConnection();
////		}
////		if (result != RES_OK) {
////			activeMainLayout->setHeight(30000);
////			Screen::setMainWidget(activeMainLayout);
////			activeMainLayout->fillSpaceVertically();
////			activeMainLayout->fillSpaceHorizontally();
////		}
//	}
