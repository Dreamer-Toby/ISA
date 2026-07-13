# ADR 0001: strict MVVM

Status: accepted

Use compile-time layers with `View -> ViewModel -> Model`. This makes gameplay independently testable and keeps SFML event/render details replaceable. App is a composition root, not an additional gameplay layer.

