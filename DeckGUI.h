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
   DeckGUI(DJAudioPlayer* player,
      juce::AudioFormatManager & formatManagerToUse,
      juce::AudioThumbnailCache & cacheToUse
   );
   ~DeckGUI() override;
   void paint (juce::Graphics&) override;
   void resized() override;

   void buttonClicked(juce::Button* button) override;

   void sliderValueChanged(juce::Slider* slider) override;
   
   bool isInterestedInFileDrag(const juce::StringArray& files) override;
   void filesDropped(const juce::StringArray& files, int x, int y) override;

   void timerCallback() override;
   
   /** loads file sent from the playlist into the deck*/
   void loadIncomingFile(juce::File & sentFile); 

   enum class PlayerState { play, pause, stop };
   PlayerState playerState;
   juce::TextButton playPauseButton{ "X" };

   /**Stops the player and reset the the current posiotion to 0 */
   void stopAndReset();

private:

   juce::TextButton stopButton{ "STOP" };
   
   juce::Slider gainSlider;
   juce::Slider speedSlider;
   juce::Slider posSlider;

   juce::TextButton loadButton{ "LOAD" };

   DJAudioPlayer *player;  

   WaveformDisplay wavefromDisplay;

   /// MY ADDITIONS
   
   
   bool mouseDown = false;
   bool mouseDragging = false;
   
   juce::TextButton fadeInPLAY{ "PLAY (FADE-IN)" };
   juce::TextButton fadeOutSTOP{ "STOP (FADE-OUT)" };

   juce::ToggleButton atStartAndEndOnly{ "Start/End only" };

   float fadeInTime = 2; //seconds
   float fadeOutTime = fadeInTime;

   int timerStep = 10; // milliseconds
   
   int fadeInSteps = fadeInTime*1000/timerStep; // converitng fadeInTime into number of steps of the counter
   int fadeOutSteps = fadeInSteps;

   int fadeInCounter = 0; // the actual counter
   int fadeOutCounter = 0; // the actual counter
   
   double initialGainValue = 0.5;
   
   double lastGainInValue = gainSlider.getValue();
   double lastGainOutValue = gainSlider.getValue();
   double gainStep;
   bool approachingEnd{ false };
   
   float mouseXRelativeDragOverWaveform = -1.0f;
   

   
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
