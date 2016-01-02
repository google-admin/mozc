// Copyright 2010-2016, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
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

#ifndef MOZC_WIN32_IME_IME_CORE_H_
#define MOZC_WIN32_IME_IME_CORE_H_

#include <windows.h>

#include <string>
#include <vector>

#include "base/port.h"
#include "testing/base/public/gunit_prod.h"
// for FRIEND_TEST()
#include "win32/base/immdev.h"
#include "win32/base/keyevent_handler.h"
#include "win32/ime/ime_input_context.h"
#include "win32/ime/ime_message_queue.h"
#include "win32/ime/ime_types.h"

namespace mozc {
namespace client {
class ClientInterface;
}  // namespace client

namespace commands {
class Context;
class Output;
}  // namespace commands

namespace win32 {

struct InputBehavior;
struct InputState;

// Currently this class is a Kitchen-sink class and can be merged with
// UIContext class.
// Note that some static methods receive raw HIMC, which makes it difficult
// to write unit tests for these methods.
// TODO(yukawa): Refactor this class with UIContext class.
// TODO(yukawa): Refactor this class to be testable.
class ImeCore {
 public:
  // Main implementation of ImeProcessKey callback.
  static KeyEventHandlerResult ImeProcessKey(
      mozc::client::ClientInterface *client,
      const VirtualKey &virtual_key,
      const LParamKeyInfo &lparam,
      const KeyboardStatus &keyboard_status,
      const InputBehavior &behavior,
      const InputState &initial_state,
      const commands::Context &context,
      InputState *next_state,
      commands::Output *output);

  // Main implementation of ImeToAsciiEx callback.
  static KeyEventHandlerResult ImeToAsciiEx(
      mozc::client::ClientInterface *client,
      const VirtualKey &virtual_key,
      BYTE scan_code,
      bool is_key_down,
      const KeyboardStatus &keyboard_status,
      const InputBehavior &behavior,
      const InputState &initial_state,
      const commands::Context &context,
      InputState *next_state,
      commands::Output *output);

  // Update |context| if surrounding text is available.
  static void UpdateContextWithSurroundingText(HIMC himc,
                                               commands::Context *context);

  // This function sends IME open message to the server w/o generating any UI
  // messages.
  static bool OpenIME(mozc::client::ClientInterface *client, DWORD next_mode);

  // This function sends IME close message to the server w/o generating any UI
  // messages.
  static bool CloseIME(mozc::client::ClientInterface *client,
                       DWORD next_mode,
                       commands::Output *output);

  // Some of the following functions have a flag named |generate_message| in
  // their parameters.  You might want to pass false to |generate_message| when
  // - |himc| is not an active context so the IME should not generate any UI
  //   message,
  // - or IME need not to generate relevant UI messages because they will be
  //   generated by IMM32 API.  This actually happens at the NotifyIME hander
  //   when action is NI_CONTEXTUPDATED.

  // This function sends SUBMIT command to the server and updates the input
  // context.  If |generate_message| is false, this function will not generate
  // any UI message.
  static bool SubmitComposition(HIMC himc, bool generate_message);

  // This function sends CANCEL command to the server and updates the input
  // context.  If |generate_message| is false, this function will not generate
  // any UI message.
  // Returns true if the operation completed successfully.
  static bool CancelComposition(HIMC himc, bool generate_message);

  // This function sends SWITCH_INPUT_MODE command to the server and updates the
  // input context.  If |generate_message| is false, this function will not
  // generate any UI message.
  // Returns true if the operation completed successfully.
  static bool SwitchInputMode(
      HIMC himc, DWORD native_mode, bool generate_message);

  // Removes and modifies unsupported bits from |raw_conversion_mode|.
  // Returns normalized conversion mode.
  static DWORD GetSupportableConversionMode(DWORD raw_conversion_mode);

  // Removes and modifies unsupported bits from |raw_sentence_mode|.
  // Returns normalized sentence mode.
  static DWORD GetSupportableSentenceMode(DWORD raw_sentence_mode);

  // Returns true if given input context is initialized for Mozc.
  static bool IsInputContextInitialized(HIMC himc);

