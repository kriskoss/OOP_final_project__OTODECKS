/*
   ==============================================================================

      WaveformDisplay.h
      Created: 24 Feb 2023 2:55:55pm
      Author:  krzys

   ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <iostream>
#include <queue>

//==============================================================================
/*
*/
//WaveformDisplay.h
class WaveformDisplay : public juce::Component,
                        public juce::ChangeListener
   {
   public:
       WaveformDisplay(
          juce::AudioFormatManager & formatManagerToUse,
          juce::AudioThumbnailCache & cacheToUse
       );
       ~WaveformDisplay() override;

       void paint (juce::Graphics&) override;
       void resized() override;

       void changeListenerCallback(juce::ChangeBroadcaster* source) override;

       void loadURL(juce::URL audioURL);
       void loadURL(juce::URL audioURL, int trackNum);
       
       /** set the relative position of the playhead*/
       void setPositionRelative(double pos);

       bool checkIfFileLoaded(); /** returns true if file is loaded, fales otherwise*/
       bool checkIfThumbnailFullyLoaded();
       bool newFileLoaded;

       //std::vector<juce::AudioThumbnail> thumbnails;
       std::queue<int> tracksBeingLoaded;

       int thumbnailLoadProgress = 0;
   private:
      bool anyFileLoaded;
      juce::AudioThumbnail audioThumb;
      
      double position;

       JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
   };
