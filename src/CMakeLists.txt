cmake_minimum_required(VERSION 3.4.1)
project(uaudio_native)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_library(ffmpeg_metadata SHARED
    ${CMAKE_SOURCE_DIR}/metadata/ffmpeg_metadata.c
)
target_link_libraries(ffmpeg_metadata
    ${CMAKE_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libavformat.so
    ${CMAKE_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libavutil.so
    ${CMAKE_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libavcodec.so
    log
)


add_library(ffmpeg_playback SHARED
    ${CMAKE_SOURCE_DIR}/playback/ffmpeg_decoder.c
    ${CMAKE_SOURCE_DIR}/playback/opensl_playback.c
)

target_link_libraries(ffmpeg_playback
    ${CMAKE_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libavformat.so
    ${CMAKE_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libavcodec.so
    ${CMAKE_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libavutil.so
    ${CMAKE_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libswresample.so
    ${CMAKE_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libswscale.so
    log
    atomic
    m
    OpenSLES
)
