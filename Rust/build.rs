/*
 * PROJECT:   Rust language support for VC-LTL
 * FILE:      build.rs
 * PURPOSE:   Implementation for build script
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

fn main() {
    if !std::env::var("CARGO_CFG_WINDOWS").is_ok() {
        println!(
            "cargo:warning=VC-LTL only supports Windows host. So the VC-LTL stage is skipped."
        );
        return;
    }

    let target_os =
        std::env::var("CARGO_CFG_TARGET_OS").expect("CARGO_CFG_TARGET_OS should be set by cargo.");
    let target_env = std::env::var("CARGO_CFG_TARGET_ENV")
        .expect("CARGO_CFG_TARGET_ENV should be set by cargo.");

    if !target_os.contains("windows") {
        println!(
            "cargo:warning=VC-LTL only supports Windows target. So the VC-LTL stage is skipped."
        );
        return;
    }

    if !target_env.contains("msvc") {
        println!(
            "cargo:warning=VC-LTL only supports MSVC toolchain. So the VC-LTL stage is skipped."
        );
        return;
    }

    let vcltl_root = std::env::var("VCLTL_ROOT")
        .or_else(|_| std::env::var("CARGO_MANIFEST_DIR"))
        .expect("VCLTL_ROOT or CARGO_MANIFEST_DIR should be set.");
    let target_arch = std::env::var("CARGO_CFG_TARGET_ARCH")
        .expect("CARGO_CFG_TARGET_ARCH should be set by cargo.");

    let version = match std::env::var("VCLTL_VERSION") {
        Ok(version) => version.parse::<usize>().unwrap(),
        _ if cfg!(feature = "win7") => 9200,
        _ if cfg!(feature = "vista") => 6000,
        _ if cfg!(feature = "xp") => 2600,
        _ => {
            println!("cargo:warning=VC-LTL needs at least one platform to choose from.");
            return;
        }
    };

    let (target_platform, version) = match (
        target_arch.contains("x86_64"),
        target_arch.contains("x86"),
        target_arch.contains("arm"),
        target_arch.contains("aarch64"),
    ) {
        (true, _, _, _) => ("x64", version.max(3790)),
        (_, true, _, _) => ("Win32", version.max(2600)),
        (_, _, true, _) => ("ARM", version.max(9200)),
        (_, _, _, true) => ("ARM64", version.max(10240)),
        _ => {
            println!("cargo:warning=VC-LTL does not support {} platform. So the VC-LTL stage is skipped.", target_arch);
            return;
        }
    };

    let version_string = if version >= 19041 {
        "10.0.19041.0"
    } else if version >= 10240 {
        "10.0.10240.0"
    } else if version >= 9200 && cfg!(feature = "win7") {
        "6.2.9200.0"
    } else if version >= 6000 && cfg!(feature = "vista") {
        "6.0.6000.0"
    } else if version >= 3790 && cfg!(feature = "xp") {
        "5.2.3790.0"
    } else if cfg!(feature = "xp") {
        "5.1.2600.0"
    } else {
        unreachable!()
    };

    let library_folder = &format!(
        "{}/TargetPlatform/{}/lib/{}",
        vcltl_root, version_string, target_platform
    );

    if !std::path::Path::new(library_folder).exists() {
        println!("cargo:warning=VC-LTL can't find lib files, please download the binary files from https://github.com/Chuyu-Team/VC-LTL/releases/latest. So the VC-LTL stage is skipped.");
        return;
    }

    for line in [
        format!("#######################################################################"),
        format!("#                                                                     #"),
        format!("#     *         *      * *             *        * * * * *  *          #"),
        format!("#      *       *     *                 *            *      *          #"),
        format!("#       *     *     *       * * * * *  *            *      *          #"),
        format!("#        *   *       *                 *            *      *          #"),
        format!("#          *           * *             * * * *      *      * * * *    #"),
        format!("#                                                                     #"),
        format!("#######################################################################"),
        format!("VC-LTL Path : {}", vcltl_root),
        format!("WindowsTargetPlatformMinVersion : {}", version_string),
        format!("Platform : {}", target_platform),
    ] {
        println!("cargo:warning={line}");
    }

    println!("cargo:rustc-link-search={}", library_folder);
}
