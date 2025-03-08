# Minipilot UNIX port

This project is the port of the [minipilot](https://github.com/terzaterza/minipilot) flight controller to a Linux (Unix) environment. Since minipilot does not assume any specific driver implementations, the same minipilot software can run both on hardware and in simulated environments on PCs.

The minipilot source code is downloaded as a git submodule to the [lib](/lib/) folder and built as a static library. This library is then linked to this project's executable.

## Bridge

This port can communicate with any simulator which supports the defined protobuf interface called [bridge](/protobuf/src/bridge.proto). Driver implementations for the accelerometer, gyroscope, telemetry io and other devices pack their requests and responses into protobuf messages and send them via UDP to the simulator socket. Responses are then read and transformed into the format which the driver interfaces expect.

Driver implementations can be found in [drivers](/src/drivers/).

## Unity simulator

One implementation of a simulator is written in C# using the Unity game engine. It allows for realistic (enough) flight dynamics, sensor reading simulation and vehicle configurations.

Vehicle definitions and environment parameters regarding the unity simulator can be found in the [unity](/src/unity/) folder.