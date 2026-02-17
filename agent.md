# Claude Instructions for WD2123 Project

## Project Overview
This is a WD2123 Serial Port replacement that uses an STM32 compatible module to emulate the chip.

## Critical Rules

### Code Style
- NO emojis in any documentation or comments
- Clean, professional code and documentation only
- Minimal comments - code should be self-explanatory
- Use concise variable names where appropriate

### Hardware Constraints
- Target: STM32
- Arduino IDE compatibility required
- Must compile with Arduino 1.8.13+

**Reserved/Avoid**
 USB D-  PA11  USB (do not use as GPIO)
 USB D+  PA12  USB (do not use as GPIO)
 SWDIO  PA13  SWD programming
 SWDCLK  PA14  SWD programming
 LSE_IN  PC14  Oscillator (causes crashes)
 LSE_OUT  PC15  Oscillator (causes crashes)

## When Starting New Conversation
1. Always check out a fresh new copy from the repo
2. Don't assume old artifacts are current
3. Always work from latest code user provides
4. Verify pin assignments haven't drifted
5. Check that diskImages[] is still declared

## Communication Style
- Be direct and concise
- Don't apologize excessively
- When wrong, acknowledge and fix immediately
- Don't use phrases like "let me check" - just check
- Focus on solutions, not explanations of why things broke

Behavioral guidelines to reduce common LLM coding mistakes. Merge with project-specific instructions as needed.

**Tradeoff:** These guidelines bias toward caution over speed. For trivial tasks, use judgment.

## 1. Think Before Coding

**Don't assume. Don't hide confusion. Surface tradeoffs.**

Before implementing:
- State your assumptions explicitly. If uncertain, ask.
- If multiple interpretations exist, present them - don't pick silently.
- If a simpler approach exists, say so. Push back when warranted.
- If something is unclear, stop. Name what's confusing. Ask.

## 2. Simplicity First

**Minimum code that solves the problem. Nothing speculative.**

- No features beyond what was asked.
- No abstractions for single-use code.
- No "flexibility" or "configurability" that wasn't requested.
- No error handling for impossible scenarios.
- If you write 200 lines and it could be 50, rewrite it.

Ask yourself: "Would a senior engineer say this is overcomplicated?" If yes, simplify.

## 3. Surgical Changes

**Touch only what you must. Clean up only your own mess.**

When editing existing code:
- Don't "improve" adjacent code, comments, or formatting.
- Don't refactor things that aren't broken.
- Match existing style, even if you'd do it differently.
- If you notice unrelated dead code, mention it - don't delete it.

When your changes create orphans:
- Remove imports/variables/functions that YOUR changes made unused.
- Don't remove pre-existing dead code unless asked.

The test: Every changed line should trace directly to the user's request.

## 4. Goal-Driven Execution

**Define success criteria. Loop until verified.**

Transform tasks into verifiable goals:
- "Add validation" → "Write tests for invalid inputs, then make them pass"
- "Fix the bug" → "Write a test that reproduces it, then make it pass"
- "Refactor X" → "Ensure tests pass before and after"

For multi-step tasks, state a brief plan:
```
1. [Step] → verify: [check]
2. [Step] → verify: [check]
3. [Step] → verify: [check]
```

Strong success criteria let you loop independently. Weak criteria ("make it work") require constant clarification.

---

**These guidelines are working if:** fewer unnecessary changes in diffs, fewer rewrites due to overcomplication, and clarifying questions come before implementation rather than after mistakes.


