# Protobuf for c language

## Procedure compile and install (CentOS 7)
### Install protobuf on CentOS7/RHEL7
Refer to [github prorobuf-c](https://github.com/protobuf-c/protobuf-c)
```markdown
Firstly, install protobuf
$ mkdir -p ~/setup/protobuf-3.4.1
$ cd ~/setup/protobuf-3.4.1
$ git clone -b v3.4.1 https://github.com/google/protobuf.git .
$ ./autogen.sh
$ ./configure --prefix=/usr --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/libexec \
    --sysconfdir=/etc --datadir=/usr/share --includedir=/usr/include --libdir=/usr/lib64 
    --sharedstatedir=/var/lib --mandir=/usr/share/man --infodir=/usr/share/info

$ make -j4
$ make install
```

### Install protobuf-c on CentOS7/RHEL7
```markdown
$ mkdir -p ~/setup/protobuf-c-1.3.0
$ cd ~/setup/protobuf-c-1.3.0
$ git clone -b v1.3.0 https://github.com/protobuf-c/protobuf-c.git .
$ ./autogen.sh
$ ./configure --prefix=/usr --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/libexec \
    --sysconfdir=/etc --datadir=/usr/share --includedir=/usr/include --libdir=/usr/lib64 
    --sharedstatedir=/var/lib --mandir=/usr/share/man --infodir=/usr/share/info

$ make -j4
$ make install
```
### Test protobuf-c
$ protoc --c_out=. t/test-proto3.proto

## Procedure rpm build (CentOS 7)
### Build protobuf
```markdown
$ cd ~/rpmbuild/SOURCES
$ wget -O protobuf-3.5.1.tar.gz https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-all-3.5.1.tar.gz
$ wget -O googlemock-1.7.0.tar.gz https://github.com/google/googlemock/archive/release-1.7.0.tar.gz#/googlemock-1.7.0.tar.gz
$ wget -O googletest-1.7.0.tar.gz https://github.com/google/googletest/archive/release-1.7.0.tar.gz#/googletest-1.7.0.tar.gz
if not, install required package
# yum -y install emacs emacs-el gcc-c++

$ wget -O protobuf-3.5.1.spec https://raw.githubusercontent.com/soft-way/utility/master/rpm/protobuf-3.5.1.spec
$ get -O ~/rpmbuild/SOURCES/ftdetect-proto.vim https://raw.githubusercontent.com/soft-way/utility/master/rpm/ftdetect-proto.vim
$ get -O ~/rpmbuild/SOURCES/protobuf-init.el https://raw.githubusercontent.com/soft-way/utility/master/rpm/protobuf-init.el
$ rpmbuild -bb protobuf-3.5.1.spec --without python --without java

```
### Build protobuf-c
```markdown
$ mkdir -p ~/setup/protobuf-c-1.3.0
$ cd ~/setup/protobuf-c-1.3.0
$ git clone -b v1.3.0 https://github.com/protobuf-c/protobuf-c.git .
$ cd ~/setup/
$ tar -cf ~/rpmbuild/SOURCES/protobuf-c-1.3.0.tar protobuf-c-1.3.0
$ gzip ~/rpmbuild/SOURCES/protobuf-c-1.3.0.tar
$ wget -O protobuf-c-1.3.0.spec https://raw.githubusercontent.com/soft-way/utility/master/rpm/protobuf-c-1.3.0.spec
$ rpmbuild -bb protobuf-c-1.3.0.spec

```

## RPM installation
```markdown
# yum localinstall protobuf-*.rpm
```
