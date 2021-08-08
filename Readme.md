# VC-LTL - An elegant way to compile lighter binaries.

> 这是VC-LTL 5.0版本，代码完全重构，仍然在试水中！如果是生产力环境中请使用[VC-LTL 4](https://github.com/Chuyu-Team/VC-LTL/releases/latest)。


* QQ群: [633710173](https://shang.qq.com/wpa/qunwpa?idkey=21d51d8ad1d77b99ea9544b399e080ec347ca6a1bc04267fb59cebf22644a42a)


## 设计目标
1. ABI与微软完全兼容，无需重新构建现有静态库即可使用。
2. 计划提供二个模式，一种是ucrt模式『优先级更高』，直接使用微软的 ucrtbase，另外一种是msvcrt。
3. 兼容 Windows Vista以及更高版本。疑问，是否需要支持Windows XP？


## 其他
有更好的意见请跟我沟通。