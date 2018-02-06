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
$ ./configure --program-prefix= --prefix=/usr --exec-prefix=/usr --bindir=/usr/bin --sbindir=/usr/sbin \
    --sysconfdir=/etc --datadir=/usr/share --includedir=/usr/include --libdir=/usr/lib64 \
    --libexecdir=/usr/libexec --localstatedir=/var --sharedstatedir=/var/lib --mandir=/usr/share/man \
    --infodir=/usr/share/info --with-systemd=/usr/lib/systemd/system --disable-icatok --disable-ccatok \
    --enable-testcases

$ make -j4
# groupadd pkcs11
$ make install

```
## RPM build on CentOS7(x86_64), SUSE12(s390x)
```markdown
$ mkdir -p $RPM_BUILD/SOURCES/opencryptoki-3.8.2
$ cd $RPM_BUILD/SOURCES/opencryptoki-3.8.2
$ git clone -b v3.8.2 https://github.com/opencryptoki/opencryptoki.git .
$ cd $RPM_BUILD/SOURCES
$ tar -cf opencryptoki-3.8.2.tar opencryptoki-3.8.2
$ gzip opencryptoki-3.8.2.tar

# for SUSE12(s390x), add below into ~/.rpmmacros
%_sharedstatedir /var/lib

$ rpmbuild -bb opencryptoki-3.8.2/rpm/opencryptoki.spec
```

## RPM package on bintray
https://dl.bintray.com/softway/rpm/7/s390x/o/
