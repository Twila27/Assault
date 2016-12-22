To edit the pixel-map of the level, ensure it's still named "Level1.png" in the Data/Images/Maps folder:
* Brown Pixel = Impassable Brick Wall
* White Pixel = Spawn Location (will drop the player in at the first white pixel it sees)
* Gray Pixel = Turret Enemy
* Black Pixel = Tank Enemy

Be wary that though the map can be generated without a solid border of brick wall, 
when an enemy or the player are found outside the program will detect and display an error.