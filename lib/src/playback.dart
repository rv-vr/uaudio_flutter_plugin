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

void startPlayback(String path) {
  final ptr = path.toNativeUtf8();
  _ffmpegStart(ptr);
  calloc.free(ptr);
}

void stopPlayback() {
  _ffmpegStop();
}

Future<void> seekTo(int milliseconds) async {
  final result = _ffmpegSeekTo(milliseconds);
  if (result != 0) {
    throw Exception('Seek failed with code $result');
  }
}
