#ifndef FFMPEG_METADATA_H
#define FFMPEG_METADATA_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns duration in seconds, or negative on error:
 *  -1 = cannot open file
 *  -2 = cannot find stream info
 */
int ffmpeg_get_media_duration(const char* filepath);

#ifdef __cplusplus
}
#endif

#endif 
