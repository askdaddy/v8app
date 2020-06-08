//
// Created by seven on 2020/6/8.
//

#include "v8.h"
#include "libplatform/libplatform.h"

char version[99];



using v8::V8;
using v8::Platform;
using v8::HandleScope;
using v8::Isolate;
using v8::ArrayBuffer;
using v8::Context;
using v8::Local;
using v8::ObjectTemplate;
using v8::NewStringType;
using v8::String;
using v8::FunctionTemplate;
using v8::Value;
using v8::FunctionCallbackInfo;
using v8::PropertyCallbackInfo;

extern void VersionGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
extern void Print(const FunctionCallbackInfo<Value> &args);
extern void Quit(const FunctionCallbackInfo<Value> &args);
extern void Load(const FunctionCallbackInfo <Value> &args);
void RunShell(Local<Context> context, Platform *pPlatform);
Local<Context> CreateShellContext(Isolate *pIsolate);

int main(int argc, char *argv[]) {
    // 初始化V8
    V8::InitializeICUDefaultLocation(argv[0]);
    V8::InitializeExternalStartupData(argv[0]);
    std::unique_ptr<Platform> platform = v8::platform::NewDefaultPlatform();
    V8::InitializePlatform(platform.get());
    V8::Initialize();
    V8::SetFlagsFromCommandLine(&argc, argv, true);

    // 创建一个新的隔离区，并将这个隔离区置为当前使用
    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
            ArrayBuffer::Allocator::NewDefaultAllocator();
    Isolate *isolate = Isolate::New(create_params);
    {
        Isolate::Scope isolate_scope(isolate);

        // 创建一个栈分配的句柄范围
        HandleScope handle_scope(isolate);

        // 初始化version
        strncpy(version, V8::GetVersion(), sizeof(version));

        // 创建一个上下文
        Local<Context> context = CreateShellContext(isolate);
        if (context.IsEmpty()) {
            fprintf(stderr, "Error creating context\n");
            return 1;
        }
        // 进入上下文编译和运行脚本
        Context::Scope context_scope(context);

        // 死循环等待用户的输入
        RunShell(context, platform.get());
    }
}
Local<Context> CreateShellContext(Isolate *isolate) {

    // 为全局对象创建一个模板
    Local<ObjectTemplate> global = ObjectTemplate::New(isolate);

    // Bind the global 'print' function to the C++ Print callback.
    global->Set(
            String::NewFromUtf8(isolate, "print", NewStringType::kNormal)
                    .ToLocalChecked(),
            FunctionTemplate::New(isolate, Print));


    // Bind the global 'load' function to the C++ Load callback.
    global->Set(String::NewFromUtf8(
            isolate, "load", NewStringType::kNormal).ToLocalChecked(),
                FunctionTemplate::New(isolate, Load));


    // Bind the 'quit' function
    global->Set(String::NewFromUtf8(
            isolate, "quit", NewStringType::kNormal).ToLocalChecked(),
                FunctionTemplate::New(isolate, Quit));

    // Bind the 'version' variable
    global->SetAccessor(String::NewFromUtf8(isolate, "version").ToLocalChecked(), VersionGetter);

    /* Binding the Point Class */
    /* 这个版本是可以支持在命令行上新建无数个Point类，并对类进行一些操作*/
    //create a pointer to a class template
//    Handle<FunctionTemplate> point_templ = FunctionTemplate::New(isolate, constructPoint);

    //assign the "Point" name to the new class template
//  point_templ->SetClassName(String::NewFromUtf8(isolate, "Point").ToLocalChecked());

    // 挂载Point类到全局对象中，保证可用
//    global->Set(String::NewFromUtf8(
//            isolate, "Point", NewStringType::kNormal).ToLocalChecked(), point_templ);

    //初始化原型模板
//    Handle<ObjectTemplate> point_proto = point_templ->PrototypeTemplate();

    // 原型模板上挂载multi方法
//    point_proto->Set(String::NewFromUtf8(
//            isolate, "multi", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, PointMulti));

    // 初始化实例模板
//    Handle<ObjectTemplate> point_inst = point_templ->InstanceTemplate();

    //set the internal fields of the class as we have the Point class internally
//    point_inst->SetInternalFieldCount(1);

    //associates the name "x"/"y" with its Get/Set functions
//    point_inst->SetAccessor(String::NewFromUtf8(isolate, "x").ToLocalChecked(), GetPointX, SetPointX);
//    point_inst->SetAccessor(String::NewFromUtf8(isolate, "y").ToLocalChecked(), GetPointY, SetPointY);

    // 给访问x设置一个拦截器吧
//    point_inst->SetHandler(NamedPropertyHandlerConfiguration(PointInterceptorGetter, PointInterceptorSetter));

    /* Binding Point class ending*/

    const Local<Context> context = Context::New(isolate, NULL, global);

    return context;
}

