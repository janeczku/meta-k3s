SUMMARY = "Startup script and systemd unit file for K3s"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://k3s.init \
           file://k3s.service \
           file://k3s.default"

S = "${WORKDIR}"

inherit update-rc.d systemd distro_features_check

RDEPENDS_${PN} = "k3s"

# The mode can be changed outside of this recipe by using
# runmode_pn-k3s-init = "my-host-name".
runmode = "server"

do_install() {
	# Old
	#install -d ${D}${systemd_unitdir}/system/
	#install -d ${D}${systemd_unitdir}/system/k3s-server.service.d/
	#install -m 0644 ${S}/10-agent.conf ${D}${systemd_unitdir}/system/k3s.service.d/
	#install -d ${D}${sysconfdir}/default/
	#install -m 0644 ${S}/k3s-server.default ${D}${sysconfdir}/default/k3s-server

	# Config
	install -Dm0644 ${S}/k3s.default ${D}${sysconfdir}/default/k3s

	# Systemd
	# if ${@bb.utils.contains('DISTRO_FEATURES','systemd','true','false',d)}; then
	install -Dm0644 ${S}/k3s.service ${D}${systemd_unitdir}/system/k3s.service
	# install -Dm0644 ${S}/10-type.conf ${D}${systemd_unitdir}/system/k3s.service.d/10-type.conf

	# Sysvinit
	install -Dm755 ${S}/k3s.init ${D}/${sysconfdir}/init.d/k3s

	# Fix paths
	sed -i -e 's,@BASE_BINDIR@,${base_bindir},g' \
		-e 's,@BINDIR@,${bindir},g' \
		-e 's,@SBINDIR@,${sbindir},g' \
		${D}${systemd_unitdir}/system/k3s.service ${D}/${sysconfdir}/init.d/k3s

	# Expand configuration placeholders
	# sed -i -e 's,@SECRET@,${K3S_CLUSTER_SECRET},g' ${D}${sysconfdir}/default/k3s-server

	# change unit type for agent mode
	if [ "${runmode}" -eq "agent" ]; then
		sed -i -e 's,Type=notify,Type=exec,g' ${D}${systemd_unitdir}/system/k3s.service
	fi
}

FILES_${PN} += " \
    ${sysconfdir}/default/k3s \
    ${systemd_unitdir}/system/k3s.service \
    ${sysconfdir}/init.d/k3s"

INITSCRIPT_NAME = "k3s"
INITSCRIPT_PARAMS = "start 9 5 2 . stop 20 0 1 6 ."

SYSTEMD_SERVICE_${PN} = "k3s.service"
SYSTEMD_AUTO_ENABLE_${PN} = "enable"
