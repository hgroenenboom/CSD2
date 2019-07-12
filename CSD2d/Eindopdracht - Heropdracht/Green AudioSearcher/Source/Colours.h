/*
  ==============================================================================

    Colours.h
    Created: 12 Jul 2019 10:22:40pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

namespace Green_AudioSearcher {
	struct Kleur {
	public:
		Colour greenColour = Colours::lightgreen.darker(0.6f);
		Colour aquaColour = Colours::aqua;
		Colour purpleColour = Colours::purple.darker(0.7f).withSaturation(0.4f);
	};
}