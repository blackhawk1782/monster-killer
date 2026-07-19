# Monster Killer — Development Milestones

A retro rail shooter built in C99 with raylib, featuring a DOOM-style 2.5D raycasting engine.

---

## Milestone 1: Engine Core ✅ COMPLETE

**Goal:** Build the foundational engine, renderer, and asset systems.

### Deliverables
- [x] Project structure with modular architecture (`engine/`, `renderer/`, `raycaster/`, `sprites/`, etc.)
- [x] `types.h` with fixed-width scalars, vectors, and engine constants
- [x] Engine lifecycle: init, shutdown, frame loop, game state machine
- [x] 320×200 render target scaled to fullscreen
- [x] DOOM-style raycaster with DDA algorithm
- [x] Textured wall rendering with per-column z-buffer
- [x] Perspective-correct floor casting
- [x] Perspective-correct ceiling casting
- [x] Distance fog (exponential)
- [x] Texture manager (256 slots, name lookup, frame slicing)
- [x] Sprite system with depth sorting and z-buffer clipping
- [x] Sprite atlas loader with animation definitions
- [x] Animation system: loop, once, ping-pong, reverse modes
- [x] Camera system: position, direction, pitch, FOV, bob, shake, zoom
- [x] Rail path system: nodes, move, wait, rotate, pan, zoom, shake
- [x] Map system: grid-based, text file loader, walls/doors/secrets/spawns
- [x] Collision detection: point, circle, ray, line-of-sight
- [x] Particle system with object pools (2048 max)
- [x] Audio manager: 3D positional sound, music streaming, volume layers
- [x] Utility modules: math, memory pools, strings, PCG random

### Acceptance Criteria
- Compiles with zero errors on Windows (MSYS2 MinGW + raylib)
- Runs at 60+ FPS target
- Clean separation of engine, renderer, and game logic

---

## Milestone 2: Player & Weapons ✅ COMPLETE

**Goal:** Implement player aiming, shooting, collision, and the weapon framework.

### Deliverables
- [x] Player struct: health, lives, score, combo, accuracy tracking
- [x] Aiming system: mouse-controlled crosshair, world direction ray, vertical pitch
- [x] Weapon framework: 6 weapon types with data-driven stats
- [x] Hitscan firing from aim direction with vertical aiming support
- [x] Damage application to enemies (with headshot bonus)
- [x] Muzzle flash, screen shake, and recoil per weapon
- [x] Weapon switching (number keys 1-6)
- [x] Reload system with R key and blocking
- [x] Bullet tracers from crosshair to hit point
- [x] Bullet impact marks (decals) on walls/floor/ceiling with correct vertical position
- [x] Blood particles on enemy hits
- [x] Spark particles on wall/floor/ceiling hits
- [x] Score and combo system with accuracy tracking
- [ ] Ammo pickup system (deferred to gameplay loop)
- [ ] Reload animation state (visual only — functional reload works)

### Weapons
| Weapon | Damage | Accuracy | Spread | Magazine | Fire Rate | Recoil |
|---|---|---|---|---|---|---|
| Pistol | 15 | 0.90 | 0.05 | 12 | 0.25s | 0.1 |
| Shotgun | 8×pellets | 0.60 | 0.25 | 8 | 0.80s | 0.4 |
| SMG | 10 | 0.75 | 0.10 | 30 | 0.10s | 0.15 |
| Machine Gun | 12 | 0.70 | 0.12 | 60 | 0.08s | 0.2 |
| Magnum | 45 | 0.95 | 0.03 | 6 | 0.50s | 0.5 |
| Rocket Launcher | 100 | 0.80 | 0.00 | 4 | 1.00s | 0.6 |

### Acceptance Criteria
- Player can aim with mouse and fire with left mouse / Z key
- Weapons have distinct feel (recoil, fire rate, spread)
- Reloading works with R key and blocks firing
- Hitscan collides with walls, floor, ceiling, and enemies at correct vertical position
- Bullet marks persist for 3 seconds and fade out
- Tracers render from crosshair to hit point

---

## Milestone 3: Enemy AI & Animation 🔄 IN PROGRESS

**Goal:** Implement enemy spawning, AI behavior, and sprite animation.

### Deliverables
- [x] Enemy state machine: idle, walk, run, attack, shoot, pain, death
- [x] Basic enemy AI: hearing radius, sight radius, line of sight
- [x] Enemy spawning from code (map marker spawning deferred)
- [x] Enemy hit detection with configurable radius
- [x] Enemy health, armor, damage, death system
- [x] Enemy score value and weak spot multiplier
- [x] Fallback sprite rendering (colored rectangles when no texture)
- [ ] 8-direction sprite rendering based on camera angle
- [ ] Path following and cover points
- [ ] Random idle behaviors (patrol, look around)
- [ ] Reaction delay and accuracy system
- [ ] Enemy weapon variety (pistol, shotgun, melee)
- [ ] Death animations and corpse sprites
- [ ] Enemy variety: grunt, soldier, heavy, sniper, melee

### Enemy Stats Template
| Stat | Grunt | Soldier | Heavy | Sniper | Melee |
|---|---|---|---|---|---|
| Health | 50 | 75 | 150 | 40 | 80 |
| Armor | 0 | 10 | 30 | 0 | 20 |
| Speed | 1.0 | 1.2 | 0.7 | 0.5 | 1.5 |
| Accuracy | 0.6 | 0.75 | 0.5 | 0.9 | 0.0 |
| Hearing | 8 | 10 | 6 | 12 | 6 |
| Sight | 16 | 20 | 14 | 24 | 10 |
| Score | 100 | 200 | 500 | 300 | 150 |

