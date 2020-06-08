//
// Created by seven on 2020/6/8.
//

#include "./app.h"
#include "v8.h"
#include "libplatform/libplatform.h"

using v8::Isolate;
using v8::Value;
using v8::FunctionCallbackInfo;
using v8::HandleScope;
using v8::String;
using v8::Local;
using v8::PropertyCallbackInfo;
using v8::MaybeLocal;
using v8::NewStringType;

extern char version[99];

extern const char *ToCString(const String::Utf8Value &value);

extern MaybeLocal<String> ReadFile(Isolate *isolate, const char *name);

extern bool ExecuteString(Isolate *isolate, Local<String> source,
                          Local<Value> name, bool print_result,
                          bool report_exceptions);

void VersionGetter(Local<String> property,
                   const PropertyCallbackInfo<Value> &info) {
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), version).ToLocalChecked());
}

// The callback that is invoked by v8 whenever the JavaScript 'quit'
// function is called.  Quits.
void Quit(const FunctionCallbackInfo<Value> &args) {
    // If not arguments are given args[0] will yield undefined which
    // converts to the integer value 0.
    int exit_code =
            args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
    fflush(stdout);
    fflush(stderr);
    exit(exit_code);
}

void Print(const FunctionCallbackInfo<Value> &args) {
    bool first = true;
    for (int i = 0; i < args.Length(); i++) {
        HandleScope handle_scope(args.GetIsolate());
        if (first) {
            first = false;
        } else {
            printf(" ");
        }
        String::Utf8Value str(args.GetIsolate(), args[i]);
        const char *cstr = ToCString(str);
        printf("%s", cstr);
    }
    printf("\n");
    fflush(stdout);
}

void Load(const FunctionCallbackInfo<Value> &args) {
    for (int i = 0; i < args.Length(); i++) {
        HandleScope handle_scope(args.GetIsolate());
        String::Utf8Value file(args.GetIsolate(), args[i]);
        if (*file == nullptr) {
            args.GetIsolate()->ThrowException(
                    String::NewFromUtf8(args.GetIsolate(), "Error loading file",
                                        NewStringType::kNormal).ToLocalChecked());
            return;
        }
        Local<String> source;
        if (!ReadFile(args.GetIsolate(), *file).ToLocal(&source)) {
            args.GetIsolate()->ThrowException(
                    String::NewFromUtf8(args.GetIsolate(), "Error loading file",
                                        NewStringType::kNormal).ToLocalChecked());
            return;
        }
        if (!ExecuteString(args.GetIsolate(), source, args[i], false, false)) {
            args.GetIsolate()->ThrowException(
                    String::NewFromUtf8(args.GetIsolate(), "Error executing file",
                                        NewStringType::kNormal).ToLocalChecked());
            return;
        }
    }
}