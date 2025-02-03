#include <iostream>
#include <windows.h>

// TODO: ы ь and ъ don't work.. also EVERYTHING IS CAPSLOCK
// TODO: maybe also find a smarter way to do this instead of a giant switch-case...

HHOOK g_hook = nullptr;
BOOL xYPressed = FALSE;
BOOL xZPressed = FALSE;
BOOL xCPressed = FALSE;
BOOL xIPressed = FALSE;

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

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
      switch (kbd->vkCode) {
      case 'A':
        SendUnicodeKey(xYPressed ? L'Я' : L'А');
        xYPressed = FALSE;
        return 1;
      case 'a':
        SendUnicodeKey(xYPressed ? L'я' : L'а');
        xYPressed = FALSE;
        return 1;
      case 'B':
        SendUnicodeKey(L'Б');
        return 1;
      case 'b':
        SendUnicodeKey(L'б');
        return 1;
      case 'V':
        SendUnicodeKey(L'В');
        return 1;
      case 'v':
        SendUnicodeKey(L'в');
        return 1;
      case 'G':
        SendUnicodeKey(L'Г');
        return 1;
      case 'g':
        SendUnicodeKey(L'г');
        return 1;
      case 'D':
        SendUnicodeKey(L'Д');
        return 1;
      case 'd':
        SendUnicodeKey(L'д');
        return 1;
      case 'E':
        SendUnicodeKey(xYPressed ? L'Е' : L'Э');
        xYPressed = FALSE;
        return 1;
      case 'e':
        SendUnicodeKey(xYPressed ? L'е' : L'э');
        xYPressed = FALSE;
        return 1;
      case 'Z':
        if (xZPressed) {
          SendUnicodeKey(L'З');
          xZPressed = FALSE;
          return 1;
        }
        xZPressed = TRUE;
        return 1;
      case 'z':
        if (xZPressed) {
          SendUnicodeKey(L'з');
          xZPressed = FALSE;
          return 1;
        }
        xZPressed = TRUE;
        return 1;
      case 'I':
        if (xIPressed) {
          SendUnicodeKey(L'И');
          xIPressed = FALSE;
          return 1;
        }
        xIPressed = TRUE;
        return 1;
      case 'i':
        if (xIPressed) {
          SendUnicodeKey(L'и');
          xIPressed = FALSE;
          return 1;
        }
        xIPressed = TRUE;
        return 1;
      case 'J':
        SendUnicodeKey(L'Й');
        return 1;
      case 'j':
        SendUnicodeKey(L'й');
        return 1;
      case 'K':
        SendUnicodeKey(L'К');
        return 1;
      case 'k':
        SendUnicodeKey(L'к');
        return 1;
      case 'L':
        SendUnicodeKey(L'Л');
        return 1;
      case 'l':
        SendUnicodeKey(L'л');
        return 1;
      case 'M':
        SendUnicodeKey(L'М');
        return 1;
      case 'm':
        SendUnicodeKey(L'м');
        return 1;
      case 'N':
        SendUnicodeKey(L'Н');
        return 1;
      case 'n':
        SendUnicodeKey(L'н');
        return 1;
      case 'O':
        SendUnicodeKey(xYPressed ? L'Ё' : L'О');
        xYPressed = FALSE;
        return 1;
      case 'o':
        SendUnicodeKey(xYPressed ? L'ё' : L'о');
        xYPressed = FALSE;
        return 1;
      case 'P':
        SendUnicodeKey(L'П');
        return 1;
      case 'p':
        SendUnicodeKey(L'п');
        return 1;
      case 'R':
        SendUnicodeKey(L'Р');
        return 1;
      case 'r':
        SendUnicodeKey(L'р');
        return 1;
      case 'S':
        SendUnicodeKey(L'С');
        return 1;
      case 's':
        SendUnicodeKey(L'с');
        return 1;
      case 'T':
        SendUnicodeKey(L'Т');
        return 1;
      case 't':
        SendUnicodeKey(L'т');
        return 1;
      case 'U':
        SendUnicodeKey(xYPressed ? L'Ю' : L'У');
        xYPressed = FALSE;
        return 1;
      case 'u':
        SendUnicodeKey(xYPressed ? L'ю' : L'у');
        xYPressed = FALSE;
        return 1;
      case 'F':
        SendUnicodeKey(L'Ф');
        return 1;
      case 'f':
        SendUnicodeKey(L'ф');
        return 1;
      case 'H':
        SendUnicodeKey(xZPressed ? L'Ж' : xCPressed ? L'Ч' : L'Х');
        xZPressed = FALSE;
        xCPressed = FALSE;
        return 1;
      case 'h':
        SendUnicodeKey(xZPressed ? L'ж' : xCPressed ? L'ч' : L'х');
        xZPressed = FALSE;
        xCPressed = FALSE;
        return 1;
      case 'C':
        if (xCPressed) {
          SendUnicodeKey(L'Ц');
          xCPressed = FALSE;
          return 1;
        }
        xCPressed = TRUE;
        return 1;
      case 'c':
        SendUnicodeKey(L'ц');
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
