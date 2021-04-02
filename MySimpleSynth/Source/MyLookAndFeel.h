/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 2 Apr 2021 3:25:46pm
    Author:  Paul-Arthur Sauvageot

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MyLookAndFeel();
    ~MyLookAndFeel();
    
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float pos, const float startAngle, const float endAngle, juce::Slider &slider);
    void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float pos, float minPos, float maxPos, const juce::Slider::SliderStyle style, juce::Slider &slider);
    
private:
    MyLookAndFeel(const MyLookAndFeel &);
    
    MyLookAndFeel &operator=(const MyLookAndFeel &);
};
