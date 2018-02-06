# Compile GMssl RPM for opencryptoki

## Procedure for CentO/RHEL7(x86_64), SUSE12 (s390x)
```markdown
$ mkdir -p $RPM_BUILD/SOURCES/gmssl-1.1.0e
$ cd $RPM_BUILD/SOURCES/gmssl-1.1.0e
$ git clone https://github.com/soft-way/gmssl-110e.git .
$ cd $RPM_BUILD/SOURCES
$ tar -cf gmssl-1.1.0e.tar gmssl-1.1.0e
$ gzip gmssl-1.1.0e.tar
$ rpmbuild -bb gmssl-1.1.0e/rpm/gmssl.spec

```

## RPM on bintray
https://dl.bintray.com/softway/rpm/7/x86_64/g/

https://dl.bintray.com/softway/rpm/7/s390x/g/
