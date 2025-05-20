part of uaudio_plugin;

class MediaInfo {
  final int? duration;
  final int? sampleRate;
  final int? bitRate;
  final String? audioCodec;
  final String? container;
  final String? title;
  final String? artist;
  final String? album;
  final String? genre;
  final String? year;
  final Uint8List? artwork;

  MediaInfo({
    this.duration,
    this.sampleRate,
    this.bitRate,
    this.audioCodec,
    this.container,
    this.title,
    this.artist,
    this.album,
    this.genre,
    this.year,
    this.artwork,
  });
}

Future<MediaInfo> getMediaInfo(String path) async {
  // Open media
  final pPath = path.toNativeUtf8();
  final openCode = _ffmpegOpen(pPath);
  calloc.free(pPath);
  if (openCode < 0) {
    throw Exception('Couldn’t open media (code $openCode)');
  }

  // Tag getter
  String? _getTag(String key) {
    final kPtr = key.toNativeUtf8();
    final tagPtr = _ffmpegGetTag(kPtr);
    calloc.free(kPtr);
    if (tagPtr.address == 0) return null;
    final tag = tagPtr.toDartString();
    malloc.free(tagPtr);
    return tag;
  }

  // Read scalar values
  final duration   = _ffmpegGetDuration();
  final sampleRate = _ffmpegGetSampleRate();
  final bitRate    = _ffmpegGetBitRate();

  // Read codec & container
  String? _getString(Pointer<Utf8> Function() fn) {
    final ptr = fn();
    if (ptr.address == 0) return null;
    final s = ptr.toDartString();
    malloc.free(ptr);
    return s;
  }
  final audioCodec = _getString(_ffmpegGetAudioCodec);
  final container  = _getString(_ffmpegGetContainerFormat);

  // Extract artwork
  final pDataPtr = calloc<Pointer<Uint8>>();
  final pSize    = calloc<Int32>();
  final artLen   = _ffmpegGetArtwork(pDataPtr, pSize);
  Uint8List? artwork;
  if (artLen > 0) {
    final dataPtr = pDataPtr.value;
    artwork = Uint8List.fromList(dataPtr.asTypedList(pSize.value));
    malloc.free(dataPtr);
  }
  calloc.free(pDataPtr);
  calloc.free(pSize);

  // Now read text tags *before* closing the context
  final title  = _getTag('title');
  final artist = _getTag('artist');
  final album  = _getTag('album');
  final genre  = _getTag('genre');
  final year   = _getTag('date') ?? _getTag('year');

  // Close the FFmpeg context
  _ffmpegClose();

  // Return the assembled MediaInfo
  return MediaInfo(
    duration:   duration,
    sampleRate: sampleRate,
    bitRate:    bitRate,
    audioCodec: audioCodec,
    container:  container,
    title:      title,
    artist:     artist,
    album:      album,
    genre:      genre,
    year:       year,
    artwork:    artwork,
  );
}

void dumpAllTags() {
  _ffmpegDumpAllTags();
}
