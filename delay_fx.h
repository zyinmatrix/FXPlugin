#ifndef _DELAY_FX_
#define _DELAY_FX_

#include <stdbool.h>
#if __APPLE__
#include <stdatomic.h>  /* permits write/read of "cmd" to be atomic */
#define MY_BOOL atomic_bool
#else
#define MY_BOOL bool
#endif

/* defines */
#define FRAMES_PER_BUFFER   1024
#define PI       3.14159265358979323846
#define C        343.0 //speed of sound, meters per second
#define BUF_LEN  8*1024
#define BUF_MID  (BUF_LEN/2)
#define NCV      5 //number of chorus voices

/* effects */
enum 
{
    MD_VIB = 1
};

class DelayFx {
public:
    DelayFx(void); //constructor
    ~DelayFx(void); //destructor
    /* public member functions */
    void init_delay_effects(float fs);
    void disable_all(void);
    void enable_vibrato(float f0, float ampl);
    void disable_vibrato(void);
    void enable_chorus(float ampl);
    void disable_chorus(void);
    void enable_echo(float ampl);
    void disable_echo(void);
    void enable_lfo(float f0);
    void disable_lfo(void);
    float *delay_effects(float *ifbuf, int N);
    float get_sample(float samp_delay);

public:
    /* public class data */
    float output[FRAMES_PER_BUFFER];

private:
    /* private class data */
    float fs; //sampling freq, Hz
    float buffer[BUF_LEN]; //delay buffer

    /* vibrato */
    MY_BOOL enab_vibrato;
    float vib_f0; //vibrato osc freq, Hz
    float vib_phase;
    float vib_phase_inc;
    float vib_osc_amp;  //vibrato oscillation amplitude, samples

    /* chorus */
    MY_BOOL enab_chorus;
    int chorus_delay[NCV]; //samples

    /* echo */
    MY_BOOL enab_echo;
    int echo_delay; //samples

    /* LFO */
    MY_BOOL enab_lfo;
    float lfo_f0; //LFO oscillation freq, Hz
    float lfo_phase;
    float lfo_phase_inc;
};
#endif /* _DELAY_FX_ */