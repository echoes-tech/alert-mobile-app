/*
 * Page.h
 *
 *  Created on: 24 juin 2013
 *      Author: gdr
 */

#ifndef PAGE_H_
#define PAGE_H_

#include <NativeUI/Widgets.h>

using namespace NativeUI;
using namespace MAUtil;

class Page : public NativeUI::VerticalLayout
{
public:
	/**
	 * Constructor.
	 */
	Page(String title, String Memo = "0");

	void setTitle(String title);

	void setMemo(String Memo);

	String getMemo();

	void setBreakLine(int height = 2, int color = 0xFFFFFF);
	/**
	 * Destructor.
	 */
	virtual ~Page();

private:
	Label* mtitle;
	Label* memo;
};


#endif /* PAGE_H_ */
