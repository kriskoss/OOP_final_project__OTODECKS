/*
  ==============================================================================

    DeckGUI.h
    Created: 19 Feb 2023 4:39:51pm
    Author:  Candidate No. EX2765

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h" 
#include "WaveformDisplay.h"
#include "Settings.h"
#include "MyLookAndFeel.h"
#include "SpectrogramComponent.h"

//==============================================================================
/*
*/



//DeckGUI.h
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,       
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer

{
public:
   DeckGUI(Settings* _settings, DJAudioPlayer* player,
      SpectrogramComponent *_simpleFFT,
      juce::AudioFormatManager & formatManagerToUse,
      juce::AudioThumbnailCache & cacheToUse
   );
   
   // ==== VIRTUAL FUNCTIONS ====
   ~DeckGUI() override;
   void paint (juce::Graphics&) override;
   void resized() override;

   void buttonClicked(juce::Button* button) override;

   void sliderValueChanged(juce::Slider* slider) override;
   
   bool isInterestedInFileDrag(const juce::StringArray& files) override;
   void filesDropped(const juce::StringArray& files, int x, int y) override;

   void timerCallback() override;
   //==============================

   /** Player states*/
   enum class PlayerState { play, pause, stop };
   PlayerState playerState;

   SpectrogramComponent* simpleFFT;
   
   // FUNCTIONS
   /** loads file sent from the playlist into the deck*/
   void loadIncomingFile(juce::File & sentFile); 

   /**Stops the player and reset the the current posiotion to 0 */
   void stopAndReset();
   void updateCurrentTitle(std::string title);
private:
   Settings *settings;
   DJAudioPlayer* player;
   WaveformDisplay wavefromDisplay;

   // Settings
   juce::Colour deckBackgroundColor = settings->deckBackgroundColor;
   MyLookAndFeel myLookAndFeel;

   // GUI ELEMENTS
   juce::TextButton stopButton{ "STOP" };
   
   juce::Slider gainSlider;
   juce::Slider speedSlider;
   juce::Slider posSlider;
   juce::TextButton playPauseButton{ "playPauseButton" };
   juce::TextButton loadButton{ "LOAD" };
   juce::Label currentTrackTitle{ "currentTrack", ""};

   juce::TextButton fadeInPLAY{ settings->fadeInPlayLabel };
   juce::TextButton fadeOutSTOP{ settings->fadeOutStopLabel };

   juce::ToggleButton atStartAndEndOnly{ settings->atStartAndEndOnlyLabel };

   // FLAGS   
   bool mouseDown = false;
   bool mouseDragging = false;
   
   //Settings
   float fadeInTime = settings->fadeInTime; //seconds
   float fadeOutTime = settings->fadeOutTime;

   int timerStep = settings->deckGUItimeStep; // milliseconds
   double initialGainValue = settings->initialGainValue;
   
   // Data members
   int fadeInSteps = fadeInTime*1000/timerStep; // converitng fadeInTime into number of steps of the counter
   int fadeOutSteps = fadeInSteps;

   int fadeInCounter = 0; // the actual counter
   int fadeOutCounter = 0; // the actual counter
   
   double lastGainInValue = initialGainValue;
   double lastGainOutValue = initialGainValue;
   double gainStep;
   bool approachingEnd{ false };
   
   float mouseXRelativeDragOverWaveform = -1.0f;
   
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
