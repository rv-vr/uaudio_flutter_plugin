library uaudio_plugin;

import 'dart:ffi';
import 'dart:io';
import 'package:ffi/ffi.dart';
import 'package:flutter/services.dart';

export 'uaudio.dart' show getMediaDuration, startPlayback, stopPlayback, PlaybackHandler;

part 'src/bindings.dart';
part 'src/metadata.dart';
part 'src/playback.dart'; 