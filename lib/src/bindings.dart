part of uaudio_plugin;

final DynamicLibrary _metaLib = Platform.isAndroid
    ? DynamicLibrary.open('libffmpeg_metadata.so')
    : DynamicLibrary.process();

final DynamicLibrary _playLib = Platform.isAndroid
    ? DynamicLibrary.open('libffmpeg_playback.so')
    : DynamicLibrary.process();

// Private C↔Dart typedefs and lookup functions for playback
typedef _StartC = Void Function(Pointer<Utf8>);
typedef _StartD = void Function(Pointer<Utf8>);
final _StartD _ffmpegStart = _playLib
    .lookup<NativeFunction<_StartC>>('ffmpeg_start_playback')
    .asFunction();

typedef _StopC = Void Function();
typedef _StopD = void Function();
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

// Private C↔Dart typedefs and lookup functions for metadata
typedef _OpenC = Int32 Function(Pointer<Utf8>);
typedef _OpenD = int Function(Pointer<Utf8>);
final _OpenD _ffmpegOpen = _metaLib
    .lookup<NativeFunction<_OpenC>>('ffmpeg_open_media')
    .asFunction();

typedef _CloseC = Void Function();
typedef _CloseD = void Function();
final _CloseD _ffmpegClose = _metaLib
    .lookup<NativeFunction<_CloseC>>('ffmpeg_close_media')
    .asFunction();

typedef _GetTagC = Pointer<Utf8> Function(Pointer<Utf8>);
typedef _GetTagD = Pointer<Utf8> Function(Pointer<Utf8>);
final _GetTagD _ffmpegGetTag = _metaLib
    .lookup<NativeFunction<_GetTagC>>('ffmpeg_get_tag')
    .asFunction();

typedef _GetDurC = Int32 Function();
typedef _GetDurD = int Function();
final _GetDurD _ffmpegGetDuration = _metaLib
    .lookup<NativeFunction<_GetDurC>>('ffmpeg_get_duration')
    .asFunction();

typedef _GetArtC = Int32 Function(Pointer<Pointer<Uint8>>, Pointer<Int32>);
typedef _GetArtD = int Function(Pointer<Pointer<Uint8>>, Pointer<Int32>);
final _GetArtD _ffmpegGetArtwork = _metaLib
    .lookup<NativeFunction<_GetArtC>>('ffmpeg_get_artwork')
    .asFunction();

typedef _DumpTagsC = Void Function();
typedef _DumpTagsD = void Function();
final _DumpTagsD _ffmpegDumpAllTags = _metaLib
    .lookup<NativeFunction<_DumpTagsC>>('ffmpeg_dump_all_tags')
    .asFunction();

// sample rate
typedef _GetSRc = Int32 Function();
typedef _GetSRd = int Function();
final _GetSRd _ffmpegGetSampleRate = _metaLib
    .lookup<NativeFunction<_GetSRc>>('ffmpeg_get_sample_rate')
    .asFunction();

// bit rate
typedef _GetBRc = Int32 Function();
typedef _GetBRd = int Function();
final _GetBRd _ffmpegGetBitRate = _metaLib
    .lookup<NativeFunction<_GetBRc>>('ffmpeg_get_bit_rate')
    .asFunction();

// audio codec
typedef _GetACc = Pointer<Utf8> Function();
typedef _GetACd = Pointer<Utf8> Function();
final _GetACd _ffmpegGetAudioCodec = _metaLib
    .lookup<NativeFunction<_GetACc>>('ffmpeg_get_audio_codec')
    .asFunction();

// container format
typedef _GetCFc = Pointer<Utf8> Function();
typedef _GetCFd = Pointer<Utf8> Function();
final _GetCFd _ffmpegGetContainerFormat = _metaLib
    .lookup<NativeFunction<_GetCFc>>('ffmpeg_get_container_format')
    .asFunction();
