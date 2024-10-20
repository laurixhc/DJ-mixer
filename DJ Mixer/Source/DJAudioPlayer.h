/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    /** Function to load un URL */
    void loadURL(URL audioURL);
    void setGain(double gain);
    /** Function to set the speed of the audio */
    void setSpeed(double ratio);
    /** Function to set the position (time) of the audio */
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    /** Function to start playing the audio */
    void start();
    /** Function to stop playing the audio */
    void stop();

    /** get the relative position of the playhead
            output: double precision (the result of dividing the current position and the length in seconds)
     */
    double getPositionRelative();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; //object of the class transportSource
    ResamplingAudioSource resampleSource{&transportSource, false, 2};

};




