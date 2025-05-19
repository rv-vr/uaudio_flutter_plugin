#include <jni.h>

#ifndef UAUDIO_JNI_H
#define UAUDIO_JNI_H

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_unotes_uaudio_AudioFocusHandler_nativePausePlayback(JNIEnv*, jobject);

#ifdef __cplusplus
}
#endif

#endif 