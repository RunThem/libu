<p align="center">
<!--
  <a href="https://github.com/RunThem/libu/">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>
-->

  <h3 align="center">libu</h3>
  <p align="center">A basic container library based on the `C23` standard!</p>
</p>
<br />

**libu** is a small, usability-oriented generic container library for C that provides **vectors**, **doubly linked lists**, **unordered maps**, and **avl tree**.

It requires C23, compiler support for `typeof` and `__VA_OPT__`.
 
## Project Requirements

1. [xmake](https://github.com/xmake-io/xmake) Build tool
2. [c2x compiler](https://zh.cppreference.com/w/c/23) equires Clang 16 or Gcc 13 or higher.

## Installation

```sh
git clone https://github.com/RunThem/libu.git && cd libu
xmake && xmake install --admin -v
```

## Examples

Check out the test code in the `tst` directory.

## MIT LICENSE

The project signed a [`MIT`](https://opensource.org/license/mit) license.
for details.

### Express Gratitude

- [xmake](https://github.com/xmake-io/xmake)
- [libu](https://github.com/koanlogic/libu)
- [mimalloc](https://github.com/microsoft/mimalloc)
- [avlmini](https://github.com/skywind3000/avlmini)
