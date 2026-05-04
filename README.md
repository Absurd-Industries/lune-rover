# LUN-E Explorer

A desk companion rover built on the XIAO ESP32-S3 Sense. The ESP32 is the edge brain — it streams MJPEG frames and accepts motor commands. All intelligence runs in browser JavaScript served from a single embedded web page (`src/page_index.h`).

## Architecture

```
┌─────────────┐   MJPEG :81    ┌──────────────────────────────────────┐
│  ESP32-S3   │ ──────────────> │  Browser (the brain)                 │
│             │                 │                                      │
│  camera     │                 │  MJPEG → <img> → canvas → pixels    │
│  2x servos  │                 │  pixels → buildWorld() → world{}     │
│  WiFi AP/STA│                 │  world{} → decide() → throttle,turn │
│             │ <────────────── │                                      │
└─────────────┘   WS :80       └──────────────────────────────────────┘
                 "30,15"
```

**Data flow every heartbeat (200ms):**
1. Browser draws the `<img>` (live MJPEG stream) onto an 80x60 canvas
2. `getImageData()` extracts raw RGBA pixels
3. Grayscale computed for motion detection
4. `buildWorld(pixels, gray)` classifies every pixel as floor/non-floor, produces `world{}`
5. `decide()` reads `world{}`, runs the FSM, sets `jT` (throttle) and `jS` (steer)
6. WebSocket sends `"throttle,turn"` text frame to ESP32 at 40 Hz
7. ESP32 `drive()` maps to servo PWM

## Hardware

| Component | Detail |
|-----------|--------|
| MCU | Seeed Studio XIAO ESP32-S3 Sense |
| Camera | OV2640 (QVGA 320x240, JPEG quality 15) |
| Drive | 2x continuous-rotation servos (PWM, 50 Hz) |
| Left servo | GPIO 1 |
| Right servo | GPIO 2 |
| Servo neutral | 1500 us, max offset ±200 us |
| Power | USB-C / battery |

## Vision System

### Floor Learning (LEARN state)

On boot (or AUTO re-engage), the rover sits still for 5 frames and samples pixel colors from the bottom-center of the frame to learn what "floor" looks like.

| Parameter | Value | Where |
|-----------|-------|-------|
| Sample region | Bottom 12 rows, center 60% width | `page_index.h` heartbeat, LEARN block |
| Frames sampled | 5 | `floorFrames >= 5` |
| Min std deviation | 8 per channel | `computeFloorRef()` |

Produces `floorRef` with per-channel mean and standard deviation: `{r:{m,s}, g:{m,s}, b:{m,s}}`.

### Floor Classification (`isFloor`)

Per-channel box test in RGB space:

```
isFloor(r,g,b) = |r - mean_r| < FLOOR_TOL * std_r
              AND |g - mean_g| < FLOOR_TOL * std_g
              AND |b - mean_b| < FLOOR_TOL * std_b
```

`FLOOR_TOL` default: **2.5** (adjustable via Tuner slider, range 1.0–5.0)

Higher = more permissive (treats more colors as floor). Lower = stricter.

### Frame Analysis (`buildWorld`)

Every heartbeat, the 80x60 nav frame is split into **5 columns** and analyzed from row 33% down:

```
 Col 0   Col 1   Col 2   Col 3   Col 4
  (L)    (LC)     (C)    (RC)     (R)
┌──────┬──────┬──────┬──────┬──────┐  row 0 (top)
│      │      │      │      │      │  
│      │ (sky / far - ignored) │   │  row 0–33%
│      │      │      │      │      │
├──────┼──────┼──────┼──────┼──────┤  row 33% (startRow)
│      │      │      │      │      │
│  Analyzed zone: floor % per col  │  
│      │      │      │      │      │
├──────┼──────┼──────┼──────┼──────┤  row ~83% (NH-10)
│  Bottom 10 rows: cliff detection │  row 83%–100%
└──────┴──────┴──────┴──────┴──────┘  row 60 (bottom)
```

**Outputs in `world{}`:**

| Field | Type | Meaning |
|-------|------|---------|
| `floorPct[5]` | float[] | Floor fraction per column (0.0–1.0) |
| `floorTotal` | float | Overall floor fraction |
| `cliff` | bool | Bottom 10 rows, center 60% has < `CLIFF_FLOOR_MIN` floor |
| `cliffCols[5]` | bool[] | Per-column cliff detection |
| `blocked` | bool | Center column (`floorPct[2]`) < 0.3 |
| `clearest` | int 0–4 | Column index with highest floor % |
| `motD` | float | Motion delta — % of pixels changed since last frame |
| `motDirBias` | float | Motion bias left vs right (-1 to +1) |

## Decision FSM (`decide()`)

