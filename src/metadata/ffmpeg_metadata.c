#include "ffmpeg_metadata.h"
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <libavutil/mem.h>      // av_free(), etc.
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <android/log.h>

#define LOG_TAG "FFmpegMeta"
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
static AVFormatContext* fmt_ctx = NULL;

// Open and read headers+tags
int ffmpeg_open_media(const char* filepath) {
    ALOGI("Opening media: %s", filepath);

    if (fmt_ctx) {
        avformat_close_input(&fmt_ctx);
        fmt_ctx = NULL;
    }

    int ret = avformat_open_input(&fmt_ctx, filepath, NULL, NULL);
    if (ret < 0) {
        ALOGI("avformat_open_input failed: %d", ret);
        return ret;
    }

    ret = avformat_find_stream_info(fmt_ctx, NULL);
    if (ret < 0) {
        ALOGI("avformat_find_stream_info failed: %d", ret);
        avformat_close_input(&fmt_ctx);
        fmt_ctx = NULL;
        return ret;
    }

    ALOGI("Media opened successfully.");
    return 0;
}

void ffmpeg_close_media(void) {
    if (!fmt_ctx) return;
    avformat_close_input(&fmt_ctx);
    fmt_ctx = NULL;
}

// Look up a dictionary tag
char* ffmpeg_get_tag(const char* tag_name) {
    if (!fmt_ctx) return NULL;

    // 1) Try global (format‑level)
    AVDictionaryEntry* tag = 
      av_dict_get(fmt_ctx->metadata, tag_name, NULL, 0);
    if (!tag) {
        // 2) Try each stream’s metadata
        for (unsigned i = 0; i < fmt_ctx->nb_streams; i++) {
            tag = av_dict_get(fmt_ctx->streams[i]->metadata, tag_name, NULL, 0);
            if (tag) break;
        }
    }
    if (!tag) return NULL;

    // Copy the value into a malloc’d buffer
    size_t len = strlen(tag->value);
    char* out = malloc(len + 1);
    memcpy(out, tag->value, len+1);
    return out;
}


int ffmpeg_get_duration(void) {
    if (!fmt_ctx) return -1;
    return (int)(fmt_ctx->duration / AV_TIME_BASE);
}

// Extract attached picture (album art) if any
int ffmpeg_get_artwork(uint8_t** out_data, int* out_size) {
    if (!fmt_ctx) return 0;
    // Look for first video stream with attached_pic disposition
    for (unsigned i = 0; i < fmt_ctx->nb_streams; i++) {
        AVStream* st = fmt_ctx->streams[i];
        if (st->disposition & AV_DISPOSITION_ATTACHED_PIC) {
            AVPacket* pkt = &st->attached_pic;
            *out_size = pkt->size;
            *out_data = malloc(pkt->size);
            if (!*out_data) return 0;
            memcpy(*out_data, pkt->data, pkt->size);
            return pkt->size;
        }
    }
    return 0;
}

void ffmpeg_dump_all_tags(void) {
    if (!fmt_ctx) {
        ALOGI("No media open");
        return;
    }

    AVDictionaryEntry* tag = NULL;
    ALOGI("== Global (format) tags ==");
    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        ALOGI("   %s = %s", tag->key, tag->value);
    }

    for (unsigned i = 0; i < fmt_ctx->nb_streams; i++) {
        ALOGI("== Stream %u tags ==", i);
        tag = NULL;
        while ((tag = av_dict_get(fmt_ctx->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            ALOGI("   %s = %s", tag->key, tag->value);
        }
        // Check attached pic disposition too
        if (fmt_ctx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
            ALOGI("   (stream %u has attached artwork)", i);
        }
    }
}

int ffmpeg_get_sample_rate(void) {
    if (!fmt_ctx) return -1;
    // find the first audio stream
    for (unsigned i = 0; i < fmt_ctx->nb_streams; i++) {
        AVStream* st = fmt_ctx->streams[i];
        if (st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            return st->codecpar->sample_rate;
        }
    }
    return -1;
}

int ffmpeg_get_bit_rate(void) {
    if (!fmt_ctx) return -1;
    return (int)(fmt_ctx->bit_rate);
}

char* ffmpeg_get_audio_codec(void) {
    if (!fmt_ctx) return NULL;
    for (unsigned i = 0; i < fmt_ctx->nb_streams; i++) {
        AVStream* st = fmt_ctx->streams[i];
        if (st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            const char* name = avcodec_get_name(st->codecpar->codec_id);
            size_t len = strlen(name);
            char* out = malloc(len+1);
            memcpy(out, name, len+1);
            return out;
        }
    }
    return NULL;
}

char* ffmpeg_get_container_format(void) {
    if (!fmt_ctx) return NULL;
    const char* name = fmt_ctx->iformat->name;  // short name
    size_t len = strlen(name);
    char* out = malloc(len+1);
    memcpy(out, name, len+1);
    return out;
}
