#ifndef _SoundManager_H_
#define _SoundManager_H_
 
#include <SDL/SDL.h>
//#include <SDL.h>
#include <SDL/SDL_mixer.h>
#include <vector>
 #include <string>
#include <string.h>
#include <stdio.h>
#include <iostream>

class Sound {
    public:   
        Sound();
        virtual ~Sound();
        //list of all allowed sound effects
        enum soundEffects{
          Shoot1, Shoot2, Walk, Run, Success, Failure, Background, Thud, Die, soundEnumCount //soundEnumCount should always be the last
        };
        std::vector<Mix_Chunk*> SoundList;
        Mix_Music* LoadMusic(const char* File);
        int LoadChunk(const char* File);
        void fadeInLoopMusic(Mix_Music* music, int loops, int ms);
        void LoopMusic(Mix_Music* music, int loops);
        void playSoundFromEnum(int soundEffectEnum);
        void playSoundFromChunk(int chunkID);
        void toggleMute(void);
        void setMute(bool val);
        void playFailure(void);
      	void playSuccess(void);
      	void playGun(void);
      	void playBackground(int numOfLoops);
      	void playBackground(int numOfLoops, int ms);
      	void pauseSounds(void);
      	void resumeSounds(void);
      	void stopMusic(void);
      	void stopSounds(void);
        void stopAllAudio(void);
        
	private:
    //Sound Effect Ids
    int chunkIds[soundEnumCount];
		void resetIDs(void);
		void init(void);
		void Cleanup();
		void loadSounds(void);
    std::string getSoundFileFromEnum(int soundEffectEnum);
};
 
#endif
