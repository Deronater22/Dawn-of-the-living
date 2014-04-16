#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <stdlib.h>

#include "engine.h"
#include "timer.h"
#include "world.h"
#include "skybox.h"
#include "worldCoord.h"
#include <ctime>

//#define Debug// for use of debug items mac 4/16/14

using namespace lcgl;

extern OGLWindow myWindow;

const float Engine::DEFAULT_MOUSE_SENSITIVITY = 1.0f;//better sensitivity? mac

float Engine::GetNormalizedPosX(LPARAM lParam)
{	
	return mouseSensitivity () * (float)((short)LOWORD(lParam) - myWindow.width()/2) / myWindow.aspect ();
}

float Engine::GetNormalizedPosY(LPARAM lParam)
{
	return mouseSensitivity () * (float)((short)HIWORD(lParam) - myWindow.height()/2) / myWindow.aspect ();
}

void Engine::initialize (const OGLWindow& myWindow)
{
	pauseIcon_.load ("bill/DOTL.bmp");//load the pause menu icon here mac 4/16/14

	try
	{
		SetFocus (myWindow.hWND ()); //maybe this will force focus here, so that we won't need to catch exceptions?
									 //HEY, IT WORKS!  If you're doing something else while this starts,
									 // it grabs focus and does not then fail to initialize.
		inputSystem_.Initialize(myWindow.hWND(), (HINSTANCE)GetModuleHandle(NULL), true, IS_USEKEYBOARD | IS_USEMOUSE);
		inputSystem_.Acquire();
		inputSystem_.Update();
	}
	//Probably a better solution here is to grab focus, or wait until we get focus.
	//  Or maybe it would be good to do this only on a WM_ACTIVATE, because then you know you've got focus!
	catch (InputSystem::CantCreate&)
	{
		MessageBox (myWindow.hWND(), "Can't create InputSystem.  Try making sure this program has focus when you start it.",
					"Fatal error", MB_OK);
		exit (-1);
	}
	catch (Keyboard::CantCreate&)
	{
		MessageBox (myWindow.hWND(), "Can't create Keyboard handler.  Try making sure this program has focus when you start it.",
					"Fatal error", MB_OK);
		exit (-1);
	}
	catch (Mouse::CantCreate&)
	{
		MessageBox (myWindow.hWND(), "Can't create Mouse handler.  Try making sure this program has focus when you start it.",
					"Fatal error", MB_OK);
		exit (-1);
	}
}

void Engine::CheckInput(float deltaTime)
{
	static float buttonDelta = 0.0f;

	int mouseDeltaX, mouseDeltaY;	// changes in the mouse position

	// decrease amount of time until next possible recognized button pressing
	buttonDelta -= deltaTime;

	if (buttonDelta < 0.0f)		buttonDelta = 0.0f;

	//Brandon Johnson fix 1-28-2010:  this prevents crashing on change of focus,
	// either for Alt-Tab or from breaking in the debugger.
	try 
	{
		inputSystem_.Update();

		// retrieve the latest mouse movements
		inputSystem_.GetMouseMovement(mouseDeltaX, mouseDeltaY);
	
		OnMouseMove(mouseDeltaX, mouseDeltaY);

		//pauses game 
		if(inputSystem_.KeyDown(DIK_P))//pause the game mac 4/12/14         
		{	OnKeyDown(DIK_P);
			if(isPaused() == false){setPause(true);}
		}

		if(inputSystem_.KeyDown(DIK_U))
		{	OnKeyDown(DIK_P);
			setPause(false);		
		}

		if (inputSystem_.KeyDown(DIK_F3))        OnKeyDown(VK_F3);
		if (inputSystem_.KeyDown(DIK_W))		OnKeyDown(VK_UP);
		if (inputSystem_.KeyDown(DIK_S))		OnKeyDown(VK_DOWN);
		if (inputSystem_.KeyDown(DIK_A))		OnKeyDown(VK_LEFT);
		if (inputSystem_.KeyDown(DIK_D))		OnKeyDown(VK_RIGHT);
		if (inputSystem_.KeyDown(DIK_ADD))		OnKeyDown(VK_ADD);
		if (inputSystem_.KeyDown(DIK_SUBTRACT))	OnKeyDown(VK_SUBTRACT);
		if (inputSystem_.KeyDown(DIK_ESCAPE))	OnKeyDown(VK_ESCAPE);
		if (inputSystem_.ButtonDown(Mouse::LEFT))
		{
			if (buttonDelta == 0.0f)
			{
				OnMouseDownL(0,0);
				buttonDelta = 0.2f;//better sensitivity? mac
			}
		}
	}
	catch(...)
	{
	}
}

