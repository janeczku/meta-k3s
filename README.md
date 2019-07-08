# meta-k3s

An OpenEmbedded Layer that provides recipes to install [k3s](https://github.com/rancher/k3s), a lightweight Kubernetes distribution for the Edge.

## Supported target architectures

The recipe installs precompiled [release binaries](https://github.com/rancher/k3s/releases/latest) from the upstream k3s project.

The following OE target architectures are supported by this recipe:

- x86_64
- aarch64 (see for example [Odroid C2 image](https://github.com/janeczku/meta-k3s-odroid-c2))
- arm64
- arm(hf)

## Layer dependencies

This layer depends on:

```
  URI: git://git.yoctoproject.org/poky.git
  branch: warrior
  
```

## Usage

### Installing as system service

In order to install k3s as a system service, add both `k3s` and `k3s-init` to the list of packages for your OE image, e.g.:

```
IMAGE_INSTALL_append = " k3s k3s-init"
```

### Worker nodes

The initscript/systemd unit installed by `k3s-init` starts k3s in server mode by default. In order to create images for k3s workers, the contents of the `/etc/default/k3s` file in the root FS must be updated like so:

```
K3S_URL=https://<URL of k3s server>
K3S_CLUSTER_SECRET=changeme
K3S_ARGS="agent"
```

The value of the `K3S_ARGS` variable is passed as argument to k3s. The supported flags are [documented here](https://rancher.com/docs/k3s/latest/en/installation/#server-options). Any of the environment variables supported by k3s can be added to the `/etc/default/k3s` file.

One approach to customizing the configuration is replacing the contents of `/etc/default/k3s` during the image build. This could be achieved by adding a `ROOTFS_POSTPROCESS_COMMAND` to the image recipe for example:

```
# Configure k3s service to start in agent mode
set_k3s_agent_config () {
  cat <<EOF > /etc/default/k3s
K3S_URL="https://k3s-master.local:6443"
K3S_CLUSTER_SECRET="changeme"
K3S_ARGS="agent"
  EOF
}

# k3s unit type should be set to 'exec' for agent mode
set_k3s_unit_type_exec () {
	sed -i -e 's,Type=notify,Type=exec,g' ${IMAGE_ROOTFS}${systemd_unitdir}/system/k3s.service
}

ROOTFS_POSTPROCESS_COMMAND += "set_k3s_agent_config;"
ROOTFS_POSTPROCESS_COMMAND += '${@bb.utils.contains("DISTRO_FEATURES", "systemd", "set_systemd_default_target; systemd_create_users;", "", d)}'
```

### Linux Kernel Compatibility

Your OE image's Linux kernel must be compiled to support all features that are required to run Kubernetes (Linux containers, networking, etc).

To verify that your Kernel configuration meets the core requirements you can use the [check-config.sh](https://github.com/moby/moby/blob/master/contrib/check-config.sh) script form the Docker open source project. 
The script reads and checks the kernel configuration from either the running kernel or the specified kernel `.config` file:

```
$ ./check-config.sh
$ ./check-config.sh /path/to/.config
```
