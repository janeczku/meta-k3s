require k3s-bin.inc
SRC_URI = "https://github.com/rancher/k3s/releases/download/v${K3S_VERSION}/k3s-armhf;unpack=0;name=k3s;downloadfilename=k3s"
SRC_URI[k3s.sha256sum] = "4f45986bddc22bf540e271daaede764790584ba1e45ec264eeec07fae80ae937"
