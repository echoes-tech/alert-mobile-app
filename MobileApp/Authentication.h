/*
 * Authentication.h
 *
 *  Created on: 3 juin 2013
 *      Author: gdr
 */

#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

#include <NativeUI/Widgets.h>// Include all widgets
//#include <NativeUI/Screen.h>
//#include <Wormhole/HighLevelHttpConnection.h>
//#include <Wormhole/MessageStreamJSON.h>
//#include <mastdlib.h>

#include "resource/Convert.h"
#include "MAHeaders.h"
#include "Util.h"
#include "ScreenMain.h"
#include "Connections.h"

#include "ActivityPage.h"

//using namespace Wormhole;
//using namespace MAUtil;
using namespace NativeUI;


class Authentication : public NativeUI::Screen/*, public HighLevelHttpConnection*/, public ButtonListener, public ListViewListener, public RadioGroupListener, public EditBoxListener, public ScreenListener, public Connections
{
public:

//	enum eAuthenticationTab {
//		USER_TOKEN = 0,
//		MEDIAS_LIST,
//		POST_MEDIA_VALUE,
//		POST_MEDIA_VALUE_VALIDATION,
//		AUTHENTICATION_VALIDATION
//			};

	/**
	 * Constructor.
	 */
	Authentication(int language, ScreenMain* );

	/**
	 * Destructor.
	 *
	 */
	virtual ~Authentication();

//	void connectUrl(String url, eAuthenticationTab fct, int verb = GET, String jsonMessage = "");
//	void dataDownloaded(MAHandle, int);
	virtual void connectUrl1();
	virtual void dataDownload1(MAUtil::YAJLDom::Value*, int result, eFonction fonction);


	virtual void listViewItemClicked(ListView* listView, ListViewItem* listViewItem);
	virtual void buttonClicked(Widget* button);
	virtual void radioButtonSelected(NativeUI::RadioGroup*, int, NativeUI::RadioButton*);

	 virtual void editBoxEditingDidEnd(EditBox* editBox) {}
	 virtual void editBoxReturn(EditBox* editBox){maWidgetSetProperty(editBox->getWidgetHandle(), MAW_EDIT_BOX_SHOW_KEYBOARD, "false");}
	 virtual void editBoxEditingDidBegin(EditBox* editBox) {maWidgetSetProperty(editBox->getWidgetHandle(), MAW_EDIT_BOX_SHOW_KEYBOARD, "true");}

	 virtual void orientationChanged(Screen* screen, int screenOrientation);

	void parseJSONUserToken(MAUtil::YAJLDom::Value* root);
	void parseJSONMediasList(MAUtil::YAJLDom::Value* root);
	void parseJSONPostMediaValue(MAUtil::YAJLDom::Value* root);
	void parseJSONPostMediaValueValidation(MAUtil::YAJLDom::Value* root);
	void parseJSONAuthenticationValidation(MAUtil::YAJLDom::Value* root);

	void createUI();
	void createPageMobileChoice();
	void authenticationAccepted();

	bool newMediaNameValid();

private:
	ActivityPage* mActivityPage;
//	ActivityIndicator* activityIndicator;
	int LANGUAGE;
//	eAuthenticationTab fonction;
//	bool mIsConnected;
//	int connERR;

	bool _vibrate;
	bool _notification;
	String _LOGINTOKEN;
	String _login;
	String _tokenConnection;
	String _tokenMobile;
	long long _idMobile;
	String _modeAuth;

	ScreenMain* screenMain;
//	View 1
	VerticalLayout *vLAuthentication;
	HorizontalLayout *hLLogin;
	HorizontalLayout *hLPassword;
	Image* icon;
	Label *presentation;
	Label *login;
	Label *password;
	EditBox *eLogin;
	EditBox *ePassword;
	Button *bValidate;

	Label* lAuthenticationMode;
	RadioGroup* rGAuthenticationChoice;
	RadioButton* rBModeCredential;
	RadioButton* rBModeNone;


//	View 2
	VerticalLayout *vLMediaChoice;
	ListView* lVMedia;
	MAUtil::Map<int, ListViewItem*> mapLVIMedia;
	MAUtil::Map<int, long long> mapMediaID;
	MAUtil::Map<int, String> mapMediaToken;
	MAUtil::Map<int, Label*> mapMediaName;
	Label* newMediaTitle;
	HorizontalLayout* hlNewMedia;
	Label* lNewMediaName;
	EditBox* ebNewMediaName;
	Button* bAddNewMedia;

};



#endif /* AUTHENTICATION_H_ */
