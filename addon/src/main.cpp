#include <node.h>
#include <v8.h>

#include "gdext.hpp"

#include <sstream>

static void DLL_isGDOpen(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();

    args.GetReturnValue().Set( v8::Boolean::New( isolate, gdext::isGDOpen() ) );
}

extern "C" NODE_MODULE_EXPORT void
NODE_MODULE_INITIALIZER(v8::Local<v8::Object> exports,
                        v8::Local<v8::Value> module,
                        v8::Local<v8::Context> context) {

    NODE_SET_METHOD(exports, "isGDOpen", DLL_isGDOpen);
}

static void FakeInit(v8::Local<v8::Object> exports,
                     v8::Local<v8::Value> module,
                     v8::Local<v8::Context> context) {}

#undef NODE_MODULE_VERSION
#define NODE_MODULE_VERSION 3
NODE_MODULE(NODE_GYP_MODULE_NAME, FakeInit)