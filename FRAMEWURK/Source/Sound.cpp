#include "Sound.h"
#include <irrKlang.h>

#pragma comment (lib, "irrKlang.lib")
using namespace irrklang;
ISoundEngine *sound = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS);


Sound::Sound()
{

}

Sound::~Sound()
{

}

void Sound::Walking()
{
	//sound->play3D("../irrKlang/media/footstep.mp3", vec3df(0,0,0), false);
}