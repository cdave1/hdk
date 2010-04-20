#include "hdSoundManager.h"

static hdSoundManager* m_SoundManagerInstance = NULL;




//#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)	
static signed int m_status;
//#else
//	int m_status;
//#endif

static hdPointerList<hdSound, kMaxSounds>* m_sounds = NULL;

static hdSound* m_nullSound;

static float m_maxMusicVolume;

static float m_maxSoundVolume;

static bool m_playVibrations = true;



void hdSoundManager::TearDown()
{	
	hdSound *sound;
	if (m_sounds != NULL)
	{
		while (!m_sounds->IsEmpty())
		{
			sound = m_sounds->GetItems()[0];
			switch(sound->type)
			{
				case e_soundTypeBackground:
				case e_soundTypeBackgroundLooping:
					SoundEngine_UnloadBackgroundMusic(sound);
					break;
				case e_soundTypeFrequent:
				case e_soundTypeNormal:
				case e_soundTypeLoop:
					SoundEngine_UnloadEffect(sound);
					break;
				case e_soundTypeVibration:
					break;
				default:
					assert(false);
					break;
			}
			m_sounds->Remove(sound);
		}
		delete m_sounds;
		m_sounds = NULL;
	}

	SoundEngine_Teardown();
}


void hdSoundManager::DestroySounds()
{
	if (m_sounds == NULL) return;
	
	hdSound *sound;
	while (!m_sounds->IsEmpty())
	{
		sound = m_sounds->GetItems()[0];
		switch(sound->type)
		{
			case e_soundTypeBackground:
			case e_soundTypeBackgroundLooping:
				//SoundEngine_UnloadBackgroundMusicTrack();
				SoundEngine_StopBackgroundMusic(sound);
				SoundEngine_UnloadBackgroundMusic(sound);
				break;
			case e_soundTypeFrequent:
			case e_soundTypeNormal:
			case e_soundTypeLoop:
				SoundEngine_StopEffect(sound);
				SoundEngine_UnloadEffect(sound);
				break;
			case e_soundTypeVibration:
				break;
			default:
				assert(false);
				break;
		}
		m_sounds->RemoveAtIndex(0);
	}
	SoundEngine_Purge();
}


void hdSoundManager::InitSoundManager()
{
	m_sounds = new hdPointerList<hdSound, kMaxSounds>();
	
	SoundEngine_Init();
	
	m_nullSound = NULL; //this->FindSound( "***r_noSound***", e_soundTypeNormal);
	
	m_maxMusicVolume = 1.0f;
	
	m_maxSoundVolume = 1.0f;
}


hdSound* hdSoundManager::GetNullSound()
{
	return m_nullSound;
}


hdSound* hdSoundManager::GetSound(const int location)
{
	hdAssert(m_sounds != NULL);
	if (location >= m_sounds->GetItemCount()) return NULL;
	return (hdSound *)m_sounds->GetItems()[location];
}


hdSound* hdSoundManager::FindSound(const char* filename, e_soundType type)
{
	hdSound	*sound;
	
	if (m_sounds == NULL)
	{
		return m_nullSound;
	}
	
	hdAssert(m_sounds != NULL); // Because I keep fucking this up.
	
	
	for (int i = 0; i < m_sounds->GetItemCount(); ++i)
	{
		if (strncmp(m_sounds->GetItems()[i]->filename, filename, kMaxSoundPathSize) == 0)
		{
			return (hdSound*)m_sounds->GetItems()[i];
		}
	}
	
	if ((sound = LoadSound(filename, type)) != NULL)
	{
		return sound;
	}
	return m_nullSound;
}


