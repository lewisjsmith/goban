# Go (Board game)

## Description:
Exploratory project based around Go. A GUI communicates with a separate engine to play the board game. A Rasperry Pi and camera can capture a physical board state to load into the engine. 

## Project aims:
- Explore program development across multiple languages.
- Implement communication between concurrent processes.
- Bridge hardware-software integration.
- Explore computer vision and machine-learning pipelines. [In-progress]

## Progress:
- C++ engine with CMake and gtest build steps [x]
- Python Pygame GUI [x]
- Std I/O communication [x] 
- Raspberry Pi to local machine video stream over UDP, preview and single frame capture [x]
- Computer vision pipeline for Go stone (piece) colour and position recognitiion.
  - Naive board boundary detection with manual tuning [x]
  - Classifier for recognising stone - and absence of. [ ] [In-progress]





