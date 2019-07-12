/*
  ==============================================================================

    FolderManagerComponent.h
    Created: 11 Jul 2019 9:39:31pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "FolderManager.h"
#include <vector>

class FolderManagerComponent : public Component, public Button::Listener {
public:
	FolderManagerComponent(FolderManager& f) 
		: folderManager(f)
	{
		openButtons.resize(size);

		for (int i = 0; i < size; i++) {
			addAndMakeVisible(&toggles[i]);
			toggles[i].setColour(toggles[i].tickColourId, color);
			toggles[i].setColour(toggles[i].tickDisabledColourId, color);
			toggles[i].addListener(this);

			addAndMakeVisible(&folderPaths[i]);
			folderPaths[i].setText("non selected");
			folderPaths[i].setColour(color);

			addAndMakeVisible(&folderIndex[i]);
			folderIndex[i].setText(std::to_string(i));
			folderIndex[i].setColour(color);

			openButtons[i] = new DrawableButton("openButton" + std::to_string(i), DrawableButton::ButtonStyle::ImageAboveTextLabel);
			openButtons[i]->addListener(this);
			addAndMakeVisible(openButtons[i]);
		}

		setSize(width, size * rowHeight);
	}

	~FolderManagerComponent() {
		for (int i = 0; i < size; i++) {
			delete openButtons[i];
		}
	}

	void paint(Graphics& g) override {
		g.setColour(color);
		for (int i = 0; i < size; i++) {
			g.fillRect(Rectangle<float>(0, i*rowHeight, width, 1));
		}
	}

	void resized() override {
		const float border = 1.0f;
		for (int i = 0; i < size; i++) {
			auto s = Rectangle<int>(startTogglesX, i * rowHeight, toggleWidth, rowHeight);
			s.reduce(border, border);
			toggles[i].setBounds(s);

			s = Rectangle<int>(startOpenButtonsX, i * rowHeight, openButtonsWidth, rowHeight);
			s.reduce(border, border);
			openButtons[i]->setBounds(s);

			s = Rectangle<int>(folderIndexX, i * rowHeight, folderIndexWidth, rowHeight);
			s.reduce(border, border);
			folderIndex[i].setBoundingBox(Parallelogram<float>(s.toFloat()));

			s = Rectangle<int>(startFolderPathX, i * rowHeight, folderPathWidth, rowHeight);
			s.reduce(border, border);
			//folderPaths[i].setBounds(s);
			folderPaths[i].setBoundingBox(Parallelogram<float>(Point<float>(startFolderPathX, i*rowHeight), Point<float>(startFolderPathX + folderPathWidth, i * rowHeight), Point<float>(startFolderPathX, i*rowHeight + rowHeight)));
		}
	}

	void buttonClicked(Button* b) override {
		for (int i = 0; i < size; i++) {
			if (b == &toggles[i]) {
				const bool buttonState = toggles[i].getToggleState();
				folderManager.toggleFolder(i, buttonState);
			}
			if (b == openButtons[i]) {
				folderManager.openFolder(i);
				folderPaths[i].setText(folderManager.getFolderPath(i));
			}
		}
	}
private:
	static const int size = 8;
	Colour color = Colours::aqua;
	FolderManager& folderManager;
	const int rowHeight = 24;

	ToggleButton toggles[size];
	DrawableText folderPaths[size];
	DrawableText folderIndex[size];
	std::vector<DrawableButton*> openButtons;

	const int folderIndexX = 0;
	const int folderIndexWidth = 0.5f * rowHeight;
	const int startTogglesX = folderIndexX + folderIndexWidth;
	const int toggleWidth = rowHeight;
	const int startOpenButtonsX = startTogglesX + toggleWidth;
	const int openButtonsWidth = rowHeight;
	const int startFolderPathX = startOpenButtonsX + openButtonsWidth;
	const int folderPathWidth = 400;

	const int width = startFolderPathX + folderPathWidth;
};
