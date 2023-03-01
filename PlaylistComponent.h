/*
  ==============================================================================

    PlaylistComponent.h
    Created: 28 Feb 2023 2:30:35pm
    Author:  krzys

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
/*
*/
//PlaylistComponent.h
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(juce::Graphics&, 
                           int rowNumber, 
                           int width, 
                           int height, 
                           bool rowIsSelected) override;

    void paintCell(juce::Graphics&, 
                     int rowNumber, 
                     int columnId, 
                     int width, 
                     int height, 
                     bool rowIsSelected) override;
    
    juce::Component* refreshComponentForCell(int rowNumber, 
                                        int columnId, 
                                        bool isRowSelected, 
                                        juce::Component* existingComponentToUpdate);

    void buttonClicked(juce::Button* button) override;
private:

   juce::TableListBox tableComponet;
   std::vector<std::string> trackTitles;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
