
 # Assembler Project 20465

## Overview
This project is a custom assembler written in C that translates assembly-like instructions into machine code. The assembler follows a **15-bit operation format**, designed to process a specific instruction set efficiently. The goal of this project is to deepen understanding of low-level programming, instruction encoding, and the inner workings of CPU architectures.

## Features
- **15-bit instruction format**
- Custom instruction set architecture (ISA)
- Lexical and syntax analysis
- Binary output generation
- Error handling and reporting

## Installation
### Prerequisites
Ensure you have the following installed on your system:
- A C compiler (GCC, Clang, or MSVC)
- Make (optional, for build automation)

### Build Instructions
Clone the repository and build the assembler:
```sh
git clone https://github.com/5wixy/CLab2024.git
cd assembler-project
make  # or use your preferred build method
```

## Usage
Compile your assembly file into machine code:
```sh
./assembler input.asm -o output.bin
```
Options:
- `-o <file>`: Specify output file
- `--debug`: Enable debugging information

## Example Assembly Code
```assembly
MAIN: add r3, LIST
LOOP: prn #48
macr m_macr
 cmp r3, #-6
 bne END
endmacr
 lea STR, r6
 inc r6
 mov *r6,K
 sub r1, r4
 m_macr
 dec K
 jmp LOOP
END: stop
STR: .string "abcd"
LIST: .data 6, -9
      .data -100
1 data. :K
```

## Roadmap
- [ ] Implement a full instruction set
- [ ] Add optimization passes
- [ ] Support for macros and symbolic labels
- [ ] Improve error messages and debugging tools

## Contributing
Contributions are welcome! Feel free to open an issue or submit a pull request.

## License
This project is licensed under the MIT License. See `LICENSE` for details.



