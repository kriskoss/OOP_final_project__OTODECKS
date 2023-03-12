/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 11 Mar 2023 11:28:31pm
    Author:  Candidate No. EX2765

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <iostream>


class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
   MyLookAndFeel();
   void drawRotarySlider(juce::Graphics& g, 
                              int x, 
                              int y, 
                              int width, 
                              int height, 
                              float sliderPos,
                              const float rotaryStartAngle, 
                              const float rotaryEndAngle, 
                              juce::Slider&) override;

   void drawButtonBackground(juce::Graphics& g,
      juce::Button& button,
      const juce::Colour& backgroundColour,
      bool,
      bool isButtonDown) override;

   void drawButtonText(juce::Graphics& g,
                        juce::TextButton& button,
                        bool isMouseOverButton,
                        bool isButtonDown) override;
};