k# attacks/event_engine.py
from dataclasses import dataclass
from typing import List, Optional

@dataclass
class Event:
    kind: str                 # "replay" | "fdia" | "spoofing" | "failsafe" ...
    sample_idx: int           # telemetri örnek index'i (i)
    logical_time: float       # telemetri zamanı (s) -> timestamp'ten veya i*dt
    note: str = ""            # kısa açıklama


class EventEngine:
    def __init__(self) -> None:
        self.events: List[Event] = []

    def add_event(
        self,
        kind: str,
        sample_idx: int,
        logical_time: float,
        note: str = "",
    ) -> None:
        self.events.append(Event(kind=kind, sample_idx=sample_idx, logical_time=logical_time, note=note))

