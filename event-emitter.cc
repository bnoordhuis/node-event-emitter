/*
 * Copyright (c) 2011, Ben Noordhuis <info@bnoordhuis.nl>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>

using namespace v8;
using namespace node;

struct Emitter: ObjectWrap {
  static void New(const FunctionCallbackInfo<Value>& args);
  static void Ping(const FunctionCallbackInfo<Value>& args);
};


void Emitter::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  assert(args.IsConstructCall());
  Emitter* self = new Emitter();
  self->Wrap(args.This());
  args.GetReturnValue().Set(args.This());
}


// emits ping event
void Emitter::Ping(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Handle<Value> argv[2] = {
    String::NewFromUtf8(isolate, "ping"), // event name
    args[0]->ToString()  // argument
  };

  MakeCallback(isolate, args.This(), "emit", 2, argv);
}


static v8::Persistent<v8::Function> constructor;
void init(Handle<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Local<FunctionTemplate> t = FunctionTemplate::New(isolate, Emitter::New);
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(String::NewFromUtf8(isolate, "Emitter"));
  NODE_SET_PROTOTYPE_METHOD(t, "ping", Emitter::Ping);
  constructor.Reset(isolate, t->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Emitter"), t->GetFunction());
}

NODE_MODULE(event_emitter, init)
