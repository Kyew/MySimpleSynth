/*
  ==============================================================================

    MyLookAndFeel.cpp
    Created: 2 Apr 2021 3:25:46pm
    Author:  Paul-Arthur Sauvageot

  ==============================================================================
*/

#include "MyLookAndFeel.h"

#define MYLOOKANDFEEL_COLOR_DARK juce::Colour(112, 112, 112)
#define MYLOOKANDFEEL_COLOR_LIGHT juce::Colour(207, 207, 207)
#define MYLOOKANDFEEL_COLOR_SHADOW juce::Colour(27, 27, 27)

MyLookAndFeel::MyLookAndFeel()
{
    setColour(juce::Label::textColourId, juce::Colours::white);
}

MyLookAndFeel::~MyLookAndFeel()
{
    
}

void MyLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float pos, const float startAngle, const float endAngle, juce::Slider &slider)
{
    float radius = (float) juce::jmin (width / 2, height / 2) - 8.0f;
    float centreX = (float) x + (float) width  * 0.5f;
    float centreY = (float) y + (float) height * 0.5f;
    float rx = centreX - radius;
    float ry = centreY - radius;
    float rw = radius * 2.0f;
    float angle = startAngle + pos * (endAngle - startAngle);
       
    //Draw knob
    g.setColour (MYLOOKANDFEEL_COLOR_DARK);
    g.fillEllipse (rx, ry, rw, rw);
    
    //Draw outline
    g.setColour (MYLOOKANDFEEL_COLOR_SHADOW);
    g.drawEllipse (rx, ry, rw, rw, 3.0f);
       
    //Draw pointer
    juce::Path p;
    float pointerLength = radius * 0.75f;
    float pointerThickness = 3.0f;
    p.addRectangle (-pointerThickness * 0.5f, -radius - 3.0f, pointerThickness, pointerLength);
    p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour (MYLOOKANDFEEL_COLOR_LIGHT);
    g.fillPath (p);
}

void MyLookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float pos, float minPos, float maxPos, const juce::Slider::SliderStyle style, juce::Slider &slider)
{
    //Only for vertical sliders
    if (!slider.isVertical())
    {
        juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, pos, minPos, maxPos, style, slider);
        return;
    }
    
    float sliderThickness = 6.0f;
    float barX = (float) x + (width - sliderThickness) / 2.0f;

    //Draw empty bar
    g.setColour(MYLOOKANDFEEL_COLOR_LIGHT);
    g.fillRect(barX, (float) y, sliderThickness, juce::jmin(pos, (float) height));
    
    //Draw filled bar
    g.setColour(MYLOOKANDFEEL_COLOR_DARK);
    g.fillRect(barX, pos, sliderThickness, y + height - pos);
    
    //Draw outline
    g.setColour(MYLOOKANDFEEL_COLOR_SHADOW);
    g.drawRect(barX - 1.0f, (float) y, sliderThickness + 2.0f, (float) height, 1.0f);
        
    //Draw thumb
    float thumbDiameter = 12.0f;
    float thumbX = (float) x + width / 2.0f - thumbDiameter / 2.0f;
    float thumbY = pos - thumbDiameter / 2.0f;
    g.setColour(MYLOOKANDFEEL_COLOR_SHADOW);
    g.fillEllipse(thumbX, thumbY, thumbDiameter, thumbDiameter);
}
