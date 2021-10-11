/*
 * PROJECT:   Rust language support for VC-LTL
 * FILE:      build.rs
 * PURPOSE:   Implementation for build script
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

fn main()
{
    if !std::env::var("CARGO_CFG_WINDOWS").is_ok()
    {
        println!("cargo:warning=VC-LTL only supports Windows host. So the VC-LTL stage is skipped.");
        return;
    }

    let target_os = std::env::var("CARGO_CFG_TARGET_OS").expect(
        "CARGO_CFG_TARGET_OS should be set by cargo.");
    let target_env = std::env::var("CARGO_CFG_TARGET_ENV").expect(
        "CARGO_CFG_TARGET_ENV should be set by cargo.");

    if !target_os.contains("windows")
    {
        println!("cargo:warning=VC-LTL only supports Windows target. So the VC-LTL stage is skipped.");
        return;
    }

    if !target_env.contains("msvc")
    {
        println!("cargo:warning=VC-LTL only supports MSVC toolchain. So the VC-LTL stage is skipped.");
        return;
    }

    let package_root = std::env::var("CARGO_MANIFEST_DIR").expect(
        "CARGO_MANIFEST_DIR should be set by cargo.");
    let target_arch = std::env::var("CARGO_CFG_TARGET_ARCH").expect(
        "CARGO_CFG_TARGET_ARCH should be set by cargo.");

    let target_platform;
    let mut minimum_version = 6000;

    if target_arch.contains("x86_64")
    {
        target_platform = "x64";
        if minimum_version < 3790
        {
            minimum_version = 3790;
        }
    }
    else if target_arch.contains("x86")
    {
        target_platform = "Win32";
        if minimum_version < 2600
        {
            minimum_version = 2600;
        }
    }
    else if target_arch.contains("arm")
    {
        target_platform = "ARM";
        if minimum_version < 9200
        {
            minimum_version = 9200;
        }
    }
    else if target_arch.contains("aarch64")
    {
        target_platform = "ARM64";
        if minimum_version < 10240
        {
            minimum_version = 10240;
        }
    }
    else
    {
        println!("cargo:warning=VC-LTL does not support {} platform. So the VC-LTL stage is skipped.", target_arch);
        return;
    }

    let minimum_version_string;

    if minimum_version >= 19041
    {
        minimum_version_string = "10.0.19041.0";
    }
    else if minimum_version >= 10240
    {
        minimum_version_string = "10.0.10240.0";
    }
    else if minimum_version >= 9200
    {
        minimum_version_string = "6.2.9200.0"
    }
    else if minimum_version >= 6000
    {
        minimum_version_string = "6.0.6000.0"
    }
    else if minimum_version >= 3790
    {
        minimum_version_string = "5.2.3790.0"
    }
    else
    {
        minimum_version_string = "5.1.2600.0"
    }

    let library_folder = &format!(
        "{}/TargetPlatform/{}/lib/{}",
        package_root,
        minimum_version_string,
        target_platform);

    if !std::path::Path::new(library_folder).exists()
    {
        println!("cargo:warning=VC-LTL can't find lib files, please download the binary files from https://github.com/Chuyu-Team/VC-LTL/releases/latest. So the VC-LTL stage is skipped.");
        return;
    }

    println!("cargo:warning=#######################################################################");
    println!("cargo:warning=#                                                                     #");
    println!("cargo:warning=#     *         *      * *             *        * * * * *  *          #");
    println!("cargo:warning=#      *       *     *                 *            *      *          #");
    println!("cargo:warning=#       *     *     *       * * * * *  *            *      *          #");
    println!("cargo:warning=#        *   *       *                 *            *      *          #");
    println!("cargo:warning=#          *           * *             * * * *      *      * * * *    #");
    println!("cargo:warning=#                                                                     #");
    println!("cargo:warning=#######################################################################");
    println!("cargo:warning=VC-LTL Path : {}", package_root);
    println!("cargo:warning=WindowsTargetPlatformMinVersion : {}", minimum_version_string);
    println!("cargo:warning=Platform : {}", target_platform);

    println!("cargo:rustc-link-search={}", library_folder);

    return;
}