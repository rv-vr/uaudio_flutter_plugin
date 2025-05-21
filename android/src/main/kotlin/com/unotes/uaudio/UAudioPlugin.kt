package com.unotes.uaudio

import android.content.Context
import android.media.AudioAttributes
import android.media.AudioFocusRequest
import android.media.AudioManager
import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel

class UAudioPlugin : FlutterPlugin, MethodChannel.MethodCallHandler {
  private lateinit var channel: MethodChannel
  private lateinit var audioManager: AudioManager
  private lateinit var focusRequest: AudioFocusRequest

  override fun onAttachedToEngine(binding: FlutterPlugin.FlutterPluginBinding) {
    channel = MethodChannel(binding.binaryMessenger, "com.unotes.uaudio/playback")
    channel.setMethodCallHandler(this)
    setupAudioFocus(binding.applicationContext)
  }

  override fun onMethodCall(call: MethodCall, result: MethodChannel.Result) {
    when (call.method) {
      "start" -> {
        val path = call.argument<String>("path")!!
        val started = startWithFocus(path)
        if (!started) result.error("AUDIO_FOCUS_FAILED", "Could not gain audio focus", null)
        else result.success(null)
      }
      "pause" -> {
        nativePausePlayback()
        result.success(null)
      }
      "resume" -> {
        nativeResumePlayback()
        result.success(null)
      }
      "stop" -> {
        nativeStopPlayback()
        abandonAudioFocus()
        result.success(null)
      }
      "seek" -> {
        val pos = call.argument<Int>("position") ?: 0
        val code = nativeSeek(pos)
        result.success(code)
      }
      "getCurrentPosition" -> {
        // call the JNI bridge directly:
        val posMs = nativeGetCurrentPosition()
        result.success(posMs)
      }
      else -> result.notImplemented()
    }
  }

  override fun onDetachedFromEngine(binding: FlutterPlugin.FlutterPluginBinding) {
    channel.setMethodCallHandler(null)
  }

  // ─── Audio Focus Setup ────────────────────────────────────────────────────

  private fun setupAudioFocus(context: Context) {
    audioManager = context.getSystemService(Context.AUDIO_SERVICE) as AudioManager
    val listener = AudioManager.OnAudioFocusChangeListener { change ->
      when (change) {
        AudioManager.AUDIOFOCUS_LOSS,
        AudioManager.AUDIOFOCUS_LOSS_TRANSIENT -> nativePausePlayback()
        AudioManager.AUDIOFOCUS_GAIN -> nativeResumePlayback()
      }
    }
    focusRequest = AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
      .setAudioAttributes(
        AudioAttributes.Builder()
          .setUsage(AudioAttributes.USAGE_MEDIA)
          .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
          .build()
      )
      .setOnAudioFocusChangeListener(listener)
      .build()
  }

  private fun startWithFocus(path: String): Boolean {
    val granted = audioManager.requestAudioFocus(focusRequest) ==
                  AudioManager.AUDIOFOCUS_REQUEST_GRANTED
    return if (granted) nativeStartPlayback(path) == 0 else false
  }

  private fun abandonAudioFocus() {
    audioManager.abandonAudioFocusRequest(focusRequest)
  }

  // ─── JNI / Native Methods ────────────────────────────────────────────────

  companion object {
    init { System.loadLibrary("ffmpeg_playback") }
  }

  private external fun nativeStartPlayback(path: String): Int
  private external fun nativePausePlayback()
  private external fun nativeResumePlayback()
  private external fun nativeStopPlayback()
  private external fun nativeSeek(positionMs: Int): Int
  private external fun nativeGetCurrentPosition(): Long
}
