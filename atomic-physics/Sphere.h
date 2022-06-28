#pragma once
#include "pch.h"
#include "Drawable.h"



class Sphere : public Drawable
{
public:
	Sphere(std::shared_ptr<MoveLookController> mlc) noexcept;
	Sphere(const Sphere&) = delete;
	void operator=(const Sphere&) = delete;

	void Update() const noexcept;
	void Draw() const noexcept_release_only;

private:
	void UpdateModelViewProjectionBuffer() const;
};