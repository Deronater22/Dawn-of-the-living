 
enum {PLAYER_ELEVATION = 14};	 //How far above the map does the camera stay?

/*
	WORLD.H

  	The World class

			   The World class represents the game world
			   of the engine. All objects, the terrain,
			   audio system, ambient world music and sound,
			   and the camera are held here.

			   render Skybox and particle functions here mac 3/15/14
*/

#include "world.h"
#include "sheman.h"
#include "Brute.h"
#include "ms3dMonster.h"
#include "skybox.h"
#include "EnviroObj.h"
#include <Windows.h>
#include "timer.h"
#include "InputSystem.h"

using namespace lcgl;

static const worldCoord STARTING_PLAYER_POSITION (346, 181, 396);

World::World(int w, int h) : _worldTrack ("sounds//darksiege.mid"),_ambient("ambient.wav"),
_explosionSound ("sounds/slimesplash.wav"), terrain (32, 1.0f),
skybox_("media/desplain"), debug("Arial" ,25), 
theSand_(1500, worldCoord(4,-6, -4), 5.0, 5.0, 5.0),gui(player),_isDebug(false)

{

	_timer.Init();									//initialize timer for fps mac 4/12/14
	_explosionSound.setRepeats (0);

	SetScreen (w, h);
	gameDone = false;								// game is just starting

	player.position () = STARTING_PLAYER_POSITION;
	player.moveTo (STARTING_PLAYER_POSITION);		//player has 2 types of position
													//through multiple inheritance -- fix this
	push_back (&terrain);
	push_back (&player);

	player.SetWorld (this); terrain.SetWorld(this);
	player.SetTerrain(&terrain);

	_worldTrack.loopForever ();	 _worldTrack.play ();	//world music track mac 5/5/14
	_ambient.loopForever    ();  _ambient.play();		//ambient sounds

	LoadWorld();

	timeStart = 300.0;			// 300 seconds = 5 minutes
	timeElapsed = 0.0;
	gui.SetCurrentTime(timeStart);
	gui.SetEnemiesLeft(numOgros + numSods + numDwarfs);

	theSand_.InitializeSystem();   // initialize particle system

}

void World:: AnimateParticles(float deltaTime)//particle system functions called here
{

	theSand_.Render();

	theSand_.Update(deltaTime);
}

void World::Animate( float deltaTime)
{

	player.Animate (deltaTime);
	skybox_.render (player.position());  //draw the skybox that moves with the player
	
	// set camera height based on player position on terrain
	player.position().y() = terrain.GetHeight(player.position().x(), 
											player.position().z()) 
								+ player.size();

	//This is the maximum allowable value for x or z
	float maxValue = terrain.GetWidth()*terrain.GetMul() - terrain.edgeWidth();
	
	if (player.position().x() <= terrain.edgeWidth())
		player.position().x() =  terrain.edgeWidth();

	if (player.position().x() >= maxValue)
		player.position().x() =  maxValue;
	
	if (player.position().z() <= terrain.edgeWidth())
		player.position().z() =  terrain.edgeWidth();
	
	if (player.position().z() >= maxValue)
		player.position().z() =  maxValue;

	ObjectSet::animate (deltaTime); //do we really need to call player.Animate separately?

	//This complicated mess is here because if we delete a dead monster, we invalidated that iterator
	// and all subsequent iterators.  So we give up and try again.
	bool lastIterationDeletedSomething;
	do
	{
		lastIterationDeletedSomething = false;
		for (vector<Object*>::iterator i = begin(); i != end(); ++i)
			if ((*i)->isDead())
			{
				erase (i);
				lastIterationDeletedSomething = true;
				break;
			}
	} while (lastIterationDeletedSomething);

	numSods  = CountObjectTypes(typeid(ShemanEnemy));           
	numOgros = CountObjectTypes(typeid(BruteEnemy));       
	numDwarfs= CountObjectTypes(typeid(MS3DMonster));

	gui.SetEnemiesLeft(numOgros + numSods + numDwarfs);
	gui.SetCurrentTime(timeStart - timeElapsed);

	if (!gameDone)		timeElapsed += deltaTime;
	else				timeElapsed = timeStart;

	if(isDebugging()== true){//new GUI debug mike 4/12/14
		//******************************DEBUG***************************************
		//debug player to display accurate (current)position
		glColor4fv(GREEN);
		print(debug,twoDCoord<float>(-0.65f,0.48f),"---Debug DOTL v0.1.6-----------");
		print(debug, twoDCoord<float>(-0.65f,0.44f), "Map pos: X:%4.0f Y:%4.0f Z:%4.0f" ,
		player.position().x(), player.position().y(), player.position().z() );
		//debug fps  mac 3/15/14
		
		print(debug,twoDCoord<float>(-0.65f,0.40f) ,"Fps: %4.0f", _timer.GetFPS(1) );
		print(debug,twoDCoord<float>(-0.65f,0.36f) ,"------------------------------------------" );
		//********************************FPS***************************************
	}
	
}

