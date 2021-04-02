/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <map>

#include "PluginProcessor.h"
#include "MyLookAndFeel.h"

//==============================================================================
/**
*/
class MySimpleSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MySimpleSynthAudioProcessorEditor (MySimpleSynthAudioProcessor&);
    ~MySimpleSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void bindParameterToComponent(juce::Component *component, const juce::String &parameterName);
    void bindFloatParameterToSlider(juce::Slider *slider, const juce::String &parameterName, float step);
    
    MySimpleSynthAudioProcessor& audioProcessor;
    
    juce::Label _headerLabel;
    
    juce::Label _envelopHeaderLabel;
    juce::Slider _envelopASlider;
    juce::Slider _envelopDSlider;
    juce::Slider _envelopSSlider;
    juce::Slider _envelopRSlider;
    
    juce::Label _filtersHeaderLabel;
    juce::Label _hpLabel;
    juce::Slider _hpSlider;
    juce::Label _lpLabel;
    juce::Slider _lpSlider;
    
    juce::Label _gainLabel;
    juce::Slider _gainSlider;

    MyLookAndFeel _lookAndFeel;
    
    std::map<juce::Component *, juce::AudioProcessorParameter *> _componentsParameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MySimpleSynthAudioProcessorEditor)
};
