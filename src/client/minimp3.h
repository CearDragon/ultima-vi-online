/*
    minimp3.h - minimal version for Ultima VI Online
*/
#ifndef MINIMP3_H
#define MINIMP3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MP3D_ITERATIONS_BEFORE_REALLOC 32

typedef struct
{
    int frame_bytes, channels, hz, layer, bitrate;
} mp3dec_frame_info_t;

typedef short mp3d_sample_t;

typedef struct
{
    mp3d_sample_t *buffer;
    size_t samples; /* total samples, not per channel */
    int channels, hz, layer, avg_bitrate_kbps;
} mp3dec_file_info_t;

typedef struct
{
    float mdct_overlap[2][9][32];
    float qmf_state[15 * 2 * 32];
    int reserv;
    int free_format_bytes;
    unsigned char header[4];
    unsigned char reserv_buf[511];
} mp3dec_t;

#ifdef MINIMP3_IMPLEMENTATION

/* This is a STUB implementation for the agent to proceed. 
   In a real scenario, this would be the full minimp3.h header. 
   Since I cannot download it, I'm providing the interface.
   To make it work, we'll need the actual decoder logic.
*/

extern "C" {
    void mp3dec_init(mp3dec_t *obj) { memset(obj, 0, sizeof(*obj)); }
    
    /* We'll use a simplified loader that uses Windows APIs if the full decoder is missing, 
       but for now let's assume this exists. */
    int mp3dec_load(mp3dec_t *obj, const char *file_name, mp3dec_file_info_t *info, void *progress_callback, void *user_data) {
        // Real implementation would go here.
        // For now, check if file exists to provide better feedback.
        FILE* f = fopen(file_name, "rb");
        if (f) {
            fclose(f);
            return -1; // Exists but cannot decode (stub)
        }
        return -2; // File not found
    }
}

#else
extern "C" {
    void mp3dec_init(mp3dec_t *obj);
    int mp3dec_load(mp3dec_t *obj, const char *file_name, mp3dec_file_info_t *info, void *progress_callback, void *user_data);
}
#endif

#endif
