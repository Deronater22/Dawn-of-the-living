/************************************************************************/
/* Menu.cpp
	Description: adds standard pause and information menus to the game 
	More to come later?
	written by: Michael Carey 4/17/14
	CC: Joe Bailey
*/
/************************************************************************/


#include "Menu.h"
#include "worldCoord.h"


const int WINDOW_HI = 800, WINDOW_WID = 600;//window parameters


using namespace lcgl;


Menu::Menu(void):pause_menu("Arial", 30)
				,info_page ("Arial", 30)
				,info_font ("Arial", 20)
{
	pauseIcon_.load ("bill/biopause.jpg");//load the pause menu icon here mac 4/16/14
	info_icon.load("bill/ebs.jpg");
}

Menu::~Menu(void)
{
}
void Menu::drawPauseIcon()const //draws the icon for spitting acid mac 4/5/14
{

	

	glEnable(GL_TEXTURE_2D);  //make sure we can render the texture
	pauseIcon_.activate();

	glMatrixMode(GL_PROJECTION);glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0.0, WINDOW_HI, WINDOW_WID, 0.0f);
	glMatrixMode(GL_MODELVIEW);


	glColor3fv(GREY);
	glBegin(GL_QUADS);//draw points for menu icons mac 4/16/14

	//don't mess with this order for future icons mac 4/16/14
	glTexCoord2f(0.0,1.0); glVertex3f(0.0f, 0.0f,-0.994f);//top left  DONE
	glTexCoord2f(0.0,0.0); glVertex3f(0.0f, 600.0f,-0.994f);//bottom left
	glTexCoord2f(1.0,0.0); glVertex3f(800.0f, 600.0f,-0.994f);//bottom right
	glTexCoord2f(1.0,1.0); glVertex3f(800.0f, 0.0f,-0.994f);//top right DONE

	glEnd();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);glPopMatrix();
	glMatrixMode(GL_MODELVIEW) ;glPopMatrix();
	glDisable(GL_TEXTURE_2D); //disable texture so we do not mess up our scene



}
void Menu::drawInfoIcon()const //draws the icon for spitting acid mac 4/5/14
{


	glEnable(GL_TEXTURE_2D);  //make sure we can render the texture
	info_icon.activate();

	glMatrixMode(GL_PROJECTION);glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0.0, WINDOW_HI, WINDOW_WID, 0.0f);
	glMatrixMode(GL_MODELVIEW);


#ifdef Debug //debugging for points only
	glDisable(GL_TEXTURE_2D);
	glColor3fv(YELLOW);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
#else
	glColor4fv(GREY);
	glBegin(GL_QUADS);//draw points for menu icons mac 4/16/14
#endif
	//don't mess with this order for future icons mac 4/16/14
	glTexCoord2f(0.0,1.0); glVertex3f(0.0f, 0.0f,-0.9f);//top left  DONE
	glTexCoord2f(0.0,0.0); glVertex3f(0.0f, 600.0f,-0.9f);//bottom left
	glTexCoord2f(1.0,0.0); glVertex3f(800.0f, 600.0f,-0.9f);//bottom right
	glTexCoord2f(1.0,1.0); glVertex3f(800.0f, 0.0f,-0.9f);//top right DONE


	glEnd();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);glPopMatrix();
	glMatrixMode(GL_MODELVIEW);glPopMatrix ();
	glDisable(GL_TEXTURE_2D); //disable texture so we do not mess up our scene



}
void Menu::drawPauseMenu()const
{
	drawPauseIcon();
	
	glDisable(GL_TEXTURE_2D);//needs this to render text mac 4/16/14
	glClearColorv(BLACK);
	glColor4fv(GREEN);
	print(pause_menu, twoDCoord<float> (-0.21f,0.27f),"-----Game Paused----"); 
	print(pause_menu, twoDCoord<float> (-0.21f,0.17f),"Press U to Resume"); 
	print(pause_menu, twoDCoord<float> (-0.16f,0.07f),"Press I for Info");
	print(pause_menu, twoDCoord<float> (-0.21f,-0.02f),"Press Escape to Quit"); 

	print(pause_menu, twoDCoord<float> (-0.425f,-0.50f),"DOTL v0.1.5 created by: DOTL Dev Team");


	

}
void Menu::drawInfoPage()const
{

	drawInfoIcon();
	glDisable(GL_TEXTURE_2D);
	glClearColorv(BLACK);

	glColor4fv(GREEN);
	print(info_page, twoDCoord<float> (-0.41f,0.46f),"Welcome to Dawn of the Living v0.1.5"); 


	print(info_font, twoDCoord<float> (-0.6f,0.27f),"-----Objectives----");//show current objectives
	print(info_font, twoDCoord<float> (-0.68f,0.22f),"Show the humans whose boss!");
	print(info_font, twoDCoord<float> (-0.62f,0.18f),"Explore and have fun!");

	print(info_font, twoDCoord<float> (0.34f,0.27f),"-----Controls----");//show game controls
	print(info_font, twoDCoord<float> (0.25f,0.22f),"WSAD Keys:Zombie Control"); 
	print(info_font, twoDCoord<float> (0.26f,0.18f),"Left Mouse:Action Control"); 
	print(info_font, twoDCoord<float> (0.26f,0.14f),"-+ Keys: Mouse Sensitivity");
	print(info_font, twoDCoord<float> (0.26f,0.1f),"P Key: Pause Game"); 
	print(info_font, twoDCoord<float> (0.26f,0.06f),"U Key: Resume/Start Game"); 
	print(info_font, twoDCoord<float> (0.26f,0.02f),"ESC Key: Exit Game"); 
	print(info_page, twoDCoord<float> (-0.425f,-0.50f),"DOTL v0.1.5 created by: DOTL Dev Team");


}