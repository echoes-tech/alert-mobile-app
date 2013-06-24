/*
 * Page.cpp
 *
 *  Created on: 24 juin 2013
 *      Author: gdr
 */

#include "Page.h"
#include "resource/Convert.h"

Page::Page(String title, String Memo)
	{
		this->fillSpaceHorizontally();
		this->fillSpaceVertically();
		if(Memo != "0")
		{
					memo = new Label(Memo);
					memo->setFont(FONT_STYLE_ITALIC);
					memo->setFontColor(0x68CCEF);
		//			followAlertPlugin->setFontSize(13);
					this->addChild(memo);
		}
		mtitle = new Label(title);
//		mtitle->setFont(FONT_STYLE_NORMAL);
		mtitle->setFontColor(0xFFFFFF);
		mtitle->fillSpaceHorizontally();
		this->addChild(mtitle);

		Label* line1 = new Label();
		line1->setHeight(2);
		line1->fillSpaceHorizontally();
		line1->setBackgroundColor(0xFFFFFF);
		this->addChild(line1);
	}

void Page::setTitle(String title)
{
	mtitle->setText(title);
}

void Page::setMemo(String Memo)
{
	memo->setText(Memo);
}

String Page::getMemo()
{
	return memo->getText();
}

void Page::setBreakLine()
{
	Label* line1 = new Label();
	line1->setHeight(2);
	line1->fillSpaceHorizontally();
	line1->setBackgroundColor(0xFFFFFF);
	this->addChild(line1);

}

Page::~Page(){}
