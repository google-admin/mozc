// Copyright 2010-2021, Google Inc.
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

#ifndef THIRD_PARTY_MOZC_SRC_WIN32_TIP_TIP_UI_ELEMENT_CONVENTIONAL_H_
#define THIRD_PARTY_MOZC_SRC_WIN32_TIP_TIP_UI_ELEMENT_CONVENTIONAL_H_

#include <msctf.h>
#include <wil/com.h>

#include "win32/tip/tip_text_service.h"

namespace mozc {
namespace win32 {
namespace tsf {

class TipUiElementConventional {
 public:
  enum UIType {
    // The UI element is a suggest window whose contents is not observable
    // from the application. No candidate UI notifications will not be
    // generated.
    kUnobservableSuggestWindow,
    // The UI element is a suggest window whose contents is observable from
    // the application. Candidate UI notifications will be generated as if the
    // window is a candidate window.
    kObservableSuggestWindow,
    // The UI element is a candidate window whose contents is observable from
    // the application. Candidate UI notifications will be generated.
    kCandidateWindow,
    // The UI element is a mode indicator window whose contents is observable
    // from the application. No candidate UI notifications will be generated
    // because this is not a candidate window.
    KIndicatorWindow,
  };

  TipUiElementConventional() = delete;
  TipUiElementConventional(const TipUiElementConventional &) = delete;
  TipUiElementConventional &operator=(const TipUiElementConventional &) =
      delete;

  // Returns a ITfUIElement object based of the given parameters.
  static wil::com_ptr_nothrow<ITfUIElement> New(
      UIType type, wil::com_ptr_nothrow<TipTextService> text_service,
      wil::com_ptr_nothrow<ITfContext> context);
};

}  // namespace tsf
}  // namespace win32
}  // namespace mozc

#endif  // THIRD_PARTY_MOZC_SRC_WIN32_TIP_TIP_UI_ELEMENT_CONVENTIONAL_H_
