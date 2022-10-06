    int time;
    juce::MidiMessage m;
 
    // iterates throught the Midi Buffer and sets the phase increment
    for (juce::MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
    {
        if (m.isNoteOn())
        {
            int noteNumber;
            double f0;
            noteNumber = m.getNoteNumber();
            f0 = m.getMidiNoteInHertz(noteNumber);
            synth.new_freq = f0;
            synth.cmd = CMD_ADD_KEY;
        }
        else if (m.isNoteOff())
        {
        }
        else if (m.isAftertouch())
        {
        }
        else if (m.isPitchWheel())
        {
        }
    }
