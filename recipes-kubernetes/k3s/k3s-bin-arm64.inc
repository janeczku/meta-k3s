require k3s-bin.inc
SRC_URI = "https://github.com/rancher/k3s/releases/download/v${K3S_VERSION}/k3s-arm64;unpack=0;name=k3s;downloadfilename=k3s"
SRC_URI[k3s.sha256sum] = "72b01abad9b6f0cc615cc999b2c4724d0b4862d063e2752eff0dc7ea69985775"