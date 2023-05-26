# mqtt_client

本工程用于MQTT和json相结合使用，测试数据接收和发送
开发环境：Linux系统 Debian/ubuntu
确保您的Debian系统已安装C语言编译器和必要的依赖库。您可以使用以下命令安装它们：

sudo apt-get update
sudo apt-get install build-essential

下载并安装Eclipse Paho库：
·在终端中执行以下命令来获取Eclipse Paho库的源代码：
git clone https://github.com/eclipse/paho.mqtt.c.git
进入克隆的目录：
cd paho.mqtt.c
构建并安装mqtt库：
make
sudo make install



注意：
在编译时遇到了缺少openssl/bio.h文件的错误。这意味着您的系统缺少OpenSSL开发库。您可以通过以下步骤来解决这个问题：
安装OpenSSL开发库：
在终端中执行以下命令以安装所需的OpenSSL开发库：
sudo apt-get update
sudo apt-get install libssl-dev
