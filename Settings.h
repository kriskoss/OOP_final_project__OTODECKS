/*
  ==============================================================================

    Settings.h
    Created: 11 Mar 2023 3:48:06pm
    Author:  krzys

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <iostream>
class Settings 
{
public:
   //DeckGUI settings
   float initialGainValue = 0.5;
   float fadeInTime = 4;      // seconds
   float fadeOutTime = fadeInTime; //seconds
   int deckGUItimeStep = 50; // miliseconds
   std::string fadeInPlayLabel = "PLAY (FADE-IN)";
   std::string fadeOutStopLabel = "STOP (FADE-OUT)";
   std::string atStartAndEndOnlyLabel = "Start/End only";

   //PlaylistComponent
   int playlistTimerStep = 200;

   std::string addFileButtonText = "Select files to upload to the playlist...";
   std::string searchFieldText = "Search";
   std::string clearSearchButtonLabel = "Reset Search";
   std::string clearPlaylistButtonLabel = "CLEAR THE PLAYLIST";
   int columnWidth = 90;

   //========== COLORS ================
   //** DeckGUI
   juce::Colour deckBackgroundColor = juce::Colours::darkslateblue;

   //WaveformDispaly
   juce::Colour playheadColorType = juce::Colours::black;
   juce::Colour areaBehindPlayhead = juce::Colours::grey;
   juce::Colour areaToBePlayed = juce::Colours::darkred;
   
   juce::Colour waveformColor = juce::Colours::deepskyblue;
   juce::Colour fileNotLoadedColor = juce::Colours::orange;
   
   // **PlaylistComponent

   juce::Colour playlistBackgroundColor = juce::Colours::darkslateblue;
   

   //DJAudioPlayer   - settings NOT IMPLEMENTED


};
