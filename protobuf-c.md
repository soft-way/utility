# protobuf for c language

## procedure
Refer to [github prorobuf-c](https://github.com/protobuf-c/protobuf-c)
```markdown
Firstly, install protobuf
$ mkdir ~/setup/protobuf-3.4.1
$ cd ~/setup/protobuf-3.4.1
$ git clone -b v3.4.1 https://github.com/google/protobuf.git .
$ ./autogen.sh
$ ./configure
$ make -j4
$ make install

$ mkdir ~/setup/protobuf-c-1.3.0
$ cd ~/setup/protobuf-c-1.3.0
$ git clone -b v1.3.0 https://github.com/protobuf-c/protobuf-c.git .
$ ./autogen.sh
$ export protobuf_LIBS="-L/usr/local/lib -lprotobuf -pthread -lpthread"
$ export protobuf_CFLAGS="-I/usr/local/include -pthread"
$ ./configure
$ make -j4
$ make install

Test protobuf-c
$ protoc --c_out=. t/test-proto3.proto
```
