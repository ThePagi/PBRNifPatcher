This is a tool for modifying Skyrim SE/AE .nif files to support the True PBR shaders included in the Community Shaders mod. The modification can be also done by hand, but this tool can batch modify all meshes according to your specification.

# Normal usage (you downloaded a True PBR texture mod)
1. [Download the patcher here](https://github.com/ThePagi/PBRNifPatcher/releases) (Assets ->  PBRNifPatcher.zip). Install it as a mod if you want to patch meshes in your Skyrim, otherwise just unpack it anywhere.
2. Add the patcher as an executable in MO2 (or whatever you do in other mod managers), by default it patches all meshes in the Data/Meshes folder. If you didn't add the patcher as a mod, you can run it directly and it will go through the ./Meshes folder.
3. Run the patcher through MO2 - configuration should already be provided by the modder and is used automatically.
4. A 'pbr_output' folder should appear in your Overwrite or in the mod you specified as output.
5. Move the 'meshes' folder outside the 'pbr_output' folder.
6. Done! If you change your meshes, delete the generated 'meshes' folder before patching, otherwise the old patched meshes will be used again!

# Modder Usage
* You can download the patcher in the Releases tab or build the tool with Visual Studio and C++20.
* Create a PBRNifPatcher folder in your mod folder and a JSON file with a unique name in it.
* CHECK THE EXAMPLE JSON CONFIGS IN THE PBRNifPatcher FOLDER IN THIS REPOSITORY!
* In the JSON file: Specify the base name of the textures you have PBR files for (and other available parameters). The "parallax" and "emissive" options should be set to true if you have the parallax and emissive textures. "subsurface" or "subsurface_foliage" turns on the corresponding shading (choose none or one) and requires a subsurface color texture.
* When your config is complete, you can run the patcher the same way users do in **Normal usage** section to generate meshes for testing or for shipping.
 
* More on the material specification at https://github.com/doodlum/skyrim-community-shaders/wiki/True-PBR
* You can check NifPatcher2.cpp if you are unsure about the program logic. 
* YOU NEED TO SHIP THE JSON CONFIG WITH YOUR TEXTURES TO ALLOW USERS TO PATCH THEIR OWN MESHES. You can ship patched meshes, but users may have mods that ovewrite them or that add new meshes using your textures.

Example config entry that sets PBR with parallax for meshes using "texture_name" diffuse texture:

 `{
  "texture": "texture_name", "emissive": false, "parallax": true, "subsurface_foliage":false, "subsurface": false,  "specular_level" : 0.04, "subsurface_color": [1,1,1], "roughness_scale" : 1, "subsurface_opacity" : 1, "displacement_scale" : 0.35
 }`

# Texture paths
* To avoid multiple issues between vanilla and pbr textures, all pbr texture paths point to /textures/pbr/... instead of just /textures/...
* Texture paths are made to follow Skyrim naming as much as possible. The path for diffuse texture remains the same, other textures have a _suffix.
* Diffuse: texturename.dds
* Normal: texturename_n.dds
* Emissive (glow): texturename_g.dds
* Parallax (height, displacement): texturename_p.dds
* RMAOS (roughness, metallic, ambient oclusion, specular): texturename_rmaos.dds
* Multilayer coat normal+roughness: texturename_cnr.dds
* Subsurface color/Multilayer coat color: texturename_s.dds

# Multilayer parallax settings example:
* "multilayer": true
* "coat_strength": 1.0,
* "coat_roughness": 1.0,
* "coat_specular_level": 0.04,
* "coat_diffuse": true,
* "coat_parallax": true,
* "coat_normal": true,

# Additional settings
* "path_contains": "dwemer" -> select mesh based on a string anywhere in texture path, does NOT set PBR texture paths unless "texture" also matches
* "nif_filter": "book04a" -> skips the current .nif file unless its path contains the given text
* "rename": "new_texture" -> renames the matched texture to a new name
* "pbr": false -> only does non-pbr modifications (as below)
* "vertex_colors": true/false -> enable or disable vertex colors, useful if the mesh used vertex colors to change the colors of the old textures
* "smooth_angle": 0.0-180.0 -> smooths the normals (and removes doubled vertices) where the angle is less extreme (flatter) than the given number in degrees, useful when sharp edges look bad with high resolution textures
* "delete": true -> removes the matched mesh, useful for example for ugly decals that even PBR cannot save
* "cubemap": "textures\\cubemaps\\copper_e.dds" -> sets a cubemap path
* "env_map_scale": 2.0: sets the environment mapping scale
* "env_mapping": true: forces the shader type to env mapping, does nothing if false
* "emissive_scale": 1 -> sets a multiplier for the emissive color
* "emissive_color": [1,1,1,1] -> sets the emissive color which multiplies color from the emissive texture
* "uv_scale": 2 -> sets the uv scale, good for objects with badly scaled uvs such as rocks
* "auto_uv": 2048 -> experimental, scales the uvs to keep the pixel density constant on average
* "slotX": "path" -> (X = 1..8) explicitly sets the whole texture path for a slot, example: "slot8": "textures\\pbr\\test\\coat.dds"

* "lock_diffuse": true -> skips changing the paths in this texture slot
* "lock_normal": true -> skips changing the paths in this texture slot
* "lock_emissive": true -> skips changing the paths in this texture slot
* "lock_parallax": true -> skips changing the paths in this texture slot
* "lock_rmaos": true -> skips changing the paths in this texture slot
* "lock_subsurface": true -> skips changing the paths in this texture slot
