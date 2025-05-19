part of '../uaudio.dart';

int getMediaDuration(String path) {
  final ptr = path.toNativeUtf8();
  final secs = _ffmpegGetDuration(ptr);
  calloc.free(ptr);
  return secs;
}
