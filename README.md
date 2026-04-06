# VR Modal Synth

A physical modeling percussion synthesizer controlled in virtual reality. 

## Features
- Physical parameter controls
- 16-voice polyphony
- Velocity and position sensitivity

## Installation / Usage

1. Download the release.
2. Extract the zip file.
3. Connect a VR headset.
4. Run the executable.

(**Note:** This project is still in early development, and releases at this point are largely experimental. It has been tested on an Meta Quest 2 with a wired connection. 
Some audio output devices may have poor performance or latency—the app will use whichever default device is selected.)

## Info
This repository is organized into two subdirectories: 
1. `synth-plugin/` contains the audio source code, wrapped as a Unity Native Audio Plugin.
2. `vr-modal-synth-app/` contains the Unity project for the VR application.

### Opening the Unity Project
To open the **Unity project**:
1. Open **Unity Hub**.
2. Click the `Add` dropdown.
3. Choose `Add project from disk`.
4. Select the `vr-modal-synth-app/` folder.

### Building the Audio Plugin
To build the **Unity Native Audio Plugin**, you will need **CMake**. Navigate to the `synth-plugin/` directory, and execute:
```bash
cmake -S . -B build
cmake --build build
```
The plugin can then be copied to the plugin directory for the Unity project: `vr-modal-synth-app/Assets/ModalVR/Plugins/`

## Planned Features / Releases
- Pedal damping
- Improved interactions
- VST plugin release
