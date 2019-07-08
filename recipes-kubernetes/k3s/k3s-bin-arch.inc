# Select pre-compiled package based on target architecture
def k3s_pkg_arch_map(d):
    arch = d.getVar('TARGET_ARCH', True)
    tune = d.getVar('TUNE_FEATURES', True)
    tune_features = tune.split()
    if arch == "x86_64":
        if not "mx32" in tune_features:
            pkg = "amd64"
    elif arch == "aarch64" or arch == "arm64":
        pkg = "arm64"
    elif arch == "arm":
        if "callconvention-hard" in tune_features:
            pkg = "armhf"
    
    if not pkg:
        raise bb.parse.SkipPackage("Target architecture '%s' is not supported by the meta-k3s layer" %arch)

    return pkg

K3S_PKG_ARCH = "${@k3s_pkg_arch_map(d)}"
require k3s-bin-${K3S_PKG_ARCH}.inc
