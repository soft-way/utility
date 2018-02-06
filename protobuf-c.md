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
$ cd $RPM_BUILD/SOURCES
$ wget -O protobuf-3.5.1.tar.gz https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-all-3.5.1.tar.gz
$ wget -O googlemock-1.7.0.tar.gz https://github.com/google/googlemock/archive/release-1.7.0.tar.gz#/googlemock-1.7.0.tar.gz
$ wget -O googletest-1.7.0.tar.gz https://github.com/google/googletest/archive/release-1.7.0.tar.gz#/googletest-1.7.0.tar.gz
$ wget -O ftdetect-proto.vim https://raw.githubusercontent.com/soft-way/utility/master/rpm/ftdetect-proto.vim
$ wget -O protobuf-init.el https://raw.githubusercontent.com/soft-way/utility/master/rpm/protobuf-init.el
if not, install required package
# yum -y install emacs emacs-el gcc-c++

$ cd $RPM_BUILD/SPECS/
$ wget -O protobuf-3.5.1.spec https://raw.githubusercontent.com/soft-way/utility/master/rpm/protobuf-3.5.1.spec
$ rpmbuild -bb protobuf-3.5.1.spec --without python --without java

```
### Build protobuf-c
```markdown
$ cd $RPM_BUILD/SOURCES
$ wget -O protobuf-c-1.3.0.zip https://github.com/protobuf-c/protobuf-c/archive/v1.3.0.zip
$ unzip protobuf-c-1.3.0.zip
$ tar -cf protobuf-c-1.3.0.tar protobuf-c-1.3.0
$ gzip protobuf-c-1.3.0.tar
$ cd $RPM_BUILD/SPECS/
$ wget -O protobuf-c-1.3.0.spec https://raw.githubusercontent.com/soft-way/utility/master/rpm/protobuf-c-1.3.0.spec
$ rpmbuild -bb protobuf-c-1.3.0.spec

```

## Procedure rpm build (SLES12)
### Build protobuf
```markdown
$ cd $RPM_BUILD/SOURCES
# for fix issue https://github.com/google/protobuf/issues/3937, download below fixed source code
$ wget -O protobuf-3.5.1.tar.gz https://dl.bintray.com/softway/generic/p/protobuf-3.5.1.tar.gz
$ wget -O googlemock-1.7.0.tar.gz https://github.com/google/googlemock/archive/release-1.7.0.tar.gz#/googlemock-1.7.0.tar.gz
$ wget -O googletest-1.7.0.tar.gz https://github.com/google/googletest/archive/release-1.7.0.tar.gz#/googletest-1.7.0.tar.gz
$ wget -O ftdetect-proto.vim https://raw.githubusercontent.com/soft-way/utility/master/rpm/ftdetect-proto.vim
$ wget -O protobuf-init.el https://raw.githubusercontent.com/soft-way/utility/master/rpm/protobuf-init.el
if not, install required package
# zypper install emacs emacs-el gcc-c++

$ cd $RPM_BUILD/SPECS/
$ wget -O protobuf-3.5.1.spec https://raw.githubusercontent.com/soft-way/utility/master/rpm/protobuf-3.5.1.spec
$ rpmbuild -bb protobuf-3.5.1.spec --without python --without java

```
### Build protobuf-c
```markdown
$ cd $RPM_BUILD/SOURCES
$ wget -O protobuf-c-1.3.0.zip https://github.com/protobuf-c/protobuf-c/archive/v1.3.0.zip
$ unzip protobuf-c-1.3.0.zip
$ tar -cf protobuf-c-1.3.0.tar protobuf-c-1.3.0
$ gzip protobuf-c-1.3.0.tar
$ cd $RPM_BUILD/SPECS/
$ wget -O protobuf-c-1.3.0.spec https://raw.githubusercontent.com/soft-way/utility/master/rpm/protobuf-c-1.3.0.spec
$ rpmbuild -bb protobuf-c-1.3.0.spec

```
## RPM on bintray
### rpm can be downloaded for local installation or set in repo configuration
https://dl.bintray.com/softway/rpm/7/s390x/p/

https://dl.bintray.com/softway/rpm/7/x86_64/p/

## RPM installation
```markdown
# yum localinstall protobuf-*.rpm
```
