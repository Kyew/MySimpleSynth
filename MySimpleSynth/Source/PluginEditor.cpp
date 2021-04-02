/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define EDITOR_BACKGROUND_COLOR juce::Colour(66, 66, 66)

//==============================================================================
MySimpleSynthAudioProcessorEditor::MySimpleSynthAudioProcessorEditor (MySimpleSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel(&_lookAndFeel);
    
    juce::Font titleFont(22, juce::Font::FontStyleFlags::bold);
    juce::Font sectionFont(18, juce::Font::FontStyleFlags::bold);
    
    _headerLabel.setText("MySimpleSynth by Paul-Arthur Sauvageot", juce::NotificationType::dontSendNotification);
    _headerLabel.setFont(titleFont);
    addAndMakeVisible(_headerLabel);
    
    _envelopHeaderLabel.setText("ADSR", juce::NotificationType::dontSendNotification);
    _envelopHeaderLabel.setJustificationType(juce::Justification::centred);
    _envelopHeaderLabel.setFont(sectionFont);
    addAndMakeVisible(_envelopHeaderLabel);
    _envelopASlider.setSliderStyle(juce::Slider::LinearVertical);
    bindFloatParameterToSlider(&_envelopASlider, "attack", 0.01f);
    addAndMakeVisible(_envelopASlider);
    _envelopDSlider.setSliderStyle(juce::Slider::LinearVertical);
    bindFloatParameterToSlider(&_envelopDSlider, "decay", 0.01f);
    addAndMakeVisible(_envelopDSlider);
    _envelopSSlider.setSliderStyle(juce::Slider::LinearVertical);
    bindFloatParameterToSlider(&_envelopSSlider, "sustain", 0.01f);
    addAndMakeVisible(_envelopSSlider);
    _envelopRSlider.setSliderStyle(juce::Slider::LinearVertical);
    bindFloatParameterToSlider(&_envelopRSlider, "release", 0.01f);
    addAndMakeVisible(_envelopRSlider);
    
    _filtersHeaderLabel.setText("Filters", juce::NotificationType::dontSendNotification);
    _filtersHeaderLabel.setJustificationType(juce::Justification::centred);
    _filtersHeaderLabel.setFont(sectionFont);
    addAndMakeVisible(_filtersHeaderLabel);
    _hpLabel.setText("HP", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(_hpLabel);
    _hpSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _hpSlider.setRotaryParameters(-(120.0f * juce::float_Pi / 180.0f), 120.0f * juce::float_Pi / 180.0f, true);
    _hpSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 70, 20);
    _hpSlider.setTextValueSuffix(" Hz");
    bindFloatParameterToSlider(&_hpSlider, "highpass_freq", 1.0f);
    addAndMakeVisible(_hpSlider);
    _lpLabel.setText("LP", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(_lpLabel);
    _lpSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _lpSlider.setRotaryParameters(-(120.0f * juce::float_Pi / 180.0f), 120.0f * juce::float_Pi / 180.0f, true);
    _lpSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 70, 20);
    _lpSlider.setTextValueSuffix(" Hz");
    bindFloatParameterToSlider(&_lpSlider, "lowpass_freq", 1.0f);
    addAndMakeVisible(_lpSlider);
    
    _gainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    _gainLabel.setJustificationType(juce::Justification::centred);
    _gainLabel.setFont(sectionFont);
    addAndMakeVisible(_gainLabel);
    _gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _gainSlider.setRotaryParameters(-(120.0f * juce::float_Pi / 180.0f), 120.0f * juce::float_Pi / 180.0f, true);
    _gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 20);
    _gainSlider.setTextValueSuffix(" db");
    bindFloatParameterToSlider(&_gainSlider, "gain", 1.0f);
    addAndMakeVisible(_gainSlider);
    setSize (400, 300);
}

MySimpleSynthAudioProcessorEditor::~MySimpleSynthAudioProcessorEditor()
{
}

