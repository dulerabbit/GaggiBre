# GaggiBre fork workflow (your GitHub only)

This repo is a **real fork** of [jniebuhr/gaggimate](https://github.com/jniebuhr/gaggimate), branded and extended as GaggiBre. Day-to-day work stays on **your** GitHub. You do not need to open PRs against upstream to ship flashes for your users.

## Remotes

```bash
git remote -v
# origin    https://github.com/dulerabbit/GaggiBre.git          # your fork (push here)
# upstream  https://github.com/jniebuhr/gaggimate.git           # read/rebase only
# legacy    https://github.com/dulerabbit/GaggiBre-legacy.git   # old SquareLine archive
```

## Where to push

| Goal | Where |
|------|--------|
| Feature / polish | Branch on `origin`, then merge into **your** `master` |
| Public web flasher | Tag on `origin` → CI → `gh-pages/stable/` |
| Nightly flasher | Push to **your** `master` → CI → `gh-pages/nightly/` |

Flash URL: https://dulerabbit.github.io/GaggiBre/stable/

## Do not bother upstream by accident

- GitHub’s **Compare & pull request** banner on a fork often defaults the **base** to `jniebuhr/gaggimate`. Always check the base repo before creating a PR.
- To merge into GaggiBre only, set base to `dulerabbit/GaggiBre` (your `master`).
- `git fetch upstream` is fine. Do **not** force-push, open random issues, or spam PRs on upstream.

## Matched flash rule

Controller and display must come from the **same** GaggiBre release (same EEZ/NanoPb generation). Do not mix this stack with old SquareLine GaggiBre binaries.

## Contributing back to Gaggimate (optional, later)

Only when you intentionally want to give a fix upstream:

1. Sign the CLA via [@jniebuhr](https://github.com/jniebuhr) (mdwasp) — see [CONTRIBUTING.md](../CONTRIBUTING.md).
2. Branch from current `upstream/master`.
3. Open a **small, focused** PR to `jniebuhr/gaggimate` (one concern).
4. Do not bundle Waveshare branding / Manual Brew / flasher changes unless upstream asked for them.

Until then: polish and release only on `dulerabbit/GaggiBre`.
