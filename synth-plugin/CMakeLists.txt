cmake_minimum_required(VERSION 3.23)

project(ModalSynthPlugin)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(SYNTH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/DelayLine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/ImpactForce.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/LowPass1P.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/MalletResonator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/BarResonator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/SinOsc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/Spatializer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/Synthesizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/TubeResonator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/Utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/Vector3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/Voice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/MonopoleRadiation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/src/DipoleRadiation.cpp
)

set(UNITY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/unity/ModalSynth.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unity/unity-sdk/AudioPluginUtil.cpp
)

add_library(ModalSynth_AudioPlugin SHARED ${SYNTH_SOURCES} ${UNITY_SOURCES})

target_include_directories(ModalSynth_AudioPlugin
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/dsp/include
        ${CMAKE_CURRENT_SOURCE_DIR}/unity/unity-sdk
)

#target_compile_options(ModalSynth_AudioPlugin PRIVATE -Wall -WX)
