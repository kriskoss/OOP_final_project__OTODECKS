/*
  ==============================================================================

    DeckGUI.h
    Created: 19 Feb 2023 4:39:51pm
    Author:  krzys

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

private:

   juce::TextButton playButton{ "PLAY" };
   juce::TextButton stopButton{ "STOP" };
   juce::Slider gainSlider;
   juce::Slider speedSlider;
   juce::Slider posSlider;

   juce::TextButton loadButton{ "LOAD" };

   DJAudioPlayer *player;  

   WaveformDisplay wavefromDisplay;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
