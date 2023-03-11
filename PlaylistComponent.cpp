/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 28 Feb 2023 2:30:35pm
    Author:  Candidate No. EX2765

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <iostream>
#include <fstream>


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
   addAndMakeVisible(loadFilesButton);
   addAndMakeVisible(searchField);
   addAndMakeVisible(clearSearchButton);
   addAndMakeVisible(clearPlaylistButton);

   loadFilesButton.addListener(this);
   searchField.addListener(this);
   clearSearchButton.addListener(this);
   clearPlaylistButton.addListener(this);

   searchField.setTextToShowWhenEmpty("Search", juce::Colours::grey);

   loadThePlaylist(); // Loads the playlist from hard drive
   
   toBeUploadedQueue = populateTheQueue(paths);
   
   startTimer(timerStep);

}

PlaylistComponent::~PlaylistComponent()
{
   stopTimer();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
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
   int rowH = getHeight() / 9;
   loadFilesButton.setBounds(0,0,getWidth(), rowH);
   tableComponet.setBounds(0, rowH, getWidth(), 7*rowH);
   searchField.setBounds(0, 8*rowH, getWidth()/3, rowH);
   clearSearchButton.setBounds(getWidth()/3, 8 * rowH, getWidth() / 6, rowH);
   clearPlaylistButton.setBounds(getWidth()*3/4, 8*rowH, getWidth() / 4, rowH);
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
   // Create "Load to Deck1" buttons
      existingComponentToUpdate = createButtonInsideCell(1,"Load to Deck1", loadToDeck1ID, existingComponentToUpdate, rowNumber, columnId);

   // Create "REMOVE" buttons
      existingComponentToUpdate = createButtonInsideCell(2,"REMOVE", removeTrackID, existingComponentToUpdate, rowNumber, columnId);

   // Create "Load to Deck2" buttons
      existingComponentToUpdate = createButtonInsideCell(4,"Load to Deck2", loadToDeck2ID, existingComponentToUpdate, rowNumber, columnId);
   
   return existingComponentToUpdate;
   
}


