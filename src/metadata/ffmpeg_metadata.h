#ifndef FFMPEG_METADATA_H
#define FFMPEG_METADATA_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Call once to open and parse the file:
int ffmpeg_open_media(const char* filepath);

// Call to close and free everything:
void ffmpeg_close_media(void);

// Helper: get a tag value by name (e.g., "artist", "album", "year", "genre", "title")
// Returns a newly‐malloc’d UTF‐8 string, or NULL if not found.
// Caller must free() the result.
char* ffmpeg_get_tag(const char* tag_name);

// Helper: get duration in seconds (as before)
int ffmpeg_get_duration(void);

// Helper: get attached artwork
// Returns a newly‐malloc’d buffer in *out_data, length in *out_size, or 0 on no art.
// Caller must free(*out_data).
int ffmpeg_get_artwork(uint8_t** out_data, int* out_size);

/** Sample rate (Hz) of the first audio stream, or –1 on error */
int ffmpeg_get_sample_rate(void);

/** Overall bit‑rate of the container (bits per second), or –1 on error */
int ffmpeg_get_bit_rate(void);

/** Name of the audio codec (e.g. "aac", "mp3"); returns malloc’d string or NULL */
char* ffmpeg_get_audio_codec(void);

/** Name of the container format (e.g. "mp3", "mov,mp4,m4a,3gp,3g2,mj2"); returns malloc’d string or NULL */
char* ffmpeg_get_container_format(void);


#ifdef __cplusplus
}
#endif
#endif // FFMPEG_METADATA_H
