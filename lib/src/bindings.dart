part of uaudio_plugin;

final DynamicLibrary _metaLib = Platform.isAndroid
    ? DynamicLibrary.open('libffmpeg_metadata.so')
    : DynamicLibrary.process();

final DynamicLibrary _playLib = Platform.isAndroid
    ? DynamicLibrary.open('libffmpeg_playback.so')
    : DynamicLibrary.process();

// Private C↔Dart typedefs
typedef _GetDurationC = Int32 Function(Pointer<Utf8>);
typedef _GetDurationD = int Function(Pointer<Utf8>);
typedef _StartC       = Void Function(Pointer<Utf8>);
typedef _StartD       = void Function(Pointer<Utf8>);
typedef _StopC        = Void Function();
typedef _StopD        = void Function();

final _GetDurationD _ffmpegGetDuration = _metaLib
    .lookup<NativeFunction<_GetDurationC>>('ffmpeg_get_media_duration')
    .asFunction();

final _StartD _ffmpegStart = _playLib
    .lookup<NativeFunction<_StartC>>('ffmpeg_start_playback')
    .asFunction();

final _StopD _ffmpegStop = _playLib
    .lookup<NativeFunction<_StopC>>('ffmpeg_stop_playback')
    .asFunction();

void startPlayback(String path) {
  final ptr = path.toNativeUtf8();
  _ffmpegStart(ptr);
  calloc.free(ptr);
}

void stopPlayback() {
  _ffmpegStop();
}