bool hdSoundManager::PlaySound(hdSound* sound)
{
	if (m_sounds == NULL)
	{
		return false;
	}
	
	hdAssert(m_sounds != NULL);
	
	if (sound == hdSoundManager::GetNullSound()) return false;
	if (strlen(sound->filename) == 0) 
	{
		hdError(-1, "Sound Engine: zero-length filename, name: %s. Aborting...", sound->filename);
		assert(false);
	}
	
	// Clamp the volume
	if (sound->type == e_soundTypeBackground ||
		sound->type == e_soundTypeBackgroundLooping)
	{
		sound->volume = hdClamp(sound->volume, 0.0f, m_maxMusicVolume);
	} 
	else
	{
		sound->volume = hdClamp(sound->volume, 0.0f, m_maxSoundVolume);
	}
	
	switch(sound->type)
	{
			//#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
		case e_soundTypeBackground:
			sound->isBackgroundPlaying = true;
			
			m_status = SoundEngine_LoadBackgroundMusic(sound);
			CheckSoundStatus("SoundEngine_LoadBackgroundMusic");
			
			m_status = SoundEngine_PlayBackgroundMusic(sound, false);
			CheckSoundStatus("SoundEngine_PlayBackgroundMusic");
			
			break;
		case e_soundTypeBackgroundLooping:
			sound->isBackgroundPlaying = true;
			
			m_status = SoundEngine_LoadBackgroundMusic(sound);
			CheckSoundStatus("SoundEngine_LoadBackgroundMusic");
			
			m_status = SoundEngine_PlayBackgroundMusic(sound, true);
			CheckSoundStatus("SoundEngine_PlayBackgroundMusic");

			break;
		case e_soundTypeFrequent:
		case e_soundTypeNormal:
		case e_soundTypeLoop:
			m_status = SoundEngine_PlayEffect(sound);
			CheckSoundStatus("SoundEngine_PlayEffect");
			break;
		case e_soundTypeVibration:
			if (m_playVibrations)
			{
				m_status = SoundEngine_PlayVibrationEffect(sound);
			}
			else 
			{
				m_status = 0;
			}
			CheckSoundStatus("SoundEngine_PlayEffect");
			break;
		default:
			hdError(-1, "Sound Engine: unrecognised type for file, name: %s. Aborting...", sound->filename);
			break;
			//#endif
	}
	return true;
}


bool hdSoundManager::StopSound(hdSound* sound)
{
	hdAssert(m_sounds != NULL);
	
	switch(sound->type)
	{
			//#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
		
		case e_soundTypeBackground: 
		case e_soundTypeBackgroundLooping:
			/*
			sound->isBackgroundPlaying = false;
			m_status = SoundEngine_StopBackgroundMusic(false);
			CheckSoundStatus();
			m_status = SoundEngine_UnloadBackgroundMusicTrack();
			CheckSoundStatus();
			 */
			
			sound->isBackgroundPlaying = false;
			m_status = SoundEngine_StopBackgroundMusic(sound);
			CheckSoundStatus("SoundEngine_StopBackgroundMusic");
			
			m_status = SoundEngine_UnloadBackgroundMusic(sound);
			CheckSoundStatus("SoundEngine_UnloadBackgroundMusic");
			break;
		case e_soundTypeFrequent:
		case e_soundTypeNormal:
		case e_soundTypeLoop:
			SoundEngine_StopEffect(sound);
			CheckSoundStatus("SoundEngine_StopEffect");
			break;
		case e_soundTypeVibration:
			break;
		default:
			hdError(-1, "Sound Engine: unrecognised type for file, name: %s. Aborting...", sound->filename);
			break;
			//#endif
	}
	
	return true;
}


hdSound* hdSoundManager::LoadLoopingSound(const char* loopFileName, const char* attackFileName, const char* decayFileName)
{
	return NULL;
	/*
	hdAssert(m_sounds != NULL);
	
	hdSound	*sound;
	char* pathBase = hdGamedir();
	
	sound = new hdSound(); //;hdAllocateSound( name );
	
	m_sounds->Add(sound);
	sound->soundId = 0;
	sound->type = e_soundTypeLoop;
	snprintf(sound->filename, kMaxSoundPathSize, loopFileName);
	
	//#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
	char loopFilePath[kMaxSoundPathSize];
	char attackFilePath[kMaxSoundPathSize];
	char decayFilePath[kMaxSoundPathSize];
	
	pathBase = hdGamedir();
	snprintf( loopFilePath, kMaxSoundPathSize, "%s/%s", pathBase, loopFileName );
	snprintf( attackFilePath, kMaxSoundPathSize, "%s/%s", pathBase, attackFileName );
	snprintf( decayFilePath, kMaxSoundPathSize, "%s/%s", pathBase, decayFileName );
	
	snprintf(sound->fullFilePath, kMaxSoundPathSize, loopFilePath);
	
	m_status = SoundEngine_LoadLoopingEffect(loopFilePath, attackFilePath, decayFilePath, (UInt32*)&sound->soundId);
	//#endif
	CheckSoundStatus();
	
	return sound;
	 */
}


