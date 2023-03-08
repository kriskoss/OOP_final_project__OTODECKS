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



//==============================================================================
/*
*/
//PlaylistComponent.h
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::Timer
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
    
    DJAudioPlayer* player;
    DeckGUI* deck1;
    DeckGUI* deck2;

    WaveformDisplay wavefromDisplay;

    std::string addFileButtonText = "Select file...";
    juce::TextButton addFileButton{ addFileButtonText };
    
private:
   

   void timerCallback() override;
   int timerStep = 500;
   
   
   juce::TableListBox tableComponet;
   
   std::vector<std::string> trackTitles;
   bool newFileAdded = false; /** USED ONLY FOR UPDATING THE PLAYLIST - It is true only when new file was added to the playlist - becomes FALSE as soon as the PLAYLIST component display updated*/
   bool loadingThumbnail = false; /** TRUE ONLY WHEN THUMBNAIL LOADING IS IN PROGRESS */
   // Helpers
   void sendFileData(juce::File& chosenFile);
   std::vector<juce::File> loadedFiles; // WIP - it is going to be a vector of loaded files including thumbnail and meta data
   
   
   
   

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
