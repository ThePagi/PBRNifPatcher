This is a tool for modifying Skyrim SE/AE .nif files to support the WIP True PBR shaders. The modification can be also done by hand, but this tool can batch modify all meshes according to your specification.

# Usage
* You can build the tool with Visual Studio and C++20. Prebuilt executable available soon (or now on Community Shaders Discord).
* Specify the base name of the textures you have PBR files for (and other available parameters) in the settings.json. The "parallax" and "emissive" options should be set to true if you have the parallax and emissive textures. "subsurface" or "subsurface_foliage" turns on the corresponding shading (choose none or one) and requires a subsurface color texture.
* More on the material specification at https://github.com/doodlum/skyrim-community-shaders/wiki/True-PBR
* Place the settings.json file in the same folder as the executable. Place your .nif files in the same folder or any subfolders.
* Run the executable. The modified .nif files will appear in an "output" folder. You can specify the -o parameter to override the original files, but this is not recommended -- as of now, even in Mod Organizer 2 it OVERWRITES THE ORIGINAL FILES instead of placing them into the overwrite folder.
* You can check NifPatcher2.cpp if you are unsure about the program logic. 

# Texture paths
* Texture paths are made to follow Skyrim naming as much as possible. The path for diffuse texture remains the same, other textures have a _suffix.
* Diffuse: texturename.dds
* Normal: texturename_n.dds
* Emissive (glow): texturename_g.dds
* Parallax (height, displacement): texturename_p.dds
* RMAOS (roughness, metallic, ambient oclusion, specular): texturename_rmaos.dds
* Subsurface color: texturename_s.dds

# Additional settings
* "pbr": false -> only does non-pbr modifications (as below)
* "vertex_colors": true/false -> enable or disable vertex colors, useful if the mesh used vertex colors to change the colors of the old textures
* "smooth_angle": 0.0-180.0 -> smooths the normals (and removes doubled vertices) where the angle is less extreme (flatter) than the given number in degrees, useful when sharp edges look bad with high resolution textures
* "delete": true -> removes the matched mesh, useful for example for ugly decals that even PBR cannot save
* "path_contains": "dwemer" -> select mesh based on a string anywhere in texture path, doesn't work for setting PBR texture names
* "cubemap": "textures\\cubemaps\\copper_e.dds" -> sets a cubemap path
* "env_map_scale": 2.0: sets the environment mapping scale
* "env_mapping": true: forces the shader type to env mapping, does nothing if false