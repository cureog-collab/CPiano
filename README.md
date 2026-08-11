# Piano Synthesizer in C
A keyboard written in C for Windows. It maps a laptop keyboard to two chromatic octaves and synthesizes audio in real time. The current instrument uses sine-wave synthesis, so it sounds closer to a low budget organ or synthesizer than an acoustic piano.

# Keyboard layout
The default mapping covers MIDI notes 48 through 71, or C3 through B4.

### Lower octave — C3 to B3

| Note | C3 | C♯3 | D3 | D♯3 | E3 | F3 | F♯3 | G3 | G♯3 | A3 | A♯3 | B3 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Key | <kbd>Z</kbd> | <kbd>S</kbd> | <kbd>X</kbd> | <kbd>D</kbd> | <kbd>C</kbd> | <kbd>V</kbd> | <kbd>G</kbd> | <kbd>B</kbd> | <kbd>H</kbd> | <kbd>N</kbd> | <kbd>J</kbd> | <kbd>M</kbd> |

### Upper octave — C4 to B4

| Note | C4 | C♯4 | D4 | D♯4 | E4 | F4 | F♯4 | G4 | G♯4 | A4 | A♯4 | B4 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Key | <kbd>Q</kbd> | <kbd>2</kbd> | <kbd>W</kbd> | <kbd>3</kbd> | <kbd>E</kbd> | <kbd>R</kbd> | <kbd>5</kbd> | <kbd>T</kbd> | <kbd>6</kbd> | <kbd>Y</kbd> | <kbd>7</kbd> | <kbd>U</kbd> |

Shift down one octave: ```[```

Shift up one octave: ```]```

Quit program: ```esc```

# Structure
* input-process.c/.h
Reads Windows virtual-key state with GetAsyncKeyState()

* controller.c/.h
Maps physical keys to MIDI notes and detects press/release edges

* event-queue.c/.h
Queue note events from the main thread for the audio thread to access

* synth.c/.h
Manages voices, MIDI tuning, oscillators, ADSR, mixing, and clipping

* audio.c/.h
Owns the miniaudio device and real-time data callback

* miniaudio.c
Compiles the miniaudio headerfile

* main.c
Initializes modules, runs the main input loop, and carries cleanup

# Build and run

From PowerShell in the repository root:

```make```
```.\build\piano.exe```

Or build and run in one command:

```make run```

# Dependencies
* miniaudio.h