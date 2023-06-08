<p align="center">
<!--
  <a href="https://github.com/RunThem/libu/">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>
-->

  <h3 align="center">libu</h3>
  <p align="center">基于C23标准&语言表达式扩展的基础库!</p>
</p>
<br />

`C` 在 `C23` 标准之前, 缺少抽象能力, 很难写出好用的容器, 但在C23标准后就不一样了, 它提供了 `typeof` 和 `auto`, 再加上`语句表达式扩展`, 就可以利用宏来实现好用的容器了. 当然, 缺点也是有的, 就是代码会急速的膨胀起来.
 
## 目录

- [上手指南](#上手指南)
  - [开发前的配置要求](#配置要求)
  - [安装步骤](#安装步骤)
- [作者](#作者)
- [鸣谢](#鸣谢)

### 上手指南

###### 配置要求

1. [xmake](https://github.com/xmake-io/xmake) 项目构建工具
2. [c2x compiler](https://zh.cppreference.com/w/c/23) 支持 `c2x` 标准的C编译器(默认为Clang 16.0)

###### **安装步骤**

```shell
git clone https://github.com/RunThem/libu.git
cd libu

xmake f -m debug --toolchain=clang && xmake

xmake lsp
```

### 作者

<a href="mailto:iccy.fun@outlook.com">RunThem</a>

### 版权说明

该项目签署了 `MIT` 授权许可, 详情请参阅 [LICENSE](https://github.com/RunThem/libu/blob/master/LICENSE)

### 鸣谢

- [xmake](https://github.com/xmake-io/xmake)
- [cc](https://github.com/JacksonAllan/CC)
- [sds](https://github.com/antirez/sds)
- [mimalloc](https://github.com/microsoft/mimalloc)
- [dbg-macro](https://github.com/eerimoq/dbg-macro)
