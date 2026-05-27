# Crash Reports

This folder is the `crash-report` agent's **deduplication memory**. One markdown
file per distinct root cause, plus a timestamped `archive/` of the original
`crash.dmp` / `crash.txt` pairs that produced each report.

## Workflow contract (see `.github/agents/crash-report.agent.md`)

- **Step 0** of every crash analysis greps this folder for prior matches on
  exception code + symbolicated `file:line` (and exception address as a
  tiebreaker). If a fix is already shipped, the agent stops *without*
  re-editing source.
- **Step 9** of every successful fix writes a new report here (or appends a
  `Recurrence log` entry to an existing one) and moves the consumed dump into
  `archive/`.

## File naming

`YYYY-MM-DD_<exception-code-without-0x>_<short-symbol>.md`

Examples:

- `2026-05-26_C0000005_loop_client-sobj_bufsize.md`
- `2026-06-14_C00000FD_stack_overflow-recursion.md`

If a recurrence is the *same* root cause, append to the existing file rather
than creating a new one. If it's a *different* bug at the same symbol, create
a new file and cross-link via `Duplicate-of` / `Related` headers.

## Status field

Each report carries a `Status:` line — `Fixed`, `Mitigated`, `Open`, or
`Duplicate-of(<file>)`. Step 0 trusts this field to decide whether to
short-circuit or continue analysis.

