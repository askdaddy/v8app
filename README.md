# C++ Applacation Embed Google's V8 engine

## 编译V8静态库


```shell script

# google 工具链 depot_tools 安装
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git

export PATH=/path/to/depot_tools:$PATH

# 获取源码/很久
fetch v8

# 安装依赖/也是很久
cd v8 && gclient sync

# 生成编译配置 pwd = v8/
tools/dev/v8gen.py x64.release

# 编译静态库
ninja -C out.gn/x64.release v8_monolith
```

执行完成后在v8/out.gn/x64.release/obj目录下的.a就是我们要的库
以上是完整编译，库会很大，实际上可以只编译需要的部分。
最后把`v8/include` 和`v8/out.gn/x64.release/obj` 两个目录cp到项目里就可以开始了。

