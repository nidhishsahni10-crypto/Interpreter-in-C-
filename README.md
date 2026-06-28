# Monkey Interpreter in C++
### SOC 2024 — Final Project

A tree-walking interpreter for the Monkey programming language, built from scratch in C++ using Flex and Bison.

Monkey supports variables, functions (including closures and higher-order functions), conditionals, loops, arithmetic, boolean logic, and more. It's a surprisingly capable little language.

---

## Building

Just run:

```bash
make
```

This will generate a `runme` executable using Flex (for lexing) and Bison (for parsing).

## Running a program

```bash
./runme < your_file.txt
```

There are two sample input files included:
- `input_monkey.txt` — a comprehensive test covering most language features, with expected outputs in comments
- `input_2.txt` — focuses on closures and higher-order functions

You can also run the built-in test with:

```bash
make test
```

## Language features

- **Variables** — `let x = 5;`
- **Arithmetic** — `+`, `-`, `*`, `/`, `^` (power)
- **Booleans & logic** — `true`, `false`, `&&`, `||`, `!`
- **Comparisons** — `<`, `>`, `<=`, `>=`, `==`, `!=`
- **Ternary expressions** — `condition ? a : b`
- **Compound assignment** — `+=`, `-=`, `*=`, `/=`, `^=`
- **Blocks / scoping** — `{ ... }`
- **If/else** — standard, with dangling-else resolved innermost-first (like C++)
- **While and do-while loops**
- **Functions and closures** — `fn(x) { ... }`
- **Higher-order functions** — functions as arguments and return values
- **Print** — `print(expr);`
- **Comments** — `// single-line comments`

## Notes

- `^` is used for exponentiation (not bitwise XOR)
- Errors are printed to stderr and execution continues where possible
- Functions must explicitly `return` a value; a bare expression at the end of a function body also counts as the return value

## Cleanup

```bash
make clean
```
