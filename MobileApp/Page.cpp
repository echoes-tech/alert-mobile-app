/*
 * Page.cpp
 *
 *  Created on: 24 juin 2013
 *      Author: gdr
 */

#include "Util.h"
#include "Page.h"
#include "resource/Convert.h"

Page::Page(String title, String Memo) {
	this->fillSpaceHorizontally();
	this->fillSpaceVertically();
	if (Memo != "0") {
		memo = new Label(Memo);
		memo->setFont(FONT_STYLE_ITALIC);
		memo->setFontColor(0x68CCEF);
		this->addChild(memo);
	}
	mtitle = new Label(title);

//		mtitle->setFontSize(EXTENT_Y(maGetTextSize(""))*2);

	mtitle->fillSpaceHorizontally();
	this->addChild(mtitle);

	Label* line1 = new Label();
	line1->setHeight(2);
	line1->fillSpaceHorizontally();
	if (getPlatform() != IOS) {
		mtitle->setFontColor(0xFFFFFF);
		line1->setBackgroundColor(0xFFFFFF);
	} else {
		mtitle->setFontColor(0x000000);
		line1->setBackgroundColor(0x000000);
	}

	this->addChild(line1);
}

void Page::setTitle(String title) {
	mtitle->setText(title);
}

void Page::setMemo(String Memo) {
	memo->setText(Memo);
}

String Page::getMemo() {
	return memo->getText();
}

void Page::setBreakLine(int height, int color) {
	Label* line1 = new Label();
	line1->setHeight(height);
	line1->fillSpaceHorizontally();
	line1->setBackgroundColor(color);
	this->addChild(line1);

}

Page::~Page() {
}
