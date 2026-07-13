# Pressure chase bar (design note)

Info only — not implemented in the product UI. Captured from the short-lived Gaggibre Test Pilot experiment; that code was removed.

## Idea

During **Manual Brew**, a horizontal bar in the center of the screen shows pressure as a live chase, not a post-shot graph.

- **Track** — dark bar spanning 0 → max bar (left to right)
- **Blue fill** — **actual** live pressure from the machine
- **White vertical line** — **target** pressure

The white line mirrors the **right-edge swipe** control: finger position on the pressure zone sets the target; the marker slides to the same value on the bar (top of swipe ≈ max bar / right end; bottom ≈ 0 / left end).

## Why it worked

You watch actual **chase** the setpoint while you adjust. Meeting fill and marker means you’re on target. No graph literacy required; useful *during* the pour rather than for comparing shots afterward.

## Status

Not shipped. Revisit only if we intentionally redesign Manual Brew around this pattern again.
