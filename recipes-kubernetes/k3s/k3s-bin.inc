SUMMARY = "k3s Lightweight Kubernetes (binary package)"
DESCRIPTION = "\
    k3s is a fully compliant production-grade Kubernetes \
    distribution built for the Edge. \
    "
HOMEPAGE = "https://k3s.io"
MAINTAINER = "Jan Brdr <jan@rancher.com>"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

INC_PR = "r0"

S = "${WORKDIR}"

inherit distro_features_check

# TODO: slirp4netns, bridge-utils ?
RDEPENDS_${PN} = "util-linux findutils pigz iproute2 ethtool iptables socat ca-certificates \
                  ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '', 'cgroup-lite', d)} \
                  "
RCONFLICTS_${PN} += "containerd cni runc kubernetes"
RPROVIDES_${PN} = "kubectl crictl"

INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT  = "1"
INSANE_SKIP_${PN} += "ldflags already-stripped"

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
