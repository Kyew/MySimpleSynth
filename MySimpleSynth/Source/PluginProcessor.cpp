/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
MySimpleSynthAudioProcessor::MySimpleSynthAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       )
{
    //Adding parameters
    addParameter(new juce::AudioParameterFloat("attack", "Attack", PARAMETERS_ATTACK_MIN, PARAMETERS_ATTACK_MAX, PARAMETERS_ATTACK_DEFAULT));
    addParameter(new juce::AudioParameterFloat("decay", "Decay", PARAMETERS_DECAY_MIN, PARAMETERS_DECAY_MAX, PARAMETERS_DECAY_DEFAULT));
    addParameter(new juce::AudioParameterFloat("sustain", "Sustain", PARAMETERS_SUSTAIN_MIN, PARAMETERS_SUSTAIN_MAX, PARAMETERS_SUSTAIN_DEFAULT));
    addParameter(new juce::AudioParameterFloat("release", "Release", PARAMETERS_RELEASE_MIN, PARAMETERS_RELEASE_MAX, PARAMETERS_RELEASE_DEFAULT));
    addParameter(new juce::AudioParameterFloat("gain", "Gain", PARAMETERS_GAIN_MIN, PARAMETERS_GAIN_MAX, PARAMETERS_GAIN_DEFAULT));
    addParameter(new juce::AudioParameterFloat("lowpass_freq", "Lowpass frequency", PARAMETERS_LOWPASS_FREQ_MIN, PARAMETERS_LOWPASS_FREQ_MAX, PARAMETERS_LOWPASS_FREQ_DEFAULT));
    addParameter(new juce::AudioParameterFloat("highpass_freq", "Highpass frequency", PARAMETERS_HIGHPASS_FREQ_MIN, PARAMETERS_HIGHPASS_FREQ_MAX, PARAMETERS_HIGHPASS_FREQ_DEFAULT));
    
    //Adding synth as listener
    addListener(&_synth);
}

MySimpleSynthAudioProcessor::~MySimpleSynthAudioProcessor()
{
}

//==============================================================================
const juce::String MySimpleSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MySimpleSynthAudioProcessor::acceptsMidi() const
{
    return true;
}

bool MySimpleSynthAudioProcessor::producesMidi() const
{
    return false;
}

bool MySimpleSynthAudioProcessor::isMidiEffect() const
{
    return false;
}

double MySimpleSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MySimpleSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MySimpleSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MySimpleSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MySimpleSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void MySimpleSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MySimpleSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    //Setting up synth
    _synth.prepare({ .sampleRate = sampleRate, .maximumBlockSize = (juce::uint32)(samplesPerBlock), .numChannels = (juce::uint32)(getTotalNumOutputChannels())});
}

void MySimpleSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MySimpleSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MySimpleSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    /*for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = _osc->processSample(0.0f);
        }
    }*/
    
    _synth.handleMidi(midiMessages);
 
    juce::dsp::AudioBlock<float> block(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
    juce::dsp::ProcessContextReplacing<float> context(block);
 
    _synth.process(context);
    _synth.applyADSR(buffer);
}

//==============================================================================
bool MySimpleSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MySimpleSynthAudioProcessor::createEditor()
{
    return new MySimpleSynthAudioProcessorEditor (*this);
}

//==============================================================================
void MySimpleSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MySimpleSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MySimpleSynthAudioProcessor();
}
