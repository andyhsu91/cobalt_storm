#include "Sound.h"
#include <string>
#include <string.h>
#include <stdio.h>
#include <iostream>

//list of all allowed sound effects
enum soundEffects{
	Idle, Shoot1, Shoot2, Walk, Run, Success, Failure, Background, Punch, enumElementCount
};

//

//Sound Effect Ids
int chunkIds[enumElementCount];


std::vector<Mix_Chunk*> SoundList;
Mix_Music* backgroundMusic;


static bool mute = false;
bool init = false;
bool debug = false;

Sound::Sound() {
	if(debug){ std::cout<<"Entered SoundManager Constructor"<<std::endl;}
	
	init();
	
	if(debug){ std::cout<<"Exiting SoundManager Constructor"<<std::endl;}
}
 
Sound::~Sound() { 
	if(debug){std::cout<<"Entering SoundManager Destructor"<<std::endl;}
	
	Cleanup();
	Mix_CloseAudio();
	
	if(debug){std::cout<<"Exiting SoundManager Destructor"<<std::endl;}
}
 
void Sound::resetIDs(void){
	for(int i=0; i<enumElementCount; i++){
		chunkIds[i]=-1;
	}
	
}

void Sound::init(void){

	int errorCode = Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
	if(errorCode != 0){
		std::cout<<"Error: Could not open SDL Mixer.\n"<<Mix_GetError()<<std::endl;
	}
	resetIDs();
	backgroundMusic=NULL;
	loadSounds();
	
}

std::string getStringFromEnum(int soundEffect)
{
  switch (soundEffect)
  {
  	case Shoot1: 	return "media/sounds/robot/shoot1.wav";
		
	case Shoot2: 	return "media/sounds/robot/shoot1.wav";
		
	case Walk: 		return "media/sounds/robot/footstep2.wav";

	case Run: 		return "media/sounds/robot/footstep2.wav";

	case Success: 	return "media/sounds/robot/success1.mp3";

	case Failure: 	return "media/sounds/robot/failure1.wav";

	case Background:return "media/sounds/background/beepboop3.wav";

	case Punch: 	return "media/sounds/robot/punch1.wav";
	
	default: 		return "Invalid";
	
  };
}

void Sound::loadSounds(void){
	//load default sound effects
	if(debug){
		std::cout<<"Entered loadSounds()"<<std::endl;
	}
	
	for(int i=0; i<enumElementCount; i++){
		if(chunkIds[i] < 0){
			const char* filename = getStringFromEnum(i).c_str();
			int errCode = strcmp(filename, "Invalid");
			if(errCode != 0){
				chunkIds[i] = LoadChunk(filename);
			}
		}
	}
	
	
	if(backgroundMusic==NULL){
			const char* background_filename = getStringFromEnum(Background).c_str();
			int errCode = strcmp(background_filename, "Invalid");
			if(errCode != 0){
				backgroundMusic=Mix_LoadMUS(background_filename);
			}
	}
	if(debug){
		std::cout<<"Exiting loadSounds()"<<std::endl;
		
	}
}

Mix_Music* Sound::LoadMusic(const char* File){
	//File type can be wav, mod, midi, ogg, mp3, or flac
	if(debug){std::cout<<"Entered LoadMusic()"<<std::endl;}
	
	if(File==NULL){
		std::cout<<"Error in LoadMusic(), File==NULL"<<std::endl;
		return NULL;
	}	
		
	Mix_Music* musicPtr = Mix_LoadMUS(File);
	
	if(musicPtr==NULL){
		std::cout<<"Error in MixLoadMUS(), could not load file."<<std::endl;
	}
	
	if(debug){ std::cout<<"Exiting LoadMusic()"<<std::endl;}
	
	return musicPtr;
}

int Sound::LoadChunk(const char* File) {
	//File type can be wav, mod, midi, ogg, or mp3
	if(debug){ std::cout<<"Entered LoadChunk()"<<std::endl;}
    Mix_Chunk* TempSound = NULL;
 	if(File==NULL){
 		std::cout<<"Error: File=NULL."<<std::endl;
 		if(debug){ std::cout<<"Exiting LoadChunk()"<<std::endl;}
        return -1;
    }
    //Mix_LoadWav name is misleading, it also supports other filetypes
    if((TempSound = Mix_LoadWAV(File)) == NULL) {
    	std::cout<<"Error: "<<File<<" cound not be loaded."<<std::endl;
    	if(debug){ std::cout<<"Exiting LoadChunk()"<<std::endl;}
        return -1;
    }
 	if(debug){std::cout<<"SoundList.size() Before:"<<SoundList.size()<<std::endl;}
    SoundList.push_back(TempSound);
    std::cout<<"Sound: "<<File<<" has been loaded."<<std::endl;
 	if(debug){std::cout<<"SoundList.size() After:"<<SoundList.size()<<std::endl;}
 	if(debug){ std::cout<<"Exiting LoadChunk()"<<std::endl;}
 	
 	//chunkID
    return (SoundList.size() - 1);
    
}
 
