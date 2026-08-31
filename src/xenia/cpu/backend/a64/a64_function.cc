/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2026 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/cpu/backend/a64/a64_function.h"

#include <atomic>

#include "xenia/base/logging.h"
#include "xenia/base/platform.h"
#include "xenia/cpu/backend/a64/a64_backend.h"
#include "xenia/cpu/processor.h"
#include "xenia/cpu/thread_state.h"

namespace xe {
namespace cpu {
namespace backend {
namespace a64 {

#if XE_PLATFORM_IOS && XE_ARCH_ARM64
namespace {
constexpr uint32_t kIOSA64CallLogLimit = 32;
std::atomic<uint32_t> ios_a64_call_log_count{0};

bool BeginIOSA64CallLog(uint32_t& log_index) {
  log_index = ios_a64_call_log_count.fetch_add(1, std::memory_order_relaxed);
  if (log_index < kIOSA64CallLogLimit) {
    return true;
  }
  if (log_index == kIOSA64CallLogLimit) {
    XELOGI("iOS A64: suppressing further host-to-guest call logs");
  }
  return false;
}
}  // namespace
#endif  // XE_PLATFORM_IOS && XE_ARCH_ARM64

A64Function::A64Function(Module* module, uint32_t address)
    : GuestFunction(module, address) {}

A64Function::~A64Function() {
  // machine_code_ is freed by code cache.
}

void A64Function::Setup(uint8_t* machine_code, size_t machine_code_length) {
  machine_code_length_.store(machine_code_length, std::memory_order_relaxed);
  machine_code_.store(machine_code, std::memory_order_release);
}

bool A64Function::CallImpl(ThreadState* thread_state, uint32_t return_address) {
  auto backend =
      reinterpret_cast<A64Backend*>(thread_state->processor()->backend());
  auto thunk = backend->host_to_guest_thunk();
  auto* code = machine_code_.load(std::memory_order_acquire);
  if (!thunk || !code) {
#if XE_PLATFORM_IOS && XE_ARCH_ARM64
    XELOGE(
        "iOS A64: missing host-to-guest target guest={:08X} "
        "thunk={:016X} code={:p}",
        address(), reinterpret_cast<uintptr_t>(thunk),
        static_cast<const void*>(code));
#endif  // XE_PLATFORM_IOS && XE_ARCH_ARM64
    return false;
  }

#if XE_PLATFORM_IOS && XE_ARCH_ARM64
  uint32_t ios_log_index = 0;
  const bool ios_log = BeginIOSA64CallLog(ios_log_index);
  if (ios_log) {
    auto* context = thread_state->context();
    XELOGI(
        "iOS A64: call[{}] enter guest={:08X}-{:08X} code={:p} "
        "ret={:08X} thid={:08X} r1={:08X} lr={:08X}",
        ios_log_index, address(), end_address(), static_cast<const void*>(code),
        return_address, thread_state->thread_id(),
        context ? uint32_t(context->r[1]) : 0,
        context ? uint32_t(context->lr) : 0);
  }
#endif  // XE_PLATFORM_IOS && XE_ARCH_ARM64

  thunk(code, thread_state->context(),
        reinterpret_cast<void*>(uintptr_t(return_address)));

#if XE_PLATFORM_IOS && XE_ARCH_ARM64
  if (ios_log) {
    auto* context = thread_state->context();
    XELOGI(
        "iOS A64: call[{}] return guest={:08X} thid={:08X} "
        "r3={:016X} r1={:08X} lr={:08X}",
        ios_log_index, address(), thread_state->thread_id(),
        context ? uint64_t(context->r[3]) : 0,
        context ? uint32_t(context->r[1]) : 0,
        context ? uint32_t(context->lr) : 0);
  }
#endif  // XE_PLATFORM_IOS && XE_ARCH_ARM64

  return true;
}

}  // namespace a64
}  // namespace backend
}  // namespace cpu
}  // namespace xe
