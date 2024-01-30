This is a tool for modifying Skyrim SE/AE .nif files to support the WIP True PBR shaders. The modification can be also done by hand, but this tool can batch modify all meshes according to your specification.

# Usage
* You can build the tool with Visual Studio and C++20. Prebuilt executable available soon (or now on Community Shaders Discord).
* Specify the base name of the textures you have PBR files for (and other available parameters) in the settings.json. The "parallax" and "emissive" options should be set to true if you have the parallax and emissive textures. "subsurface" or "subsurface_foliage" turns on the corresponding shading (choose none or one) and requires a subsurface color texture.
* More on the material specification at https://github.com/doodlum/skyrim-community-shaders/wiki/True-PBR
* Place the settings.json file in the same folder as the executable. Place your .nif files in the same folder or any subfolders.
* Run the executable. The modified .nif files will appear in an "output" folder. You can specify the -o parameter to override the original files, but this is not recommended -- as of now, even in Mod Organizer 2 it OVERWRITES THE ORIGINAL FILES instead of placing them into the overwrite folder.
* You can check NifPatcher2.cpp if you are unsure about the program logic. 
