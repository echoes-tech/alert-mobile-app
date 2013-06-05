/*
 * Authentication.cpp
 *
 *  Created on: 3 juin 2013
 *      Author: gdr
 */

#include "Authentication.h"
#include "MAHeaders.h"


Authentication::Authentication(int language, ScreenMain* mScreenMain) :
		Screen(), LANGUAGE(language) {
//	authenticationAccepted = false;
	mIsConnected = false;
	_modeAuth = "credential";
	_idMobile = 0;
	_tokenConnection = "";
	_tokenMobile = "";
	_login = "";

	String config;
	eFile eFileTmp = tryToRead(config);
	if(eFileTmp == FILE_NOT_EXIST)
	{
		createUI();
	}
	else if(eFileTmp == FILE_OPEN_ERROR)
	{
		maPanic(1, "ERROR FILE STRORAGE");
	}
	else
	{
		Convert::formatJSONBeforeParse(config);
		MAUtil::YAJLDom::Value* root = YAJLDom::parse((const unsigned char*) config.c_str(), config.size());
		_modeAuth = root->getValueForKey("authentication_mode")->toString();
		_idMobile = root->getValueForKey("id_media_value")->toInt();
		_tokenConnection = root->getValueForKey("token_authent")->toString();
		_tokenMobile = root->getValueForKey("token_mobile")->toString();
		_login = root->getValueForKey("login")->toString();

		if(_modeAuth == "credential")
		{
			createUI();
		}
		else if (_modeAuth == "none")
		{
//			TODO : va voir toute les appmobile du compte, il faudrait utilisé get /medias/3/media_values/id (pas encore creer dans l'API) AUTHENTICATION_VALIDATION
			_LOGINTOKEN = "?login=" + _login + "&token=" + _tokenConnection;
			String urlTmp = HOST;
			urlTmp += "/medias/3/";
			urlTmp += _LOGINTOKEN;
			connectUrl(urlTmp, AUTHENTICATION_VALIDATION);
		}
		else
		{
			createUI();
		}
//		lprintfln(_tokenMobile.c_str());
//		lprintfln(_modeAuth.c_str());
//		lprintfln("%d", _idMobile);
//		lprintfln(_tokenConnection.c_str());
//		lprintfln(_login.c_str());


	}
	screenMain = mScreenMain;

}

/**
 * Destructor.
 */
Authentication::~Authentication() {

}

void Authentication::dataDownloaded(MAHandle data, int result) {
	lprintfln("dataDownloaded Authentication");
	mIsConnected = false;
	this->close();
	String sMessage = "";

	if (result == RES_OK) {
		connERR = 0;
		char * jsonData = new char[maGetDataSize(data) + 1];
		maReadData(data, jsonData, 0, maGetDataSize(data));
		String jsonTmp = jsonData;
		Convert::formatJSONBeforeParse(jsonTmp);
		lprintfln("début parse");
		MAUtil::YAJLDom::Value* root = YAJLDom::parse(
				(const unsigned char*) jsonTmp.c_str(), maGetDataSize(data));
		lprintfln("fin parse");
		switch (fonction) {
		case USER_TOKEN:
			parseJSONUserToken(root);
			break;
		case MEDIAS_LIST:
			parseJSONMediasList(root);
			break;
		case POST_MEDIA_VALUE:
			parseJSONPostMediaValue(root);
			break;
		case POST_MEDIA_VALUE_VALIDATION:
			parseJSONPostMediaValueValidation(root);
			break;
		case AUTHENTICATION_VALIDATION:
			parseJSONAuthenticationValidation(root);
		default:
			break;
		}
	} else if (result == CONNERR_DNS) {
		connERR++;
		lprintfln("AlertTab DataDownload result = %d", result);
		lprintfln("DNS resolution error.");
	} else if (result == CONNERR_GENERIC && fonction == USER_TOKEN){
		presentation->setText(
				"erreur d'authentification verifier votre login et password");
	} else if (result == 404 && fonction == MEDIAS_LIST) {
		createPageMobileChoice();
	} else if (result == 404 && fonction == AUTHENTICATION_VALIDATION) { //si AUTHENTICATION_VALIDATION renvoie 404 c'est qu'il y a un probléme dans identification du mobile il faut donc la refaire.
		_idMobile = 0;
		_tokenMobile = "";
		String urlTmp = HOST;
		urlTmp += "/medias/3";
		urlTmp += _LOGINTOKEN ;
		connectUrl(urlTmp, MEDIAS_LIST);

	}else if (result == CONNERR_GENERIC && fonction == AUTHENTICATION_VALIDATION) {//si AUTHENTICATION_VALIDATION renvoie CONNERR_GENERIC c'est qu'il y a un probléme dans identification du user il devra donc remettre ses credential.
		_tokenConnection = "";
		_login = "";
		createUI();
	}

	else {
		connERR++;
		lprintfln("AlertTab DataDownload result = %d", result);
	}
	if (connERR >= 3) {
		sMessage = "Connection Error. ERREUR :";
		sMessage += Convert::toString(result);
		maMessageBox("Connection Error", sMessage.c_str());
		getSystemConnection();
	}

}

