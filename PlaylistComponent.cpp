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
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player,DeckGUI* _deck1,DeckGUI* _deck2,juce::AudioFormatManager& formatManagerToUse,juce::AudioThumbnailCache& cacheToUse):player(_player),  
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
   addAndMakeVisible(searchButton);
   addAndMakeVisible(loadedItemsButton);
   addAndMakeVisible(searchField);

   addFileButton.addListener(this);
   searchButton.addListener(this);
   loadedItemsButton.addListener(this);
   loadedItemsButton.addListener(this);
   searchField.addListener(this);

   paths = { "C:/_MOJE/_1_MOJE/CS Computer Science/UoL/2022 October/OOP/uol_oop_tracks/tracks/stomper_reggae_bit.mp3",
               "C:/_MOJE/_1_MOJE/CS Computer Science/UoL/2022 October/OOP/uol_oop_tracks/tracks/fast_melody_thing.mp3",
               "C:/_MOJE/_1_MOJE/CS Computer Science/UoL/2022 October/OOP/uol_oop_tracks/tracks/twindrive.mp3",
               "C:/_MOJE/_1_MOJE/CS Computer Science/UoL/2022 October/OOP/uol_oop_tracks/tracks/stomper1.mp3",
               "C:/_MOJE/_1_MOJE/CS Computer Science/UoL/2022 October/OOP/uol_oop_tracks/tracks/fast_melody_regular_drums.mp3",
               "C:/_MOJE/_1_MOJE/CS Computer Science/UoL/2022 October/OOP/uol_oop_tracks/tracks/bleep_2.mp3" };
   toBeUploadedQueue = populateTheQueue(paths);
   
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
   tableComponet.setBounds(0, rowH, getWidth(), 7*rowH);
   searchButton.setBounds(0, 8*rowH, getWidth()/2, rowH);
   loadedItemsButton.setBounds(getWidth()/2, 8*rowH, getWidth() / 2, rowH);
   searchField.setBounds(0, 9*rowH, getWidth(), 1 * rowH);
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
   if (button == &loadedItemsButton)
   {
      DBG("\nPlaylistComponent::buttonClicked -> LOADED ITEMS: ");
      for (int i = 0; i < loadedFiles.size(); ++i)
      {
         DBG("PlaylistComponent::buttonClicked -> loadedItems: "  + loadedFiles[i].getFileName() + "\t   [" + std::to_string(i) + "]");
      }


      DBG("\nPlaylistComponent::buttonClicked -> TRACKTITLES: ");
      for (int i = 0; i < trackTitles.size(); ++i)
      {
         DBG("PlaylistComponent::buttonClicked -> trackTitles: [" + std::to_string(i) + "]  "+ trackTitles[i]  );
      }
   }
   
   if (button == &searchButton)
   {
      //hiddenRows.addRange(juce::Range<int>(0, 1));
      //hiddenRows.addRange(juce::Range<int>(2, 3));
      //tableComponet.setSelectedRows(hiddenRows);
      
      // STORING FULL PLAYLIST DATA
      
      searchedTrackTitles.clear();
      searchedloadedFiles.clear();
      
      for (int i=0; i<trackTitles.size();++i)
      {
         if (i%2==0)
         { 
            searchedTrackTitles.push_back(trackTitles[i]);
            searchedloadedFiles.push_back(loadedFiles[i]);
         }
      }
      trackTitles = searchedTrackTitles;
      loadedFiles = searchedloadedFiles;

      std::vector<std::string> searchedTrackTitles;
      tableComponet.updateContent();
      int tr = 0;
      for (std::string s:trackTitles)
      {
         tr++;
         DBG("PlaylistComponent::buttonClicked -> searchBUTTON: track: " + s  + "  " + std::to_string(tr)); 
      }
      
   }
   if (button == &addFileButton && loadingThumbnail==false)
   {  // ADDBUTTON PRESSED
      
      DBG("PlaylistComponent::buttonClicked ADDING ITEM");
      
      wavefromDisplay.newFileLoaded = false;
      newFileAdded = true;

      if (!loadedFilesOriginal.empty() && !trackTitles.empty())
      {
         trackTitles = trackTitlesOriginal;
         loadedFiles = loadedFilesOriginal;
         tableComponet.updateContent();
      }
      
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

               trackTitlesOriginal = trackTitles; // Makes a copy to use it when calling SEARCH
               loadedFilesOriginal = loadedFiles; // Makes a copy to use it when calling SEARCH
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

   
   /// UPLOAD STORED TRACKS
   while (!toBeUploadedQueue.empty() && !loadingThumbnail)
   {
      wavefromDisplay.newFileLoaded = false;
      newFileAdded = true;

      juce::File chosenFile(toBeUploadedQueue.front());
      sendFileData(chosenFile);
      loadingThumbnail = true;
      toBeUploadedQueue.pop();
   };



   //FILE OPENED SUCCESSFULLY - UPDATE PLAYLIST FOR NEW ITEM
   if (newFileAdded)
   {
      if (wavefromDisplay.newFileLoaded)
      {
         newFileAdded = false;      // TRUE only for one iteration - only to enable table content update
         tableComponet.updateContent();
         
         
         loadingThumbnail = true; // WHEN newFileAdded detected - loading od the THUMBNAIL STARTS
      }
   }

   // THUMBNAIL READY - UPDATING TRACK NAME IN THE PLAYLIST
   if (wavefromDisplay.checkIfThumbnailFullyLoaded() && loadingThumbnail)
   {
      loadingThumbnail = false; // Thumbnail loading has FINISHED

      DBG("PlaylistComponent::timerCallback  THUMBNAI FULLY LOADED;"); // DO NOT REMOVE

      int track_index = wavefromDisplay.tracksBeingLoaded.front(); //
      trackTitles[track_index] = loadedFiles[track_index].getFileName().toStdString();
      
      wavefromDisplay.tracksBeingLoaded.pop();
      repaint();
      
      addFileButton.setButtonText(addFileButtonText);

      trackTitlesOriginal = trackTitles; // Makes a copy to use it when calling SEARCH
      loadedFilesOriginal = loadedFiles; // Makes a copy to use it when calling SEARCH
   }

   // THUMBNAIL LOADING PROGRESS 
   if (loadingThumbnail)
   {
      std::string s = "Pleas wait...File load progress: ";
      s.append(std::to_string(wavefromDisplay.thumbnailLoadProgress));
      s.append("%");
      addFileButton.setButtonText(s);
   }
}


