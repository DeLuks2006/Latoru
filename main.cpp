#include <iostream>
#include <unordered_map>
#include <windows.h>

// TODO: ы ь and ъ don't work..

HHOOK g_hook = nullptr;
BOOL xYPressed = FALSE;
BOOL xZPressed = FALSE;
BOOL xCPressed = FALSE;
BOOL xIPressed = FALSE;

std::unordered_map<WPARAM, std::pair<wchar_t, wchar_t>> keymap = {
  {'B', {L'Б', L'б'}}, {'C', {L'Ц', L'ц'}},
  {'D', {L'Д', L'д'}}, {'F', {L'Ф', L'ф'}},
  {'G', {L'Г', L'г'}}, {'H', {L'Х', L'х'}},
  {'J', {L'Й', L'й'}}, {'K', {L'К', L'к'}},
  {'L', {L'Л', L'л'}}, {'M', {L'М', L'м'}},
  {'N', {L'Н', L'н'}}, {'P', {L'П', L'п'}},
  {'R', {L'Р', L'р'}}, {'S', {L'С', L'с'}},
  {'T', {L'Т', L'т'}}, {'U', {L'У', L'у'}},
  {'V', {L'В', L'в'}}, {'Y', {L'Ы', L'ы'}},
};

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
VOID SendUnicodeKey(wchar_t ch);

int main() {
  std::cout << "[LATORU] Started... Enjoy writing in cyrillic!" << std::endl;

  g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
  if (!g_hook) {
    std::cerr << "[LATORU] Failed to initialize hook." << std::endl;
    return 1;
  }
  std::cout << "[LATORU] To quit, please press the [ESC] key." << std::endl;
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  UnhookWindowsHookEx(g_hook);
  std::cout << "[LATORU] Goodbye!" << std::endl;
  return 0;
}

// Sends a Unicode character properly
VOID SendUnicodeKey(const wchar_t ch) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wScan = ch;
  input.ki.dwFlags = KEYEVENTF_UNICODE; // Send Unicode instead of virtual key
  SendInput(1, &input, sizeof(INPUT));

  input.ki.dwFlags |= KEYEVENTF_KEYUP; // Release the key
  SendInput(1, &input, sizeof(INPUT));
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0) {
    KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;

    if (kbd->vkCode == VK_ESCAPE) {
      PostQuitMessage(0);
    }
    BOOL isUpper = (GetKeyState(VK_SHIFT) & 0x8000) || (GetKeyState(VK_CAPITAL) & 0x0001);
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
      switch (kbd->vkCode) {
      case 'A':
      case 'a':
        if (isUpper) {
          SendUnicodeKey(xYPressed ? L'Я' : L'А');
        }
        else {
          SendUnicodeKey(xYPressed ? L'я' : L'а');
        }
        xYPressed = FALSE;
        return 1;
      case 'E':
      case 'e':
        if (isUpper) {
          SendUnicodeKey(xYPressed ? L'Е' : L'Э');
        }
        else {
          SendUnicodeKey(xYPressed ? L'е' : L'э');
        }
        xYPressed = FALSE;
        return 1;
      case 'Z':
      case 'z':
        if (xZPressed) {
          SendUnicodeKey(isUpper ? L'З' : L'з');
          xZPressed = FALSE;
          return 1;
        }
        xZPressed = TRUE;
        return 1;
      case 'I':
      case 'i':
        if (xIPressed) {
          SendUnicodeKey(isUpper ? L'И' : L'и');
          xIPressed = FALSE;
          return 1;
        }
        xIPressed = TRUE;
        return 1;
      case 'O':
      case 'o':
        if (isUpper) {
          SendUnicodeKey(xYPressed ? L'Ё' : L'О');
        }
        else {
          SendUnicodeKey(xYPressed ? L'ё' : L'о');
        }
        xYPressed = FALSE;
        return 1;
      case 'U':
      case 'u':
        if (isUpper) {
          SendUnicodeKey(xYPressed ? L'Ю' : L'У');
        }
        else {
          SendUnicodeKey(xYPressed ? L'ю' : L'у');
        }
        xYPressed = FALSE;
        return 1;
      case 'H':
      case 'h':
        if (isUpper) {
          SendUnicodeKey(xZPressed ? L'Ж' : xCPressed ? L'Ч' : L'Х');
        }
        else {
          SendUnicodeKey(xZPressed ? L'ж' : xCPressed ? L'ч' : L'х');
        }
        xZPressed = FALSE;
        xCPressed = FALSE;
        return 1;
      case 'C':
      case 'c':
        if (xCPressed) {
          SendUnicodeKey(isUpper ? L'Ц' : L'ц');
          xCPressed = FALSE;
        }
        xCPressed = TRUE;
        return 1;
      case VK_BACK:
        if (xYPressed) {
          xYPressed = FALSE;
          return 1;
        }
        if (xZPressed) {
          xZPressed = FALSE;
          return 1;
        }
        break;
      case 'Y':
      case 'y':
        xYPressed = TRUE;
        return 1;
      case '\'':
        SendUnicodeKey(xIPressed ? L'ы' : L'ь');
        xIPressed = FALSE;
        return 1;
      case '#':
        SendUnicodeKey(L'ъ');
      default:
        auto it = keymap.find(kbd->vkCode);
        if (it != keymap.end()) {
          SendUnicodeKey(isUpper ? it->second.first : it->second.second);
          return 1;
        }
        xYPressed = FALSE; // Reset if any other key is pressed
        xZPressed = FALSE;
        xCPressed = FALSE;
        xIPressed = FALSE;
        break;
      }
    }
  }
  return CallNextHookEx(g_hook, nCode, wParam, lParam);
}
