# ADR 0002: pure Model and render DTOs

Status: accepted

Model uses plain C++ math/state and never includes SFML. ViewModel publishes copied, read-only display records; View never receives Model objects. This costs a small snapshot copy but gives a simple, auditable boundary.

