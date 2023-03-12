//MainComponent.cpp
#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent()
{
    setSize (1000, 600);

    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels (0, 2);  
    }

    addAndMakeVisible(simpleFFT);
    addAndMakeVisible(deckGUI1);  
    addAndMakeVisible(deckGUI2);  

    addAndMakeVisible(playlistComponent);
    

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
   mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
   mixerSource.addInputSource(&player1, false); //mixerSource.addInputSource automatically calls prepareToPlay
   mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
   mixerSource.getNextAudioBlock(bufferToFill);


   // Sends data to SpectogramComponent
   if (bufferToFill.buffer->getNumChannels() > 0)
   {
      auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

      for (auto i = 0; i < bufferToFill.numSamples; ++i)
      {
         simpleFFT.pushNextSampleIntoFifo(channelData[i]);

      }

   }
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    juce::Rectangle<int> background(getWidth(), getHeight()/2);

    // Fill it with a color of your choice
    g.setColour(juce::Colours::darkslateblue);
    g.fillRect(background);
}

void MainComponent::resized()
{
   double rowH = (getHeight() / 2) / 10;
   
   deckGUI1.setBounds(0, 0,getWidth()/ 2, getHeight()/2);
   deckGUI2.setBounds(getWidth() / 2, 0,getWidth()/ 2, getHeight()/2);

   playlistComponent.setBounds(10, getHeight()/2+10, getWidth()-20, getHeight() / 2-20);
   simpleFFT.setBounds(0, 5*rowH, getWidth(), 5 * rowH);
   
}

