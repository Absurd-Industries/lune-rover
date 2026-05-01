# LUN-E Explorer

A web controlled UI for https://github.com/whybitlabs/T-01-The-Explorer; built on the XIAO ESP32-S3 Sense. MJPEG camera stream, WebSocket drive controls, ESP-NOW pairing, captive portal WiFi setup, and experimental autonomous navigation — all served from a single embedded web page.

## Hardware

| Component | Detail |
|-----------|--------|
| MCU | Seeed Studio XIAO ESP32-S3 Sense |
| Camera | OV2640 (QVGA, JPEG quality 15) |
| Drive | 2x continuous-rotation servos (PWM, 50 Hz) |
| Left servo | GPIO 1 |
| Right servo | GPIO 2 |
| Power | USB-C / battery |

The camera pin mapping is for the XIAO ESP32-S3 Sense onboard camera module — see `src/config.h` for the full pinout.

## Features

### Drive
- **Touchscreen joystick** — drag to steer, proportional throttle and turn
- **WASD / arrow keys** — hold to drive, multi-key support (forward + turn)
- **Speed presets** — SLO (25%), MED (55%), MAX (100%)
- **500 ms safety timeout** — motors stop if no command is received

### Camera & Visual Modes
- **MJPEG stream** at ~20 fps on port 81
- **RAW** — direct stream, zero processing overhead
- **NVIS** — night vision (green-tinted brightness boost)
- **EDGE** — Sobel edge detection
- **THRM** — false-color thermal mapping
- **MOTN** — inter-frame motion detection
- **ENHANCE** — contrast and saturation boost
- **CAM OFF** — disable stream to save bandwidth
- **FULL / FIT** — toggle between object-fit cover and natural size
- **SNAP** — capture a still frame as PNG

### Networking
- **WebSocket drive** (`/ws`) — binary-lean text frames (`throttle,turn`), 40 Hz send rate, ~6 bytes per command
- **HTTP fallback** — `/drive?t=&s=` if WebSocket is unavailable
- **Dual HTTP servers** — stream server on port 81 (core 0), control server on port 80 (core 1)
- **ESP-NOW** — pairs with handheld controllers automatically on broadcast, Layer 2 with ~1-2 ms latency
- **Captive portal** — when no WiFi is saved, any HTTP request redirects to `/wifi` setup page
- **mDNS** — accessible at `explorer.local` when connected to a network
- **WiFi setup page** — scan for networks, enter credentials, saved to NVS (persists across reboots)

### Autonomous Navigation (disabled in UI, code preserved)
- Floor color learning (samples bottom-center patch over 5 frames)
- Cliff detection (brightness gradient + floor color deviation)
- Per-zone edge-density obstacle scoring (5x3 grid)
- Dead-reckoning occupancy grid (64x64) with mini-map visualization
- Wall following with exploration bias toward unknown cells
- FSM: LEARN → SCAN → FWD → CLIFF / AVOID / REV

## Performance Optimizations

| Optimization | Effect |
|-------------|--------|
| Dual-core pinning | Stream on core 0, controls on core 1 — no contention |
| TCP_NODELAY | Disables Nagle's algorithm, eliminates 40 ms buffering delay |
| WiFi.setSleep(false) | Prevents 50-100 ms modem-sleep radio pauses |
| High task priority | Control server runs at `configMAX_PRIORITIES - 1` |
| Low timeouts | `recv_wait_timeout` and `send_wait_timeout` set to 1 ms |
| JPEG quality 15 | Smaller frames, faster WiFi transfer at QVGA |
| 80x60 nav canvas | Autonomous analysis runs on a tiny downsampled frame |
| Visual mode throttle | Post-processing capped at ~12 fps via setInterval(83) |

## Project Structure

```
src/
├── main.cpp        Entry point, shared state, setup/loop
├── config.h        Pin definitions, servo tuning, network defaults
├── camera.cpp/h    OV2640 init and sensor tuning
├── drive.cpp/h     Differential drive, servo PWM, safety stop
├── network.cpp/h   WiFi, ESP-NOW, HTTP servers, WebSocket, captive portal
├── page_index.h    Full pilot UI (HTML/CSS/JS in PROGMEM)
└── page_wifi.h     WiFi setup page (HTML/CSS/JS in PROGMEM)
```

## Building

Requires [PlatformIO](https://platformio.org/).

```bash
# Build
pio run

# Upload
pio run --target upload

# Monitor serial
pio device monitor
```

The board config targets `esp32-s3-devkitc-1` with pioarduino platform, PSRAM enabled, and USB CDC on boot. See `platformio.ini` for details.

## First-Time Setup

1. Flash the firmware
2. Connect to the `LUN-E` WiFi network (password: `explorer123`)
3. A captive portal will open — select your home WiFi and enter the password
4. The rover reboots and connects to your network
5. Open `http://explorer.local` in a browser to pilot

WiFi credentials persist in NVS across power cycles. To reset, hit `/wifi-reset` or reflash.

## Network Defaults

| Setting | Value |
|---------|-------|
| AP SSID | `LUN-E` |
| AP Password | `explorer123` |
| mDNS | `explorer.local` |
| Default channel | 6 |
| Stream port | 81 |
| Control port | 80 |