```
         ┌──────────┐
         │   LEARN  │  (boot: 5 frames, no motor)
         └────┬─────┘
              │ floor learned
              v
         ┌──────────┐
    ┌───>│   SCAN   │<──────────────────────┐
    │    └────┬─────┘                       │
    │         │ center > 50% floor          │
    │         v                             │
    │    ┌──────────┐  blocked/cliff   ┌────┴─────┐
    │    │   FWD    │ ───────────────> │  AVOID   │
    │    └──┬───┬───┘                 └────┬─────┘
    │       │   │                          │ center clear
    │       │   │ stuck > STUCK_THRESH     │
    │       │   v                     ┌────┘
    │       │ ┌──────────┐            │
    │       │ │   REV    │ ───────────┘
    │       │ └──────────┘  unstuck
    │       │
    │       │ cliff detected (any state except LEARN)
    │       v
    │  ┌──────────┐
    └──│  CLIFF   │  turn-in-place → flip → reverse if stuck
       └──────────┘
```

### State Details & Motor Values

| State | Throttle (`t`) | Steer (`s`) | Duration / Exit |
|-------|---------------|-------------|-----------------|
| **LEARN** | 0 | 0 | 5 frames → SCAN |
| **SCAN** | 0 | `20 * turnDir` | Center > 50% floor → FWD. 16 ticks → flip direction |
| **FWD** | **30** | `(clearest-2)*8` ± side bias (6) | blocked → AVOID, stuck → REV, cliff → CLIFF |
| **AVOID** | **-10** | `35 * turnDir` | Center > 50% clear → FWD |
| **REV** | **-25** | `20 * turnDir` | stuckN drains to 0 → SCAN |
| **CLIFF** | 0 | `25 * turnDir` | Cliff clears → SCAN. 5 ticks turn, then flip. After 2 flips → reverse (`t=-20`, 3 ticks) |

### Steering Logic in FWD

```javascript
steer = (clearest - 2) * 8      // -16 to +16 based on best column
if left wall:  steer += 6       // nudge right
if right wall: steer -= 6       // nudge left
```

`clearest` is the column (0–4) with the highest floor percentage. Column 2 = center = no steer correction.

### Cliff Recovery Sequence

1. Enter CLIFF → turn in place toward clearest column for 5 ticks (1s at 200ms heartbeat)
2. Still cliff? → flip turn direction, 5 more ticks. Increment `cliffFlips`.
3. After 2 flips (still cliff) → short reverse (`t=-20`, 3 ticks), play "stuck" sound
4. Cliff clears → SCAN, reset flips

## Tunable Parameters

### Via Tuner Panel (live, no reflash)

| Slider | Variable | Default | Range | Effect |
|--------|----------|---------|-------|--------|
| Floor Tol | `FLOOR_TOL` | 2.5 | 1.0–5.0 | Floor color tolerance multiplier. Higher = more permissive |
| Cliff Sens | `CLIFF_FLOOR_MIN` | 0.25 (25%) | 0.10–0.60 | Min floor % in bottom rows before cliff triggers. Lower = more sensitive |
| Speed | `maxSpd` | 55 | 20–100 | Max throttle for manual control (joystick/keyboard) |
| Auto Dly | `autoResumeMs` | 5000 | 2000–15000 | Ms before auto resumes after manual override |
| Stuck Thr | `STUCK_THRESH` | 6 | 3–15 | Consecutive low-motion ticks in FWD before declaring stuck |

### In Code (requires reflash)

| Constant | Value | File | Line | Effect |
|----------|-------|------|------|--------|
| Heartbeat interval | 200 ms | `page_index.h` | heartbeat `setInterval` | Decisions per second (5/s) |
| Rover View fps | 83 ms (~12fps) | `page_index.h` | rover view `setInterval` | Visual overlay frame rate |
| Nav canvas | 80x60 | `page_index.h` | `NW=80, NH=60` | Analysis resolution (pixels iterated per heartbeat) |
| FWD throttle | 30 | `page_index.h` | `decide()` FWD block | Forward speed in auto |
| SCAN steer | 20 | `page_index.h` | `decide()` SCAN block | Turn speed while scanning |
| AVOID throttle | -10 | `page_index.h` | `decide()` AVOID block | Reverse speed while avoiding |
| AVOID steer | 35 | `page_index.h` | `decide()` AVOID block | Turn speed while avoiding |
| REV throttle | -25 | `page_index.h` | `decide()` REV block | Reverse speed when stuck |
| CLIFF steer | 25 | `page_index.h` | `decide()` CLIFF block | Turn speed at cliff |
| Column steer gain | 8 | `page_index.h` | `decide()` FWD block | Steering per column offset |
| Side bias | 6 | `page_index.h` | `decide()` FWD block | Extra steer when wall on one side |
| Blocked threshold | 0.3 | `page_index.h` | `buildWorld()` | Center column floor % below this = blocked |
| LEARN sample rows | bottom 12 | `page_index.h` | heartbeat LEARN block | Rows sampled for floor color |
| LEARN sample width | center 60% | `page_index.h` | heartbeat LEARN block | Width sampled for floor color |
| LEARN frames | 5 | `page_index.h` | heartbeat LEARN block | Frames before floor ref computed |
| Min std dev | 8 | `page_index.h` | `computeFloorRef()` | Floor std floor (prevents zero-division) |
| Motion threshold | 20 | `page_index.h` | `buildWorld()` | Gray delta to count as changed pixel |
| Cliff bottom rows | 10 | `page_index.h` | `buildWorld()` | Rows checked for cliff |
| Cliff center width | 60% | `page_index.h` | `buildWorld()` | Width checked for cliff |
| Map size | 64x64 | `page_index.h` | `MS=64` | Occupancy grid cells |
| Trail length | 30 | `page_index.h` | `mapTrail` | Map trail dots stored |
| Drive cmd rate | 25 ms (40 Hz) | `page_index.h` | `startCmd()` | WebSocket send interval |
| Stream delay | 30 ms (~30fps) | `network.cpp` | `streamHandler` | Firmware MJPEG frame pacing |
| JPEG quality | 15 | `camera.cpp` | `jpeg_quality` | Lower = smaller/faster, noisier |
| Frame buffers | 2 | `camera.cpp` | `fb_count` | Double-buffered for GRAB_LATEST |
| Servo max offset | ±200 us | `config.h` | `MAX_OFFSET` | Physical servo speed limit |
| Safety timeout | 500 ms | `config.h` | `DRIVE_TIMEOUT_MS` | Motors stop if no command |