  // This function receives a command output and next state to update the
  // following field in INPUTCONTEXT structure associated with |himc|.
  // - fOpen
  // - fdwConversion
  // - hCompStr
  // - hCandInfo
  // - hPrivate
  // Based on which field is actually updated, this function generates
  // UI messages into |message_queue|.  If |message_queue| is NULL, this
  // function will not generate any UI message.
  // Returns true if the operation completed successfully.
  static bool UpdateContext(HIMC himc,
                            const InputState &next_state,
                            const commands::Output &new_output,
                            MessageQueue *message_queue);

  // This function sends IME Off command to the server and updates the input
  // context.  If |generate_message| is false, this function will not generate
  // any UI message.
  // Returns true if the operation completed successfully.
  static BOOL IMEOff(HIMC himc, bool generate_message);

  // This function sends HIGHLIGHT_CANDIDATE command to the server and updates
  // the input context.  If |generate_message| is false, this function will not
  // generate any UI message.
  // Returns true if the operation completed successfully.
  static BOOL HighlightCandidate(HIMC himc, int32 candidate_index,
                                 bool generate_message);

  // This function sends SELECT_CANDIDATE command to the server and updates the
  // input context.  If |generate_message| is false, this function will not
  // generate any UI message.
  // Returns true if the operation completed successfully.
  static BOOL CloseCandidate(HIMC himc, bool generate_message);

  // Returns true if the |himc| is the active input context for the current
  // thread.
  static bool IsActiveContext(HIMC himc);

  // This method at least turns on the IME and try to invoke reconversion.
  // Returns true if IME is turned on eitherway.
  // Although this method updates the input context and generates UI messages,
  // it seems to be safe to call this method from UI message handler.
  static bool TurnOnIMEAndTryToReconvertFromIME(HIMC himc);

  // Query reconversion from Application.
  // Returns true if reconversion is available.
  static bool QueryReconversionFromApplication(
      HIMC himc, RECONVERTSTRING *composition_info,
      RECONVERTSTRING *reading_info);

  // Invokes reconversion from Application.
  // Returns true if reconversion is succeeded.
  static bool ReconversionFromApplication(
      HIMC himc, const RECONVERTSTRING *composition_info,
      const RECONVERTSTRING *reading_info);

 private:
  // Sort UI messages to be the same order to that of Office IME 2010
  // (IMM32 mode) on Windows 7.  See b/3488848 for details.
  // - [IMN_SETOPENSTATUS for IME-On]
  // - [IMN_SETCONVERSIONMODE]
  // - [IMN_CLOSECANDIDATE]
  // - [WM_IME_STARTCOMPOSITION]
  // - [WM_IME_COMPOSITION]
  // - [IMN_OPENCANDIDATE or IMN_CHANGECANDIDATE]
  // - [WM_IME_ENDCOMPOSITION]
  // - [IMN_SETOPENSTATUS for IME-Off]
  static void SortIMEMessages(
      const vector<UIMessage> &composition_messages,
      const vector<UIMessage> &candidate_messages,
      bool previous_open_status,
      DWORD previous_conversion_mode,
      bool next_open_status,
      DWORD next_conversion_mode,
      vector<UIMessage> *sorted_messages);

  // Internal use only.
  // This method implements main part of UpdateContext. Note that this method
  // ignores callback field, which is supposed to be handled in the
  // UpdateContext before this method is called.
  static bool UpdateContextMain(HIMC himc,
                                const InputState &next_state,
                                const commands::Output &new_output,
                                MessageQueue *message_queue);

  // Returns the target string in UTF8 for the reconversion invoked by IME.
  // Returns an empty string if the target strin is not available.
  static string GetTextForReconversionFromIME(HIMC himc);

  FRIEND_TEST(ImeCoreTest, TemporalConversionModeMessageOrderTest);
  FRIEND_TEST(ImeCoreTest, CandidateMessageOrderTest);
  FRIEND_TEST(ImeCoreTest, CompositionMessageOrderTest);

  DISALLOW_COPY_AND_ASSIGN(ImeCore);
};

}  // namespace win32
}  // namespace mozc
#endif  // MOZC_WIN32_IME_IME_CORE_H_
