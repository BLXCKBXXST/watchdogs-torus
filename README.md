# watchdogs-torus

Watch Dogs style ASCII torus animation written in pure C.  
No dependencies — just `gcc` and a terminal.

```
      ,d$$$$$$SSIi.
    ,$$$SSSSS$$SSIi:.
   j$$$SSSSS$$$SIIi..
   S$$$$$SSS$$SSIIi-
  j?'``?S$SI7*'"#?IL:.
  ?:     $$S?    `7i'
  iL    j$?$L     I7
```

## Files

| File | Description |
|---|---|
| `spin.c` | Base animation — torus with glitch effect |
| `spinfetch.c` | Neofetch clone — torus + system info side by side |
| `spin_chaos.c` | Chaos mode — torus teleports to random screen positions |

## Build & Run

```bash
# Base torus
gcc -O2 -o spin spin.c && ./spin

# Neofetch style
gcc -O2 -o spinfetch spinfetch.c && ./spinfetch

# Chaos teleport mode
gcc -O2 -o spin_chaos spin_chaos.c && ./spin_chaos
```

Stop any of them with `Ctrl+C`.

## Requirements

- Linux / macOS (or WSL on Windows)
- GCC or Clang
- UTF-8 terminal with ANSI color support

## Controls

All programs run in an infinite loop. Press `Ctrl+C` to exit.  
`spin_chaos` restores the cursor and cleans up on exit.
