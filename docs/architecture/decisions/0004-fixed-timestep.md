# ADR 0004: fixed timestep

Status: accepted

Use a 60 Hz accumulator and express all Model rates per second. Rendering is decoupled from simulation, and deterministic tests compare simulated results across render schedules.

