#./configure --prefix=/opt/run/openssh/
#edit Makefile and add -lcurl
rm -rf /opt/run/openssh/
make && make install
sudo /opt/run/openssh/sbin/sshd -d 
