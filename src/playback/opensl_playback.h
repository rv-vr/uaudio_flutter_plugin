#ifndef OPENSL_PLAYBACK_H
#define OPENSL_PLAYBACK_H

#ifdef __cplusplus
extern "C" {
#endif


void ffmpeg_start_playback(const char* filepath);


void ffmpeg_stop_playback();

#ifdef __cplusplus
}
#endif

#endif 
