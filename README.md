# Piano Synthesizer in C
A keyboard written in C for Windows. It maps a laptop keyboard to two chromatic octaves and synthesizes audio in real time. The current instrument uses sine-wave synthesis, so it sounds closer to a low budget organ or synthesizer than an acoustic piano.

# Keyboard layout
The default mapping covers MIDI notes 48 through 71, or C3 through B4.

**Lower octave**

Lower octave
Black:    S   D       G   H   J
White:  Z   X   C   V   B   N   M
Notes:  C  C#   D  D#   E   F  F#   G  G#   A  A#   B

Upper octave
Black:    2   3       5   6   7
White:  Q   W   E   R   T   Y   U
Notes:  C  C#   D  D#   E   F  F#   G  G#   A  A#   B

Shift down one octave: [

Shift up one octave:   ]

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