void World::FadeScreen()
{
	glPushMatrix();
		glLoadIdentity();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_TEXTURE_2D);
		glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
		glBegin(GL_QUADS);
			glVertex3f(-5, -5, -3.0);
			glVertex3f(5, -5, -3.0);
			glVertex3f(5, 5, -3.0);
			glVertex3f(-5, 5, -3.0);
		glEnd();
		glColor4fv(WHITE);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	glPopMatrix();
}

void World::Draw()
{
	ObjectSet::Draw (player);
	gui.Draw();
	
	if (gameDone)
	{
		FadeScreen();
		if (numOgros + numSods + numDwarfs <=0) gui.DrawWinner();
		else									gui.DrawLoser();
	}
}

void World::OnPrepare()
{
	glClearColorv(terrain.fogColor);
	ObjectSet::Prepare ();
	ObjectSet::ProcessCollisions ();

	if ((numOgros + numSods + numDwarfs <= 0) || (timeElapsed >= timeStart)) gameDone = true;
}

void World::distribute (Enemy* enemy)
{
	push_back (enemy);

	enemy->position_.x() = (float)(rand() % (int)(terrain.GetWidth() * terrain.GetMul()));
	enemy->setElevation(0.0f);
	enemy->position_.z() = (float)(rand() % (int)(terrain.GetWidth() * terrain.GetMul()));
}

void World::loadExplosionTexture ()
{
	_explosionTexture.load("bill//slime.bmp");	 //for rockets
	_explosionTexture.activate();
	_explosionTexture.repeat();
	_explosionTexture.setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
	_explosionTexture.buildMipmaps (); 
}

void World::LoadWorld()
{
	
	loadExplosionTexture (); //for rocket explosions
	static Sound explosionSound ("explode.wav");	//Should be done in World, once

	numOgros = numSods = (rand() % (MAX_ENEMIES-4)) + 4;	// random # from 4 to MAX
	
	//I don't do this to dwarfs; there's only 1 dwarf in our world.
	numDwarfs = 0;

	OgroModel.load("models\\male\\male.md2" , "models\\male\\6doom1.pcx" );//new male by mike re skinned from q2 model
	SodModel.load ("models\\hunt\\tris.md2", "models\\hunt\\ctf_r.pcx");//new male by mike re skinned from q2 model
	AcidModel.load("models\\acid\\throwup2.md2"  , "models\\acid\\slime.jpg");
	
	//Alternate models Mac 4/6/14
	//SodModel.load("models\\male\\male.md2" , "models\\male\\mskin.pcx" );//new male by mike re skinned from q2 model
	//SodModel.load("models\\girl\\girl.md2" , "models\\girl\\g9.pcx");//new girl by mike re skinned from q2 model
	
	//dwarfModel.load ("models\\dwarf\\dwarf1.ms3d");

	
	// generate enemies
	for (int enemyIdx = 0; enemyIdx < numOgros; enemyIdx++)
	{
		BruteEnemy* bruteEnemy = new BruteEnemy (OgroModel, this, &player); distribute (bruteEnemy);
		ShemanEnemy*   shemanEnemy = new  ShemanEnemy (SodModel, this, &player); distribute ( shemanEnemy);
	}

	//I note that the enemies are always in sync -- maybe introduce a random element so they start
	// on different frames.
	//This synchronization happened BEFORE I started using Models apart from instances (I think)
	
	//MS3DMonster* dwarf = new MS3DMonster (dwarfModel, this, &player);
	//dwarf->position_ =  player.position(); dwarf->position_.x() += 75.0f;       //no dwarf for now mike 4/5 14
	//push_back (dwarf);
	//++numDwarfs;

	////Simple test monster, always there when we start, for debugging
	//BruteEnemy* enemy = new BruteEnemy (OgroModel, this, &player); 
	//enemy->position_ = player.position(); enemy->position_.x() += 50.0f;//causes enemy behind you bug mac 4/7/14
	//++numOgros; push_back (enemy);

}
World::~World ()
{
}