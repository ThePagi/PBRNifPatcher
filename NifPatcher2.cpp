// NifPatcher2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "nifly/src/NifFile.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cctype>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;
using namespace std::filesystem;

std::string str_tolower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		// static_cast<int(*)(int)>(std::tolower)         // wrong
		// [](int c){ return std::tolower(c); }           // wrong
		// [](char c){ return std::tolower(c); }          // wrong
		[](unsigned char c) { return std::tolower(c); } // correct
	);
	return s;
}

bool set_pbr_textures(NifFile& nif, json settings) {
	auto modified = false;
	for (const auto shape : nif.GetShapes())
	{
		auto paths = nif.GetTexturePathRefs(shape);
		if (paths.size() < 1)
			continue;
		const auto shader = nif.GetShader(shape);
		if (!shader)
			continue;
		const auto bslsp = dynamic_cast<BSLightingShaderProperty*>(shader);
		if (!bslsp)
			continue;
		auto tex_path = str_tolower(paths[0].get());
		if (tex_path.length() < 4)
			continue;
		tex_path.pop_back(); // remove ".dds"
		tex_path.pop_back();
		tex_path.pop_back();
		tex_path.pop_back();
		for (auto& element : settings) {
			//std::cout << element << '\n';
			if (!tex_path.ends_with(element["texture"]))
				continue;
			modified = true;
			if (element.contains("delete") && element["delete"]) {
				// delete this fkin mesh bro
				nif.DeleteShape(shape);
				break;
			}
			if (element.contains("smooth_angle")) {
				nif.CalcNormalsForShape(shape, true, true, element["smooth_angle"]);
			}
			if (element.contains("vertex_colors")) {
				shape->SetVertexColors(element["vertex_colors"]);
				if (element["vertex_colors"])
					bslsp->shaderFlags2 |= SLSF2_VERTEX_COLORS;
				else
					bslsp->shaderFlags2 &= ~SLSF2_VERTEX_COLORS;
			}
			if (element.contains("pbr") && !element["pbr"])
				continue;

			std::string empty_path = "";
			auto diffuse = tex_path + ".dds";
			nif.SetTextureSlot(shape, diffuse, 0);
			auto normal = tex_path + "_n.dds";
			nif.SetTextureSlot(shape, normal, 1);
			if (element.contains("emissive") && element["emissive"]) {
				auto glow = tex_path + "_g.dds";
				nif.SetTextureSlot(shape, glow, 2);
			}
			else {
				nif.SetTextureSlot(shape, empty_path, 2);
			}
			if (element.contains("parallax") && element["parallax"]) {
				auto parallax = tex_path + "_p.dds";
				nif.SetTextureSlot(shape, parallax, 3);
			}
			else {
				nif.SetTextureSlot(shape, empty_path, 3);
			}
			nif.SetTextureSlot(shape, empty_path, 4); // unused
			auto rmaos = tex_path + "_rmaos.dds";
			nif.SetTextureSlot(shape, rmaos, 5);
			nif.SetTextureSlot(shape, empty_path, 6); // unused
			if ((element.contains("subsurface_foliage") && element["subsurface_foliage"])
				|| (element.contains("subsurface") && element["subsurface"])) {
				auto subsurface = tex_path + "_s.dds";
				nif.SetTextureSlot(shape, subsurface, 7);
			}
			else {
				nif.SetTextureSlot(shape, empty_path, 7);
			}


			// revert to default shader type, remove flags used in other types
			bslsp->bslspShaderType = BSLSP_DEFAULT;
			bslsp->shaderFlags1 &= ~SLSF1_ENVIRONMENT_MAPPING;
			bslsp->shaderFlags1 &= ~SLSF1_FACEGEN_RGB_TINT;
			bslsp->shaderFlags1 &= ~SLSF1_PARALLAX;
			bslsp->shaderFlags1 &= ~SLSF1_EXTERNAL_EMITTANCE;
			bslsp->shaderFlags2 &= ~SLSF2_GLOW_MAP;
			bslsp->shaderFlags2 &= ~SLSF2_BACK_LIGHTING;
			bslsp->shaderFlags2 &= ~SLSF2_MULTI_LAYER_PARALLAX;

			bslsp->shaderFlags2 |= SLSF2_UNUSED01; // "PBR FLAG"
			// pbr shader switch
			if (element.contains("subsurface_foliage") && element["subsurface_foliage"] && element.contains("subsurface") && element["subsurface"]) {
				cout << "Error: Subsurface and foliage shader chosen at once, undefined behavior!" << endl;
			}
			if (element.contains("subsurface_foliage") && element["subsurface_foliage"]) {
				bslsp->shaderFlags2 |= SLSF2_SOFT_LIGHTING;
			}
			else {
				bslsp->shaderFlags2 &= ~SLSF2_SOFT_LIGHTING;
			}
			if (element.contains("subsurface") && element["subsurface"]) {
				bslsp->shaderFlags2 |= SLSF2_RIM_LIGHTING;
			}
			else {
				bslsp->shaderFlags2 &= ~SLSF2_RIM_LIGHTING;
			}
			
			// texture scale values
			if (element.contains("specular_level")) {
				shader->SetGlossiness(element["specular_level"]);
			}
			if (element.contains("subsurface_color") && element["subsurface_color"].size() > 2) {
				shader->SetSpecularColor(Vector3(element["subsurface_color"][0], element["subsurface_color"][1], element["subsurface_color"][2]));
			}
			if (element.contains("roughness_scale")) {
				shader->SetSpecularStrength(element["roughness_scale"]);
			}
			if (element.contains("subsurface_opacity")) {
				bslsp->softlighting = element["subsurface_opacity"];
			}
			if (element.contains("displacement_scale")) {
				bslsp->rimlightPower = element["displacement_scale"];
			}
		}
	}
	return modified;
}

int main(int argc, char* argv[])
{
	bool overwrite = argc > 1 && argv[1][0] == '-' && argv[1][1] == 'o';
	json j;
	try {
		std::ifstream f("settings.json");
		j = json::parse(f);
	}
	catch (json::parse_error& ex)
	{
		std::cerr << "Json parse error at byte " << ex.byte << std::endl;
		cout << "Error, quitting!" << endl;
		getchar();
		return 1;
	}
	for (auto& element : j) {
		element["texture"] = str_tolower(element["texture"]).insert(0, 1, '\\');
	}
	auto save_options = NifSaveOptions();
	save_options.optimize = false;
	save_options.sortBlocks = false;
	for (recursive_directory_iterator i("."), end; i != end; ++i) {
		if (i->path().string().starts_with(".\\output"))
			continue;
		if (!is_directory(i->path()) && i->path().extension().compare(".nif") == 0) {
			//cout << "Processing " << i->path() << "\n";
			NifFile nif;
			if (nif.Load(i->path()) == 0) {
				if (set_pbr_textures(nif, j)) {
					cout << "Modified " << i->path() << "\n";
					path out_path;
					if (overwrite)
						out_path = path(".") / path(i->path().lexically_normal());
					else
						out_path = path(".\\output") / path(i->path().lexically_normal());
					create_directories(out_path.parent_path());
					if (nif.Save(out_path, save_options) != 0) {
						cout << "Error saving " << out_path << "\n";
					}
				}
			}
			else
				cout << "Error opening " << i->path() << "\n";
		}
	}
	cout << endl << "Finished!" << endl;
	getchar();
	return 0;
}
