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
#include <nan.h>

using namespace v8;
using namespace node;

namespace {

struct Emitter: ObjectWrap {
  static NAN_METHOD(New);
  static NAN_METHOD(Ping);
};


NAN_METHOD(Emitter::New) {
  assert(info.IsConstructCall());
  Emitter* self = new Emitter();
  self->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}


// emits ping event
NAN_METHOD(Emitter::Ping) {
  Handle<Value> argv[2] = {
    Nan::New("ping").ToLocalChecked(), // event name
    info[0]->ToString()  // argument
  };

  Nan::MakeCallback(info.This(), "emit", 2, argv);
}


extern "C" NAN_MODULE_INIT(init) {
  Local<FunctionTemplate> t = Nan::New<FunctionTemplate>(Emitter::New);
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(Nan::New("Emitter").ToLocalChecked());
  Nan::SetPrototypeMethod(t, "ping", Emitter::Ping);

  Nan::Set(target, Nan::New("Emitter").ToLocalChecked(), t->GetFunction());
}

NODE_MODULE(eventemitter, init)

} // anonymous namespace