void Engine::GameCycle(float deltaTime)
{
	
	
	if (useDInput_)
	{
		CheckInput(deltaTime);


		
		OnPrepare();							// setup opengl for frame (clear, identity)

		world().Prepare();						// prepare objects and perform collisions
		
		
		if( !(isPaused()))						//if the game is not paused mac 4/12/14
		{
			world().AnimateParticles(deltaTime);//set up particle system mac 3/15/14

			world().Animate(deltaTime);	// move/orient objects

		
			world().Draw();				// draw objects
		}

		else//the pause menu implementation is here Mike && Deron
		{
			glDisable(GL_TEXTURE_2D);//needs this to render text mac 4/16/14
			glClearColorv(BLACK);
			glColor4fv(GREEN);
			print(pause_menu, twoDCoord<float> (-0.21f,0.27f),"-----Game Paused----"); 
			print(pause_menu, twoDCoord<float> (-0.21f,0.17f),"Press U to Resume"); 
			print(pause_menu, twoDCoord<float> (-0.225f,0.07f),"Press Escape to Quit");
			print(pause_menu, twoDCoord<float> (-0.425f,-0.50f),"DOTL v0.1.5 created by: DOTL Dev Team");

#if Debug//set of reference points for drawing textured quads mac 4/16/14
			//text for coordinates
			print(pause_menu, twoDCoord<float> (0.225f,-0.1f),"X");
			print(pause_menu, twoDCoord<float> (-0.225f,-0.1f),"X");
			print(pause_menu, twoDCoord<float> (-0.225f,-0.4f),"X");
			print(pause_menu, twoDCoord<float> (0.225f,-0.4f),"X");
#endif
			
			
			drawIcon();
		}

	}

}

LRESULT Engine::EnterMessageLoop(const OGLWindow& myWindow)
{
	MSG msg;
	Timer timer;

	timer.Init();

	for (;;)
	{	
		GameCycle(timer.GetElapsedSeconds()); SwapBuffers(myWindow.hDC());			

		while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			// we always update if there are any events, even if we're paused
			if (!GetMessage (&msg, NULL, 0, 0))	return msg.wParam;

			TranslateMessage (&msg);
   			DispatchMessage (&msg);
		}
	}

	return msg.wParam;
}
void Engine::drawIcon()const //draws the icon for spitting acid mac 4/5/14
{

	const int WINDOW_HI = 800, WINDOW_WID = 600;//window parameters

	glEnable(GL_TEXTURE_2D);  //make sure we can render the texture
	pauseIcon_.activate();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0.0, WINDOW_HI, WINDOW_WID, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

#ifdef Debug //debugging for points only
	glColor3fv(YELLOW);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
#else
	glBegin(GL_QUADS);//draw points for menu icons mac 4/16/14
#endif
	//don't mess with this order for future icons mac 4/16/14
	glTexCoord2f(0.0,1.0); glVertex2f(288.0, 345.0);//top left  DONE
	glTexCoord2f(0.0,0.0); glVertex2f(288.0, 535.0);//bottom left
	glTexCoord2f(1.0,0.0); glVertex2f(525.0, 535.0);//bottom right
	glTexCoord2f(1.0,1.0); glVertex2f(525.0, 345.0);//top right DONE
	

	glEnd();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D); //disable texture so we do not mess up our scene



}