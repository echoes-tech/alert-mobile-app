/*
 * ActivityPage.h
 *
 *  Created on: 21 juin 2013
 *      Author: gdr
 */

#ifndef ACTIVITYPAGE_H_
#define ACTIVITYPAGE_H_

#include <NativeUI/Widgets.h>

using namespace NativeUI;
class ActivityPage : public NativeUI::VerticalLayout
{
public:
	/**
	 * Constructor.
	 */
	ActivityPage();

	/**
	 * Destructor.
	 */
	virtual ~ActivityPage();

private:
	ActivityIndicator *mActivityIndicator;
};

#endif /* ACTIVITYPAGE_H_ */