//================= HELPERS======================
void PlaylistComponent::sendFileData(juce::File & chosenFile)
{
   loadedFiles.push_back(chosenFile);     // File stored in the array
   
   int trackNum = trackTitles.size();     // Gets the file number
   trackTitles.push_back("Loading...");   // Initally "Loading" message will appar as a track name - only when thumbnail is ready then the correct the track name will replace loading message. This happens
   juce::URL chosenFileURL(chosenFile);
   player->loadURL(juce::URL{ chosenFile });
   wavefromDisplay.loadURL(juce::URL{ chosenFile },trackNum);
}

std::queue<std::string> PlaylistComponent::populateTheQueue(std::vector<std::string> s_vector)
{
   std::queue<std::string> s_queue;
   for (std::string s: s_vector)
   {
      s_queue.push(s);
   }
   return s_queue;
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor& t)
{
   
   //MAKE FUNCITON: INPUT vector of strings, RETURN: vector of integers - the indices of the input vector matching strings
   std::string searchFor = t.getText().toStdString();
   DBG("\nPlaylistComponent::textEditorTextChanged:  TEXT CHANGED->new text is: " + searchFor);
   
   searchedTrackTitles.clear();
   searchedloadedFiles.clear();
   trackTitles = trackTitlesOriginal;  
   loadedFiles = loadedFilesOriginal;

   for (int i = 0; i < trackTitles.size(); ++i)
   {
      
      size_t found = trackTitles[i].find(searchFor);
      //DBG("PlaylistComponent::textEditorTextChanged: FOUND value: " + std::to_string(found));
      
      if (found >= 0 && found < trackTitles[i].size())
      {
         DBG("PlaylistComponent::textEditorTextChanged: ITEMS FOUND IN " + trackTitles[i] + "[" + std::to_string(i)+"]");
         searchedTrackTitles.push_back(trackTitles[i]);
         searchedloadedFiles.push_back(loadedFiles[i]);
      }
   }
   trackTitles = searchedTrackTitles;
   loadedFiles = searchedloadedFiles;
   tableComponet.updateContent();
   
}