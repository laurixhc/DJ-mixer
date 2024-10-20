/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    //We make visible all buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(upVolButton);
    addAndMakeVisible(downVolButton);
    addAndMakeVisible(upSpeedButton);
    addAndMakeVisible(downSpeedButton);
    addAndMakeVisible(rewindButton);
    addAndMakeVisible(loopButton);
       
    //we make visible all sliders
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    //we make visible the audio graphic
    addAndMakeVisible(waveformDisplay);

    //we add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    upVolButton.addListener(this);
    downVolButton.addListener(this);
    upSpeedButton.addListener(this);
    downSpeedButton.addListener(this);
    rewindButton.addListener(this);
    loopButton.addListener(this);

    //we change the buttons color
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
    upVolButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
    downVolButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
    upSpeedButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
    downSpeedButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
    rewindButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
    loopButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
    
    //we add listeners and change colors
    volSlider.addListener(this);
    volSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0,128,128));

    speedSlider.addListener(this);
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0,128,128));
    posSlider.addListener(this);
    posSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0,128,128));

    //We add labels with the sliders name
    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    volumeLabel.setColour(juce::Label::textColourId,  juce::Colour(0,128,128));

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::NotificationType::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
    speedLabel.setColour(juce::Label::textColourId,  juce::Colour(0,128,128));
    
    addAndMakeVisible(posLabel);
    posLabel.setText("Time", juce::NotificationType::dontSendNotification);
    posLabel.setJustificationType(juce::Justification::centred);
    posLabel.setColour(juce::Label::textColourId,  juce::Colour(0,128,128));
    
    //we set sliders range
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5); //We set the initial value to the middle
    volSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0,128,128));
    volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
    
    speedSlider.setRange(0.1, 10.0);
    speedSlider.setValue(1.0);
    speedSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0,128,128));
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
    
    posSlider.setRange(0.0, 1.0);
    posSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0,128,128));
    posSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);

    //we start the timer
    startTimer(10);
    
    //we define the initial color of the loopButton
    color = loopButton.findColour(juce::TextButton::buttonColourId);
    

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.
     */

    //We define the variables rowH, diameter and radio
    double rowH = getHeight() / 13;
    float diameter =getHeight() * 0.35;
    float radio = diameter/2.0;
    
    //we fill the background
    g.fillAll(juce::Colour(200, 238, 238));

    g.saveState();
    Colour endColour;
    Colour startColour(0, 128, 128);
    // We define the end color for the degraded
    if (speedSlider.getValue()>2)
    {
        endColour=juce::Colour(255, 50, 50);
    }
    else
    {
        endColour=juce::Colour(255, 255, 255);
    }
    // Create a GradientFillType object for the degraded fill
    ColourGradient gradient(startColour, 0.0f, 0.0f, endColour, getWidth(), getHeight(), true);
    
    g.setGradientFill(gradient);
    g.fillEllipse(diskBounds);
    g.addTransform(AffineTransform::rotation(rotationAngle, radio+(getWidth() - diameter) / 2.0, radio+rowH*5));
    g.setColour(juce::Colour(0, 128, 128));
    //we draw the ellipse
    g.drawEllipse(diskBounds, 2.0f);
    g.restoreState();
    
    //for the line in the disk (using the rotationAngle)
    g.setColour(juce::Colour(200, 238, 238));
    float xEnd = (radio+(getWidth() - diameter) / 2.0) + radio * std::cos(rotationAngle);
    float yEnd = radio+rowH*5 + radio * std::sin(rotationAngle);
    g.drawLine(radio+(getWidth() - diameter) / 2.0, radio+rowH*5, xEnd, yEnd, 2.0f);
    
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.saveState();
    g.setColour(juce::Colour(200, 238, 238));
    g.fillEllipse(diskBounds_ins);
    String text = "Speed";
    g.addTransform(AffineTransform::rotation(rotationAngle, radio+(getWidth() - diameter) / 2.0, radio+rowH*5));
    g.setColour(Colours::white);
    g.drawText(text, diskBounds_ins, Justification::centred, true);
    g.setColour(juce::Colour(0, 128, 128));
    g.drawEllipse(diskBounds_ins, 2.0f);
    g.restoreState();
    
    
}

