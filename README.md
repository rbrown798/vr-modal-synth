# VR Modal Synth

A physical modeling percussion synthesizer controlled in virtual reality. 

## Features
- Physical parameter controls
- 16-voice polyphony
- Velocity and position sensitivity
- Pedal/controller damping

## Info
This repository consists of two parts: 
1. The Unity native audio plugin source code, located in `synth-plugin/`
2. The VR Unity project source code, located in `vr-modal-synth-app/`

## How to build
To build the Unity native audio plugin, nativage to the `synth-plugin/` directory, and execute:
```bash
cmake -S . -B build
cmake --build build
```

To open the Unity project:
1. Open Unity Hub.
2. Click the `Add` dropdown.
3. Choose `Add project from disk`.
4. Select the `vr-modal-synth-app/` folder. 