void Authentication::parseJSONAuthenticationValidation(MAUtil::YAJLDom::Value* root)
{
	lprintfln("parseJSONAuthenticationValidation");
		if (NULL == root || YAJLDom::Value::NUL == root->getType()
				|| YAJLDom::Value::ARRAY != root->getType()) {
			lprintfln("Root node is not valid\n");
		} else {
			lprintfln("Root node is valid :) \n");
			bool isAuth = false;
			for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
				MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);

				if(valueTmp->getValueForKey("id")->toInt() == _idMobile && valueTmp->getValueForKey("token")->toString() == _tokenMobile){
					isAuth = true;
				}
			}
			if(isAuth)
			{
				authenticationAccepted();
			}
			else
			{
				_idMobile = 0;
				_tokenMobile = "";
				String urlTmp = HOST;
				urlTmp += "/medias/3";
				urlTmp += _LOGINTOKEN ;
				connectUrl(urlTmp, MEDIAS_LIST);
			}
		}
}

void Authentication::parseJSONPostMediaValueValidation(
		MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONPostMediaValueValidation");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY == root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");

		if (root->getValueForKey("is_confirmed")->toString() == "true") {
			tryToWrite(_login, _tokenMobile, _tokenConnection, _modeAuth , _idMobile);
			createPageAuthenticationMode();
		}
	}
}

void Authentication::parseJSONPostMediaValue(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONPostMediaValue");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY == root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");

		_tokenMobile = root->getValueForKey("token")->toString();
		_idMobile = root->getValueForKey("id")->toInt();
	}
	String urlTmp = HOST;
		urlTmp += "/medias/3/media_values/" + Convert::toString(_idMobile) + "/validate/";
		urlTmp += _LOGINTOKEN;
		String message = "{\"mev_validation\": true ,\"mev_token\" : \"" + _tokenMobile + "\"}";
		connectUrl(urlTmp, POST_MEDIA_VALUE_VALIDATION, POST, message);
}

void Authentication::parseJSONMediasList(MAUtil::YAJLDom::Value* root) {
	lprintfln("parseJSONMediasList");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY != root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
		for (int idx = 0; idx <= root->getNumChildValues() - 1; idx++) {
			MAUtil::YAJLDom::Value* valueTmp = root->getValueByIndex(idx);

			mapMediaID[idx] = valueTmp->getValueForKey("id")->toInt();
			mapMediaName[idx] = new Label(valueTmp->getValueForKey("value")->toString());
			mapMediaToken[idx] = valueTmp->getValueForKey("token")->toString();
		}
		createPageMobileChoice();
	}
}


