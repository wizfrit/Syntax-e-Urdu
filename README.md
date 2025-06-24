# 🌐 Syntax-e-Urdu – Compiler Frontend for Urdu-Inspired Language

**Syntax-e-Urdu** is a compiler frontend written in **C++** for a custom Urdu-style programming language. It processes constructs like `Agar`, `Wagarna`, `Adadi`, and more — covering the full frontend pipeline: **lexical analysis**, **syntax parsing**, and **semantic analysis**.

---

## 🚀 Summary

> A C++-based compiler frontend that recognizes Urdu-inspired syntax, tokenizes it, parses it using context-free grammar, and performs semantic validation — simulating how modern compilers process high-level languages.

---

## 📦 Features

- ✅ **Lexical Analyzer** using transition and advance tables
- ✅ **Token generation** for Urdu-like syntax (e.g., `Agar`, `Wagarna`, `Adadi`)
- ✅ **Parsing** using a custom context-free grammar (CFG)
- ✅ **Semantic Analyzer** for type checks, undeclared variables, and control flow validation
- ✅ Generates:
  - Symbol Table
  - Literal Table
  - Error Log
  - Parse Tree or Production Logs
- ✅ Full support for comments and whitespace handling (including `"Meow"` category)
- ✅ Built entirely in C++ with dynamic memory handling

---

## 🧠 How It Works

### 1️⃣ Lexical Analysis
- Reads Urdu-style code using a dual-pointer 25-character buffer.
- Recognizes tokens via a **Transition Table** and **Advance Table**.
- Outputs: `symbol_table.txt`, `literal_table.txt`, `error_log.txt`

### 2️⃣ Syntax Parsing
- Parses token stream using a **custom CFG**
- Validates structure for conditionals, loops, expressions
- Outputs: `parse_log.txt`, optional parse tree

### 3️⃣ Semantic Analysis
- Validates type compatibility, declaration usage, and control flow
- Outputs: `semantic_log.txt` with success or errors

---

## Notes

- Reserved Urdu-like keywords are pre-defined
- All stages are independent C++ modules
- The "Meow" token represents collapsed whitespace sequences
- Designed as a learning project for compiler construction in native languages

## Author

- Developed as a semester project for Compiler Construction
- Muhammad Waiz Shabbir

