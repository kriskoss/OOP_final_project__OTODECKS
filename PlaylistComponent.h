/*
  ==============================================================================

    PlaylistComponent.h
    Created: 28 Feb 2023 2:30:35pm
    Author:  Candidate No. EX2765

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Helpers.h"
#include "DJAudioPlayer.h" 
#include "DeckGUI.h"

#include "WaveformDisplay.h"
#include <queue>


//==============================================================================
/*
*/
//PlaylistComponent.h
class PlaylistComponent : public juce::Component,
                     public juce::TableListBoxModel,
                     public juce::Button::Listener,
                     public juce::Timer,
                     public juce::TextEditor::Listener
                    
{
public:
   PlaylistComponent(Settings * settings, DJAudioPlayer* _player,
      DeckGUI* _deck1,
      DeckGUI* _deck2,
      juce::AudioFormatManager& formatManagerToUse,
      juce::AudioThumbnailCache& cacheToUse);

   ~PlaylistComponent() override;

   // ======= PURE VIRTUAL FUNCTIONS =======
     // * Component
   void paint(juce::Graphics& g) override;
   void resized() override;

   // * TableListBoxModel
   int getNumRows() override;
   void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
   void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

   juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate);

   // * Button
   void buttonClicked(juce::Button* button) override;
   // ========================================
   Settings* settings;

     // * TextEditor
   void textEditorTextChanged(juce::TextEditor& tc);


private:
   // OBJECTS/ OBJECT REFERENCES
   DJAudioPlayer* player;
   DeckGUI* deck1;
   DeckGUI* deck2;
   WaveformDisplay wavefromDisplay;
   juce::TableListBox tableComponet;

   // PRIVATE FUNCTIONS
   void timerCallback() override;
   void sendFileData(juce::File& chosenFile); /** Sends selected file to be stored and processed to be ready to use by decks*/
   void saveThePlaylist();/** Saves the playlist to the disk*/
   void updateAndSavePlaylistData(); /** Update the playlist data and stores it on the disk*/
   void loadThePlaylist();
   void createClearPlaylistAlertWindow();
   void addNewFiles();
juce::Component* createButtonInsideCell(int selectedColumn, 
                                          std::string buttonLabel, 
                                          std::string idElement, 
                                          juce::Component* existingComponentToUpdate, 
                                          int rowNumber, 
                                          int columnId);
   

   // Includes SETTINGS
   std::string addFileButtonText = settings->addFileButtonText;
   juce::TextButton loadFilesButton{ addFileButtonText };
   juce::TextEditor searchField{ settings->searchFieldText };
   juce::TextButton clearSearchButton{ settings->clearSearchButtonLabel};
   juce::TextButton clearPlaylistButton{ settings->clearPlaylistButtonLabel };
   int timerStep = settings->playlistTimerStep;
   int columnWidth = settings->columnWidth;

   juce::Colour playlistBackgroundColor = settings->playlistBackgroundColor;
   
   // PRIVATE DATA MEMBERS
   std::vector<std::string> trackTitles{};
   std::vector<std::string> trackTitlesMain{};
   std::vector<std::string> searchedTrackTitles{};

   std::vector<juce::File> loadedFiles{};
   std::vector<juce::File> loadedFilesMain{};
   std::vector<juce::File> searchedloadedFiles{};
   
   std::vector<int> trackTotalTimes{};
   std::vector<int> trackTotalTimesMain{};
   std::vector<int> searchedTrackTotalTimes{};

   
   std::vector<std::string> paths;
   std::queue<std::string> toBeUploadedQueue;
   std::queue<std::string> populateTheQueue(std::vector<std::string> vector_of_strings);  /** Transfers data from the vector to the queue */

   std::string loadToDeck1ID = "IDloadToDeck1";
   std::string loadToDeck2ID = "IDloadToDeck2";
   std::string removeTrackID = "REMOVE";


   //FLAGS
   bool newFileAdded = false;       /** USED ONLY FOR UPDATING THE PLAYLIST - It is true only when new file was added to the playlist - becomes FALSE as soon as the PLAYLIST component display updated*/
   bool loadingThumbnail = false;   /** TRUE ONLY WHEN THUMBNAIL LOADING IS IN PROGRESS */

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
