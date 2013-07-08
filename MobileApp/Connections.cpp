/*
 * Connections.cpp
 *
 *  Created on: 5 juil. 2013
 *      Author: gdr
 */

#include "Connections.h"

Connections::Connections(){
	mIsConnected = false;
}

 Connections::~Connections(){

	}

	void Connections::connectUrl(String url, eFonction fct, int verb, String jsonMessage){
		lprintfln("connectUrl");
		connectUrl1();

		if (mIsConnected == false) {
			mIsConnected = true;
			fonction = fct;
			lprintfln(url.c_str());
			if (verb == GET) {

				int tmp = this->get(url.c_str());
				lprintfln("GET send = %d", tmp);
			} else if (verb == POST) {
				lprintfln(jsonMessage.c_str());
				int tmp = this->postJsonRequest(url.c_str(), jsonMessage.c_str());
				lprintfln("POST send = %d", tmp);
			} else if (verb == DELETE) {
				int tmp = this->deleteJsonRequest(url.c_str());
				lprintfln("DELETE send = %d", tmp);
			}

		} else {
			lprintfln("Déjà connecté: %d", fonction);
		}
	}

	void Connections::dataDownloaded(MAHandle data, int result){

		this->close();
		lprintfln("dataDownloaded");
		mIsConnected = false;
		String sMessage = "";
		MAUtil::YAJLDom::Value *root;
		if (result == RES_OK) {
			connERR = 0;
			char * jsonData = new char[maGetDataSize(data) + 1];
			maReadData(data, jsonData, 0, maGetDataSize(data));
			String jsonTmp = jsonData;
			Convert::formatJSONBeforeParse(jsonTmp);

			root = YAJLDom::parse(
					(const unsigned char*) jsonTmp.c_str(), maGetDataSize(data));
			delete jsonData;
		}
		else if (result == CONNERR_GENERIC&& fonction == AUTHENTICATION_VALIDATION) { //si AUTHENTICATION_VALIDATION renvoie CONNERR_GENERIC c'est qu'il y a un probléme dans identification du user il devra donc remettre ses credentials.
			connERR = 0;
		}
		else if (result < 0)
		{
			connERR++;
		}
		if (connERR >= 3) {
		getSystemConnection();
		String sMessage = "Connection ERROR. ERREUR:";
		sMessage += Convert::toString(result);
		maMessageBox("Connection Error", sMessage.c_str());
		}
		dataDownload1(root, result, fonction);
	}

	// le deletejsonrequest n'est pas implementé dans HighLevelHttpConnection je l'ai donc fait ici
	// il faudra faire de même si on a besoin du PUT par exemple
	//HELP fichier cpp de HighLevelHttpConnection -> https://github.com/MoSync/MoSync/blob/master/libs/Wormhole/HighLevelHttpConnection.cpp
	int Connections::deleteJsonRequest(const char* url)
	{
		lprintfln("test 1");
	int result = create(url, HTTP_DELETE);
	lprintfln("test 2");
	if (result < 0)
	{
		lprintfln("test 3");
	return WORMHOLE_HTTP_ERROR;

	}
	finish();
	return WORMHOLE_HTTP_SUCCESS;
	}
