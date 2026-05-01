# LUN-E Companion UI — Implementation Plan

## Phase 1: Layout Shell [DONE]
- [x] 3-column CSS grid (left: identity+controls+telemetry, center: camera+dialogue+log, right: map+quests)
- [x] Camera stream, joystick, keyboard, visual modes all working in new layout
- [x] Mobile stacking via media query

## Phase 2: Personality Engine + Eyes [DONE]
- [x] `mood` state object with 7 moods (HAPPY, SCARED, EXCITED, ANNOYED, CURIOUS, CONTENT, SLEEPY)
- [x] `updateMood()` in 500ms heartbeat — transitions from cliff/motion/stuck/idle/energy
- [x] CSS animated eyes — pupil tracking, blink, per-mood expressions (squint, wide, droopy, asymmetric)
- [x] Motor emotes — happy spin, scared shake+reverse, excited wiggles, annoyed jerks, curious turn, sleepy decel
- [x] Ambient mood modifiers in nav FSM (content sway, curious slow, sleepy pause)
- [x] Accent color shifts per mood

## Phase 3: Sound System [DONE]
- [x] `SND` object with `chirp()`, `seq()`, `play()`
- [x] Procedural sounds for all moods + quest events + cliff + boot
- [x] Hooked into mood transitions, quest completions, cliff detection
- [x] Mute toggle

## Phase 4: Dialogue System [DONE]
- [x] Typewriter effect at 30ms/char
- [x] Message pools per mood (7 pools, 5 messages each)
- [x] Human text input with keyword parser (go, stop, look, yes/no, quest, hello, sleep)
- [x] COMMS panel with scrolling message history

## Phase 5: Quest System [DONE]
- [x] 5 demo quests (PIONEER, EDGEWALKER, WATCHDOG, MOTION HUNTER, SOCIAL BUTTERFLY)
- [x] Quest tracker UI (active card + progress bar + quest log)
- [x] `checkQuests()` in heartbeat loop
- [x] Quest unlock progression (PIONEER unlocks EDGEWALKER + WATCHDOG)
- [x] Quest events trigger dialogue + sound

## Phase 6: Reactive Layer + Idle [DONE]
- [x] `reactiveLayer()` with novelty detection (running brightness avg vs current zone comparison)
- [x] Environment classification (open / cluttered / corridor / corner) from edge density patterns
- [x] Motion direction detection (left-half vs right-half motD → `motDirBias`)
- [x] Curious mood biases steering toward motion source
- [x] Idle behaviors when boredom exceeds thresholds:
  - Look around (slow turn for 4s)
  - Wiggle (quick back-and-forth personality display)
  - Settle (find corner, stop for 2s, say "Nice spot")
  - Seek attention (request messages every 15s when social need high)
  - Nap (SLEEPY, stop for 10s)
- [x] Priority system: manual > emote > idle behavior > nav FSM
- [x] 7 request messages ("Can you move that?", "Show me something new!", etc.)
- [x] Reactive novelty triggers CURIOUS mood with dialogue

## Phase 7: Polish [TODO]
- [ ] Tune mood transition thresholds from real-world testing
- [ ] Eye polish: smoother blink timing, pupil easing, breathing glow on mood indicator
- [ ] Subtle CSS animations (panel borders pulse with mood, energy bar color)
- [ ] Quest UI improvements (completion animations, unlock notifications)
- [ ] Full end-to-end loop test: boot → idle → curious → explore → cliff → scare → recovery → quest → human chat → happy → sleepy
