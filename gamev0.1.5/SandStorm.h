/****************************************************************************
// represents a sand storm

//created by Michael Carey
*****************************************************************************/
#include "Particles.h"
#include "texture.h"
#include "FRAND.h"


extern const lcgl::worldCoord  SAND_P_VELOCITY;
extern const lcgl::worldCoord  VELOCITY_VARIATION;
extern const float             SAND_GRAIN_SIZE;


using namespace lcgl;

class Sandstorm : public ParticleSystem
{


#ifdef FAST_CPU//less particles for slower processors mac 4/22/14

	enum{GRAINS_PER_SEC = 800};
#else

	enum{GRAINS_PER_SEC = 250};
#endif

	
public:
	Sandstorm(int maxParticles, const worldCoord& origin, 
			float height, float width, float depth);
	~Sandstorm();

	void  Update(float elapsedTime);
	void  Render();

	static float Frand(int x);
	static float Frand(){return FRAND;};

	void  InitializeSystem();

protected:
	void    InitializeParticle(int index);

private:

	float   height;
	float   width;
	float   depth;

	lcgl::Texture2D sandTex;//actual sand texture

};


