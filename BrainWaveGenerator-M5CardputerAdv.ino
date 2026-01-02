#include <M5Cardputer.h>
#include <math.h>

#define SAMPLE_RATE     48000
#define BUFFER_SAMPLES  512

// Preset Parameters
float baseFreq1 = 220.0f;
float baseFreq2 = 280.0f;
float pulseRate = 6.0f;
float maxVariance = 10.0f;
unsigned long varianceInterval = 5000;

float currentFreq1 = baseFreq1;
float currentFreq2 = baseFreq2;

int fieldIndex = 0;
unsigned long lastInputTime = 0;
const unsigned long debounceDelay = 200;
unsigned long lastVarianceTime = 0;

uint8_t volume = 140;

// Phase accumulators
float phase1 = 0.0f;
float phase2 = 0.0f;
float phaseLFO = 0.0f;

int16_t audioBuffer[BUFFER_SAMPLES * 2];

// Editing
String editString = "";
bool inEditMode = false;

// Playback state
bool isPlaying = false;

const uint16_t fieldY[3] = {28, 62, 96};

void applyRandomDrift() {
    float drift = (random(- (int)(maxVariance * 20), (int)(maxVariance * 20) + 1)) / 20.0f;
    currentFreq1 = baseFreq1 + drift;
    currentFreq2 = baseFreq2 + drift;
}

String getDisplayValue(int idx) {
    float val = (idx == 0) ? baseFreq1 : (idx == 1) ? baseFreq2 : pulseRate;
    if (inEditMode && idx == fieldIndex) {
        return editString.length() > 0 ? editString : "0.0";
    }
    return String(val, 1);
}

void drawScreen() {
    M5Cardputer.Display.fillScreen(BLACK);

    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(TFT_WHITE);
    M5Cardputer.Display.setCursor(10, 8);
    M5Cardputer.Display.println("Brainwave Generator");

    M5Cardputer.Display.setTextSize(2);

    for (int i = 0; i < 3; i++) {
        M5Cardputer.Display.setCursor(10, fieldY[i]);
        M5Cardputer.Display.setTextColor(i == fieldIndex ? TFT_YELLOW : TFT_WHITE);
        if (i == 0) M5Cardputer.Display.print("F1: ");
        if (i == 1) M5Cardputer.Display.print("F2: ");
        if (i == 2) M5Cardputer.Display.print("Pulse: ");
        M5Cardputer.Display.print(getDisplayValue(i));
        if (i == fieldIndex && inEditMode) M5Cardputer.Display.print("_");
        M5Cardputer.Display.println(" Hz");
    }

    M5Cardputer.Display.setTextSize(3);
    M5Cardputer.Display.setTextColor(isPlaying ? TFT_GREEN : TFT_RED);
    int statusX = (240 - (isPlaying ? 140 : 130)) / 2;
    M5Cardputer.Display.setCursor(statusX, 140);
    M5Cardputer.Display.println(isPlaying ? "PLAYING" : "STOPPED");

    // Volume display
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(TFT_CYAN);
    M5Cardputer.Display.setCursor(170, 8);
    M5Cardputer.Display.print("Vol: ");
    uint8_t percent = (volume * 100 + 127) / 255;
    M5Cardputer.Display.print(percent);
    M5Cardputer.Display.print("%");

    // Battery level
    M5Cardputer.Display.setTextColor(TFT_LIGHTGREY);
    M5Cardputer.Display.setCursor(10, 225);
    M5Cardputer.Display.printf("Bat: %d%%", M5Cardputer.Power.getBatteryLevel());

    M5Cardputer.Display.setCursor(100, 225);
    M5Cardputer.Display.println("BtnA:Play Fn+;/.:Vol");
}

void updateVolumeDisplayOnly() {
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(TFT_CYAN);
    M5Cardputer.Display.setCursor(170, 8);
    M5Cardputer.Display.fillRect(170, 8, 70, 10, BLACK);
    M5Cardputer.Display.print("Vol: ");
    uint8_t percent = (volume * 100 + 127) / 255;
    M5Cardputer.Display.print(percent);
    M5Cardputer.Display.print("%");
}

void updateActiveFieldLine() {
    uint16_t y = fieldY[fieldIndex];
    M5Cardputer.Display.fillRect(0, y, 240, 30, BLACK);

    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.setCursor(10, y);
    M5Cardputer.Display.setTextColor(TFT_YELLOW);

    if (fieldIndex == 0) M5Cardputer.Display.print("F1: ");
    if (fieldIndex == 1) M5Cardputer.Display.print("F2: ");
    if (fieldIndex == 2) M5Cardputer.Display.print("Pulse: ");

    String display = editString.length() > 0 ? editString : "0.0";
    M5Cardputer.Display.print(display);
    M5Cardputer.Display.print("_");
    M5Cardputer.Display.println(" Hz");
}

