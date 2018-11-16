/*
 * Simple sound playback using ALSA API and libasound.
 *
 * Compile:
 * $ cc -o play sound_playback.c -lasound
 * 
 * Usage:
 * $ ./play <sample_rate> <channels> <seconds> < <file>
 * 
 * Examples:
 * $ ./play 44100 2 5 < /dev/urandom
 * $ ./play 22050 1 8 < /path/to/file.wav
 *
 * Copyright (C) 2009 Alessandro Ghedini <alessandro@ghedini.me>
 * --------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Alessandro Ghedini wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we
 * meet some day, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * --------------------------------------------------------------
 */

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>


#define PCM_DEVICE "default"

int main(int argc, char **argv) {
    unsigned int pcm, tmp, dir;
    int rate, channels, seconds;
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    char *buff;
    int buff_size, loops;

    int stream = atoi(argv[1]);

    rate 	 = 44100; //atoi(argv[1]);
    channels = 1; //atoi(argv[2]);
    seconds  = 1; //atoi(argv[3]);

    /* Open the PCM device in playback mode */
    if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
		    SND_PCM_STREAM_PLAYBACK, 0) < 0) 
	printf("ERROR: Can't open \"%s\" PCM device. %s\n",
		    PCM_DEVICE, snd_strerror(pcm));

    /* Allocate parameters object and fill it with default values*/
    snd_pcm_hw_params_alloca(&params);

    snd_pcm_hw_params_any(pcm_handle, params);

    /* Set parameters */
    if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
		    SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
	printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

    if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE) < 0) 
	printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

    if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0) 
	printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

    if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0) < 0) 
	printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

    /* Write parameters */
    if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0)
	printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

    /* Resume information */
    printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));

    printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

    snd_pcm_hw_params_get_channels(params, &tmp);
    printf("channels: %i ", tmp);

    snd_pcm_hw_params_get_rate(params, &tmp, 0);
    printf("rate: %d bps\n", tmp);

    printf("seconds: %d\n", seconds);	

    /* Allocate buffer to hold single period */
    snd_pcm_hw_params_get_period_size(params, &frames, 0);

    buff_size = frames * channels * 2 /* 2 -> sample size */;

    snd_pcm_hw_params_get_period_time(params, &tmp, NULL);

    FILE *fGB = fopen("/usr/share/w48play/Gassenbesetzt.raw", "rb");
    fseek(fGB, 0, SEEK_END);
    long fsizeGB = ftell(fGB);
    fseek(fGB, 0, SEEK_SET);  //same as rewind(f);
char *    buffGB = (char *) malloc(fsizeGB + 1);
    fread(buffGB, fsizeGB, 1, fGB);
    fclose(fGB);
    buffGB[fsizeGB] = 0;

    FILE *fWT = fopen("/usr/share/w48play/Waehlton.raw", "rb");
    fseek(fWT, 0, SEEK_END);
    long fsizeWT = ftell(fWT);
    fseek(fWT, 0, SEEK_SET);  //same as rewind(f);
char *    buffWT = (char *) malloc(fsizeWT + 1);
    fread(buffWT, fsizeWT, 1, fWT);
    fclose(fWT);
    buffWT[fsizeWT] = 0;


    FILE *f = fopen("/usr/share/w48play/Impuls.raw", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);
    buff = (char *) malloc(fsize + 1);
    fread(buff, fsize, 1, f);
    fclose(f);
    buff[fsize] = 0;


if (stream == 1) {
for (loops = (seconds * 1000000) / 2; loops > 0; loops--) {

        if (pcm = snd_pcm_writei(pcm_handle, buffGB, frames) == -EPIPE) {
	    printf("XRUN.\n");
	    snd_pcm_prepare(pcm_handle);
	} else if (pcm < 0) {
	    printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
	}
}
}

if (stream == 2) {
        if (pcm = snd_pcm_writei(pcm_handle, buffWT, frames) == -EPIPE) {
	    printf("XRUN.\n");
	    snd_pcm_prepare(pcm_handle);
	} else if (pcm < 0) {
	    printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
	}
}

if (stream == 3) {
        if (pcm = snd_pcm_writei(pcm_handle, buff, frames) == -EPIPE) {
	    printf("XRUN.\n");
	    snd_pcm_prepare(pcm_handle);
	} else if (pcm < 0) {
	    printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
	}
}




    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    free(buff);

    return 0;
}