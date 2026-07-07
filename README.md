# Go (Board game)

## Description:
Exploratory project based around Go. A GUI communicates with an engine to modifying game state. A Rasperry Pi and camera can capture a physical board state to load into the engine. 

## Project aims:
- Explore program development across multiple languages
- Implement communication between concurrent processes
- Bridge hardware-software integration
- Explore computer vision and machine-learning pipelines

## Progress:
- C++ engine with CMake and gtest build steps ✓
  - Single and group stone detection and elimination ✓
  - Self-capture detection ✓
  - Ko repetition detection ✓
  - Scoring [To-do]
  - AI-bot adversary [To-do] 
- Python Pygame GUI ✓
  - Place pieces ✓
  - Buttons for board actions (reset, resize, scan a photo) [To-do] 
- Std I/O communication ✓
- Raspberry Pi to local machine video stream over UDP, preview and single frame capture. Commands sent via SSH. ✓
- Computer vision pipeline for Go stone (piece) colour and position recognitiion ✓
  - Naive board boundary detection with manual tuning ✓
  - Classifier for recognising the stone, or absence of ✓
  - 9x9 and 13x13 scanning [To-do] 
  - Automatic tuning [To-do] 

## Approach:
*Write-up coming soon...*





