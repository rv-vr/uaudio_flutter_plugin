## v0.1

- Basic playback functionality (play, stop, get duration).
## v0.2

- Extended metadata extraction functionality (mediainfo data model, artwork, audio-specific metadata, song-specific ,metadata)

## v0.3

- Pause/seek function

## v0.4

- Audio focus is now handled entirely on the native (Kotlin) side for Android. Dart API is simplified to direct playback controls via MethodChannel.
- PlaybackHandler methods (start, pause, resume, stop, seek) now automatically manage audio focus without requiring manual intervention from Dart.