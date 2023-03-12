/*
  ==============================================================================

    MyLookAndFeel.cpp
    Created: 11 Mar 2023 11:28:31pm
    Author:  krzys

  ==============================================================================
*/

#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel()
{
   setColour(juce::Slider::thumbColourId, juce::Colours::red);
   
}

void MyLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                             int x,
                                             int y,
                                             int width,
                                             int height,
                                             float sliderPos,
                                             const float rotaryStartAngle,
                                             const float rotaryEndAngle,
                                             juce::Slider&)
{
   float radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
   float centreX = (float)x + (float)width * 0.5f;
   float centreY = (float)y + (float)height * 0.5f;
   float rx = centreX - radius;
   float ry = centreY - radius;
   float rw = radius * 2.0f;
   float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

   // fill
   g.setColour(juce::Colours::steelblue);
   g.setOpacity(0.5f);
   g.fillEllipse(rx, ry, rw, rw);
   g.setOpacity(1.0f);

   // outline
   g.setColour(juce::Colours::olivedrab);
   g.drawEllipse(rx, ry, rw, rw, 3.0f);

   juce::Path p;
   float pointerLength = radius * 0.90f;
   float pointerThickness = 3.0f;
   p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
   p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

   // pointer
   g.setColour(juce::Colours::darkblue);
   g.fillPath(p);
}

   void MyLookAndFeel::drawButtonBackground(juce::Graphics& g,
                              juce::Button& button, 
                              const juce::Colour& backgroundColour,
                              bool, 
                              bool isButtonDown) 
   {
      auto buttonArea = button.getLocalBounds();
      auto edge = 4;

      buttonArea.removeFromLeft(edge);
      buttonArea.removeFromTop(edge);

      // shadow
      g.setColour(juce::Colours::darkgrey.withAlpha(0.5f));
      g.fillRect(buttonArea);

      auto offset = isButtonDown ? -edge / 2 : -edge;
      buttonArea.translate(offset, offset);

      g.setColour(juce::Colours::steelblue);
      g.fillRect(buttonArea);
   }

   void MyLookAndFeel::drawButtonText(juce::Graphics& g,
      juce::TextButton& button,
      bool isMouseOverButton,
      bool isButtonDown)
   {

      auto font = getTextButtonFont(button, button.getHeight());
      
      g.setFont(font);
      g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
         : juce::TextButton::textColourOffId)
         .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

      auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
      auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;

      auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
      auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
      auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
      auto textWidth = button.getWidth() - leftIndent - rightIndent;

      auto edge = 4;
      auto offset = isButtonDown ? edge / 2 : 0;

      if (textWidth > 0)
         g.drawFittedText(button.getButtonText(),
            leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge,
            juce::Justification::centred, 2);
   }



   
