Project: Assault
Milestone: In-Class Checkpoint #3
Name: Benjamin Gibson
Class: Software Development I
Instructor: Squirrel Eiserloh


//-----------------------------------------------------------------------------
Known Issues
	1. AI angles seem liberal despite ReactToPlayer() code in EnemyTank/EnemyTurret.cpp using literal angle amounts.
		I find their overzealous fire adds to the game by letting you see (and have to dodge) more bullets more often (and can probably accept the point loss).
		I think the cause is the use of CalcShortestAngularDisplacement in comparing against the desired angle range.
	2. The Map::ENEMY_RANGE_IN_TILES is currently set low relative to the spec's suggestion for the sake of the smaller map. 
		Able to flesh this out with map loading if desired in the future.
	3. Fade-in on respawn and bonus features unimplemented (fade-in on death was implemented).
		Primarily cut due to time management.
	
Build Notes
	Because of the requirement on Textures (to be under Data/Images), 
	.exe will crash if it cannot find the directory Data/Images in its folder.
	Sound will likewise not play if the .exe cannot find the directory Data/Audio in its folder.

	
//-----------------------------------------------------------------------------
How to Use
	1: Keyboard
		W: Forward
		S: Backward
		A/D: Turn
		Spacebar: Fire
		P: Pause/Respawn
		ESC: Return (Playing > Paused > Title > Exit Game)
	2: Xbox Controller
		Both Sticks Forward: Forward
		Both Sticks Backward: Backward
		Both Sticks Opposite: Turn
		Right Trigger: Fire
		Start: Pause/Respawn
		Back: Return (Playing > Paused > Title > Exit Game)
	
//-----------------------------------------------------------------------------
Attributions
	Ogrebane: Stone Tile				http://opengameart.org/users/ogrebane
	Gwes: Grass Tile					http://opengameart.org/users/gwes
	Sound: Guildhall SFX
	
//-----------------------------------------------------------------------------
Deep Learning
	Once again, as in Asteroids, I feel great tension while staring at the list of bonus features. 
	My first thought was that, this time around, I was going to focus on making it the portfolio piece for SD1.
	However, with it being the final week of the first module, I have no time for that goal.
	In general, I'm worried about Guildhall because the program's so all-consuming. 
	There's not really time to spend with friends, and not really time to spend on other personal interests. 
	That thought doesn't go away, because I don't know that the rest of life in the industry will be anything different. 
	Applying this mindset, I will approach bonus features assuming I will not be able to get to them before the assignment deadline.
	Yet, if I continue in this way, what sets me apart from everyone else's unique features? Where does that leave me, if I'm not implementing any?
	In general, I struggle comparing myself to the other SDs, because it seems so difficult to professionally distinguish your skills amongst those who are taught the same skill set.
	Hell, maybe this pushing back on the juice will become what sets me apart. 
	But if I only ever want to ensure the core program works, not something awesome on top of it, I'm definitely less presentable in a portfolio.