# VC-LTL - An elegant way to compile lighter binaries.
![license](https://img.shields.io/github/license/Chuyu-Team/VC-LTL5)
![downloads](https://img.shields.io/github/downloads/Chuyu-Team/VC-LTL5/total)
![contributors](https://img.shields.io/github/contributors-anon/Chuyu-Team/VC-LTL5)
![release](https://img.shields.io/github/v/release/Chuyu-Team/VC-LTL5?include_prereleases)
![nuget](https://img.shields.io/nuget/vpre/VC-LTL)
[![Build&Test](https://github.com/Chuyu-Team/VC-LTL5/actions/workflows/Build&Test.yml/badge.svg)](https://github.com/Chuyu-Team/VC-LTL5/actions/workflows/Build&Test.yml)

> 这是VC-LTL 5.0版本，代码完全重构，仍然在试水中！如果是生产力环境中请使用[VC-LTL 4](https://github.com/Chuyu-Team/VC-LTL/releases/latest)。


* QQ群: [633710173](https://shang.qq.com/wpa/qunwpa?idkey=21d51d8ad1d77b99ea9544b399e080ec347ca6a1bc04267fb59cebf22644a42a)


## 设计目标
1. ABI与微软完全兼容，无需重新构建现有静态库即可使用。
2. 提供二个模式，一种是ucrt模式，直接使用微软的 ucrtbase，另外一种是VC-LTL 4.0的msvcrt。
3. 兼容 Windows XP、Windows Vista以及更高版本（如果需要兼容XP，则必须与YY-Thunks联动）。


## 其他
有更好的意见请跟我沟通。