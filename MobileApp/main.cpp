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

		mMainScreen = new ScreenMain();
		addTimer(this, TIMER_PERIOD, 0);
		addFocusListener(this);

		Authentication* mAuthentication = new Authentication(getSystemLanguage(), mMainScreen);
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
