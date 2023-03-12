//MainComponent.h
#pragma once

#include <JuceHeader.h>
#include "Settings.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"    
#include "PlaylistComponent.h"
#include "SpectrogramComponent.h"


    //==============================================================================
    /*
        This component lives inside our window, and this is where you should put all
        your controls and content.
    */
class MainComponent : public juce::AudioAppComponent
    {
    public:
        //==============================================================================
        MainComponent();
        ~MainComponent() override;

        //==============================================================================
        void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        //==============================================================================
        void paint (juce::Graphics& g) override;
        void resized() override;

        
    private:
        //==============================================================================
        // Your private member variables go here...
       Settings settings;
       
       juce::AudioFormatManager formatManager;
       juce::AudioThumbnailCache thumbCache{ 100 };
       SpectrogramComponent simpleFFT{ formatManager, &mixerSource };

        DJAudioPlayer player1{formatManager};
        DeckGUI deckGUI1{&settings,&player1, &simpleFFT,  formatManager, thumbCache};     
        
        DJAudioPlayer player2{formatManager};
        DeckGUI deckGUI2{&settings,&player2, &simpleFFT,formatManager, thumbCache};

        juce::MixerAudioSource mixerSource;

        DJAudioPlayer playerPL{ formatManager };
        PlaylistComponent playlistComponent{&settings, &playerPL,&deckGUI1,&deckGUI2, formatManager,thumbCache };

        
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    };
