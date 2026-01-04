# 🚀 Assembly Language Assembler in C 🚀

A comprehensive assembler program for a specific assembly language, written in C. This project simulates the operation of common system programs and demonstrates a complete large-scale software development process.

---

## 📖 Table of Contents
- [Project Structure](#-project-structure)
- [Usage](#-usage)
- [Testing](#-testing)
- [Documentation](#-documentation)

---

## 🧩 Project Structure

The assembler is structured into three main stages:

1. **Pre-Processing**
2. **First Pass**
3. **Second Pass**

These stages form the core algorithm of the assembler.

### Pre-Processing
The pre-assembler reads the source file line by line, identifies macro definitions, stores them in a macro table, and replaces occurrences in the source file.

### First Pass
This pass identifies symbols (labels) and assigns numerical memory addresses to them.

### Second Pass
Generates the final machine code by replacing operation names with their binary equivalents and symbols with memory locations.


---

## 🔧 Usage

Run the assembler using:
```
./assembler file_name_1 ... file_name_N
```
Outputs the machine code corresponding to the provided assembly files.

---

## 🧪 Testing
* Multiple input files demonstrate the assembler’s handling of operations, data types, and errors.
* Test files are included in the Tests and Input_and_output_ex folders.

---

## 📚 Documentation
* Each function is documented with header comments explaining its purpose and usage.
* Important variables are explained.
* Detailed comments throughout the code make it easier to understand.

