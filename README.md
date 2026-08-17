# CAST-128 demonstration

A small Qt Widgets application that demonstrates one-block CAST-128 (CAST5)
encryption and decryption with a 128-bit key. The cipher implementation is kept
separate from the GUI and is checked against the 128-bit known-answer vector in
[RFC 2144, Appendix B.1](https://www.rfc-editor.org/rfc/rfc2144#appendix-B.1).

> **Educational use only.** The GUI applies CAST-128 directly in ECB mode with
> zero padding and no authentication. It therefore reveals repeated blocks,
> cannot reliably preserve trailing zero bytes, and cannot detect tampering.
> CAST-128 also has a 64-bit block size and is unsuitable for modern application
> encryption. Use a maintained authenticated-encryption library for real data.

## Build and run

Install a C++11 compiler plus Qt 5 development tools, then run:

```sh
cd CAST128_Example
qmake CAST128_Example.pro
make
./CAST128_Example
```

The key field accepts exactly 32 hexadecimal characters. Plaintext is encoded
as UTF-8; ciphertext is displayed as hexadecimal.

## Test

The core test has no Qt dependency:

```sh
make test
```

This project is available under the [MIT License](LICENSE).
