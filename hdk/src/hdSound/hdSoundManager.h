

#ifndef _HD_SOUND_MANAGER_
#define _HD_SOUND_MANAGER_

#include "hdMath.h"
#include "hdCore.h"
#include "hdCollections.h"
#include "hdSound/hdSoundEngineInterface.h"
#include "hdSound/hdSound.h"
#include "hdSound/hdSoundSystemEnums.h"



/*
 * A set of static methods, wrapped around an engine that deals with sound.
 */
class hdSoundManager
{
public:

	static void CheckSoundStatus(const char *location);

	static void TearDown();
	
	static void InitSoundManager();
	
	static hdSound* FindSound(const char* filename, e_soundType type);
	
	static hdSound* LoadSound(const char* filepath, e_soundType type);
	
	static hdSound* LoadLoopingSound(const char* filepath, const char* attackFilepath, const char* decayFilePath);
	
	static hdSound* GetSound(const int location);
	
	static bool PlaySound(hdSound* sound);
	
	static bool StopSound(hdSound* sound);
	
	static bool UnloadSound(hdSound *sound);
	
	static hdSound* GetNullSound();
	
	static void DestroySounds();
	
	static void SetMusicMaxVolume(const float vol);
	
	static void SetSoundMaxVolume(const float vol);
	
	static void SetVibrationsOn();
	
	static void SetVibrationsOff();
	
	
	/*
private:

	hdPointerList<hdSound, kMaxSounds>* m_sounds;
	
	hdSound* m_nullSound;

	
	static void CheckSoundStatus();

	
protected:
	hdSoundManager();
	
	~hdSoundManager();
	
public:
	
	static hdSoundManager* Instance();
	
	static void TearDown();
	
	void Initialise();
	
	hdSound* FindSound(const char* filename, e_soundType type);
	
	hdSound* LoadSound(const char* filepath, e_soundType type);
	
	hdSound* LoadLoopingSound(const char* filepath, const char* attackFilepath, const char* decayFilePath);
	
	hdSound* GetSound(const int location);
	
	static bool PlaySound(hdSound* sound);
	
	static bool StopSound(hdSound* sound);
	
	hdSound* GetNullSound()
	{
		return m_nullSound;
	}
	
	void DestroySounds();
	 */
};


#endif