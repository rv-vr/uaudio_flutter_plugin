package com.unotes.uaudio

import android.content.Context
import android.media.AudioManager

class AudioFocusHelper(private val context: Context) {
    private val audioManager = context.getSystemService(Context.AUDIO_SERVICE) as AudioManager

    fun requestFocus(): Boolean {
        return audioManager.requestAudioFocus(
            focusChangeListener,
            AudioManager.STREAM_MUSIC,
            AudioManager.AUDIOFOCUS_GAIN
        ) == AudioManager.AUDIOFOCUS_REQUEST_GRANTED
    }

    fun releaseFocus() {
        audioManager.abandonAudioFocus(focusChangeListener)
    }

    private val focusChangeListener = AudioManager.OnAudioFocusChangeListener { focusChange ->
        when (focusChange) {
            AudioManager.AUDIOFOCUS_LOSS -> nativeStopPlayback()
        }
    }

    external fun nativeStopPlayback()
}