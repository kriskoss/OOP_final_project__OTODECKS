/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 28 Feb 2023 2:30:35pm
    Author:  krzys

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================
//PlaylistComponent.cpp
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player, DeckGUI* _deck1,
   DeckGUI* _deck2,
   juce::AudioFormatManager& formatManagerToUse,
   juce::AudioThumbnailCache& cacheToUse
):player(_player),  
   deck1(_deck1),
   deck2(_deck2),
   wavefromDisplay(formatManagerToUse, cacheToUse)
{
     // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
   
   tableComponet.getHeader().addColumn("LEFT", 1, 80);
   tableComponet.getHeader().addColumn("Track title", 3, 400);
   tableComponet.getHeader().addColumn("RIGHT", 4, 80);
   tableComponet.getHeader().addColumn("Remove", 2, 80);
   tableComponet.setModel(this);
   
   addAndMakeVisible(tableComponet);
   addAndMakeVisible(addFileButton);

   addFileButton.addListener(this);

   startTimer(timerStep);
}

PlaylistComponent::~PlaylistComponent()
{
   stopTimer();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
   DBG("PlaylistComponent::resized() CALLED!");

   // This method is where you should set the bounds of any child
    // components that your component contains..
   int rowH = getHeight() / 10;
   addFileButton.setBounds(0,0,getWidth(), rowH);
   tableComponet.setBounds(0, rowH, getWidth(), 9*rowH);
}

int PlaylistComponent::getNumRows()
{
   return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,int rowNumber,int width,int height,bool rowIsSelected)
{
   if (rowIsSelected)
   {
      g.fillAll(juce::Colours::orange);
   }
   else
   {
      g.fillAll(juce::Colours::darkgrey);
   }
}

void PlaylistComponent::paintCell(juce::Graphics& g,int rowNumber,int columnId,int width,int height,bool rowIsSelected) 
{
   g.drawText(trackTitles[rowNumber], 
               1, 0, 
               width - 4, height, 
               juce::Justification::centred, 
               true);
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId,bool isRowSelected,juce::Component* existingComponentToUpdate)
{
   // PLAY on LEFT deck
   if (columnId == 1) 
   {
      if (existingComponentToUpdate == nullptr)
      {
         juce::TextButton* btn = new juce::TextButton{ "PLAY-L" };
         //juce::String id{ std::to_string(rowNumber) };
         juce::String id{ "PLAY-L," + std::to_string(rowNumber)};
         btn->setComponentID(id);

         btn->addListener(this);
         existingComponentToUpdate = btn;
      }
   }

   // REMOVE column
   if (columnId == 2)
   {
      if (existingComponentToUpdate == nullptr)
      {
         juce::TextButton* btn = new juce::TextButton{ "REMOVE" };
         //juce::String id{ std::to_string(rowNumber) };
         juce::String id{ "REMOVE," + std::to_string(rowNumber)};
         btn->setComponentID(id);

         btn->addListener(this);
         existingComponentToUpdate = btn;
      }
   }

   // PLAY on RIGHT deck
   if (columnId == 4)
   {
      if (existingComponentToUpdate == nullptr)
      {
         juce::TextButton* btn = new juce::TextButton{ "PLAY-R" };
         //juce::String id{std::to_string(rowNumber)};
         juce::String id{ "PLAY-R," + std::to_string(rowNumber) };
         btn->setComponentID(id);

         btn->addListener(this);
         existingComponentToUpdate = btn;
      }
   }

   return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(juce::Button* button)
{
   
   if (button == &addFileButton && loadingThumbnail==false)
   {  // ADDBUTTON PRESSED
      
      DBG("PlaylistComponent::buttonClicked ADDING ITEM");
      
      wavefromDisplay.newFileLoaded = false;
      newFileAdded = true;
      
      if (false) /// LOAD SPECIFIC FILE ==== WIP - FOR DEVELOPMENT ONLY - load file automatically!!!!!!!!!
      {
         std::string path = "C:/_MOJE/_1_MOJE/CS Computer Science/UoL/2022 October/OOP/uol_oop_tracks/tracks/aon_inspired.mp3";
         juce::File chosenFile(path);
         sendFileData(chosenFile);
      }
      else  // FILE CHOOSER 
      {
         auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;// - configure the dialogue
         player->fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)// LAMBDA FUNCTION- launch out of the main thread
            {
               juce::File chosenFile = chooser.getResult();
               sendFileData(chosenFile);
            }
         );
      }
   }
   else
   {  //ANY BUTTON INSIDE THE TABLE PRESSED
      //int id = std::stoi(button->getComponentID().toStdString());
      //DBG("PlaylistComponent::buttonClicked - clicked" + trackTitles[id] + ", ComponentID " + id);
      std::string id = button->getComponentID().toStdString();
      DBG("PlaylistComponent::buttonClicked - ComponentID: " + id);
      
      std::vector<std::string> idTokenised = Helpers::tokenise(id, ',');
      for (int i=0;i<idTokenised.size();++i)
      {
         if (i == 0) {
            if (idTokenised[i] == "REMOVE") // CELL's "REMOVE" button pressed
            {
               DBG("PlaylistComponent::buttonClicked: removing element: " + idTokenised[1]);
               trackTitles.erase(trackTitles.begin() + std::stoi(idTokenised[1]));
               loadedFiles.erase(loadedFiles.begin() + std::stoi(idTokenised[1]));
               tableComponet.updateContent();
            }
         }
         // LOAD playlist item to DECK 1
         if (idTokenised[i] == "PLAY-L")
         {
            DBG("PlaylistComponent::buttonClicked: sending file: "+ idTokenised[1] + " to DECK1: " );
            int index = std::stoi(idTokenised[1]);
            deck1->loadIncomingFile(loadedFiles[index]);
            
         }
         // LOAD playlist item to DECK 2
         if (idTokenised[i] == "PLAY-R")
         {
            DBG("PlaylistComponent::buttonClicked: sending file: " + idTokenised[1] + "to DECK2: ");
            int index = std::stoi(idTokenised[1]);
            deck2->loadIncomingFile(loadedFiles[index]);

         }
      }
   }
}

