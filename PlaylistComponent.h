/*
  ==============================================================================

    PlaylistComponent.h
    Created: 28 Feb 2023 2:30:35pm
    Author:  krzys

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
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::Timer,
                           public juce::TextEditor::Listener
{
public:
   PlaylistComponent(DJAudioPlayer* _player,
                     DeckGUI* _deck1,
                     DeckGUI* _deck2,
                     juce::AudioFormatManager& formatManagerToUse,
                     juce::AudioThumbnailCache& cacheToUse);

    ~PlaylistComponent() override;

    // ======= PURE VIRTUAL FUNCTIONS =======
      // * Component
    void paint (juce::Graphics& g) override;
    void resized() override;

      // * TableListBoxModel
    int getNumRows() override;
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate);

      // * Button
    void buttonClicked(juce::Button* button) override;
    // ========================================

      // * TextEditor
    void textEditorTextChanged(juce::TextEditor& tc);

    juce::SparseSet<int> hiddenRows;

     
private:
   DJAudioPlayer* player;
   DeckGUI* deck1;
   DeckGUI* deck2;

   WaveformDisplay wavefromDisplay;
   
   juce::TableListBox tableComponet;
   
   std::string addFileButtonText = "Select file...";
   juce::TextButton addFileButton{ addFileButtonText };
   juce::TextButton searchButton{ "SEARCH TEST"};
   juce::TextButton loadedItemsButton{ "SHOW LOADED ITEMS"};
   juce::TextEditor searchField{"SEARCH PLAYLIST"};


   void timerCallback() override;
   int timerStep = 500;
   
   
   std::vector<std::string> trackTitles; 
   std::vector<std::string> trackTitlesOriginal; 
   std::vector<std::string> searchedTrackTitles{};
   

   std::vector<juce::File> loadedFiles; 
   std::vector<juce::File> searchedloadedFiles; 
   std::vector<juce::File> loadedFilesOriginal{};
   
   
   std::vector<std::string> paths; 
   std::queue<std::string> toBeUploadedQueue;

   //FLAGS
   bool newFileAdded = false;       /** USED ONLY FOR UPDATING THE PLAYLIST - It is true only when new file was added to the playlist - becomes FALSE as soon as the PLAYLIST component display updated*/
   bool loadingThumbnail = false;   /** TRUE ONLY WHEN THUMBNAIL LOADING IS IN PROGRESS */
   
   

      
      // HELPERS
   
   void sendFileData(juce::File& chosenFile); /** Sends selected file to be stored and processed to be ready to use by decks*/
   std::queue<std::string> populateTheQueue(std::vector<std::string> vector_of_strings);  /** Transfers data from the vector to the queue */
   

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
