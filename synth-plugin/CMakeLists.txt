cmake_minimum_required(VERSION 3.16)

project(ModalSynthAudioPlugin)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_STANDALONE OFF)
option(BUILD_UNITY_PLUGIN "test" ON) 
option(BUILD_VST_PLUGIN OFF)

set(SHARED_SOURCES
    dsp/src/DelayLine.cpp
    dsp/src/Differentiator.cpp
    dsp/src/HighPass1P.cpp
    dsp/src/ImpactForce.cpp
    dsp/src/LowPass1P.cpp
    dsp/src/MalletResonator.cpp
    dsp/src/ModalBank.cpp
    dsp/src/SinOsc.cpp
    dsp/src/Spatializer.cpp
    dsp/src/Synthesizer.cpp
    dsp/src/Tube.cpp
    dsp/src/Utils.cpp
    dsp/src/Vector3.cpp
    dsp/src/Voice.cpp
    dsp/src/MonopoleRadiation.cpp
    dsp/src/DipoleRadiation.cpp
)

set(UNITY_SOURCES
    unity/ModalSynth.cpp
    unity/unity-sdk/AudioPluginUtil.cpp
)

#set(JUCE_SOURCES
#)


if(BUILD_UNITY_PLUGIN)
    add_library(ModalSynthAudioPlugin SHARED ${SHARED_SOURCES} ${UNITY_SOURCES})
    target_include_directories(ModalSynthAudioPlugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/unity/unity-sdk)

    target_include_directories(ModalSynthAudioPlugin PRIVATE
        "${PROJECT_SOURCE_DIR}/dsp/include"
    )
    
    if(WIN32)
        target_compile_definitions(ModalSynthAudioPlugin PRIVATE UNITY_WIN=1)
    elseif(APPLE)
        target_compile_definitions(ModalSynthAudioPlugin PRIVATE UNITY_OSX=1)
    elseif(UNIX)
        target_compile_definitions(ModalSynthAudioPlugin PRIVATE UNITY_LINUX=1)
    endif()
    

endif()
