#include <jni.h>
#include "ffmpeg_decoder.h"    // for ffmpeg_seek_to
#include "opensl_playback.h"   // for start/pause/resume/stop

// Note the package & class must match your Kotlin: com.unotes.uaudio.UAudioPlugin

JNIEXPORT jint JNICALL
Java_com_unotes_uaudio_UAudioPlugin_nativeStartPlayback(
    JNIEnv* env, jobject thiz, jstring jpath) {
  const char* path = (*env)->GetStringUTFChars(env, jpath, NULL);
  int ret = ffmpeg_start_playback(path);
  (*env)->ReleaseStringUTFChars(env, jpath, path);
  return ret;
}


JNIEXPORT void JNICALL
Java_com_unotes_uaudio_UAudioPlugin_nativePausePlayback(
    JNIEnv* env, jobject thiz) {
  ffmpeg_pause_playback();
}

JNIEXPORT void JNICALL
Java_com_unotes_uaudio_UAudioPlugin_nativeResumePlayback(
    JNIEnv* env, jobject thiz) {
  ffmpeg_resume_playback();
}

JNIEXPORT void JNICALL
Java_com_unotes_uaudio_UAudioPlugin_nativeStopPlayback(
    JNIEnv* env, jobject thiz) {
  ffmpeg_stop_playback();
}

JNIEXPORT jint JNICALL
Java_com_unotes_uaudio_UAudioPlugin_nativeSeek(
    JNIEnv* env, jobject thiz, jint positionMs) {
  return ffmpeg_seek_to((int64_t)positionMs);
}


JNIEXPORT jlong JNICALL
Java_com_unotes_uaudio_UAudioPlugin_nativeGetCurrentPosition(JNIEnv* env, jobject thiz) {
    return (jlong)ffmpeg_get_current_position();
}

