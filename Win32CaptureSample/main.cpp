#include "pch.h"
#include "SimpleCapture.h"
#include "App.h"
#include "SampleWindow.h"

using namespace winrt;
using namespace Windows::Graphics::Capture;
using namespace Windows::UI::Composition;

int __stdcall WinMain(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    LPSTR     cmdLine,
    int       cmdShow)
{
    // Initialize COM
    init_apartment(apartment_type::single_threaded);

    // Check to see that capture is supported
    auto isCaptureSupported = winrt::Windows::Graphics::Capture::GraphicsCaptureSession::IsSupported();
    if (!isCaptureSupported)
    {
        MessageBox(
            NULL,
            L"Screen capture is not supported on this device for this release of Windows!",
            L"Win32CaptureSample",
            MB_OK | MB_ICONERROR);

        return 1;
    }

    // Create a DispatcherQueue for our thread
    auto controller = CreateDispatcherQueueControllerForCurrentThread();

    // Initialize Composition
    auto compositor = Compositor();
    auto root = compositor.CreateContainerVisual();
    root.RelativeSizeAdjustment({ 1.0f, 1.0f });
    root.Size({ -220.0f, 0.0f });
    root.Offset({ 220.0f, 0.0f, 0.0f });

    // Create the picker
    auto picker = GraphicsCapturePicker();

    // Create the app
    auto app = std::make_shared<App>(root, picker);

    // Create the window
    auto window = SampleWindow(instance, cmdShow, app);

    // Initialize the picker with our window
    window.Initialize(picker);

    // Hookup the visual tree to the window
    auto target = window.CreateWindowTarget(compositor);
    target.Root(root);

    // Message pump
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}