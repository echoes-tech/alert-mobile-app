
#ifndef SCREEN_MAIN_H_
#define SCREEN_MAIN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include "TrackingTab.h"
#include "HomeTab.h"
#include "OptionTab.h"
#include "AlertTab.h"

using namespace NativeUI;

/**
 * This is the main screen of the application.
 */
class ScreenMain : public TabScreen, public CustomEventListener
{
public:
	/**
	 * Constructor.
	 */
	ScreenMain();

	/**
	 * Destructor.
	 */
	virtual ~ScreenMain();

	void createUI(String loginToken, long long idMobile);

    void pullRequest();

	/**
	 * Handle keypresses.
	 */
	virtual void handleKeyPress(int keyCode);

	/**
	 * Handle pointer presses.
	 */
	virtual void handlePointerPressed(MAPoint2d point);

	/**
	 * Handle pointer moves.
	 */
	virtual void handlePointerMoved(MAPoint2d point);

	/**
	 * Handle pointer releases.
	 */
	virtual void handlePointerReleased(MAPoint2d point);

	void customEvent(const MAEvent& event);
//	void drawChangeVerticalHorizontal(int width, int height);

	virtual void handleWidgetEvent(
				Widget* widget,
				MAWidgetEventData* widgetEventData){lprintfln("EVENT LISTENER");}


	private:
	int LANGUAGE;

	bool uiCreated;
	bool landscape;

	AlertTab* alertTab;
	TrackingTab* trackingTab;
	HomeTab* homeTab;
	OptionTab* optionTab;

};

#endif
