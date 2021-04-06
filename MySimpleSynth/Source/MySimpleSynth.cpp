/*
  ==============================================================================

    MySimpleSynth.cpp
    Created: 29 Mar 2021 5:55:02pm
    Author:  Paul-Arthur Sauvageot

  ==============================================================================
*/

#include "MySimpleSynth.h"
#include "Parameters.h"

#include <iostream>

MySimpleSynth::MySimpleSynth() : MySynthProcessorChain(), _lowpass(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, PARAMETERS_LOWPASS_FREQ_DEFAULT)), _highpass(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100, PARAMETERS_HIGHPASS_FREQ_DEFAULT))
{
    //Played note
    _activeNote = -1;
    
    //Setting up oscillator
    auto& osc = get<0>();
    osc.initialise([] (float x) { return x / juce::MathConstants<float>::pi;  }, 100);
    
    //Setting up gain
    auto& gain = get<1>();
    gain.setGainDecibels(PARAMETERS_GAIN_DEFAULT);
    gain.setRampDurationSeconds(0.1f);
    
    //Setting up ADSR
    _adsrParameters = {PARAMETERS_ATTACK_DEFAULT, PARAMETERS_DECAY_DEFAULT, PARAMETERS_SUSTAIN_DEFAULT, PARAMETERS_RELEASE_DEFAULT};
}

MySimpleSynth::~MySimpleSynth()
{
}

void MySimpleSynth::process(const juce::dsp::ProcessContextReplacing<float> &context)
{
    if (_activeNote == -1)
    {
        return;
    }
    
    MySynthProcessorChain::process(context);
    _lowpass.process(context);
    _highpass.process(context);
}

void MySimpleSynth::prepare(const juce::dsp::ProcessSpec &spec)
{
    MySynthProcessorChain::prepare(spec);
    
    _adsr.setSampleRate(spec.sampleRate);
    _adsr.setParameters(_adsrParameters);
    
    *_lowpass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, PARAMETERS_LOWPASS_FREQ_DEFAULT);
    _lowpass.prepare(spec);
    _lowpass.reset();
    
    *_highpass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, PARAMETERS_HIGHPASS_FREQ_DEFAULT);
    _highpass.prepare(spec);
    _highpass.reset();
    
    _sampleRate = spec.sampleRate;
}

void MySimpleSynth::handleMidi(juce::MidiBuffer &midiBuffer)
{
    for (juce::MidiBufferIterator it = midiBuffer.begin(); it != midiBuffer.end(); ++it)
    {
        if ((*it).getMessage().isNoteOn())
        {
            noteOn((*it).getMessage().getNoteNumber());
        }
        else if ((*it).getMessage().isNoteOff())
        {
            noteOff((*it).getMessage().getNoteNumber());
        }
    }
}

void MySimpleSynth::applyADSR(juce::AudioBuffer<float> &buffer)
{
    _adsr.applyEnvelopeToBuffer(buffer, 0, buffer.getNumSamples());
}

void MySimpleSynth::noteOn(int noteNumber)
{
    _activeNote = noteNumber;
    
    auto& osc = get<0>();
    osc.setFrequency(440.0 * powf(2.0f, ((float)_activeNote - 69.0f)/12.0f));
    _adsr.noteOn();
}

void MySimpleSynth::noteOff(int noteNumber)
{
    if (noteNumber == _activeNote)
    {
        _adsr.noteOff();
    }
}

void MySimpleSynth::audioProcessorParameterChanged (juce::AudioProcessor *processor, int parameterIndex, float newValue)
{
    juce::AudioProcessorParameterWithID *param = dynamic_cast<juce::AudioProcessorParameterWithID *>(processor->getParameters()[parameterIndex]);
    
    if (!param)
    {
        return;
    }
    
    if (param->paramID == "attack")
    {
        _adsrParameters.attack = newValue;
        _adsr.setParameters(_adsrParameters);
    }
    else if (param->paramID == "decay")
    {
        _adsrParameters.decay = newValue;
        _adsr.setParameters(_adsrParameters);
    }
    else if (param->paramID == "sustain")
    {
        _adsrParameters.sustain = newValue;
        _adsr.setParameters(_adsrParameters);
    }
    else if (param->paramID == "release")
    {
        _adsrParameters.release = newValue;
        _adsr.setParameters(_adsrParameters);
    }
    else if (param->paramID == "gain")
    {
        auto& gain = get<1>();
        gain.setGainDecibels(newValue);
    }
    else if (param->paramID == "lowpass_freq")
    {
        *_lowpass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(_sampleRate, newValue);
    }
    else if (param->paramID == "highpass_freq")
    {
        *_highpass.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(_sampleRate, newValue);
    }
}

void MySimpleSynth::audioProcessorChanged (juce::AudioProcessor *processor, const ChangeDetails &details)
{
    (void) processor;
    (void) details;
}
