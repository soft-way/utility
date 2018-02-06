# Compile opencryptoki
Refer to [github](https://github.com/opencryptoki/opencryptoki)
## Procedure for local installation
```markdown
You may need below required package for building opencryptoki
$ yum -y install automake autoconf libtool openssl-devel git rpm-build
$ yum -y install openldap-devel libitm libitm-devel flex bison expect ksh trousers-devel
$ mkdir -p ~/setup/opencryptoki-3.8.2
$ cd ~/setup/opencryptoki-3.8.2
$ git clone -b v3.8.2 https://github.com/opencryptoki/opencryptoki.git .
$ ./bootstrap.sh
$ ./configure --enable-debug --enable-testcases 
$ make -j4
# groupadd pkcs11
$ make install

```
## RPM build on CentOS7
```markdown
$ mkdir -p ~/setup/opencryptoki-3.8.2
$ cd ~/setup/opencryptoki-3.8.2
$ git clone -b v3.8.2 https://github.com/opencryptoki/opencryptoki.git .
$ cd ..
$ tar -cf ~/rpmbuild/SOURCES/opencryptoki-3.8.2.tar opencryptoki-3.8.2
$ gzip ~/rpmbuild/SOURCES/opencryptoki-3.8.2.tar
$ rpmbuild -bb opencryptoki-3.8.2/rpm/opencryptoki.spec --define "dist .el7"
```
