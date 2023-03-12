/*
  ==============================================================================

    SpectrogramComponent.h
    Created: 12 Mar 2023 2:12:31pm
    Author:  krzys

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
//==============================================================================
/*
*/

class SpectrogramComponent : public juce::AudioAppComponent,
                           private juce::Timer
{
public:
   SpectrogramComponent(juce::AudioFormatManager& formatManager, juce::MixerAudioSource * _mixerSource);

   ~SpectrogramComponent() override;
   //==============================================================================
   //void prepareToPlay(int, double) override {}
   //void releaseResources() override {}
   //void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

   juce::MixerAudioSource* mixerSource;

   void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
   void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
   void releaseResources() override;
   //==============================================================================
   void paint(juce::Graphics& g) override;

   void timerCallback() override;

   void pushNextSampleIntoFifo(float sample) noexcept;

   void drawNextLineOfSpectrogram();

   void clearSpectogram();
   static constexpr auto fftOrder = 10;                // [1]
   static constexpr auto fftSize = 1 << fftOrder;     // [2]

   void stopSpectogram();
   void startSpectogram();

private:
   
   juce::dsp::FFT forwardFFT;                          // [3]
   juce::Image spectrogramImage;
   

   std::array<float, fftSize> fifo;                    // [4]
   std::array<float, fftSize * 2> fftData;             // [5]
   int fifoIndex = 0;                                  // [6]
   bool nextFFTBlockReady = false;                     // [7]

   bool spectogramMoving = true;

   //AudioSource 
   juce::AudioFormatManager& formatManager;
   std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
   juce::AudioTransportSource transportSource;
   
   juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrogramComponent)
};

