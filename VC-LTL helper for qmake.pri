Release {

# 作者：GPBeta（https://github.com/GPBeta），mingkuang（https://github.com/mingkuang-Chuyu）
# 修改日期：2021-08-20
#
#
# VC-LTL自动化加载配置，建议你将此文件单独复制到你的工程再使用，该文件能自动识别当前环境是否存在VC-LTL，并且自动应用。
#
# 使用方法：
#   1. 在自己的pro文件中添加“include("VC-LTL helper for qmake.pri")”。
#
# VC-LTL默认搜索顺序
#   1. VC-LTL helper for qmake.pri所在目录，即 $$PWD
#   2. VC-LTL helper for qmake.pri所在根目录下的VC-LTL目录，即 $$PWD/VC-LTL
#   3. VC-LTL helper for qmake.pri所在父目录，即$$PWD/..
#   4. 工程文件(*.pro)所在目录，即 $$_PRO_FILE_PWD_
#   5. 工程文件(*.pro)所在根目录下的VC-LTL目录，即，即 $$_PRO_FILE_PWD_/..
#   6. 注册表HKEY_CURRENT_USER\Code\VC-LTL@Root
#
# 把VC-LTL放在其中一个位置即可，VC-LTL就能被自动引用。
#
# 如果你对默认搜索顺序不满，你可以修改此文件。你也可以直接指定VC_LTL_Root环境变量更加任性的去加载VC-LTL。
#




#  ---------------------------------------------------------------------VC-LTL设置---------------------------------------------------------------------

# 控制最小兼容系统版本，目前可用版本为 6.0.6000.0（默认）    6.2.9200.0     10.0.10240.0    10.0.19041.0
# 注意：VC-LTL依赖YY-Thunks，否则可能无法兼容早期系统。如果需要支持Windows XP，该值必须为5.1.2600.0。
# WindowsTargetPlatformMinVersion = 10.0.10240.0

# VC-LTL使用的CRT模式，SupportLTL可能值为：
#   * false：禁用VC_LTL
#   * true：默认值，让VC-LTL自动适应。当最小兼容版本>=10.0时使用ucrt模式，其他系统使用msvcrt模式。
#   * msvcrt：使用msvcrt.dll作为CRT。注意：msvcrt模式可能不完全支持所有ucrt的新功能。比如setloacl不支持UTF8。
#   * ucrt：使用ucrtbase.dll作为CRT。注意：早期系统可能需要下载VC-LTL.Redist.Dlls.zip，感谢msvcr14x项目提供兼容XP系统的ucrtbase.dll。
# 如果需要兼容XP时也使用ucrt，请指定 SupportLTL=ucrt。
# SupportLTL=ucrt

# -----------------------------------------------------------------------------------------------------------------------------------------------------


!isEmpty(VC_LTL_Root) {
# pri文件根目录存在VC-LTL？
} else:exists($$PWD/_msvcrt.h) {
    VC_LTL_Root = $$PWD
# pri文件根目录下存在VC-LTL？
} else:exists($$PWD/VC-LTL/_msvcrt.h) {
    VC_LTL_Root = $$PWD/VC-LTL
# pri文件父目录存在VC-LTL？
} else:exists($$PWD/../_msvcrt.h) {
    VC_LTL_Root = $$PWD/..
# pri文件父目录存在VC-LTL？
} else:exists($$PWD/../VC-LTL/_msvcrt.h) {
    VC_LTL_Root = $$PWD/../VC-LTL
# pro文件根目录下存在VC-LTL？
} else:exists($$_PRO_FILE_PWD_/VC-LTL/_msvcrt.h) {
    VC_LTL_Root = $$_PRO_FILE_PWD_/VC-LTL
# pro文件父目录存在VC-LTL？
} else:exists($$_PRO_FILE_PWD_/../VC-LTL/_msvcrt.h) {
    VC_LTL_Root = $$_PRO_FILE_PWD_/../VC-LTL
# 读取注册表 HKCU\Code\VC-LTL@Root
} else:greaterThan(QT_VERSION, 5.12.0) {
    VC_LTL_Root = $$read_registry(HKCU, Code\\VC-LTL\\Root)
# QT版本过低，读注册表只能采用reg query
} else {
    VC_LTL_Root = $$system(reg query HKCU\\Code\\VC-LTL /v Root, blob)
	VC_LTL_Root ~= s/.*(\\w:.*)/\\1
	VC_LTL_Root = $$replace(VC_LTL_Root, \r, "")
    VC_LTL_Root = $$replace(VC_LTL_Root, \n, "")
}

!isEmpty(VC_LTL_Root): include($$VC_LTL_Root/config/config.pri)

}