### Acceptance Criteria
- Enemies spawn in open areas (not inside walls)
- Enemies react to player presence with appropriate states
- Headshots deal bonus damage when weak spot enabled
- Death animations play once and leave a corpse sprite
- Enemy sprites render correctly with depth sorting

---

## Milestone 4: Scripted Rail Movement 🔄 IN PROGRESS

**Goal:** Implement the on-rails camera system with scripted paths and events.

### Deliverables
- [x] Rail path node system: position, angle, speed, wait time
- [x] Camera movement along paths with smooth rotation
- [x] Wall collision detection on rail path (prevents camera going through walls)
- [x] Axis sliding when wall collision detected
- [x] Straight corridor test map for rail path
- [ ] Full rail path scripting from map files
- [ ] Camera look targets (lock onto enemies or points)
- [ ] Scripted events at nodes: spawn, door, sound, explosion
- [ ] Stop, wait, and resume movement
- [ ] Camera pan and zoom scripting
- [ ] Scripted camera shake events
- [ ] Path branching and decision points
- [ ] Speed curves (ease-in, ease-out)

### Rail Node Types
| Type | Description |
|---|---|
| `MOVE` | Travel to next node at given speed |
| `WAIT` | Pause for specified time |
| `ROTATE` | Rotate to target angle |
| `PAN` | Pan camera left/right |
| `ZOOM` | Change FOV/zoom level |
| `SHAKE` | Trigger camera shake |
| `EVENT` | Fire a game event |

### Acceptance Criteria
- Camera follows predefined path without player input
- Camera does not pass through walls (collision detection)
- Smooth transitions between nodes
- Events trigger reliably at node boundaries
- Player can still aim and shoot while moving

---

## Milestone 5: Events, Bosses, Scoring & First Mission 🔄 IN PROGRESS

**Goal:** Implement the event system, boss battles, scoring, and a complete playable mission.

### Deliverables
- [x] Event system: 10 event types with trigger volumes
- [x] Boss struct with multi-phase health
- [x] Basic boss AI: phase transitions, attack patterns
- [x] Scoring system: kills, headshots, combos, accuracy
- [x] Combo multiplier system
- [x] Arcade rank system: S+, S, A, B, C, D, E
- [x] Real-time HUD: score, combo, accuracy, health, ammo
- [x] Weapon switching (number keys + mouse wheel)
- [x] Reload indicator and "RELOAD!" warning
- [x] Bullet tracers and impact marks
- [x] Blood, spark, and explosion particle effects
- [ ] Boss introduction cutscenes
- [ ] Boss weak points and helper spawning
- [ ] Special boss attacks (area denial, rapid fire)
- [ ] Boss death sequences
- [ ] Secret discovery scoring
- [ ] Mission complete screen with stats
- [ ] Continue screen and lives system
- [ ] First playable mission map with 3 sections + boss arena

### Scoring Breakdown
| Action | Points |
|---|---|
| Enemy kill | 100 |
| Headshot | +150 bonus |
| Boss defeat | 5,000 |
| Secret found | 500 |
| Combo x2 | ×1.5 |
| Combo x5 | ×2.0 |
| Combo x10 | ×3.0 |
| No-damage section | 1,000 |
| Speed bonus | up to 2,000 |

### Rank Thresholds
| Rank | Score Required |
|---|---|
| S+ | 100,000 |
| S | 80,000 |
| A | 60,000 |
| B | 40,000 |
| C | 20,000 |
| D | 10,000 |
| E | 5,000 |

### Acceptance Criteria
- Complete mission can be played from title to mission complete
- Boss has 3 distinct phases with visual/audio cues
- Player receives a rank based on performance
- Score, combo, and accuracy display in real-time
- Game over and continue flow works correctly

---

## Future Expansion Roadmap

The engine is designed so future developers can add content **without modifying engine code**:

| Feature | How to Add |
|---|---|
| **New Weapons** | Add entry in `weapons/weapon_data.c` |
| **New Enemies** | Add type in `enemy/enemy.c`, spawn via map `E` markers |
| **New Bosses** | Add AI in `boss/boss_ai.c`, spawn via map `B` markers |
| **New Maps** | Create text file in `assets/maps/`, load with `Map_Load()` |
| **New Sprite Sheets** | Add to `assets/sprites/`, load with `SpriteAtlas_Load()` |
| **New Camera Paths** | Define nodes in map file or code |
| **New Events** | Register with `EventSystem_Register()` |
| **New Missions** | New map + rail path + event script |

---

## Build System

### Requirements
- MSYS2 with MinGW-w64 (`G:\Games\developed\engine`)
- raylib 5.0+ (`C:\raylib`)
- Windows 10/11

### Scripts
| Script | Purpose |
|---|---|
| `build.bat` | Release build → `build_msys\MonsterKiller.exe` |
| `build_debug.bat` | Debug build → `build_debug\MonsterKiller_debug.exe` |
| `clean.bat` | Remove all build directories |

### Manual Build
```batch
cd G:\Games\developed\monster-killer
build_debug.bat
build_debug\MonsterKiller_debug.exe
```

---

## Architecture Principles

1. **No global variables** — single engine instance accessor only
2. **Modular headers** — every `.c` has a matching `.h`
3. **Data-driven design** — weapons, enemies, maps configurable
4. **Object pools** — particles and entities use pre-allocated memory
5. **320×200 render target** — pixel-perfect retro resolution
6. **Clean separation** — engine, renderer, game logic independent

---

*Last updated: 2026-07-20*