void Sound::Cleanup() {
    for(int i = 0;i < SoundList.size();i++) {
        Mix_FreeChunk(SoundList[i]);
    }
    SoundList.clear();
}
 
void Sound::fadeInLoopMusic(Mix_Music* music, int loops, int ms){
	//ms= milliseconds it takes to fade in
	if(music==NULL){
		std::cout<<"Error in LoopMusic(), *music=NULL."<<std::endl;
		return;
	}
	if(ms<0){
		std::cout<<"Error in LoopMusic(), ms < 0."<<std::endl;
		return;
	}
	
	int errorCode = Mix_FadeInMusic(music, loops, ms);
	if(errorCode!=0){
		std::cout<<"Error in Mix_FadeInMusic(): "<< Mix_GetError() << std::endl;
		return;
	}
}

void Sound::LoopMusic(Mix_Music* music, int loops){
	//loops==-1 plays music infinitely
	if(music==NULL){
		std::cout<<"Error in LoopMusic(), *music=NULL."<<std::endl;
		return;
	}	
	
	int errorCode = Mix_PlayMusic(music, loops);
	if(errorCode!=0){
		std::cout<<"Error in Mix_PlayMusic(): "<< Mix_GetError() << std::endl;
		return;
	}
}
 
void Sound::PlaySound(int chunkID) {
	
	if(debug){std::cout<<"Entered PlaySound("<<chunkID<<")"<<std::endl;}
    if(chunkID < 0){
    	std::cout<<"Invalid. ID < 0"<<std::endl;
    	return;
    }
    if(chunkID >= SoundList.size()){ 
    	std::cout<<"Invalid. ID >= SoundList.size() = "<<SoundList.size()<<std::endl;
    	return;
    }
    if(SoundList[chunkID] == NULL){
    	std::cout<<"Empty SoundList."<<std::endl;
    	return;
    }
    Mix_PlayChannel(-1, SoundList[chunkID], 0);
    if(debug){std::cout<<"Exiting PlaySound()"<<std::endl;}
}

void Sound::toggleMute(void){
	setMute(!mute);
}

void Sound::setMute(bool val){
	if(val==true){
		stopAllAudio();
	}
	if(val==false){
		playBackground(-1);
	}
	
	mute=val;
}

void Sound::playFailure(void){
	if(!mute){

		if(debug){std::cout<<"entered playFailure()"<<std::endl;}

		PlaySound(chunkIds[Failure]);
		if(debug){std::cout<<"exiting playFailure()"<<std::endl;}
	}
}

void Sound::playSuccess(void){
	if(!mute){

		if(debug){std::cout<<"entered playSuccess("<<chunkIds[Success]<<")"<<std::endl;}
		PlaySound(chunkIds[Success]);
		if(debug){std::cout<<"Exiting playSuccess()"<<std::endl;}
	}
}
 
void Sound::playGun(void){
	if(!mute){
		PlaySound(chunkIds[Shoot1]);
	}
}


void Sound::playBackground(int numLoops){
	playBackground(numLoops, 0);
}

void Sound::playBackground(int numLoops, int ms){
	if(!mute){
		if(debug){std::cout<<"entered playBackground()"<<std::endl;}
		if(backgroundMusic==NULL){
			const char* background_filename = getStringFromEnum(Background).c_str();
			int errCode = strcmp(background_filename, "Invalid");
			if(errCode != 0){
				backgroundMusic=Mix_LoadMUS(background_filename);
			}
			//backgroundMusic=Mix_LoadMUS(BACKGROUND.c_str());
		}
		fadeInLoopMusic(backgroundMusic, numLoops, ms);
		if(debug){std::cout<<"exiting playBackground()"<<std::endl;}
	}
} 
 
void Sound::pauseSounds(void){
	//can't pause background music, only play and stop.
	//pause sound effects
	Mix_Pause(-1);
}

void Sound::resumeSounds(void){
	//resume sound effects
	Mix_Resume(-1);
}

void Sound::stopMusic(void)
{  	//stop background music
	if(backgroundMusic!=NULL){
		Mix_FreeMusic(backgroundMusic);
	}
   	backgroundMusic=NULL; //backgroundMusic now points to garbage, so set it to null
} 

void Sound::stopSounds(void)
{  	//stop sound effects
	Mix_HaltChannel(-1); 
}

void Sound::stopAllAudio(void)
{  	
   	stopMusic();
   	stopSounds();
   	
}
