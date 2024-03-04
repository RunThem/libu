<p align="center">
<!--
  <a href="https://github.com/RunThem/libu/">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>
-->

  <h3 align="center">libu</h3>
  <p align="center">基于C23标准 & 逗号表达式的基础容器库!</p>
</p>
<br />

使用 `C23` 标准的特性与逗号表达式, 再使用宏进行封装, 实现的比较好用的泛型容器,
缺点那就还是老生常谈的问题了, 此处就不过多介绍了.
 
## 目录

- [上手指南](#上手指南)
  - [配置要求](#配置要求)
  - [安装步骤](#安装步骤)
  - [容器](#容器)
- [作者](#作者)
- [鸣谢](#鸣谢)

### 上手指南

#### 配置要求

1. [xmake](https://github.com/xmake-io/xmake) 项目构建工具
2. [c2x compiler](https://zh.cppreference.com/w/c/23) 仅支持 `Clang 16`, 以及 `Gcc 13` 以上的版本

#### **安装步骤**

```shell
git clone https://github.com/RunThem/libu.git
cd libu
xmake lsp && xmake
```

#### 容器

目前实现的容器:
- vec
- map
- avl
- lst

### 作者

<a href="mailto:iccy.fun@outlook.com">RunThem</a>

### 版权说明

该项目签署了 `MIT` 授权许可, 详情请参阅 [LICENSE](https://github.com/RunThem/libu/blob/master/LICENSE)

### 鸣谢

- [xmake](https://github.com/xmake-io/xmake)
- [koanlogic/libu](https://github.com/koanlogic/libu)
- [mimalloc](https://github.com/microsoft/mimalloc)
- [avlmini](https://github.com/skywind3000/avlmini)
