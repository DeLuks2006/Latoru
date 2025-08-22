#include <iostream>
#include <unordered_map>
#include <windows.h>

HHOOK g_hook = nullptr;

enum State {
  ST_NONE,
  ST_Y,
  ST_Z,
  ST_C,
  ST_S,
  ST_SH
};

struct Rule {
  UINT vk;             // virtual key
  State needState;     // required prior state
  wchar_t lower;       // lowercase cyrillic
  wchar_t upper;       // uppercase
  State nextState;     // next state
  bool consumesPrev;   // backspace previous
};

// rule set
static const Rule rules[] = {
  { 'A', ST_NONE, L'а', L'А', ST_NONE, false },
  { 'B', ST_NONE, L'б', L'Б', ST_NONE, false },
  { 'C', ST_NONE, L'ц', L'Ц', ST_C,    false },
  { 'D', ST_NONE, L'д', L'Д', ST_NONE, false },
  { 'E', ST_NONE, L'е', L'Е', ST_NONE, false },
  { 'F', ST_NONE, L'ф', L'Ф', ST_NONE, false },
  { 'G', ST_NONE, L'г', L'Г', ST_NONE, false },
  { 'H', ST_NONE, L'х', L'Х', ST_NONE, false },
  { 'I', ST_NONE, L'и', L'И', ST_NONE, false },
  { 'J', ST_NONE, L'й', L'Й', ST_NONE, false },
  { 'K', ST_NONE, L'к', L'К', ST_NONE, false },
  { 'L', ST_NONE, L'л', L'Л', ST_NONE, false },
  { 'M', ST_NONE, L'м', L'М', ST_NONE, false },
  { 'N', ST_NONE, L'н', L'Н', ST_NONE, false },
  { 'O', ST_NONE, L'о', L'О', ST_NONE, false },
  { 'P', ST_NONE, L'п', L'П', ST_NONE, false },
  { 'R', ST_NONE, L'р', L'Р', ST_NONE, false },
  { 'S', ST_NONE, L'с', L'С', ST_S,    false },
  { 'T', ST_NONE, L'т', L'Т', ST_NONE, false },
  { 'U', ST_NONE, L'у', L'У', ST_NONE, false },
  { 'V', ST_NONE, L'в', L'В', ST_NONE, false },
  { 'Y', ST_NONE, L'ы', L'Ы', ST_Y,    false },
  { 'Z', ST_NONE, L'з', L'З', ST_Z,    false },

  // apostrophe
  { VK_OEM_7, ST_NONE, L'ь', L'ъ', ST_NONE, false },

  // digraphs
  { 'A', ST_Y, L'я', L'Я', ST_NONE, true },
  { 'E', ST_Y, L'э', L'Э', ST_NONE, true },
  { 'O', ST_Y, L'ё', L'Ё', ST_NONE, true },
  { 'U', ST_Y, L'ю', L'Ю', ST_NONE, true },
  { 'H', ST_Z, L'ж', L'Ж', ST_NONE, true },
  { 'H', ST_C, L'ч', L'Ч', ST_NONE, true },
  { 'H', ST_S, L'ш', L'Ш', ST_SH,   true },
  { 'H', ST_SH, L'щ', L'Щ', ST_NONE, true },
};

inline bool IsUpper() {
  return GetKeyState(VK_SHIFT) & 0x8000 || GetKeyState(VK_CAPITAL) & 1;
}

void SendUnicodeKey(const wchar_t ch) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wScan = ch;
  input.ki.dwFlags = KEYEVENTF_UNICODE;
  SendInput(1, &input, sizeof(INPUT));
  input.ki.dwFlags |= KEYEVENTF_KEYUP;
  SendInput(1, &input, sizeof(INPUT));
}

void SendBackspace() {
  INPUT ip = {0};
  ip.type = INPUT_KEYBOARD;
  ip.ki.wVk = VK_BACK;
  SendInput(1, &ip, sizeof(INPUT));
  ip.ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, &ip, sizeof(INPUT));
}

State g_state = ST_NONE;

void ProcessKey(UINT vk) {
  bool upper = (GetKeyState(VK_SHIFT) & 0x8000) || (GetKeyState(VK_CAPITAL) & 1);

  for (auto& r : rules) {
    if (r.vk == vk && r.needState == g_state) {
      if (r.consumesPrev) SendBackspace();
      SendUnicodeKey(upper ? r.upper : r.lower);
      g_state = r.nextState;
      return;
    }
  }
  g_state = ST_NONE;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
    auto* kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    if (kbd->vkCode == VK_ESCAPE) {
      PostQuitMessage(0);
      return 1;
    }
    if (kbd->vkCode == VK_BACK) {
      g_state = ST_NONE;
      return CallNextHookEx(g_hook, nCode, wParam, lParam);
    }

    bool handled = false;
    bool upper = (GetKeyState(VK_SHIFT) & 0x8000) || (GetKeyState(VK_CAPITAL) & 1);

    for (auto& r : rules) {
      if (r.vk == kbd->vkCode && r.needState == g_state) {
        if (r.consumesPrev) SendBackspace();
        SendUnicodeKey(upper ? r.upper : r.lower);
        g_state = r.nextState;
        handled = true;
        break;
      }
    }

    if (!handled) {
      g_state = ST_NONE;
      return CallNextHookEx(g_hook, nCode, wParam, lParam); // allow normal key
    }

    return 1; // block original key if handled
  }

  return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

int main() {
  std::cout << "[LATORU] Started..." << std::endl;
  g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
  if (!g_hook) return 1;
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  UnhookWindowsHookEx(g_hook);
  return 0;
}