void PlaylistComponent::timerCallback()
{
   if (newFileAdded)
   {
      //FILE OPENED SUCCESSFULLY 
      if (wavefromDisplay.newFileLoaded)
      {
         newFileAdded = false;      // TRUE only for one iteration - only to enable table content update
         tableComponet.updateContent();
         
         loadingThumbnail = true; // WHEN newFileAdded detected - loading od the THUMBNAIL STARTS
         DBG("PlaylistComponent::timerCallback--> newFileAdded = false;");
         
      }
   }
   // THUMBNAIL READY
   if (wavefromDisplay.checkIfThumbnailFullyLoaded() && loadingThumbnail)
   {
      loadingThumbnail = false; // Thumb loading has FINISHED

      DBG("PlaylistComponent::timerCallback  THUMBNAI FULLY LOADED;");

      int track_index = wavefromDisplay.tracksBeingLoaded.front();
      trackTitles[track_index] = loadedFiles[track_index].getFileName().toStdString();
      //trackTitles[0] = "Loaded!";
      wavefromDisplay.tracksBeingLoaded.pop();
      DBG("PlaylistComponent::timerCallback  QUEUE size:" + std::to_string(wavefromDisplay.tracksBeingLoaded.size()));
      repaint();
      addFileButton.setButtonText(addFileButtonText);
   }

   if (loadingThumbnail)
   {
      std::string s = "Pleas wait...File load progress: ";
      s.append(std::to_string(wavefromDisplay.thumbnailLoadProgress));
      s.append("%");
      addFileButton.setButtonText(s);
   }
   else
   {
      
      
   }
   
   
}


//================= HELPERS======================
void PlaylistComponent::sendFileData(juce::File & chosenFile)
{
   loadedFiles.push_back(chosenFile);
   DBG("PlaylistComponent::sendFileData: " + std::to_string(loadedFiles.size()));
   
   int trackNum = trackTitles.size();
   trackTitles.push_back("Loading...");
   //trackTitles.push_back((chosenFile.getFileName()).toStdString());
   juce::URL chosenFileURL(chosenFile);
   player->loadURL(juce::URL{ chosenFile });
   wavefromDisplay.loadURL(juce::URL{ chosenFile },trackNum);
}