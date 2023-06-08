<p align="center">
<!--
  <a href="https://github.com/RunThem/libu/">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>
-->

  <h3 align="center">libu</h3>
  <p align="center">快速开始你的项目!</p>
</p>
<br />

 简单的C项目模板
 
## 目录

- [上手指南](#上手指南)
  - [开发前的配置要求](#配置要求)
  - [安装步骤](#安装步骤)
- [作者](#作者)
- [鸣谢](#鸣谢)

### 上手指南

将本仓库克隆至本地并改名.

###### 配置要求

1. [xmake](https://github.com/xmake-io/xmake) 项目构建工具
2. [c2x compiler](https://zh.cppreference.com/w/c/23) 支持 `c2x` 标准的C编译器

###### **安装步骤**

```shell
git clone https://github.com/RunThem/libu.git ${your_repository}
cd ${your_repository}

bash -c 'sed -i "s/libu/$(basename `pwd`)/g" README.md xmake.lua && git update-ref -d HEAD && git add . && git commit -m "first commit" && git remote remove origin'

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