void PlaylistComponent::buttonClicked(juce::Button* button)
{
   // CLEAR SEARCH button
   if (button == &clearSearchButton)
   {
      searchField.setText("");
   }
   
   //CLEAR PLAYLIST button
   if (button == &clearPlaylistButton && !loadedFiles.empty()) // ACTIVE ONLY WHEN: there are any files loaded already
   {
      createClearPlaylistAlertWindow();
   }
   
   // LOAD FILES button 
   if (button == &loadFilesButton && loadingThumbnail==false) // ACTIVE ONLY WHEN : previous files upload complete
   {  
      addNewFiles();
   }

   
   //ANY BUTTON INSIDE THE TABLE PRESSED
      
   std::string id = button->getComponentID().toStdString();
   DBG("PlaylistComponent::buttonClicked - ComponentID: " + id);
      
   std::vector<std::string> idTokenised = Helpers::tokenise(id, ',');
   for (int i=0;i<idTokenised.size();++i)
   {
      if (i == 0) {
         if (idTokenised[i] == removeTrackID && !loadingThumbnail) // CELL's "REMOVE" button pressed - button is not active when thumbnail is being genereated
         {
            DBG("PlaylistComponent::buttonClicked: removing element: " + idTokenised[1]);
            trackTitles.erase(trackTitles.begin() + std::stoi(idTokenised[1]));
            loadedFiles.erase(loadedFiles.begin() + std::stoi(idTokenised[1]));
            tableComponet.updateContent();

            updateAndSavePlaylistData();
               
         }
      }
      // LOAD playlist item to DECK 1
      if (idTokenised[i] == loadToDeck1ID)
      {
         DBG("PlaylistComponent::buttonClicked: sending file: "+ idTokenised[1] + " to DECK1: " );
         int index = std::stoi(idTokenised[1]);
         deck1->loadIncomingFile(loadedFiles[index]);
         
         // RESTARTING PLAYER in DECK 1 when new file loaded
         deck1->stopAndReset();
      }
      // LOAD playlist item to DECK 2
      if (idTokenised[i] == loadToDeck2ID)
      {
         DBG("PlaylistComponent::buttonClicked: sending file: " + idTokenised[1] + "to DECK2: ");
         int index = std::stoi(idTokenised[1]);
         deck2->loadIncomingFile(loadedFiles[index]);
         
         // RESTARTING PLAYER in DECK 2 when new file loaded
         deck2->stopAndReset();

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
      
      std::ifstream f(toBeUploadedQueue.front()); // TESTING FILE PATH - if incorrect it will not be processed (WIP)
      if (f.is_open())
      {
         sendFileData(chosenFile);
         loadingThumbnail = true;
      }
      
      else
      {
         DBG("ERROR PlaylistComponent::timerCallback() - ERROR: BAD PATH!\n");

      };
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
      
      loadFilesButton.setButtonText(addFileButtonText);

      updateAndSavePlaylistData();
   }

   // THUMBNAIL LOADING PROGRESS 
   if (loadingThumbnail)
   {
      std::string s = "Pleas wait...File load progress: ";
      s.append(std::to_string(wavefromDisplay.thumbnailLoadProgress));
      s.append("%");
      loadFilesButton.setButtonText(s);
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
   trackTitles = trackTitlesMain;  
   loadedFiles = loadedFilesMain;

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

void PlaylistComponent::saveThePlaylist()
{
   // SAVING PLAYLIST ON THE DISK (WIP
      // Open an output file stream  /// REFERENCE: https://stackoverflow.com/questions/6406356/how-to-write-vector-values-to-a-file
   std::ofstream f("./playlist.txt");

   // Check if the file is open
   if (f.is_open())
   {
      // Loop through the vector elements
      for (const auto& s : loadedFiles)
      {
         // Write each element to the file with a newline character
         f << s.getFullPathName() << '\n';
      }

      // Close the file
      f.close();
   }
   else
   {
      // Print an error message if the file cannot be opened
      DBG("Unable to open output file\n");
   }
}


void PlaylistComponent::loadThePlaylist()
{
   // Open an input file stream
   std::ifstream f("playlist.txt");

// Check if the file is open
   if (f.is_open())
   {
      // Create a string variable to store each line
      std::string s;

      // Loop through the file lines
      while (getline(f, s))
      {
         // Push each line into the vector
         paths.push_back(s);
      }

      // Close the file
      f.close();
   }
   else
   {
      // Print an error message if the file cannot be opened
      DBG("Unable to open input file\n");
   }
}


void PlaylistComponent::updateAndSavePlaylistData()
{
   trackTitlesMain = trackTitles; // Makes a copy to use it when calling SEARCH
   loadedFilesMain = loadedFiles; // Makes a copy to use it when calling SEARCH

   saveThePlaylist();
}

juce::Component* PlaylistComponent::createButtonInsideCell(int selectedColumn, std::string buttonLabel, std::string idElement, juce::Component* existingComponentToUpdate, int rowNumber, int columnId)
{
   if (columnId == selectedColumn )
   {
      if (existingComponentToUpdate == nullptr)
      {
         juce::TextButton* btn = new juce::TextButton{ buttonLabel };
         juce::String id{ idElement + "," + std::to_string(rowNumber) };
         btn->setComponentID(id);

         btn->addListener(this);
         existingComponentToUpdate = btn;
      }
   }

   return existingComponentToUpdate;
}

void PlaylistComponent::createClearPlaylistAlertWindow() {
   // Create an AlertWindow 
   juce::AlertWindow* alert = new juce::AlertWindow("", "", juce::AlertWindow::QuestionIcon);

   alert->showOkCancelBox(juce::AlertWindow::NoIcon,
      "Remove the playlist",
      "Do you want to remove all tracks in the playlist?",
      "OK",
      "Cancel",
      this,
      juce::ModalCallbackFunction::create([=](int result)
         {
            // Check if the user clicked OK
            if (result == 1)
            {
               trackTitles.clear();
               loadedFiles.clear();

               updateAndSavePlaylistData();
               tableComponet.updateContent();
            }
   // Delete the alert window object
   delete alert;
         }));
}

void PlaylistComponent::addNewFiles()
{
   wavefromDisplay.newFileLoaded = false;
   newFileAdded = true; //FLAG to let know timer that we are initated the process of adding add new file

   if (!(loadedFilesMain.empty() || trackTitles.empty())) // UPDATES
   {
      // Updates the main data base of the loaded tracks - when search is used then sub vectors are created - it is necessary to store the original list before search initated to be able to restore it once search finished
      trackTitles = trackTitlesMain;
      loadedFiles = loadedFilesMain;

      // New tracks added - it is necessary to update the dispaly of the playlist table
      tableComponet.updateContent();
   }


   // FILES CHOOSER 
   auto fileChooserFlags = juce::FileBrowserComponent::canSelectMultipleItems;// - configure the dialogue
   player->fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)// LAMBDA FUNCTION- launch out of the main thread
      {
         juce::Array<juce::File> chosenFiles = chooser.getResults();
   for (auto& file : chosenFiles)
   {
      toBeUploadedQueue.push((file.getFullPathName().toStdString()));
   }
      }
   );
}