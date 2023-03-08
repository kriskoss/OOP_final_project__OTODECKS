/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 24 Feb 2023 2:55:55pm
    Author:  krzys

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
//WaveformDisplay.cpp
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse):
                                 audioThumb(1000,formatManagerToUse,cacheToUse),
                                 anyFileLoaded(false),
                                 position(0)
{
   audioThumb.addChangeListener(this);

}

WaveformDisplay::~WaveformDisplay()
{
}


void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::yellow);
    if (anyFileLoaded)
    {
       audioThumb.drawChannel(g,
          getLocalBounds(),
          0,
          audioThumb.getTotalLength(),
          0,
          1.0f
       );
       
       int playheadWidth = getWidth() / 200;
       
       int posXRelative = 0;
       // Prevents negative values
       if (position * getWidth() >= 0)
       {
          posXRelative = position * getWidth();
       }

       // Playhead look
       g.setColour(juce::Colours::black);
       g.fillRect(posXRelative, 0, playheadWidth, getHeight());
       
       //===== Area already played =====
       g.saveState();
       
       g.setColour(juce::Colours::grey);
       g.setOpacity(0.3);
       g.fillRect(0, 2, posXRelative, getHeight());
       
       //=====Area to be played====
       
       g.setColour(juce::Colours::darkred);
       // Checks if rect after the play head is less then 0 
       int widthAfter = getWidth() - posXRelative - playheadWidth;
       if (widthAfter < 0)
       {
          widthAfter = 0;
       }

       g.fillRect(posXRelative + playheadWidth, 5, widthAfter, 3);
       
       g.restoreState();
       //^^^^^^^^^^^^^^^^^END^^^^^^^^^^^^
       
       
    }
    else
    {
       g.setFont(20.0f);
       g.drawText("File not loaded yet...", getLocalBounds(),
          juce::Justification::centred, true);   // draw some placeholder text
    }

    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setColour (juce::Colours::grey);
    
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
   audioThumb.clear();
   anyFileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
   newFileLoaded = anyFileLoaded; // Required for updating the Playlist when new file added 
   if (newFileLoaded)
   {
      //DBG("WaveformDisplay::loadURL: new file loaded! -- Number of thumbnails: " + std::to_string(thumbnails.size()));

   }
   else
   {
      DBG("WaveformDisplay::loadURL: NEW FILE FAILDED TO LOAD !");
   }

}
void WaveformDisplay::loadURL(juce::URL audioURL, int _trackNum)
{
   loadURL(audioURL);
   tracksBeingLoaded.push(_trackNum);
   DBG("WaveformDisplay::loadURL (2): LODADING TRACK " + std::to_string(_trackNum));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
   thumbnailLoadProgress =  audioThumb.getProportionComplete()*100;
   DBG("wfd: change received! - progress" + std::to_string(thumbnailLoadProgress )+ "%");
   repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
   if (pos != position)
   {
      position = pos;
      repaint();
   }
}


bool WaveformDisplay::checkIfFileLoaded()
{
   if (anyFileLoaded)
   {
      return true;
   }
   return false;
}

bool WaveformDisplay::checkIfThumbnailFullyLoaded()
{
   if (audioThumb.isFullyLoaded())
   {
      thumbnailLoadProgress = 0;
   }

   return audioThumb.isFullyLoaded();
}