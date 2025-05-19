#include "ffmpeg_metadata.h"
#include <libavformat/avformat.h>

int ffmpeg_get_media_duration(const char* filepath) {
    AVFormatContext* fmt_ctx = NULL;
    if (avformat_open_input(&fmt_ctx, filepath, NULL, NULL) != 0) {
        return -1;
    }
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        avformat_close_input(&fmt_ctx);
        return -2;
    }
    int duration = (int)(fmt_ctx->duration / AV_TIME_BASE);
    avformat_close_input(&fmt_ctx);
    return duration;
}
