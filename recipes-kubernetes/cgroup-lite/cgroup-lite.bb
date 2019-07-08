SUMMARY = "Light-weight package to set up cgroups at system boot."
HOMEPAGE = "https://launchpad.net/ubuntu/+source/cgroup-lite"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://debian/copyright;md5=5d5da4e0867cf06014f87102154d0102"

PV = "1.15"

SRC_URI = "https://launchpad.net/ubuntu/+archive/primary/+files/cgroup-lite_${PV}.tar.xz"
SRC_URI += "file://cgroup-lite.init"
SRC_URI[md5sum] = "1438c1f4a7227c0dedfce5f86f02591d"
SRC_URI[sha256sum] = "02f44c70ed3cf27b9e89e5266492fddf4b455336ab4e03abc85e92297537201f"

# cgroup-lite requires the following commands: grep sed rmdir mount umount mountpoint
RDEPENDS_${PN} = "busybox"

inherit allarch update-rc.d systemd

do_install() {
	install -d ${D}/bin
	install -m 0755 ${S}/scripts/cgroups-mount ${D}/bin
	install -m 0755 ${S}/scripts/cgroups-umount ${D}/bin

	install -d ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/cgroup-lite.init ${D}${sysconfdir}/init.d/cgroup-lite

	install -d ${D}${systemd_unitdir}/system
	ln -sf /dev/null ${D}${systemd_unitdir}/system/cgroup-init.service
}

INITSCRIPT_NAME = "cgroup-lite"
INITSCRIPT_PARAMS = "defaults 10 20"

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "cgroup-lite.service"
SYSTEMD_AUTO_ENABLE_${PN} = "mask"