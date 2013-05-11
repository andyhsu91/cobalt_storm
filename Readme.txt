--------------------------------------------------------------------
--------------------------------------------------------------------
   ______      __          ____       _____ __                     
  / ____/___  / /_  ____ _/ / /_     / ___// /_____  _________ ___ 
 / /   / __ \/ __ \/ __ `/ / __/     \__ \/ __/ __ \/ ___/ __ `__ \
/ /___/ /_/ / /_/ / /_/ / / /_      ___/ / /_/ /_/ / /  / / / / / /
\____/\____/_.___/\__,_/_/\__/     /____/\__/\____/_/  /_/ /_/ /_/ 
--------------------------------------------------------------------
--------------------------------------------------------------------                                                                   


Members and EID:
--------------------------------------------------------------------
	Alejandro Weibel    (aaw738)
	Andy Hsu       		(ah28348)
	Nelson Cheng 		(nc7559)
	Benjamin Maxfield  	(bsm762)        
	Timmy Lee     		(tl7272)
	Aaron Angert  		(aa35226)
	
	
Build Instructions:
--------------------------------------------------------------------
./builtit will compile it
./cobaltstorm will run it


Controls:
--------------------------------------------------------------------
	- Keyboard
		- WASD for movement
		- O for firing projectiles
		- Mouse for forcing camera direction
	- Razer Hydra
		- Left Joystick for movement


Comments:
--------------------------------------------------------------------
	- We only have 2 sets of Hydra controllers so control implementation
	  is slower for the Hydra than the Keyboard.
	- We ended up finding rigging animation to be far harder than
	  anticipated so without a proper model, the parts slowed down.
	  

Functionality that's been implemented:
--------------------------------------------------------------------
	- Added projectiles
	- Added camera tracking for auto lockon for opponents
	- Finished flat environment. Need to add cover.
	- Prototyped a GUI. Not merged from lack of styling.
	- Implemented a health bar. Not in from lack of styling.
	- Added background music


Issues Encountered:
--------------------------------------------------------------------
	- Found animation to be harder than anticipated. No full model.
	- Custom GUI styling requires XML files outside of CEGUI to be used.
	

What we still have to accomplish:
--------------------------------------------------------------------
	- Finish modeling and animation work.
	- New sound effects for movements or attacks
	- Time limit. After limit, low health player loses.
	- Need to add collision callback from previous project to handle hits
	- Duplicate current projectile and alter size, speed, etc for new weapon

