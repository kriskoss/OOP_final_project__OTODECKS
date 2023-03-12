/*
   ==============================================================================

      WaveformDisplay.h
      Created: 24 Feb 2023 2:55:55pm
      Author:  Candidate No. EX2765

   ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Settings.h"
#include <vector>
#include <iostream>
#include <queue>

//==============================================================================
/*
*/
//WaveformDisplay.h
class WaveformDisplay : public juce::Component,
   public juce::ChangeListener,
   public juce::MouseListener
{
public:
   WaveformDisplay(Settings *_settings,
      juce::AudioFormatManager& formatManagerToUse,
      juce::AudioThumbnailCache& cacheToUse
   );
   ~WaveformDisplay() override;
   // ============= VIRTUAL FUNCTIONS =================
   void paint(juce::Graphics&) override;
   void resized() override;

   void changeListenerCallback(juce::ChangeBroadcaster* source) override;

   void mouseDrag(const juce::MouseEvent& e) override;
   void mouseDown(const juce::MouseEvent& e) override;
   void mouseUp(const juce::MouseEvent& e) override;
   bool getIfMouseIsDragging();
   bool checkIfMouseDown();
   
   // =================================================
   Settings* settings;
   
   juce::Colour waveformColor = settings->waveformColor;
   juce::Colour fileNotLoadedColor = settings->fileNotLoadedColor;

   juce::Colour playheadColor = settings->playheadColorType;
   juce::Colour areaBehindPlayhead= settings->areaBehindPlayhead;
   juce::Colour areaToBePlayed = settings->areaToBePlayed;
   
   

   
   //FUNCTIONS
   /** Upoloads the file - responsible for calling thumbnail generation*/
   void loadURL(juce::URL audioURL);

   /** Upoloads the file - responsible for calling thumbnail generation. Additionally it pushes the track number into the QUEUE to monitor keep track of available tracks in the playlist*/
   void loadURL(juce::URL audioURL, int trackNum);

   /** set the relative position of the playhead on the waveform display to match the location in the player */
   void setPositionRelative(double pos);

   bool checkIfFileLoaded(); /** returns true if file is loaded, fales otherwise*/
   bool checkIfThumbnailFullyLoaded();

   /** Gets the x-coordinate of the mouse over the waveform dispaly - used to calcuate the relative postion in the song and set the playhead accordingly*/
   float getMousePosX();

   /** Sets the flag to flase - used by the playlist to indicate that it is ready to load next files*/
   void setNewFileLoadedToFalse();
   /** Checks if new file was loaded by the file chooser*/
   bool getNewFileLoaded();

   //DATA MEMBERS
   float mousePosX = -1;

   /**QUEUE of tracks which are waiting to be uploaded to the playlist*/
   std::queue<int> tracksBeingLoaded;
   
   /** Variable used to indicate the thumbnail generation progress*/
   int thumbnailLoadProgress = 0;

   int getTracktTotalTime();

private:
   juce::AudioThumbnail audioThumb;

   //FLAGS
   bool newFileLoaded = false;
   bool mouseIsDown = false;
   bool mouseIsDraggingOverWaveform = false;
   bool anyFileLoaded = false;
   double position = -1;



   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
