#include "pch.h"
#include "App.h"
#include "SampleWindow.h"

#include <CommCtrl.h>

using namespace winrt;
using namespace Windows::Graphics::Capture;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Composition::Desktop;

SampleWindow::SampleWindow(
    HINSTANCE instance,
    int cmdShow,
    std::shared_ptr<App> app)
{
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"Win32CaptureSample";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    WINRT_VERIFY(RegisterClassEx(&wcex));
    WINRT_ASSERT(!m_window);

    WINRT_VERIFY(CreateWindow(
        L"Win32CaptureSample",
        L"Win32CaptureSample",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        NULL,
        NULL,
        instance,
        this));

    WINRT_ASSERT(m_window);

    ShowWindow(m_window, cmdShow);
    UpdateWindow(m_window);

    m_app = app;
    m_windows = EnumerationWindow::EnumerateAllWindows();

    CreateControls(instance);
}

LRESULT SampleWindow::MessageHandler(UINT const message, WPARAM const wparam, LPARAM const lparam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            auto command = HIWORD(wparam);
            auto hwnd = (HWND)lparam;
            switch (command)
            {
            case CBN_SELCHANGE:
                {
                    auto index = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
                    if (hwnd == m_windowComboBoxHwnd)
                    {
                        auto window = m_windows[index];
                        m_app->StartCaptureFromWindowHandle(window.Hwnd());
                    }
                }
                break;
            case BN_CLICKED:
                {
                    auto ignored = m_app->StartCaptureWithPickerAsync();
                }
                break;
            }
        }
        break;
    default:
        return base_type::MessageHandler(message, wparam, lparam);
        break;
    }

    return 0;
}

void SampleWindow::CreateControls(HINSTANCE instance)
{
    // Create window combo box
    HWND windowComboBoxHwnd = CreateWindow(
        WC_COMBOBOX,
        L"",
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        10,
        10,
        200,
        200,
        m_window,
        NULL,
        instance,
        NULL);
    WINRT_VERIFY(windowComboBoxHwnd);

    // Populate combo box
    for (auto& window : m_windows)
    {
        SendMessage(windowComboBoxHwnd, CB_ADDSTRING, 0, (LPARAM)window.Title().c_str());
    }

    // Create button
    HWND buttonHwnd = CreateWindow(
        WC_BUTTON,
        L"Use Picker",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10,
        40,
        200,
        30,
        m_window,
        NULL,
        instance,
        NULL);
    WINRT_VERIFY(buttonHwnd);

    m_windowComboBoxHwnd = windowComboBoxHwnd;
    m_buttonHwnd = buttonHwnd;
}

