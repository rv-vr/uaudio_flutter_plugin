part of uaudio_plugin;


class PlaybackHandler {
  static const MethodChannel _focusChannel =
      MethodChannel('com.unotes.uaudio/audio_focus');

  static Future<bool> requestAudioFocus() async {
    final bool granted = await _focusChannel.invokeMethod('requestFocus');
    return granted;
  }

  static Future<void> abandonAudioFocus() async {
    await _focusChannel.invokeMethod('releaseFocus');
  }
}