void Authentication::parseJSONUserToken(MAUtil::YAJLDom::Value* root)
 {
	lprintfln("parseJSONUserToken");
	if (NULL == root || YAJLDom::Value::NUL == root->getType()
			|| YAJLDom::Value::ARRAY == root->getType()) {
		lprintfln("Root node is not valid\n");
	} else {
		lprintfln("Root node is valid :) \n");
			_tokenConnection = root->getValueForKey("token")->toString();
			lprintfln(_tokenConnection.c_str());
			_LOGINTOKEN = "?login=" + _login + "&token=" + _tokenConnection;
			tryToWrite(_login, _tokenMobile, _tokenConnection, _modeAuth , _idMobile);
			if(_tokenMobile == "")
			{
				String urlTmp = HOST;
				urlTmp += "/medias/3";
				urlTmp += _LOGINTOKEN ;
				connectUrl(urlTmp, MEDIAS_LIST);
			}
			else
			{
				String urlTmp = HOST;
				urlTmp += "/medias/3";
				urlTmp += _LOGINTOKEN ;
				connectUrl(urlTmp, AUTHENTICATION_VALIDATION);
			}

	}
}

void Authentication::authenticationAccepted() {
	screenMain->createUI(_LOGINTOKEN);
}

void Authentication::connectUrl(String url, eAuthenticationTab fct, int verb,
	String jsonMessage) {
lprintfln("connectUrl");
lprintfln(url.c_str());

if (mIsConnected == false) {
	mIsConnected = true;
	fonction = fct;
	lprintfln(url.c_str());
	if (verb == GET) {
		lprintfln("GET");
		int tmp = this->get(url.c_str());
		lprintfln("GET send = %d", tmp);
	} else if (verb == POST) {
		lprintfln("POST");
		lprintfln(jsonMessage.c_str());
		int tmp = this->postJsonRequest(url.c_str(), jsonMessage.c_str());
		lprintfln("POST send = %d", tmp);
	}
} else {
	lprintfln("Déjà connecté: %d", fonction);
}
}

void Authentication::createPageMobileChoice() {
	vLMediaChoice = new VerticalLayout();
	vLMediaChoice->fillSpaceHorizontally();
	vLMediaChoice->fillSpaceVertically();

	newMediaTitle = new Label("Nouveau terminal ?");
	newMediaTitle->setHeight(100);
	newMediaTitle->fillSpaceHorizontally();
	vLMediaChoice->addChild(newMediaTitle);

	hlNewMedia = new HorizontalLayout();
	hlNewMedia->setHeight(70);
	vLMediaChoice->addChild(hlNewMedia);
	lNewMediaName = new Label("Name : ");
	hlNewMedia->addChild(lNewMediaName);
	ebNewMediaName = new EditBox();
	ebNewMediaName->fillSpaceHorizontally();
	hlNewMedia->addChild(ebNewMediaName);
	bAddNewMedia = new Button();
	bAddNewMedia->addButtonListener(this);
	bAddNewMedia->setText("send");
	hlNewMedia->addChild(bAddNewMedia);


	if (mapMediaID.size() != 0) {
		Label* oldMediaTitle = new Label(
				"Utiliser un terminal déjà existant ?");
		oldMediaTitle->fillSpaceHorizontally();
		oldMediaTitle->setHeight(100);
		vLMediaChoice->addChild(oldMediaTitle);
		lVMedia = new ListView();
		lVMedia->fillSpaceVertically();
		lVMedia->addListViewListener(this);
		vLMediaChoice->addChild(lVMedia);
		for(int idx = 0; idx < mapMediaID.size(); idx++)
		{
			mapLVIMedia[idx] = new ListViewItem();
			mapLVIMedia[idx]->addChild(mapMediaName[idx]);
			lVMedia->addChild(mapLVIMedia[idx]);

		}
	}
	Screen::setMainWidget(vLMediaChoice);

}

