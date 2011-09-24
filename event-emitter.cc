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

using namespace v8;
using namespace node;

namespace {

struct Emitter: ObjectWrap {
  static Handle<Value> New(const Arguments& args);
  static Handle<Value> Ping(const Arguments& args);
};


Handle<Value> Emitter::New(const Arguments& args) {
  HandleScope scope;

  assert(args.IsConstructCall());
  Emitter* self = new Emitter();
  self->Wrap(args.This());

  return scope.Close(args.This());
}


// emits ping event
Handle<Value> Emitter::Ping(const Arguments& args) {
  HandleScope scope;

  Local<Value> emit_v = args.This()->Get(String::NewSymbol("emit"));
  assert(emit_v->IsFunction());
  Local<Function> emit_f = emit_v.As<Function>();

  Handle<Value> argv[2] = {
    String::New("ping"), // event name
    args[0]->ToString()  // argument
  };

  TryCatch tc;

  emit_f->Call(args.This(), 2, argv);

  if (tc.HasCaught())
    FatalException(tc);

  return Undefined();
}


extern "C" void init(Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(Emitter::New);
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(String::New("Emitter"));
  NODE_SET_PROTOTYPE_METHOD(t, "ping", Emitter::Ping);

  target->Set(String::NewSymbol("Emitter"), t->GetFunction());
}

} // anonymous namespace
