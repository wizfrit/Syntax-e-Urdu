# 🧠 Semantic Analyzer – Syntax-e-Urdu

This module implements the **Semantic Analysis** phase of the `Syntax-e-Urdu` compiler frontend. It performs **context-sensitive checks** based on a limited, grammar-driven rule set to validate the correctness of Urdu-style code beyond syntax.

## 🎯 Purpose

While the parser ensures the structure is valid, this module checks whether the code makes **logical sense** — such as:
- Using declared variables
- Matching types during assignments
- Ensuring control conditions are semantically valid

> ⚠️ Note: This semantic analyzer does **not define its own CFG** from scratch. It extends and augments the existing CFG with semantic rules, applied only to a **limited set of constructs**.

## 📌 Focused Scope

This module handles only:
- Variable declarations (`Adadi`, `Kirdar`, etc.)
- Assignments
- `Agar` / `Wagarna` conditions
- Basic expressions and type usage

It does **not** support:
- Full type inference
- Nested scoping
- Function semantics

## ⚙️ Features

- Verifies **identifiers are declared** before use
- Checks **type compatibility** in assignments and conditionals
- Confirms **valid use of literals and operators**
- Logs semantic errors (undeclared, type mismatch, misuse)

## 🧠 How It Works

1. Loads `tokens.txt` (from lexer) and `parse_log.txt` (from parser)
2. Reads semantic rules built **on top of the existing CFG**
3. Traverses the parse structure for specific patterns
4. Applies semantic validation rules:
   - Symbol existence
   - Type correctness
   - Logical validity
5. Writes all findings to `semantic_log.txt`

## 📁 Files in This Module

```text
semantic/
├── semantic.cpp        # Semantic analysis implementation
├── tokens.txt          # Input: token stream from lexer
├── parse_log.txt       # Input: parse steps from parser
├── semantic_log.txt    # Output: semantic results
```

