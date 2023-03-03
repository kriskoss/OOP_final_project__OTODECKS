   /*
     ==============================================================================

       WaveformDisplay.h
       Created: 24 Feb 2023 2:55:55pm
       Author:  krzys

     ==============================================================================
   */

   #pragma once

   #include <JuceHeader.h>

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
       
       /** set the relative position of the playhead*/
       void setPositionRelative(double pos);

       bool checkIfFileLoaded(); /** returns true if file is loaded, fales otherwise*/
   private:

      juce::AudioThumbnail audioThumb;
      bool fileLoaded;
      double position;

       JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
   };
