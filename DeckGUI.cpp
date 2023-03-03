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
   addAndMakeVisible(pauseButton);
   addAndMakeVisible(gainSlider);
   addAndMakeVisible(speedSlider);
   addAndMakeVisible(posSlider);
   addAndMakeVisible(loadButton);
   addAndMakeVisible(wavefromDisplay);
   
   addAndMakeVisible(fadeInToggle);
   addAndMakeVisible(fadeOutToggle);
   addAndMakeVisible(atStartAndEndOnly);
   
   addAndMakeVisible(fadeInPLAY);
   addAndMakeVisible(fadeOutSTOP);
   

   //LISTENERS  //---A5--- ADDING LISTENERS
   playButton.addListener(this);
   pauseButton.addListener(this);
   gainSlider.addListener(this);
   speedSlider.addListener(this);
   posSlider.addListener(this);
   loadButton.addListener(this);

   fadeInToggle.addListener(this);
   fadeOutToggle.addListener(this);
   atStartAndEndOnly.addListener(this);

   fadeInPLAY.addListener(this);
   fadeOutSTOP.addListener(this);


   // SLIDERS PARAMETERS
   gainSlider.setRange(0, 1); 
   gainSlider.setValue(initialGainValue);

   speedSlider.setRange(0.1, 5);
   speedSlider.setValue(1);

   posSlider.setRange(0, 1);
   startTimer(timerStep);
   
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
    double rowH = getHeight() / 9;
    
    playButton.setBounds(0, 0, getWidth() / 2, rowH);
    pauseButton.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    fadeInPLAY.setBounds(0, rowH, getWidth() / 2, rowH);
    fadeOutSTOP.setBounds(getWidth() / 2, rowH, getWidth() / 2, rowH);
    gainSlider.setBounds(0, 2 * rowH, getWidth(), rowH);
    speedSlider.setBounds(0, 3 * rowH, getWidth(), rowH);
    posSlider.setBounds(0, 4 * rowH, getWidth(), rowH);
    wavefromDisplay.setBounds(0, 5 * rowH, getWidth(), rowH*2);
    loadButton.setBounds(0, 7* rowH, getWidth(), rowH);
    fadeInToggle.setBounds(0, 8* rowH, 80, rowH);
    fadeOutToggle.setBounds(80, 8* rowH, 80, rowH);
    atStartAndEndOnly.setBounds(160, 8 * rowH, 120, rowH);
}



