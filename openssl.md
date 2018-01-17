# Compile openssl-1.1.0g
## Procedure
```markdown
$ mkdir ~/setup/openssl-1.1.0g
$ cd ~/setup/openssl-1.1.0g
$ git clone -b OpenSSL_1_1_0g https://github.com/openssl/openssl.git .
$ cd ~/setup
$ tar -cf ~/rpmbuild/SOURCES/openssl-1.1.0g.tar openssl-1.1.0g
$ gzip ~/rpmbuild/SOURCES/openssl-1.1.0g.tar
Copy a spec file to openssl code dir, the spec file is in current repo utility/rpm
$ cp ../utility/rpm/openssl-1.1.0.spec ~/setup/openssl-1.1.0g/openssl.spec
$ rpmbuild -bb ~/setup/openssl-1.1.0g/openssl.spec

```