hdSound* hdSoundManager::LoadSound(const char* filename, e_soundType type)
{
	hdAssert(m_sounds != NULL);
	
	// looping sounds should be loaded with LoadLoopingSound.
	assert(type != e_soundTypeLoop);
	
	
	hdSound	*sound;
	char* pathBase = FileSystem_BaseDir();
	
	sound = new hdSound();
	
	m_sounds->Add(sound);
	sound->soundId = 0;
	sound->type = type;
	snprintf(sound->filename, kMaxSoundPathSize, "%s", filename);
	snprintf(sound->fullFilePath, kMaxSoundPathSize, "%s%s", pathBase, filename );
	
	switch( type )
	{
		case e_soundTypeBackground:
		case e_soundTypeBackgroundLooping:
			m_status = SoundEngine_LoadBackgroundMusic(sound);
			CheckSoundStatus("[hdSoundManager::LoadSound] e_soundTypeBackground");
			break;
		case e_soundTypeFrequent:
			m_status = SoundEngine_LoadEffect(sound, true);
			CheckSoundStatus("[hdSoundManager::LoadSound] e_soundTypeFrequent");
			break;
		case e_soundTypeNormal:
			m_status = SoundEngine_LoadEffect(sound, false);
			CheckSoundStatus("[hdSoundManager::LoadSound] e_soundTypeNormal");
			break;
		case e_soundTypeVibration:
			break;
		default:
			assert(false);
			break;
	}
	
	
	
	return sound;
}



bool hdSoundManager::UnloadSound(hdSound *sound)
{
	hdAssert(m_sounds != NULL);
	
	switch(sound->type)
	{
			//#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
		case e_soundTypeBackground:
		case e_soundTypeBackgroundLooping:
			SoundEngine_UnloadBackgroundMusic(sound);
			break;
		case e_soundTypeFrequent:
		case e_soundTypeNormal:
		case e_soundTypeLoop:
			SoundEngine_UnloadEffect(sound);
			CheckSoundStatus("SoundEngine_UnloadEffect");
			break;
		case e_soundTypeVibration:
			break;
		default:
			hdError(-1, "Sound Engine: unrecognised type for file, name: %s. Aborting...", sound->filename);
			break;
			//#endif
	}
	m_sounds->Remove(sound);
	
	return true;
}


void hdSoundManager::CheckSoundStatus(const char *location)
{
	//#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
	if (m_status == 0) return;
	hdPrintf("[hdSoundManager] iPhone sound engine error %d: %s\n", (int)m_status, location);
	//#endif
}


void hdSoundManager::SetMusicMaxVolume(const float vol)
{
	m_maxMusicVolume = hdClamp(vol, 0.0f, 1.0f);
	/*
	if (m_maxMusicVolume == 0.0f)
	{
		// pause all
		hdSound *sound;
		for (int i = 0; i < m_sounds->GetItemCount(); ++i)
		{
			sound = m_sounds->GetItems()[i];
			
			if (sound->type == e_soundTypeBackground || 
				sound->type == e_soundTypeBackgroundLooping)
			{
				StopSound(sound);
			}
		}
	}*/
}



void hdSoundManager::SetSoundMaxVolume(const float vol)
{
	m_maxSoundVolume = hdClamp(vol, 0.0f, 1.0f);
}


void hdSoundManager::SetVibrationsOn()
{
	m_playVibrations = true;
}


void hdSoundManager::SetVibrationsOff()
{
	m_playVibrations = false;
}
