#include <ma.h> 				// Syscalls
#include <MAUtil/String.h>		// C++ String class
#include <MAUtil/Moblet.h>		// Moblet class
#include <conprint.h>			// lprintfln for logging
#include <NativeUI/Widgets.h>// Include all widgets
#include "ScreenMain.h"			// Main UI screen
#include "Util.h"
#include "Authentication.h"


// How often the timer will be invoked
#define TIMER_PERIOD 60000

using namespace MAUtil;
using namespace NativeUI;

/**
 * Moblet for the  application.
 */
class NativeUIMoblet: public Moblet, public FocusListener, public TimerListener{
public:
	/**
	 * Constructor that creates the UI.
	 */
	NativeUIMoblet() {
		// Create the main user interface screen.
		getSystemConnection();

		//	maScreenSetOrientation(MA_SCREEN_ORIENTATION_DYNAMIC);
//maScreenSetOrientation(MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN);
//			// iOS and Windows Phone.
//			maScreenSetSupportedOrientations(
//					MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT
//							| MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT
//							| MA_SCREEN_ORIENTATION_PORTRAIT
//							| MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN);

		mMainScreen = new ScreenMain();
		addTimer(this, TIMER_PERIOD, 0);
		addFocusListener(this);

		// Update the display.
//		drawScreen();
		Authentication* mAuthentication = new Authentication(getSystemLanguage(), mMainScreen);
//		mAuthentication->show();
	}

	// send timer event to trackingTabObject
	void runTimerEvent() {
		maReportResourceInformation();
		mMainScreen->pullRequest();
//		maToast("timer", MA_TOAST_DURATION_LONG); // TODO a supprimer juste pour test
	}

	/**
	 * Method for handling events that do not have predefined methods.
	 */
	void customEvent(const MAEvent& event) {

//		lprintfln("test event %d",event.type);

		// If the event type is screen changed we update the display.
//		if (EVENT_TYPE_SCREEN_CHANGED == event.type) {
//			drawScreen();
//			lprintfln("yoooooooooooooooo1");
//		} else
//			if (EVENT_TYPE_ALERT == event.type) {
			mMainScreen->customEvent(event);
//		}
//			else if (EVENT_TYPE_ALERT_DISMISSED == event.type)
	}

	/**
	 * Method that draws display data to the screen.
	 */
//	void drawScreen()
//		{
//			MAExtent screenSize = maGetScrSize();
//			int width = EXTENT_X(screenSize);
//			int height = EXTENT_Y(screenSize);
//			int x = 20;
//			int y = height / 2;
//			char orientationText[128];
////			mMainScreen->drawChangeVerticalHorizontal(width, height);
//			if (width > height) // Landscape
//			{
//				// Set the background color.
//				maSetColor(0x000099);
//
//				// Set text.
//				sprintf(orientationText, "Landscape %d %d", width, height);
//			}
//			else // Portrait
//			{
//				// Set the background color.
//				maSetColor(0x009900);
//
//				// Set text.
//				sprintf(orientationText, "Portrait %d %d", width, height);
//			}
//
//			// Fill background
//			maFillRect(0, 0, width, height);
//
//			// Use white to display the text.
//			maSetColor(0xFFFFFF);
//
//			// Draw the text.
//			maDrawText(x, y, orientationText);
//
//			// Redraw the screen.
//			maUpdateScreen();
//		}
	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet() {
		delete mMainScreen;
	}

	/**
	 * Called when a key is pressed.
	 * This is a callback method declared in class Moblet.
	 */
	void keyPressEvent(int keyCode, int nativeCode) {

//		if (keyCode == MAK_MENU) {
//			lprintfln("maSendToBackground() = %d", maSendToBackground());
//		}
		// Let the screen handle the keypress.
		mMainScreen->handleKeyPress(keyCode);
	}

	/**
	 * Called when the screen was touched.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerPressEvent(MAPoint2d point) {
		if (getPlatform() != WINDOWSPHONE7)
			mMainScreen->handlePointerPressed(point);
	}

	/**
	 * Called when the pointer was moved on the screen.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerMoveEvent(MAPoint2d point) {
		if (getPlatform() != WINDOWSPHONE7)
			mMainScreen->handlePointerMoved(point);
	}

	/**
	 * Called when the screen was released.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerReleaseEvent(MAPoint2d point) {
		mMainScreen->handlePointerReleased(point);
	}

	virtual void focusLost() {
		lprintfln("focus lost");
	}

	virtual void focusGained() {
		mMainScreen->setActiveTab(0);
		lprintfln("focus gained");
	}

private:
	ScreenMain* mMainScreen;
};

/**
 * Main function that is called when the program starts.
 */


extern "C" int MAMain() {

		maSyscallPanicsDisable();
		maAutostartOn();
		Moblet::run(new NativeUIMoblet());
}
