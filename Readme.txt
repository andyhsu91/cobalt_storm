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
		- Mouse Click for Bombs or Bullets
		- Mouse movement for camera direction
		- Shift for Boost
		- Middle Click for Lock On/Off toggle
		- M for Pause
		
		
	- Razer Hydra
		- Left Joystick for movement
		- Right Joystick for camera
		- Right Joystick button for On/Off toggle
		- Left/Right Bumper for Bombs/Bullets


Comments:
--------------------------------------------------------------------
	- Multiplayer functionality works correctly
	  

Functionality that's been implemented:
--------------------------------------------------------------------
	- Added projectiles
	- Added camera tracking for auto lockon for opponents
	- Finished flat environment. Need to add cover.
	- Prototyped a GUI. Not merged from lack of styling.
	- Implemented a health bar.
	- Added background music and sound Effects
	- Boost
	- Added Lock On functionality
	- GUI: Health Bars, Ammo Bars
	- Animation: Walking, Shooting
	- Created Bomb Model
	- Collision callback for detecting collisions between robots
	- Sending player position, projectile positions, player healths, and button states over the network
	


Issues Encountered:
--------------------------------------------------------------------
	- Robot Model could not be created due to animation requirements and lack of experience in animating models
	- Robots walk through mountains
	- Had problems putting static images on screen for GUI
	- Every once in a while camera goes crazy in lock on mode
	- 
	

What we still have to accomplish:
--------------------------------------------------------------------
	- Time limit
	- Melee Attack (Headbutt)
	- Thinking about making a better terrain
	- Settings button exists, but we don't know what settings to make available
	- Game restart after game over
	- Time delay after game over to show dying animation
	
	
	

