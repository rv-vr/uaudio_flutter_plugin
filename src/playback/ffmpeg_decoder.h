#ifndef FFMPEG_DECODER_H
#define FFMPEG_DECODER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


int ffmpeg_init_decoder(const char* filepath);
int ffmpeg_fill_pcm_buffer(int16_t* outBuf, int max_samples);
int ffmpeg_seek_to(int64_t ms);
void ffmpeg_cleanup(void);
int ffmpeg_get_current_position();


#ifdef __cplusplus
}
#endif

#endif 