void commitEditAndMoveToNext() {
    String valStr = editString.length() == 0 ? "0.0" : editString;
    float newVal = valStr.toFloat();
    if (newVal < 0.1f) newVal = 0.1f;

    if (fieldIndex == 0) baseFreq1 = newVal;
    else if (fieldIndex == 1) baseFreq2 = newVal;
    else pulseRate = newVal;

    currentFreq1 = baseFreq1;
    currentFreq2 = baseFreq2;

    fieldIndex = (fieldIndex + 1) % 3;

    inEditMode = true;
    float nextVal = (fieldIndex == 0) ? baseFreq1 : (fieldIndex == 1) ? baseFreq2 : pulseRate;
    editString = String(nextVal, 1);
    if (editString.endsWith(".0")) editString.remove(editString.length() - 2);

    drawScreen();
}

void enterEditMode() {
    inEditMode = true;
    float val = (fieldIndex == 0) ? baseFreq1 : (fieldIndex == 1) ? baseFreq2 : pulseRate;
    editString = String(val, 1);
    if (editString.endsWith(".0")) editString.remove(editString.length() - 2);
    updateActiveFieldLine();
}

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);

    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Speaker.setVolume(volume);
    M5Cardputer.Speaker.begin();

    randomSeed(analogRead(0));
    lastVarianceTime = millis();

    drawScreen();
}

void loop() {
    M5Cardputer.update();

    if (isPlaying) {
        float inc1 = 2.0f * M_PI * currentFreq1 / SAMPLE_RATE;
        float inc2 = 2.0f * M_PI * currentFreq2 / SAMPLE_RATE;
        float lfoInc = 2.0f * M_PI * pulseRate / SAMPLE_RATE;

        for (int i = 0; i < BUFFER_SAMPLES; i++) {
            float left = sinf(phase1);
            phase1 += inc1;
            if (phase1 >= 2.0f * M_PI) phase1 -= 2.0f * M_PI;

            float lfo = (sinf(phaseLFO) + 1.0f) * 0.5f;
            phaseLFO += lfoInc;
            if (phaseLFO >= 2.0f * M_PI) phaseLFO -= 2.0f * M_PI;

            float right = sinf(phase2) * lfo;
            phase2 += inc2;
            if (phase2 >= 2.0f * M_PI) phase2 -= 2.0f * M_PI;

            audioBuffer[i * 2]     = (int16_t)(left  * 32767.0f * 0.9f);
            audioBuffer[i * 2 + 1] = (int16_t)(right * 32767.0f * 0.9f);
        }

        M5Cardputer.Speaker.playRaw(audioBuffer, BUFFER_SAMPLES * 2, SAMPLE_RATE, true, 1, 0, false);

        if (millis() - lastVarianceTime >= varianceInterval) {
            applyRandomDrift();
            lastVarianceTime = millis();
        }
    }

    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
        if (millis() - lastInputTime < debounceDelay) return;
        lastInputTime = millis();

        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

        if (status.fn) {
            bool volChanged = false;
            for (char c : status.word) {
                if (c == ';') { if (volume < 245) volume += 10; volChanged = true; }
                else if (c == '.') { if (volume > 10) volume -= 10; volChanged = true; }
            }
            if (volChanged) {
                M5Cardputer.Speaker.setVolume(volume);
                updateVolumeDisplayOnly();
            }
            return;
        }

        if (isPlaying) return;  // Block keyboard during playback

        // Automatically enter edit mode on first keypress (no Escape needed)
        if (!inEditMode) {
            enterEditMode();
        }

        bool lineChanged = false;

        if (!status.word.empty()) {
            for (char c : status.word) {
                if (isdigit(c) || (c == '.' && editString.indexOf('.') == -1)) {
                    editString += c;
                    lineChanged = true;
                }
            }
        }

        if (status.del && editString.length() > 0) {
            editString.remove(editString.length() - 1);
            lineChanged = true;
        }

        if (lineChanged) updateActiveFieldLine();

        if (status.enter) {
            commitEditAndMoveToNext();
        }
    }

    if (M5Cardputer.BtnA.wasPressed()) {
        if (inEditMode) {
            String valStr = editString.length() == 0 ? "0.0" : editString;
            float newVal = valStr.toFloat();
            if (newVal < 0.1f) newVal = 0.1f;
            if (fieldIndex == 0) baseFreq1 = newVal;
            else if (fieldIndex == 1) baseFreq2 = newVal;
            else pulseRate = newVal;
            currentFreq1 = baseFreq1;
            currentFreq2 = baseFreq2;
            inEditMode = false;
            editString = "";
            drawScreen();
        }

        isPlaying = !isPlaying;
        if (isPlaying) {
            currentFreq1 = baseFreq1;
            currentFreq2 = baseFreq2;
            applyRandomDrift();
            phase1 = phase2 = phaseLFO = 0.0f;
            M5Cardputer.Speaker.stop();
        } else {
            M5Cardputer.Speaker.stop();
            drawScreen();  // Full redraw when stopping
        }
    }
}
