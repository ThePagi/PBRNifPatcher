This is a tool for modifying Skyrim SE/AE .nif files to support the WIP True PBR shaders. The modification can be also done by hand, but this tool can batch modify all meshes according to your specification.

# Normal usage (you downloaded a True PBR texture mod)
1. Download the patcher.
2. Add the patcher as an executable in MO2 (or whatever you do in other mod managers), by default it patches all meshes in the Data folder.
3. Run the patcher through MO2 - configuration should already be provided by the modder and is used automatically.
4. A 'pbr_output' folder should appear in your Overwrite or in the mod you specified as output.
5. Move the 'meshes' folder outside the 'pbr_output' folder.
6. Done! If you change your meshes, delete the generated 'meshes' folder before patching, otherwise the old patched meshes will be used again!

# Modder Usage
* You can build the tool with Visual Studio and C++20. Prebuilt executable available soon (or now on Community Shaders Discord).
* Specify the base name of the textures you have PBR files for (and other available parameters) in a JSON file in the PBRNifPatcher folder (see included example config). The "parallax" and "emissive" options should be set to true if you have the parallax and emissive textures. "subsurface" or "subsurface_foliage" turns on the corresponding shading (choose none or one) and requires a subsurface color texture.
* More on the material specification at https://github.com/doodlum/skyrim-community-shaders/wiki/True-PBR
* You can check NifPatcher2.cpp if you are unsure about the program logic. 
* YOU NEED TO SHIP THE JSON CONFIG WITH YOUR TEXTURES TO ALLOW USERS TO PATCH THEIR OWN MESHES. You can ship patched meshes, but users may have mods that ovewrite them or that add new meshes using your textures.

# Texture paths
* To avoid multiple issues between vanilla and pbr textures, all pbr texture paths point to /textures/pbr/... instead of just /textures/...
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
* "emissive_scale": 1 -> sets a multiplier for the emissive color
* "emissive_color": [1,1,1,1] -> sets the emissive color which multiplies color from the emissive texture
