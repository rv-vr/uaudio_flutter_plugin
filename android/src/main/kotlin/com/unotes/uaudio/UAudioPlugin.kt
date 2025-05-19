package com.unotes.uaudio

import android.content.Context
import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel

class UAudioPlugin : FlutterPlugin, MethodChannel.MethodCallHandler {
  private lateinit var channel: MethodChannel
  private lateinit var audioFocus: AudioFocusHelper

  override fun onAttachedToEngine(binding: FlutterPlugin.FlutterPluginBinding) {
    channel = MethodChannel(binding.binaryMessenger, "com.unotes.uaudio/audio_focus")
    channel.setMethodCallHandler(this)
    audioFocus = AudioFocusHelper(binding.applicationContext)
  }

  override fun onMethodCall(call: MethodCall, result: MethodChannel.Result) {
    when (call.method) {
      "requestFocus" -> {
        val granted = audioFocus.requestFocus()
        result.success(granted)
      }
      "releaseFocus" -> {
        audioFocus.releaseFocus()
        result.success(null)
      }
      else -> result.notImplemented()
    }
  }

  override fun onDetachedFromEngine(binding: FlutterPlugin.FlutterPluginBinding) {
    channel.setMethodCallHandler(null)
  }
}
