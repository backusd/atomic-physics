#pragma once


namespace Constants
{
	// Default radius for every element (all values in nanometers)
	// Values taken from here: https://en.wikipedia.org/wiki/Atomic_radii_of_the_elements_(data_page)
	const float AtomicRadii[11] = {
		0.0f,	// Invalid value to take up the 0 index spot
		0.025f,	// Hydrogen
		0.120f,	// Helium
		0.145f,	// Lithium
		0.105f, // Beryllium
		0.085f, // Boron
		0.070f, // Carbon
		0.065f, // Nitrogen
		0.060f, // Oxygen
		0.050f, // Flourine
		0.160f  // Neon
	};

	// Create a constant for the radius to use when rendering in the Ball & Stick state
	// The value is half that of the Hydrogen radius
	//const float BallAndStickAtomRadius = 0.0125f;

	// Constant for rendering bond cylinder when in Ball & Stick state
	//const float BallAndStickBondRadius = AtomicRadii[1] / 6.0f;

}