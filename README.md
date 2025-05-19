# u_audio

A Flutter-based audio component designed for **U-Notes**, a music player application. This library provides essential audio playback functionality and integrates seamlessly with Android's native audio systems.

## Supported Platforms

- **Android**

## Installation

To integrate `u_audio` into your Flutter project, follow these steps:

1. Clone the repository into the same directory as your project:
   ```bash
   git clone <repository_url>
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

## Features

### Core Functions

- **`int getMediaDuration(String path)`**  
  Retrieves the duration of an audio file (in seconds) from the specified file path.

- **`void startPlayback(String path)`**  
  Initiates playback of the audio file located at the specified path.

- **`void stopPlayback()`**  
  Stops the currently playing audio.

### `PlaybackHandler` Class

The `PlaybackHandler` class manages Android's native **AudioFocus** system, ensuring smooth audio transitions and focus management.

- **`Future<bool> requestAudioFocus()`**  
  Requests audio focus, muting other audio sources and prioritizing the playback of the current audio.

- **`Future<void> abandonAudioFocus()`**  
  Releases audio focus, allowing other applications to resume their audio playback.

## Libraries and Dependencies

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

For additional details or contributions, please refer to the repository's [documentation](./docs) or contact the maintainers.
