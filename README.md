# Map Art
The schemetatics are created using [MapartCraft](https://rebane2001.com/mapartcraft/). 
The map arts are built on the end on top of glass causing the map to be transparent.
# Minecraft
The mod injects every draw sprite function and draws hapboo there instead, there is another function that runs every tick and changes the habpoo sprite to the next one if enough time has passed. The animation can be made faster by changing this time.If the speed is increased enough times the interval will become negative and will cause to game to crash.
# Portal 2
The portal gun model is made by using [This Guide](https://steamcommunity.com/sharedfiles/filedetails/?id=2960094253) by AlexAdvDev. It can be installed by putting the "pak01_dir.vpk" file in the portal folder from project files to a folder named "portal2_dlc3" in your portal2 folder
# Cursor
The cursor is made by converting the habpoo gif into a cursor file and copying it to every cursor type. Linux users can install it by copying the hapboo folder in the cursor folder to "~/.local/share/icons"
# Background
The background is animated by using a simple shell script that extracts every frame from the given file and changes the background to the next frame every 0.08 seconds.
# HabpoOS
It is not an actual OS. it's just a program that prints the habpoo, generated using a another program that reads png files. Then the program is passed as a init to linux.
# Video
The audio for the video is made by using a shell script that reads a text file and generates the audio using stream elements API. The video was edited using kdenlive.
