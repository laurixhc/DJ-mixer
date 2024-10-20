/*
  ==============================================================================

    PlaylistComponent.h
    Created: 11 Feb 2024 12:43:16pm
    Author:  Juan Carlos Henriquez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "DeckGUI.h"

#include <vector>
#include <string>
#include <fstream>

//==============================================================================
/*
*/


class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,public Button::Listener
{
public:
    PlaylistComponent(DJAudioPlayer* player,
                      AudioFormatManager &     formatManagerToUse,
                      AudioThumbnailCache &     cacheToUse, DeckGUI *    deckGUI1);
    ~PlaylistComponent() override;
    
    /** Function to paint */
    void paint (juce::Graphics&) override;
    /** Function to resized when the screen size is changed */
    void resized() override;
    
    /** Function to know the number of rows:
            output: int (number of rows
     */
    int getNumRows() override;
    
    /** Function to paint a row background*/
    void paintRowBackground(Graphics&, int rowNumber, int width, int height,bool rowIsSelected) override;
    
    /** Function to paint each cell*/
    void paintCell (Graphics &,int rowNumber,int columnId,int width, int height, bool rowIsSelected) override;
    
    /** Function to refresh the information od each cell*/
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    /** Function to make some actions when a button is clicked*/
    void buttonClicked(Button*button) override;
    
    
private:
    
    TableListBox tableComponent; // Table list box where the playlist is

    DJAudioPlayer* player; //object of the class DJAudioPlayer
    
    DeckGUI* deckGUI1; //object of the class DeckGUI
    
    std::vector<std::vector<std::string>> trackTitles;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