void DeckGUI::buttonClicked(juce::Button* button) //---A4---MOVED from MainComponent.cpp
{
   DBG("Button clicked!");



   if (button == &playButton && !player->isPlaying() )
   {
       DBG("   * playButton!   MainComponent::buttonClicked");
       player->start();
       
       // FADE IN works only if file load is complete and FADE IN ticked
       if (fadeInToggle.getToggleState() && wavefromDisplay.checkIfFileLoaded())
       {
          // FADE IN activation logic - runs only when FADE-IN ticked, or if at the beginning of the file in case START/END only ticked

          if ((atStartAndEndOnly.getToggleState() && player->getPositionRelative() == 0) || 
             !atStartAndEndOnly.getToggleState())
          {
             lastGainInValue = gainSlider.getValue(); // Storing lastGainInValue before play button clicked

             gainStep = lastGainInValue / fadeInSteps; // Calculates the amount of which the gain will be increased at each step of the fade in
             if (fadeOutCounter > 0) // stops FADE OUT in case FADE IN itialized when the FADE OUT is running 
             {
                fadeOutCounter = 0;
                gainStep = abs(lastGainInValue-lastGainOutValue  ) / fadeInSteps;
                lastGainInValue = lastGainOutValue;
                
             }
             else
             {
                gainSlider.setValue(0);
             }
             DBG("DeckGUI::buttonClicked -> playButton:  AT THE START " + std::to_string(player->getPositionRelative()));
             fadeInCounter = fadeInSteps;
             
             
             
             
          }
       }


   }
   if (button ==&pauseButton) 
   {
       DBG("   * pauseButton!    MainComponent::buttonClicked");
       if (fadeOutToggle.getToggleState() && wavefromDisplay.checkIfFileLoaded() && fadeOutCounter==0)
       {
          lastGainOutValue = gainSlider.getValue(); // Storing lastGainOutValue before play button clicked

          gainStep = lastGainOutValue / fadeOutSteps; // Calculates the amount of which the gain will be increased at each step of the fade in
          DBG("DeckGUI::buttonClicked -> playButton:  fadeOutSteps:" + fadeOutSteps);
          
          DBG("DeckGUI::buttonClicked -> playButton:  AT THE START " + std::to_string(player->getPositionRelative()));
          fadeOutCounter = fadeOutSteps;
          
          if (fadeOutCounter == 0)
          {
             gainSlider.setValue(lastGainOutValue);
             player->stop();
          }
       }
       else 
       {
          {
             if (fadeOutCounter == 0)
                player->stop();
          }
       }
   }
   if (button == &loadButton)
   {
       
       DBG("   * loadButton!    MainComponent::buttonClicked");
       // - configure the dialogue
       auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
       
       
       /// ==== FOR DEVELOPMENT ONLY!!!!!!!!!
       juce::File myfile("C:/_MOJE/_1_MOJE/CS Computer Science/UoL/2022 October/OOP/uol_oop_tracks/tracks/aon_inspired.mp3");
       juce::URL chosenFile(myfile);
       player->loadURL(juce::URL{ chosenFile });
       wavefromDisplay.loadURL(juce::URL{ chosenFile });


       //player->fChooser.launchAsync( // LAMBDA FUNCTION- launch out of the main thread
       //    fileChooserFlags, 
       //    [this](const juce::FileChooser& chooser)
       //    {
       //        juce::File chosenFile = chooser.getResult();
       //        
       //        player->loadURL(juce::URL{ chosenFile });
       //        wavefromDisplay.loadURL(juce::URL{ chosenFile });
       //    }
       //);
       
       //^^^^^^^^^^^^^END^^^^^^^^^^^^^^^^^^^^
   }

   
   // Fade In ON/OFF Toogle 
   if (button == &fadeInToggle)
   {
      DBG("   * fadeInButton!    MainComponent::buttonClicked");
      if (button->getToggleState())
      {
         DBG(" DeckGUI::buttonClicked:TRUE");
         
      }
      else
      {
         DBG(" DeckGUI::buttonClicked:FALSE");
      }
      

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


      if (fadeInCounter > 0)
      {
         /// FADE IN - counter
         fadeInCounter--;
         DBG("DeckGUI::timerCallback-> FADE IN: " + std::to_string(fadeInCounter));

         if (fadeInCounter == 0)
         {
            gainSlider.setValue(lastGainInValue); // Resetting gain value to the one just before fade in counter initialized (just before play button pressed)
         }
         else
         {
            //Gain increased to create FADE IN effect
            double newGainValue = gainSlider.getValue() + gainStep;
            
            // Updates the slider valeu to match the current gain value
            gainSlider.setValue(newGainValue);
         }
      }

      // FADE OUT counter
      if (fadeOutCounter > 0)
      {
         fadeOutCounter--;

         if (fadeOutCounter == 0)
         {
            // Player stops when FADE OUT complete
            player->stop();
            
            // Resets the slider value to the one before FADE OUT started
            gainSlider.setValue(lastGainOutValue);
            approachingEnd = false;
            

         }
         else
         {
            //Gain reduced to create FADE OUT effect
            double newGainValue = gainSlider.getValue() - gainStep;
            
            // Updates the slider valeu to match the current gain value
            gainSlider.setValue(newGainValue);
         }


      }
      /*if (player->timeToEnd() > 0)
      {
         DBG("DecKGUI::timecaller   test" + std::to_string(player->timeToEnd()));
      }*/
      
      // FADE OUT initialization at the END
      if (player->timeToEnd()<fadeOutTime && player->timeToEnd()>0 && fadeOutToggle.getToggleState()&& !approachingEnd)
      {
         approachingEnd = true;
         lastGainOutValue = gainSlider.getValue();
         fadeOutCounter = ceil(player->timeToEnd() * 1000 / timerStep);
         fadeInCounter = 0; // stops FADE IN in case it runs when FADE OUT initiated
         DBG("DecKGUI::timecaller   FADEing OUT - END" );
      }
}