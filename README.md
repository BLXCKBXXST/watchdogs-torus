# watchdogs-torus

```
                                          g▒g▒p▒g▒▒g
                                        pN▒g▒p▒g▒▒▓▒ge
                                     ▒▒▒▒▒▒▓▓▒▓▒▓▒
                                _0▒▓▒▓▒▓▓▒▒▒▒▒▒▒!
                                4▒▒▒▒▒▓▓▓▒▓▓▒▒▒▒▒Y
                               |` ˜˜ ˜ #00▓▓0▒MMM"m|
                                            `gM▓M7
                            |               00Q0                    |
                             #▒____g▒0▓▓P______0
                             #▒0g_p#▓▓04▒▒&,__M#
                             0▒▒▒▒▒00        ]0▒▒▒▒00
                                  |\j▒▒0’          ‘0▒▒▒4M’
                                   |\#▒▒&▒▒gg▒▒0& |
                                " ▒▒00▒▒▒▒00▒▒’d
                               %   ¼▒    ˜ P▒¼▒▒|¼¼|
                                         ¸  ▒▒
                                    ▒9▒¸                  ▒
                                  M▒9▒¸▒▒   ]▒]   *▒j¸g
                                 l▒g▒▒]   @▒9     ▒▓?
                                   ˜ ▒0▒▒▓p ▒g▒▒▓
                                      @▒▒?▓▓?▓?▓?
                                          M0?▓  ▓?^
                             
▓█████▄ ▓█████ ▓█████▄   ██████ ▓█████  ▄████▄  
▒██▀ ██▌▓█   ▀ ▒██▀ ██▌▒██    ▒ ▓█   ▀ ▒██▀ ▀█  
░██   █▌▒███   ░██   █▌░ ▓██▄   ▒███   ▒▓█    ▄ 
░▓█▄   ▌▒▓█  ▄ ░▓█▄   ▌  ▒   ██▒▒▓█  ▄ ▒▓▓▄ ▄██▒
░▒████▓ ░▒████▒░▒████▓ ▒██████▒▒░▒████▒▒ ▓███▀ ░
 ▒▒▓  ▒ ░░ ▒░ ░ ▒▒▓  ▒ ▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ░▒ ▒  ░
 ░ ▒  ▒  ░ ░  ░ ░ ▒  ▒ ░ ░▒  ░ ░ ░ ░  ░  ░  ▒   
 ░ ░  ░    ░    ░ ░  ░ ░  ░  ░     ░   ░        
   ░       ░  ░   ░          ░     ░  ░░ ░      
 ░              ░                      ░        
```

> **Watch Dogs** style ASCII torus animation — written in pure C, zero dependencies.

---

## Overview

This project is a terminal animation inspired by the hacking aesthetic of the **Watch Dogs** video game series. It renders a spinning ASCII torus (donut shape) with a glitch effect — random character corruption and line shifts that simulate signal interference. Three variants are available: a simple looping animation, a system info display (neofetch-style), and a chaos mode where the torus teleports around the screen.

Everything is implemented in **pure C** using only POSIX/standard library calls. No external libraries, no build system — just `gcc` and a UTF-8 terminal.

---

## Files

| File | Description |
|---|---|
| `spin.c` | Base animation — torus spins with random glitch effects |
| `spinfetch.c` | Neofetch clone — spinning torus on the left, live system info on the right |
| `spin_chaos.c` | Chaos mode — torus teleports to random screen positions with a ghost trail effect |

---

## Build & Run

```bash
# Base torus
gcc -O2 -o spin spin.c
./spin

# Neofetch style (Linux only — reads /proc, /etc/os-release)
gcc -O2 -o spinfetch spinfetch.c
./spinfetch

# Chaos teleport mode
gcc -O2 -o spin_chaos spin_chaos.c
./spin_chaos
```

Stop any program with `Ctrl+C`. `spin_chaos` restores the cursor on exit.

---

## How It Works

### Torus frames (`spin.c`)
The animation consists of **10 hand-crafted ASCII frames** (A–J) stored as string literals. Each frame is a slightly rotated view of a torus shape built from characters like `$`, `S`, `I`, `?`, `` ` ``, `.`  
Frames are cycled at ~110ms intervals using `usleep()`.

### Glitch system
Two types of glitch are randomly triggered (~20% of frames):
- **Character glitch** — replaces random non-space characters with symbols like `#@%&!?/|`
- **Line shift glitch** — horizontally offsets random lines by 1–5 spaces

Glitch frames flash at 30ms with random ANSI colors (green / magenta / cyan / red).

### System info (`spinfetch.c`)
Collects live data using POSIX syscalls and `/proc`:
- **OS** — parsed from `/etc/os-release`
- **Kernel / Arch** — via `uname()`
- **CPU** — parsed from `/proc/cpuinfo`
- **RAM** — via `sysinfo()`
- **Uptime** — via `sysinfo()`
- **Shell** — from `$SHELL` environment variable
- **Packages** — counted via `dpkg-query` or `/usr/bin` fallback

The torus and info panel are rendered side by side using ANSI cursor positioning (`\033[row;colH`) without clearing the screen between frames.

### Chaos mode (`spin_chaos.c`)
- Reads terminal size via `ioctl(TIOCGWINSZ)`
- Torus stays at one random position for 2–5 full rotation cycles
- **Teleport effect** has 3 phases:
  1. Glitch-flash and disappear at old position
  2. 2–4 ghostly mid-points appear briefly across the screen
  3. Glitch-flash and stabilize at new position
- Handles `SIGWINCH` (terminal resize) and `SIGINT`/`SIGTERM` (clean exit)
- Hides the cursor during runtime (`\033[?25l`), restores on exit

---

## Requirements

- **OS**: Linux or macOS (WSL works on Windows)
- **Compiler**: GCC or Clang
- **Terminal**: UTF-8, ANSI color support (any modern terminal emulator)
- `spinfetch.c` requires Linux (`/proc` filesystem)

---

## Customization

| What | Where | Default |
|---|---|---|
| Frame speed | `usleep(110000)` | 110 ms |
| Glitch chance | `rand() % 100 < 20` | 20% |
| Glitch speed | `usleep(30000)` | 30 ms |
| Teleport stay | `rand() % 4 + 2` | 2–5 cycles |
| Ghost steps | `rand() % 3 + 2` | 2–4 points |
| Torus color | `GREEN` define | `\033[92m` |
