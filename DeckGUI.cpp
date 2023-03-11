/*
  ==============================================================================

    DeckGUI.cpp
    Created: 19 Feb 2023 4:39:51pm
    Author:  Candidate No. EX2765

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
   addAndMakeVisible(stopButton);
   addAndMakeVisible(playPauseButton);
   addAndMakeVisible(gainSlider);
   addAndMakeVisible(speedSlider);
   addAndMakeVisible(loadButton);
   addAndMakeVisible(wavefromDisplay);
   
   addAndMakeVisible(atStartAndEndOnly);
   
   addAndMakeVisible(fadeInPLAY);
   addAndMakeVisible(fadeOutSTOP);
   

   //LISTENERS  //---A5--- ADDING LISTENERS
   stopButton.addListener(this);
   playPauseButton.addListener(this);
   gainSlider.addListener(this);
   speedSlider.addListener(this);
   loadButton.addListener(this);

   atStartAndEndOnly.addListener(this);

   fadeInPLAY.addListener(this);
   fadeOutSTOP.addListener(this);


   playerState = PlayerState::stop;

   // SLIDERS PARAMETERS
   gainSlider.setRange(0, 1); 
   gainSlider.setValue(initialGainValue);

   speedSlider.setRange(0.1, 5);
   speedSlider.setValue(1);


   startTimer(timerStep);
   
}

DeckGUI::~DeckGUI()
{
   stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
 
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
    double rowH = getHeight() / 10;
    
    playPauseButton.setBounds(0, 0, getWidth(), rowH);

    fadeInPLAY.setBounds(0, rowH, getWidth() / 2, rowH);
    fadeOutSTOP.setBounds(getWidth() / 2, rowH, getWidth() / 2, rowH);

    stopButton.setBounds(0, 2 * rowH, getWidth(), rowH);
    gainSlider.setBounds(0, 3 * rowH, getWidth(), rowH);
    speedSlider.setBounds(0, 4 * rowH, getWidth(), rowH);
    wavefromDisplay.setBounds(0, 6 * rowH, getWidth(), rowH*2);
    loadButton.setBounds(0, 8* rowH, getWidth(), rowH);
    atStartAndEndOnly.setBounds(0, 9 * rowH, 120, rowH);
}



void DeckGUI::buttonClicked(juce::Button* button) //---A4---MOVED from MainComponent.cpp
{
   DBG("Button clicked!");

   if (button == &playPauseButton && wavefromDisplay.checkIfFileLoaded())
   {
      if (!player->isPlaying()) //PLAY
      {
         

         // BEGINING FADE-IN START
         if ((atStartAndEndOnly.getToggleState() && player->getPositionRelative() == 0))
         {
            lastGainInValue = gainSlider.getValue(); // Storing lastGainInValue before play button clicked
            
            if (fadeOutCounter > 0) //RESETS FADE-OUT if running
            {
               fadeOutCounter = 0;
               gainStep = abs(lastGainInValue - lastGainOutValue) / fadeInSteps;
               lastGainInValue = lastGainOutValue;

            }
            // FADE-IN INITIALIZATION
            gainStep = lastGainInValue / fadeInSteps; // Calculates the amount of which the gain will be increased at each step of the fade in
            fadeInCounter = fadeInSteps;
            gainSlider.setValue(0);
            player->start();
            /*button->setButtonText("PAUSE");*/
            playerState = PlayerState::play;
         }
         else // IMMEDIATE START
         {
            player->start();
            playerState = PlayerState::play;
            
            /*button->setButtonText("PAUSE");*/
            fadeInCounter = 0;
            fadeOutCounter = 0;
            lastGainInValue = gainSlider.getValue();
            
         }
      }
      else    // IMMEDIATE PAUSE
      {
         playerState = PlayerState::pause;
         player->stop();
         //button->setButtonText("PLAY");
         playerState = PlayerState::pause;
         fadeInCounter = 0;
         fadeOutCounter = 0;
         gainSlider.setValue(lastGainInValue);
      }
   }

   if (button == &stopButton)
   {
      playerState = PlayerState::stop;
      stopAndReset();

   }

   if (button == &fadeInPLAY && (!player->isPlaying() || fadeOutCounter>0) && wavefromDisplay.checkIfFileLoaded())
   {
      DBG("   * playButton!   MainComponent::buttonClicked");
      player->start();
      playerState = PlayerState::play;
    
      // FADE IN activation logic - runs only when FADE-IN ticked, or if at the beginning of the file in case START/END only ticked
      

      lastGainInValue = gainSlider.getValue(); // Storing lastGainInValue before play button clicked
      
      if (fadeOutCounter > 0) // stops FADE OUT in case FADE IN itialized when the FADE OUT is running 
      {
         fadeOutCounter = 0;
         gainStep = abs(lastGainInValue - lastGainOutValue) / fadeInSteps;
         gainSlider.setValue(lastGainInValue);
         lastGainInValue = lastGainOutValue;
         

      }
      else
      {
         // FADE IN starts
         gainStep = lastGainInValue / fadeInSteps; // Calculates the amount of which the gain will be increased at each step of the fade in
         gainSlider.setValue(0);
         
      }
      fadeInCounter = fadeInSteps;
   }

   if (button ==&fadeOutSTOP && player->isPlaying())
   {
       DBG("   * stopButton!    MainComponent::buttonClicked");
       if (fadeOutCounter == 0)
       {

          // Reset of the FADE-IN
          if (fadeInCounter > 0)
          {
             fadeInCounter = 0;
             gainStep = abs(lastGainInValue - lastGainOutValue) / fadeOutSteps;
             lastGainOutValue = lastGainInValue;
          }
          // FADE-OUT START
          lastGainOutValue = gainSlider.getValue(); // Storing lastGainOutValue before play button clicked
          gainStep = lastGainOutValue / fadeOutSteps; // Calculates the amount of which the gain will be increased at each step of the fade in
          fadeOutCounter = fadeOutSteps; // ACTUAL INITIALIZATION
       }
       else 
       {
         if (fadeOutCounter == 0)
         player->stop();
         playerState = PlayerState::stop;
          
       }
   }


   if (button == &loadButton)
   {
       
       DBG("   * loadButton!    MainComponent::buttonClicked");
       // - configure the dialogue
       auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
       
       
       /*/// ==== WIP -FOR DEVELOPMENT ONLY!!!!!!!!!
       juce::File myfile("C:/_MOJE/_1_MOJE/CS Computer Science/UoL/2022 October/OOP/uol_oop_tracks/tracks/aon_inspired.mp3");
       juce::URL chosenFile(myfile);
       player->loadURL(juce::URL{ chosenFile });
       wavefromDisplay.loadURL(juce::URL{ chosenFile });*/


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
   if (playerState==PlayerState::stop)
   {
      playPauseButton.setButtonText("STOP STATE - PLAY/PAUSE");
   }
   if (playerState == PlayerState::play)
   {
      playPauseButton.setButtonText("PLAY STATE - PAUSE");
      playPauseButton.setAlpha(1);
   }
   
   if (playerState == PlayerState::pause)
   {
      playPauseButton.setButtonText("PAUSE STATE - PLAY");
   }

   // UPDATES PLAYHEAD POSITION TO THE PLAYER POSITION   
   wavefromDisplay.setPositionRelative(
         player->getPositionRelative());
      

   // UPDATES PLAYHEAD POSITION TO MOUSE POSITON
   if (wavefromDisplay.getIfMouseIsDragging() || wavefromDisplay.checkIfMouseDown())
   {
      mouseXRelativeDragOverWaveform = wavefromDisplay.getMousePosX() / wavefromDisplay.getWidth();
      player->setPositionRelative(mouseXRelativeDragOverWaveform);
   }


   //========= DRAGGING PLAYHEAD LOGIC ============
   // WHEN MOUSE BUTTON HELD PRESSED OVER THE WAVEFORM - PLAYING PAUSES
   if (wavefromDisplay.checkIfMouseDown())
   {
      player->stop();
      playerState = PlayerState::pause;
      mouseDown = true;
   }

   // WHEN MOUSE BUTTOBN RELEASED - PLAYER PLAYS AGAIN only IF WAS PLAYING BEFORE
   if (!wavefromDisplay.checkIfMouseDown() && mouseDown && playerState == PlayerState::play)
   {
      mouseDown = false;
      player->start();
      playerState = PlayerState::play;
   }
   
   // WHEN MOUSE IS DRAGGED - then the player playes
   if (wavefromDisplay.getIfMouseIsDragging() )
   {
      player->start();
      playerState = PlayerState::play;
      mouseDown = true;
      mouseDragging = true;
   }

   // WHEN MOUSE DRAGGED - if was PLAYING BEFORE then it will continue playing, otherwise PLAYING WILL PAUSE or STOP DEPENDING ON PREVIOUS STATE
   if (!wavefromDisplay.getIfMouseIsDragging() && mouseDragging && playerState!=PlayerState::play)
   {
      mouseDragging = false;
      player->stop();
      playerState = PlayerState::pause;
   }

   


   // ======== FADE IN and FADE OUT LOGIC =================

   /// FADE IN - counter
   if (fadeInCounter > 0)
   {
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
         playerState = PlayerState::pause;
            
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

   // END FADE-OUT
      // FADE OUT initialization at the END
   if (player->timeToEnd()<fadeOutTime && player->timeToEnd()>0 && atStartAndEndOnly.getToggleState()&& !approachingEnd)
   {
      approachingEnd = true;
      lastGainOutValue = gainSlider.getValue();
      fadeOutCounter = ceil(player->timeToEnd() * 1000 / timerStep);
      fadeInCounter = 0; // stops FADE IN in case it runs when FADE OUT initiated
      DBG("DecKGUI::timecaller   FADEing OUT - END" );
   }

   if (player->getPositionRelative() >0.99f)
   {
      stopAndReset();
   }
}

void DeckGUI::stopAndReset()
{
   
   player->setPosition(0);
   
   fadeInCounter = 0;
   fadeInCounter = 0;
   fadeOutCounter = 0;
   gainSlider.setValue(lastGainInValue);

   player->stop();
   playerState = PlayerState::stop;
}

void DeckGUI::loadIncomingFile(juce::File & sentFile)
{
   juce::URL chosenFile(sentFile);
   player->loadURL(juce::URL{ chosenFile });
   wavefromDisplay.loadURL(juce::URL{ chosenFile });
   
}
