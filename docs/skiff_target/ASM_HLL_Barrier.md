
- The target binary will always a `uint64` constant called `_mc` to act as a memory counter for the memory-slot based program stack.
- The target binary will also have a `uint64_t` constant called `_fc` to act as the frame counter for the memory-slot based program stack

- The target binary will then allocate a fixed sized "stack" in memory slot 1. This is where the local variables location data will be stored (where they exist in their own memory slots) and be generally treated as the program stack as conventional machines are used to.

- The Skiff VM stack object and operations will be reserved for temporary variable storage and calculations

- Parameters of functions will each be contained in their own memory slots, with the addresses stored in the skiff stack for 

```

 _mc - Memory Counter (highest allocated point)
 _fc - Frame counter (entry to current operating function




              ┌───────────────────────────┬───────────────────┬───────────────┐
              │                           │                   │               │
              ├───┬───┬───┬────┬───┬──────┼──────────┬────┬───┼────┬────┬─────┼───┬──┐
Memory Slot 1 │   │   │   │    │   │      │          │    │   │    │    │     │   │  │
              │   │   │   │    │   │      │          │    │   │    │    │     │   │  │
              ├───┴───┴───┴────┴───┴──────┼──────────┴────┴───┼────┴────┴──▲──┼───┴──┘
              │                           │                   │            │  │
              │                           │                   │            │  │
              │                           │                   │            │  │
              │                           │                   │            │  │
              │                           │                   │            │  │
              │    frame 0                │  frame 1          │  frame 2   │  │
              └───────────────────────────┴───────────────────▲────────────┼──┘
                                                              │            │       ┌─────┐
                                                              │            └───────┤ _mc │
                                                              │                    └─────┘
                                                              │
                                                              │                    ┌─────┐
                                                              └────────────────────┤ _fc │
                                                                                   └─────┘

```