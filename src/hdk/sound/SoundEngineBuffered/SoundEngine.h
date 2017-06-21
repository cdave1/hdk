/*

File: SoundEngine.h
Abstract: These functions play background music tracks, multiple sound effects,
and support stereo panning with a low-latency response.

 Version: 2.0.3 BETA    - the Stormy Productions remix
 
 PLEASE NOTE:   This code was based on Apple's 1.7 SoundEngine code available 
 from the Crash Landing demo supplied with the iPhone SDK 2.0.
 
 Apple has acknowledged significant problems with the original code and has 
 in fact removed the code from future SDKs.    Per the original source code 
 agreement, Apple grants the right to anyone to modify and reuse the software
 in any manner (see original licensing agreement below).  As such, I 
 have changed the structure of the code to better suit my needs.
 
 THIS IS NOT A DROP-IN REPLACEMENT FOR THE 1.7 SOUND ENGINE!!!  
 
 Specifically, I removed the tying of a sound effect buffer to one OpenAL source. 
 Instead, you load a sound effect, which merely creates the static buffer.  To
 play the sound effect, you then "prime" it, which assigns it to an OpenAL source.
 If you want to play multiple instances of the same sound, you just prime the sound
 multiple times.  Each time you prime a sound effect, you are returned an OpenAL
 source.   There are a maximum of 32 sources and the code assumes a source is 
 available for use if you haven't yet primed it in the past or if it is currently
 in the AL_STOPPED state.  (If you don't know what that means, feel free to ignore
 it.)  I tried to hide the OpenAL details, while at the same time exposing the source ID
 for those who want direct control of the OpenAL features for a source.
 
 By making the sounds distinct from the sources, this now allows an easy method 
 for playing multiple instances of the same sound effect without using any extra buffers.
 If you tried to play the same sound with the original 1.7 SoundEngine code, it 
 would simply stop the sound and restart it.  Not ideal.
 
 The ideal usage of this library is to first load all your sound effects once. Then,
 when you want to play a sound effect, prime it, and start it.  Once you start
 a sound, you can pretty much ignore it.  OpenAL will handle stopping it for you 
 and as long as you don't exceed the 32 simultaneous sound limit, you should be ok.
 
 This is my first pass at modifying this library.  I assume it still has bugs,
 however I thought it might be useful for others looking for a quick and (very)
 dirty solution for playing multiple instances of the same sound.
 
 My latest version of the code will always be available at:
 
 http://stormyprods.com/SoundEngine
 
 Cheers,
 Brian
 
 Below is Apple's original license:
 
Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

/*==================================================================================================
	SoundEngine.h
==================================================================================================*/
#if !defined(__SoundEngine_h__)
#define __SoundEngine_h__

//==================================================================================================
//	Includes
//==================================================================================================

