#./configure --prefix=/opt/run/openssh/
#edit Makefile and add -lcurl
rm -rf /opt/run/openssh/
make && make install
sudo /opt/run/openssh/sbin/sshd

## note ##
#root@gitshell:/tmp/gitshellopenssh# ls -ld /home/git/.ssh/authorized_keys
#-rw-r--r-- 1 git git 0 Aug 26 12:50 /home/git/.ssh/authorized_keys
#root@gitshell:/tmp/gitshellopenssh# ls -ld /home/git/.ssh/
#drwx------ 2 git git 4096 Aug 26 12:50 /home/git/.ssh/
