require k3s-bin.inc
SRC_URI = "https://github.com/rancher/k3s/releases/download/v${K3S_VERSION}/k3s;unpack=0;name=k3s;downloadfilename=k3s"
SRC_URI[k3s.sha256sum] = "df7ff885145cf58a8c32e5c7b66eb2659708d433f360e36b59350c431bf81e46"
