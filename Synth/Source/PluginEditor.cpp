/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "synth.h"
#include "delay_fx.h"
enum RadioButtonIDs {
    FxButtons = 10001
};

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor (SynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 450);
    
    // these define the parameters of our slider object
    mVolumeSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    mVolumeSlider.setRange(0.0, 1.0, 0.1);
    mVolumeSlider.addListener(this);
    mVolumeSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 90, 30);
    mVolumeSlider.setPopupDisplayEnabled (true, false, this);
    mVolumeSlider.setTextValueSuffix (" Volume");
    mVolumeSlider.setValue(1.0);
    addAndMakeVisible (&mVolumeSlider);
    
    mVolumeLabel.setFont(15.0f);
    mVolumeLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    mVolumeLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    mVolumeLabel.setJustificationType(juce::Justification::centredTop);
    mVolumeLabel.attachToComponent(&mVolumeSlider, true);
    
    mSustainSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    mSustainSlider.setRange(0.0, 1.0, 0.1);
    mSustainSlider.addListener(this);
    mSustainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 90, 30);
    mSustainSlider.setPopupDisplayEnabled (true, false, this);
    mSustainSlider.setTextValueSuffix (" Sustain");
    mSustainSlider.setValue(1.0);
    addAndMakeVisible (&mSustainSlider);
    
    mSustainLabel.setFont(15.0f);
    mSustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, true);
    
    addAndMakeVisible (&mEffectsOffFx);
    addAndMakeVisible (&mVibratoFx);
    addAndMakeVisible (&mChorusFx);
    addAndMakeVisible (&mEchoFx);
    addAndMakeVisible (&mLfoFx);
    
    mEffectsOffFx.onClick = [this] { audioProcessor.delay_fx.disable_all(); };
    mVibratoFx.onClick = [this] { audioProcessor.delay_fx.disable_all();
        audioProcessor.delay_fx.enable_vibrato(5, 0.4); };
    mChorusFx.onClick = [this] { audioProcessor.delay_fx.disable_all();
        audioProcessor.delay_fx.enable_chorus(30); };
    mEchoFx.onClick = [this] { audioProcessor.delay_fx.disable_all();
        audioProcessor.delay_fx.enable_echo(50); };
    mLfoFx.onClick = [this] { audioProcessor.delay_fx.disable_all();
        audioProcessor.delay_fx.enable_lfo(10); };
    
    mEffectsOffFx.setRadioGroupId(FxButtons);
    mVibratoFx.setRadioGroupId(FxButtons);
    mChorusFx.setRadioGroupId(FxButtons);
    mEchoFx.setRadioGroupId(FxButtons);
    mLfoFx.setRadioGroupId(FxButtons);
}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
    
}

//==============================================================================
void SynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
//    g.drawFittedText("Volume", 0, 0, 300, 300, juce::Justification::centred, 1);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    int sliderLength = 300;
    
    mVolumeSlider.setBounds(150 - 5, getHeight()/2 - sliderLength/2, 10, sliderLength);
    mSustainSlider.setBounds(300 - 5, getHeight()/2 - sliderLength/2, 10, sliderLength);
    
    int xToggle = 450;
    mEffectsOffFx.setBounds(xToggle, getHeight()/2 - 120, 150, 20);
    mVibratoFx.setBounds(xToggle, getHeight()/2 - 60, 150, 20);
    mChorusFx.setBounds(xToggle, getHeight()/2 + 0, 150, 20);
    mEchoFx.setBounds(xToggle, getHeight()/2 + 60, 150, 20);
    mLfoFx.setBounds(xToggle, getHeight()/2 + 120, 150, 20);
    
    
}

void SynthAudioProcessorEditor::sliderValueChanged (juce::Slider* slider){
    if(slider == &mVolumeSlider){
        
        audioProcessor.synth.set_ampl(mVolumeSlider.getValue());
        
    }else if (slider == &mSustainSlider){
        
        audioProcessor.synth.set_decay(mSustainSlider.getValue());
    }
}