//==============================================================================
void MySimpleSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(EDITOR_BACKGROUND_COLOR);
}

void MySimpleSynthAudioProcessorEditor::resized()
{
    juce::Rectangle<int> editorArea = getLocalBounds();
    
    int headerHeight = 50;
    int headerMargins = 10;
    juce::Rectangle<int> headerArea = editorArea.removeFromTop(headerHeight).reduced(headerMargins);
    _headerLabel.setBounds(headerArea);
    
    int adsrSliderWidth = 15;
    int adsrSliderSpace = 8;
    int adsrAreaMargin = 10;
    int adsrAndFiltersAreaWidth = (int)((float)editorArea.getWidth() * 0.36f);
    int adsrHeaderHeight = 30;
    juce::Rectangle<int> adsrArea = editorArea.removeFromLeft(adsrAndFiltersAreaWidth).reduced(adsrAreaMargin);
    _envelopHeaderLabel.setBounds(adsrArea.removeFromTop(adsrHeaderHeight));
    adsrArea.removeFromLeft((adsrArea.getWidth() - (adsrSliderWidth * 4 + adsrSliderSpace * 3)) / 2);
    _envelopASlider.setBounds(adsrArea.removeFromLeft(adsrSliderWidth));
    adsrArea.removeFromLeft(adsrSliderSpace);
    _envelopDSlider.setBounds(adsrArea.removeFromLeft(adsrSliderWidth));
    adsrArea.removeFromLeft(adsrSliderSpace);
    _envelopSSlider.setBounds(adsrArea.removeFromLeft(adsrSliderWidth));
    adsrArea.removeFromLeft(adsrSliderSpace);
    _envelopRSlider.setBounds(adsrArea.removeFromLeft(adsrSliderWidth));
    
    int filtersHeaderHeight = 30;
    int filtersAreaMargin = 10;
    int filterTitleHeight = 30;
    juce::Rectangle<int> filterArea = editorArea.removeFromLeft(adsrAndFiltersAreaWidth).reduced(filtersAreaMargin);
    _filtersHeaderLabel.setBounds(filterArea.removeFromTop(filtersHeaderHeight));
    juce::Rectangle<int> HPArea = filterArea.removeFromTop(filterArea.getHeight() / 2);
    _hpLabel.setBounds(HPArea.removeFromTop(filterTitleHeight));
    _hpSlider.setBounds(HPArea);
    juce::Rectangle<int> LPArea = filterArea;
    _lpLabel.setBounds(LPArea.removeFromTop(filterTitleHeight));
    _lpSlider.setBounds(LPArea);
    
    int gainAreaMargin = 10;
    int gainHeaderHeight = 30;
    juce::Rectangle<int> gainArea = editorArea.removeFromTop(((float)editorArea.getHeight()) * 0.8f).reduced(gainAreaMargin);
    _gainLabel.setBounds(gainArea.removeFromTop(gainHeaderHeight));
    _gainSlider.setBounds(gainArea);
}

void MySimpleSynthAudioProcessorEditor::bindParameterToComponent(juce::Component *component, const juce::String &parameterName)
{
    auto& params = getAudioProcessor()->getParameters();

    for (auto p : params)
    {
        if (auto* param = dynamic_cast<juce::AudioProcessorParameterWithID*> (p))
        {
            if (param->paramID == parameterName)
            {
                _componentsParameters[component] = param;
                return;
            }
        }
    }
}

void MySimpleSynthAudioProcessorEditor::bindFloatParameterToSlider(juce::Slider *slider, const juce::String &parameterName, float step)
{
    bindParameterToComponent(slider, parameterName);
    
    juce::AudioParameterFloat *param = (juce::AudioParameterFloat *)_componentsParameters[slider];
    slider->setRange(param->getNormalisableRange().start, param->getNormalisableRange().end, step);
    slider->setValue(param->get());
    slider->onValueChange = [this, slider] { _componentsParameters[slider]->setValueNotifyingHost(slider->getValue()); };
}
