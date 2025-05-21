#include "opensl_playback.h"
#include "ffmpeg_decoder.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#define PCM_BUFFER_SIZE  (2048*2)
static int16_t pcm_bufs[2][PCM_BUFFER_SIZE];
static int cur_buf = 0;

static SLObjectItf      engineObj   = NULL;
static SLEngineItf      engineEng   = NULL;
static SLObjectItf      outputMix   = NULL;
static SLObjectItf      playerObj   = NULL;
static SLPlayItf        playerPlay  = NULL;
static SLAndroidSimpleBufferQueueItf bq = NULL;

static void enqueue_buf();

static void bq_cb(SLAndroidSimpleBufferQueueItf bq, void* ctx) {
    enqueue_buf();
}

int ffmpeg_start_playback(const char* filepath) {

    if (playerPlay != NULL) {
        ffmpeg_stop_playback();
    }

    int ret = ffmpeg_init_decoder(filepath);
    if (ret < 0) {
        return ret;  // propagate the error to caller
    }

    slCreateEngine(&engineObj, 0, NULL, 0, NULL, NULL);
    (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
    (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engineEng);

    (*engineEng)->CreateOutputMix(engineEng, &outputMix, 0, NULL, NULL);
    (*outputMix)->Realize(outputMix, SL_BOOLEAN_FALSE);

    SLDataLocator_AndroidSimpleBufferQueue locBQ = {
        SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2
    };
    SLDataFormat_PCM fmtPCM = {
        SL_DATAFORMAT_PCM,
        2,                              
        48000 * 1000,                   
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,
        SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource src = { &locBQ, &fmtPCM };

    SLDataLocator_OutputMix locOut = { SL_DATALOCATOR_OUTPUTMIX, outputMix };
    SLDataSink sink = { &locOut, NULL };

    const SLInterfaceID ids[1] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE };
    const SLboolean req[1]   = { SL_BOOLEAN_TRUE };
    (*engineEng)->CreateAudioPlayer(
        engineEng, &playerObj, &src, &sink, 1, ids, req
    );
    (*playerObj)->Realize(playerObj, SL_BOOLEAN_FALSE);
    (*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &playerPlay);
    (*playerObj)->GetInterface(playerObj, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &bq);

    (*bq)->RegisterCallback(bq, bq_cb, NULL);

    enqueue_buf();
    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);

    return 0;
}

static void enqueue_buf() {
    int16_t* buf = pcm_bufs[cur_buf];
    int count = ffmpeg_fill_pcm_buffer(buf, PCM_BUFFER_SIZE);
    if (count > 0) {
        (*bq)->Enqueue(bq, buf, count * sizeof(int16_t));
        cur_buf ^= 1;
    } else {
        (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_STOPPED);
    }
}

void ffmpeg_stop_playback() {
    if (playerPlay) {
        (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_STOPPED);
    }

    if (playerObj)  { (*playerObj)->Destroy(playerObj);  playerObj  = NULL; }
    if (outputMix)  { (*outputMix)->Destroy(outputMix);  outputMix  = NULL; }
    if (engineObj)  { (*engineObj)->Destroy(engineObj);  engineObj  = NULL; }

    playerPlay = NULL;
    bq         = NULL;
    engineEng  = NULL;

    ffmpeg_cleanup();
}

void ffmpeg_pause_playback() {
    if (playerPlay) {
        (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PAUSED);
    }
}

void ffmpeg_resume_playback() {
    if (playerPlay) {
        (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
    }
}

