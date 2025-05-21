part of uaudio_plugin;

class PlaybackHandler {
  static const MethodChannel _channel =
      MethodChannel('com.unotes.uaudio/playback');

  /// Starts playback (and requests audio focus under the hood).
  static Future<void> start(String path) =>
      _channel.invokeMethod('start', {'path': path});

  /// Pauses playback on focus loss or user request.
  static Future<void> pause() =>
      _channel.invokeMethod('pause');

  /// Resumes playback on focus gain or user request.
  static Future<void> resume() =>
      _channel.invokeMethod('resume');

  /// Stops playback and abandons audio focus.
  static Future<void> stop() =>
      _channel.invokeMethod('stop');

  /// Seek to the given position (in milliseconds).
  static Future<int> seek(int positionMs) async {
    final result = await _channel.invokeMethod<int>('seek', {
      'position': positionMs,
    });
    return result ?? -1;
  }

  // Get current position in milliseconds
  static Future<int> getCurrentPosition() async {
    final result = await _channel.invokeMethod<int>('getCurrentPosition');
    return result ?? -1;
  }

}
