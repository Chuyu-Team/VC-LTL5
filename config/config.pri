#
#  请不要直接使用此配置，应该使用VC-LTL helper for qmake.pri
#

# VC-LTL核心版本号，由于4.X并不兼容3.X。此值可以用于兼容性判断。
LTL_CoreVersion = 5

# 默认VC工具集版本
VisualStudioVersion = $$(VisualStudioVersion)

InternalSupportLTL = $$(SupportLTL)
isEmpty(SupportLTL) {
    InternalSupportLTL = true
}

equals(VisualStudioVersion, 14.0) {
} else:equals(VisualStudioVersion, 15.0) {
} else:equals(VisualStudioVersion, 16.0) {
} else:equals(VisualStudioVersion, 17.0) {
} else {
    error("VC-LTL: For Visual Studio 2015 , 2017, 2019 or 2022 only")
}


SUPPORTED_PLATFORM_LIST = x86 x64 arm arm64

LTLPlatform = $$(Platform)

isEmpty(LTLPlatform) {
    LTLPlatform = Win32
} else {
    !contains(SUPPORTED_PLATFORM_LIST, $$LTLPlatform) {
        error("VC-LTL: Unsupported platform: $$LTLPlatform")
    }
}

equals(LTLPlatform, x86) {
    LTLPlatform = Win32
} else:equals(LTLPlatform, arm64) {
    InternalSupportLTL = ucrt
}

# 环境变量选项
SupportWinXP_t = $$SupportWinXP
isEmpty(SupportWinXP_t){
    SupportWinXP_t = $$(SupportWinXP)
}

#匹配最佳 TargetPlatform
isEmpty(WindowsTargetPlatformMinVersion) {
    equals(LTLPlatform, arm64) {
        InternalLTLCRTVersion = 10.0.10240.0
    } else:equals(LTLPlatform, arm) {
        InternalLTLCRTVersion = 6.2.9200.0
    } else:!equals(SupportWinXP_t, true) {
        InternalLTLCRTVersion = 6.0.6000.0
    } else:equals(LTLPlatform, x64) {
        InternalLTLCRTVersion = 5.2.3790.0
    } else {
        InternalLTLCRTVersion = 5.1.2600.0
    }
} else {
    LTLWindowsTargetPlatformMinVersionTmp = $$(WindowsTargetPlatformMinVersion)
    LTLWindowsTargetPlatformMinVersionTmp = $$split(LTLWindowsTargetPlatformMinVersionTmp, ".")
    LTLWindowsTargetPlatformMinVersionBuild = $$member(LTLWindowsTargetPlatformMinVersionTmp, 2)

    !lessThan(LTLWindowsTargetPlatformMinVersionBuild, 19041) {
        InternalLTLCRTVersion = 10.0.19041.0
    } else:!lessThan(LTLWindowsTargetPlatformMinVersionBuild, 10240) {
        InternalLTLCRTVersion = 10.0.10240.0
    } else:equals(LTLPlatform, arm64) {
        InternalLTLCRTVersion = 10.0.10240.0
    } else:!lessThan(LTLWindowsTargetPlatformMinVersionBuild, 9200) {
        InternalLTLCRTVersion = 6.2.9200.0
    } else:equals(LTLPlatform, arm) {
        InternalLTLCRTVersion = 6.2.9200.0
    } else:!equals(SupportWinXP_t, true) {
        InternalLTLCRTVersion = 6.0.6000.0
    } else:equals(LTLPlatform, x64) {
        InternalLTLCRTVersion = 5.2.3790.0
    } else {
        InternalLTLCRTVersion = 5.1.2600.0
    }
}

equals(InternalSupportLTL, true) {
    LTLWindowsTargetPlatformMinVersionTmp = $$(WindowsTargetPlatformMinVersion)
    LTLWindowsTargetPlatformMinVersionTmp = $$split(LTLWindowsTargetPlatformMinVersionTmp, ".")
    LTLWindowsTargetPlatformMinVersionBuild = $$member(LTLWindowsTargetPlatformMinVersionTmp, 2)

    lessThan(LTLWindowsTargetPlatformMinVersionBuild, 10240) {
        InternalSupportLTL = msvcrt
    } else {
        InternalSupportLTL = ucrt
    }
} else equals(InternalSupportLTL, msvcrt) {
    LTLWindowsTargetPlatformMinVersionTmp = $$(WindowsTargetPlatformMinVersion)
    LTLWindowsTargetPlatformMinVersionTmp = $$split(LTLWindowsTargetPlatformMinVersionTmp, ".")
    LTLWindowsTargetPlatformMinVersionBuild = $$member(LTLWindowsTargetPlatformMinVersionTmp, 2)

    !lessThan(LTLWindowsTargetPlatformMinVersionBuild, 10240) {
        InternalLTLCRTVersion = 6.2.9200.0
    }
} else equals(InternalSupportLTL, ucrt) {
    LTLWindowsTargetPlatformMinVersionTmp = $$(WindowsTargetPlatformMinVersion)
    LTLWindowsTargetPlatformMinVersionTmp = $$split(LTLWindowsTargetPlatformMinVersionTmp, ".")
    LTLWindowsTargetPlatformMinVersionBuild = $$member(LTLWindowsTargetPlatformMinVersionTmp, 2)
    
    lessThan(LTLWindowsTargetPlatformMinVersionBuild, 10240) {
        InternalLTLCRTVersion = 10.0.10240.0
    }
}

!exists($$VC_LTL_Root/TargetPlatform/$$InternalLTLCRTVersion/lib/$$LTLPlatform) {
    error("VC-LTL: Cannot find lib files, please download latest pre-build packages from https://github.com/Chuyu-Team/VC-LTL5/releases/latest")
}





# 搜索VC工具集版本
VCToolsVersion = $$(VCToolsVersion)



# 打印信息
VC_LTL_Info = \
$$escape_expand(\\n)####################################################################### \
$$escape_expand(\\n)#                                                                     # \
$$escape_expand(\\n)# 8b           d8  ,ad8888ba,         88     888888888888 88          # \
$$escape_expand(\\n)# `8b         d8' d8"'    `"8b        88          88      88          # \
$$escape_expand(\\n)#  `8b       d8' d8'                  88          88      88          # \
$$escape_expand(\\n)#   `8b     d8'  88                   88          88      88          # \
$$escape_expand(\\n)#    `8b   d8'   88          aaaaaaaa 88          88      88          # \
$$escape_expand(\\n)#     `8b d8'    Y8,         """""""" 88          88      88          # \
$$escape_expand(\\n)#      `888'      Y8a.    .a8P        88          88      88          # \
$$escape_expand(\\n)#       `8'        `"Y8888Y"'         88888888888 88      88888888888 # \
$$escape_expand(\\n)#                                                                     # \
$$escape_expand(\\n)####################################################################### \
$$escape_expand(\\n)VC-LTL Path : $$VC_LTL_Root \
$$escape_expand(\\n)VC Tools Version : $$VCToolsVersion \
$$escape_expand(\\n)$$InternalSupportLTL Mode : $$InternalLTLCRTVersion \
$$escape_expand(\\n)Platform : $$LTLPlatform \
$$escape_expand(\\n)#######################################################################

message($$VC_LTL_Info)


# 修改头文件及库搜索路径
QMAKE_INCDIR += \
    $$VC_LTL_Root/TargetPlatform/header \
    $$VC_LTL_Root/TargetPlatform/$$InternalLTLCRTVersion/header

QMAKE_LIBS += \
    -L$$VC_LTL_Root/TargetPlatform/$$InternalLTLCRTVersion/lib/$$LTLPlatform
