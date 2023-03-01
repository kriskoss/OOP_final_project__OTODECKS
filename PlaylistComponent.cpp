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
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

   trackTitles.push_back("Track 1");
   trackTitles.push_back("Track 2");
   trackTitles.push_back("Track 3");
   trackTitles.push_back("Track 4");
   trackTitles.push_back("Track 5");
   trackTitles.push_back("Track 6");

   tableComponet.getHeader().addColumn("LEFT", 1, 80);
   tableComponet.getHeader().addColumn("Track title", 2, 400);
   tableComponet.getHeader().addColumn("RIGHT", 3, 80);
   //tableComponet.getHeader().addColumn("Artists", 1, 400);
   tableComponet.setModel(this);
   addAndMakeVisible(tableComponet);
}

PlaylistComponent::~PlaylistComponent()
{
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
    // This method is where you should set the bounds of any child
    // components that your component contains..
   tableComponet.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
   return trackTitles.size();
}
void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                                          int rowNumber,
                                          int width,
                                          int height,
                                          bool rowIsSelected)
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

void PlaylistComponent::paintCell(juce::Graphics& g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected) 
{
   g.drawText(trackTitles[rowNumber], 
               1, 0, 
               width - 4, height, 
               juce::Justification::centred, 
               true);
}


juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                            int columnId,
                                                            bool isRowSelected,
                                                            juce::Component* existingComponentToUpdate)
{
   if (columnId == 1)
   {
      if (existingComponentToUpdate == nullptr)
      {
         juce::TextButton* btn = new juce::TextButton{ "PLAY-L" };
         btn->addListener(this);
         existingComponentToUpdate = btn;
      }
   }

   if (columnId == 3)
   {
      if (existingComponentToUpdate == nullptr)
      {
         juce::TextButton* btn = new juce::TextButton{ "PLAY-R" };
         juce::String id{std::to_string(rowNumber)};
         btn->setComponentID(id);

         btn->addListener(this);
         existingComponentToUpdate = btn;
      }
   }
   return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(juce::Button* button)
{
   int id = std::stoi(button->getComponentID().toStdString());
   DBG("PlaylistComponent::buttonClicked - clicked"+ trackTitles[id]);
}
