SUMMARY = "K3s Lightweight Kubernetes (binary version)"
DESCRIPTION = "\
    K3s Lightweight Kubernetes (binary version) \
     \
    "
HOMEPAGE = "https://github.com/rancher/k3s"
MAINTAINER = "Jan Brdr <jan@rancher.com>"
LICENSE = "Apache-2.0"

LIC_FILES_CHKSUM = "file://LICENSE;md5=2ee41112a44fe7014dce33e26468ba93"

SRC_URI_append = " \
    https://github.com/rancher/k3s/LICENSE;unpack=0;name=license"

SRC_URI[license.md5sum] = "2ee41112a44fe7014dce33e26468ba93"
SRC_URI[license.sha256sum] = "0d542e0c8804e39aa7f37eb00da5a762149dc682d7829451287e11b938e94594"

INC_PR = "r0"

S = "${WORKDIR}"

INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT  = "1"
INSANE_SKIP_${PN} = "${ERROR_QA} ${WARN_QA}"
INSANE_SKIP_${PN} += "ldflags already-stripped"
ALLOW_EMPTY_${PN} = "1"

RDEPENDS_${PN} = "util-linux ethtool iproute2 iproute2-tc ebtables iptables socat "
RCONFLICTS_${PN} += "containerd cni runc kubernetes"
RPROVIDES_${PN} = "kubectl crictl"

do_unpack[noexec] = "1"
do_patch[noexec] = "1"
do_configure[noexec] = "1"
do_compile[noexec] = "1"

do_install() {
    # Install binary
    install -d ${D}${bindir}
    install -m 755 ${S}/k3s ${D}${bindir}/k3s
    
    # Create symbolic Links
    ln -s -r ${D}${bindir}/k3s ${D}${bindir}/kubectl
    ln -s -r ${D}${bindir}/k3s ${D}${bindir}/crictl

    # K3s config directory
    install -d ${D}${sysconfdir}/rancher/k3s

    # K3s data directory
    install -d ${D}${localstatedir}/lib/rancher/k3s

    # CNI config directory
    install -d ${D}${sysconfdir}/cni/net.d

    # containerd socket file directory
    install -d ${D}/run/k3s/containerd
}

FILES_${PN} += " \
    ${bindir}/k3s \
    ${bindir}/kubectl \
    ${bindir}/crictl \
    ${localstatedir}/lib/rancher/k3s/ \
    ${sysconfdir}/rancher/k3s \
    ${sysconfdir}/cni/net.d/ \
    /run/k3s/containerd/"
