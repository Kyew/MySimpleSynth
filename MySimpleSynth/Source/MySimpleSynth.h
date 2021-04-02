/*
  ==============================================================================

    MySimpleSynth.h
    Created: 29 Mar 2021 5:55:02pm
    Author:  Paul-Arthur Sauvageot

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

typedef juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Gain<float>> MySynthProcessorChain;

class MySimpleSynth : public MySynthProcessorChain, public juce::AudioProcessorListener
{
public:
    MySimpleSynth();
    ~MySimpleSynth();
    
    void prepare(const juce::dsp::ProcessSpec &spec);
    void process(const juce::dsp::ProcessContextReplacing<float> &context);
    void handleMidi(juce::MidiBuffer &midiBuffer);
    void applyADSR(juce::AudioBuffer<float> &buffer);
    
    virtual void audioProcessorParameterChanged (juce::AudioProcessor *processor, int parameterIndex, float newValue);
    virtual void audioProcessorChanged (juce::AudioProcessor *processor, const ChangeDetails &details);
    
private:
    MySimpleSynth(const MySimpleSynth &);
    
    MySimpleSynth &operator=(const MySimpleSynth &);
    
    void noteOn(int noteNumber);
    void noteOff(int noteNumber);
    
    int _activeNote;
    juce::ADSR _adsr;
    juce::ADSR::Parameters _adsrParameters;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> _lowpass, _highpass;
    float _sampleRate;
};
