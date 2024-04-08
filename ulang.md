# ulang

现在是幻想时间, 幻想将来我会实现的一门编译语言.

该语言是一门静态类型, 编译型语言, 将代码翻译成 `C` 代码. 主要将想弥补 `C` 语言的缺点.

## 模块
```sh
ulang/
├── bin
└── src
    └── std
        ├── fmt
        │   └── fmt.u
        └── net
            ├── http
            │   └── http.u
            ├── ip.u
            └── ipv6.u
```

```rust
pkg main

use std::fmt;
use std::net;
use std::net::http;

fn main() {
  let ip: net::ip = net::ip::new("192.168.1.1");

  fmt::println("{}", ip);
}
```

## 基本数据类型

```rust
pkg main

use std::fmt;

fn main() {
  let x: bool = true;
  let y: bool = false;

  fmt::println("{}", x);
  fmt::println("{}", y);
}
```
