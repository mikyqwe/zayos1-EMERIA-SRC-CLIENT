// Copyright (c) 2020 Marshall A. Greenblatt. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//
// $hash=370cdeaa3252a9ed0e1a627d858dcab23af24ee1$
//

#ifndef CEF_INCLUDE_CAPI_CEF_THREAD_CAPI_H_
#define CEF_INCLUDE_CAPI_CEF_THREAD_CAPI_H_
#pragma once

#include "capi/cef_task_capi.h"
#include "internal/cef_thread_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

///
// A simple thread abstraction that establishes a message loop on a new thread.
// The consumer uses cef_task_runner_t to execute code on the thread's message
// loop. The thread is terminated when the cef_thread_t object is destroyed or
// stop() is called. All pending tasks queued on the thread's message loop will
// run to completion before the thread is terminated. cef_thread_create() can be
// called on any valid CEF thread in either the browser or render process. This
// structure should only be used for tasks that require a dedicated thread. In
// most cases you can post tasks to an existing CEF thread instead of creating a
// new one; see cef_task.h for details.
///
typedef struct _cef_thread_t {
  ///
  // Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  // Returns the cef_task_runner_t that will execute code on this thread's
  // message loop. This function is safe to call from any thread.
  ///
  struct _cef_task_runner_t*(CEF_CALLBACK* get_task_runner)(
      struct _cef_thread_t* self);

  ///
  // Returns the platform thread ID. It will return the same value after stop()
  // is called. This function is safe to call from any thread.
  ///
  cef_platform_thread_id_t(CEF_CALLBACK* get_platform_thread_id)(
      struct _cef_thread_t* self);

  ///
  // Stop and join the thread. This function must be called from the same thread
  // that called cef_thread_create(). Do not call this function if
  // cef_thread_create() was called with a |stoppable| value of false (0).
  ///
  void(CEF_CALLBACK* stop)(struct _cef_thread_t* self);

  ///
  // Returns true (1) if the thread is currently running. This function must be
  // called from the same thread that called cef_thread_create().
  ///
  int(CEF_CALLBACK* is_running)(struct _cef_thread_t* self);
} cef_thread_t;

///
// Create and start a new thread. This function does not block waiting for the
// thread to run initialization. |display_name| is the name that will be used to
// identify the thread. |priority| is the thread execution priority.
// |message_loop_type| indicates the set of asynchronous events that the thread
// can process. If |stoppable| is true (1) the thread will stopped and joined on
// destruction or when stop() is called; otherwise, the the thread cannot be
// stopped and will be leaked on shutdown. On Windows the |com_init_mode| value
// specifies how COM will be initialized for the thread. If |com_init_mode| is
// set to COM_INIT_MODE_STA then |message_loop_type| must be set to ML_TYPE_UI.
///
CEF_EXPORT cef_thread_t* cef_thread_create(
    const cef_string_t* display_name,
    cef_thread_priority_t priority,
    cef_message_loop_type_t message_loop_type,
    int stoppable,
    cef_com_init_mode_t com_init_mode);

#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_CEF_THREAD_CAPI_H_
