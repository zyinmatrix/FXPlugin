#include <stdio.h>
#include <stdlib.h> //for exit()
#include <math.h>   //for sin()
#include "delay_fx.h"

/* constructor */
DelayFx::DelayFx(void){
    ;
}

/* destructor */
DelayFx::~DelayFx(void) 
{
    ;
}

void DelayFx::init_delay_effects(float fs) {
    this->fs = fs;
    for (int i=0; i<BUF_LEN; i++) {
        buffer[i] = 0;
    }
    disable_all();
}

void DelayFx::disable_all(void)
{
    enab_vibrato = false;
    enab_chorus = false;
    enab_echo = false;
    enab_lfo = false;
}

void DelayFx::enable_vibrato(float f0, float ampl)
/* f0 in Hz, ampl in ms */
{
    vib_f0 = f0;
    vib_phase = 0;
    vib_phase_inc = 2*PI*f0/fs;
    vib_osc_amp = (ampl/1000.0)*fs;
    enab_vibrato = true;
}
void DelayFx::disable_vibrato(void) 
{
    enab_vibrato = false;
}

void DelayFx::enable_chorus(float ampl) 
{
    for (int i=0; i<NCV; i++) {
        float rand_num = rand();
        chorus_delay[i] = (int)((ampl/1000.0)*fs) * 
            2.0*(rand_num/RAND_MAX - 0.5);
        printf("chorus: %d %d\n", i, chorus_delay[i]);
    }
    enab_chorus = true;
}
void DelayFx::disable_chorus(void) 
{
    enab_chorus = false;
}

void DelayFx::enable_echo(float ampl)
{
    echo_delay = ampl/1000.0*fs;
    enab_echo = true;
}
void DelayFx::disable_echo(void) 
{
    enab_echo = false;
}

void DelayFx::enable_lfo(float f0)
{
    lfo_f0 = f0;
    lfo_phase = 0;
    lfo_phase_inc = 2*PI*f0/fs;
    enab_lfo = true;
}
void DelayFx::disable_lfo(void) 
{
    enab_lfo = false;
}

float * DelayFx::delay_effects(float *ibuf, int N)
{
    int j, k, n, num_buf;
    float samp_v;

    /* number of callback buffers in BUF_LEN */
    num_buf = BUF_LEN/N;
    /* shift buffer */
    j = BUF_LEN-num_buf*N;
    k = BUF_LEN-(num_buf-1)*N;
    n = (num_buf-1)*N;
    for (int i=0; i<n; i++) {
        buffer[j] = buffer[k];
        j++;
        k++;
    }
    /* copy new samples to buffer 
     * continue at index j
     */
    for (int i=0; i<N; i++) {
        buffer[j] = ibuf[i];
        j++;
    }

    /* start from this point in buffer */
    k = BUF_MID;
    for (int i=0; i<N; i++) {
        /* original, no effect */
        samp_v = buffer[k+i];
        if (enab_vibrato) {
            /* sine value for vibrato oscillator */
            float sin_v = vib_osc_amp * sin(vib_phase);
            vib_phase += vib_phase_inc;        
            /* interpolate to get sample at delay */
            samp_v = get_sample(k+i+sin_v);
        }
        if (enab_chorus) {
            for (int j=0; j<NCV; j++) {
                samp_v += buffer[k+i+chorus_delay[j]];
            }
            samp_v /= NCV; //to prevent overflow
        }
        if (enab_echo) {
            samp_v += get_sample(k+i-echo_delay);
            samp_v /= 2; //to prevent overflow
        }
        if (enab_lfo) {
            float sin_v = sin(lfo_phase);
            lfo_phase += lfo_phase_inc;        
            samp_v *= sin_v;
        }
        output[i] = samp_v;
    }
    return &output[0];
}

float DelayFx::get_sample(float samp_delay)
{
    int whole;
    float frac, a, b, v;

    whole = floor(samp_delay);
    frac = samp_delay-whole;
    a = buffer[whole]; //sample before target time
    b = buffer[whole+1]; //sample after target time
    /* linear interpolation */
    v = a*(1.0-frac) + b*frac;

    return v;
}