void DeckGUI::resized()
{
    //we define the rowH parameter
    double rowH = getHeight() / 13;
    //we set playbutton, stopButton and volSlider bounds
    playButton.setBounds(0, 0, getWidth()/2, 2*rowH);
    stopButton.setBounds(getWidth()/2, 0, getWidth()/2, 2*rowH);
    volSlider.setBounds(getWidth()*3/20, rowH * 2, getWidth()*4/5, rowH);
    
    //we set the up and down buttons
    upVolButton.setBounds(getWidth()-getWidth()/20, (rowH * 2)+2, getWidth()*0.9/20, (rowH/2)-3);
    downVolButton.setBounds(getWidth()-getWidth()*1/20, (rowH * 2)+rowH/2, getWidth()*0.9/20, (rowH/2)-3);
    volumeLabel.setBounds(0,rowH*2,getWidth()*3/20,rowH);
    
    //we set the spped slider bounds
    speedSlider.setBounds(getWidth()*3/20, rowH * 3, getWidth()*4/5, rowH);
    
    upSpeedButton.setBounds(getWidth()-getWidth()/20, (rowH * 3), (getWidth()*0.9/20), (rowH/2)-3);
    downSpeedButton.setBounds(getWidth()-getWidth()*1/20, (rowH * 3)+rowH/2, getWidth()*0.9/20, (rowH/2)-3);
    speedLabel.setBounds(0,rowH*3,getWidth()*3/20,rowH);
    
    //we set the time slider and the waveformDisplay
    posSlider.setBounds(getWidth()*3/20, rowH * 4, getWidth()*4/5, rowH);
    waveformDisplay.setBounds(0, rowH * 10, getWidth(), rowH * 2);
    
    //we set the diskbounds bounds depending on the diameter (radio)
    diskBounds = getLocalBounds().toFloat().reduced(10);
    float diameter =getHeight() * 0.35;
    float x = (getWidth() - diameter) / 2.0;
    float y = rowH*5;
    diskBounds = juce::Rectangle<float>(x, y, diameter, diameter);
    
    float diameter_ins =getHeight() * 0.2;
    diskBounds_ins = juce::Rectangle<float>(x+(diameter/2-diameter_ins/2), y+(diameter/2-diameter_ins/2), diameter_ins, diameter_ins);
    
    //we set the load, rewind and loop buttons bounds
    loadButton.setBounds(0, rowH * 12, getWidth()-140, rowH);
    rewindButton.setBounds(getWidth()-80,rowH * 12,80, rowH);
    loopButton.setBounds(getWidth()-140,rowH * 12,60, rowH);
    posLabel.setBounds(5,rowH*4,getWidth()*3/20,rowH);

}

void DeckGUI:: drawWave(URL url)
{
    //we stop if a file is playing
    is_playing = false;
    //we load the waveformDisplay
    waveformDisplay.loadURL(url);
}

void DeckGUI::buttonClicked(Button* button)
{
    //if the start button is clicked, we play the audio
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        is_playing = true;
        player->start();
    }
    //if the stop button is clicked, we stop the audio
     if (button == &stopButton)
    {
        is_playing = false;
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    //if the load button is clicked, we load the audio is chosen
       if (button == &loadButton)
    {
        is_playing = false;
        auto fileChooserFlags =
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {

            player->loadURL(URL{chooser.getResult()});
            // and now the waveformDisplay as well
            drawWave(URL{chooser.getResult()});
        });
    }
    
    //if upVolume button is clicked, the volume goes up
    if(button == &upVolButton)
    {
        volSlider.setValue(volSlider.getValue()+0.05);
    }
    //if downVolume button is clicked, the volume goes down
    if (button == &downVolButton)
    {
        volSlider.setValue(volSlider.getValue()-0.05);
    }
    //if upSpeed button is clicked, the speed goes up
    if(button == &upSpeedButton)
    {
        speedSlider.setValue(speedSlider.getValue()+0.5);
    }
    //if upSpeed button is clicked, the speed goes down
    if (button == &downSpeedButton)
    {
        speedSlider.setValue(speedSlider.getValue()-0.5);
    }
    
    //if rewind button is clicked, then the audio will start from the beginning
    if (button == &rewindButton)
    {
        player->setPositionRelative(0);
        player->start();
    }
    
    //if loop button is clicked, then the audio will start from the beginning when it gets to the end
    if (button == &loopButton)
    {
        if (isButtonClicked) {
            button->setColour(juce::TextButton::buttonColourId, color);
            condition = false;
        }
        else {
            button->setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
            condition = true;
        }
        is_playing = true;
        isButtonClicked = !isButtonClicked;
    }
    
    

}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    //for the waveformDisplay, we set the position
    waveformDisplay.setPositionRelative(
                                        player->getPositionRelative());
    
    //if the mouse button is not down (posSlider), we update the position of the slider
    if (!posSlider.isMouseButtonDown())
    {
        double positionRelative = player->getPositionRelative();
        //if the value of the relative position is NaN, then we are going to make it 0
        if (std::isnan(positionRelative)) {
            positionRelative = 0.0;
        }

        posSlider.setValue(positionRelative);
        
        
    }
    
    // if the loop button is clicked, then if the audio have finished, we start it again
    if (condition) {
        if (posSlider.getValue()==1)
        {
            player->setPositionRelative(0);
            player->start();
        }
    }
    
    // to get the rotation angle
    if (is_playing && posSlider.getValue()!=1)
    {
            float rotationSpeed = speedSlider.getValue()/100; // disk rotation speed
            rotationAngle += rotationSpeed;
            // repaint the component
            repaint();
    }
    
}

