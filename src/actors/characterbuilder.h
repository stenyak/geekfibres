#ifndef CHARACTERBUILDER_H
#define CHARACTERBUILDER_H

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <fstream>
#include <dirent.h>
#include "../tools/config.h"
#include "../tools/sonido.h"
#include "../tools/listaimagenes.h"

using namespace std;
typedef struct {
	string actionName;
	string actionAnimationPath; // location of the animation directory

	ListaImagenes * animationImages; // images of the animation;

	int numberOfFrames;
	bool imagesLoaded;	    // indicates if the images have already been loaded
	
	bool doesFire;		    // these three indicate if this character does those things 
	bool doesMeleeAttack;	// in this animation
	bool doesJump;
	bool doesDie;		
	bool doesSound;		    // indicates if a sound file must be played
	
	int bboxXOffset;	    // animations have an offset between the image and the
	int bboxYOffset;	    // bounding box used in the physics engine

	bool isFacingRight;	    // indicates if this animation is facing right or left
            				// (to be able to fire and move acordingly)
	
	int frameToFire; 	    // in wich frame of the animation it must fire
	int frameToMelee; 	    // in wich frame of the animation it hurts the player
	int frameToJump;	    // in wich frame of the animation it must jump
	int frameToDie;
	int frameToSound;
	
	int shotBullets; 	    // number of bullets shot
	float dispersionX,
	      dispersionY;   	// bullets dispersion in this animation
	int jumpForce;		
	
	int accelX; 		    // horizontal acceleration for this animation

	string fileToSound;	    // sound file name to play
	Sonido * loadedSound;	// sound file loaded into memory
	
    
	//---------
	// Information for the AI, in case this character is AI-controlled.
	//---------
	float probFaceToFace;   // probability that this action takes place when 
	                        // the character is in contact with the player

	float probShotRange;    // probability that this action takes place when
       	                    // the character establishes eye-contact with the 
			                // player, at a mid-range distance

	float probNoContact;    // probability that this action takes place when
                            // the character has no idea of where the player is	
				
	float probJumpableWall; // probability that this action takes place when
            				// the character encounters a wall it can jump over
				
	float probUnjumpableWall;// probability that this action takes place
       		        		// when the character encounters a wall it is
            				// unable to jump
	
} CharacterAction;

struct CharacterData
{
	string characterName;
	
    map <string , CharacterAction *> actions;   // all the actions this character
                                                // is able to perform
    int sizex, sizey;   // bbox size
	
	int commonXOffset;  // sometimes all the animations of a character have a certain
	int commonYOffset;  // common offset. This is here to simplify calculating new
			            // offsets for new animations
	
	int updateTime;     // time it takes for the IA to think of a new state when
                        // nothing surrounding the character changes
	
};


class CharacterBuilder
{
  private:
    static map <string,CharacterData *> characterTypes;
    static CharacterAction * LoadAction(string filename);
    static CharacterData * LoadCharacter(string filename);

  public:
    
    static void LoadCharacterTypes();   // loads all character types present in 
                                        // the characters directory

    static CharacterData * GetCharacterByName(string name);
};

#endif