## Rover View

Toggled via the "Rover View" button. Runs at ~12fps independently of the nav heartbeat. Shows exactly what the rover's vision system sees:

- **Green tint** = classified as floor
- **Red tint** = classified as non-floor (object/unknown)
- **Yellow dots** = motion detected (pixel changed since last frame)
- **Bottom bars** = floor confidence per column (green > 50%, red < 50%)
- **Red column border** = cliff detected in that column
- **Orange arrow** = steering direction (points toward clearest column)
- **Top overlay** = current FSM state + overall floor % + cliff warning

## Camera & Controls

- **RAW** — direct MJPEG stream, zero processing
- **Rover View** — floor/object classification overlay
- **SNAP** — capture current frame as PNG
- **Speed presets** — SLO (25%), MED (55%), MAX (100%) for manual control
- **AUTO** — toggle autonomous mode (re-triggers LEARN on enable)
- **MUTE** — toggle Web Audio sounds

## Networking

- **WebSocket** (`/ws`) — text frames `"throttle,turn"`, 40 Hz
- **HTTP fallback** — `/drive?t=&s=` if WS unavailable
- **Dual servers** — stream on port 81 (core 0), control on port 80 (core 1)
- **ESP-NOW** — auto-pairs with broadcast controllers
- **Captive portal** — redirects to `/wifi` when no saved credentials
- **mDNS** — `explorer.local`

## Performance

| Optimization | Effect |
|-------------|--------|
| Dual-core pinning | Stream on core 0, controls on core 1 — no contention |
| TCP_NODELAY | Eliminates 40 ms Nagle buffering |
| WiFi.setSleep(false) | Prevents modem-sleep radio pauses |
| High task priority | Control server at `configMAX_PRIORITIES - 1` |
| CAMERA_GRAB_LATEST | Always gets freshest frame, skips stale buffers |
| 30 ms stream pacing | ~30 fps MJPEG delivery |
| 80x60 nav canvas | 4,800 pixels per analysis cycle |
| 200 ms heartbeat | 5 decisions/second |

## Project Structure

```
src/
├── main.cpp        Entry point, shared state, safety timeout loop
├── config.h        Pin definitions, servo tuning, network defaults
├── camera.cpp/h    OV2640 init (QVGA, Q15, double-buffered, GRAB_LATEST)
├── drive.cpp/h     Differential drive: drive(throttle,turn) → servo PWM
├── network.cpp/h   WiFi, ESP-NOW, dual HTTP servers, WebSocket, captive portal
├── page_index.h    Companion UI + autonomous brain (HTML/CSS/JS in PROGMEM)
└── page_wifi.h     WiFi setup page (HTML/CSS/JS in PROGMEM)
```

## Building

Requires [PlatformIO](https://platformio.org/).

```bash
pio run                    # Build
pio run --target upload    # Flash
pio device monitor         # Serial output
```

## First-Time Setup

1. Flash the firmware
2. Connect to the `LUN-E` WiFi network (password: `explorer123`)
3. Captive portal opens — select your WiFi, enter password
4. Rover reboots, connects to your network
5. Open `http://explorer.local` to pilot

WiFi credentials persist in NVS. Reset via `/wifi-reset` or reflash.

## Network Defaults

| Setting | Value |
|---------|-------|
| AP SSID | `LUN-E` |
| AP Password | `explorer123` |
| mDNS | `explorer.local` |
| Default channel | 6 |
| Stream port | 81 |
| Control port | 80 |
