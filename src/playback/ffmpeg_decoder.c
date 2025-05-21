#include "ffmpeg_decoder.h"
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <string.h>

// FFmpeg state
static char             g_filepath[1024];
static AVFormatContext* g_fmt_ctx = NULL;
static AVCodecContext*  g_dec_ctx = NULL;
static SwrContext*      g_swr_ctx = NULL;
static int              g_stream_idx = -1;
static AVPacket*        g_pkt = NULL;
static AVFrame*         g_frame = NULL;

void ffmpeg_init_decoder(const char* filepath) {
    strncpy(g_filepath, filepath, sizeof(g_filepath)-1);
    g_filepath[sizeof(g_filepath)-1] = '\0';

    avformat_network_init();
    g_fmt_ctx = avformat_alloc_context();
    avformat_open_input(&g_fmt_ctx, g_filepath, NULL, NULL);
    avformat_find_stream_info(g_fmt_ctx, NULL);

    g_stream_idx = av_find_best_stream(
        g_fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0
    );
    AVStream* st = g_fmt_ctx->streams[g_stream_idx];

    const AVCodec* codec = avcodec_find_decoder(st->codecpar->codec_id);
    g_dec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(g_dec_ctx, st->codecpar);
    avcodec_open2(g_dec_ctx, codec, NULL);

    g_swr_ctx = swr_alloc();

    av_opt_set_chlayout(g_swr_ctx, "in_chlayout", &g_dec_ctx->ch_layout, 0);
    av_opt_set_int(g_swr_ctx, "in_sample_rate",
                   g_dec_ctx->sample_rate, 0);
    av_opt_set_sample_fmt(g_swr_ctx, "in_sample_fmt",
                          g_dec_ctx->sample_fmt, 0);

    AVChannelLayout out_chlayout = AV_CHANNEL_LAYOUT_STEREO;
    av_opt_set_chlayout(g_swr_ctx, "out_chlayout", &out_chlayout, 0);
    av_opt_set_int(g_swr_ctx, "out_sample_rate",
                   48000, 0);
    av_opt_set_sample_fmt(g_swr_ctx, "out_sample_fmt",
                          AV_SAMPLE_FMT_S16, 0);

    swr_init(g_swr_ctx);


    g_pkt   = av_packet_alloc();
    g_frame = av_frame_alloc();
}

int ffmpeg_fill_pcm_buffer(int16_t* outBuf, int max_samples) {
    int out_count = 0;
    uint8_t* out_planes[1] = { (uint8_t*)outBuf };

    while (out_count == 0) {
        if (av_read_frame(g_fmt_ctx, g_pkt) < 0) {
            return 0;
        }
        if (g_pkt->stream_index == g_stream_idx) {
            avcodec_send_packet(g_dec_ctx, g_pkt);
            if (avcodec_receive_frame(g_dec_ctx, g_frame) == 0) {
                int samples_converted = swr_convert(
                    g_swr_ctx,
                    out_planes,         
                    max_samples / 2,    
                    (const uint8_t**)g_frame->data,
                    g_frame->nb_samples 
                );
                out_count = samples_converted * 2;  
            }
        }
        av_packet_unref(g_pkt);
    }
    return out_count;
}

int ffmpeg_seek_to(int64_t ms) {
    if (!g_fmt_ctx || g_stream_idx < 0)
        return -1;

    int64_t ts = (ms * AV_TIME_BASE) / 1000;

    // Seek
    if (av_seek_frame(g_fmt_ctx, -1, ts, AVSEEK_FLAG_BACKWARD) < 0) {
        return -2;
    }

    // Flush decoder buffers
    avcodec_flush_buffers(g_dec_ctx);

    // ❌ DON'T free swr_ctx unless you recreate it properly
    // if (g_swr_ctx) swr_free(&g_swr_ctx);

    return 0;
}


void ffmpeg_cleanup(void) {
    if (g_frame)    { av_frame_free(&g_frame);       g_frame   = NULL; }
    if (g_pkt)      { av_packet_free(&g_pkt);        g_pkt     = NULL; }
    if (g_swr_ctx)  { swr_free(&g_swr_ctx);          g_swr_ctx = NULL; }
    if (g_dec_ctx)  { avcodec_free_context(&g_dec_ctx); g_dec_ctx = NULL; }
    if (g_fmt_ctx)  { avformat_close_input(&g_fmt_ctx); g_fmt_ctx = NULL; }
    g_stream_idx = -1;
}
