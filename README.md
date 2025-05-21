# uaudio

A Flutter-based audio component designed for **U-Notes**, a music player application. This library provides essential audio playback functionality and integrates seamlessly with Android's native audio systems. 

## Supported Platforms

- **Android**

## Installation

To integrate `uaudio` into your Flutter project, follow these steps:

1. Clone the repository into the same directory as your project:
   ```bash
   git clone https://github.com/rv-vr/uaudio.git
   ```

2. Add the plugin to your project's `pubspec.yaml` file:
   ```yaml
   dependencies:
     uaudio:
       path: ../uaudio
   ```

3. Fetch the dependencies:
   ```bash
   flutter pub get
   ```

## Functions

The following Dart methods are available via the `PlaybackHandler` class.  
**Audio focus is managed automatically on the native (Kotlin) side; you do not need to request or abandon audio focus manually.**

### PlaybackHandler Methods

- **`Future<void> start(String path)`**  
  Starts playback of the audio file at the given path. Automatically requests audio focus.

- **`Future<void> pause()`**  
  Pauses playback. May also be triggered by audio focus loss.

- **`Future<void> resume()`**  
  Resumes playback. May also be triggered by audio focus gain.

- **`Future<void> stop()`**  
  Stops playback and abandons audio focus.

- **`Future<int> seek(int positionMs)`**  
  Seeks to the specified position (in milliseconds) in the current audio track.

### MediaInfo Methods

- **`Future<MediaInfo> getMediaInfo(String path)`**  
  Extracts metadata from the specified file path and returns a `MediaInfo` object.

  The `MediaInfo` class contains:
  - `int duration`
  - `int sampleRate`
  - `int bitRate`
  - `String audioCodec`
  - `String container`
  - `String title`
  - `String artist`
  - `String album`
  - `String genre`
  - `String year`
  - `Uint8List artwork`

---

## Example Implementation

Below is a minimal example of how to use `PlaybackHandler` in your Flutter app:

```dart
import 'package:flutter/material.dart';
import 'package:uaudio/uaudio.dart';

class AudioPlayerExample extends StatelessWidget {
  final String path = "/sdcard/Music/sample.mp3";

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        ElevatedButton(
          onPressed: () async {
            try {
              await PlaybackHandler.start(path);
            } catch (e) {
              print('Failed to start playback: $e');
            }
          },
          child: Text('Play'),
        ),
        ElevatedButton(
          onPressed: () => PlaybackHandler.pause(),
          child: Text('Pause'),
        ),
        ElevatedButton(
          onPressed: () => PlaybackHandler.resume(),
          child: Text('Resume'),
        ),
        ElevatedButton(
          onPressed: () => PlaybackHandler.stop(),
          child: Text('Stop'),
        ),
        ElevatedButton(
          onPressed: () async {
            final code = await PlaybackHandler.seek(30000); // Seek to 30s
            print('Seek result: $code');
          },
          child: Text('Seek to 30s'),
        ),
      ],
    );
  }
}
```

**Note:**  
- Audio focus is handled automatically; you do not need to manage it in Dart.
- Make sure to request storage permissions if accessing files on device storage.

---

For more details, see the [full example in `main.dart`](lib/main.dart).
## Libraries used

### Audio Processing

- **FFmpeg**  
  Utilized for audio decoding, resampling, equalization, and applying advanced audio effects.

### Audio Output

- **OpenSL ES**  
  A legacy audio library for Android, used for audio output.

- **AAudio** *(On development)*  
  A modern audio library for Android, offering improved performance and lower latency.

## Development Tools

The following tools are required for building and maintaining this library:

- **Gradle**  
  For project build automation.

- **CMake**  
  For managing the native build process.

- **Flutter**  
  For cross-platform application development.

- **Android NDK**  
  For compiling native C/C++ code.

---

For additional details or contributions, please refer to the repository's [documentation](https://github.com/example/uaudio/docs) or contact the maintainers.
