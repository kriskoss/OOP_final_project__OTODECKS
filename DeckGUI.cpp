/*
  ==============================================================================

    DeckGUI.cpp
    Created: 19 Feb 2023 4:39:51pm
    Author:  krzys

  ==============================================================================
*/

//DeckGUI.cpp
#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
               juce::AudioFormatManager & formatManagerToUse,
               juce::AudioThumbnailCache & cacheToUse
) :player(_player),
wavefromDisplay( formatManagerToUse, cacheToUse)

{
   addAndMakeVisible(playButton);
   addAndMakeVisible(stopButton);
   addAndMakeVisible(gainSlider);
   addAndMakeVisible(speedSlider);
   addAndMakeVisible(posSlider);
   addAndMakeVisible(loadButton);
   addAndMakeVisible(wavefromDisplay);

   //LISTENERS  //---A5--- ADDING LISTENERS
   playButton.addListener(this);
   stopButton.addListener(this);
   gainSlider.addListener(this);
   speedSlider.addListener(this);
   posSlider.addListener(this);
   loadButton.addListener(this);

   // SLIDERS PARAMETERS
   gainSlider.setRange(0, 1); 
   gainSlider.setValue(0.5);

   speedSlider.setRange(0.1, 5);
   speedSlider.setValue(1);

   posSlider.setRange(0, 1);

   startTimer(100);
}

DeckGUI::~DeckGUI()
{
   stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    //g.setColour (juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("DeckGUI", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text

    
}

void DeckGUI::resized()
{
     DBG("DeckGUI::resized");
    double rowH = getHeight() / 8;
    
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    gainSlider.setBounds(0, 2 * rowH, getWidth(), rowH);
    speedSlider.setBounds(0, 3 * rowH, getWidth(), rowH);
    posSlider.setBounds(0, 4 * rowH, getWidth(), rowH);
    wavefromDisplay.setBounds(0, 5 * rowH, getWidth(), rowH*2);
    loadButton.setBounds(0, getHeight() - rowH, getWidth(), rowH);

}



void DeckGUI::buttonClicked(juce::Button* button) //---A4---MOVED from MainComponent.cpp
{
   DBG("Button clicked!");
   if (button == &playButton)
   {
       DBG("   * playButton!   MainComponent::buttonClicked");
       player->start();

   }
   if (button ==&stopButton) 
   {
       DBG("   * stopButton!    MainComponent::buttonClicked");
       player->stop();
   }
   if (button == &loadButton)
   {
       
       DBG("   * loadButton!    MainComponent::buttonClicked");
       // - configure the dialogue
       auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
       
       player->fChooser.launchAsync( // LAMBDA FUNCTION- launch out of the main thread
           fileChooserFlags, 
           [this](const juce::FileChooser& chooser)
           {
               juce::File chosenFile = chooser.getResult();
               player->loadURL(juce::URL{ chosenFile });
               wavefromDisplay.loadURL(juce::URL{ chosenFile });
           }
       );
   }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) //---A4---MOVED from MainComponent.cpp
{
   if (slider == &gainSlider)
      {
          player->setGain(slider->getValue());

      }
      if (slider == &speedSlider)
      {
          player->setSpeed(slider->getValue());
      }

      if (slider == &posSlider)
      {
          player->setPositionRelative(slider->getValue());
      }
}


bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
   DBG("isInterestedInFileDrag: true");
   return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) 
{
   DBG("DeckGUI::filesDropped");
   for (juce::String filename : files)
   {
      DBG("DeckGUI::filesDropped---> files: " + filename);
      
      juce::URL fileURL = juce::URL{ juce::File{filename} };
      player->loadURL(fileURL);
      DBG(filename);
      return;
   }
   
      
}

void DeckGUI::timerCallback()
{
   wavefromDisplay.setPositionRelative(
           player->getPositionRelative());
}