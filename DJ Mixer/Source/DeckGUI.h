/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    /** Function to paint */
    void paint (Graphics&) override;
    /** Function to resized when the screen size is changed */
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    /** Function that is called when a period of time have pass (here we have things that we want change every x period of time) */
    void timerCallback() override;
    
    /** Function to draw an object of the WaveformDisplay class */
    void drawWave(URL url);

    
private:

    //We create text buttons
    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
    TextButton upVolButton{"+"};
    TextButton downVolButton{"-"};
    TextButton upSpeedButton{"+"};
    TextButton downSpeedButton{"-"};
    TextButton rewindButton{"REWIND"};
    TextButton loopButton{"LOOP"};
    
    //we create booleans
    bool isButtonClicked = false;
    bool condition = false;
    bool is_playing = false;
    
    //We create labels
    Label volumeLabel;
    Label speedLabel;
    Label posLabel;
    Label imeLabel;
    
    //We create a color object
    juce::Colour color;
  
    //We create sliders
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    juce::FileChooser fChooser{"Select a file..."}; //To select a file
    
    WaveformDisplay waveformDisplay; //Object of the class WaveformDisplay

    DJAudioPlayer* player;  //pointer of the class DJAudioPlayer
    
    AudioTransportSource transportSource;  //object of the class transportSource
    
    //To create the disk, we need rectangles and an angle
    Rectangle<float> diskBounds;
    Rectangle<float> diskBounds_ins;
    float rotationAngle = 0.0f;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