void Authentication::createPageAuthenticationMode()
{
	vLAuthenticationModeChoice = new VerticalLayout();
	vLAuthenticationModeChoice->fillSpaceHorizontally();
	vLAuthenticationModeChoice->fillSpaceVertically();

	authenticationModeTitle = new Label("Quel mode d'authentification voulez vous choisr pour cette application ?");
	authenticationModeTitle->fillSpaceHorizontally();
	vLAuthenticationModeChoice->addChild(authenticationModeTitle);

	lVAuthenticationMode = new ListView() ;
	lVAuthenticationMode->fillSpaceHorizontally();
	lVAuthenticationMode->addListViewListener(this);
	vLAuthenticationModeChoice->addChild(lVAuthenticationMode);

	lVIModeCredential = new ListViewItem();
	lModeCredential = new Label("Avec les identifiants habituelle");
	lVIModeCredential->addChild(lModeCredential);
	lVAuthenticationMode->addChild(lVIModeCredential);

	lVIModeNone = new ListViewItem();
	lModeNone = new Label("Aucune authentification");
	lVIModeNone->addChild(lModeNone);
	lVAuthenticationMode->addChild(lVIModeNone);
	Screen::setMainWidget(vLAuthenticationModeChoice);
}

void Authentication::createUI() {
	vLAuthentication = new VerticalLayout();
	vLAuthentication->fillSpaceHorizontally();
	vLAuthentication->fillSpaceVertically();
	icon = new Image();

	icon->setImage(LOGO);
	icon->fillSpaceHorizontally();

	vLAuthentication->addChild(icon);
	presentation = new Label("AUTHENTIFICATION");
	presentation->setHeight(100);
	presentation->fillSpaceHorizontally();

	vLAuthentication->addChild(presentation);
	login = new Label("Login : ");

	eLogin = new EditBox();
	eLogin->fillSpaceHorizontally();
	hLLogin = new HorizontalLayout();
	hLLogin->setHeight(70);
	hLLogin->addChild(login);
	hLLogin->addChild(eLogin);
	vLAuthentication->addChild(hLLogin);

	password = new Label("Password : ");
	ePassword = new EditBox();
	ePassword->setEditMode(EDIT_BOX_MODE_PASSWORD);
	ePassword->fillSpaceHorizontally();
	hLPassword = new HorizontalLayout();
	hLPassword->setHeight(70);
	hLPassword->addChild(password);
	hLPassword->addChild(ePassword);
	hLPassword->fillSpaceVertically();
	vLAuthentication->addChild(hLPassword);

	bValidate = new Button();
	bValidate->setText("Send");
	bValidate->addButtonListener(this);
	bValidate->fillSpaceHorizontally();
	vLAuthentication->addChild(bValidate);
	Screen::setMainWidget(vLAuthentication);
}

void Authentication::buttonClicked(Widget* button) {
	lprintfln("bouton click");
	if (button == bValidate) {
		_login = eLogin->getText();

		String urlTmp = HOST;
		urlTmp += "/users/";
		urlTmp += "?login=" + _login + "&password=" + ePassword->getText();
		connectUrl(urlTmp, USER_TOKEN);
		maWidgetSetProperty(eLogin->getWidgetHandle(),
				MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
		maWidgetSetProperty(ePassword->getWidgetHandle(),
				MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
	} else if (button == bAddNewMedia) {
		String urlTmp = HOST;
		urlTmp += "/medias/";
		urlTmp += _LOGINTOKEN;
		String message = "{\"med_id\": 3";
		message += ",\"mev_value\" : \"" + ebNewMediaName->getText() + "\"}";
		connectUrl(urlTmp, POST_MEDIA_VALUE, POST, message);
	}

}

void Authentication::listViewItemClicked(ListView* listView,
	ListViewItem* listViewItem) {
lprintfln("Clicked");
if (listView == lVAuthenticationMode) {
	if(listViewItem == lVIModeNone)
	{
		_modeAuth = "none";
	}
	else{
		_modeAuth = "credential";
	}
	tryToWrite(_login, _tokenMobile, _tokenConnection, _modeAuth , _idMobile);
	authenticationAccepted();
}
}
