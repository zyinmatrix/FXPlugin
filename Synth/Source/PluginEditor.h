/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SynthAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::Slider::Listener
{
public:
    SynthAudioProcessorEditor (SynthAudioProcessor&);
    ~SynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void sliderValueChanged (juce::Slider* slider) override;
    SynthAudioProcessor& audioProcessor;
    juce::Slider mVolumeSlider, mSustainSlider;
    juce::Label mVolumeLabel, mSustainLabel;
    juce::ToggleButton mEffectsOffFx {"Disable All Fx"};
    juce::ToggleButton mVibratoFx {"Enable Vibrato"};
    juce::ToggleButton mChorusFx {"Enable Chorus"};
    juce::ToggleButton mEchoFx {"Enable Echo"};
    juce::ToggleButton mLfoFx {"Enable LFO"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioProcessorEditor)
};
