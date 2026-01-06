/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class Aap_eqAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    Aap_eqAudioProcessorEditor(Aap_eqAudioProcessor&);
    ~Aap_eqAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Reference to processor
    Aap_eqAudioProcessor& audioProcessor;

    //==============================================================================
    // UI Components
    juce::Slider bassSlider;
    juce::Slider midSlider;
    juce::Slider trebleSlider;

    //==============================================================================
    // Parameter Attachments
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> bassAttachment;
    std::unique_ptr<SliderAttachment> midAttachment;
    std::unique_ptr<SliderAttachment> trebleAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Aap_eqAudioProcessorEditor)
};
