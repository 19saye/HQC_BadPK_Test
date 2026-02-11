# attacks/secure_layer.py
from __future__ import annotations
from typing import Optional
from .event_engine import EventEngine


class SecureLayer:
    """
    Basit demo güvenlik katmanı:
    - PQC key mismatch => failsafe event
    - Signal discrepancy (incoming != original) => attack event
    """

    def __init__(self, engine: EventEngine) -> None:
        self.engine = engine

    def process_packet(
        self,
        *,
        attack_kind: str,
        sample_idx: int,
        logical_time: float,
        original_value: float,
        incoming_value: float,
        expected_key: str,
        received_key: str,
        note: str = "",
    ) -> None:
        # 1) PQC key kontrolü (demo)
        if received_key != expected_key:
            self.engine.add_event(
                kind="failsafe",
                sample_idx=sample_idx,
                logical_time=logical_time,
                note=f"key_mismatch | {attack_kind} | {note}".strip(),
            )
            return

        # 2) Veri farkı varsa saldırı olayı üret (demo)
        if incoming_value != original_value:
            self.engine.add_event(
                kind=attack_kind,
                sample_idx=sample_idx,
                logical_time=logical_time,
                note=note,
            )

