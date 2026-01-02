# BrainWaveGenerator-M5CardputerAdv

A fully standalone binaural beats / brainwave entrainment generator for the **M5Cardputer-Adv**.

This version runs entirely on the device — no Wi-Fi, no web browser, no phone required. Using the built-in keyboard, you can directly set two carrier frequencies (F1 and F2) and a pulse rate. The device generates real-time stereo audio: a steady sine wave in one ear and an amplitude-modulated (pulsed) sine wave in the other, producing classic binaural beats with a natural rhythmic feel.

Periodic random "drift" is applied to both carriers for a more organic, human-like variation.

Designed and optimized specifically for the **M5Cardputer-Adv** with its ES8311 audio codec and 3.5mm headphone jack for the best possible sound quality.

## Features

- **Direct keyboard input** for F1, F2, and Pulse rate
- **Real-time binaural beat generation** with pure sine waves
- **Amplitude-modulated pulsing** on one channel
- **Random drift** for natural, organic variation
- **Volume control** via Fn + ; (up) / . (down)
- **Battery level display**
- **Zero screen refresh during playback** — maximum audio smoothness
- **Immediate editing after stop** — no need to press Escape
- **Fully standalone** — no network or external device needed

## Hardware Requirements

- M5Cardputer-Adv (recommended for superior audio via ES8311 codec and headphone jack)
- Compatible with original M5Cardputer
- Headphones strongly recommended for clean binaural effect

## Setup and Installation

1. Open the Arduino IDE
2. Install the **M5Cardputer** library via Library Manager (search "M5Cardputer")
3. Copy the provided `.ino` file into a new sketch
4. Select board: **M5Cardputer**
5. Upload the code

## Usage

- Edit values using the keyboard:
  - Navigate fields with **Enter**
  - Type numbers or decimals directly
  - Backspace with **Del**
  - Start typing to automatically enter edit mode
- **BtnA**: Toggle Play / Stop
- **Fn + ;** : Volume up
- **Fn + .** : Volume down

During playback, the screen remains static (except when adjusting volume) to ensure perfectly smooth audio.

## Science-Based Frequency Combinations

Suggested combinations based on established brainwave entrainment research. The **pulse** targets specific brainwave bands.

| State                  | F1 (Hz) | F2 (Hz) | Pulse (Hz) | Target Band       | Effect                              |
|------------------------|---------|---------|------------|-------------------|-------------------------------------|
| Deep Relaxation        | 100     | 104     | 4          | Theta (4–8 Hz)    | Deep meditation, creativity         |
| Focus & Concentration  | 200     | 215     | 15         | Beta (13–30 Hz)   | Alertness, focus, problem-solving   |
| Deep Sleep Induction   | 150     | 154     | 4          | Delta (0.5–4 Hz)  | Promotes restorative sleep          |
| Meditation & Mindfulness | 200   | 210     | 10         | Alpha (8–13 Hz)   | Calm, relaxed awareness             |
| Quick Mental Refresh   | 500     | 520     | 20         | High Beta         | Energy boost, mental clarity        |
| Creativity Boost       | 300     | 308     | 8          | Alpha/Theta border| Balanced relaxation + insight       |

**Note**: Higher carrier frequencies keep tones comfortable while the **pulse** delivers the entrainment effect. Results vary — use at a comfortable volume.

These are wellness and exploration tools, not medical devices.

## Credits & Inspiration

This standalone version is derived from the original Wi-Fi/web-based project:  
https://github.com/aat440hz/BrainWaveGenerator-M5Cardputer

Special thanks to the M5Stack community and researchers in sound, binaural beats, and brainwave entrainment.

## License

MIT License — feel free to fork, modify, and share!

Enjoy the waves 🌊🧠💗
