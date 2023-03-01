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
                                 fileLoaded(false),
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
    if (fileLoaded)
    {
       audioThumb.drawChannel(g,
          getLocalBounds(),
          0,
          audioThumb.getTotalLength(),
          0,
          1.0f
       );
       int playheadWidth = getWidth() / 100;
       int posXRelative = position * getWidth();

       // Playhead look
       g.setColour(juce::Colours::pink);
       g.drawRect(posXRelative, 0, playheadWidth, getHeight());
       
       //Area already played
       g.setColour(juce::Colours::green);
       g.drawRect(0, 2, posXRelative, 2);
       
       //Area to be played
       g.setColour(juce::Colours::red);
       g.drawRect(posXRelative + playheadWidth, 2, getWidth() - posXRelative - playheadWidth, 2);

       // Resetting colour
       g.setColour(juce::Colours::yellow);
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
   fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
   
   if (fileLoaded)
   {
      DBG("wfd: loaded!");
   }
   else
   {
      DBG("wfd: NOT loaded!");
   }

}


void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
   DBG("wfd: change received!");
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

