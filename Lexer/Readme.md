# 🔠 Lexical Analyzer – Syntax-e-Urdu

This module implements the **Lexical Analyzer** for Urdu-style programming code. It scans source code character-by-character using a deterministic finite automaton (DFA) driven by a transition table, and generates a valid stream of tokens as the first phase of compilation.

## 🧩 Purpose

The lexer reads raw Urdu-style source code and breaks it into meaningful tokens like `Agar`, `Adadi`, identifiers, literals, and operators. It also handles formatting, comments, and whitespace intelligently — preparing clean output for the parser.

## 🔍 Core Features

- Transition table–based DFA implementation
- 25-character dual-pointer **input buffer**
- Custom handling of:
  - Urdu-style keywords (`Agar`, `Wagarna`, `Adadi`, etc.)
  - Identifiers and numeric literals
  - Operators and punctuation
  - **Single-line and multi-line comments**
  - Consecutive whitespace (collapsed into a `"Meow"` token)

## 🧪 Output Files

After analyzing the input, the lexer generates:

- `symbol_table.txt` – valid identifiers and keywords with indexes
- `literal_table.txt` – numeric constants with indexes
- `error_log.txt` – invalid, unrecognized, or malformed tokens

## 🧠 How It Works

1. Loads `transition_table.txt` and `advance_table.txt`
2. Reads source code using a buffer and two pointers
3. Transitions states based on input character and current state
4. On reaching a final state:
   - Generates a token
   - Inserts it into the appropriate table
   - Logs any errors if applicable
5. Repeats until EOF

### 🔧 Transition Table Workflow

The lexer is driven by a **DFA (Deterministic Finite Automaton)** defined externally in a file named `transition_table.txt`. This table defines how the lexer transitions between states based on character classes (e.g., letters, digits, operators).

#### 🛠️ Creation of the Transition Table
- The transition table is defined as a plain text file, where:
  - **Rows** represent current states
  - **Columns** represent input character classes
  - **Cell values** represent the next state to transition to
- It allows adding new token rules (e.g., Urdu keywords, operators) **without changing C++ code** — just by updating the table.

#### 📥 Importing the Transition Table
- At runtime, `main.cpp` reads `transition_table.txt` and loads it into a **2D dynamic array** (e.g., `int** transitionTable`).
- It also loads a secondary `advance_table.txt` that tells the lexer **whether to move the pointer forward** on each transition.

#### ⚙️ Dynamic Lexer Behavior
- When the lexer runs, it:
  1. Identifies the **input class** of the current character (e.g., digit, letter, symbol).
  2. Uses the current `state` and character class to **index the transition table** and find the next state.
  3. Checks `advanceTable[state][charClass]` to decide if it should advance the input buffer pointer.
  4. When it hits an **accepting/final state**, it:
     - Captures the token
     - Adds it to the appropriate table (symbol, literal)
     - Resets the state and continues scanning

This table-driven design makes the lexer **fully modular**, easy to update, and language-independent — perfect for a compiler like `Syntax-e-Urdu`.


## 📁 Files in This Module

```text
lexer/
├── main.cpp              # Lexical analyzer source
├── transition_table.txt  # DFA transitions for each state and input class
├── advance_table.txt     # Controls pointer movement per transition
├── input.txt             # Urdu-style source code to tokenize
├── symbol_table.txt      # Output: identifiers and keywords
├── literal_table.txt     # Output: numeric constants
├── error_log.txt         # Output: lexical errors
