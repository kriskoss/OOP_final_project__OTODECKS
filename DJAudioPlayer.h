/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 5 Feb 2023 11:48:51pm
    Author:  Candidate No. EX2765

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>

//DJAudioPlayer.h
class DJAudioPlayer: public juce::AudioSource
{
public:

    DJAudioPlayer(juce::AudioFormatManager & formatManager);
    ~DJAudioPlayer();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;


    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    double timeToEnd();

    void start();
    void stop();

    /** gets current relative position in the audio file of the playhead*/
    double getPositionRelative();

    juce::FileChooser fChooser{ "Select a file..." };

    /**Check if the player is currently playing any music*/
    bool isPlaying();

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};

    
};