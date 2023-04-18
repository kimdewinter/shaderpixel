#ifndef OWNER_H_INCLUDED
#define OWNER_H_INCLUDED

#include "Model.h"
#include "Renderer.h"
#include "Gui.h"

class ModelOwner
{
public:
	ModelOwner(std::string const name, Model model, Renderer &renderer, Gui &gui) noexcept;
	~ModelOwner() noexcept;

	// owned objects
	std::string const name;
	Model model;

	// users of owned objects
	Renderer &renderer;
	Gui &gui;
};

class ShaderOwner
{
public:
	ShaderOwner(std::string const name, Shader model, Renderer &renderer) noexcept;
	~ShaderOwner() noexcept;

	// owned objects
	std::string const name;
	Shader shader;

	// users of owned objects
	Renderer &renderer;
};

#endif
