/*
  ==============================================================================

    LabelSlider.h
    Created: 19 Dec 2018 11:34:38pm
    Author:  Harold Groenenboom

  ==============================================================================
*/

// research: 
// structure: ++
// build state: + 
// tested: +
// optimized / cleaned: +
// functionalities: +
// JUCE need: ++

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

// Simple slider which automaticly adds a label on its left
// made for simple adding of slider objects
class LabelSlider : public Component, public Slider::Listener {
public:
	LabelSlider() {
		addAndMakeVisible(&s);
		s.addListener(this);
	}

	void paint(Graphics& g) override {
		g.setColour(Colours::white);
		g.drawText(text, getLocalBounds().removeFromLeft( (int)(0.25f * getWidth()) ), Justification::centred);
	}

	void resized() override {
		s.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, (int)(getWidth() * 0.15f), (int)(getHeight() * 0.5f) );

		auto l = getLocalBounds();
		auto lS = l.removeFromLeft( (int)(getWidth() * 0.25f) );

		s.setBounds(l);
	}

	void setSlider(float min, float max, float interval, float midVal, float initVal, String txt) {
		text = txt;
		
		s.setRange(min, max, interval);
		s.setSkewFactorFromMidPoint(midVal);
		s.setValue(initVal, juce::dontSendNotification);

		repaint();
	}

	std::function<void(float)> callBack = nullptr;
	Slider s;
private:
	void sliderValueChanged(Slider* s) override {
		if (callBack != nullptr) {
			callBack(s->getValue());
		}
	}

	String text;
};