//	System Includes
#include <CoreAudio/CoreAudioTypes.h>
#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#if defined(__cplusplus)
extern "C"
{
#endif

//==================================================================================================
//	Sound Engine
//==================================================================================================
/*!
    @enum SoundEngine error codes
    @abstract   These are the error codes returned from the SoundEngine API.
    @constant   kSoundEngineErrUnitialized 
		The SoundEngine has not been initialized. Use SoundEngine_Initialize().
    @constant   kSoundEngineErrInvalidID 
		The specified EffectID was not found. This can occur if the effect has not been loaded, or
		if an unloaded is trying to be accessed.
    @constant   kSoundEngineErrFileNotFound 
		The specified file was not found.
    @constant   kSoundEngineErrInvalidFileFormat 
		The format of the file is invalid. Effect data must be little-endian 8 or 16 bit LPCM.
    @constant   kSoundEngineErrDeviceNotFound 
		The output device was not found.

*/
enum {
		kSoundEngineErrUnitialized			= 1,
		kSoundEngineErrInvalidID			= 2,
		kSoundEngineErrFileNotFound			= 3,
		kSoundEngineErrInvalidFileFormat	= 4,
		kSoundEngineErrDeviceNotFound		= 5,
		kSoundEngineErrNoSourcesAvailable   = 6,
};


/*!
    @function       SoundEngine_Initialize
    @abstract       Initializes and sets up the sound engine. Calling after a previous initialize will
						reset the state of the SoundEngine, with all previous effects and music tracks
						erased. Note: This is not required, loading an effect or background track will 
						initialize as necessary.
    @param          inMixerOutputRate
                        A Float32 that represents the output sample rate of the mixer unit. Setting this to 
						0 will use the default rate (the sample rate of the device)
	@result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_Initialize(Float32 inMixerOutputRate);

/*!
    @function       SoundEngine_Teardown
    @abstract       Tearsdown the sound engine.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_Teardown();

/*!
    @function       SoundEngine_SetMasterVolume
    @abstract       Sets the overall volume of all sounds coming from the process
    @param          inValue
                        A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetMasterVolume(Float32 inValue);

/*!
    @function       SoundEngine_SetListenerPosition
    @abstract       Sets the position of the listener in the 3D space
    @param          inX
                        A Float32 that represents the listener's position along the X axis.
    @param          inY
                        A Float32 that represents the listener's position along the Y axis.
    @param          inZ
                        A Float32 that represents the listener's position along the Z axis.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetListenerPosition(Float32 inX, Float32 inY, Float32 inZ);

/*!
    @function       SoundEngine_SetListenerGain
    @abstract       Sets the gain of the listener. Must be >= 0.0
    @param          inValue
                        A Float32 that represents the listener's gain
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetListenerGain(Float32 inValue);

/*!
    @function       SoundEngine_LoadBackgroundMusicTrack
    @abstract       Tells the background music player which file to play
    @param          inPath
                        The absolute path to the file to play.
    @param          inAddToQueue
                        If true, file will be added to the current background music queue. If
						false, queue will be cleared and only loop the specified file.
    @param          inLoadAtOnce
                        If true, file will be loaded completely into memory. If false, data will be 
						streamed from the file as needed. For games without large memory pressure and/or
						small background music files, this can save memory access and improve power efficiency
	@result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_LoadBackgroundMusicTrack(const char* inPath, Boolean inAddToQueue, Boolean inLoadAtOnce);

/*!
    @function       SoundEngine_UnloadBackgroundMusicTrack
    @abstract       Tells the background music player to release all resources and stop playing.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_UnloadBackgroundMusicTrack();

/*!
    @function       SoundEngine_StartBackgroundMusic
    @abstract       Tells the background music player to start playing.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StartBackgroundMusic();

/*!
    @function       SoundEngine_StopBackgroundMusic
    @abstract       Tells the background music player to stop playing.
    @param          inAddToQueue
                        If true, playback will stop when all tracks have completed. If false, playback
						will stop immediately.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StopBackgroundMusic(Boolean inStopAtEnd);

/*!
    @function       SoundEngine_SetBackgroundMusicVolume
    @abstract       Sets the volume for the background music player
    @param          inValue
                        A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetBackgroundMusicVolume(Float32 inValue);


/*!
    @function       SoundEngine_LoadEffect
    @abstract       Loads a sound effect from a file and return an ID to refer to that effect.
    @param          inPath
                        The absolute path to the file to load.
	@param			outEffectID
						A UInt32 ID that refers to the effect.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_LoadEffect(const char* inPath, UInt32* outEffectID);

/*!
    @function       SoundEngine_UnloadEffect
    @abstract       Releases all resources associated with the given effect ID
    @param          inEffectID
                        The ID of the effect to unload.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_UnloadEffect(UInt32 inEffectID);

	
/*!
 @function       SoundEngine_StartEffect
 @abstract       Binds the sound effect buffer to a source
 @param          inEffectID
					The ID of the effect to prime.
 @param			outSourceID
					A ALuint ID that refers to the OpenAL source.
 @result         A OSStatus indicating success or failure.
 */
OSStatus  SoundEngine_PrimeEffect(UInt32 inEffectID, ALuint *outSourceID);
	
	
/*!
    @function       SoundEngine_StartEffect
    @abstract       Starts playback of a source
    @param          sourceID
                        The ID of the source to start.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StartEffect(ALuint sourceID);

/*!
    @function       SoundEngine_StopEffect
    @abstract       Stops playback of a source
	@param          sourceID
						The ID of the source to stop.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StopEffect(ALuint sourceID);

/*!
    @function       SoundEngine_Vibrate
    @abstract       Tells the device to vibrate
*/
#if TARGET_OS_IPHONE
	#define SoundEngine_Vibrate() AudioServicesPlaySystemSound(kSystemSoundID_Vibrate)
#endif

/*!
    @function       SoundEngine_SetEffectPitch
    @abstract       Applies pitch shifting to an effect
	@param          sourceID
						The ID of the source to adjust.
    @param          inValue
                        A Float32 that represents the pitch scalar, with 1.0 being unchanged. Must 
						be greater than 0.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetEffectPitch(ALuint sourceID, Float32 inValue);

/*!
    @function       SoundEngine_SetEffectVolume
    @abstract       Sets the volume for an effect
	@param          sourceID
						The ID of the source to adjust.
    @param          inValue
                        A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetEffectLevel(ALuint sourceID, Float32 inValue);

/*!
    @function       SoundEngine_SetEffectPosition
    @abstract       Tells the engine whether a given effect should loop when played or if it should
					play through just once and stop.
	@param          sourceID
						The ID of the source to adjust.
    @param          inX
                        A Float32 that represents the effect's position along the X axis. Maximum distance
						is 100000.0 (absolute, not per axis), reference distance (distance from which gain 
						begins to attenuate) is 1.0
    @param          inY
                        A Float32 that represents the effect's position along the Y axis. Maximum distance
						is 100000.0 (absolute, not per axis), reference distance (distance from which gain 
						begins to attenuate) is 1.0
	@param          inZ
                        A Float32 that represents the effect's position along the Z axis. Maximum distance
						is 100000.0 by default (absolute, not per axis), reference distance (distance from 
						which gain begins to attenuate) is 1.0
	@result         A OSStatus indicating success or failure.
*/
OSStatus	SoundEngine_SetEffectPosition(ALuint sourceID, Float32 inX, Float32 inY, Float32 inZ);

/*!
   @function       SoundEngine_SetEffectsVolume
   @abstract       Sets the overall volume for the effects
   @param          inValue
                       A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
   @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetEffectsVolume(Float32 inValue);

/*!
   @function       SoundEngine_SetMaxDistance
   @abstract       Sets the maximum distance for effect attenuation. Gain will attenuate between the
				   reference distance and the maximum distance, after which gain will be 0.0
   @param          inValue
                       A Float32 that represents the level. Must be greater than 0.0.
   @result         A OSStatus indicating success or failure.
*/
OSStatus	SoundEngine_SetMaxDistance(Float32 inValue);

/*!
   @function       SoundEngine_SetReferenceDistance
   @abstract       Sets the distance at which effect gain attenuation begins. It will attenuate between
				   the reference distance and the maximum distance. Sounds closer than the reference
				   distance will have no attenuation applied
   @param          inValue
                       A Float32 that represents the level. Must be greater than 0.0.
   @result         A OSStatus indicating success or failure.
*/
OSStatus	SoundEngine_SetReferenceDistance(Float32 inValue);

#if defined(__cplusplus)
}
#endif

#endif
