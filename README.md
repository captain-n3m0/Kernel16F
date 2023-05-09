# Kernel16F

Kernel16F is a simple operating system kernel written in x86 assembly and C. It is designed to run on 16-bit x86 processors and provides basic functionality such as keyboard input, screen output, and memory management.
## Contributors

[@captain-n3m0](https://github.com/captain-n3m0)
[@Abhrankan-Chakrabarti](https://github.com/Abhrankan-Chakrabarti)

## Getting Started

To build and run Kernel16F, you will need a 16-bit x86 processor emulator such as QEMU or Bochs. You will also need a cross-compiler that can compile code for the 16-bit x86 architecture.

1. Clone the repository:
```bash
git clone https://github.com/captain-n3m0/Kernel16F.git
```

2. Install the cross-compiler:
```bash
sudo apt-get install binutils gcc make nasm
```

3. Build the kernel:
```bash
cd Kernel16F
make
```

4. Run the kernel:
```bash
qemu-system-i386 -kernel kernel16f.bin
```

## Features

Kernel16F provides the following features:

- Keyboard input: The kernel can read input from the keyboard and display it on the screen.
- Screen output: The kernel can display text on the screen using VGA text mode.
- Memory management: The kernel can allocate and free memory using a simple heap implementation.
- Interrupt handling: The kernel can handle interrupts such as keyboard interrupts and timer interrupts.
- Basic shell: The kernel provides a simple shell that allows the user to execute basic commands.

## Contributing

Contributions to Kernel16F are welcome! If you find a bug or have an idea for a new feature, please open an issue or submit a pull request.

Before contributing, please read the [code of conduct](CODE_OF_CONDUCT.md) and [contributing guidelines](CONTRIBUTING.md).

## License

Kernel16F is licensed under the [MIT License](LICENSE).
