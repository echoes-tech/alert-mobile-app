
#include <ma.h> 				// Syscalls
#include <MAUtil/String.h>		// C++ String class
#include <MAUtil/Moblet.h>		// Moblet class

#include <conprint.h>			// lprintfln for logging

#include <NativeUI/Widgets.h>// Include all widgets

#include "ScreenMain.h"			// Main UI screen
#include "Util.h"

// How often the timer will be invoked
#define TIMER_PERIOD 60000


using namespace MAUtil;
using namespace NativeUI;

/**
 * Moblet for the  application.
 */
class NativeUIMoblet : public Moblet, public FocusListener, public TimerListener
{
public:
	/**
	 * Constructor that creates the UI.
	 */
	NativeUIMoblet()
	{
		addTimer(this, TIMER_PERIOD, 0);
		addFocusListener(this);
		maScreenSetOrientation(SCREEN_ORIENTATION_DYNAMIC);

				// iOS and Windows Phone.
				maScreenSetSupportedOrientations(
					MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT |
					MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT |
					MA_SCREEN_ORIENTATION_PORTRAIT |
					MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN);

				// Update the display.
				drawScreen();


		// Create the main user interface screen.
		mMainScreen = new ScreenMain();

		// Show the screen.
		mMainScreen->show();
	}

	// send timer event to trackingTabObject
	void runTimerEvent()
		 {
		mMainScreen->pullRequest();
				}

	/**
		 * Method for handling events that do not have predefined methods.
		 */
		void customEvent(const MAEvent& event)
		{
			// If the event type is screen changed we update the display.
			if (EVENT_TYPE_SCREEN_CHANGED == event.type)
			{
				drawScreen();
			}
		}

		/**
		 * Method that draws display data to the screen.
		 */
		void drawScreen()
		{
			MAExtent screenSize = maGetScrSize();
			int width = EXTENT_X(screenSize);
			int height = EXTENT_Y(screenSize);
			int x = 20;
			int y = height / 2;
			char orientationText[128];

			if (width > height) // Landscape
			{
				// Set the background color.
				maSetColor(0x000099);

				// Set text.
				sprintf(orientationText, "Landscape %d %d", width, height);
			}
			else // Portrait
			{
				// Set the background color.
				maSetColor(0x009900);

				// Set text.
				sprintf(orientationText, "Portrait %d %d", width, height);
			}

			// Fill background
			maFillRect(0, 0, width, height);

			// Use white to display the text.
			maSetColor(0xFFFFFF);

			// Draw the text.
			maDrawText(x, y, orientationText);

			// Redraw the screen.
			maUpdateScreen();
		}


	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		delete mMainScreen;
	}

	/**
	 * Called when a key is pressed.
	 * This is a callback method declared in class Moblet.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{

		if(keyCode == MAK_1 || keyCode == MAK_SOFTRIGHT)
				{

//					maSendToBackground(); //
//			maAlert("Application Update Available", "test" , "Yes", "", "No");
//			lprintfln("maBringToForeground() = %d" ,maBringToForeground());
//								lprintfln("maSendToBackground() = %d" ,maSendToBackground());
//								lprintfln("aprés wait");
			exit();

//								lprintfln("maBringToForeground() = %d" ,maBringToForeground());
//								maBringToForeground();

				}

		// Let the screen handle the keypress.
		mMainScreen->handleKeyPress(keyCode);
	}

	/**
	 * Called when the screen was touched.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerPressEvent(MAPoint2d point)
	{
		if(getPlatform() != WINDOWSPHONE7) mMainScreen->handlePointerPressed(point);
	}

	/**
	 * Called when the pointer was moved on the screen.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerMoveEvent(MAPoint2d point)
	{
		if(getPlatform() != WINDOWSPHONE7) mMainScreen->handlePointerMoved(point);
	}

	/**
	 * Called when the screen was released.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerReleaseEvent(MAPoint2d point)
	{
		mMainScreen->handlePointerReleased(point);
	}

	virtual void focusLost(){lprintfln("focus lost"); toto = 1;}

	virtual void focusGained(){lprintfln("focus gained"); toto = 0;}

//	void customEvent(const MAEvent& event);
//		        {
////		          if(event.type == EVENT_TYPE_FOCUS_LOST)
////		          {
////		                  //Do things when sent to background
////		          }
////		              else if(event.type == EVENT_TYPE_FOCUS_GAINED)
////		              {
////		                 //Do things when brought back from background
////		              }
//		        }
private:
	ScreenMain* mMainScreen;
	int toto;
};

/**
 * Main function that is called when the program starts.
 */
//int MAMain() GCCATTRIB(noreturn);

static int RUN = 0;

extern "C" int MAMain()
{
//	lprintfln("maBringToForeground() = %d" ,maBringToForeground());
//	maLockKeypad();

if (RUN == 0)
{
	RUN=1;
	Moblet::run(new NativeUIMoblet());

}
//	return 0;
}
