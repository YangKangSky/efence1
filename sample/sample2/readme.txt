source /opt/amlogic/rdkm/environment-setup-armv7at2hf-neon-rdkmllib32-linux-gnueabi


build library:

有效果：
$CXX -fsanitize=address -fno-omit-frame-pointer -shared -fPIC library.cpp -o libtest.so 

没效果：
$CXX  -shared -fPIC library.cpp -o libtest.so 


build test.cpp 

$CXX -fsanitize=address -fno-omit-frame-pointer test.cpp -L. -ltest -o mytest

$CXX test.cpp -L. -ltest -o mytest


test:
ASAN_OPTIONS=detect_leaks=1 LD_LIBRARY_PATH=. ./test

LD_PRELOAD=/usr/lib/libasan.so.5 

TARGET_CFLAGS += "-fsanitize=address -fno-omit-frame-pointer"
TARGET_CXXFLAGS += "-fsanitize=address -fno-omit-frame-pointer"


a:调试库的问题
$CXX -g -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -shared -fPIC library.cpp -o libtest.so -Wl,-rpath=/usr/lib
$CXX test.cpp -L. -ltest -o mytest


export LD_PRELOAD=/usr/lib/libasan.so.5.0.0 usr/bin/mytest


LD_PRELOAD=/usr/lib/libasan.so.5.0.0 mytest 


export LD_PRELOAD=/data/lib/libasan.so.5.0.0

/lib/systemd/system/wpeframework.service


/lib/systemd/system/wpeframework.service:35:ExecStartPre=/bin/systemctl set-environment LD_PRELOAD=/usr/lib/libwesteros_gl.so.0.0.0

ExecStartPre=/bin/systemctl set-environment  ASAN_OPTIONS=abort_on_error=0
ExecStartPre=/bin/systemctl set-environment LD_PRELOAD=/data/lib/libasan.so.5.0.0

ExecStartPre=/bin/systemctl set-environment  LD_LIBRARY_PATH=/data/lib

export ASAN_OPTIONS=verify_asan_link_order=0

systemctl stop wpeframework.service 
systemctl start wpeframework.service 

export


detect_leaks=0


ExecStartPre=/bin/systemctl set-environment  ASAN_OPTIONS=abort_on_error=0,detect_leaks=0                         
ExecStartPre=/bin/systemctl set-environment LD_PRELOAD=/usr/lib/libwesteros_gl.so.0.0.0 /data/lib/libasan.so.5.0.0


ExecStartPre=/bin/systemctl set-environment  ASAN_OPTIONS=verify_asan_link_order=0


find . -type f -exec arm-linux-gnueabi-readelf -d {} \; | grep libasan











