// dear imgui, v1.73
// (demo code)

// Message to the person tempted to delete this file when integrating Dear ImGui into their code base:
// Do NOT remove this file from your project! Think again! It is the most useful reference code that you and other coders
// will want to refer to and call. Have the ImGui::ShowDemoWindow() function wired in an always-available debug menu of
// your game/app! Removing this file from your project is hindering access to documentation for everyone in your team,
// likely leading you to poorer usage of the library.
// Everything in this file will be stripped out by the linker if you don't call ImGui::ShowDemoWindow().
// If you want to link core Dear ImGui in your shipped builds but want an easy guarantee that the demo will not be linked,
// you can setup your imconfig.h with #define IMGUI_DISABLE_DEMO_WINDOWS and those functions will be empty.
// In other situation, whenever you have Dear ImGui available you probably want this to be available for reference.
// Thank you,
// -Your beloved friend, imgui_demo.cpp (that you won't delete)

// Message to beginner C/C++ programmers about the meaning of the 'static' keyword:
// In this demo code, we frequently we use 'static' variables inside functions. A static variable persist across calls, so it is
// essentially like a global variable but declared inside the scope of the function. We do this as a way to gather code and data
// in the same place, to make the demo source code faster to read, faster to write, and smaller in size.
// It also happens to be a convenient way of storing simple UI related information as long as your function doesn't need to be
// reentrant or used in multiple threads. This might be a pattern you will want to use in your code, but most of the real data
// you would be editing is likely going to be stored outside your functions.

// The Demo code is this file is designed to be easy to copy-and-paste in into your application!
// Because of this:
// - We never omit the ImGui:: namespace when calling functions, even though most of our code is already in the same namespace.
// - We try to declare static variables in the local scope, as close as possible to the code using them.
// - We never use any of the helpers/facilities used internally by dear imgui, unless it has been exposed in the public API (imgui.h).
// - We never use maths operators on ImVec2/ImVec4. For other imgui sources files, they are provided by imgui_internal.h w/ IMGUI_DEFINE_MATH_OPERATORS,
//   for your own sources file they are optional and require you either enable those, either provide your own via IM_VEC2_CLASS_EXTRA in imconfig.h.
//   Because we don't want to assume anything about your support of maths operators, we don't use them in imgui_demo.cpp.

/*

Index of this file:

// [SECTION] Forward Declarations, Helpers
// [SECTION] Demo Window / ShowDemoWindow()
// [SECTION] About Window / ShowAboutWindow()
// [SECTION] Style Editor / ShowStyleEditor()
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
// [SECTION] Example App: Simple Overlay / ShowExampleAppSimpleOverlay()
// [SECTION] Example App: Manipulating Window Titles / ShowExampleAppWindowTitles()
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wold-style-cast"             // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"    // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"   // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored "-Wformat-security"            // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"      // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wunused-macros"              // warning : warning: macro is not used                         // we define snprintf/vsnprintf on Windows so they are available, but not always used.
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning : zero as null pointer constant                  // some standard header variations use #define NULL 0
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"           // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#endif
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"          // warning : macro name is a reserved identifier                //
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"                      // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"          // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"              // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"             // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"                   // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wmisleading-indentation"       // [__GNUC__ >= 6] warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#endif

// Play it nice with Windows users. Notepad in 2017 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#define snprintf    _snprintf
#define vsnprintf   _vsnprintf
#else
#define IM_NEWLINE  "\n"
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations, Helpers
//-----------------------------------------------------------------------------

#if !defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS) && defined(IMGUI_DISABLE_TEST_WINDOWS) && !defined(IMGUI_DISABLE_DEMO_WINDOWS)   // Obsolete name since 1.53, TEST->DEMO
#define IMGUI_DISABLE_DEMO_WINDOWS
#endif

#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)

// Forward Declarations
static void ShowExampleAppDocuments(bool* p_open);
static void ShowExampleAppMainMenuBar();
static void ShowExampleAppConsole(bool* p_open);
static void ShowExampleAppLog(bool* p_open);
static void ShowExampleAppLayout(bool* p_open);
static void ShowExampleAppPropertyEditor(bool* p_open);
static void ShowExampleAppLongText(bool* p_open);
static void ShowExampleAppAutoResize(bool* p_open);
static void ShowExampleAppConstrainedResize(bool* p_open);
static void ShowExampleAppSimpleOverlay(bool* p_open);
static void ShowExampleAppWindowTitles(bool* p_open);
static void ShowExampleAppCustomRendering(bool* p_open);
static void ShowExampleMenuFile();

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see misc/fonts/README.txt)
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

// Helper to display basic user controls.
void ImGui::ShowUserGuide()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::BulletText("Double-click on title bar to collapse window.");
    ImGui::BulletText("Click and drag on lower corner to resize window\n(double-click to auto fit window to its contents).");
    if (io.ConfigWindowsMoveFromTitleBarOnly)
        ImGui::BulletText("Click and drag on title bar to move window.");
    else
        ImGui::BulletText("Click and drag on any empty space to move window.");
    ImGui::BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields.");
    ImGui::BulletText("CTRL+Click on a slider or drag box to input value as text.");
    if (io.FontAllowUserScaling)
        ImGui::BulletText("CTRL+Mouse Wheel to zoom window contents.");
    ImGui::BulletText("Mouse Wheel to scroll.");
    ImGui::BulletText("While editing text:\n");
    ImGui::Indent();
    ImGui::BulletText("Hold SHIFT or use mouse to select text.");
    ImGui::BulletText("CTRL+Left/Right to word jump.");
    ImGui::BulletText("CTRL+A or double-click to select all.");
    ImGui::BulletText("CTRL+X,CTRL+C,CTRL+V to use clipboard.");
    ImGui::BulletText("CTRL+Z,CTRL+Y to undo/redo.");
    ImGui::BulletText("ESCAPE to revert.");
    ImGui::BulletText("You can apply arithmetic operators +,*,/ on numerical values.\nUse +- to subtract.");
    ImGui::Unindent();
}

//-----------------------------------------------------------------------------
// [SECTION] Demo Window / ShowDemoWindow()
//-----------------------------------------------------------------------------
// - ShowDemoWindowWidgets()
// - ShowDemoWindowLayout()
// - ShowDemoWindowPopups()
// - ShowDemoWindowColumns()
// - ShowDemoWindowMisc()
//-----------------------------------------------------------------------------

// We split the contents of the big ShowDemoWindow() function into smaller functions (because the link time of very large functions grow non-linearly)
static void ShowDemoWindowWidgets();
static void ShowDemoWindowLayout();
static void ShowDemoWindowPopups();
static void ShowDemoWindowColumns();
static void ShowDemoWindowMisc();

// Demonstrate most Dear ImGui features (this is big function!)
// You may execute this function to experiment with the UI and understand what it does. You may then search for keywords in the code when you are interested by a specific feature.
void ImGui::ShowDemoWindow(bool* p_open)
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!"); // Exceptionally add an extra assert here for people confused with initial dear imgui setup

    // Examples Apps (accessible from the "Examples" menu)
    static bool show_app_documents = false;
    static bool show_app_main_menu_bar = false;
    static bool show_app_console = false;
    static bool show_app_log = false;
    static bool show_app_layout = false;
    static bool show_app_property_editor = false;
    static bool show_app_long_text = false;
    static bool show_app_auto_resize = false;
    static bool show_app_constrained_resize = false;
    static bool show_app_simple_overlay = false;
    static bool show_app_window_titles = false;
    static bool show_app_custom_rendering = false;

    if (show_app_documents)           ShowExampleAppDocuments(&show_app_documents);
    if (show_app_main_menu_bar)       ShowExampleAppMainMenuBar();
    if (show_app_console)             ShowExampleAppConsole(&show_app_console);
    if (show_app_log)                 ShowExampleAppLog(&show_app_log);
    if (show_app_layout)              ShowExampleAppLayout(&show_app_layout);
    if (show_app_property_editor)     ShowExampleAppPropertyEditor(&show_app_property_editor);
    if (show_app_long_text)           ShowExampleAppLongText(&show_app_long_text);
    if (show_app_auto_resize)         ShowExampleAppAutoResize(&show_app_auto_resize);
    if (show_app_constrained_resize)  ShowExampleAppConstrainedResize(&show_app_constrained_resize);
   /* if (show_app_simple_overlay)  */    ShowExampleAppSimpleOverlay(&show_app_simple_overlay);
    if (show_app_window_titles)       ShowExampleAppWindowTitles(&show_app_window_titles);
    if (show_app_custom_rendering)    ShowExampleAppCustomRendering(&show_app_custom_rendering);

    // Dear ImGui Apps (accessible from the "Tools" menu)
    static bool show_app_metrics = false;
    static bool show_app_style_editor = false;
    static bool show_app_about = false;

    if (show_app_metrics)             { ImGui::ShowMetricsWindow(&show_app_metrics); }
    if (show_app_style_editor)        { ImGui::Begin("Style Editor", &show_app_style_editor); ImGui::ShowStyleEditor(); ImGui::End(); }
    if (show_app_about)               { ImGui::ShowAboutWindow(&show_app_about); }

    // Demonstrate the various window flags. Typically you would just use the default!
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = false;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

    // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Dear ImGui Demo", p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    // Most "big" widgets share a common width settings by default.
    //ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // Use 2/3 of the space for widgets and 1/3 for labels (default)
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);           // Use fixed width for labels (by passing a negative value), the rest goes to widgets. We choose a width proportional to our font size.

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Examples"))
        {
            ImGui::MenuItem("Main menu bar", NULL, &show_app_main_menu_bar);
            ImGui::MenuItem("Console", NULL, &show_app_console);
            ImGui::MenuItem("Log", NULL, &show_app_log);
            ImGui::MenuItem("Simple layout", NULL, &show_app_layout);
            ImGui::MenuItem("Property editor", NULL, &show_app_property_editor);
            ImGui::MenuItem("Long text display", NULL, &show_app_long_text);
            ImGui::MenuItem("Auto-resizing window", NULL, &show_app_auto_resize);
            ImGui::MenuItem("Constrained-resizing window", NULL, &show_app_constrained_resize);
            ImGui::MenuItem("Simple overlay", NULL, &show_app_simple_overlay);
            ImGui::MenuItem("Manipulating window titles", NULL, &show_app_window_titles);
            ImGui::MenuItem("Custom rendering", NULL, &show_app_custom_rendering);
            ImGui::MenuItem("Documents", NULL, &show_app_documents);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Metrics", NULL, &show_app_metrics);
            ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
            ImGui::MenuItem("About Dear ImGui", NULL, &show_app_about);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("PROGRAMMER GUIDE:");
        ImGui::BulletText("Please see the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
        ImGui::BulletText("Please see the comments in imgui.cpp.");
        ImGui::BulletText("Please see the examples/ application.");
        ImGui::BulletText("Enable 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
        ImGui::BulletText("Enable 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");
        ImGui::Separator();

        ImGui::Text("USER GUIDE:");
        ImGui::ShowUserGuide();
    }

    if (ImGui::CollapsingHeader("Configuration"))
    {
        ImGuiIO& io = ImGui::GetIO();

        if (ImGui::TreeNode("Configuration##2"))
        {
            ImGui::CheckboxFlags("io.ConfigFlags: NavEnableKeyboard", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
            ImGui::CheckboxFlags("io.ConfigFlags: NavEnableGamepad", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NavEnableGamepad);
            ImGui::SameLine(); HelpMarker("Required back-end to feed in gamepad inputs in io.NavInputs[] and set io.BackendFlags |= ImGuiBackendFlags_HasGamepad.\n\nRead instructions in imgui.cpp for details.");
            ImGui::CheckboxFlags("io.ConfigFlags: NavEnableSetMousePos", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NavEnableSetMousePos);
            ImGui::SameLine(); HelpMarker("Instruct navigation to move the mouse cursor. See comment for ImGuiConfigFlags_NavEnableSetMousePos.");
            ImGui::CheckboxFlags("io.ConfigFlags: NoMouse", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NoMouse);
            if (io.ConfigFlags & ImGuiConfigFlags_NoMouse) // Create a way to restore this flag otherwise we could be stuck completely!
            {
                if (fmodf((float)ImGui::GetTime(), 0.40f) < 0.20f)
                {
                    ImGui::SameLine();
                    ImGui::Text("<<PRESS SPACE TO DISABLE>>");
                }
                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)))
                    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            }
            ImGui::CheckboxFlags("io.ConfigFlags: NoMouseCursorChange", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NoMouseCursorChange);
            ImGui::SameLine(); HelpMarker("Instruct back-end to not alter mouse cursor shape and visibility.");
            ImGui::Checkbox("io.ConfigInputTextCursorBlink", &io.ConfigInputTextCursorBlink);
            ImGui::SameLine(); HelpMarker("Set to false to disable blinking cursor, for users who consider it distracting");
            ImGui::Checkbox("io.ConfigWindowsResizeFromEdges", &io.ConfigWindowsResizeFromEdges);
            ImGui::SameLine(); HelpMarker("Enable resizing of windows from their edges and from the lower-left corner.\nThis requires (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors) because it needs mouse cursor feedback.");
            ImGui::Checkbox("io.ConfigWindowsMoveFromTitleBarOnly", &io.ConfigWindowsMoveFromTitleBarOnly);
            ImGui::Checkbox("io.MouseDrawCursor", &io.MouseDrawCursor);
            ImGui::SameLine(); HelpMarker("Instruct Dear ImGui to render a mouse cursor for you. Note that a mouse cursor rendered via your application GPU rendering path will feel more laggy than hardware cursor, but will be more in sync with your other visuals.\n\nSome desktop applications may use both kinds of cursors (e.g. enable software cursor only when resizing/dragging something).");
            ImGui::TreePop();
            ImGui::Separator();
        }

        if (ImGui::TreeNode("Backend Flags"))
        {
            HelpMarker("Those flags are set by the back-ends (imgui_impl_xxx files) to specify their capabilities.");
            ImGuiBackendFlags backend_flags = io.BackendFlags; // Make a local copy to avoid modifying actual back-end flags.
            ImGui::CheckboxFlags("io.BackendFlags: HasGamepad", (unsigned int *)&backend_flags, ImGuiBackendFlags_HasGamepad);
            ImGui::CheckboxFlags("io.BackendFlags: HasMouseCursors", (unsigned int *)&backend_flags, ImGuiBackendFlags_HasMouseCursors);
            ImGui::CheckboxFlags("io.BackendFlags: HasSetMousePos", (unsigned int *)&backend_flags, ImGuiBackendFlags_HasSetMousePos);
            ImGui::CheckboxFlags("io.BackendFlags: RendererHasVtxOffset", (unsigned int *)&backend_flags, ImGuiBackendFlags_RendererHasVtxOffset);
            ImGui::TreePop();
            ImGui::Separator();
        }

        if (ImGui::TreeNode("Style"))
        {
            ImGui::ShowStyleEditor();
            ImGui::TreePop();
            ImGui::Separator();
        }

        if (ImGui::TreeNode("Capture/Logging"))
        {
            ImGui::TextWrapped("The logging API redirects all text output so you can easily capture the content of a window or a block. Tree nodes can be automatically expanded.");
            HelpMarker("Try opening any of the contents below in this window and then click one of the \"Log To\" button.");
            ImGui::LogButtons();
            ImGui::TextWrapped("You can also call ImGui::LogText() to output directly to the log without a visual output.");
            if (ImGui::Button("Copy \"Hello, world!\" to clipboard"))
            {
                ImGui::LogToClipboard();
                ImGui::LogText("Hello, world!");
                ImGui::LogFinish();
            }
            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader("Window options"))
    {
        ImGui::Checkbox("No titlebar", &no_titlebar); ImGui::SameLine(150);
        ImGui::Checkbox("No scrollbar", &no_scrollbar); ImGui::SameLine(300);
        ImGui::Checkbox("No menu", &no_menu);
        ImGui::Checkbox("No move", &no_move); ImGui::SameLine(150);
        ImGui::Checkbox("No resize", &no_resize); ImGui::SameLine(300);
        ImGui::Checkbox("No collapse", &no_collapse);
        ImGui::Checkbox("No close", &no_close); ImGui::SameLine(150);
        ImGui::Checkbox("No nav", &no_nav); ImGui::SameLine(300);
        ImGui::Checkbox("No background", &no_background);
        ImGui::Checkbox("No bring to front", &no_bring_to_front);
    }

    // All demo contents
    ShowDemoWindowWidgets();
    ShowDemoWindowLayout();
    ShowDemoWindowPopups();
    ShowDemoWindowColumns();
    ShowDemoWindowMisc();

    // End of ShowDemoWindow()
    ImGui::End();
}

static void ShowDemoWindowWidgets()
{
    if (!ImGui::CollapsingHeader("Widgets"))
        return;

    if (ImGui::TreeNode("Basic"))
    {
        static int clicked = 0;
        if (ImGui::Button("Button"))
            clicked++;
        if (clicked & 1)
        {
            ImGui::SameLine();
            ImGui::Text("Thanks for clicking me!");
        }

        static bool check = true;
        ImGui::Checkbox("checkbox", &check);

        static int e = 0;
        ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
        ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
        ImGui::RadioButton("radio c", &e, 2);

        // Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
        for (int i = 0; i < 7; i++)
        {
            if (i > 0)
                ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i/7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i/7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i/7.0f, 0.8f, 0.8f));
            ImGui::Button("Click");
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }

        // Use AlignTextToFramePadding() to align text baseline to the baseline of framed elements (otherwise a Text+SameLine+Button sequence will have the text a little too high by default)
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Hold to repeat:");
        ImGui::SameLine();

        // Arrow buttons with Repeater
        static int counter = 0;
        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { counter--; }
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { counter++; }
        ImGui::PopButtonRepeat();
        ImGui::SameLine();
        ImGui::Text("%d", counter);

        ImGui::Text("Hover over me");
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("I am a tooltip");

        ImGui::SameLine();
        ImGui::Text("- or me");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("I am a fancy tooltip");
            static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
            ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
            ImGui::EndTooltip();
        }

        ImGui::Separator();

        ImGui::LabelText("label", "Value");

        {
            // Using the _simplified_ one-liner Combo() api here
            // See "Combo" section for examples of how to use the more complete BeginCombo()/EndCombo() api.
            const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
            static int item_current = 0;
            ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
            ImGui::SameLine(); HelpMarker("Refer to the \"Combo\" section below for an explanation of the full BeginCombo/EndCombo API, and demonstration of various flags.\n");
        }

        {
            static char str0[128] = "Hello, world!";
            ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
            ImGui::SameLine(); HelpMarker("USER:\nHold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n\nPROGRAMMER:\nYou can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputText() to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example (this is not demonstrated in imgui_demo.cpp).");

            static char str1[128] = "";
            ImGui::InputTextWithHint("input text (w/ hint)", "enter text here", str1, IM_ARRAYSIZE(str1));

            static int i0 = 123;
            ImGui::InputInt("input int", &i0);
            ImGui::SameLine(); HelpMarker("You can apply arithmetic operators +,*,/ on numerical values.\n  e.g. [ 100 ], input \'*2\', result becomes [ 200 ]\nUse +- to subtract.\n");

            static float f0 = 0.001f;
            ImGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

            static double d0 = 999999.00000001;
            ImGui::InputDouble("input double", &d0, 0.01f, 1.0f, "%.8f");

            static float f1 = 1.e10f;
            ImGui::InputFloat("input scientific", &f1, 0.0f, 0.0f, "%e");
            ImGui::SameLine(); HelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

            static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
            ImGui::InputFloat3("input float3", vec4a);
        }

        {
            static int i1 = 50, i2 = 42;
            ImGui::DragInt("drag int", &i1, 1);
            ImGui::SameLine(); HelpMarker("Click and drag to edit value.\nHold SHIFT/ALT for faster/slower edit.\nDouble-click or CTRL+click to input value.");

            ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%");

            static float f1=1.00f, f2=0.0067f;
            ImGui::DragFloat("drag float", &f1, 0.005f);
            ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
        }

        {
            static int i1=0;
            ImGui::SliderInt("slider int", &i1, -1, 3);
            ImGui::SameLine(); HelpMarker("CTRL+click to input value.");

            static float f1=0.123f, f2=0.0f;
            ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("slider float (curve)", &f2, -10.0f, 10.0f, "%.4f", 2.0f);

            static float angle = 0.0f;
            ImGui::SliderAngle("slider angle", &angle);

            // Using the format string to display a name instead of an integer.
            // Here we completely omit '%d' from the format string, so it'll only display a name.
            // This technique can also be used with DragInt().
            enum Element { Element_Fire, Element_Earth, Element_Air, Element_Water, Element_COUNT };
            const char* element_names[Element_COUNT] = { "Fire", "Earth", "Air", "Water" };
            static int current_element = Element_Fire;
            const char* current_element_name = (current_element >= 0 && current_element < Element_COUNT) ? element_names[current_element] : "Unknown";
            ImGui::SliderInt("slider enum", &current_element, 0, Element_COUNT - 1, current_element_name);
            ImGui::SameLine(); HelpMarker("Using the format string parameter to display a name instead of the underlying integer.");
        }

        {
            static float col1[3] = { 1.0f,0.0f,0.2f };
            static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
            ImGui::ColorEdit3("color 1", col1);
            ImGui::SameLine(); HelpMarker("Click on the colored square to open a color picker.\nClick and hold to use drag and drop.\nRight-click on the colored square to show options.\nCTRL+click on individual component to input value.\n");

            ImGui::ColorEdit4("color 2", col2);
        }

        {
            // List box
            const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
            static int listbox_item_current = 1;
            ImGui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

            //static int listbox_item_current2 = 2;
            //ImGui::SetNextItemWidth(-1);
            //ImGui::ListBox("##listbox2", &listbox_item_current2, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
        }

        ImGui::TreePop();
    }

    // Testing ImGuiOnceUponAFrame helper.
    //static ImGuiOnceUponAFrame once;
    //for (int i = 0; i < 5; i++)
    //    if (once)
    //        ImGui::Text("This will be displayed only once.");

    if (ImGui::TreeNode("Trees"))
    {
        if (ImGui::TreeNode("Basic trees"))
        {
            for (int i = 0; i < 5; i++)
            {
                // Use SetNextItemOpen() so set the default state of a node to be open.
                // We could also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
                if (i == 0)
                    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
                {
                    ImGui::Text("blah blah");
                    ImGui::SameLine();
                    if (ImGui::SmallButton("button")) {};
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Advanced, with Selectable nodes"))
        {
            HelpMarker("This is a more typical looking tree with selectable nodes.\nClick to select, CTRL+Click to toggle, click on arrows or double-click to open.");
            static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            static bool align_label_with_current_x_position = false;
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnArrow", (unsigned int*)&base_flags, ImGuiTreeNodeFlags_OpenOnArrow);
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnDoubleClick", (unsigned int*)&base_flags, ImGuiTreeNodeFlags_OpenOnDoubleClick);
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth", (unsigned int*)&base_flags, ImGuiTreeNodeFlags_SpanAvailWidth);
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", (unsigned int*)&base_flags, ImGuiTreeNodeFlags_SpanFullWidth);
            ImGui::Checkbox("Align label with current X position)", &align_label_with_current_x_position);
            ImGui::Text("Hello!");
            if (align_label_with_current_x_position)
                ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

            static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
            int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
            for (int i = 0; i < 6; i++)
            {
                // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
                ImGuiTreeNodeFlags node_flags = base_flags;
                const bool is_selected = (selection_mask & (1 << i)) != 0;
                if (is_selected)
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                if (i < 3)
                {
                    // Items 0..2 are Tree Node
                    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
                    if (ImGui::IsItemClicked())
                        node_clicked = i;
                    if (node_open)
                    {
                        ImGui::BulletText("Blah blah\nBlah Blah");
                        ImGui::TreePop();
                    }
                }
                else
                {
                    // Items 3..5 are Tree Leaves
                    // The only reason we use TreeNode at all is to allow selection of the leaf.
                    // Otherwise we can use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
                    node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                    ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
                    if (ImGui::IsItemClicked())
                        node_clicked = i;
                }
            }
            if (node_clicked != -1)
            {
                // Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
                if (ImGui::GetIO().KeyCtrl)
                    selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
                else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
                    selection_mask = (1 << node_clicked);           // Click to single-select
            }
            if (align_label_with_current_x_position)
                ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Collapsing Headers"))
    {
        static bool closable_group = true;
        ImGui::Checkbox("Show 2nd header", &closable_group);
        if (ImGui::CollapsingHeader("Header", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                ImGui::Text("Some content %d", i);
        }
        if (ImGui::CollapsingHeader("Header with a close button", &closable_group))
        {
            ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                ImGui::Text("More content %d", i);
        }
        /*
        if (ImGui::CollapsingHeader("Header with a bullet", ImGuiTreeNodeFlags_Bullet))
            ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
        */
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Bullets"))
    {
        ImGui::BulletText("Bullet point 1");
        ImGui::BulletText("Bullet point 2\nOn multiple lines");
        if (ImGui::TreeNode("Tree node"))
        {
            ImGui::BulletText("Another bullet point");
            ImGui::TreePop();
        }
        ImGui::Bullet(); ImGui::Text("Bullet point 3 (two calls)");
        ImGui::Bullet(); ImGui::SmallButton("Button");
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Text"))
    {
        if (ImGui::TreeNode("Colored Text"))
        {
            // Using shortcut. You can use PushStyleColor()/PopStyleColor() for more flexibility.
            ImGui::TextColored(ImVec4(1.0f,0.0f,1.0f,1.0f), "Pink");
            ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Yellow");
            ImGui::TextDisabled("Disabled");
            ImGui::SameLine(); HelpMarker("The TextDisabled color is stored in ImGuiStyle.");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Word Wrapping"))
        {
            // Using shortcut. You can use PushTextWrapPos()/PopTextWrapPos() for more flexibility.
            ImGui::TextWrapped("This text should automatically wrap on the edge of the window. The current implementation for text wrapping follows simple rules suitable for English and possibly other languages.");
            ImGui::Spacing();

            static float wrap_width = 200.0f;
            ImGui::SliderFloat("Wrap width", &wrap_width, -20, 600, "%.0f");

            ImGui::Text("Test paragraph 1:");
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + wrap_width, pos.y), ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight()), IM_COL32(255,0,255,255));
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
            ImGui::Text("The lazy dog is a good dog. This paragraph is made to fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
            ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255,255,0,255));
            ImGui::PopTextWrapPos();

            ImGui::Text("Test paragraph 2:");
            pos = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + wrap_width, pos.y), ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight()), IM_COL32(255,0,255,255));
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
            ImGui::Text("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh");
            ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255,255,0,255));
            ImGui::PopTextWrapPos();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("UTF-8 Text"))
        {
            // UTF-8 test with Japanese characters
            // (Needs a suitable font, try Noto, or Arial Unicode, or M+ fonts. Read misc/fonts/README.txt for details.)
            // - From C++11 you can use the u8"my text" syntax to encode literal strings as UTF-8
            // - For earlier compiler, you may be able to encode your sources as UTF-8 (e.g. Visual Studio save your file as 'UTF-8 without signature')
            // - FOR THIS DEMO FILE ONLY, BECAUSE WE WANT TO SUPPORT OLD COMPILERS, WE ARE *NOT* INCLUDING RAW UTF-8 CHARACTERS IN THIS SOURCE FILE.
            //   Instead we are encoding a few strings with hexadecimal constants. Don't do this in your application!
            //   Please use u8"text in any language" in your application!
            // Note that characters values are preserved even by InputText() if the font cannot be displayed, so you can safely copy & paste garbled characters into another application.
            ImGui::TextWrapped("CJK text will only appears if the font was loaded with the appropriate CJK character ranges. Call io.Font->AddFontFromFileTTF() manually to load extra character ranges. Read misc/fonts/README.txt for details.");
            ImGui::Text("Hiragana: \xe3\x81\x8b\xe3\x81\x8d\xe3\x81\x8f\xe3\x81\x91\xe3\x81\x93 (kakikukeko)"); // Normally we would use u8"blah blah" with the proper characters directly in the string.
            ImGui::Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");
            static char buf[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
            //static char buf[32] = u8"NIHONGO"; // <- this is how you would write it with C++11, using real kanjis
            ImGui::InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf));
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Images"))
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::TextWrapped("Below we are displaying the font texture (which is the only texture we have access to in this demo). Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. Hover the texture for a zoomed view!");

        // Here we are grabbing the font texture because that's the only one we have access to inside the demo code.
        // Remember that ImTextureID is just storage for whatever you want it to be, it is essentially a value that will be passed to the render function inside the ImDrawCmd structure.
        // If you use one of the default imgui_impl_XXXX.cpp renderer, they all have comments at the top of their file to specify what they expect to be stored in ImTextureID.
        // (for example, the imgui_impl_dx11.cpp renderer expect a 'ID3D11ShaderResourceView*' pointer. The imgui_impl_opengl3.cpp renderer expect a GLuint OpenGL texture identifier etc.)
        // If you decided that ImTextureID = MyEngineTexture*, then you can pass your MyEngineTexture* pointers to ImGui::Image(), and gather width/height through your own functions, etc.
        // Using ShowMetricsWindow() as a "debugger" to inspect the draw data that are being passed to your render will help you debug issues if you are confused about this.
        // Consider using the lower-level ImDrawList::AddImage() API, via ImGui::GetWindowDrawList()->AddImage().
        ImTextureID my_tex_id = io.Fonts->TexID;
        float my_tex_w = (float)io.Fonts->TexWidth;
        float my_tex_h = (float)io.Fonts->TexHeight;

        ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            float region_sz = 32.0f;
            float region_x = io.MousePos.x - pos.x - region_sz * 0.5f; if (region_x < 0.0f) region_x = 0.0f; else if (region_x > my_tex_w - region_sz) region_x = my_tex_w - region_sz;
            float region_y = io.MousePos.y - pos.y - region_sz * 0.5f; if (region_y < 0.0f) region_y = 0.0f; else if (region_y > my_tex_h - region_sz) region_y = my_tex_h - region_sz;
            float zoom = 4.0f;
            ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
            ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
            ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
            ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
            ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
            ImGui::EndTooltip();
        }
        ImGui::TextWrapped("And now some textured buttons..");
        static int pressed_count = 0;
        for (int i = 0; i < 8; i++)
        {
            ImGui::PushID(i);
            int frame_padding = -1 + i;     // -1 = uses default padding
            if (ImGui::ImageButton(my_tex_id, ImVec2(32,32), ImVec2(0,0), ImVec2(32.0f/my_tex_w,32/my_tex_h), frame_padding, ImVec4(0.0f,0.0f,0.0f,1.0f)))
                pressed_count += 1;
            ImGui::PopID();
            ImGui::SameLine();
        }
        ImGui::NewLine();
        ImGui::Text("Pressed %d times.", pressed_count);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Combo"))
    {
        // Expose flags as checkbox for the demo
        static ImGuiComboFlags flags = 0;
        ImGui::CheckboxFlags("ImGuiComboFlags_PopupAlignLeft", (unsigned int*)&flags, ImGuiComboFlags_PopupAlignLeft);
        ImGui::SameLine(); HelpMarker("Only makes a difference if the popup is larger than the combo");
        if (ImGui::CheckboxFlags("ImGuiComboFlags_NoArrowButton", (unsigned int*)&flags, ImGuiComboFlags_NoArrowButton))
            flags &= ~ImGuiComboFlags_NoPreview;     // Clear the other flag, as we cannot combine both
        if (ImGui::CheckboxFlags("ImGuiComboFlags_NoPreview", (unsigned int*)&flags, ImGuiComboFlags_NoPreview))
            flags &= ~ImGuiComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both

        // General BeginCombo() API, you have full control over your selection data and display type.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag stored in the object itself, etc.)
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
        static const char* item_current = items[0];            // Here our selection is a single pointer stored outside the object.
        if (ImGui::BeginCombo("combo 1", item_current, flags)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (item_current == items[n]);
                if (ImGui::Selectable(items[n], is_selected))
                    item_current = items[n];
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }

        // Simplified one-liner Combo() API, using values packed in a single constant string
        static int item_current_2 = 0;
        ImGui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

        // Simplified one-liner Combo() using an array of const char*
        static int item_current_3 = -1; // If the selection isn't within 0..count, Combo won't display a preview
        ImGui::Combo("combo 3 (array)", &item_current_3, items, IM_ARRAYSIZE(items));

        // Simplified one-liner Combo() using an accessor function
        struct FuncHolder { static bool ItemGetter(void* data, int idx, const char** out_str) { *out_str = ((const char**)data)[idx]; return true; } };
        static int item_current_4 = 0;
        ImGui::Combo("combo 4 (function)", &item_current_4, &FuncHolder::ItemGetter, items, IM_ARRAYSIZE(items));

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Selectables"))
    {
        // Selectable() has 2 overloads:
        // - The one taking "bool selected" as a read-only selection information. When Selectable() has been clicked is returns true and you can alter selection state accordingly.
        // - The one taking "bool* p_selected" as a read-write selection information (convenient in some cases)
        // The earlier is more flexible, as in real application your selection may be stored in a different manner (in flags within objects, as an external list, etc).
        if (ImGui::TreeNode("Basic"))
        {
            static bool selection[5] = { false, true, false, false, false };
            ImGui::Selectable("1. I am selectable", &selection[0]);
            ImGui::Selectable("2. I am selectable", &selection[1]);
            ImGui::Text("3. I am not selectable");
            ImGui::Selectable("4. I am selectable", &selection[3]);
            if (ImGui::Selectable("5. I am double clickable", selection[4], ImGuiSelectableFlags_AllowDoubleClick))
                if (ImGui::IsMouseDoubleClicked(0))
                    selection[4] = !selection[4];
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Selection State: Single Selection"))
        {
            static int selected = -1;
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d", n);
                if (ImGui::Selectable(buf, selected == n))
                    selected = n;
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Selection State: Multiple Selection"))
        {
            HelpMarker("Hold CTRL and click to select multiple items.");
            static bool selection[5] = { false, false, false, false, false };
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d", n);
                if (ImGui::Selectable(buf, selection[n]))
                {
                    if (!ImGui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
                        memset(selection, 0, sizeof(selection));
                    selection[n] ^= 1;
                }
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Rendering more text into the same line"))
        {
            // Using the Selectable() override that takes "bool* p_selected" parameter and toggle your booleans automatically.
            static bool selected[3] = { false, false, false };
            ImGui::Selectable("main.c",    &selected[0]); ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");
            ImGui::Selectable("Hello.cpp", &selected[1]); ImGui::SameLine(300); ImGui::Text("12,345 bytes");
            ImGui::Selectable("Hello.h",   &selected[2]); ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("In columns"))
        {
            ImGui::Columns(3, NULL, false);
            static bool selected[16] = { 0 };
            for (int i = 0; i < 16; i++)
            {
                char label[32]; sprintf(label, "Item %d", i);
                if (ImGui::Selectable(label, &selected[i])) {}
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Grid"))
        {
            static bool selected[4*4] = { true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true };
            for (int i = 0; i < 4*4; i++)
            {
                ImGui::PushID(i);
                if (ImGui::Selectable("Sailor", &selected[i], 0, ImVec2(50,50)))
                {
                    // Note: We _unnecessarily_ test for both x/y and i here only to silence some static analyzer. The second part of each test is unnecessary.
                    int x = i % 4;
                    int y = i / 4;
                    if (x > 0)           { selected[i - 1] ^= 1; }
                    if (x < 3 && i < 15) { selected[i + 1] ^= 1; }
                    if (y > 0 && i > 3)  { selected[i - 4] ^= 1; }
                    if (y < 3 && i < 12) { selected[i + 4] ^= 1; }
                }
                if ((i % 4) < 3) ImGui::SameLine();
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Alignment"))
        {
            HelpMarker("Alignment applies when a selectable is larger than its text content.\nBy default, Selectables uses style.SelectableTextAlign but it can be overriden on a per-item basis using PushStyleVar().");
            static bool selected[3*3] = { true, false, true, false, true, false, true, false, true };
            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    ImVec2 alignment = ImVec2((float)x / 2.0f, (float)y / 2.0f);
                    char name[32];
                    sprintf(name, "(%.1f,%.1f)", alignment.x, alignment.y);
                    if (x > 0) ImGui::SameLine();
                    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, alignment);
                    ImGui::Selectable(name, &selected[3*y+x], ImGuiSelectableFlags_None, ImVec2(80,80));
                    ImGui::PopStyleVar();
                }
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Text Input"))
    {
        if (ImGui::TreeNode("Multi-line Text Input"))
        {
            // Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
            // and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.
            static char text[1024 * 16] =
                "/*\n"
                " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
                " the hexadecimal encoding of one offending instruction,\n"
                " more formally, the invalid operand with locked CMPXCHG8B\n"
                " instruction bug, is a design flaw in the majority of\n"
                " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
                " processors (all in the P5 microarchitecture).\n"
                "*/\n\n"
                "label:\n"
                "\tlock cmpxchg8b eax\n";

            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
            HelpMarker("You can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputTextMultiline() to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example. (This is not demonstrated in imgui_demo.cpp)");
            ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", (unsigned int*)&flags, ImGuiInputTextFlags_ReadOnly);
            ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", (unsigned int*)&flags, ImGuiInputTextFlags_AllowTabInput);
            ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine", (unsigned int*)&flags, ImGuiInputTextFlags_CtrlEnterForNewLine);
            ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Filtered Text Input"))
        {
            static char buf1[64] = ""; ImGui::InputText("default", buf1, 64);
            static char buf2[64] = ""; ImGui::InputText("decimal", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
            static char buf3[64] = ""; ImGui::InputText("hexadecimal", buf3, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
            static char buf4[64] = ""; ImGui::InputText("uppercase", buf4, 64, ImGuiInputTextFlags_CharsUppercase);
            static char buf5[64] = ""; ImGui::InputText("no blank", buf5, 64, ImGuiInputTextFlags_CharsNoBlank);
            struct TextFilters { static int FilterImGuiLetters(ImGuiInputTextCallbackData* data) { if (data->EventChar < 256 && strchr("imgui", (char)data->EventChar)) return 0; return 1; } };
            static char buf6[64] = ""; ImGui::InputText("\"imgui\" letters", buf6, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);

            ImGui::Text("Password input");
            static char bufpass[64] = "password123";
            ImGui::InputText("password", bufpass, 64, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
            ImGui::SameLine(); HelpMarker("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging.\n");
            ImGui::InputTextWithHint("password (w/ hint)", "<password>", bufpass, 64, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
            ImGui::InputText("password (clear)", bufpass, 64, ImGuiInputTextFlags_CharsNoBlank);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Resize Callback"))
        {
            // If you have a custom string type you would typically create a ImGui::InputText() wrapper than takes your type as input.
            // See misc/cpp/imgui_stdlib.h and .cpp for an implementation of this using std::string.
            HelpMarker("Demonstrate using ImGuiInputTextFlags_CallbackResize to wire your resizable string type to InputText().\n\nSee misc/cpp/imgui_stdlib.h for an implementation of this for std::string.");
            struct Funcs
            {
                static int MyResizeCallback(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                    {
                        ImVector<char>* my_str = (ImVector<char>*)data->UserData;
                        IM_ASSERT(my_str->begin() == data->Buf);
                        my_str->resize(data->BufSize);  // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
                        data->Buf = my_str->begin();
                    }
                    return 0;
                }

                // Tip: Because ImGui:: is a namespace you would typicall add your own function into the namespace in your own source files.
                // For example, you may add a function called ImGui::InputText(const char* label, MyString* my_str).
                static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
                {
                    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
                    return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
                }
            };

            // For this demo we are using ImVector as a string container.
            // Note that because we need to store a terminating zero character, our size/capacity are 1 more than usually reported by a typical string class.
            static ImVector<char> my_str;
            if (my_str.empty())
                my_str.push_back(0);
            Funcs::MyInputTextMultiline("##MyStr", &my_str, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
            ImGui::Text("Data: %p\nSize: %d\nCapacity: %d", (void*)my_str.begin(), my_str.size(), my_str.capacity());
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    // Plot/Graph widgets are currently fairly limited.
    // Consider writing your own plotting widget, or using a third-party one (see "Wiki->Useful Widgets", or github.com/ocornut/imgui/issues/2747)
    if (ImGui::TreeNode("Plots Widgets"))
    {
        static bool animate = true;
        ImGui::Checkbox("Animate", &animate);

        static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
        ImGui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));

        // Create a dummy array of contiguous float values to plot
        // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float and the sizeof() of your structure in the Stride parameter.
        static float values[90] = { 0 };
        static int values_offset = 0;
        static double refresh_time = 0.0;
        if (!animate || refresh_time == 0.0)
            refresh_time = ImGui::GetTime();
        while (refresh_time < ImGui::GetTime()) // Create dummy data at fixed 60 hz rate for the demo
        {
            static float phase = 0.0f;
            values[values_offset] = cosf(phase);
            values_offset = (values_offset+1) % IM_ARRAYSIZE(values);
            phase += 0.10f*values_offset;
            refresh_time += 1.0f/60.0f;
        }

        // Plots can display overlay texts
        // (in this example, we will display an average value)
        {
            float average = 0.0f;
            for (int n = 0; n < IM_ARRAYSIZE(values); n++)
                average += values[n];
            average /= (float)IM_ARRAYSIZE(values);
            char overlay[32];
            sprintf(overlay, "avg %f", average);
            ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0,80));
        }
        ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0,80));

        // Use functions to generate output
        // FIXME: This is rather awkward because current plot API only pass in indices. We probably want an API passing floats and user provide sample rate/count.
        struct Funcs
        {
            static float Sin(void*, int i) { return sinf(i * 0.1f); }
            static float Saw(void*, int i) { return (i & 1) ? 1.0f : -1.0f; }
        };
        static int func_type = 0, display_count = 70;
        ImGui::Separator();
        ImGui::SetNextItemWidth(100);
        ImGui::Combo("func", &func_type, "Sin\0Saw\0");
        ImGui::SameLine();
        ImGui::SliderInt("Sample count", &display_count, 1, 400);
        float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
        ImGui::PlotLines("Lines", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
        ImGui::PlotHistogram("Histogram", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
        ImGui::Separator();

        // Animate a simple progress bar
        static float progress = 0.0f, progress_dir = 1.0f;
        if (animate)
        {
            progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
        }

        // Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width,
        // or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
        ImGui::ProgressBar(progress, ImVec2(0.0f,0.0f));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Progress Bar");

        float progress_saturated = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
        char buf[32];
        sprintf(buf, "%d/%d", (int)(progress_saturated*1753), 1753);
        ImGui::ProgressBar(progress, ImVec2(0.f,0.f), buf);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Color/Picker Widgets"))
    {
        static ImVec4 color = ImVec4(114.0f/255.0f, 144.0f/255.0f, 154.0f/255.0f, 200.0f/255.0f);

        static bool alpha_preview = true;
        static bool alpha_half_preview = false;
        static bool drag_and_drop = true;
        static bool options_menu = true;
        static bool hdr = false;
        ImGui::Checkbox("With Alpha Preview", &alpha_preview);
        ImGui::Checkbox("With Half Alpha Preview", &alpha_half_preview);
        ImGui::Checkbox("With Drag and Drop", &drag_and_drop);
        ImGui::Checkbox("With Options Menu", &options_menu); ImGui::SameLine(); HelpMarker("Right-click on the individual color widget to show options.");
        ImGui::Checkbox("With HDR", &hdr); ImGui::SameLine(); HelpMarker("Currently all this does is to lift the 0..1 limits on dragging widgets.");
        ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

        ImGui::Text("Color widget:");
        ImGui::SameLine(); HelpMarker("Click on the colored square to open a color picker.\nCTRL+click on individual component to input value.\n");
        ImGui::ColorEdit3("MyColor##1", (float*)&color, misc_flags);

        ImGui::Text("Color widget HSV with Alpha:");
        ImGui::ColorEdit4("MyColor##2", (float*)&color, ImGuiColorEditFlags_DisplayHSV | misc_flags);

        ImGui::Text("Color widget with Float Display:");
        ImGui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float | misc_flags);

        ImGui::Text("Color button with Picker:");
        ImGui::SameLine(); HelpMarker("With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\nWith the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only be used for the tooltip and picker popup.");
        ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

        ImGui::Text("Color button with Custom Picker Popup:");

        // Generate a dummy default palette. The palette will persist and can be edited.
        static bool saved_palette_init = true;
        static ImVec4 saved_palette[32] = { };
        if (saved_palette_init)
        {
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f, saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
                saved_palette[n].w = 1.0f; // Alpha
            }
            saved_palette_init = false;
        }

        static ImVec4 backup_color;
        bool open_popup = ImGui::ColorButton("MyColor##3b", color, misc_flags);
        ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
        open_popup |= ImGui::Button("Palette");
        if (open_popup)
        {
            ImGui::OpenPopup("mypicker");
            backup_color = color;
        }
        if (ImGui::BeginPopup("mypicker"))
        {
            ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
            ImGui::Separator();
            ImGui::ColorPicker4("##picker", (float*)&color, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
            ImGui::SameLine();

            ImGui::BeginGroup(); // Lock X position
            ImGui::Text("Current");
            ImGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60,40));
            ImGui::Text("Previous");
            if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60,40)))
                color = backup_color;
            ImGui::Separator();
            ImGui::Text("Palette");
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                ImGui::PushID(n);
                if ((n % 8) != 0)
                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
                if (ImGui::ColorButton("##palette", saved_palette[n], ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip, ImVec2(20,20)))
                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

                // Allow user to drop colors into each palette entry
                // (Note that ColorButton is already a drag source by default, unless using ImGuiColorEditFlags_NoDragDrop)
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
                    ImGui::EndDragDropTarget();
                }

                ImGui::PopID();
            }
            ImGui::EndGroup();
            ImGui::EndPopup();
        }

        ImGui::Text("Color button only:");
        ImGui::ColorButton("MyColor##3c", *(ImVec4*)&color, misc_flags, ImVec2(80,80));

        ImGui::Text("Color picker:");
        static bool alpha = true;
        static bool alpha_bar = true;
        static bool side_preview = true;
        static bool ref_color = false;
        static ImVec4 ref_color_v(1.0f,0.0f,1.0f,0.5f);
        static int display_mode = 0;
        static int picker_mode = 0;
        ImGui::Checkbox("With Alpha", &alpha);
        ImGui::Checkbox("With Alpha Bar", &alpha_bar);
        ImGui::Checkbox("With Side Preview", &side_preview);
        if (side_preview)
        {
            ImGui::SameLine();
            ImGui::Checkbox("With Ref Color", &ref_color);
            if (ref_color)
            {
                ImGui::SameLine();
                ImGui::ColorEdit4("##RefColor", &ref_color_v.x, ImGuiColorEditFlags_NoInputs | misc_flags);
            }
        }
        ImGui::Combo("Display Mode", &display_mode, "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
        ImGui::SameLine(); HelpMarker("ColorEdit defaults to displaying RGB inputs if you don't specify a display mode, but the user can change it with a right-click.\n\nColorPicker defaults to displaying RGB+HSV+Hex if you don't specify a display mode.\n\nYou can change the defaults using SetColorEditOptions().");
        ImGui::Combo("Picker Mode", &picker_mode, "Auto/Current\0Hue bar + SV rect\0Hue wheel + SV triangle\0");
        ImGui::SameLine(); HelpMarker("User can right-click the picker to change mode.");
        ImGuiColorEditFlags flags = misc_flags;
        if (!alpha)            flags |= ImGuiColorEditFlags_NoAlpha;        // This is by default if you call ColorPicker3() instead of ColorPicker4()
        if (alpha_bar)         flags |= ImGuiColorEditFlags_AlphaBar;
        if (!side_preview)     flags |= ImGuiColorEditFlags_NoSidePreview;
        if (picker_mode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
        if (picker_mode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
        if (display_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
        if (display_mode == 2) flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
        if (display_mode == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
        if (display_mode == 4) flags |= ImGuiColorEditFlags_DisplayHex;
        ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

        ImGui::Text("Programmatically set defaults:");
        ImGui::SameLine(); HelpMarker("SetColorEditOptions() is designed to allow you to set boot-time default.\nWe don't have Push/Pop functions because you can force options on a per-widget basis if needed, and the user can change non-forced ones with the options menu.\nWe don't have a getter to avoid encouraging you to persistently save values that aren't forward-compatible.");
        if (ImGui::Button("Default: Uint8 + HSV + Hue Bar"))
            ImGui::SetColorEditOptions(ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_PickerHueBar);
        if (ImGui::Button("Default: Float + HDR + Hue Wheel"))
            ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

        // HSV encoded support (to avoid RGB<>HSV round trips and singularities when S==0 or V==0)
        static ImVec4 color_stored_as_hsv(0.23f, 1.0f, 1.0f, 1.0f);
        ImGui::Spacing();
        ImGui::Text("HSV encoded colors");
        ImGui::SameLine(); HelpMarker("By default, colors are given to ColorEdit and ColorPicker in RGB, but ImGuiColorEditFlags_InputHSV allows you to store colors as HSV and pass them to ColorEdit and ColorPicker as HSV. This comes with the added benefit that you can manipulate hue values with the picker even when saturation or value are zero.");
        ImGui::Text("Color widget with InputHSV:");
        ImGui::ColorEdit4("HSV shown as RGB##1", (float*)&color_stored_as_hsv, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("HSV shown as HSV##1", (float*)&color_stored_as_hsv, ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
        ImGui::DragFloat4("Raw HSV values", (float*)&color_stored_as_hsv, 0.01f, 0.0f, 1.0f);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Range Widgets"))
    {
        static float begin = 10, end = 90;
        static int begin_i = 100, end_i = 1000;
        ImGui::DragFloatRange2("range", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%");
        ImGui::DragIntRange2("range int (no bounds)", &begin_i, &end_i, 5, 0, 0, "Min: %d units", "Max: %d units");
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Data Types"))
    {
        // The DragScalar/InputScalar/SliderScalar functions allow various data types: signed/unsigned int/long long and float/double
        // To avoid polluting the public API with all possible combinations, we use the ImGuiDataType enum to pass the type,
        // and passing all arguments by address.
        // This is the reason the test code below creates local variables to hold "zero" "one" etc. for each types.
        // In practice, if you frequently use a given type that is not covered by the normal API entry points, you can wrap it
        // yourself inside a 1 line function which can take typed argument as value instead of void*, and then pass their address
        // to the generic function. For example:
        //   bool MySliderU64(const char *label, u64* value, u64 min = 0, u64 max = 0, const char* format = "%lld")
        //   {
        //      return SliderScalar(label, ImGuiDataType_U64, value, &min, &max, format);
        //   }

        // Limits (as helper variables that we can take the address of)
        // Note that the SliderScalar function has a maximum usable range of half the natural type maximum, hence the /2 below.
        #ifndef LLONG_MIN
        ImS64 LLONG_MIN = -9223372036854775807LL - 1;
        ImS64 LLONG_MAX = 9223372036854775807LL;
        ImU64 ULLONG_MAX = (2ULL * 9223372036854775807LL + 1);
        #endif
        const char    s8_zero  = 0,   s8_one  = 1,   s8_fifty  = 50, s8_min  = -128,        s8_max = 127;
        const ImU8    u8_zero  = 0,   u8_one  = 1,   u8_fifty  = 50, u8_min  = 0,           u8_max = 255;
        const short   s16_zero = 0,   s16_one = 1,   s16_fifty = 50, s16_min = -32768,      s16_max = 32767;
        const ImU16   u16_zero = 0,   u16_one = 1,   u16_fifty = 50, u16_min = 0,           u16_max = 65535;
        const ImS32   s32_zero = 0,   s32_one = 1,   s32_fifty = 50, s32_min = INT_MIN/2,   s32_max = INT_MAX/2,    s32_hi_a = INT_MAX/2 - 100,    s32_hi_b = INT_MAX/2;
        const ImU32   u32_zero = 0,   u32_one = 1,   u32_fifty = 50, u32_min = 0,           u32_max = UINT_MAX/2,   u32_hi_a = UINT_MAX/2 - 100,   u32_hi_b = UINT_MAX/2;
        const ImS64   s64_zero = 0,   s64_one = 1,   s64_fifty = 50, s64_min = LLONG_MIN/2, s64_max = LLONG_MAX/2,  s64_hi_a = LLONG_MAX/2 - 100,  s64_hi_b = LLONG_MAX/2;
        const ImU64   u64_zero = 0,   u64_one = 1,   u64_fifty = 50, u64_min = 0,           u64_max = ULLONG_MAX/2, u64_hi_a = ULLONG_MAX/2 - 100, u64_hi_b = ULLONG_MAX/2;
        const float   f32_zero = 0.f, f32_one = 1.f, f32_lo_a = -10000000000.0f, f32_hi_a = +10000000000.0f;
        const double  f64_zero = 0.,  f64_one = 1.,  f64_lo_a = -1000000000000000.0, f64_hi_a = +1000000000000000.0;

        // State
        static char   s8_v  = 127;
        static ImU8   u8_v  = 255;
        static short  s16_v = 32767;
        static ImU16  u16_v = 65535;
        static ImS32  s32_v = -1;
        static ImU32  u32_v = (ImU32)-1;
        static ImS64  s64_v = -1;
        static ImU64  u64_v = (ImU64)-1;
        static float  f32_v = 0.123f;
        static double f64_v = 90000.01234567890123456789;

        const float drag_speed = 0.2f;
        static bool drag_clamp = false;
        ImGui::Text("Drags:");
        ImGui::Checkbox("Clamp integers to 0..50", &drag_clamp); ImGui::SameLine(); HelpMarker("As with every widgets in dear imgui, we never modify values unless there is a user interaction.\nYou can override the clamping limits by using CTRL+Click to input a value.");
        ImGui::DragScalar("drag s8",        ImGuiDataType_S8,     &s8_v,  drag_speed, drag_clamp ? &s8_zero  : NULL, drag_clamp ? &s8_fifty  : NULL);
        ImGui::DragScalar("drag u8",        ImGuiDataType_U8,     &u8_v,  drag_speed, drag_clamp ? &u8_zero  : NULL, drag_clamp ? &u8_fifty  : NULL, "%u ms");
        ImGui::DragScalar("drag s16",       ImGuiDataType_S16,    &s16_v, drag_speed, drag_clamp ? &s16_zero : NULL, drag_clamp ? &s16_fifty : NULL);
        ImGui::DragScalar("drag u16",       ImGuiDataType_U16,    &u16_v, drag_speed, drag_clamp ? &u16_zero : NULL, drag_clamp ? &u16_fifty : NULL, "%u ms");
        ImGui::DragScalar("drag s32",       ImGuiDataType_S32,    &s32_v, drag_speed, drag_clamp ? &s32_zero : NULL, drag_clamp ? &s32_fifty : NULL);
        ImGui::DragScalar("drag u32",       ImGuiDataType_U32,    &u32_v, drag_speed, drag_clamp ? &u32_zero : NULL, drag_clamp ? &u32_fifty : NULL, "%u ms");
        ImGui::DragScalar("drag s64",       ImGuiDataType_S64,    &s64_v, drag_speed, drag_clamp ? &s64_zero : NULL, drag_clamp ? &s64_fifty : NULL);
        ImGui::DragScalar("drag u64",       ImGuiDataType_U64,    &u64_v, drag_speed, drag_clamp ? &u64_zero : NULL, drag_clamp ? &u64_fifty : NULL);
        ImGui::DragScalar("drag float",     ImGuiDataType_Float,  &f32_v, 0.005f,  &f32_zero, &f32_one, "%f", 1.0f);
        ImGui::DragScalar("drag float ^2",  ImGuiDataType_Float,  &f32_v, 0.005f,  &f32_zero, &f32_one, "%f", 2.0f); ImGui::SameLine(); HelpMarker("You can use the 'power' parameter to increase tweaking precision on one side of the range.");
        ImGui::DragScalar("drag double",    ImGuiDataType_Double, &f64_v, 0.0005f, &f64_zero, NULL,     "%.10f grams", 1.0f);
        ImGui::DragScalar("drag double ^2", ImGuiDataType_Double, &f64_v, 0.0005f, &f64_zero, &f64_one, "0 < %.10f < 1", 2.0f);

        ImGui::Text("Sliders");
        ImGui::SliderScalar("slider s8 full",     ImGuiDataType_S8,     &s8_v,  &s8_min,   &s8_max,   "%d");
        ImGui::SliderScalar("slider u8 full",     ImGuiDataType_U8,     &u8_v,  &u8_min,   &u8_max,   "%u");
        ImGui::SliderScalar("slider s16 full",    ImGuiDataType_S16,    &s16_v, &s16_min,  &s16_max,  "%d");
        ImGui::SliderScalar("slider u16 full",    ImGuiDataType_U16,    &u16_v, &u16_min,  &u16_max,  "%u");
        ImGui::SliderScalar("slider s32 low",     ImGuiDataType_S32,    &s32_v, &s32_zero, &s32_fifty,"%d");
        ImGui::SliderScalar("slider s32 high",    ImGuiDataType_S32,    &s32_v, &s32_hi_a, &s32_hi_b, "%d");
        ImGui::SliderScalar("slider s32 full",    ImGuiDataType_S32,    &s32_v, &s32_min,  &s32_max,  "%d");
        ImGui::SliderScalar("slider u32 low",     ImGuiDataType_U32,    &u32_v, &u32_zero, &u32_fifty,"%u");
        ImGui::SliderScalar("slider u32 high",    ImGuiDataType_U32,    &u32_v, &u32_hi_a, &u32_hi_b, "%u");
        ImGui::SliderScalar("slider u32 full",    ImGuiDataType_U32,    &u32_v, &u32_min,  &u32_max,  "%u");
        ImGui::SliderScalar("slider s64 low",     ImGuiDataType_S64,    &s64_v, &s64_zero, &s64_fifty,"%I64d");
        ImGui::SliderScalar("slider s64 high",    ImGuiDataType_S64,    &s64_v, &s64_hi_a, &s64_hi_b, "%I64d");
        ImGui::SliderScalar("slider s64 full",    ImGuiDataType_S64,    &s64_v, &s64_min,  &s64_max,  "%I64d");
        ImGui::SliderScalar("slider u64 low",     ImGuiDataType_U64,    &u64_v, &u64_zero, &u64_fifty,"%I64u ms");
        ImGui::SliderScalar("slider u64 high",    ImGuiDataType_U64,    &u64_v, &u64_hi_a, &u64_hi_b, "%I64u ms");
        ImGui::SliderScalar("slider u64 full",    ImGuiDataType_U64,    &u64_v, &u64_min,  &u64_max,  "%I64u ms");
        ImGui::SliderScalar("slider float low",   ImGuiDataType_Float,  &f32_v, &f32_zero, &f32_one);
        ImGui::SliderScalar("slider float low^2", ImGuiDataType_Float,  &f32_v, &f32_zero, &f32_one,  "%.10f", 2.0f);
        ImGui::SliderScalar("slider float high",  ImGuiDataType_Float,  &f32_v, &f32_lo_a, &f32_hi_a, "%e");
        ImGui::SliderScalar("slider double low",  ImGuiDataType_Double, &f64_v, &f64_zero, &f64_one,  "%.10f grams", 1.0f);
        ImGui::SliderScalar("slider double low^2",ImGuiDataType_Double, &f64_v, &f64_zero, &f64_one,  "%.10f", 2.0f);
        ImGui::SliderScalar("slider double high", ImGuiDataType_Double, &f64_v, &f64_lo_a, &f64_hi_a, "%e grams", 1.0f);

        static bool inputs_step = true;
        ImGui::Text("Inputs");
        ImGui::Checkbox("Show step buttons", &inputs_step);
        ImGui::InputScalar("input s8",      ImGuiDataType_S8,     &s8_v,  inputs_step ? &s8_one  : NULL, NULL, "%d");
        ImGui::InputScalar("input u8",      ImGuiDataType_U8,     &u8_v,  inputs_step ? &u8_one  : NULL, NULL, "%u");
        ImGui::InputScalar("input s16",     ImGuiDataType_S16,    &s16_v, inputs_step ? &s16_one : NULL, NULL, "%d");
        ImGui::InputScalar("input u16",     ImGuiDataType_U16,    &u16_v, inputs_step ? &u16_one : NULL, NULL, "%u");
        ImGui::InputScalar("input s32",     ImGuiDataType_S32,    &s32_v, inputs_step ? &s32_one : NULL, NULL, "%d");
        ImGui::InputScalar("input s32 hex", ImGuiDataType_S32,    &s32_v, inputs_step ? &s32_one : NULL, NULL, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
        ImGui::InputScalar("input u32",     ImGuiDataType_U32,    &u32_v, inputs_step ? &u32_one : NULL, NULL, "%u");
        ImGui::InputScalar("input u32 hex", ImGuiDataType_U32,    &u32_v, inputs_step ? &u32_one : NULL, NULL, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
        ImGui::InputScalar("input s64",     ImGuiDataType_S64,    &s64_v, inputs_step ? &s64_one : NULL);
        ImGui::InputScalar("input u64",     ImGuiDataType_U64,    &u64_v, inputs_step ? &u64_one : NULL);
        ImGui::InputScalar("input float",   ImGuiDataType_Float,  &f32_v, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("input double",  ImGuiDataType_Double, &f64_v, inputs_step ? &f64_one : NULL);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Multi-component Widgets"))
    {
        static float vec4f[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
        static int vec4i[4] = { 1, 5, 100, 255 };

        ImGui::InputFloat2("input float2", vec4f);
        ImGui::DragFloat2("drag float2", vec4f, 0.01f, 0.0f, 1.0f);
        ImGui::SliderFloat2("slider float2", vec4f, 0.0f, 1.0f);
        ImGui::InputInt2("input int2", vec4i);
        ImGui::DragInt2("drag int2", vec4i, 1, 0, 255);
        ImGui::SliderInt2("slider int2", vec4i, 0, 255);
        ImGui::Spacing();

        ImGui::InputFloat3("input float3", vec4f);
        ImGui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);
        ImGui::SliderFloat3("slider float3", vec4f, 0.0f, 1.0f);
        ImGui::InputInt3("input int3", vec4i);
        ImGui::DragInt3("drag int3", vec4i, 1, 0, 255);
        ImGui::SliderInt3("slider int3", vec4i, 0, 255);
        ImGui::Spacing();

        ImGui::InputFloat4("input float4", vec4f);
        ImGui::DragFloat4("drag float4", vec4f, 0.01f, 0.0f, 1.0f);
        ImGui::SliderFloat4("slider float4", vec4f, 0.0f, 1.0f);
        ImGui::InputInt4("input int4", vec4i);
        ImGui::DragInt4("drag int4", vec4i, 1, 0, 255);
        ImGui::SliderInt4("slider int4", vec4i, 0, 255);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Vertical Sliders"))
    {
        const float spacing = 4;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

        static int int_value = 0;
        ImGui::VSliderInt("##int", ImVec2(18,160), &int_value, 0, 5);
        ImGui::SameLine();

        static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
        ImGui::PushID("set1");
        for (int i = 0; i < 7; i++)
        {
            if (i > 0) ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i/7.0f, 0.5f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i/7.0f, 0.6f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i/7.0f, 0.7f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i/7.0f, 0.9f, 0.9f));
            ImGui::VSliderFloat("##v", ImVec2(18,160), &values[i], 0.0f, 1.0f, "");
            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                ImGui::SetTooltip("%.3f", values[i]);
            ImGui::PopStyleColor(4);
            ImGui::PopID();
        }
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID("set2");
        static float values2[4] = { 0.20f, 0.80f, 0.40f, 0.25f };
        const int rows = 3;
        const ImVec2 small_slider_size(18, (float)(int)((160.0f - (rows - 1) * spacing) / rows));
        for (int nx = 0; nx < 4; nx++)
        {
            if (nx > 0) ImGui::SameLine();
            ImGui::BeginGroup();
            for (int ny = 0; ny < rows; ny++)
            {
                ImGui::PushID(nx*rows+ny);
                ImGui::VSliderFloat("##v", small_slider_size, &values2[nx], 0.0f, 1.0f, "");
                if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                    ImGui::SetTooltip("%.3f", values2[nx]);
                ImGui::PopID();
            }
            ImGui::EndGroup();
        }
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID("set3");
        for (int i = 0; i < 4; i++)
        {
            if (i > 0) ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
            ImGui::VSliderFloat("##v", ImVec2(40,160), &values[i], 0.0f, 1.0f, "%.2f\nsec");
            ImGui::PopStyleVar();
            ImGui::PopID();
        }
        ImGui::PopID();
        ImGui::PopStyleVar();
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Drag and Drop"))
    {
        {
            // ColorEdit widgets automatically act as drag source and drag target.
            // They are using standardized payload strings IMGUI_PAYLOAD_TYPE_COLOR_3F and IMGUI_PAYLOAD_TYPE_COLOR_4F to allow your own widgets
            // to use colors in their drag and drop interaction. Also see the demo in Color Picker -> Palette demo.
            ImGui::BulletText("Drag and drop in standard widgets");
            ImGui::SameLine();
            HelpMarker("You can drag from the colored squares.");
            ImGui::Indent();
            static float col1[3] = { 1.0f,0.0f,0.2f };
            static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
            ImGui::ColorEdit3("color 1", col1);
            ImGui::ColorEdit4("color 2", col2);
            ImGui::Unindent();
        }

        {
            ImGui::BulletText("Drag and drop to copy/swap items");
            ImGui::Indent();
            enum Mode
            {
                Mode_Copy,
                Mode_Move,
                Mode_Swap
            };
            static int mode = 0;
            if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
            if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
            if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }
            static const char* names[9] = { "Bobby", "Beatrice", "Betty", "Brianna", "Barry", "Bernard", "Bibi", "Blaine", "Bryn" };
            for (int n = 0; n < IM_ARRAYSIZE(names); n++)
            {
                ImGui::PushID(n);
                if ((n % 3) != 0)
                    ImGui::SameLine();
                ImGui::Button(names[n], ImVec2(60,60));

                // Our buttons are both drag sources and drag targets here!
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));    // Set payload to carry the index of our item (could be anything)
                    if (mode == Mode_Copy) { ImGui::Text("Copy %s", names[n]); }    // Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
                    if (mode == Mode_Move) { ImGui::Text("Move %s", names[n]); }
                    if (mode == Mode_Swap) { ImGui::Text("Swap %s", names[n]); }
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int payload_n = *(const int*)payload->Data;
                        if (mode == Mode_Copy)
                        {
                            names[n] = names[payload_n];
                        }
                        if (mode == Mode_Move)
                        {
                            names[n] = names[payload_n];
                            names[payload_n] = "";
                        }
                        if (mode == Mode_Swap)
                        {
                            const char* tmp = names[n];
                            names[n] = names[payload_n];
                            names[payload_n] = tmp;
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::PopID();
            }
            ImGui::Unindent();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Querying Status (Active/Focused/Hovered etc.)"))
    {
        // Submit an item (various types available) so we can query their status in the following block.
        static int item_type = 1;
        ImGui::Combo("Item Type", &item_type, "Text\0Button\0Button (w/ repeat)\0Checkbox\0SliderFloat\0InputText\0InputFloat\0InputFloat3\0ColorEdit4\0MenuItem\0TreeNode (w/ double-click)\0ListBox\0");
        ImGui::SameLine();
        HelpMarker("Testing how various types of items are interacting with the IsItemXXX functions.");
        bool ret = false;
        static bool b = false;
        static float col4f[4] = { 1.0f, 0.5, 0.0f, 1.0f };
        static char str[16] = {};
        if (item_type == 0) { ImGui::Text("ITEM: Text"); }                                              // Testing text items with no identifier/interaction
        if (item_type == 1) { ret = ImGui::Button("ITEM: Button"); }                                    // Testing button
        if (item_type == 2) { ImGui::PushButtonRepeat(true); ret = ImGui::Button("ITEM: Button"); ImGui::PopButtonRepeat(); } // Testing button (with repeater)
        if (item_type == 3) { ret = ImGui::Checkbox("ITEM: Checkbox", &b); }                            // Testing checkbox
        if (item_type == 4) { ret = ImGui::SliderFloat("ITEM: SliderFloat", &col4f[0], 0.0f, 1.0f); }   // Testing basic item
        if (item_type == 5) { ret = ImGui::InputText("ITEM: InputText", &str[0], IM_ARRAYSIZE(str)); }  // Testing input text (which handles tabbing)
        if (item_type == 6) { ret = ImGui::InputFloat("ITEM: InputFloat", col4f, 1.0f); }               // Testing +/- buttons on scalar input
        if (item_type == 7) { ret = ImGui::InputFloat3("ITEM: InputFloat3", col4f); }                   // Testing multi-component items (IsItemXXX flags are reported merged)
        if (item_type == 8) { ret = ImGui::ColorEdit4("ITEM: ColorEdit4", col4f); }                     // Testing multi-component items (IsItemXXX flags are reported merged)
        if (item_type == 9) { ret = ImGui::MenuItem("ITEM: MenuItem"); }                                // Testing menu item (they use ImGuiButtonFlags_PressedOnRelease button policy)
        if (item_type == 10){ ret = ImGui::TreeNodeEx("ITEM: TreeNode w/ ImGuiTreeNodeFlags_OpenOnDoubleClick", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_NoTreePushOnOpen); } // Testing tree node with ImGuiButtonFlags_PressedOnDoubleClick button policy.
        if (item_type == 11){ const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi" }; static int current = 1; ret = ImGui::ListBox("ITEM: ListBox", &current, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)); }

        // Display the value of IsItemHovered() and other common item state functions. 
        // Note that the ImGuiHoveredFlags_XXX flags can be combined.
        // Because BulletText is an item itself and that would affect the output of IsItemXXX functions, 
        // we query every state in a single call to avoid storing them and to simplify the code 
        ImGui::BulletText(
            "Return value = %d\n"
            "IsItemFocused() = %d\n"
            "IsItemHovered() = %d\n"
            "IsItemHovered(_AllowWhenBlockedByPopup) = %d\n"
            "IsItemHovered(_AllowWhenBlockedByActiveItem) = %d\n"
            "IsItemHovered(_AllowWhenOverlapped) = %d\n"
            "IsItemHovered(_RectOnly) = %d\n"
            "IsItemActive() = %d\n"
            "IsItemEdited() = %d\n"
            "IsItemActivated() = %d\n"
            "IsItemDeactivated() = %d\n"
            "IsItemDeactivatedAfterEdit() = %d\n"
            "IsItemVisible() = %d\n"
            "IsItemClicked() = %d\n"
            "GetItemRectMin() = (%.1f, %.1f)\n"
            "GetItemRectMax() = (%.1f, %.1f)\n"
            "GetItemRectSize() = (%.1f, %.1f)",
            ret,
            ImGui::IsItemFocused(),
            ImGui::IsItemHovered(),
            ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
            ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem),
            ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped),
            ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly),
            ImGui::IsItemActive(),
            ImGui::IsItemEdited(),
            ImGui::IsItemActivated(),
            ImGui::IsItemDeactivated(),
            ImGui::IsItemDeactivatedAfterEdit(),
            ImGui::IsItemVisible(),
            ImGui::IsItemClicked(),
            ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y,
            ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y,
            ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y
        );

        static bool embed_all_inside_a_child_window = false;
        ImGui::Checkbox("Embed everything inside a child window (for additional testing)", &embed_all_inside_a_child_window);
        if (embed_all_inside_a_child_window)
            ImGui::BeginChild("outer_child", ImVec2(0, ImGui::GetFontSize() * 20), true);

        // Testing IsWindowFocused() function with its various flags. 
        // Note that the ImGuiFocusedFlags_XXX flags can be combined.
        ImGui::BulletText(
            "IsWindowFocused() = %d\n"
            "IsWindowFocused(_ChildWindows) = %d\n"
            "IsWindowFocused(_ChildWindows|_RootWindow) = %d\n"
            "IsWindowFocused(_RootWindow) = %d\n"
            "IsWindowFocused(_AnyWindow) = %d\n",
            ImGui::IsWindowFocused(),
            ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows),
            ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_RootWindow),
            ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow),
            ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow));

        // Testing IsWindowHovered() function with its various flags.
        // Note that the ImGuiHoveredFlags_XXX flags can be combined.
        ImGui::BulletText(
            "IsWindowHovered() = %d\n"
            "IsWindowHovered(_AllowWhenBlockedByPopup) = %d\n"
            "IsWindowHovered(_AllowWhenBlockedByActiveItem) = %d\n"
            "IsWindowHovered(_ChildWindows) = %d\n"
            "IsWindowHovered(_ChildWindows|_RootWindow) = %d\n"
            "IsWindowHovered(_ChildWindows|_AllowWhenBlockedByPopup) = %d\n"
            "IsWindowHovered(_RootWindow) = %d\n"
            "IsWindowHovered(_AnyWindow) = %d\n",
            ImGui::IsWindowHovered(),
            ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
            ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem),
            ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows),
            ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow),
            ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup),
            ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow),
            ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow));

        ImGui::BeginChild("child", ImVec2(0, 50), true);
        ImGui::Text("This is another child window for testing the _ChildWindows flag.");
        ImGui::EndChild();
        if (embed_all_inside_a_child_window)
            ImGui::EndChild();

        static char dummy_str[] = "This is a dummy field to be able to tab-out of the widgets above.";
        ImGui::InputText("dummy", dummy_str, IM_ARRAYSIZE(dummy_str), ImGuiInputTextFlags_ReadOnly);

        // Calling IsItemHovered() after begin returns the hovered status of the title bar.
        // This is useful in particular if you want to create a context menu (with BeginPopupContextItem) associated to the title bar of a window.
        static bool test_window = false;
        ImGui::Checkbox("Hovered/Active tests after Begin() for title bar testing", &test_window);
        if (test_window)
        {
            ImGui::Begin("Title bar Hovered/Active tests", &test_window);
            if (ImGui::BeginPopupContextItem()) // <-- This is using IsItemHovered()
            {
                if (ImGui::MenuItem("Close")) { test_window = false; }
                ImGui::EndPopup();
            }
            ImGui::Text(
                "IsItemHovered() after begin = %d (== is title bar hovered)\n"
                "IsItemActive() after begin = %d (== is window being clicked/moved)\n",
                ImGui::IsItemHovered(), ImGui::IsItemActive());
            ImGui::End();
        }

        ImGui::TreePop();
    }
}

static void ShowDemoWindowLayout()
{
    if (!ImGui::CollapsingHeader("Layout"))
        return;

    if (ImGui::TreeNode("Child windows"))
    {
        HelpMarker("Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window.");
        static bool disable_mouse_wheel = false;
        static bool disable_menu = false;
        ImGui::Checkbox("Disable Mouse Wheel", &disable_mouse_wheel);
        ImGui::Checkbox("Disable Menu", &disable_menu);

        static int line = 50;
        bool goto_line = ImGui::Button("Goto");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        goto_line |= ImGui::InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);

        // Child 1: no border, enable horizontal scrollbar
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | (disable_mouse_wheel ? ImGuiWindowFlags_NoScrollWithMouse : 0);
            ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 260), false, window_flags);
            for (int i = 0; i < 100; i++)
            {
                ImGui::Text("%04d: scrollable region", i);
                if (goto_line && line == i)
                    ImGui::SetScrollHereY();
            }
            if (goto_line && line >= 100)
                ImGui::SetScrollHereY();
            ImGui::EndChild();
        }

        ImGui::SameLine();

        // Child 2: rounded border
        {
            ImGuiWindowFlags window_flags = (disable_mouse_wheel ? ImGuiWindowFlags_NoScrollWithMouse : 0) | (disable_menu ? 0 : ImGuiWindowFlags_MenuBar);
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("Child2", ImVec2(0, 260), true, window_flags);
            if (!disable_menu && ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Menu"))
                {
                    ShowExampleMenuFile();
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Columns(2);
            for (int i = 0; i < 100; i++)
            {
                char buf[32];
                sprintf(buf, "%03d", i);
                ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                ImGui::NextColumn();
            }
            ImGui::EndChild();
            ImGui::PopStyleVar();
        }

        ImGui::Separator();

        // Demonstrate a few extra things
        // - Changing ImGuiCol_ChildBg (which is transparent black in default styles)
        // - Using SetCursorPos() to position the child window (because the child window is an item from the POV of the parent window)
        //   You can also call SetNextWindowPos() to position the child window. The parent window will effectively layout from this position.
        // - Using ImGui::GetItemRectMin/Max() to query the "item" state (because the child window is an item from the POV of the parent window)
        //   See "Widgets" -> "Querying Status (Active/Focused/Hovered etc.)" section for more details about this.
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
            ImGui::BeginChild("blah", ImVec2(200, 100), true, ImGuiWindowFlags_None);
            for (int n = 0; n < 50; n++)
                ImGui::Text("Some test %d", n);
            ImGui::EndChild();
            ImVec2 child_rect_min = ImGui::GetItemRectMin();
            ImVec2 child_rect_max = ImGui::GetItemRectMax();
            ImGui::PopStyleColor();
            ImGui::Text("Rect of child window is: (%.0f,%.0f) (%.0f,%.0f)", child_rect_min.x, child_rect_min.y, child_rect_max.x, child_rect_max.y);
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Widgets Width"))
    {
        // Use SetNextItemWidth() to set the width of a single upcoming item.
        // Use PushItemWidth()/PopItemWidth() to set the width of a group of items.
        static float f = 0.0f;
        ImGui::Text("SetNextItemWidth/PushItemWidth(100)");
        ImGui::SameLine(); HelpMarker("Fixed width.");
        ImGui::SetNextItemWidth(100);
        ImGui::DragFloat("float##1", &f);

        ImGui::Text("SetNextItemWidth/PushItemWidth(GetWindowWidth() * 0.5f)");
        ImGui::SameLine(); HelpMarker("Half of window width.");
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5f);
        ImGui::DragFloat("float##2", &f);

        ImGui::Text("SetNextItemWidth/PushItemWidth(GetContentRegionAvail().x * 0.5f)");
        ImGui::SameLine(); HelpMarker("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
        ImGui::DragFloat("float##3", &f);

        ImGui::Text("SetNextItemWidth/PushItemWidth(-100)");
        ImGui::SameLine(); HelpMarker("Align to right edge minus 100");
        ImGui::SetNextItemWidth(-100);
        ImGui::DragFloat("float##4", &f);

        // Demonstrate using PushItemWidth to surround three items. Calling SetNextItemWidth() before each of them would have the same effect.
        ImGui::Text("SetNextItemWidth/PushItemWidth(-1)");
        ImGui::SameLine(); HelpMarker("Align to right edge");
        ImGui::PushItemWidth(-1);
        ImGui::DragFloat("##float5a", &f);
        ImGui::DragFloat("##float5b", &f);
        ImGui::DragFloat("##float5c", &f);
        ImGui::PopItemWidth();

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Basic Horizontal Layout"))
    {
        ImGui::TextWrapped("(Use ImGui::SameLine() to keep adding items to the right of the preceding item)");

        // Text
        ImGui::Text("Two items: Hello"); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1,1,0,1), "Sailor");

        // Adjust spacing
        ImGui::Text("More spacing: Hello"); ImGui::SameLine(0, 20);
        ImGui::TextColored(ImVec4(1,1,0,1), "Sailor");

        // Button
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Normal buttons"); ImGui::SameLine();
        ImGui::Button("Banana"); ImGui::SameLine();
        ImGui::Button("Apple"); ImGui::SameLine();
        ImGui::Button("Corniflower");

        // Button
        ImGui::Text("Small buttons"); ImGui::SameLine();
        ImGui::SmallButton("Like this one"); ImGui::SameLine();
        ImGui::Text("can fit within a text block.");

        // Aligned to arbitrary position. Easy/cheap column.
        ImGui::Text("Aligned");
        ImGui::SameLine(150); ImGui::Text("x=150");
        ImGui::SameLine(300); ImGui::Text("x=300");
        ImGui::Text("Aligned");
        ImGui::SameLine(150); ImGui::SmallButton("x=150");
        ImGui::SameLine(300); ImGui::SmallButton("x=300");

        // Checkbox
        static bool c1 = false, c2 = false, c3 = false, c4 = false;
        ImGui::Checkbox("My", &c1); ImGui::SameLine();
        ImGui::Checkbox("Tailor", &c2); ImGui::SameLine();
        ImGui::Checkbox("Is", &c3); ImGui::SameLine();
        ImGui::Checkbox("Rich", &c4);

        // Various
        static float f0 = 1.0f, f1 = 2.0f, f2 = 3.0f;
        ImGui::PushItemWidth(80);
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
        static int item = -1;
        ImGui::Combo("Combo", &item, items, IM_ARRAYSIZE(items)); ImGui::SameLine();
        ImGui::SliderFloat("X", &f0, 0.0f, 5.0f); ImGui::SameLine();
        ImGui::SliderFloat("Y", &f1, 0.0f, 5.0f); ImGui::SameLine();
        ImGui::SliderFloat("Z", &f2, 0.0f, 5.0f);
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(80);
        ImGui::Text("Lists:");
        static int selection[4] = { 0, 1, 2, 3 };
        for (int i = 0; i < 4; i++)
        {
            if (i > 0) ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
            ImGui::PopID();
            //if (ImGui::IsItemHovered()) ImGui::SetTooltip("ListBox %d hovered", i);
        }
        ImGui::PopItemWidth();

        // Dummy
        ImVec2 button_sz(40, 40);
        ImGui::Button("A", button_sz); ImGui::SameLine();
        ImGui::Dummy(button_sz); ImGui::SameLine();
        ImGui::Button("B", button_sz);

        // Manually wrapping (we should eventually provide this as an automatic layout feature, but for now you can do it manually)
        ImGui::Text("Manually wrapping:");
        ImGuiStyle& style = ImGui::GetStyle();
        int buttons_count = 20;
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        for (int n = 0; n < buttons_count; n++)
        {
            ImGui::PushID(n);
            ImGui::Button("Box", button_sz);
            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
            if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
                ImGui::SameLine();
            ImGui::PopID();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Tabs"))
    {
        if (ImGui::TreeNode("Basic"))
        {
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                if (ImGui::BeginTabItem("Avocado"))
                {
                    ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Broccoli"))
                {
                    ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Cucumber"))
                {
                    ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::Separator();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Advanced & Close Button"))
        {
            // Expose a couple of the available flags. In most cases you may just call BeginTabBar() with no flags (0).
            static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
            ImGui::CheckboxFlags("ImGuiTabBarFlags_Reorderable", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_Reorderable);
            ImGui::CheckboxFlags("ImGuiTabBarFlags_AutoSelectNewTabs", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_AutoSelectNewTabs);
            ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
            ImGui::CheckboxFlags("ImGuiTabBarFlags_NoCloseWithMiddleMouseButton", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_NoCloseWithMiddleMouseButton);
            if ((tab_bar_flags & ImGuiTabBarFlags_FittingPolicyMask_) == 0)
                tab_bar_flags |= ImGuiTabBarFlags_FittingPolicyDefault_;
            if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
                tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
            if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
                tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

            // Tab Bar
            const char* names[4] = { "Artichoke", "Beetroot", "Celery", "Daikon" };
            static bool opened[4] = { true, true, true, true }; // Persistent user state
            for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
            {
                if (n > 0) { ImGui::SameLine(); }
                ImGui::Checkbox(names[n], &opened[n]);
            }

            // Passing a bool* to BeginTabItem() is similar to passing one to Begin(): the underlying bool will be set to false when the tab is closed.
            if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
                    if (opened[n] && ImGui::BeginTabItem(names[n], &opened[n]))
                    {
                        ImGui::Text("This is the %s tab!", names[n]);
                        if (n & 1)
                            ImGui::Text("I am an odd tab.");
                        ImGui::EndTabItem();
                    }
                ImGui::EndTabBar();
            }
            ImGui::Separator();
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Groups"))
    {
        HelpMarker("BeginGroup() basically locks the horizontal position for new line. EndGroup() bundles the whole group so that you can use \"item\" functions such as IsItemHovered()/IsItemActive() or SameLine() etc. on the whole group.");
        ImGui::BeginGroup();
        {
            ImGui::BeginGroup();
            ImGui::Button("AAA");
            ImGui::SameLine();
            ImGui::Button("BBB");
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Button("CCC");
            ImGui::Button("DDD");
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::Button("EEE");
            ImGui::EndGroup();
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("First group hovered");
        }
        // Capture the group size and create widgets using the same size
        ImVec2 size = ImGui::GetItemRectSize();
        const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
        ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

        ImGui::Button("ACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f, size.y));
        ImGui::SameLine();
        ImGui::Button("REACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f, size.y));
        ImGui::EndGroup();
        ImGui::SameLine();

        ImGui::Button("LEVERAGE\nBUZZWORD", size);
        ImGui::SameLine();

        if (ImGui::ListBoxHeader("List", size))
        {
            ImGui::Selectable("Selected", true);
            ImGui::Selectable("Not Selected", false);
            ImGui::ListBoxFooter();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Text Baseline Alignment"))
    {
        HelpMarker("This is testing the vertical alignment that gets applied on text to keep it aligned with widgets. Lines only composed of text or \"small\" widgets fit in less vertical spaces than lines with normal widgets.");

        ImGui::Text("One\nTwo\nThree"); ImGui::SameLine();
        ImGui::Text("Hello\nWorld"); ImGui::SameLine();
        ImGui::Text("Banana");

        ImGui::Text("Banana"); ImGui::SameLine();
        ImGui::Text("Hello\nWorld"); ImGui::SameLine();
        ImGui::Text("One\nTwo\nThree");

        ImGui::Button("HOP##1"); ImGui::SameLine();
        ImGui::Text("Banana"); ImGui::SameLine();
        ImGui::Text("Hello\nWorld"); ImGui::SameLine();
        ImGui::Text("Banana");

        ImGui::Button("HOP##2"); ImGui::SameLine();
        ImGui::Text("Hello\nWorld"); ImGui::SameLine();
        ImGui::Text("Banana");

        ImGui::Button("TEST##1"); ImGui::SameLine();
        ImGui::Text("TEST"); ImGui::SameLine();
        ImGui::SmallButton("TEST##2");

        ImGui::AlignTextToFramePadding(); // If your line starts with text, call this to align it to upcoming widgets.
        ImGui::Text("Text aligned to Widget"); ImGui::SameLine();
        ImGui::Button("Widget##1"); ImGui::SameLine();
        ImGui::Text("Widget"); ImGui::SameLine();
        ImGui::SmallButton("Widget##2"); ImGui::SameLine();
        ImGui::Button("Widget##3");

        // Tree
        const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        ImGui::Button("Button##1");
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::TreeNode("Node##1")) { for (int i = 0; i < 6; i++) ImGui::BulletText("Item %d..", i); ImGui::TreePop(); }    // Dummy tree data

        ImGui::AlignTextToFramePadding();         // Vertically align text node a bit lower so it'll be vertically centered with upcoming widget. Otherwise you can use SmallButton (smaller fit).
        bool node_open = ImGui::TreeNode("Node##2");  // Common mistake to avoid: if we want to SameLine after TreeNode we need to do it before we add child content.
        ImGui::SameLine(0.0f, spacing); ImGui::Button("Button##2");
        if (node_open) { for (int i = 0; i < 6; i++) ImGui::BulletText("Item %d..", i); ImGui::TreePop(); }   // Dummy tree data

        // Bullet
        ImGui::Button("Button##3");
        ImGui::SameLine(0.0f, spacing);
        ImGui::BulletText("Bullet text");

        ImGui::AlignTextToFramePadding();
        ImGui::BulletText("Node");
        ImGui::SameLine(0.0f, spacing); ImGui::Button("Button##4");

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Scrolling"))
    {
        // Vertical scroll functions
        HelpMarker("Use SetScrollHereY() or SetScrollFromPosY() to scroll to a given vertical position.");

        static int track_item = 50;
        static bool enable_track = true;
        static bool enable_extra_decorations = false;
        static float scroll_to_off_px = 0.0f;
        static float scroll_to_pos_px = 200.0f;

        ImGui::Checkbox("Decoration", &enable_extra_decorations);
        ImGui::SameLine();
        HelpMarker("We expose this for testing because scrolling sometimes had issues with window decoration such as menu-bars.");

        ImGui::Checkbox("Track", &enable_track);
        ImGui::PushItemWidth(100);
        ImGui::SameLine(140); enable_track |= ImGui::DragInt("##item", &track_item, 0.25f, 0, 99, "Item = %d");

        bool scroll_to_off = ImGui::Button("Scroll Offset");
        ImGui::SameLine(140); scroll_to_off |= ImGui::DragFloat("##off", &scroll_to_off_px, 1.00f, 0, 9999, "+%.0f px");

        bool scroll_to_pos = ImGui::Button("Scroll To Pos");
        ImGui::SameLine(140); scroll_to_pos |= ImGui::DragFloat("##pos", &scroll_to_pos_px, 1.00f, -10, 9999, "X/Y = %.0f px");
        ImGui::PopItemWidth();

        if (scroll_to_off || scroll_to_pos)
            enable_track = false;

        ImGuiStyle& style = ImGui::GetStyle();
        float child_w = (ImGui::GetContentRegionAvail().x - 4 * style.ItemSpacing.x) / 5;
        if (child_w < 1.0f)
            child_w = 1.0f;
        ImGui::PushID("##VerticalScrolling");
        for (int i = 0; i < 5; i++)
        {
            if (i > 0) ImGui::SameLine();
            ImGui::BeginGroup();
            const char* names[] = { "Top", "25%", "Center", "75%", "Bottom" };
            ImGui::TextUnformatted(names[i]);

            ImGuiWindowFlags child_flags = enable_extra_decorations ? ImGuiWindowFlags_MenuBar : 0;
            ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)i), ImVec2(child_w, 200.0f), true, child_flags);
            if (ImGui::BeginMenuBar())
            {
                ImGui::TextUnformatted("abc");
                ImGui::EndMenuBar();
            }
            if (scroll_to_off)
                ImGui::SetScrollY(scroll_to_off_px);
            if (scroll_to_pos)
                ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_pos_px, i * 0.25f);
            for (int item = 0; item < 100; item++)
            {
                if (enable_track && item == track_item)
                {
                    ImGui::TextColored(ImVec4(1,1,0,1), "Item %d", item);
                    ImGui::SetScrollHereY(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
                }
                else
                {
                    ImGui::Text("Item %d", item);
                }
            }
            float scroll_y = ImGui::GetScrollY();
            float scroll_max_y = ImGui::GetScrollMaxY();
            ImGui::EndChild();
            ImGui::Text("%.0f/%.0f", scroll_y, scroll_max_y);
            ImGui::EndGroup();
        }
        ImGui::PopID();

        // Horizontal scroll functions
        ImGui::Spacing();
        HelpMarker("Use SetScrollHereX() or SetScrollFromPosX() to scroll to a given horizontal position.\n\nUsing the \"Scroll To Pos\" button above will make the discontinuity at edges visible: scrolling to the top/bottom/left/right-most item will add an additional WindowPadding to reflect on reaching the edge of the list.\n\nBecause the clipping rectangle of most window hides half worth of WindowPadding on the left/right, using SetScrollFromPosX(+1) will usually result in clipped text whereas the equivalent SetScrollFromPosY(+1) wouldn't.");
        ImGui::PushID("##HorizontalScrolling");
        for (int i = 0; i < 5; i++)
        {
            float child_height = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
            ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar | (enable_extra_decorations ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0);
            ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)i), ImVec2(-100, child_height), true, child_flags);
            if (scroll_to_off)
                ImGui::SetScrollX(scroll_to_off_px);
            if (scroll_to_pos)
                ImGui::SetScrollFromPosX(ImGui::GetCursorStartPos().x + scroll_to_pos_px, i * 0.25f);
            for (int item = 0; item < 100; item++)
            {
                if (enable_track && item == track_item)
                {
                    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
                    ImGui::SetScrollHereX(i * 0.25f); // 0.0f:left, 0.5f:center, 1.0f:right
                }
                else
                {
                    ImGui::Text("Item %d", item);
                }
                ImGui::SameLine();
            }
            float scroll_x = ImGui::GetScrollX();
            float scroll_max_x = ImGui::GetScrollMaxX();
            ImGui::EndChild();
            ImGui::SameLine();
            const char* names[] = { "Left", "25%", "Center", "75%", "Right" };
            ImGui::Text("%s\n%.0f/%.0f", names[i], scroll_x, scroll_max_x);
            ImGui::Spacing();
        }
        ImGui::PopID();

        // Miscellaneous Horizontal Scrolling Demo
        HelpMarker("Horizontal scrolling for a window has to be enabled explicitly via the ImGuiWindowFlags_HorizontalScrollbar flag.\n\nYou may want to explicitly specify content width by calling SetNextWindowContentWidth() before Begin().");
        static int lines = 7;
        ImGui::SliderInt("Lines", &lines, 1, 15);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
        ImGui::BeginChild("scrolling", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30), true, ImGuiWindowFlags_HorizontalScrollbar);
        for (int line = 0; line < lines; line++)
        {
            // Display random stuff (for the sake of this trivial demo we are using basic Button+SameLine. If you want to create your own time line for a real application you may be better off
            // manipulating the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the widgets yourself. You may also want to use the lower-level ImDrawList API)
            int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
            for (int n = 0; n < num_buttons; n++)
            {
                if (n > 0) ImGui::SameLine();
                ImGui::PushID(n + line * 1000);
                char num_buf[16];
                sprintf(num_buf, "%d", n);
                const char* label = (!(n%15)) ? "FizzBuzz" : (!(n%3)) ? "Fizz" : (!(n%5)) ? "Buzz" : num_buf;
                float hue = n*0.05f;
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
                ImGui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
        }
        float scroll_x = ImGui::GetScrollX();
        float scroll_max_x = ImGui::GetScrollMaxX();
        ImGui::EndChild();
        ImGui::PopStyleVar(2);
        float scroll_x_delta = 0.0f;
        ImGui::SmallButton("<<"); if (ImGui::IsItemActive()) { scroll_x_delta = -ImGui::GetIO().DeltaTime * 1000.0f; } ImGui::SameLine();
        ImGui::Text("Scroll from code"); ImGui::SameLine();
        ImGui::SmallButton(">>"); if (ImGui::IsItemActive()) { scroll_x_delta = +ImGui::GetIO().DeltaTime * 1000.0f; } ImGui::SameLine();
        ImGui::Text("%.0f/%.0f", scroll_x, scroll_max_x);
        if (scroll_x_delta != 0.0f)
        {
            ImGui::BeginChild("scrolling"); // Demonstrate a trick: you can use Begin to set yourself in the context of another window (here we are already out of your child window)
            ImGui::SetScrollX(ImGui::GetScrollX() + scroll_x_delta);
            ImGui::EndChild();
        }
        ImGui::Spacing();

        static bool show_horizontal_contents_size_demo_window = false;
        ImGui::Checkbox("Show Horizontal contents size demo window", &show_horizontal_contents_size_demo_window);

        if (show_horizontal_contents_size_demo_window)
        {
            static bool show_h_scrollbar = true;
            static bool show_button = true;
            static bool show_tree_nodes = true;
            static bool show_text_wrapped = false;
            static bool show_columns = true;
            static bool show_tab_bar = true;
            static bool show_child = false;
            static bool explicit_content_size = false;
            static float contents_size_x = 300.0f;
            if (explicit_content_size)
                ImGui::SetNextWindowContentSize(ImVec2(contents_size_x, 0.0f));
            ImGui::Begin("Horizontal contents size demo window", &show_horizontal_contents_size_demo_window, show_h_scrollbar ? ImGuiWindowFlags_HorizontalScrollbar : 0);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
            HelpMarker("Test of different widgets react and impact the work rectangle growing when horizontal scrolling is enabled.\n\nUse 'Metrics->Tools->Show windows rectangles' to visualize rectangles.");
            ImGui::Checkbox("H-scrollbar", &show_h_scrollbar);
            ImGui::Checkbox("Button", &show_button);            // Will grow contents size (unless explicitly overwritten)
            ImGui::Checkbox("Tree nodes", &show_tree_nodes);    // Will grow contents size and display highlight over full width
            ImGui::Checkbox("Text wrapped", &show_text_wrapped);// Will grow and use contents size
            ImGui::Checkbox("Columns", &show_columns);          // Will use contents size
            ImGui::Checkbox("Tab bar", &show_tab_bar);          // Will use contents size
            ImGui::Checkbox("Child", &show_child);              // Will grow and use contents size
            ImGui::Checkbox("Explicit content size", &explicit_content_size);
            ImGui::Text("Scroll %.1f/%.1f %.1f/%.1f", ImGui::GetScrollX(), ImGui::GetScrollMaxX(), ImGui::GetScrollY(), ImGui::GetScrollMaxY());
            if (explicit_content_size)
            {
                ImGui::SameLine();
                ImGui::SetNextItemWidth(100);
                ImGui::DragFloat("##csx", &contents_size_x);
                ImVec2 p = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + 10, p.y + 10), IM_COL32_WHITE);
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + contents_size_x - 10, p.y), ImVec2(p.x + contents_size_x, p.y + 10), IM_COL32_WHITE);
                ImGui::Dummy(ImVec2(0, 10));
            }
            ImGui::PopStyleVar(2);
            ImGui::Separator();
            if (show_button)
            {
                ImGui::Button("this is a 300-wide button", ImVec2(300, 0));
            }
            if (show_tree_nodes)
            {
                bool open = true;
                if (ImGui::TreeNode("this is a tree node"))
                {
                    if (ImGui::TreeNode("another one of those tree node..."))
                    {
                        ImGui::Text("Some tree contents");
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                ImGui::CollapsingHeader("CollapsingHeader", &open);
            }
            if (show_text_wrapped)
            {
                ImGui::TextWrapped("This text should automatically wrap on the edge of the work rectangle.");
            }
            if (show_columns)
            {
                ImGui::Columns(4);
                for (int n = 0; n < 4; n++)
                {
                    ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);
            }
            if (show_tab_bar && ImGui::BeginTabBar("Hello"))
            {
                if (ImGui::BeginTabItem("OneOneOne")) { ImGui::EndTabItem(); }
                if (ImGui::BeginTabItem("TwoTwoTwo")) { ImGui::EndTabItem(); }
                if (ImGui::BeginTabItem("ThreeThreeThree")) { ImGui::EndTabItem(); }
                if (ImGui::BeginTabItem("FourFourFour")) { ImGui::EndTabItem(); }
                ImGui::EndTabBar();
            }
            if (show_child)
            {
                ImGui::BeginChild("child", ImVec2(0,0), true);
                ImGui::EndChild();
            }
            ImGui::End();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Clipping"))
    {
        static ImVec2 size(100, 100), offset(50, 20);
        ImGui::TextWrapped("On a per-widget basis we are occasionally clipping text CPU-side if it won't fit in its frame. Otherwise we are doing coarser clipping + passing a scissor rectangle to the renderer. The system is designed to try minimizing both execution and CPU/GPU rendering cost.");
        ImGui::DragFloat2("size", (float*)&size, 0.5f, 1.0f, 200.0f, "%.0f");
        ImGui::TextWrapped("(Click and drag)");
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec4 clip_rect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
        ImGui::InvisibleButton("##dummy", size);
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging()) { offset.x += ImGui::GetIO().MouseDelta.x; offset.y += ImGui::GetIO().MouseDelta.y; }
        ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(90, 90, 120, 255));
        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*2.0f, ImVec2(pos.x + offset.x, pos.y + offset.y), IM_COL32(255, 255, 255, 255), "Line 1 hello\nLine 2 clip me!", NULL, 0.0f, &clip_rect);
        ImGui::TreePop();
    }
}

static void ShowDemoWindowPopups()
{
    if (!ImGui::CollapsingHeader("Popups & Modal windows"))
        return;

    // The properties of popups windows are:
    // - They block normal mouse hovering detection outside them. (*)
    // - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
    // - Their visibility state (~bool) is held internally by Dear ImGui instead of being held by the programmer as we are used to with regular Begin() calls.
    //   User can manipulate the visibility state by calling OpenPopup().
    // (*) One can use IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even when normally blocked by a popup.
    // Those three properties are connected. The library needs to hold their visibility state because it can close popups at any time.

    // Typical use for regular windows:
    //   bool my_tool_is_active = false; if (ImGui::Button("Open")) my_tool_is_active = true; [...] if (my_tool_is_active) Begin("My Tool", &my_tool_is_active) { [...] } End();
    // Typical use for popups:
    //   if (ImGui::Button("Open")) ImGui::OpenPopup("MyPopup"); if (ImGui::BeginPopup("MyPopup") { [...] EndPopup(); }

    // With popups we have to go through a library call (here OpenPopup) to manipulate the visibility state.
    // This may be a bit confusing at first but it should quickly make sense. Follow on the examples below.

    if (ImGui::TreeNode("Popups"))
    {
        ImGui::TextWrapped("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.");

        static int selected_fish = -1;
        const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
        static bool toggles[] = { true, false, false, false, false };

        // Simple selection popup
        // (If you want to show the current selection inside the Button itself, you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
        if (ImGui::Button("Select.."))
            ImGui::OpenPopup("my_select_popup");
        ImGui::SameLine();
        ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
        if (ImGui::BeginPopup("my_select_popup"))
        {
            ImGui::Text("Aquarium");
            ImGui::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (ImGui::Selectable(names[i]))
                    selected_fish = i;
            ImGui::EndPopup();
        }

        // Showing a menu with toggles
        if (ImGui::Button("Toggle.."))
            ImGui::OpenPopup("my_toggle_popup");
        if (ImGui::BeginPopup("my_toggle_popup"))
        {
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                ImGui::MenuItem(names[i], "", &toggles[i]);
            if (ImGui::BeginMenu("Sub-menu"))
            {
                ImGui::MenuItem("Click me");
                ImGui::EndMenu();
            }

            ImGui::Separator();
            ImGui::Text("Tooltip here");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("I am a tooltip over a popup");

            if (ImGui::Button("Stacked Popup"))
                ImGui::OpenPopup("another popup");
            if (ImGui::BeginPopup("another popup"))
            {
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    ImGui::MenuItem(names[i], "", &toggles[i]);
                if (ImGui::BeginMenu("Sub-menu"))
                {
                    ImGui::MenuItem("Click me");
                    if (ImGui::Button("Stacked Popup"))
                        ImGui::OpenPopup("another popup");
                    if (ImGui::BeginPopup("another popup"))
                    {
                        ImGui::Text("I am the last one here.");
                        ImGui::EndPopup();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndPopup();
            }
            ImGui::EndPopup();
        }

        // Call the more complete ShowExampleMenuFile which we use in various places of this demo
        if (ImGui::Button("File Menu.."))
            ImGui::OpenPopup("my_file_popup");
        if (ImGui::BeginPopup("my_file_popup"))
        {
            ShowExampleMenuFile();
            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Context menus"))
    {
        // BeginPopupContextItem() is a helper to provide common/simple popup behavior of essentially doing:
        //    if (IsItemHovered() && IsMouseReleased(0))
        //       OpenPopup(id);
        //    return BeginPopup(id);
        // For more advanced uses you may want to replicate and cuztomize this code. This the comments inside BeginPopupContextItem() implementation.
        static float value = 0.5f;
        ImGui::Text("Value = %.3f (<-- right-click here)", value);
        if (ImGui::BeginPopupContextItem("item context menu"))
        {
            if (ImGui::Selectable("Set to zero")) value = 0.0f;
            if (ImGui::Selectable("Set to PI")) value = 3.1415f;
            ImGui::SetNextItemWidth(-1);
            ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
            ImGui::EndPopup();
        }

        // We can also use OpenPopupOnItemClick() which is the same as BeginPopupContextItem() but without the Begin call.
        // So here we will make it that clicking on the text field with the right mouse button (1) will toggle the visibility of the popup above.
        ImGui::Text("(You can also right-click me to open the same popup as above.)");
        ImGui::OpenPopupOnItemClick("item context menu", 1);

        // When used after an item that has an ID (here the Button), we can skip providing an ID to BeginPopupContextItem().
        // BeginPopupContextItem() will use the last item ID as the popup ID.
        // In addition here, we want to include your editable label inside the button label. We use the ### operator to override the ID (read FAQ about ID for details)
        static char name[32] = "Label1";
        char buf[64]; sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
        ImGui::Button(buf);
        if (ImGui::BeginPopupContextItem())
        {
            ImGui::Text("Edit name:");
            ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::SameLine(); ImGui::Text("(<-- right-click here)");

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Modals"))
    {
        ImGui::TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside the window.");

        if (ImGui::Button("Delete.."))
            ImGui::OpenPopup("Delete?");

        if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
            ImGui::Separator();

            //static int dummy_i = 0;
            //ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");

            static bool dont_ask_me_next_time = false;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
            ImGui::PopStyleVar();

            if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        if (ImGui::Button("Stacked modals.."))
            ImGui::OpenPopup("Stacked 1");
        if (ImGui::BeginPopupModal("Stacked 1", NULL, ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Dummy menu item")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it.");

            // Testing behavior of widgets stacking their own regular popups over the modal.
            static int item = 1;
            static float color[4] = { 0.4f,0.7f,0.0f,0.5f };
            ImGui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
            ImGui::ColorEdit4("color", color);

            if (ImGui::Button("Add another modal.."))
                ImGui::OpenPopup("Stacked 2");

            // Also demonstrate passing a bool* to BeginPopupModal(), this will create a regular close button which will close the popup.
            // Note that the visibility state of popups is owned by imgui, so the input value of the bool actually doesn't matter here.
            bool dummy_open = true;
            if (ImGui::BeginPopupModal("Stacked 2", &dummy_open))
            {
                ImGui::Text("Hello from Stacked The Second!");
                if (ImGui::Button("Close"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Menus inside a regular window"))
    {
        ImGui::TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
        ImGui::Separator();
        // NB: As a quirk in this very specific example, we want to differentiate the parent of this menu from the parent of the various popup menus above.
        // To do so we are encloding the items in a PushID()/PopID() block to make them two different menusets. If we don't, opening any popup above and hovering our menu here
        // would open it. This is because once a menu is active, we allow to switch to a sibling menu by just hovering on it, which is the desired behavior for regular menus.
        ImGui::PushID("foo");
        ImGui::MenuItem("Menu item", "CTRL+M");
        if (ImGui::BeginMenu("Menu inside a regular window"))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        ImGui::PopID();
        ImGui::Separator();
        ImGui::TreePop();
    }
}

static void ShowDemoWindowColumns()
{
    if (!ImGui::CollapsingHeader("Columns"))
        return;

    ImGui::PushID("Columns");

    static bool disable_indent = false;
    ImGui::Checkbox("Disable tree indentation", &disable_indent);
    ImGui::SameLine();
    HelpMarker("Disable the indenting of tree nodes so demo columns can use the full window width.");
    if (disable_indent)
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);

    // Basic columns
    if (ImGui::TreeNode("Basic"))
    {
        ImGui::Text("Without border:");
        ImGui::Columns(3, "mycolumns3", false);  // 3-ways, no border
        ImGui::Separator();
        for (int n = 0; n < 14; n++)
        {
            char label[32];
            sprintf(label, "Item %d", n);
            if (ImGui::Selectable(label)) {}
            //if (ImGui::Button(label, ImVec2(-FLT_MIN,0.0f))) {}
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::Separator();

        ImGui::Text("With border:");
        ImGui::Columns(4, "mycolumns"); // 4-ways, with border
        ImGui::Separator();
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("Path"); ImGui::NextColumn();
        ImGui::Text("Hovered"); ImGui::NextColumn();
        ImGui::Separator();
        const char* names[3] = { "One", "Two", "Three" };
        const char* paths[3] = { "/path/one", "/path/two", "/path/three" };
        static int selected = -1;
        for (int i = 0; i < 3; i++)
        {
            char label[32];
            sprintf(label, "%04d", i);
            if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
                selected = i;
            bool hovered = ImGui::IsItemHovered();
            ImGui::NextColumn();
            ImGui::Text(names[i]); ImGui::NextColumn();
            ImGui::Text(paths[i]); ImGui::NextColumn();
            ImGui::Text("%d", hovered); ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Borders"))
    {
        // NB: Future columns API should allow automatic horizontal borders.
        static bool h_borders = true;
        static bool v_borders = true;
        static int columns_count = 4;
        const int lines_count = 3;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::DragInt("##columns_count", &columns_count, 0.1f, 2, 10, "%d columns");
        if (columns_count < 2)
            columns_count = 2;
        ImGui::SameLine();
        ImGui::Checkbox("horizontal", &h_borders);
        ImGui::SameLine();
        ImGui::Checkbox("vertical", &v_borders);
        ImGui::Columns(columns_count, NULL, v_borders);
        for (int i = 0; i < columns_count * lines_count; i++)
        {
            if (h_borders && ImGui::GetColumnIndex() == 0)
                ImGui::Separator();
            ImGui::Text("%c%c%c", 'a' + i, 'a' + i, 'a' + i);
            ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
            ImGui::Text("Avail %.2f", ImGui::GetContentRegionAvail().x);
            ImGui::Text("Offset %.2f", ImGui::GetColumnOffset());
            ImGui::Text("Long text that is likely to clip");
            ImGui::Button("Button", ImVec2(-FLT_MIN, 0.0f));
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
        if (h_borders)
            ImGui::Separator();
        ImGui::TreePop();
    }

    // Create multiple items in a same cell before switching to next column
    if (ImGui::TreeNode("Mixed items"))
    {
        ImGui::Columns(3, "mixed");
        ImGui::Separator();

        ImGui::Text("Hello");
        ImGui::Button("Banana");
        ImGui::NextColumn();

        ImGui::Text("ImGui");
        ImGui::Button("Apple");
        static float foo = 1.0f;
        ImGui::InputFloat("red", &foo, 0.05f, 0, "%.3f");
        ImGui::Text("An extra line here.");
        ImGui::NextColumn();

        ImGui::Text("Sailor");
        ImGui::Button("Corniflower");
        static float bar = 1.0f;
        ImGui::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
        ImGui::NextColumn();

        if (ImGui::CollapsingHeader("Category A")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
        if (ImGui::CollapsingHeader("Category B")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
        if (ImGui::CollapsingHeader("Category C")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::TreePop();
    }

    // Word wrapping
    if (ImGui::TreeNode("Word-wrapping"))
    {
        ImGui::Columns(2, "word-wrapping");
        ImGui::Separator();
        ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
        ImGui::TextWrapped("Hello Left");
        ImGui::NextColumn();
        ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
        ImGui::TextWrapped("Hello Right");
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::TreePop();
    }

    // Scrolling columns
    /*
    if (ImGui::TreeNode("Vertical Scrolling"))
    {
        ImGui::BeginChild("##header", ImVec2(0, ImGui::GetTextLineHeightWithSpacing()+ImGui::GetStyle().ItemSpacing.y));
        ImGui::Columns(3);
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("Path"); ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::EndChild();
        ImGui::BeginChild("##scrollingregion", ImVec2(0, 60));
        ImGui::Columns(3);
        for (int i = 0; i < 10; i++)
        {
            ImGui::Text("%04d", i); ImGui::NextColumn();
            ImGui::Text("Foobar"); ImGui::NextColumn();
            ImGui::Text("/path/foobar/%04d/", i); ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::TreePop();
    }
    */

    if (ImGui::TreeNode("Horizontal Scrolling"))
    {
        ImGui::SetNextWindowContentSize(ImVec2(1500.0f, 0.0f));
        ImGui::BeginChild("##ScrollingRegion", ImVec2(0, ImGui::GetFontSize() * 20), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Columns(10);
        int ITEMS_COUNT = 2000;
        ImGuiListClipper clipper(ITEMS_COUNT);  // Also demonstrate using the clipper for large list
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                for (int j = 0; j < 10; j++)
                {
                    ImGui::Text("Line %d Column %d...", i, j);
                    ImGui::NextColumn();
                }
        }
        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Tree"))
    {
        ImGui::Columns(2, "tree", true);
        for (int x = 0; x < 3; x++)
        {
            bool open1 = ImGui::TreeNode((void*)(intptr_t)x, "Node%d", x);
            ImGui::NextColumn();
            ImGui::Text("Node contents");
            ImGui::NextColumn();
            if (open1)
            {
                for (int y = 0; y < 3; y++)
                {
                    bool open2 = ImGui::TreeNode((void*)(intptr_t)y, "Node%d.%d", x, y);
                    ImGui::NextColumn();
                    ImGui::Text("Node contents");
                    if (open2)
                    {
                        ImGui::Text("Even more contents");
                        if (ImGui::TreeNode("Tree in column"))
                        {
                            ImGui::Text("The quick brown fox jumps over the lazy dog");
                            ImGui::TreePop();
                        }
                    }
                    ImGui::NextColumn();
                    if (open2)
                        ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
        ImGui::Columns(1);
        ImGui::TreePop();
    }

    if (disable_indent)
        ImGui::PopStyleVar();
    ImGui::PopID();
}

static void ShowDemoWindowMisc()
{
    if (ImGui::CollapsingHeader("Filtering"))
    {
        // Helper class to easy setup a text filter.
        // You may want to implement a more feature-full filtering scheme in your own application.
        static ImGuiTextFilter filter;
        ImGui::Text("Filter usage:\n"
                    "  \"\"         display all lines\n"
                    "  \"xxx\"      display lines containing \"xxx\"\n"
                    "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
                    "  \"-xxx\"     hide lines containing \"xxx\"");
        filter.Draw();
        const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
        for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
            if (filter.PassFilter(lines[i]))
                ImGui::BulletText("%s", lines[i]);
    }

    if (ImGui::CollapsingHeader("Inputs, Navigation & Focus"))
    {
        ImGuiIO& io = ImGui::GetIO();

        // Display ImGuiIO output flags
        ImGui::Text("WantCaptureMouse: %d", io.WantCaptureMouse);
        ImGui::Text("WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
        ImGui::Text("WantTextInput: %d", io.WantTextInput);
        ImGui::Text("WantSetMousePos: %d", io.WantSetMousePos);
        ImGui::Text("NavActive: %d, NavVisible: %d", io.NavActive, io.NavVisible);

        // Display Keyboard/Mouse state
        if (ImGui::TreeNode("Keyboard, Mouse & Navigation State"))
        {
            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse pos: <INVALID>");
            ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f)   { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i))          { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i))         { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

            ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f)     { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
            ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i))             { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i))            { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
            ImGui::Text("Chars queue:");    for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

            ImGui::Text("NavInputs down:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputs[i] > 0.0f)                    { ImGui::SameLine(); ImGui::Text("[%d] %.2f", i, io.NavInputs[i]); }
            ImGui::Text("NavInputs pressed:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] == 0.0f)    { ImGui::SameLine(); ImGui::Text("[%d]", i); }
            ImGui::Text("NavInputs duration:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] >= 0.0f)   { ImGui::SameLine(); ImGui::Text("[%d] %.2f", i, io.NavInputsDownDuration[i]); }

            ImGui::Button("Hovering me sets the\nkeyboard capture flag");
            if (ImGui::IsItemHovered())
                ImGui::CaptureKeyboardFromApp(true);
            ImGui::SameLine();
            ImGui::Button("Holding me clears the\nthe keyboard capture flag");
            if (ImGui::IsItemActive())
                ImGui::CaptureKeyboardFromApp(false);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Tabbing"))
        {
            ImGui::Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
            static char buf[32] = "dummy";
            ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
            ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
            ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
            ImGui::PushAllowKeyboardFocus(false);
            ImGui::InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
            //ImGui::SameLine(); HelpMarker("Use ImGui::PushAllowKeyboardFocus(bool)\nto disable tabbing through certain widgets.");
            ImGui::PopAllowKeyboardFocus();
            ImGui::InputText("5", buf, IM_ARRAYSIZE(buf));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Focus from code"))
        {
            bool focus_1 = ImGui::Button("Focus on 1"); ImGui::SameLine();
            bool focus_2 = ImGui::Button("Focus on 2"); ImGui::SameLine();
            bool focus_3 = ImGui::Button("Focus on 3");
            int has_focus = 0;
            static char buf[128] = "click on a button to set focus";

            if (focus_1) ImGui::SetKeyboardFocusHere();
            ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive()) has_focus = 1;

            if (focus_2) ImGui::SetKeyboardFocusHere();
            ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive()) has_focus = 2;

            ImGui::PushAllowKeyboardFocus(false);
            if (focus_3) ImGui::SetKeyboardFocusHere();
            ImGui::InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive()) has_focus = 3;
            ImGui::PopAllowKeyboardFocus();

            if (has_focus)
                ImGui::Text("Item with focus: %d", has_focus);
            else
                ImGui::Text("Item with focus: <none>");

            // Use >= 0 parameter to SetKeyboardFocusHere() to focus an upcoming item
            static float f3[3] = { 0.0f, 0.0f, 0.0f };
            int focus_ahead = -1;
            if (ImGui::Button("Focus on X")) { focus_ahead = 0; } ImGui::SameLine();
            if (ImGui::Button("Focus on Y")) { focus_ahead = 1; } ImGui::SameLine();
            if (ImGui::Button("Focus on Z")) { focus_ahead = 2; }
            if (focus_ahead != -1) ImGui::SetKeyboardFocusHere(focus_ahead);
            ImGui::SliderFloat3("Float3", &f3[0], 0.0f, 1.0f);

            ImGui::TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code.");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Dragging"))
        {
            ImGui::TextWrapped("You can use ImGui::GetMouseDragDelta(0) to query for the dragged amount on any widget.");
            for (int button = 0; button < 3; button++)
                ImGui::Text("IsMouseDragging(%d):\n  w/ default threshold: %d,\n  w/ zero threshold: %d\n  w/ large threshold: %d",
                    button, ImGui::IsMouseDragging(button), ImGui::IsMouseDragging(button, 0.0f), ImGui::IsMouseDragging(button, 20.0f));

            ImGui::Button("Drag Me");
            if (ImGui::IsItemActive())
                ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor

            // Drag operations gets "unlocked" when the mouse has moved past a certain threshold (the default threshold is stored in io.MouseDragThreshold)
            // You can request a lower or higher threshold using the second parameter of IsMouseDragging() and GetMouseDragDelta()
            ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
            ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
            ImVec2 mouse_delta = io.MouseDelta;
            ImGui::Text("GetMouseDragDelta(0):\n  w/ default threshold: (%.1f, %.1f),\n  w/ zero threshold: (%.1f, %.1f)\nMouseDelta: (%.1f, %.1f)", value_with_lock_threshold.x, value_with_lock_threshold.y, value_raw.x, value_raw.y, mouse_delta.x, mouse_delta.y);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Mouse cursors"))
        {
            const char* mouse_cursors_names[] = { "Arrow", "TextInput", "Move", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand" };
            IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == ImGuiMouseCursor_COUNT);

            ImGui::Text("Current mouse cursor = %d: %s", ImGui::GetMouseCursor(), mouse_cursors_names[ImGui::GetMouseCursor()]);
            ImGui::Text("Hover to see mouse cursors:");
            ImGui::SameLine(); HelpMarker("Your application can render a different mouse cursor based on what ImGui::GetMouseCursor() returns. If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, otherwise your backend needs to handle it.");
            for (int i = 0; i < ImGuiMouseCursor_COUNT; i++)
            {
                char label[32];
                sprintf(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
                ImGui::Bullet(); ImGui::Selectable(label, false);
                if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
                    ImGui::SetMouseCursor(i);
            }
            ImGui::TreePop();
        }
    }
}

//-----------------------------------------------------------------------------
// [SECTION] About Window / ShowAboutWindow()
// Access from Dear ImGui Demo -> Tools -> About
//-----------------------------------------------------------------------------

void ImGui::ShowAboutWindow(bool* p_open)
{
    if (!ImGui::Begin("About Dear ImGui", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
    ImGui::Separator();
    ImGui::Text("By Omar Cornut and all Dear ImGui contributors.");
    ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");

    static bool show_config_info = false;
    ImGui::Checkbox("Config/Build Information", &show_config_info);
    if (show_config_info)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();

        bool copy_to_clipboard = ImGui::Button("Copy to clipboard");
        ImGui::BeginChildFrame(ImGui::GetID("cfginfos"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 18), ImGuiWindowFlags_NoMove);
        if (copy_to_clipboard)
        {
            ImGui::LogToClipboard();
            ImGui::LogText("```\n"); // Back quotes will make the text appears without formatting when pasting to GitHub
        }

        ImGui::Text("Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
        ImGui::Separator();
        ImGui::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));
        ImGui::Text("define: __cplusplus=%d", (int)__cplusplus);
#ifdef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_OBSOLETE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_WIN32_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_MATH_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_MATH_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_ALLOCATORS
        ImGui::Text("define: IMGUI_DISABLE_DEFAULT_ALLOCATORS");
#endif
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
        ImGui::Text("define: IMGUI_USE_BGRA_PACKED_COLOR");
#endif
#ifdef _WIN32
        ImGui::Text("define: _WIN32");
#endif
#ifdef _WIN64
        ImGui::Text("define: _WIN64");
#endif
#ifdef __linux__
        ImGui::Text("define: __linux__");
#endif
#ifdef __APPLE__
        ImGui::Text("define: __APPLE__");
#endif
#ifdef _MSC_VER
        ImGui::Text("define: _MSC_VER=%d", _MSC_VER);
#endif
#ifdef __MINGW32__
        ImGui::Text("define: __MINGW32__");
#endif
#ifdef __MINGW64__
        ImGui::Text("define: __MINGW64__");
#endif
#ifdef __GNUC__
        ImGui::Text("define: __GNUC__=%d", (int)__GNUC__);
#endif
#ifdef __clang_version__
        ImGui::Text("define: __clang_version__=%s", __clang_version__);
#endif
        ImGui::Separator();
        ImGui::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
        ImGui::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
        ImGui::Text("io.ConfigFlags: 0x%08X", io.ConfigFlags);
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard)        ImGui::Text(" NavEnableKeyboard");
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)         ImGui::Text(" NavEnableGamepad");
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos)     ImGui::Text(" NavEnableSetMousePos");
        if (io.ConfigFlags & ImGuiConfigFlags_NavNoCaptureKeyboard)     ImGui::Text(" NavNoCaptureKeyboard");
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)                  ImGui::Text(" NoMouse");
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)      ImGui::Text(" NoMouseCursorChange");
        if (io.MouseDrawCursor)                                         ImGui::Text("io.MouseDrawCursor");
        if (io.ConfigMacOSXBehaviors)                                   ImGui::Text("io.ConfigMacOSXBehaviors");
        if (io.ConfigInputTextCursorBlink)                              ImGui::Text("io.ConfigInputTextCursorBlink");
        if (io.ConfigWindowsResizeFromEdges)                            ImGui::Text("io.ConfigWindowsResizeFromEdges");
        if (io.ConfigWindowsMoveFromTitleBarOnly)                       ImGui::Text("io.ConfigWindowsMoveFromTitleBarOnly");
        if (io.ConfigWindowsMemoryCompactTimer >= 0.0f)                 ImGui::Text("io.ConfigWindowsMemoryCompactTimer = %.1ff", io.ConfigWindowsMemoryCompactTimer);
        ImGui::Text("io.BackendFlags: 0x%08X", io.BackendFlags);
        if (io.BackendFlags & ImGuiBackendFlags_HasGamepad)             ImGui::Text(" HasGamepad");
        if (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)        ImGui::Text(" HasMouseCursors");
        if (io.BackendFlags & ImGuiBackendFlags_HasSetMousePos)         ImGui::Text(" HasSetMousePos");
        if (io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset)   ImGui::Text(" RendererHasVtxOffset");
        ImGui::Separator();
        ImGui::Text("io.Fonts: %d fonts, Flags: 0x%08X, TexSize: %d,%d", io.Fonts->Fonts.Size, io.Fonts->Flags, io.Fonts->TexWidth, io.Fonts->TexHeight);
        ImGui::Text("io.DisplaySize: %.2f,%.2f", io.DisplaySize.x, io.DisplaySize.y);
        ImGui::Text("io.DisplayFramebufferScale: %.2f,%.2f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImGui::Separator();
        ImGui::Text("style.WindowPadding: %.2f,%.2f", style.WindowPadding.x, style.WindowPadding.y);
        ImGui::Text("style.WindowBorderSize: %.2f", style.WindowBorderSize);
        ImGui::Text("style.FramePadding: %.2f,%.2f", style.FramePadding.x, style.FramePadding.y);
        ImGui::Text("style.FrameRounding: %.2f", style.FrameRounding);
        ImGui::Text("style.FrameBorderSize: %.2f", style.FrameBorderSize);
        ImGui::Text("style.ItemSpacing: %.2f,%.2f", style.ItemSpacing.x, style.ItemSpacing.y);
        ImGui::Text("style.ItemInnerSpacing: %.2f,%.2f", style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);

        if (copy_to_clipboard)
        {
            ImGui::LogText("\n```\n");
            ImGui::LogFinish();
        }
        ImGui::EndChildFrame();
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Style Editor / ShowStyleEditor()
//-----------------------------------------------------------------------------
// - ShowStyleSelector()
// - ShowFontSelector()
// - ShowStyleEditor()
//-----------------------------------------------------------------------------

// Demo helper function to select among default colors. See ShowStyleEditor() for more advanced options.
// Here we use the simplified Combo() api that packs items into a single literal string. Useful for quick combo boxes where the choices are known locally.
bool ImGui::ShowStyleSelector(const char* label)
{
    static int style_idx = -1;
    if (ImGui::Combo(label, &style_idx, "Classic\0Dark\0Light\0"))
    {
        switch (style_idx)
        {
        case 0: ImGui::StyleColorsClassic(); break;
        case 1: ImGui::StyleColorsDark(); break;
        case 2: ImGui::StyleColorsLight(); break;
        }
        return true;
    }
    return false;
}

// Demo helper function to select among loaded fonts.
// Here we use the regular BeginCombo()/EndCombo() api which is more the more flexible one.
void ImGui::ShowFontSelector(const char* label)
{
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font_current = ImGui::GetFont();
    if (ImGui::BeginCombo(label, font_current->GetDebugName()))
    {
        for (int n = 0; n < io.Fonts->Fonts.Size; n++)
        {
            ImFont* font = io.Fonts->Fonts[n];
            ImGui::PushID((void*)font);
            if (ImGui::Selectable(font->GetDebugName(), font == font_current))
                io.FontDefault = font;
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    HelpMarker(
        "- Load additional fonts with io.Fonts->AddFontFromFileTTF().\n"
        "- The font atlas is built when calling io.Fonts->GetTexDataAsXXXX() or io.Fonts->Build().\n"
        "- Read FAQ and documentation in misc/fonts/ for more details.\n"
        "- If you need to add/remove fonts at runtime (e.g. for DPI change), do it before calling NewFrame().");
}

void ImGui::ShowStyleEditor(ImGuiStyle* ref)
{
    // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to an internally stored reference)
    ImGuiStyle& style = ImGui::GetStyle();
    static ImGuiStyle ref_saved_style;

    // Default to using internal storage as reference
    static bool init = true;
    if (init && ref == NULL)
        ref_saved_style = style;
    init = false;
    if (ref == NULL)
        ref = &ref_saved_style;

    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

    if (ImGui::ShowStyleSelector("Colors##Selector"))
        ref_saved_style = style;
    ImGui::ShowFontSelector("Fonts##Selector");

    // Simplified Settings
    if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
        style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
    { bool window_border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &window_border)) style.WindowBorderSize = window_border ? 1.0f : 0.0f; }
    ImGui::SameLine();
    { bool frame_border = (style.FrameBorderSize > 0.0f); if (ImGui::Checkbox("FrameBorder", &frame_border)) style.FrameBorderSize = frame_border ? 1.0f : 0.0f; }
    ImGui::SameLine();
    { bool popup_border = (style.PopupBorderSize > 0.0f); if (ImGui::Checkbox("PopupBorder", &popup_border)) style.PopupBorderSize = popup_border ? 1.0f : 0.0f; }

    // Save/Revert button
    if (ImGui::Button("Save Ref"))
        *ref = ref_saved_style = style;
    ImGui::SameLine();
    if (ImGui::Button("Revert Ref"))
        style = *ref;
    ImGui::SameLine();
    HelpMarker("Save/Revert in local non-persistent storage. Default Colors definition are not affected. Use \"Export Colors\" below to save them somewhere.");

    ImGui::Separator();

    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Sizes"))
        {
            ImGui::Text("Main");
            ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
            ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
            ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
            ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
            ImGui::Text("Borders");
            ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::Text("Rounding");
            ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
            ImGui::Text("Alignment");
            ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
            int window_menu_button_position = style.WindowMenuButtonPosition + 1;
            if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                style.WindowMenuButtonPosition = window_menu_button_position - 1;
            ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
            ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f"); ImGui::SameLine(); HelpMarker("Alignment applies when a button is larger than its text content.");
            ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f"); ImGui::SameLine(); HelpMarker("Alignment applies when a selectable is larger than its text content.");
            ImGui::Text("Safe Area Padding"); ImGui::SameLine(); HelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
            ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Colors"))
        {
            static int output_dest = 0;
            static bool output_only_modified = true;
            if (ImGui::Button("Export Unsaved"))
            {
                if (output_dest == 0)
                    ImGui::LogToClipboard();
                else
                    ImGui::LogToTTY();
                ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;" IM_NEWLINE);
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const ImVec4& col = style.Colors[i];
                    const char* name = ImGui::GetStyleColorName(i);
                    if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                        ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE, name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
                }
                ImGui::LogFinish();
            }
            ImGui::SameLine(); ImGui::SetNextItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0");
            ImGui::SameLine(); ImGui::Checkbox("Only Modified Colors", &output_only_modified);

            static ImGuiTextFilter filter;
            filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

            static ImGuiColorEditFlags alpha_flags = 0;
            ImGui::RadioButton("Opaque", &alpha_flags, 0); ImGui::SameLine();
            ImGui::RadioButton("Alpha", &alpha_flags, ImGuiColorEditFlags_AlphaPreview); ImGui::SameLine();
            ImGui::RadioButton("Both", &alpha_flags, ImGuiColorEditFlags_AlphaPreviewHalf); ImGui::SameLine();
            HelpMarker("In the color list:\nLeft-click on colored square to open color picker,\nRight-click to open edit options menu.");

            ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
            ImGui::PushItemWidth(-160);
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const char* name = ImGui::GetStyleColorName(i);
                if (!filter.PassFilter(name))
                    continue;
                ImGui::PushID(i);
                ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
                {
                    // Tips: in a real user application, you may want to merge and use an icon font into the main font, so instead of "Save"/"Revert" you'd use icons.
                    // Read the FAQ and misc/fonts/README.txt about using icon fonts. It's really easy and super convenient!
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) ref->Colors[i] = style.Colors[i];
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) style.Colors[i] = ref->Colors[i];
                }
                ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                ImGui::TextUnformatted(name);
                ImGui::PopID();
            }
            ImGui::PopItemWidth();
            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Fonts"))
        {
            ImGuiIO& io = ImGui::GetIO();
            ImFontAtlas* atlas = io.Fonts;
            HelpMarker("Read FAQ and misc/fonts/README.txt for details on font loading.");
            ImGui::PushItemWidth(120);
            for (int i = 0; i < atlas->Fonts.Size; i++)
            {
                ImFont* font = atlas->Fonts[i];
                ImGui::PushID(font);
                bool font_details_opened = ImGui::TreeNode(font, "Font %d: \"%s\"\n%.2f px, %d glyphs, %d file(s)", i, font->ConfigData ? font->ConfigData[0].Name : "", font->FontSize, font->Glyphs.Size, font->ConfigDataCount);
                ImGui::SameLine(); if (ImGui::SmallButton("Set as default")) { io.FontDefault = font; }
                if (font_details_opened)
                {
                    ImGui::PushFont(font);
                    ImGui::Text("The quick brown fox jumps over the lazy dog");
                    ImGui::PopFont();
                    ImGui::DragFloat("Font scale", &font->Scale, 0.005f, 0.3f, 2.0f, "%.1f");   // Scale only this font
                    ImGui::SameLine(); HelpMarker("Note than the default embedded font is NOT meant to be scaled.\n\nFont are currently rendered into bitmaps at a given size at the time of building the atlas. You may oversample them to get some flexibility with scaling. You can also render at multiple sizes and select which one to use at runtime.\n\n(Glimmer of hope: the atlas system should hopefully be rewritten in the future to make scaling more natural and automatic.)");
                    ImGui::InputFloat("Font offset", &font->DisplayOffset.y, 1, 1, "%.0f");
                    ImGui::Text("Ascent: %f, Descent: %f, Height: %f", font->Ascent, font->Descent, font->Ascent - font->Descent);
                    ImGui::Text("Fallback character: '%c' (U+%04X)", font->FallbackChar, font->FallbackChar);
                    ImGui::Text("Ellipsis character: '%c' (U+%04X)", font->EllipsisChar, font->EllipsisChar);
                    const float surface_sqrt = sqrtf((float)font->MetricsTotalSurface);
                    ImGui::Text("Texture surface: %d pixels (approx) ~ %dx%d", font->MetricsTotalSurface, (int)surface_sqrt, (int)surface_sqrt);
                    for (int config_i = 0; config_i < font->ConfigDataCount; config_i++)
                        if (const ImFontConfig* cfg = &font->ConfigData[config_i])
                            ImGui::BulletText("Input %d: \'%s\', Oversample: (%d,%d), PixelSnapH: %d", config_i, cfg->Name, cfg->OversampleH, cfg->OversampleV, cfg->PixelSnapH);
                    if (ImGui::TreeNode("Glyphs", "Glyphs (%d)", font->Glyphs.Size))
                    {
                        // Display all glyphs of the fonts in separate pages of 256 characters
                        for (int base = 0; base < 0x10000; base += 256)
                        {
                            int count = 0;
                            for (int n = 0; n < 256; n++)
                                count += font->FindGlyphNoFallback((ImWchar)(base + n)) ? 1 : 0;
                            if (count > 0 && ImGui::TreeNode((void*)(intptr_t)base, "U+%04X..U+%04X (%d %s)", base, base + 255, count, count > 1 ? "glyphs" : "glyph"))
                            {
                                float cell_size = font->FontSize * 1;
                                float cell_spacing = style.ItemSpacing.y;
                                ImVec2 base_pos = ImGui::GetCursorScreenPos();
                                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                                for (int n = 0; n < 256; n++)
                                {
                                    ImVec2 cell_p1(base_pos.x + (n % 16) * (cell_size + cell_spacing), base_pos.y + (n / 16) * (cell_size + cell_spacing));
                                    ImVec2 cell_p2(cell_p1.x + cell_size, cell_p1.y + cell_size);
                                    const ImFontGlyph* glyph = font->FindGlyphNoFallback((ImWchar)(base + n));
                                    draw_list->AddRect(cell_p1, cell_p2, glyph ? IM_COL32(255, 255, 255, 100) : IM_COL32(255, 255, 255, 50));
                                    if (glyph)
                                        font->RenderChar(draw_list, cell_size, cell_p1, ImGui::GetColorU32(ImGuiCol_Text), (ImWchar)(base + n)); // We use ImFont::RenderChar as a shortcut because we don't have UTF-8 conversion functions available to generate a string.
                                    if (glyph && ImGui::IsMouseHoveringRect(cell_p1, cell_p2))
                                    {
                                        ImGui::BeginTooltip();
                                        ImGui::Text("Codepoint: U+%04X", base + n);
                                        ImGui::Separator();
                                        ImGui::Text("AdvanceX: %.1f", glyph->AdvanceX);
                                        ImGui::Text("Pos: (%.2f,%.2f)->(%.2f,%.2f)", glyph->X0, glyph->Y0, glyph->X1, glyph->Y1);
                                        ImGui::Text("UV: (%.3f,%.3f)->(%.3f,%.3f)", glyph->U0, glyph->V0, glyph->U1, glyph->V1);
                                        ImGui::EndTooltip();
                                    }
                                }
                                ImGui::Dummy(ImVec2((cell_size + cell_spacing) * 16, (cell_size + cell_spacing) * 16));
                                ImGui::TreePop();
                            }
                        }
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                ImGui::PopID();
            }
            if (ImGui::TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexWidth, atlas->TexHeight))
            {
                ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
                ImGui::Image(atlas->TexID, ImVec2((float)atlas->TexWidth, (float)atlas->TexHeight), ImVec2(0, 0), ImVec2(1, 1), tint_col, border_col);
                ImGui::TreePop();
            }

            HelpMarker("Those are old settings provided for convenience.\nHowever, the _correct_ way of scaling your UI is currently to reload your font at the designed size, rebuild the font atlas, and call style.ScaleAllSizes() on a reference ImGuiStyle structure.");
            static float window_scale = 1.0f;
            if (ImGui::DragFloat("window scale", &window_scale, 0.005f, 0.3f, 2.0f, "%.2f"))   // scale only this window
                ImGui::SetWindowFontScale(window_scale);
            ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, 0.3f, 2.0f, "%.2f");      // scale everything
            ImGui::PopItemWidth();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Rendering"))
        {
            ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines); ImGui::SameLine(); HelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");
            ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
            ImGui::PushItemWidth(100);
            ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, "%.2f", 2.0f);
            if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;
            ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
            ImGui::PopItemWidth();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::PopItemWidth();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
//-----------------------------------------------------------------------------
// - ShowExampleAppMainMenuBar()
// - ShowExampleMenuFile()
//-----------------------------------------------------------------------------

// Demonstrate creating a "main" fullscreen menu bar and populating it.
// Note the difference between BeginMainMenuBar() and BeginMenuBar():
// - BeginMenuBar() = menu-bar inside current window we Begin()-ed into (the window needs the ImGuiWindowFlags_MenuBar flag)
// - BeginMainMenuBar() = helper to create menu-bar-sized window at the top of the main viewport + call BeginMenuBar() into it.
static void ShowExampleAppMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

// Note that shortcuts are currently provided for display only (future version will add flags to BeginMenu to process shortcuts)
static void ShowExampleMenuFile()
{
    ImGui::MenuItem("(dummy menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}
    ImGui::Separator();
    if (ImGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        static bool b = true;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::Checkbox("Check", &b);
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Colors"))
    {
        float sz = ImGui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x+sz, p.y+sz), ImGui::GetColorU32((ImGuiCol)i));
            ImGui::Dummy(ImVec2(sz, sz));
            ImGui::SameLine();
            ImGui::MenuItem(name);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (ImGui::MenuItem("Checked", NULL, true)) {}
    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, here we are using a more C++ like approach of declaring a class to hold the data and the functions.
struct ExampleAppConsole
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;

    ExampleAppConsole()
    {
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;
        Commands.push_back("HELP");
        Commands.push_back("HISTORY");
        Commands.push_back("CLEAR");
        Commands.push_back("CLASSIFY");  // "classify" is only here to provide an example of "C"+[tab] completing to "CL" and displaying matches.
        AutoScroll = true;
        ScrollToBottom = false;
        AddLog("Welcome to Dear ImGui!");
    }
    ~ExampleAppConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }

    // Portable helpers
    static int   Stricmp(const char* str1, const char* str2)         { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
    static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
    static char* Strdup(const char *str)                             { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
    static void  Strtrim(char* str)                                  { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

    void    ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void    Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520,600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Console"))
                *p_open = false;
            ImGui::EndPopup();
        }

        ImGui::TextWrapped("This example implements a console with basic coloring, completion and history. A more elaborate implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
        ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

        // TODO: display items starting from the bottom

        if (ImGui::SmallButton("Add Dummy Text"))  { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
        if (ImGui::SmallButton("Add Dummy Error")) { AddLog("[error] something went wrong"); } ImGui::SameLine();
        if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy");
        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        ImGui::Separator();

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Options, Filter
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::Separator();

        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::Selectable("Clear")) ClearLog();
            ImGui::EndPopup();
        }

        // Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
        // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
        // You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
        // To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
        //     ImGuiListClipper clipper(Items.Size);
        //     while (clipper.Step())
        //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        // However, note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
        // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
        // and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
        // If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
        if (copy_to_clipboard)
            ImGui::LogToClipboard();
        for (int i = 0; i < Items.Size; i++)
        {
            const char* item = Items[i];
            if (!Filter.PassFilter(item))
                continue;

            // Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
            bool pop_color = false;
            if (strstr(item, "[error]"))            { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true; }
            else if (strncmp(item, "# ", 2) == 0)   { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f)); pop_color = true; }
            ImGui::TextUnformatted(item);
            if (pop_color)
                ImGui::PopStyleColor();
        }
        if (copy_to_clipboard)
            ImGui::LogFinish();

        if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        ScrollToBottom = false;

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
        {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void    ExecCommand(const char* command_line)
    {
        AddLog("# %s\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size-1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, "CLEAR") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "HELP") == 0)
        {
            AddLog("Commands:");
            for (int i = 0; i < Commands.Size; i++)
                AddLog("- %s", Commands[i]);
        }
        else if (Stricmp(command_line, "HISTORY") == 0)
        {
            int first = History.Size - 10;
            for (int i = first > 0 ? first : 0; i < History.Size; i++)
                AddLog("%3d: %s\n", i, History[i]);
        }
        else
        {
            AddLog("Unknown command: '%s'\n", command_line);
        }

        // On commad input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
    {
        ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
        return console->TextEditCallback(data);
    }

    int     TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
            {
                // Example of TEXT COMPLETION

                // Locate beginning of current word
                const char* word_end = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf)
                {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';')
                        break;
                    word_start--;
                }

                // Build a list of candidates
                ImVector<const char*> candidates;
                for (int i = 0; i < Commands.Size; i++)
                    if (Strnicmp(Commands[i], word_start, (int)(word_end-word_start)) == 0)
                        candidates.push_back(Commands[i]);

                if (candidates.Size == 0)
                {
                    // No match
                    AddLog("No match for \"%.*s\"!\n", (int)(word_end-word_start), word_start);
                }
                else if (candidates.Size == 1)
                {
                    // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
                    data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, " ");
                }
                else
                {
                    // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
                    int match_len = (int)(word_end - word_start);
                    for (;;)
                    {
                        int c = 0;
                        bool all_candidates_matches = true;
                        for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                            if (i == 0)
                                c = toupper(candidates[i][match_len]);
                            else if (c == 0 || c != toupper(candidates[i][match_len]))
                                all_candidates_matches = false;
                        if (!all_candidates_matches)
                            break;
                        match_len++;
                    }

                    if (match_len > 0)
                    {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }

                    // List matches
                    AddLog("Possible matches:\n");
                    for (int i = 0; i < candidates.Size; i++)
                        AddLog("- %s\n", candidates[i]);
                }

                break;
            }
        case ImGuiInputTextFlags_CallbackHistory:
            {
                // Example of HISTORY
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    if (HistoryPos == -1)
                        HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.Size)
                            HistoryPos = -1;
                }

                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos)
                {
                    const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
        }
        return 0;
    }
};

static void ShowExampleAppConsole(bool* p_open)
{
    static ExampleAppConsole console;
    console.Draw("Example: Console", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
//-----------------------------------------------------------------------------

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct ExampleAppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
    bool                AutoScroll;     // Keep scrolling if already at the bottom

    ExampleAppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void    Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
            Clear();
        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        if (Filter.IsActive())
        {
            // In this example we don't use the clipper when Filter is enabled.
            // This is because we don't have a random access on the result on our filter.
            // A real application processing logs with ten of thousands of entries may want to store the result of search/filter.
            // especially if the filtering function is not trivial (e.g. reg-exp).
            for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                if (Filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            // The simplest and easy way to display the entire buffer:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
            // Here we instead demonstrate using the clipper to only process lines that are within the visible area.
            // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
            // Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
            // both of which we can handle since we an array pointing to the beginning of each line of text.
            // When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
            // Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
            ImGuiListClipper clipper;
            clipper.Begin(LineOffsets.Size);
            while (clipper.Step())
            {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }
};

// Demonstrate creating a simple log window with basic filtering.
static void ShowExampleAppLog(bool* p_open)
{
    static ExampleAppLog log;

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Example: Log", p_open);
    if (ImGui::SmallButton("[Debug] Add 5 entries"))
    {
        static int counter = 0;
        for (int n = 0; n < 5; n++)
        {
            const char* categories[3] = { "info", "warn", "error" };
            const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
            log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                ImGui::GetFrameCount(), categories[counter % IM_ARRAYSIZE(categories)], ImGui::GetTime(), words[counter % IM_ARRAYSIZE(words)]);
            counter++;
        }
    }
    ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
    log.Draw("Example: Log", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
//-----------------------------------------------------------------------------

// Demonstrate create a window with multiple child windows.
static void ShowExampleAppLayout(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Example: Simple layout", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close")) *p_open = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // left
        static int selected = 0;
        ImGui::BeginChild("left pane", ImVec2(150, 0), true);
        for (int i = 0; i < 100; i++)
        {
            char label[128];
            sprintf(label, "MyObject %d", i);
            if (ImGui::Selectable(label, selected == i))
                selected = i;
        }
        ImGui::EndChild();
        ImGui::SameLine();

        // right
        ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
                ImGui::Text("MyObject: %d", selected);
                ImGui::Separator();
                if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                {
                    if (ImGui::BeginTabItem("Description"))
                    {
                        ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Details"))
                    {
                        ImGui::Text("ID: 0123456789");
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
            ImGui::EndChild();
            if (ImGui::Button("Revert")) {}
            ImGui::SameLine();
            if (ImGui::Button("Save")) {}
        ImGui::EndGroup();
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
//-----------------------------------------------------------------------------

// Demonstrate create a simple property editor.
static void ShowExampleAppPropertyEditor(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Example: Property editor", p_open))
    {
        ImGui::End();
        return;
    }

    HelpMarker("This example shows how you may implement a property editor using two columns.\nAll objects/fields data are dummies here.\nRemember that in many simple cases, you can use ImGui::SameLine(xxx) to position\nyour cursor horizontally instead of using the Columns() API.");

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
    ImGui::Columns(2);
    ImGui::Separator();

    struct funcs
    {
        static void ShowDummyObject(const char* prefix, int uid)
        {
            ImGui::PushID(uid);                      // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
            ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
            bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
            ImGui::NextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("my sailor is rich");
            ImGui::NextColumn();
            if (node_open)
            {
                static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };
                for (int i = 0; i < 8; i++)
                {
                    ImGui::PushID(i); // Use field index as identifier.
                    if (i < 2)
                    {
                        ShowDummyObject("Child", 424242);
                    }
                    else
                    {
                        // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                        ImGui::AlignTextToFramePadding();
                        ImGui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "Field_%d", i);
                        ImGui::NextColumn();
                        ImGui::SetNextItemWidth(-1);
                        if (i >= 5)
                            ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
                        else
                            ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
                        ImGui::NextColumn();
                    }
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    };

    // Iterate dummy objects with dummy members (all the same data)
    for (int obj_i = 0; obj_i < 3; obj_i++)
        funcs::ShowDummyObject("Object", obj_i);

    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
//-----------------------------------------------------------------------------

// Demonstrate/test rendering huge amount of text, and the incidence of clipping.
static void ShowExampleAppLongText(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(520,600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Example: Long text display", p_open))
    {
        ImGui::End();
        return;
    }

    static int test_type = 0;
    static ImGuiTextBuffer log;
    static int lines = 0;
    ImGui::Text("Printing unusually long amount of text.");
    ImGui::Combo("Test type", &test_type, "Single call to TextUnformatted()\0Multiple calls to Text(), clipped\0Multiple calls to Text(), not clipped (slow)\0");
    ImGui::Text("Buffer contents: %d lines, %d bytes", lines, log.size());
    if (ImGui::Button("Clear")) { log.clear(); lines = 0; }
    ImGui::SameLine();
    if (ImGui::Button("Add 1000 lines"))
    {
        for (int i = 0; i < 1000; i++)
            log.appendf("%i The quick brown fox jumps over the lazy dog\n", lines+i);
        lines += 1000;
    }
    ImGui::BeginChild("Log");
    switch (test_type)
    {
    case 0:
        // Single call to TextUnformatted() with a big buffer
        ImGui::TextUnformatted(log.begin(), log.end());
        break;
    case 1:
        {
            // Multiple calls to Text(), manually coarsely clipped - demonstrate how to use the ImGuiListClipper helper.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
            ImGuiListClipper clipper(lines);
            while (clipper.Step())
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                    ImGui::Text("%i The quick brown fox jumps over the lazy dog", i);
            ImGui::PopStyleVar();
            break;
        }
    case 2:
        // Multiple calls to Text(), not clipped (slow)
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
        for (int i = 0; i < lines; i++)
            ImGui::Text("%i The quick brown fox jumps over the lazy dog", i);
        ImGui::PopStyleVar();
        break;
    }
    ImGui::EndChild();
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
//-----------------------------------------------------------------------------

// Demonstrate creating a window which gets auto-resized according to its content.
static void ShowExampleAppAutoResize(bool* p_open)
{
    if (!ImGui::Begin("Example: Auto-resizing window", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }

    static int lines = 10;
    ImGui::Text("Window will resize every-frame to the size of its content.\nNote that you probably don't want to query the window size to\noutput your content because that would create a feedback loop.");
    ImGui::SliderInt("Number of lines", &lines, 1, 20);
    for (int i = 0; i < lines; i++)
        ImGui::Text("%*sThis is line %d", i * 4, "", i); // Pad with space to extend size horizontally
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
//-----------------------------------------------------------------------------

// Demonstrate creating a window with custom resize constraints.
static void ShowExampleAppConstrainedResize(bool* p_open)
{
    struct CustomConstraints // Helper functions to demonstrate programmatic constraints
    {
        static void Square(ImGuiSizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = (data->DesiredSize.x > data->DesiredSize.y ? data->DesiredSize.x : data->DesiredSize.y); }
        static void Step(ImGuiSizeCallbackData* data)   { float step = (float)(int)(intptr_t)data->UserData; data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step); }
    };

    static bool auto_resize = false;
    static int type = 0;
    static int display_lines = 10;
    if (type == 0) ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 0),    ImVec2(-1, FLT_MAX));      // Vertical only
    if (type == 1) ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1),    ImVec2(FLT_MAX, -1));      // Horizontal only
    if (type == 2) ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX)); // Width > 100, Height > 100
    if (type == 3) ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1),  ImVec2(500, -1));          // Width 400-500
    if (type == 4) ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 400),  ImVec2(-1, 500));          // Height 400-500
    if (type == 5) ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0),     ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square);                     // Always Square
    if (type == 6) ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0),     ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Step, (void*)(intptr_t)100); // Fixed Step

    ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
    if (ImGui::Begin("Example: Constrained Resize", p_open, flags))
    {
        const char* desc[] =
        {
            "Resize vertical only",
            "Resize horizontal only",
            "Width > 100, Height > 100",
            "Width 400-500",
            "Height 400-500",
            "Custom: Always Square",
            "Custom: Fixed Steps (100)",
        };
        if (ImGui::Button("200x200")) { ImGui::SetWindowSize(ImVec2(200, 200)); } ImGui::SameLine();
        if (ImGui::Button("500x500")) { ImGui::SetWindowSize(ImVec2(500, 500)); } ImGui::SameLine();
        if (ImGui::Button("800x200")) { ImGui::SetWindowSize(ImVec2(800, 200)); }
        ImGui::SetNextItemWidth(200);
        ImGui::Combo("Constraint", &type, desc, IM_ARRAYSIZE(desc));
        ImGui::SetNextItemWidth(200);
        ImGui::DragInt("Lines", &display_lines, 0.2f, 1, 100);
        ImGui::Checkbox("Auto-resize", &auto_resize);
        for (int i = 0; i < display_lines; i++)
            ImGui::Text("%*sHello, sailor! Making this line long enough for the example.", i * 4, "");
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple Overlay / ShowExampleAppSimpleOverlay()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple static window with no decoration + a context-menu to choose which corner of the screen to use.
static void ShowExampleAppSimpleOverlay(bool* p_open)
{
    const float DISTANCE = 8.0f;
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    if (corner != -1)
    {
        ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
       ImGui::Text("M0ne0N Project FreeType");
	   /*ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");*/
       /* if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
            ImGui::EndPopup();
        }*/
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Manipulating Window Titles / ShowExampleAppWindowTitles()
//-----------------------------------------------------------------------------

// Demonstrate using "##" and "###" in identifiers to manipulate ID generation.
// This apply to all regular items as well. Read FAQ section "How can I have multiple widgets with the same label? Can I have widget without a label? (Yes). A primer on the purpose of labels/IDs." for details.
static void ShowExampleAppWindowTitles(bool*)
{
    // By default, Windows are uniquely identified by their title.
    // You can use the "##" and "###" markers to manipulate the display/ID.

    // Using "##" to display same title but have unique identifier.
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("Same title as another window##1");
    ImGui::Text("This is window 1.\nMy title is the same as window 2, but my identifier is unique.");
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(100, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Same title as another window##2");
    ImGui::Text("This is window 2.\nMy title is the same as window 1, but my identifier is unique.");
    ImGui::End();

    // Using "###" to display a changing title but keep a static identifier "AnimatedTitle"
    char buf[128];
    sprintf(buf, "Animated title %c %d###AnimatedTitle", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3], ImGui::GetFrameCount());
    ImGui::SetNextWindowPos(ImVec2(100, 300), ImGuiCond_FirstUseEver);
    ImGui::Begin(buf);
    ImGui::Text("This window has a changing title.");
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
//-----------------------------------------------------------------------------

// Demonstrate using the low-level ImDrawList to draw custom shapes.
static void ShowExampleAppCustomRendering(bool* p_open)
{
    if (!ImGui::Begin("Example: Custom rendering", p_open))
    {
        ImGui::End();
        return;
    }

    // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of overloaded operators, etc.
    // Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your types and ImVec2/ImVec4.
    // ImGui defines overloaded operators but they are internal to imgui.cpp and not exposed outside (to avoid messing with your types)
    // In this example we are not using the maths operators!
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    if (ImGui::BeginTabBar("##TabBar"))
    {
        // Primitives
        if (ImGui::BeginTabItem("Primitives"))
        {
            static float sz = 36.0f;
            static float thickness = 3.0f;
            static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
            ImGui::DragFloat("Size", &sz, 0.2f, 2.0f, 72.0f, "%.0f");
            ImGui::DragFloat("Thickness", &thickness, 0.05f, 1.0f, 8.0f, "%.02f");
            ImGui::ColorEdit4("Color", &colf.x);
            const ImVec2 p = ImGui::GetCursorScreenPos();
            const ImU32 col = ImColor(colf);
            float x = p.x + 4.0f, y = p.y + 4.0f;
            float spacing = 10.0f;
            ImDrawCornerFlags corners_none = 0;
            ImDrawCornerFlags corners_all = ImDrawCornerFlags_All;
            ImDrawCornerFlags corners_tl_br = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotRight;
            for (int n = 0; n < 2; n++)
            {
                // First line uses a thickness of 1.0f, second line uses the configurable thickness
                float th = (n == 0) ? 1.0f : thickness;
                draw_list->AddCircle(ImVec2(x + sz*0.5f, y + sz*0.5f), sz*0.5f, col, 6, th);                x += sz + spacing;  // Hexagon
                draw_list->AddCircle(ImVec2(x + sz*0.5f, y + sz*0.5f), sz*0.5f, col, 20, th);               x += sz + spacing;  // Circle
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 0.0f,  corners_none, th);     x += sz + spacing;  // Square
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f, corners_all, th);      x += sz + spacing;  // Square with all rounded corners
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f, corners_tl_br, th);    x += sz + spacing;  // Square with two rounded corners
                draw_list->AddTriangle(ImVec2(x+sz*0.5f,y), ImVec2(x+sz, y+sz-0.5f), ImVec2(x, y+sz-0.5f), col, th);      x += sz + spacing;      // Triangle
                draw_list->AddTriangle(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col, th); x += sz*0.4f + spacing; // Thin triangle
                draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y), col, th);                               x += sz + spacing;  // Horizontal line (note: drawing a filled rectangle will be faster!)
                draw_list->AddLine(ImVec2(x, y), ImVec2(x, y + sz), col, th);                               x += spacing;       // Vertical line (note: drawing a filled rectangle will be faster!)
                draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y + sz), col, th);                          x += sz + spacing;  // Diagonal line
                draw_list->AddBezierCurve(ImVec2(x, y), ImVec2(x + sz*1.3f, y + sz*0.3f), ImVec2(x + sz - sz*1.3f, y + sz - sz*0.3f), ImVec2(x + sz, y + sz), col, th);
                x = p.x + 4;
                y += sz + spacing;
            }
            draw_list->AddCircleFilled(ImVec2(x + sz*0.5f, y + sz*0.5f), sz*0.5f, col, 6);              x += sz + spacing;  // Hexagon
            draw_list->AddCircleFilled(ImVec2(x + sz*0.5f, y + sz*0.5f), sz*0.5f, col, 32);             x += sz + spacing;  // Circle
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col);                        x += sz + spacing;  // Square
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f);                 x += sz + spacing;  // Square with all rounded corners
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f, corners_tl_br);  x += sz + spacing;  // Square with two rounded corners
            draw_list->AddTriangleFilled(ImVec2(x+sz*0.5f,y), ImVec2(x+sz, y+sz-0.5f), ImVec2(x, y+sz-0.5f), col);      x += sz + spacing;      // Triangle
            draw_list->AddTriangleFilled(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col); x += sz*0.4f + spacing; // Thin triangle
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + thickness), col);                 x += sz + spacing;  // Horizontal line (faster than AddLine, but only handle integer thickness)
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + thickness, y + sz), col);                 x += spacing*2.0f;  // Vertical line (faster than AddLine, but only handle integer thickness)
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + 1, y + 1), col);                          x += sz;            // Pixel (faster than AddLine)
            draw_list->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + sz, y + sz), IM_COL32(0, 0, 0, 255), IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 0, 255), IM_COL32(0, 255, 0, 255));
            ImGui::Dummy(ImVec2((sz + spacing) * 9.8f, (sz + spacing) * 3));
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Canvas"))
        {
            static ImVector<ImVec2> points;
            static bool adding_line = false;
            if (ImGui::Button("Clear")) points.clear();
            if (points.Size >= 2) { ImGui::SameLine(); if (ImGui::Button("Undo")) { points.pop_back(); points.pop_back(); } }
            ImGui::Text("Left-click and drag to add lines,\nRight-click to undo");

            // Here we are using InvisibleButton() as a convenience to 1) advance the cursor and 2) allows us to use IsItemHovered()
            // But you can also draw directly and poll mouse/keyboard by yourself. You can manipulate the cursor using GetCursorPos() and SetCursorPos().
            // If you only use the ImDrawList API, you can notify the owner window of its extends by using SetCursorPos(max).
            ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
            if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
            if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
            draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));
            draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(255, 255, 255, 255));

            bool adding_preview = false;
            ImGui::InvisibleButton("canvas", canvas_size);
            ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x, ImGui::GetIO().MousePos.y - canvas_pos.y);
            if (adding_line)
            {
                adding_preview = true;
                points.push_back(mouse_pos_in_canvas);
                if (!ImGui::IsMouseDown(0))
                    adding_line = adding_preview = false;
            }
            if (ImGui::IsItemHovered())
            {
                if (!adding_line && ImGui::IsMouseClicked(0))
                {
                    points.push_back(mouse_pos_in_canvas);
                    adding_line = true;
                }
                if (ImGui::IsMouseClicked(1) && !points.empty())
                {
                    adding_line = adding_preview = false;
                    points.pop_back();
                    points.pop_back();
                }
            }
            draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), true);      // clip lines within the canvas (if we resize it, etc.)
            for (int i = 0; i < points.Size - 1; i += 2)
                draw_list->AddLine(ImVec2(canvas_pos.x + points[i].x, canvas_pos.y + points[i].y), ImVec2(canvas_pos.x + points[i + 1].x, canvas_pos.y + points[i + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
            draw_list->PopClipRect();
            if (adding_preview)
                points.pop_back();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("BG/FG draw lists"))
        {
            static bool draw_bg = true;
            static bool draw_fg = true;
            ImGui::Checkbox("Draw in Background draw list", &draw_bg);
            ImGui::SameLine(); HelpMarker("The Background draw list will be rendered below every Dear ImGui windows.");
            ImGui::Checkbox("Draw in Foreground draw list", &draw_fg);
            ImGui::SameLine(); HelpMarker("The Foreground draw list will be rendered over every Dear ImGui windows.");
            ImVec2 window_pos = ImGui::GetWindowPos();
            ImVec2 window_size = ImGui::GetWindowSize();
            ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
            if (draw_bg)
                ImGui::GetBackgroundDrawList()->AddCircle(window_center, window_size.x * 0.6f, IM_COL32(255, 0, 0, 200), 48, 10+4);
            if (draw_fg)
                ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * 0.6f, IM_COL32(0, 255, 0, 200), 48, 10);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()
//-----------------------------------------------------------------------------

// Simplified structure to mimic a Document model
struct MyDocument
{
    const char* Name;           // Document title
    bool        Open;           // Set when the document is open (in this demo, we keep an array of all available documents to simplify the demo)
    bool        OpenPrev;       // Copy of Open from last update.
    bool        Dirty;          // Set when the document has been modified
    bool        WantClose;      // Set when the document
    ImVec4      Color;          // An arbitrary variable associated to the document

    MyDocument(const char* name, bool open = true, const ImVec4& color = ImVec4(1.0f,1.0f,1.0f,1.0f))
    {
        Name = name;
        Open = OpenPrev = open;
        Dirty = false;
        WantClose = false;
        Color = color;
    }
    void DoOpen()       { Open = true; }
    void DoQueueClose() { WantClose = true; }
    void DoForceClose() { Open = false; Dirty = false; }
    void DoSave()       { Dirty = false; }

    // Display dummy contents for the Document
    static void DisplayContents(MyDocument* doc)
    {
        ImGui::PushID(doc);
        ImGui::Text("Document \"%s\"", doc->Name);
        ImGui::PushStyleColor(ImGuiCol_Text, doc->Color);
        ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
        ImGui::PopStyleColor();
        if (ImGui::Button("Modify", ImVec2(100, 0)))
            doc->Dirty = true;
        ImGui::SameLine();
        if (ImGui::Button("Save", ImVec2(100, 0)))
            doc->DoSave();
        ImGui::ColorEdit3("color", &doc->Color.x);  // Useful to test drag and drop and hold-dragged-to-open-tab behavior.
        ImGui::PopID();
    }

    // Display context menu for the Document
    static void DisplayContextMenu(MyDocument* doc)
    {
        if (!ImGui::BeginPopupContextItem())
            return;

        char buf[256];
        sprintf(buf, "Save %s", doc->Name);
        if (ImGui::MenuItem(buf, "CTRL+S", false, doc->Open))
            doc->DoSave();
        if (ImGui::MenuItem("Close", "CTRL+W", false, doc->Open))
            doc->DoQueueClose();
        ImGui::EndPopup();
    }
};

struct ExampleAppDocuments
{
    ImVector<MyDocument> Documents;

    ExampleAppDocuments()
    {
        Documents.push_back(MyDocument("Lettuce",             true,  ImVec4(0.4f, 0.8f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument("Eggplant",            true,  ImVec4(0.8f, 0.5f, 1.0f, 1.0f)));
        Documents.push_back(MyDocument("Carrot",              true,  ImVec4(1.0f, 0.8f, 0.5f, 1.0f)));
        Documents.push_back(MyDocument("Tomato",              false, ImVec4(1.0f, 0.3f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument("A Rather Long Title", false));
        Documents.push_back(MyDocument("Some Document",       false));
    }
};

// [Optional] Notify the system of Tabs/Windows closure that happened outside the regular tab interface.
// If a tab has been closed programmatically (aka closed from another source such as the Checkbox() in the demo, as opposed
// to clicking on the regular tab closing button) and stops being submitted, it will take a frame for the tab bar to notice its absence.
// During this frame there will be a gap in the tab bar, and if the tab that has disappeared was the selected one, the tab bar
// will report no selected tab during the frame. This will effectively give the impression of a flicker for one frame.
// We call SetTabItemClosed() to manually notify the Tab Bar or Docking system of removed tabs to avoid this glitch.
// Note that this completely optional, and only affect tab bars with the ImGuiTabBarFlags_Reorderable flag.
static void NotifyOfDocumentsClosedElsewhere(ExampleAppDocuments& app)
{
    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
    {
        MyDocument* doc = &app.Documents[doc_n];
        if (!doc->Open && doc->OpenPrev)
            ImGui::SetTabItemClosed(doc->Name);
        doc->OpenPrev = doc->Open;
    }
}

void ShowExampleAppDocuments(bool* p_open)
{
    static ExampleAppDocuments app;

    // Options
    static bool opt_reorderable = true;
    static ImGuiTabBarFlags opt_fitting_flags = ImGuiTabBarFlags_FittingPolicyDefault_;

    if (!ImGui::Begin("Example: Documents", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            int open_count = 0;
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                open_count += app.Documents[doc_n].Open ? 1 : 0;

            if (ImGui::BeginMenu("Open", open_count < app.Documents.Size))
            {
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                {
                    MyDocument* doc = &app.Documents[doc_n];
                    if (!doc->Open)
                        if (ImGui::MenuItem(doc->Name))
                            doc->DoOpen();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Close All Documents", NULL, false, open_count > 0))
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                    app.Documents[doc_n].DoQueueClose();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // [Debug] List documents with one checkbox for each
    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
    {
        MyDocument* doc = &app.Documents[doc_n];
        if (doc_n > 0)
            ImGui::SameLine();
        ImGui::PushID(doc);
        if (ImGui::Checkbox(doc->Name, &doc->Open))
            if (!doc->Open)
                doc->DoForceClose();
        ImGui::PopID();
    }

    ImGui::Separator();

    // Submit Tab Bar and Tabs
    {
        ImGuiTabBarFlags tab_bar_flags = (opt_fitting_flags) | (opt_reorderable ? ImGuiTabBarFlags_Reorderable : 0);
        if (ImGui::BeginTabBar("##tabs", tab_bar_flags))
        {
            if (opt_reorderable)
                NotifyOfDocumentsClosedElsewhere(app);

            // [DEBUG] Stress tests
            //if ((ImGui::GetFrameCount() % 30) == 0) docs[1].Open ^= 1;            // [DEBUG] Automatically show/hide a tab. Test various interactions e.g. dragging with this on.
            //if (ImGui::GetIO().KeyCtrl) ImGui::SetTabItemSelected(docs[1].Name);  // [DEBUG] Test SetTabItemSelected(), probably not very useful as-is anyway..

            // Submit Tabs
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
            {
                MyDocument* doc = &app.Documents[doc_n];
                if (!doc->Open)
                    continue;

                ImGuiTabItemFlags tab_flags = (doc->Dirty ? ImGuiTabItemFlags_UnsavedDocument : 0);
                bool visible = ImGui::BeginTabItem(doc->Name, &doc->Open, tab_flags);

                // Cancel attempt to close when unsaved add to save queue so we can display a popup.
                if (!doc->Open && doc->Dirty)
                {
                    doc->Open = true;
                    doc->DoQueueClose();
                }

                MyDocument::DisplayContextMenu(doc);
                if (visible)
                {
                    MyDocument::DisplayContents(doc);
                    ImGui::EndTabItem();
                }
            }

            ImGui::EndTabBar();
        }
    }

    // Update closing queue
    static ImVector<MyDocument*> close_queue;
    if (close_queue.empty())
    {
        // Close queue is locked once we started a popup
        for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
        {
            MyDocument* doc = &app.Documents[doc_n];
            if (doc->WantClose)
            {
                doc->WantClose = false;
                close_queue.push_back(doc);
            }
        }
    }

    // Display closing confirmation UI
    if (!close_queue.empty())
    {
        int close_queue_unsaved_documents = 0;
        for (int n = 0; n < close_queue.Size; n++)
            if (close_queue[n]->Dirty)
                close_queue_unsaved_documents++;

        if (close_queue_unsaved_documents == 0)
        {
            // Close documents when all are unsaved
            for (int n = 0; n < close_queue.Size; n++)
                close_queue[n]->DoForceClose();
            close_queue.clear();
        }
        else
        {
            if (!ImGui::IsPopupOpen("Save?"))
                ImGui::OpenPopup("Save?");
            if (ImGui::BeginPopupModal("Save?"))
            {
                ImGui::Text("Save change to the following items?");
                ImGui::SetNextItemWidth(-1.0f);
                if (ImGui::ListBoxHeader("##", close_queue_unsaved_documents, 6))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                        if (close_queue[n]->Dirty)
                            ImGui::Text("%s", close_queue[n]->Name);
                    ImGui::ListBoxFooter();
                }

                if (ImGui::Button("Yes", ImVec2(80, 0)))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                    {
                        if (close_queue[n]->Dirty)
                            close_queue[n]->DoSave();
                        close_queue[n]->DoForceClose();
                    }
                    close_queue.clear();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("No", ImVec2(80, 0)))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                        close_queue[n]->DoForceClose();
                    close_queue.clear();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(80, 0)))
                {
                    close_queue.clear();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    ImGui::End();
}

// End of Demo code
#else

void ImGui::ShowAboutWindow(bool*) {}
void ImGui::ShowDemoWindow(bool*) {}
void ImGui::ShowUserGuide() {}
void ImGui::ShowStyleEditor(ImGuiStyle*) {}

#endif

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class AEULTVNVtdhw
{
public:
    string uclvQRMZRsPwN;

    AEULTVNVtdhw();
    double LxjSHllzwphutU();
    void fxgJc(bool hqKkZf, int iMajo);
    int YPMngzLXhJTtiaT(double HoaYjpKXKmm);
    void aKxsWqS(bool hIqJi, double fTxDbAhWll, double kVtYEjSOFe);
    string boxwGe(double lBbGycvsnPBqy, string LTcgfAFA, bool ZRAYJ, string icZEz, string iutJDcLpuNPgejD);
    double xMkBNbdCFVKBMZCD();
    void QtZDjBHG(double vLioysWEgG);
protected:
    string QXxPObvydgb;
    bool IYDwHH;
    bool tWptMhMMPYiCFF;
    int QNMuGYhicxWUHQ;
    int tJyRULsYwurPrkiy;
    int osgiIGC;

    int mswWZZSmRP(double xusroajasZjJKEY, double ngkfBmaBAiW, int kbQvPYMYI, double NWWNogb);
    string JbWEMe(double MhIiMzvSZY, double OrdcP, int kvplS);
    void WxgoDlKTunx(bool cYadljiQpgmW, bool XfhFanpC, string RbNREPavfiKsRpz, double ilCwMyfGXFRvZ, string DwUgzTgTUXTZJLi);
    double swTOHKNfMOZesnpa(string haIbHwpbfRXJhjQ, bool IlCrBThOO, bool FklIL);
    double TmQhZZmHbUqf();
private:
    int PtiYfGEkVIP;
    double ehAwvLqVoTMeM;
    string MkBQmGbdPD;
    int dBUObwpIpXQHeYfp;

    bool PIwaJJbURCDYt(string OrPOpzEzwJeBD, bool JxcoHOZa, int tKICfbj);
    bool zKUizbuOxqre();
};

double AEULTVNVtdhw::LxjSHllzwphutU()
{
    bool PPmlMKfHVnjFikb = false;
    double HsIeImZMGhZUEN = -343366.7509473859;
    int DFhIAdQaezGHMan = -938168569;
    double gSTxWonuXlIbFf = -781309.5849180664;
    double XKhUAbVCgEHZer = 1007477.2129786815;
    string dITXMIuETgk = string("VucJMs");
    bool urXfyFjGoYIAK = false;
    bool opfWaEcAYOB = true;
    int bfkELzTQ = 1642568464;

    return XKhUAbVCgEHZer;
}

void AEULTVNVtdhw::fxgJc(bool hqKkZf, int iMajo)
{
    int FEiOvKY = -1728099912;
    double xAKiaXiP = -233757.9575004859;
    string iJpzzIHHSnnMJd = string("kBSygLFCzFNVCcWoZGCvFeZehWJVPSTtGobIsYPbBbzObeabuLLovyDKRuOg");
    double vIglrYZzJ = 786008.9959401215;
    bool CVTBsSelONmGua = false;
    double ZDDWRxdbl = 995996.598358359;
    double DClcQG = -224624.39221588874;
}

int AEULTVNVtdhw::YPMngzLXhJTtiaT(double HoaYjpKXKmm)
{
    string vavWPMtQWn = string("ueeBaXYEmbxDwZsfLpuUjGzjpKgBNtziRwiwWhwiquDFbqmARIUxOFvxOHEkGTYiQKzYVZJXMpVtDgnjUpEIGbtrAxHPpxoyPvMDYZyueSPUpJdEwOZyvqpNnfkoTzLJSKqaOwxXvEvHFQyPfMweDjfkaWwVgXuhsxZqeqlQNOnzrLmGJSJbFEiDjbwPDzRISShgoElPSxbYzUtpzgHPZEPrBPYEWXK");
    string qRZdMsSdYcomJdqE = string("KmjGzsiryJblxnfzNXcVjQXDCjyyjkGfDlvROvHZARmFGqpTqbLMtcVfdUreJmcvXnTFgJMHivFoPxoZAuDIBWHKJynnqLcfEAeTziCGlMGmOUhruQGZxEHYNoKiOqGciHaCVjldRjSMVIgtLWsRQQxrjKGkKkObfymAfgwQoGwEpyjCHcnhayetNXimKiQrTlxHafnVBeZqDiLgFeQIXNPkloplJBtdxmqQGau");
    double VKvfJASHaaYEJ = -172829.93299129125;
    double VWJnVAlrbywasYHr = -93623.56381386635;
    string DXoTAiy = string("rDUAGamFGTIvYtuUpqXQhMyzC");
    string bniEaXHonkUccr = string("gTYTBCcCqbWXragdSWXmeFITQaZostRvtZvpQVeadofhnbPAlTqGFwVigWIQpnFgzBNxanpVAYDrZlOIKYAkIdxDVjQU");
    bool TeseUKJRwAsmwRw = true;
    bool FomUuujwJ = true;
    string IvLaBVQBoF = string("qqYNBDmokvJrnxilWFPJJRQUEbnEoDkMmvuaQuuVzHLXAMViddWajmvJGsdklnJBoNoYLQQT");

    return -2086189488;
}

void AEULTVNVtdhw::aKxsWqS(bool hIqJi, double fTxDbAhWll, double kVtYEjSOFe)
{
    string ShrHILxlQhvsmSm = string("VwrIXTeWqBkPgRpfmpQUpEJCEmtwJDDIjixRguOrTdUsiOJfzqsedvTZQozerMqMhzInSyTFrdqJVCmurWaGNmSOZDmibVchVOJMnrlKFhRpVhpoPKNqRXmmwsjfznBhdBCDrDddYHmvdGWLwNFYKsYFKFLPCnZIRboGoMtxRmwEVzgICgKOXcocsRYSQcE");
    bool UrSoFQxDCmT = false;
    double jtefMVrb = 254042.54118525668;
    double RLNaLELtlEvo = 563128.1696815244;
    bool FjRccavde = true;
    int VMrjdWRWXRTfDdn = 773318647;
    bool chHaPCMUwchPwHFS = false;
    double GTngyYAc = 713555.2040184148;
    bool mchbJ = false;

    for (int XNafwsUnIxQXn = 207651646; XNafwsUnIxQXn > 0; XNafwsUnIxQXn--) {
        fTxDbAhWll -= kVtYEjSOFe;
        RLNaLELtlEvo -= RLNaLELtlEvo;
    }

    if (fTxDbAhWll == 254042.54118525668) {
        for (int FznqGIjegLtznu = 768732612; FznqGIjegLtznu > 0; FznqGIjegLtznu--) {
            fTxDbAhWll /= fTxDbAhWll;
        }
    }

    for (int ZtlsBOVDzAEmo = 618704004; ZtlsBOVDzAEmo > 0; ZtlsBOVDzAEmo--) {
        GTngyYAc = jtefMVrb;
        fTxDbAhWll *= jtefMVrb;
        fTxDbAhWll = kVtYEjSOFe;
        mchbJ = ! chHaPCMUwchPwHFS;
        GTngyYAc *= fTxDbAhWll;
    }

    if (kVtYEjSOFe < -699322.9882913701) {
        for (int KTAFK = 1951189230; KTAFK > 0; KTAFK--) {
            fTxDbAhWll = RLNaLELtlEvo;
            UrSoFQxDCmT = mchbJ;
            kVtYEjSOFe = kVtYEjSOFe;
            RLNaLELtlEvo = RLNaLELtlEvo;
        }
    }

    for (int qdssniNgyg = 425721189; qdssniNgyg > 0; qdssniNgyg--) {
        GTngyYAc += RLNaLELtlEvo;
        GTngyYAc /= kVtYEjSOFe;
    }
}

string AEULTVNVtdhw::boxwGe(double lBbGycvsnPBqy, string LTcgfAFA, bool ZRAYJ, string icZEz, string iutJDcLpuNPgejD)
{
    double qMJmHZjdpttY = 806370.5854976736;
    bool jwqzdCmVOxqPGC = false;
    double mVgZmRyTTl = -20138.227120477124;
    double jQzTafuatQjmbR = 550025.9033275201;
    int KLTuXTnI = 704322064;
    int QoKPrKDxTIIAJJr = 1441675603;

    for (int evIdwxO = 820736796; evIdwxO > 0; evIdwxO--) {
        lBbGycvsnPBqy /= qMJmHZjdpttY;
        mVgZmRyTTl += mVgZmRyTTl;
        iutJDcLpuNPgejD = iutJDcLpuNPgejD;
    }

    for (int woCvLyQmoF = 853923122; woCvLyQmoF > 0; woCvLyQmoF--) {
        qMJmHZjdpttY += mVgZmRyTTl;
        lBbGycvsnPBqy /= mVgZmRyTTl;
    }

    return iutJDcLpuNPgejD;
}

double AEULTVNVtdhw::xMkBNbdCFVKBMZCD()
{
    double mrdwuSqbQaNFGQ = -482078.1869008867;
    string OmIxXH = string("vUOJtXpv");
    double dwiwhPNP = -808436.2891806575;
    double eQUSRHNkx = 149991.9507151136;
    string XdvNy = string("FxatOFWJEWuqBLsOLCiXTPuOSjaZmVJciJxBfQLyKWnDNedXmmYwnhMsbGzleaMO");
    double nqbXpW = 324387.01629859366;
    double pdNzk = -778673.2660325253;

    if (eQUSRHNkx <= -778673.2660325253) {
        for (int gIkwFpocSIItr = 1625393951; gIkwFpocSIItr > 0; gIkwFpocSIItr--) {
            mrdwuSqbQaNFGQ += eQUSRHNkx;
            nqbXpW += eQUSRHNkx;
            nqbXpW += eQUSRHNkx;
            nqbXpW *= nqbXpW;
        }
    }

    for (int BddTQFePswsvFTI = 253497172; BddTQFePswsvFTI > 0; BddTQFePswsvFTI--) {
        pdNzk /= mrdwuSqbQaNFGQ;
        pdNzk -= eQUSRHNkx;
        dwiwhPNP -= nqbXpW;
    }

    return pdNzk;
}

void AEULTVNVtdhw::QtZDjBHG(double vLioysWEgG)
{
    double xooCo = 822239.1053436158;
    string FmmgwKwgZcUs = string("aOopHzNBLwBdNOlfcQlZvHGmguhntKDfvtUnvWnHKLDPOchdHlRJeKGmjyHijGngShEZOlVMnhyuDdqcpAHcYozNmHwweFGcWqOgOKodsawoWGeI");

    if (FmmgwKwgZcUs < string("aOopHzNBLwBdNOlfcQlZvHGmguhntKDfvtUnvWnHKLDPOchdHlRJeKGmjyHijGngShEZOlVMnhyuDdqcpAHcYozNmHwweFGcWqOgOKodsawoWGeI")) {
        for (int dAxHLikbW = 2078633793; dAxHLikbW > 0; dAxHLikbW--) {
            xooCo += vLioysWEgG;
            xooCo -= xooCo;
            FmmgwKwgZcUs += FmmgwKwgZcUs;
            xooCo *= xooCo;
        }
    }
}

int AEULTVNVtdhw::mswWZZSmRP(double xusroajasZjJKEY, double ngkfBmaBAiW, int kbQvPYMYI, double NWWNogb)
{
    int KrkXjX = -1438850568;
    int GDPRsJA = 2133776442;
    int FUJfkQPuE = 577529389;
    string jHCfOktrLpwQLh = string("AXFpdrYRFwChCELEzCVaDXyRlSIxFxYEnCMgOozvCmRITjMlggMrmjgNRDqEfUbqGnHDEBylxt");
    bool TdWCTRjLgBAtI = true;
    double HdWwnqXGtFqKGG = 1018963.2741869002;
    bool TNbpqeugaKCtQ = true;
    double UxhUToJINyTI = 598801.9005448027;
    double fYdDgOYNtpjdW = 50814.28284264232;
    string enrYwHTISG = string("CtogSFmDGAiYOGiEACSUXHQMnHTlAKnUHRHIUgJyTe");

    for (int Rtyfa = 1245178936; Rtyfa > 0; Rtyfa--) {
        xusroajasZjJKEY *= xusroajasZjJKEY;
    }

    for (int qLBQjIpetc = 398784219; qLBQjIpetc > 0; qLBQjIpetc--) {
        HdWwnqXGtFqKGG += NWWNogb;
    }

    return FUJfkQPuE;
}

string AEULTVNVtdhw::JbWEMe(double MhIiMzvSZY, double OrdcP, int kvplS)
{
    bool pHFRpSRNM = true;
    double OpgKUZ = -84144.06447755623;

    if (MhIiMzvSZY <= -84144.06447755623) {
        for (int gAntzlEhNTvMJEsa = 875965253; gAntzlEhNTvMJEsa > 0; gAntzlEhNTvMJEsa--) {
            OpgKUZ = MhIiMzvSZY;
            OpgKUZ = OpgKUZ;
            MhIiMzvSZY *= MhIiMzvSZY;
            OpgKUZ += OpgKUZ;
        }
    }

    for (int ZByenK = 1042037003; ZByenK > 0; ZByenK--) {
        MhIiMzvSZY *= OrdcP;
        MhIiMzvSZY /= OrdcP;
        kvplS /= kvplS;
        OrdcP /= OrdcP;
    }

    if (MhIiMzvSZY != -364413.3646593447) {
        for (int PzCCM = 847441022; PzCCM > 0; PzCCM--) {
            OpgKUZ *= OrdcP;
            MhIiMzvSZY = MhIiMzvSZY;
        }
    }

    return string("SijJJffdzaTdjwdtTbHGCgMRUTEvODIHpFwNfVjaZfeCwVrbltHxpudpONaHCVjaLJIKqnoitcSWMNAcrkBtOsaioqINMWObFVGFSabqmDJCSyxRxNtufitFmPXGhLHgFHrdNPJgyNUITbzsoTqPDVmzoiIIaeunmyXwltpxAMsTpIIgLHYAhpzXzOnrHfikPOdfuAIMyFSqCpmBCptqdqiYyLQdnYGUayClDQslUkM");
}

void AEULTVNVtdhw::WxgoDlKTunx(bool cYadljiQpgmW, bool XfhFanpC, string RbNREPavfiKsRpz, double ilCwMyfGXFRvZ, string DwUgzTgTUXTZJLi)
{
    bool bQNAvLCYbX = true;
    bool GsoXSUKrWhS = false;
    double viycEyY = 789204.0719613621;
    bool QjswhxcDWtdndMb = false;
    double HJJpQYRFIQFZJY = 724391.9361912743;

    for (int dxXvPf = 1081312044; dxXvPf > 0; dxXvPf--) {
        HJJpQYRFIQFZJY *= HJJpQYRFIQFZJY;
        viycEyY -= ilCwMyfGXFRvZ;
        ilCwMyfGXFRvZ *= ilCwMyfGXFRvZ;
    }

    if (QjswhxcDWtdndMb == false) {
        for (int JyhaFtyxnEtIfmw = 99610279; JyhaFtyxnEtIfmw > 0; JyhaFtyxnEtIfmw--) {
            ilCwMyfGXFRvZ = viycEyY;
            QjswhxcDWtdndMb = GsoXSUKrWhS;
        }
    }

    for (int HPGXQWHx = 1246686826; HPGXQWHx > 0; HPGXQWHx--) {
        QjswhxcDWtdndMb = XfhFanpC;
    }

    for (int PHnwULfG = 1041473209; PHnwULfG > 0; PHnwULfG--) {
        XfhFanpC = GsoXSUKrWhS;
        bQNAvLCYbX = ! bQNAvLCYbX;
        HJJpQYRFIQFZJY /= viycEyY;
        cYadljiQpgmW = XfhFanpC;
    }
}

double AEULTVNVtdhw::swTOHKNfMOZesnpa(string haIbHwpbfRXJhjQ, bool IlCrBThOO, bool FklIL)
{
    double wxYkhAYNSJTfat = -487658.9423057648;
    int sjsiVfTOGoWA = -1684929056;
    bool uLSQDkiUZRtXiDoH = true;
    int VKLjzIDlaB = 1272459772;
    double PXFWXm = -676113.6478792646;
    bool mENxsXclmmaamsVQ = false;

    return PXFWXm;
}

double AEULTVNVtdhw::TmQhZZmHbUqf()
{
    bool VIjwj = true;
    double lbDztAinTYdiHKzp = 425930.24648171896;

    if (lbDztAinTYdiHKzp >= 425930.24648171896) {
        for (int YbYVSoDnXnt = 1280390940; YbYVSoDnXnt > 0; YbYVSoDnXnt--) {
            continue;
        }
    }

    for (int baFJFRxIlymjKQ = 1842508586; baFJFRxIlymjKQ > 0; baFJFRxIlymjKQ--) {
        VIjwj = ! VIjwj;
        VIjwj = ! VIjwj;
        lbDztAinTYdiHKzp = lbDztAinTYdiHKzp;
    }

    if (VIjwj == true) {
        for (int lABXiEX = 1456421315; lABXiEX > 0; lABXiEX--) {
            lbDztAinTYdiHKzp /= lbDztAinTYdiHKzp;
            VIjwj = VIjwj;
            lbDztAinTYdiHKzp *= lbDztAinTYdiHKzp;
            lbDztAinTYdiHKzp -= lbDztAinTYdiHKzp;
            lbDztAinTYdiHKzp = lbDztAinTYdiHKzp;
        }
    }

    if (VIjwj == true) {
        for (int sTvkSbEKyw = 1047594557; sTvkSbEKyw > 0; sTvkSbEKyw--) {
            VIjwj = ! VIjwj;
            lbDztAinTYdiHKzp /= lbDztAinTYdiHKzp;
            lbDztAinTYdiHKzp *= lbDztAinTYdiHKzp;
        }
    }

    if (lbDztAinTYdiHKzp != 425930.24648171896) {
        for (int WsmsUU = 1962755565; WsmsUU > 0; WsmsUU--) {
            VIjwj = ! VIjwj;
            lbDztAinTYdiHKzp += lbDztAinTYdiHKzp;
            VIjwj = VIjwj;
        }
    }

    if (lbDztAinTYdiHKzp > 425930.24648171896) {
        for (int UExlQcXML = 1030123158; UExlQcXML > 0; UExlQcXML--) {
            VIjwj = VIjwj;
            lbDztAinTYdiHKzp = lbDztAinTYdiHKzp;
            VIjwj = VIjwj;
            VIjwj = VIjwj;
            lbDztAinTYdiHKzp *= lbDztAinTYdiHKzp;
        }
    }

    for (int adDEKf = 705781261; adDEKf > 0; adDEKf--) {
        VIjwj = VIjwj;
        VIjwj = ! VIjwj;
    }

    return lbDztAinTYdiHKzp;
}

bool AEULTVNVtdhw::PIwaJJbURCDYt(string OrPOpzEzwJeBD, bool JxcoHOZa, int tKICfbj)
{
    double jfslPD = 700455.568833553;
    double FXLJoZwuSkPf = 157169.47827324184;
    bool SdRFMXJUAYUIijzn = false;
    bool PzSWafaAyqqgjh = false;
    bool KSnfDYOU = true;
    double LRqehiFbPd = -1036785.406730934;

    for (int AWwHwoAZwT = 1715199104; AWwHwoAZwT > 0; AWwHwoAZwT--) {
        PzSWafaAyqqgjh = SdRFMXJUAYUIijzn;
    }

    if (jfslPD != -1036785.406730934) {
        for (int PjrXlp = 728764565; PjrXlp > 0; PjrXlp--) {
            continue;
        }
    }

    for (int loxfgGZBW = 412292748; loxfgGZBW > 0; loxfgGZBW--) {
        SdRFMXJUAYUIijzn = ! JxcoHOZa;
        KSnfDYOU = SdRFMXJUAYUIijzn;
    }

    if (SdRFMXJUAYUIijzn == false) {
        for (int OKqpfcFlICb = 1070268897; OKqpfcFlICb > 0; OKqpfcFlICb--) {
            jfslPD = FXLJoZwuSkPf;
            tKICfbj *= tKICfbj;
            KSnfDYOU = ! SdRFMXJUAYUIijzn;
            FXLJoZwuSkPf += jfslPD;
        }
    }

    for (int cBHCa = 900978859; cBHCa > 0; cBHCa--) {
        KSnfDYOU = ! JxcoHOZa;
    }

    for (int VNmlpmNbcNdD = 467095244; VNmlpmNbcNdD > 0; VNmlpmNbcNdD--) {
        FXLJoZwuSkPf -= FXLJoZwuSkPf;
        tKICfbj -= tKICfbj;
        KSnfDYOU = ! KSnfDYOU;
    }

    return KSnfDYOU;
}

bool AEULTVNVtdhw::zKUizbuOxqre()
{
    bool RPHGnodnne = true;
    bool NXubiILzAts = false;
    int pHjLRpHx = -1728841502;
    bool klsZjDUA = false;
    double RCxeZbAPRLRD = 849980.9104493503;
    bool soRhDNAhvDjFcj = true;
    double xAuDwZeICKxS = -186488.22687628813;
    string UJLYKTjXSN = string("HJJVRjkeACEWoulSXDuUdbXMQpOQqWWAZDeTFmgwcQyqrpDldWbmWxvhzMFECFFwDgUhjuaVoqOdOsrxTkIUkqkLyVkffngMHLGSWyTCXSQIjMQYBFXFYskbbcpoSFjtBiRCjHrLyeXscFuVgyOlM");

    for (int tgUZOUhBPrLq = 359336597; tgUZOUhBPrLq > 0; tgUZOUhBPrLq--) {
        continue;
    }

    for (int gLRLXyVDT = 1503665254; gLRLXyVDT > 0; gLRLXyVDT--) {
        RPHGnodnne = ! NXubiILzAts;
        soRhDNAhvDjFcj = RPHGnodnne;
        soRhDNAhvDjFcj = ! RPHGnodnne;
        UJLYKTjXSN = UJLYKTjXSN;
        RCxeZbAPRLRD /= RCxeZbAPRLRD;
        klsZjDUA = NXubiILzAts;
    }

    return soRhDNAhvDjFcj;
}

AEULTVNVtdhw::AEULTVNVtdhw()
{
    this->LxjSHllzwphutU();
    this->fxgJc(true, 280126916);
    this->YPMngzLXhJTtiaT(-1020017.1646064981);
    this->aKxsWqS(false, -699322.9882913701, -220347.9192026058);
    this->boxwGe(813468.1040426231, string("qDPWjiXZpSyiqSVFAmFShecZBgaQRPvqiRMBfHVCdUaIJLTwweGmgOlYYJKuARvMIYrJxZJWljjCZhIaCgivmwiNUXHnEUszULzqyQHvFegiVYlcKxBdEOXqereIrCqWDbiIdeslvBZZnEReCNtJLhkeGaSqmFwQErYcpFRZCmapiGJBPKDXJNnZbJkJkuazKrtPdfkcKbNZeWygobxuJbyJYHgnrXHtpPDJxVPUlntyaGqVs"), false, string("znqrlHVcxeSjSTIjoiuLeEQcCGIqdvPlDpUDHWiAiyFvghFYoIlCooEEFfdKKlrFAswyICwrObmgBqDbAUwEFfoMfdxNNoVoNdQuKwTpdESqadPhAcZpPkFiqPGLQbOohzowxAPODtsrJYhTpZPuNpTMUhROxYHtPYRGKauBDTtdMVbsEhuKThafSAyYPNlKrgZUoTiuZQrMSZFNpMCQdvZrSXIVGPsaCghehXZPfGkSbFLse"), string("JvYeiMXJXZUlZCYjiZEBiGZFXfqII"));
    this->xMkBNbdCFVKBMZCD();
    this->QtZDjBHG(-34491.244277943675);
    this->mswWZZSmRP(164830.16489811585, 598400.0064760436, -1531473281, 233192.57406778375);
    this->JbWEMe(-364413.3646593447, 835146.3960289676, -2064501130);
    this->WxgoDlKTunx(true, false, string("sylFZQOBPUJfeLJHYFiRYYqqfrzjzUuDZWRruCarekCMnwPyPumzCnqTsntKqSbVfdPouXCargnoiReShm"), -522633.14078567055, string("fllVLgGkBVHFucxXGzXgSxwZbwamkwCmzZJgRzRSuqTPfyFPyKJpZQFlLquka"));
    this->swTOHKNfMOZesnpa(string("bbXiMLfRqSSylywPySuWrHqVjOQmkSNmhbUkqMozvGpgBzKauUcydCNwIhRkRNHGMUmQVAuZOJjrLGjigmUumaydpEFeQeLJRNcMXLyaTqlRoaPcrWufcWDGXrmfFSyFqcRjhkSvEFJYFXihsQvOzQjSrAYKjCKIpNebqihRzNOGeSiUlFutuSOUpTqeYpDTE"), true, false);
    this->TmQhZZmHbUqf();
    this->PIwaJJbURCDYt(string("MmUOnvKBsQkgGFyESuKyIFQrOwcPUlmaiVlHrQsgsCAUVDogSHmzaOZdmmBPpvdfNyGHZgcyxshTbkkFzxRLWKFpGbGbRpmNnfrnRgmJErqkxEESqWBfwclpFXNWyMImmOfFBsvyQgiuNMByMjZSAuuwLPeiqPHUdSXCCWlHOHeNkXWIh"), true, -2002801013);
    this->zKUizbuOxqre();
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class TVHOQWw
{
public:
    int InKscAAcLqa;
    bool XRAGbB;
    double KdBtREQGGlyRQdNg;
    int APJAU;
    string mGihZqkOy;
    double IzquRYUnQsvbGHDM;

    TVHOQWw();
    bool vfRXmMJD();
    int DNDTmpiutLwfgk(double oCNPpPyACCA);
protected:
    string tmNOfDdInWults;
    string nPtgFSAxhxbxHK;
    double wPOpNNVEzCi;
    double iFXkIWhqpXNSh;

private:
    string aXmIKK;
    double RiRslsStjxPR;
    int xAiqANiNeUAD;

    int qbLKkXcj(double Dzlqk);
    int XMScjccbcB(int YHMuOMeIG);
    double nSscVfCsGoRldmWk(double EhbMo, double MylWKXggZndEQ);
    string yJYaGl(string FXKdTzbvpsLrK, string akFwERVAXZgZh);
};

bool TVHOQWw::vfRXmMJD()
{
    string vFnrr = string("kiztyNFEpBXZllUDshofOSSZLnJBObzHEsgThiaiEezxXFUrKKfBsXxWmYAjOdiTRAFckmzdTRvUKYlxNZAMUPkfZknyuIssMBDfbnZIqYWGijepHFPPcaKyURjiHGcNskmoFbDRHbfcrJVAJDYnrGReDwIrrDRwMmpauSgIHYTPwglrWeWaJFelfTIAjTWAnXTMFNsHBouvuvBpaXwPOq");
    bool iWPLSnrQvtNweWl = true;
    int YvqxNBSwzqfOHF = 957667636;

    return iWPLSnrQvtNweWl;
}

int TVHOQWw::DNDTmpiutLwfgk(double oCNPpPyACCA)
{
    int rTFGEbwOEIqZ = 218348948;
    bool bQdUxkwvExAzYBa = false;
    int wetwTjBXQ = -1408170183;
    double DIulNSnf = 922131.6964389349;
    double CINym = -85052.6245796945;

    if (CINym > -85161.39444722395) {
        for (int sBKuNvODAF = 551318351; sBKuNvODAF > 0; sBKuNvODAF--) {
            DIulNSnf -= oCNPpPyACCA;
            CINym -= CINym;
            wetwTjBXQ /= rTFGEbwOEIqZ;
        }
    }

    return wetwTjBXQ;
}

int TVHOQWw::qbLKkXcj(double Dzlqk)
{
    bool PCayjaXq = false;
    bool prdrAckEMoOJusp = true;
    double TkdMEexZMr = -64588.988075997535;
    bool QFtDNsqhTiegGi = false;

    if (QFtDNsqhTiegGi == false) {
        for (int LUsbVONwDrkGXAa = 2090622544; LUsbVONwDrkGXAa > 0; LUsbVONwDrkGXAa--) {
            prdrAckEMoOJusp = ! prdrAckEMoOJusp;
            PCayjaXq = ! QFtDNsqhTiegGi;
            TkdMEexZMr -= TkdMEexZMr;
            TkdMEexZMr *= Dzlqk;
        }
    }

    if (QFtDNsqhTiegGi == false) {
        for (int YXbAZCvNU = 1514469180; YXbAZCvNU > 0; YXbAZCvNU--) {
            continue;
        }
    }

    return -1151927479;
}

int TVHOQWw::XMScjccbcB(int YHMuOMeIG)
{
    bool setqoStYtNTacW = true;
    double UAFykjz = 134585.14113814983;
    int sKdmDIUx = -182309980;
    int qxJevmgo = -734244135;
    double FPWCeR = -691975.1614920055;
    bool VYxyuMRTDmcy = false;
    int pFJBKt = -726397146;
    double WyRZphBOZG = -290671.99290621484;
    double uXDAneAbOHCHLtC = -816864.4501963882;

    for (int wcJDE = 2020638907; wcJDE > 0; wcJDE--) {
        UAFykjz -= uXDAneAbOHCHLtC;
        qxJevmgo += YHMuOMeIG;
        uXDAneAbOHCHLtC += UAFykjz;
    }

    for (int EcwKrZHMnONNe = 1981456477; EcwKrZHMnONNe > 0; EcwKrZHMnONNe--) {
        YHMuOMeIG *= pFJBKt;
    }

    for (int jJtcYhXNasJHPSf = 698890000; jJtcYhXNasJHPSf > 0; jJtcYhXNasJHPSf--) {
        qxJevmgo /= YHMuOMeIG;
        pFJBKt /= pFJBKt;
        sKdmDIUx += pFJBKt;
    }

    return pFJBKt;
}

double TVHOQWw::nSscVfCsGoRldmWk(double EhbMo, double MylWKXggZndEQ)
{
    string XIORl = string("LHqPUFHjwyfWgFXyQehcVWUFhXLCtaawUwYnMTxhyYsyfn");
    int AuZjLgveHvYvoS = 1757251604;
    bool XVDPPwNBqTmgyKH = true;
    bool MtMtVXlug = true;
    bool StlJcydFWRRb = true;
    string WzvkMilO = string("IQaQMbFDkvrmorTvrVTGGmCVVidKkBmmxlYnKKcKXqSEKDdsQRMCwScxplffVioYBmNTqvRjJjVUGAKeeIuTYYunTAXBbEfXWIDuUQeuOFkayGiWrXPDknCrcDJWLUZykrmobXudGciJZEyxqKKcjkOPUFCsyuHiPKcmoi");
    double TBeZwEDykPm = -156643.38846503585;
    string XcvQTXESjs = string("QYhgsQjkhljBEJDHnDhwIwUbsqGZjdbuXEhPzcTDtiJdMFFiEkmEoTeTSVjnPIhinDuJmYIVjVPmRY");
    bool OCtuiIWdGvaka = true;

    for (int qBDlOqDlIdEoVGFD = 1845958291; qBDlOqDlIdEoVGFD > 0; qBDlOqDlIdEoVGFD--) {
        MtMtVXlug = ! OCtuiIWdGvaka;
    }

    return TBeZwEDykPm;
}

string TVHOQWw::yJYaGl(string FXKdTzbvpsLrK, string akFwERVAXZgZh)
{
    int OHwWxOBMvMlTXrZO = 1953922915;
    double OWsSGthJMQ = 60407.13095138926;
    bool FwwoAfcRW = false;
    string JgGjeL = string("vQQZFHAqZPmrVdDPDsrixVuoqUmugVQwNXsPYubwdXypnEhNjsSStovGEVwpkibiEjulN");
    string RKWBiChtlVnDnGq = string("aqiSaxRmmcPxBUZbiGTluXMtfdnZSkQaAWLEcoCDPmUylohcvYRfaYNJYNVWlOikysRokgBUlKvePyagHauIyYuDjbJIpKPZZQTFzfKqujwfwbaQGBzoXwbBhYpkNldBMXBJiKIoyPcXSAwvcTegqnKgqtcovBJnfbVoEbNQjFWSpShvdzQMPAfvQxVuw");
    double krPBfOKYWRRh = 345631.1269968436;

    for (int JeehD = 833754654; JeehD > 0; JeehD--) {
        JgGjeL = akFwERVAXZgZh;
    }

    if (FXKdTzbvpsLrK > string("aqiSaxRmmcPxBUZbiGTluXMtfdnZSkQaAWLEcoCDPmUylohcvYRfaYNJYNVWlOikysRokgBUlKvePyagHauIyYuDjbJIpKPZZQTFzfKqujwfwbaQGBzoXwbBhYpkNldBMXBJiKIoyPcXSAwvcTegqnKgqtcovBJnfbVoEbNQjFWSpShvdzQMPAfvQxVuw")) {
        for (int ACSerDmZhyeHS = 132836301; ACSerDmZhyeHS > 0; ACSerDmZhyeHS--) {
            continue;
        }
    }

    if (akFwERVAXZgZh == string("KwfSsJAkNAwWBmmQExkMBcNmJZDbDGeEWJUaLIZsiIwZhPNGRgLiuLZrKsnVRvcWxbhhCnoLiULOdPK")) {
        for (int cXGbW = 1199494648; cXGbW > 0; cXGbW--) {
            continue;
        }
    }

    for (int UpfJNlCgoMsK = 1457489106; UpfJNlCgoMsK > 0; UpfJNlCgoMsK--) {
        JgGjeL += akFwERVAXZgZh;
    }

    for (int kOtmNGhqOoyVBPU = 344880709; kOtmNGhqOoyVBPU > 0; kOtmNGhqOoyVBPU--) {
        continue;
    }

    for (int lFrWXN = 530385972; lFrWXN > 0; lFrWXN--) {
        continue;
    }

    return RKWBiChtlVnDnGq;
}

TVHOQWw::TVHOQWw()
{
    this->vfRXmMJD();
    this->DNDTmpiutLwfgk(-85161.39444722395);
    this->qbLKkXcj(-973028.5060529225);
    this->XMScjccbcB(1254091206);
    this->nSscVfCsGoRldmWk(847663.9080495007, 282518.97661786486);
    this->yJYaGl(string("KRCCDpJjNJaZiAHAMxwdWgAvEfCYmBxYceScLpzhyuCdOfUDwJuAuzofGevIxfIvCKZJOQhIJXzHIzyHPGIelPDwSPsnQmUtpYGsfZOmZzetZNqmaATLuotvajtRqYYwpczfARFkXggbcrWaFQuxjsgCjiKSDXqgolOeluSaV"), string("KwfSsJAkNAwWBmmQExkMBcNmJZDbDGeEWJUaLIZsiIwZhPNGRgLiuLZrKsnVRvcWxbhhCnoLiULOdPK"));
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HjWbOxd
{
public:
    int XoxZZvTAjVpS;
    double wpEixHlBrZ;
    double rHPROvd;
    double fiCVj;
    int mFmHmbpvh;
    int KOicXUBFa;

    HjWbOxd();
    string CskmIexj(int aIkFVQG, string nwhUSSBAlDrKUOPu, bool gRFavLa, int kOXRJgjfyoCEr);
    double CMkQGAlzBQVpI(double YNgyLLGJhqY, int vIpwrAVAaLNK, string FwIWaX, double pxiwUa);
    double WdpqZgWlAd(bool PjXhtFFJxlDkGXTj, double BmbgOHzYCWhP, double MJhwGiyq, int CbOoaLlR, double BbWHZPuSiB);
    string jobRxbx();
    void XsVucYLB(double KXnVzvwBXfAHOfH, double RYVxBELXKsbUfxP, double byyFcBAer, double tgVWZBeJcN, int mlnVBYbZYZwEOst);
protected:
    string QxxSDJHLPUP;
    string vHsmKdAlsgGnZPK;
    string uiTzovGlLwpeUJf;
    string FKgJJLyvCGfu;
    string JXELoWjcpDObcGW;
    int zOlaeGpX;

    double qrrWDpOY();
    int gFWhvJHNcXN(bool ihaSmEMufmzSAW, bool IEsnr, bool IbMEXvMmAiq);
private:
    string RcZDRdmCHQaUVs;
    string IDWEK;
    double AMNlHXso;
    int zMeLhbSCdlQA;
    bool xbgusJMZBT;

};

string HjWbOxd::CskmIexj(int aIkFVQG, string nwhUSSBAlDrKUOPu, bool gRFavLa, int kOXRJgjfyoCEr)
{
    int GZIbZuMbXnDprW = 2039368630;
    double WCeyPMUtUjMwO = -908910.1095777297;
    string WiXXSHBrpqYA = string("KWfAJWufdeUpTZHXsHOlHBFieHNAotuDvCHePcvvfCduKQmWoxVHfmBLnksNUnxMYbjtamtuSUcywOxVrAXPyeagiyZlRSIsougxixUnNljLVzxsIzHSNxrggefvYJbQrKemIr");
    double EQTsssPMp = 680113.8422310301;
    string gZMwI = string("CoxXmOwEFnniuaOyVGxuOkTCKXwqqvQxaeRLPfvlbMpZlwMgOEDfGdztfMCpHLxefbCnKEyZsZdJMNLQfnUhudwtQQiRNZfduVsMlQGkJZqBeicamkvAFAUuznxFMsDFRHDTcJOXnnxbTWddAsQGxdFTLIGBwUqotyoUcgkoJSlmRuMdRsQXdEsVeVUwqVmYYaSXdSUGYFfq");
    double ZMnCmfWfLzpCbIuB = -474690.57924724295;
    double cAHCGxhgb = 227152.64261515983;
    bool HcWgc = false;
    string hbfYQq = string("xnDSNMSZqealLBBxCUKunxYmRrgrGkIxLxdUSEdoUgArGSKMmptBMgOGCkwPHEzdciBzbKfGlAAFeYSpiptFGIOoDoEkPETuOXtUeFJwdSSQLiMPRajwwDFjDSrBghNUgKaNLhXXYakKkZLkXFntnxCXsUxIkqWMsqGojqrVsyoEYKPmMgtnDIsFlGTiZWGDBAfpOVmoaSCjMtfCnSMwNuCCAcFLHvaTCwkLcTfrmgAllpjAcZQ");

    for (int jFGwVkbvxgeQ = 133147423; jFGwVkbvxgeQ > 0; jFGwVkbvxgeQ--) {
        continue;
    }

    return hbfYQq;
}

double HjWbOxd::CMkQGAlzBQVpI(double YNgyLLGJhqY, int vIpwrAVAaLNK, string FwIWaX, double pxiwUa)
{
    bool FAQqcuSJ = true;

    for (int jsQmozyJl = 402190106; jsQmozyJl > 0; jsQmozyJl--) {
        continue;
    }

    for (int XDbYGJEmbE = 161005934; XDbYGJEmbE > 0; XDbYGJEmbE--) {
        continue;
    }

    return pxiwUa;
}

double HjWbOxd::WdpqZgWlAd(bool PjXhtFFJxlDkGXTj, double BmbgOHzYCWhP, double MJhwGiyq, int CbOoaLlR, double BbWHZPuSiB)
{
    bool UfdjLUQMp = false;
    double uwAaa = 115550.13110864596;
    double kRKDVmrkjXJGaztH = 702061.78946066;
    int kTBLHQjZs = -283884680;
    string clUbDRRXpIWQlBgr = string("gwvACYIpjOMAnbbDHNTfUrOsjVmPwtWMjrvKAUHJGZCozUeOACNaUkdSJrPFAIzAhocLkyUhilPXLrLSdpEoCbfuPzCuVGMnyaViJPgPuSeJOSxIYxxJIelUKBjh");
    int hilWXRUlwfiPz = 1381616028;
    string CpCVxCLC = string("nsuDtfWbHkFEAoknRUZChVJONUIKvQeIDtiUqYGvxkgdtWeSNLDbcZehrxQPIpXqbbcidSFXXUhTtdvqEsoupnJqYxnFjnIcgfLiTmPmuqPQziolRKyzfavMhzfcVTiFwxSEakSSn");
    double aYMgvcRAlfAK = -793962.7996126568;

    return aYMgvcRAlfAK;
}

string HjWbOxd::jobRxbx()
{
    string PDUlKcN = string("KpLOHyuAQYBtfMRZrAFiFOrIEENcbluAsOibZLfrxYm");
    bool qkEKasZsBej = false;
    double VJSvgKCaKHSYQe = -219514.49031221095;
    bool HJkBSMgMmupulT = false;
    int CQwMJWEt = -638423201;
    int tLdNpxWOkuzD = 225691575;
    double HwWNndG = 1000385.8085597939;
    double VYvRqabHjvIZOqHl = 442088.033599587;

    for (int KNzEFbxspf = 342190876; KNzEFbxspf > 0; KNzEFbxspf--) {
        VJSvgKCaKHSYQe -= VJSvgKCaKHSYQe;
        HwWNndG /= VJSvgKCaKHSYQe;
        qkEKasZsBej = ! qkEKasZsBej;
        VYvRqabHjvIZOqHl -= HwWNndG;
        HwWNndG /= VJSvgKCaKHSYQe;
    }

    return PDUlKcN;
}

void HjWbOxd::XsVucYLB(double KXnVzvwBXfAHOfH, double RYVxBELXKsbUfxP, double byyFcBAer, double tgVWZBeJcN, int mlnVBYbZYZwEOst)
{
    double RNHxAzTTreY = 947308.71493238;
    int ziZqLwht = -1379635533;

    if (KXnVzvwBXfAHOfH != -384113.4270404359) {
        for (int YvzgWkqOsYbFZ = 845199225; YvzgWkqOsYbFZ > 0; YvzgWkqOsYbFZ--) {
            tgVWZBeJcN = tgVWZBeJcN;
            byyFcBAer -= RNHxAzTTreY;
            RNHxAzTTreY = RNHxAzTTreY;
            KXnVzvwBXfAHOfH += byyFcBAer;
        }
    }

    for (int xddWaFsstg = 764609622; xddWaFsstg > 0; xddWaFsstg--) {
        continue;
    }

    if (tgVWZBeJcN > -384113.4270404359) {
        for (int BUfOfaTBTQatdS = 1833965206; BUfOfaTBTQatdS > 0; BUfOfaTBTQatdS--) {
            byyFcBAer -= tgVWZBeJcN;
            RNHxAzTTreY /= KXnVzvwBXfAHOfH;
            RNHxAzTTreY *= byyFcBAer;
            tgVWZBeJcN /= byyFcBAer;
            mlnVBYbZYZwEOst += ziZqLwht;
        }
    }

    if (RYVxBELXKsbUfxP >= 947308.71493238) {
        for (int wyXRt = 2144761572; wyXRt > 0; wyXRt--) {
            KXnVzvwBXfAHOfH /= byyFcBAer;
            tgVWZBeJcN -= RNHxAzTTreY;
            tgVWZBeJcN += RYVxBELXKsbUfxP;
        }
    }

    if (RYVxBELXKsbUfxP == 947308.71493238) {
        for (int qqbwyenhgxzBUz = 2137332831; qqbwyenhgxzBUz > 0; qqbwyenhgxzBUz--) {
            tgVWZBeJcN *= tgVWZBeJcN;
            RNHxAzTTreY += RNHxAzTTreY;
            byyFcBAer /= RNHxAzTTreY;
        }
    }

    for (int fkjFRaLRor = 2028163469; fkjFRaLRor > 0; fkjFRaLRor--) {
        continue;
    }
}

double HjWbOxd::qrrWDpOY()
{
    string IZAZCaz = string("fzAklUpnBaJhmEESOhoDAfdjfkNWzssjtsnshMoxOjKAVUJlSWelJLPRWBHgLUYONOegUzjbAgPvgHiQtlSiyWAPXSWMYtGYewzhwUBllDgQYXHJgWjymwuyJSZriqYmJKaVZvXfgHjvdDDXOeCNKDRDZDgWFnSrcyLWszAhtndSVNMMAYjcKhWUwcXWGDPCRFSDhsHHvXkQVaWfuWai");
    int Zprpynm = 1394250378;
    bool CgBAkOzVlvJdA = false;
    string tyatPh = string("SzYcVYoeygggDVfeWzezlVfAqFqLKJftLyeStveBDsPcOZQVUhJlDvJCIxANtyotcYSsvjopOnhLLtXExXrKTMvVSWOiHDxjLCvyTjfTYbRIPaTKTN");
    string frJCTIwiSS = string("fhCivgNvBzDxnIxconNVYWiRzaonCIDuuBNmsmDxzXxxyHDVtIPAIRmcKpsCAykfuTmEWKuhmGCVfpMChxeEBlzFcJKZSHXlZqKGjfpqXDwjeybJjkPaSZnaoGjOdJNOKflSiveVuHXAqDihOJdARPagZBIqwzpnlXXRKwPGfYSCRYINiJqqfLoqKuYrXMtqjsWEabXtZuboWJWqehatLACcOMwtjZhOnaYvXnPDOG");
    bool zPvmUulzgFQ = true;
    int otIAucmnY = -933332937;
    bool DBCTHXddBMuQJX = true;
    bool JUcmXQTpgZcu = false;
    int pmXlUSLda = 1343954452;

    for (int pfzhOJU = 2040408116; pfzhOJU > 0; pfzhOJU--) {
        IZAZCaz += tyatPh;
        frJCTIwiSS += tyatPh;
    }

    if (JUcmXQTpgZcu != true) {
        for (int UfqXsxQ = 750056732; UfqXsxQ > 0; UfqXsxQ--) {
            tyatPh += tyatPh;
            tyatPh = IZAZCaz;
            JUcmXQTpgZcu = JUcmXQTpgZcu;
        }
    }

    for (int kcEQkjWeWua = 2041895264; kcEQkjWeWua > 0; kcEQkjWeWua--) {
        Zprpynm -= pmXlUSLda;
        DBCTHXddBMuQJX = ! JUcmXQTpgZcu;
    }

    for (int BTxgwIsSWCk = 1463450112; BTxgwIsSWCk > 0; BTxgwIsSWCk--) {
        continue;
    }

    return -449277.8490616803;
}

int HjWbOxd::gFWhvJHNcXN(bool ihaSmEMufmzSAW, bool IEsnr, bool IbMEXvMmAiq)
{
    double uNWwwRqQBWzh = 151737.78228819906;
    bool dIBiIQwx = false;
    int zCsaPCakDfexCd = -1453516719;
    bool AKHIM = true;
    bool IBezoAvfjQhr = true;
    string fwjPeUbGPif = string("ejiYCGaZNeEJorQMeTJNvmKuuvgrsddcPJDbldbHOiOyvAmYyyKeNrzrhKUFVVpRagPxGAxuMUPBotvtXGyHNcVFEDQscCRTeSuPKlPOkjqboWpevOVrfongvayVqkHUaHuUsCvGnfuYXncWbhuATYSMHtnnChIMiVAqwAgkgz");
    int mpEJigE = 983448427;
    bool YmqlGFWV = true;
    string ATpiNUuzcnN = string("XHHJwkztxmaiKnzxTDHMwEtEZTAQTQHEBkZAzogFPTLINaCOSnrLbdaSlxqlewSQygBFoJrIXvBxCQjotEOvDekfnIQucIcuDmDLvvBwgBMCzkjXcreTjklFHyyBReVChAcpkaRsaKdKGaFOhypUFrjHTCuDGZYzfHKnomvjHPvJvudHbFrZMQQxDLjspKBsNMMAVRNrUgqqAXacQdR");

    for (int KrJxlJyqFzkVvQB = 66990125; KrJxlJyqFzkVvQB > 0; KrJxlJyqFzkVvQB--) {
        IEsnr = IEsnr;
        IEsnr = dIBiIQwx;
    }

    for (int CJsugOpBKpRaDBPe = 211873647; CJsugOpBKpRaDBPe > 0; CJsugOpBKpRaDBPe--) {
        continue;
    }

    for (int ixVBS = 1842598548; ixVBS > 0; ixVBS--) {
        IEsnr = IEsnr;
        IBezoAvfjQhr = AKHIM;
        IBezoAvfjQhr = ! AKHIM;
        AKHIM = YmqlGFWV;
    }

    return mpEJigE;
}

HjWbOxd::HjWbOxd()
{
    this->CskmIexj(-191531875, string("RhbGEcVuGOZhRPCjxahUWIksPljxbNZFMGGcTypQOeahTylejMDAEILFXRrLpMwvTBAcKRhHnmZeXW"), true, 1179390040);
    this->CMkQGAlzBQVpI(791829.4574541696, -1428340777, string("vZrwnHCkRtxsSmLfyNsEQToCDyXEPRrzRbvSqvFggWcmVOLuxZjzlYxXvtBOvUXRGzHMfFqoJjafCXzrlwcEdxxwdlefoIjbDYRwInHYtWkslmPfIKmwoCPaoCdqUOPpaTUzSLUSMYqmtlnhoYlDmntgWwCYTovsHCEkiYUIPUBURHDkAYuTWJzcPOpLYhDxvzKFnKJnacUNsVeVwlNqKPbKjThFeCJXbCv"), -175904.95552644352);
    this->WdpqZgWlAd(false, -9046.676929646525, 516996.2641324463, 857207664, -908648.0704421807);
    this->jobRxbx();
    this->XsVucYLB(-881107.8647384052, 935525.5737778472, 570559.1231612767, -384113.4270404359, 1360541759);
    this->qrrWDpOY();
    this->gFWhvJHNcXN(false, false, true);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class TZzPAYBlEreOZ
{
public:
    string eaIsBTY;
    double aPMzbZTM;
    bool IknDykcBqxqsBzxu;
    bool MnDjKLreipckYCL;

    TZzPAYBlEreOZ();
    void WiaKx(string leSKVEQMAsfz, string QcsBTpYgwADcv, string INAjjLvb);
    double hgMLVYR(double KAulDrp, bool WBByopKBrGgWgKe, double SDUUwbwt, double zpPwVPTAXLqwgX, int tqoIheGpOQ);
    double cZpdIoG(string EKcGTKzztauAkXXk, int dkNNguqpxB, int tstrKAlMWYa);
protected:
    double cuctonL;
    bool udoTlqQNUrIUUBP;
    double OWboBDRxcu;
    bool alKQNHM;
    double lToNYAItY;

private:
    double UnCpAEVvARajJ;
    double AmkxzDZHsQUTO;
    string PkszErdUtEywV;
    bool HedPJtCkEYuI;
    string HvcAUvQJeyO;

    string KcjzidmeOO(string OTLWhOHuVO, int Ohjumu, string NKqvQIYDVcTEnv);
    bool kUBDDHSRHzcSAaTR();
};

void TZzPAYBlEreOZ::WiaKx(string leSKVEQMAsfz, string QcsBTpYgwADcv, string INAjjLvb)
{
    double YHlpSedU = -444854.2684736813;
    int TLXodhsYtcUqPW = 977277898;
    double XgbiBw = 831913.2218625944;
    int GRBium = -1404558789;
    double sMnjjWCmtJGD = -957242.8881226314;
    double qCkhM = 373032.4728629918;
    string BWtudqgqghUZPoNQ = string("acBUKGfEMuMdjaWlwXzLYdtZyWUiZNqfkdMcmLNJRYlIKccnlIUDNezhOfVuLOZHffOcPSMoXulabObAVSSkuVttKoMQqxCnjOARTCDcRKuhDVUkGfjeamjmXjZtnDKYhTTYmNwmlGrhHXCbWWpTsaOpKzKMTQQgFUKRfkcNTTL");

    if (BWtudqgqghUZPoNQ != string("NteKMUUFmcqIqdoZdzOYDJHVBPfbliLVbcOUlwLEqQFHMAWeYvTSJraEQIgwUWHaXxJNXgYHmFAoBVIVKIcVCUFjEbNDpnQagAuOJcfToifDAsCfzEheWVhoHrQNNjItLumVccCcBjtEufqraQLbllTmerEephsRXACgDRtjNWzSczShQuTVUDPbaEUUgmSrEgOXMhNQxsqntoQarOnBITGQNbCPJEzcvXCsSnGhYDmuDSQlgB")) {
        for (int lNsqvQqTVfz = 374955027; lNsqvQqTVfz > 0; lNsqvQqTVfz--) {
            qCkhM -= qCkhM;
            leSKVEQMAsfz = leSKVEQMAsfz;
            qCkhM -= qCkhM;
            INAjjLvb = leSKVEQMAsfz;
        }
    }

    for (int jpBOKe = 2031131869; jpBOKe > 0; jpBOKe--) {
        sMnjjWCmtJGD -= qCkhM;
        XgbiBw += XgbiBw;
        sMnjjWCmtJGD -= XgbiBw;
        BWtudqgqghUZPoNQ += BWtudqgqghUZPoNQ;
    }

    for (int BUifHsHdeyD = 1695996914; BUifHsHdeyD > 0; BUifHsHdeyD--) {
        XgbiBw += YHlpSedU;
        qCkhM *= XgbiBw;
    }

    for (int EcvoJKLmU = 1775310549; EcvoJKLmU > 0; EcvoJKLmU--) {
        leSKVEQMAsfz = BWtudqgqghUZPoNQ;
    }
}

double TZzPAYBlEreOZ::hgMLVYR(double KAulDrp, bool WBByopKBrGgWgKe, double SDUUwbwt, double zpPwVPTAXLqwgX, int tqoIheGpOQ)
{
    int BBEGHwBk = -240642858;

    if (SDUUwbwt <= -282182.86726741696) {
        for (int ynTKqvpp = 615168335; ynTKqvpp > 0; ynTKqvpp--) {
            SDUUwbwt -= KAulDrp;
            KAulDrp /= zpPwVPTAXLqwgX;
            WBByopKBrGgWgKe = ! WBByopKBrGgWgKe;
            SDUUwbwt = zpPwVPTAXLqwgX;
            zpPwVPTAXLqwgX /= zpPwVPTAXLqwgX;
            zpPwVPTAXLqwgX -= SDUUwbwt;
            WBByopKBrGgWgKe = ! WBByopKBrGgWgKe;
        }
    }

    if (BBEGHwBk >= -240642858) {
        for (int cMalonKBVfXAekSd = 2006621152; cMalonKBVfXAekSd > 0; cMalonKBVfXAekSd--) {
            tqoIheGpOQ = tqoIheGpOQ;
            BBEGHwBk -= tqoIheGpOQ;
        }
    }

    for (int nHrzbb = 510659993; nHrzbb > 0; nHrzbb--) {
        SDUUwbwt -= zpPwVPTAXLqwgX;
    }

    for (int mFoWHogkwDvs = 865547159; mFoWHogkwDvs > 0; mFoWHogkwDvs--) {
        BBEGHwBk -= BBEGHwBk;
        tqoIheGpOQ = tqoIheGpOQ;
    }

    for (int obiRrsAR = 94052820; obiRrsAR > 0; obiRrsAR--) {
        zpPwVPTAXLqwgX = zpPwVPTAXLqwgX;
    }

    return zpPwVPTAXLqwgX;
}

double TZzPAYBlEreOZ::cZpdIoG(string EKcGTKzztauAkXXk, int dkNNguqpxB, int tstrKAlMWYa)
{
    double rwNwJScsYhQWuloZ = 720794.2252619393;

    for (int RRYdcU = 208160231; RRYdcU > 0; RRYdcU--) {
        EKcGTKzztauAkXXk = EKcGTKzztauAkXXk;
    }

    for (int arLbFqag = 1059013637; arLbFqag > 0; arLbFqag--) {
        dkNNguqpxB -= tstrKAlMWYa;
    }

    return rwNwJScsYhQWuloZ;
}

string TZzPAYBlEreOZ::KcjzidmeOO(string OTLWhOHuVO, int Ohjumu, string NKqvQIYDVcTEnv)
{
    string NOUfZYOiNrGsnv = string("TtoAFUAShnmOtimJQhIObhNbbEUlxKlxvJekpkKyWyN");

    if (Ohjumu < -246554629) {
        for (int PEYUjkyHObEfiMAR = 1346040697; PEYUjkyHObEfiMAR > 0; PEYUjkyHObEfiMAR--) {
            OTLWhOHuVO = OTLWhOHuVO;
            NOUfZYOiNrGsnv = NOUfZYOiNrGsnv;
            NKqvQIYDVcTEnv = NOUfZYOiNrGsnv;
        }
    }

    return NOUfZYOiNrGsnv;
}

bool TZzPAYBlEreOZ::kUBDDHSRHzcSAaTR()
{
    double SbNNjmYRcaK = 207495.3519717489;
    double gTmYhckCofirZ = 534561.1750718872;
    int xsKzo = -1643408977;
    double DJLQNwCXZNDQlN = -120429.76300153331;
    string gvMSoqMqC = string("ABEjAeeYCiZevfjsIvvQnSgQmmvyrTcZuGyyGkSvGQEuHqexnoWepXXjMECWwjAKLyKDcrXfpPGDKtxHtbInZbUXsEGKrdhYNIFPGiIsoOIa");

    for (int eAYvHppL = 456933548; eAYvHppL > 0; eAYvHppL--) {
        DJLQNwCXZNDQlN += DJLQNwCXZNDQlN;
        gTmYhckCofirZ = gTmYhckCofirZ;
        SbNNjmYRcaK = SbNNjmYRcaK;
    }

    for (int IelPDqHg = 1342349410; IelPDqHg > 0; IelPDqHg--) {
        DJLQNwCXZNDQlN *= DJLQNwCXZNDQlN;
        SbNNjmYRcaK += gTmYhckCofirZ;
        gTmYhckCofirZ += DJLQNwCXZNDQlN;
        gTmYhckCofirZ /= gTmYhckCofirZ;
    }

    for (int OTVAAte = 54347921; OTVAAte > 0; OTVAAte--) {
        DJLQNwCXZNDQlN = DJLQNwCXZNDQlN;
    }

    for (int RpkteNR = 1293589391; RpkteNR > 0; RpkteNR--) {
        DJLQNwCXZNDQlN *= gTmYhckCofirZ;
        SbNNjmYRcaK -= SbNNjmYRcaK;
        gTmYhckCofirZ /= SbNNjmYRcaK;
    }

    return false;
}

TZzPAYBlEreOZ::TZzPAYBlEreOZ()
{
    this->WiaKx(string("NteKMUUFmcqIqdoZdzOYDJHVBPfbliLVbcOUlwLEqQFHMAWeYvTSJraEQIgwUWHaXxJNXgYHmFAoBVIVKIcVCUFjEbNDpnQagAuOJcfToifDAsCfzEheWVhoHrQNNjItLumVccCcBjtEufqraQLbllTmerEephsRXACgDRtjNWzSczShQuTVUDPbaEUUgmSrEgOXMhNQxsqntoQarOnBITGQNbCPJEzcvXCsSnGhYDmuDSQlgB"), string("QffnKuttGozWcqbmlwSMsKrhVvGOJOadIHxWyGTVfcVaxPWUEzuYsBTsrOVkttmHHzcHbtyezTxkiLUSixwwZhaMpY"), string("szksxwQBSZXiewRggXacgEVORMpfTYZXcqijWharVybYPQQmZjwK"));
    this->hgMLVYR(526783.7962634357, false, 971410.1368429734, -282182.86726741696, 1496402894);
    this->cZpdIoG(string("rZzbDLjGZSbwkqzAjXXuDKQoQFBUTavMtwscEXnYOlNYpDnFbrFOApxHlAhuCnTxyLNhPiNiZyTYuaKeaXXEXgkupfIMqcCZrzpvIQekANSWDbDCEjbfgGLWzhFBeWvyJi"), -1569118637, -1439306590);
    this->KcjzidmeOO(string("mZVYRKtjdDFsKjRHWKIqdfXiXovcwUCZzTud"), -246554629, string("xuYWtNxZBviFXmiWuaFGLvluhLUBZCgvIGPvTtyFkdCJDzhEackwDkXQiUyGMoDSSJIIurPETlKKfjiebchwhQRzkuwgxDIJdUvKegkaCKQAzDFcgmFUtKmKGfbcnTJhwOVIMrpTElRiloHZqQbywrqxnXYaPCkgjJ"));
    this->kUBDDHSRHzcSAaTR();
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jwQCFi
{
public:
    string hKDLwZKFJUSV;
    int zdFlZlaxjEYEf;
    double mhYHqqb;

    jwQCFi();
    string wzfyULFVYQZtREm(string cyGEEiw);
    bool TGOQMgiKLsAgT(bool ImwHFw, int HTrub, double TESdtlFykNV, bool VaTxRwuEwmEWRkn);
    void AwtPyKxXUnv(bool RIqoRNxVOHGk, int BslkGjbeZsqiEE, double BnneniICFM);
    bool LWMLlXzxT(string NhClqTbPhfC, double rAZoiDvUE, string wdOSGQq);
    void uxtvVWLrJtKzypUj(bool MkrRmfMYHN, int xCHadKDBiotm);
protected:
    double iPqJxrydb;
    int hUZIZwnU;
    string ZLeZYDBG;

    void trLXcUJbKGIQQ(double WGvulYxRaa, int AFzdAPiVcD, double HbOqMn, int eslHZyUqRseq, double ljuhTbVbkqC);
    bool gqbHRCZjDFOiCGd(bool CpVcuq, int bNbxwjteWbeRNtT, int ciRDNLbdkgzS);
private:
    double aRQeyf;

    int EItzCroFAZ(double QsiKxBKYfRQWl, bool hzphRvirSz, bool Eiczn);
    string GzklYafVczC(string rKDooO, string ffrDio, bool jmHZPaBh);
    double YdPZsCEg(int oivfAbQbFLEsuuK);
    int WnHtNgjHPSvhQfm();
    string ZaIwemyqGhT(string krleejPXYHmtOc, bool FMKdXFEOp, string eqmXVHawITGBEh, string BtkWbkPAmP);
    void jXlDaNeFUW(bool SEMrBDAkDTVTyXb, int BwyOyUlx, string ABUvMpQdmoTz, int LNYCQWrapBJ);
    string SxyYP(bool UjikEUL, int CBTNAyR, bool dwOfrxrZmg, int CHlWK);
};

string jwQCFi::wzfyULFVYQZtREm(string cyGEEiw)
{
    int yCpqAYWBwgPuKq = 1820410446;
    int HmCQt = 701653458;
    int GcPgHBacTdQ = -475964497;
    bool WdkEylI = false;
    string NyKRB = string("YiXFlJdQCnJgPfDbLPkSOLzBvJuayllfYRTgvXDNAhloqCXgPXmxdNMxZYLhJUpswiybkfGLgmqloQIBAlJuBjOTGRLCxpeWoUCXlGueVjwQTXxdEDhywKXkYhSLIcPytHGGeQ");
    int staOfUc = -1745650706;

    for (int FqYDdmiNKhlWvkk = 1493922224; FqYDdmiNKhlWvkk > 0; FqYDdmiNKhlWvkk--) {
        cyGEEiw = cyGEEiw;
        GcPgHBacTdQ /= GcPgHBacTdQ;
        cyGEEiw += NyKRB;
        staOfUc /= staOfUc;
        GcPgHBacTdQ = yCpqAYWBwgPuKq;
    }

    for (int BBOrEbK = 1951303954; BBOrEbK > 0; BBOrEbK--) {
        GcPgHBacTdQ *= yCpqAYWBwgPuKq;
        staOfUc /= yCpqAYWBwgPuKq;
        yCpqAYWBwgPuKq = yCpqAYWBwgPuKq;
        yCpqAYWBwgPuKq = staOfUc;
    }

    for (int Lolybwx = 351811696; Lolybwx > 0; Lolybwx--) {
        HmCQt *= HmCQt;
    }

    return NyKRB;
}

bool jwQCFi::TGOQMgiKLsAgT(bool ImwHFw, int HTrub, double TESdtlFykNV, bool VaTxRwuEwmEWRkn)
{
    int jkYSnraskAAH = 338149164;
    string btMISIGURUUhiJmq = string("sssAabfgowmmEEFKUwClvfIqcAWCpnzzNazgyHconloVGRkpthilTCNWZUfzYuimxIpmxjTblRNsMrPolUcSbXjgfkJViZflxbwtLAivUOSSDVCQGWrZCfpSxIzQPyFlvLvBJCNUKmdXqKyFimaEiWuyOGEKrHhyMrbNmdiOCAOlQqQG");
    double oQjHjzwt = 478044.60830931144;
    bool snzXaCYnVyEfYYN = false;
    bool qkGmxZhpd = false;
    int vOwnmpUyM = -1381209153;
    int qZRhjWZrTAi = -1998210069;
    int sqMcdkX = 343885582;
    bool iuXKcU = false;

    if (HTrub <= 343885582) {
        for (int MTYFhQvxfEAQtU = 822645743; MTYFhQvxfEAQtU > 0; MTYFhQvxfEAQtU--) {
            continue;
        }
    }

    for (int SGoGA = 1925894455; SGoGA > 0; SGoGA--) {
        sqMcdkX = vOwnmpUyM;
        jkYSnraskAAH *= sqMcdkX;
        vOwnmpUyM /= qZRhjWZrTAi;
        vOwnmpUyM /= jkYSnraskAAH;
    }

    for (int vTSBVmlJmvGk = 1820424078; vTSBVmlJmvGk > 0; vTSBVmlJmvGk--) {
        continue;
    }

    if (qZRhjWZrTAi >= 343885582) {
        for (int jTFss = 1053736367; jTFss > 0; jTFss--) {
            VaTxRwuEwmEWRkn = iuXKcU;
        }
    }

    return iuXKcU;
}

void jwQCFi::AwtPyKxXUnv(bool RIqoRNxVOHGk, int BslkGjbeZsqiEE, double BnneniICFM)
{
    int QYIdJiVisr = 550719676;
    double islWICvvLY = -798836.7408051399;
    bool AnybVWvbdbPtSLYj = true;
    string CSbnRdQuGFmzpap = string("CIdUFIJJAnoknNDHOhdnGBsejHbHVRTvDduXgSSYCKqQDNPmsvDMdbEBcaGHxPENzBceUPhHjKAuVidsjwsUbitEicAitMULEXupsGPixVBVuPTcCqbgkbmyiQUjHqbbWXZxIJSWhyqNMcIhdfIZD");
    int lnoCYjNIdkJ = -707694125;

    for (int DgGpgFUAB = 382179279; DgGpgFUAB > 0; DgGpgFUAB--) {
        continue;
    }

    for (int bPeGaE = 1881568019; bPeGaE > 0; bPeGaE--) {
        continue;
    }
}

bool jwQCFi::LWMLlXzxT(string NhClqTbPhfC, double rAZoiDvUE, string wdOSGQq)
{
    double zeXSLxKZ = -672107.9156012574;
    double xDXnp = -258447.10558694563;
    int GokTaZnojrmz = -1367984761;
    string JOrxhskVI = string("EKnKCVceqjYSB");
    int MAColtyyk = -1489362369;
    bool BSXRKnqVAXKjJqpm = false;
    string NakxgAbul = string("QGGYuXTVThfbgsGJavXgqzvKPkGXvmZQnnNbBwVLjYkEGanoOgfTEkuhUvWiMhVvrsJwfDvuDXKilrCBwdBYpdYRDHEJEPKApBchKncwurHfuWHMkpaHpMcopSilegLGWONCqHLriDpiODfnJlHzOBBRrGcqAUSUxqojVySsWUjrwIGLPZDUqpVGnDkhIafoyQhjaLbjwFUMZHpLkddOIrwZhjeRNphWEEaXUxeXkClQQ");
    string fzfdCzEYbrQJUAC = string("eSXbhdRJiGuZTcthWqmEgjhsBHVHYWUTTLqIbktdYzcnTkQxrZpJdqGzJGNhWsdsWDsugGicblkEsiIWRoVwFpgrn");
    string yvPja = string("laLTvzsypVrQMIzloaFDboBmmEhitzxjVemPrvrbQjLtWrwZvEyalEZavzSaDLYFhvaYBbWXcxhgaLOshUxlRTbtypAjDUpgvuDpRvqdRWhgWKxulOmUoRwgWxZpOxgogAjoQCAuarVjutoYEneSzDagmPmMh");
    bool TzNsAp = true;

    if (NhClqTbPhfC >= string("laLTvzsypVrQMIzloaFDboBmmEhitzxjVemPrvrbQjLtWrwZvEyalEZavzSaDLYFhvaYBbWXcxhgaLOshUxlRTbtypAjDUpgvuDpRvqdRWhgWKxulOmUoRwgWxZpOxgogAjoQCAuarVjutoYEneSzDagmPmMh")) {
        for (int ymBHEzf = 1138273215; ymBHEzf > 0; ymBHEzf--) {
            fzfdCzEYbrQJUAC += JOrxhskVI;
        }
    }

    for (int mMlAhZU = 568168490; mMlAhZU > 0; mMlAhZU--) {
        fzfdCzEYbrQJUAC = fzfdCzEYbrQJUAC;
        GokTaZnojrmz += GokTaZnojrmz;
        rAZoiDvUE += xDXnp;
    }

    for (int YJzptKBoiJzUbL = 1047106025; YJzptKBoiJzUbL > 0; YJzptKBoiJzUbL--) {
        JOrxhskVI += wdOSGQq;
        JOrxhskVI += NakxgAbul;
        NakxgAbul += wdOSGQq;
    }

    if (fzfdCzEYbrQJUAC < string("RgDrEDZVqmttyBMnaSHDCMBiBbdYTkFoXeVtsqmYUsdWRZAPUQKUODRyQjhvoXsMyOOWXbfGdjVukpGFxyDOcdGGmhkFGzXbXoqbAhZRLKXSKnFlUVjKxkblmzOswPbVBJliT")) {
        for (int qnskcbwrYvAbZLw = 1311050839; qnskcbwrYvAbZLw > 0; qnskcbwrYvAbZLw--) {
            JOrxhskVI += JOrxhskVI;
            fzfdCzEYbrQJUAC += wdOSGQq;
        }
    }

    return TzNsAp;
}

void jwQCFi::uxtvVWLrJtKzypUj(bool MkrRmfMYHN, int xCHadKDBiotm)
{
    double BqBfbrt = -646072.7771382491;
    bool wAKKlDdWlTm = true;

    if (wAKKlDdWlTm == true) {
        for (int rHJGh = 1534923600; rHJGh > 0; rHJGh--) {
            MkrRmfMYHN = MkrRmfMYHN;
            MkrRmfMYHN = wAKKlDdWlTm;
        }
    }
}

void jwQCFi::trLXcUJbKGIQQ(double WGvulYxRaa, int AFzdAPiVcD, double HbOqMn, int eslHZyUqRseq, double ljuhTbVbkqC)
{
    double ofFuPFhikiqCiwX = 286569.3061274324;
    string DBLgaeiG = string("VYSXbKmFARSMMjSlxLwSvWYEqvZKDblBWiBSySCzmpFmhWQiziwbAXaVrOsfGyDDaIpTAthHRDXKZIXBpfFoUriyIPeGyLajTPGeqYPMtlaieXRsRzXfvBvwZNhtIkMnzNaGOuQbFZTpywrJiQwQTTsFmePUkFqwXzYkHPlBsAIRopoNXvFbyDuMZvZZHkihdItdSXvMMMFTtwBuC");
    int pkFgcAEFnkU = 700306287;
    int HxigopAbppQ = 273704463;
    bool nxDnDVKLDEOLSnCa = false;
    string zdzWhSoL = string("PIyfscsXTJCzHUhBezPDlCeWXlyPHqHgfOfSkspNlenpPoUfRWvvjzqXIKVUUroSJoYXOTkZIYjGxyyhmxIGhWAgULucJzCAafyLsvOYbnaBMzGRwqnnDgnviliaRwfSprGHLmbQwKgRQeKmRJbqqPrrsnfJpFfMhSOKuqHUDKLKqlNdKQMVQJawWZBaTYEWTbioJMcgwXbuFPYOGbuKUadkhXBGFUvPfiGDwuTDawyvJcgTWRIGvEYNwWcs");
    string KOcASRWrYqClxJM = string("eaNaVHgitFTqtKVVKbayxoLSpvWPNBLJnRBaZICDcPxcEXtcGbjpILLZsaJDECIhjQspncHByhNvTeReWldjHLPaRGSHieYSRFgmWUiBsIHBKzBUFXIuHuUHtiqSlOIQVnltzesRgtrWBKjfKmfZwRnLIMVvLIGcaqOnUNPTxOmuNNrQWlQDMyzvUGloQuNGtwinKnOVdFfOrXLIMNzmMajhPaEeAv");

    for (int LkPXufiWIzedylue = 73792353; LkPXufiWIzedylue > 0; LkPXufiWIzedylue--) {
        DBLgaeiG = zdzWhSoL;
        AFzdAPiVcD -= AFzdAPiVcD;
    }
}

bool jwQCFi::gqbHRCZjDFOiCGd(bool CpVcuq, int bNbxwjteWbeRNtT, int ciRDNLbdkgzS)
{
    bool gsPtHr = false;
    int aRwtIzbDTN = 500957755;
    string LcPnvupZvOuqK = string("vuvqyTpbKDSqSfsYFnqWyDCnrCPdCDaDkOVsHrlXQMmTSdBVRjRgBrikbUBAuckXZmzqfjHwugOiGMkWKqAAkhXQcZpcaeOPrDMuncxmlsAdIrVwjMcgLGYPiUujZpdtNASIemDgDKiSpajCZNHotvWGEGISkutAPNtZpHmXCsittySWVIobancVjWcFPEwdIVmawMPdwkWkwCAoZRcBlkVpT");
    int lqMECxgxpdRVfhO = 614301157;
    string GJcmEyo = string("rotMTfqjEpjiBaiBwKwtfXFDJaWXLxyyhMEfQGimmOezrfuxjQjEiRMNzQMbcWehrqKVfrefmHHjDSBlPRCuScLhdDPgnWaCTrmwiEjYsqILCVlQrLaWzOVXlkcVKQICYYozyRltPUtWxEnRBeCVGFdmuxUhRdxdgQaCbpdMopsTHODwye");
    string jnIjrrjSuYYz = string("GrcYzCPbmJrUrvyoKTGMYlLKLyAwYPpufVEPhINijBZDEXCSgVlIyfqxPeKWCnGLtLzLYDcrrEuYxXWMsPSmzttuiohShUZkfiUTCgZhplRzhNvNh");

    for (int nCOZrvw = 780150991; nCOZrvw > 0; nCOZrvw--) {
        aRwtIzbDTN = bNbxwjteWbeRNtT;
        ciRDNLbdkgzS *= aRwtIzbDTN;
    }

    if (LcPnvupZvOuqK > string("rotMTfqjEpjiBaiBwKwtfXFDJaWXLxyyhMEfQGimmOezrfuxjQjEiRMNzQMbcWehrqKVfrefmHHjDSBlPRCuScLhdDPgnWaCTrmwiEjYsqILCVlQrLaWzOVXlkcVKQICYYozyRltPUtWxEnRBeCVGFdmuxUhRdxdgQaCbpdMopsTHODwye")) {
        for (int KIwLNMQjbq = 1486170314; KIwLNMQjbq > 0; KIwLNMQjbq--) {
            gsPtHr = CpVcuq;
            aRwtIzbDTN -= aRwtIzbDTN;
            ciRDNLbdkgzS += aRwtIzbDTN;
        }
    }

    if (lqMECxgxpdRVfhO < 500957755) {
        for (int IsZxotFsg = 1831130892; IsZxotFsg > 0; IsZxotFsg--) {
            LcPnvupZvOuqK += GJcmEyo;
            ciRDNLbdkgzS -= bNbxwjteWbeRNtT;
            bNbxwjteWbeRNtT /= ciRDNLbdkgzS;
            aRwtIzbDTN *= ciRDNLbdkgzS;
            ciRDNLbdkgzS *= lqMECxgxpdRVfhO;
            LcPnvupZvOuqK += LcPnvupZvOuqK;
        }
    }

    if (jnIjrrjSuYYz >= string("rotMTfqjEpjiBaiBwKwtfXFDJaWXLxyyhMEfQGimmOezrfuxjQjEiRMNzQMbcWehrqKVfrefmHHjDSBlPRCuScLhdDPgnWaCTrmwiEjYsqILCVlQrLaWzOVXlkcVKQICYYozyRltPUtWxEnRBeCVGFdmuxUhRdxdgQaCbpdMopsTHODwye")) {
        for (int MqZXAAFRePIqBcbp = 2068908734; MqZXAAFRePIqBcbp > 0; MqZXAAFRePIqBcbp--) {
            gsPtHr = gsPtHr;
            lqMECxgxpdRVfhO /= ciRDNLbdkgzS;
        }
    }

    for (int KmjzIb = 1312553195; KmjzIb > 0; KmjzIb--) {
        lqMECxgxpdRVfhO -= lqMECxgxpdRVfhO;
    }

    for (int VshqePrn = 823924334; VshqePrn > 0; VshqePrn--) {
        continue;
    }

    return gsPtHr;
}

int jwQCFi::EItzCroFAZ(double QsiKxBKYfRQWl, bool hzphRvirSz, bool Eiczn)
{
    string WSDNdiqQ = string("uCQHzaiwciawWlehRhxaeOCdMkrNjtqHzueuRTnkSZnVcaTDYCzAzFRTidSGHZXuOcmjtHYP");
    string oTMpkOgMCVM = string("qzWaQtdtEkModldfKMGRMkQhDmIfKGwrRXRnqizwzWaCRXXvmLMgnOVZxIRZPKXtxWTagZvJvXWmLlpGJfTIjCzcN");
    string DfNcDvwkL = string("kkscjkgxTDNJLGfoupWLbVMtItEbEWqLUReJPtUPilOBNftVnAztiVbWxtkrYZDkehnvOnerjukClZkdLGyZmpAPtJBSNQVswfA");
    string nLWFOLtKQGuaQb = string("qRympnLKPxikqfhrfgqBpfMWDaMhZEDLlmlFthUlTftpXBoMCAezxRtFiHKLUMvwHuxXcadlQOaDjPxSBRuNVyLgDsgwGFlrzlrCIRJpfOjeENLUuKdCDBwKLyzMfempryBvhfiyhjJyZkdvpKqYpNtnFLOWBBIwlCQXKyuxdlaEhVCNWKsLrRIiLYgismlaNrzvbkRpqbMroDNBGZxUqwlAIJzTdQzUcwxnnGbBvhuWltXTElYmfJMYUk");
    string CzyEaLji = string("fLIXxsjNjxBSspBrjhnJRqUKAPgchRjicXrDZxfGpJvwzVyiMGQGLxKGNTgIUMEqrhiCabqelsWwMqPecbGMigrOficmnNSQhaFlkKaQRDlaBxxzCVrdfaAgnhqRsQoJGHMlESUzjLNMjgZzIoWBigEFVsRfSZLXZsxsPHrcydwLwSZBOiEsbbCdbKByOVVaBGykGEfGznnXYNRdIyjXyWBZGyUKOJrlzOwaMvcX");
    double xqLHxIg = 580177.9788172933;
    int hUPJohXR = 1107019912;

    for (int lWTlDGUMHbfXOgVb = 2084697167; lWTlDGUMHbfXOgVb > 0; lWTlDGUMHbfXOgVb--) {
        hUPJohXR += hUPJohXR;
        nLWFOLtKQGuaQb += oTMpkOgMCVM;
        QsiKxBKYfRQWl -= xqLHxIg;
        WSDNdiqQ = DfNcDvwkL;
    }

    for (int kyRtJCvguy = 602291343; kyRtJCvguy > 0; kyRtJCvguy--) {
        WSDNdiqQ = DfNcDvwkL;
        nLWFOLtKQGuaQb = DfNcDvwkL;
        xqLHxIg *= xqLHxIg;
    }

    for (int oJdNkZclvysQp = 1097843480; oJdNkZclvysQp > 0; oJdNkZclvysQp--) {
        continue;
    }

    for (int tKHXMgOZStdQN = 1820511295; tKHXMgOZStdQN > 0; tKHXMgOZStdQN--) {
        CzyEaLji += oTMpkOgMCVM;
    }

    return hUPJohXR;
}

string jwQCFi::GzklYafVczC(string rKDooO, string ffrDio, bool jmHZPaBh)
{
    string uwjkdodwgDsG = string("JpupSbVXKocgRocMZRCyHBNQsEuAPRDhePMWjhNFuKOBRgVLtJnSrnwwCmDRMMNRxAmDWiSnUjOaZgieIFogHPcSseBZDqHsrpcwdaFFRiDhILCVALDcqcdlgqkQUSAfBntEXsQoCDLSWHkKiOveaCRNfommKAoPI");
    bool mWZRyhgrYbefA = true;
    string iGchn = string("eeJmWkfUSDJssyPMnJhWNIqXtZqvATLywSzCRuUtIwZYdpQaXuPVOgIoSFFrrTLZHHzCZXZkhslHElqnXVKDivSyCoOxzvXUVXIbBYeQuaVyGyPEZPMheHGgzUDnBocqVzPkODylllhSLOckLLuCkNeOIyLTllOxPVWfZkybFAVfJFmIMxRqkVGKCyOP");
    bool BJcqJaJAWHkXjvf = false;
    bool JYtDbDGeCtozT = true;
    string wFAcp = string("TxCIGcDZcnqSXriQtEdgBEvPjcqcKOgXrZBbIrZlBcAZlKnaVmtyHSFLYFFpdSGVRVwlpIbQeTYxIpswOunSZAjgTlEdxrlAJgkOQNQqTtllbFmFhpRpdfsbRtdRg");
    int HDeczpbODhKXv = 223172516;
    string EOIruOIIaKDkqOYJ = string("UUYxOUaEGGuwcAZeMofWbiBGqRCECOESVSXlCaDIcMcLLyqckXVleDXaKebgnqOKFebaRKnlZPgpqjOAZUYpsTQqQEzmrrzCWmvlIITomwPmZVbhiYfLiVgKTlZJNrFkMMDtCWhArOyBSKjrgBpuniPoUbKbbfPoeWzYTXOWfhtXgMCLYyKNQyJrPcajbiKUAUPcykDWVN");

    for (int gTMrzJWonvUdS = 1403847372; gTMrzJWonvUdS > 0; gTMrzJWonvUdS--) {
        HDeczpbODhKXv += HDeczpbODhKXv;
    }

    for (int PcgQI = 85743383; PcgQI > 0; PcgQI--) {
        wFAcp = wFAcp;
        wFAcp = wFAcp;
    }

    for (int FQSjvljzQUtoEV = 1378115237; FQSjvljzQUtoEV > 0; FQSjvljzQUtoEV--) {
        JYtDbDGeCtozT = ! jmHZPaBh;
        EOIruOIIaKDkqOYJ += wFAcp;
        jmHZPaBh = ! JYtDbDGeCtozT;
        ffrDio += ffrDio;
    }

    for (int XFvqjmqDOFVWEqv = 1865098802; XFvqjmqDOFVWEqv > 0; XFvqjmqDOFVWEqv--) {
        EOIruOIIaKDkqOYJ += EOIruOIIaKDkqOYJ;
        wFAcp = ffrDio;
    }

    for (int ZEzBnQUL = 1017829712; ZEzBnQUL > 0; ZEzBnQUL--) {
        ffrDio += ffrDio;
    }

    for (int gwRjR = 1877148208; gwRjR > 0; gwRjR--) {
        rKDooO += wFAcp;
        EOIruOIIaKDkqOYJ += iGchn;
        rKDooO += wFAcp;
        HDeczpbODhKXv -= HDeczpbODhKXv;
        iGchn = ffrDio;
    }

    for (int OklUM = 1262458695; OklUM > 0; OklUM--) {
        BJcqJaJAWHkXjvf = mWZRyhgrYbefA;
        iGchn += ffrDio;
        uwjkdodwgDsG = iGchn;
    }

    return EOIruOIIaKDkqOYJ;
}

double jwQCFi::YdPZsCEg(int oivfAbQbFLEsuuK)
{
    string esiCY = string("oRMtYtCbtgsgBKlbtkxNcDPZLnrYSGRxcevRgxAKDPNPXJTXcTgzXwYiTdzDgTjqXuRmHwpZVbMXiJbECGqpWPtccnEnVr");
    string yZlhavkKXBWAYL = string("RGlhRHJScVICHwqxTGIWJvMSncegbGeeZNvpuLGoiEeGJdu");
    string zbzcXKJ = string("tRTjwIWdUFtJxcxObEYZcJrnJzsfbsWwZbHRwGCFyOZUGtlcFKWzGxsgABPHozPaISXGsrzuVkaDlXWxcqudfexeJxnihYvYKOIeRjPpcaXsfsDdTegJdkJrvNkaPtDfMC");

    if (esiCY > string("RGlhRHJScVICHwqxTGIWJvMSncegbGeeZNvpuLGoiEeGJdu")) {
        for (int LUumofCFVdJC = 1077169922; LUumofCFVdJC > 0; LUumofCFVdJC--) {
            zbzcXKJ = esiCY;
        }
    }

    for (int YnUaJcNvFGIMLT = 1348134592; YnUaJcNvFGIMLT > 0; YnUaJcNvFGIMLT--) {
        oivfAbQbFLEsuuK += oivfAbQbFLEsuuK;
        zbzcXKJ = zbzcXKJ;
        zbzcXKJ = yZlhavkKXBWAYL;
        zbzcXKJ = esiCY;
        esiCY = zbzcXKJ;
        esiCY += zbzcXKJ;
        yZlhavkKXBWAYL = zbzcXKJ;
    }

    if (zbzcXKJ >= string("tRTjwIWdUFtJxcxObEYZcJrnJzsfbsWwZbHRwGCFyOZUGtlcFKWzGxsgABPHozPaISXGsrzuVkaDlXWxcqudfexeJxnihYvYKOIeRjPpcaXsfsDdTegJdkJrvNkaPtDfMC")) {
        for (int ucuCvLxlZ = 385275812; ucuCvLxlZ > 0; ucuCvLxlZ--) {
            esiCY = yZlhavkKXBWAYL;
            esiCY += zbzcXKJ;
        }
    }

    if (zbzcXKJ != string("RGlhRHJScVICHwqxTGIWJvMSncegbGeeZNvpuLGoiEeGJdu")) {
        for (int RlgwtpupilHs = 393384378; RlgwtpupilHs > 0; RlgwtpupilHs--) {
            zbzcXKJ = yZlhavkKXBWAYL;
            oivfAbQbFLEsuuK *= oivfAbQbFLEsuuK;
        }
    }

    return 260598.46975414254;
}

int jwQCFi::WnHtNgjHPSvhQfm()
{
    double zEiDArGjDVVSRlLU = 131030.18046471376;
    double ZjhkqRaH = 210664.4848581488;
    bool GXErkYT = false;
    int ididRMclPSYY = 1171328470;
    string lRZJMaHxiiagRkoR = string("LXTltTFfmyXMgdXmilTaWXNbUmQKTWQDWdkBqQYcyHSWmdU");
    int lmZEkbdL = -1921192876;
    int xkGBsjtzNCoh = 1784417070;
    string XSFUSQmxUTyx = string("MAMtSSXvUxlbNLAkHTGQiKIBYHDDnmGwYbYwudtSkMPdwIxOOkbinOYIlAqWkCngbCybxwpGJNpUmBEqqJfTagSMEXsRWNPDsAMyMaiffJFkmatQxAsTpnTYpeEyubXezWlwxiduUeGiHjEPksgTCfDtGpXarCFcQwgwxzvbewtoaXcdGPCnzdkiPlmYGIQlNwpcwPHZNLQTZcHNkOVQUFvLIqWMqkFC");
    bool NYXhoCFyIinb = false;

    if (lmZEkbdL != 1784417070) {
        for (int BHkwtylADrybYy = 2074578219; BHkwtylADrybYy > 0; BHkwtylADrybYy--) {
            continue;
        }
    }

    for (int CxubnK = 1242822097; CxubnK > 0; CxubnK--) {
        XSFUSQmxUTyx += XSFUSQmxUTyx;
        ididRMclPSYY = lmZEkbdL;
        ZjhkqRaH -= zEiDArGjDVVSRlLU;
        NYXhoCFyIinb = ! NYXhoCFyIinb;
    }

    for (int RVWePXmDDorQO = 125906798; RVWePXmDDorQO > 0; RVWePXmDDorQO--) {
        ZjhkqRaH = zEiDArGjDVVSRlLU;
        ididRMclPSYY *= xkGBsjtzNCoh;
    }

    if (XSFUSQmxUTyx != string("MAMtSSXvUxlbNLAkHTGQiKIBYHDDnmGwYbYwudtSkMPdwIxOOkbinOYIlAqWkCngbCybxwpGJNpUmBEqqJfTagSMEXsRWNPDsAMyMaiffJFkmatQxAsTpnTYpeEyubXezWlwxiduUeGiHjEPksgTCfDtGpXarCFcQwgwxzvbewtoaXcdGPCnzdkiPlmYGIQlNwpcwPHZNLQTZcHNkOVQUFvLIqWMqkFC")) {
        for (int HoPEmsHP = 773042761; HoPEmsHP > 0; HoPEmsHP--) {
            lmZEkbdL /= lmZEkbdL;
            xkGBsjtzNCoh = ididRMclPSYY;
        }
    }

    return xkGBsjtzNCoh;
}

string jwQCFi::ZaIwemyqGhT(string krleejPXYHmtOc, bool FMKdXFEOp, string eqmXVHawITGBEh, string BtkWbkPAmP)
{
    int RSJRTvfn = -624970327;
    double svydbqG = -706001.1790264186;
    string VRXWV = string("QthyQYrGvkJWHewEazOgmrpKbwNSxhzewpjdsHitqwMl");
    bool EHCebNUxoNNfG = true;

    for (int AarPhWijdHoiOEQn = 1064919372; AarPhWijdHoiOEQn > 0; AarPhWijdHoiOEQn--) {
        eqmXVHawITGBEh = krleejPXYHmtOc;
    }

    for (int SvuCkSOPFgROE = 1453364595; SvuCkSOPFgROE > 0; SvuCkSOPFgROE--) {
        EHCebNUxoNNfG = FMKdXFEOp;
    }

    return VRXWV;
}

void jwQCFi::jXlDaNeFUW(bool SEMrBDAkDTVTyXb, int BwyOyUlx, string ABUvMpQdmoTz, int LNYCQWrapBJ)
{
    double ABHCiIdVpivtJBD = 94508.0890494731;
    bool TfRlZqozPg = true;
    int OUUMxiWvXiwv = 806259781;
    string cbBVYzm = string("UYqZpLAyTFIbwOmGIcKmgVuSXMhKPZFmznQQKAlaahTRDyVCPhGAXEnvqhFCLVcXcBSGvEIjSfeVZNxlxOaiFijocbSrjGrbSSVvdWeMmmWLkMIMcmyciDhodWlhdHgmolDEslQRZWmmWFUsbJyLfXaQebtOSLhTvFIKOjSDrpyLeHRwWezsxuDG");
    int hASSakKJoDEcHLRb = -1760221190;
    double rrAFbhUDF = 438084.85788016027;

    if (BwyOyUlx <= -1760221190) {
        for (int PjrqcHlYSURrMWZ = 1856865520; PjrqcHlYSURrMWZ > 0; PjrqcHlYSURrMWZ--) {
            hASSakKJoDEcHLRb /= hASSakKJoDEcHLRb;
            ABHCiIdVpivtJBD += rrAFbhUDF;
            TfRlZqozPg = SEMrBDAkDTVTyXb;
        }
    }

    if (OUUMxiWvXiwv > -311417858) {
        for (int NybeuQQRvrWYTCly = 43961213; NybeuQQRvrWYTCly > 0; NybeuQQRvrWYTCly--) {
            continue;
        }
    }

    if (TfRlZqozPg != true) {
        for (int DgbnOqvujqV = 1337770617; DgbnOqvujqV > 0; DgbnOqvujqV--) {
            cbBVYzm = cbBVYzm;
            LNYCQWrapBJ /= BwyOyUlx;
            BwyOyUlx += hASSakKJoDEcHLRb;
            BwyOyUlx /= hASSakKJoDEcHLRb;
        }
    }
}

string jwQCFi::SxyYP(bool UjikEUL, int CBTNAyR, bool dwOfrxrZmg, int CHlWK)
{
    double pcoukR = -170799.57219214077;
    double YFIVhnAwBy = -526516.7965781544;
    bool phLmAg = true;
    string UDRBzXJXXwFeNI = string("ZeRNeWMNsupPWsVmQAFHtxPUwleyxrutDYzocZlLVGdZPIpLXWxRPGPAyzOIsNpXGgNQFxNcfAUnbuBAVhGegHICJIOHpIbpgnzkIihRTvWIhphcRncETqeyTr");
    int PCjDbQfm = 651077316;
    int XkIChB = -20627552;
    bool ayDbcajcs = false;
    bool IlzODDBObhmZoa = true;

    for (int UrrRZJWxEmHk = 1732949607; UrrRZJWxEmHk > 0; UrrRZJWxEmHk--) {
        continue;
    }

    return UDRBzXJXXwFeNI;
}

jwQCFi::jwQCFi()
{
    this->wzfyULFVYQZtREm(string("fcpyctUrgrkMRpJEGWLmFxOaMHJlkkYXAJWaTJwfgyTAxFZCBFNlWqOUxaWUucmWbFwLvWTbiYhcqpUJ"));
    this->TGOQMgiKLsAgT(true, 1678579127, -306192.2114004974, true);
    this->AwtPyKxXUnv(true, 531271725, -389300.2976167336);
    this->LWMLlXzxT(string("uVkEzutKXrTXaYtIyEwFGjwSTZigpBOQfxDYiSOctgLSbYFlrbrlgCvfORzLeKphcagXGUwUMnluZJJFISdBAvJwCMbJmCygmUdrvApQOctqxBoOIDlrfqgbwmExanDqsXdLtWT"), -63444.42700631752, string("RgDrEDZVqmttyBMnaSHDCMBiBbdYTkFoXeVtsqmYUsdWRZAPUQKUODRyQjhvoXsMyOOWXbfGdjVukpGFxyDOcdGGmhkFGzXbXoqbAhZRLKXSKnFlUVjKxkblmzOswPbVBJliT"));
    this->uxtvVWLrJtKzypUj(true, -1509057204);
    this->trLXcUJbKGIQQ(-836793.9782051633, -1772186361, -523657.6959527289, -1546861116, -698791.9099286989);
    this->gqbHRCZjDFOiCGd(true, -1810368532, 210228929);
    this->EItzCroFAZ(256309.13231746558, false, true);
    this->GzklYafVczC(string("cixfButLSDroWUjTKUGhVsOhCCvEDHrBIWvmVPHfpKpOFADoPUInPiDreXVUtACmwrDRmqfopDXlEMykmSrIwcKeSpjAecDUHHmizQsfdydQlrdDlQWEvlufdSAkTRpivlrhwefNHGLXhWhzrcAurlJYveaB"), string("ZHIWlxJGjJeSw"), true);
    this->YdPZsCEg(-600526017);
    this->WnHtNgjHPSvhQfm();
    this->ZaIwemyqGhT(string("nciJVuoqLxurwCRRBZBvrisEBiBkLEohlygYXSsLIctDwiPEGhXzatYVRNLvQvEegMdaNdfeTkUhGjssWSWdRPktSyVIMTtgbeizdwYphRyhKGGfnsPgudDnyWOcecKHjJsoUHgTWmcZOKZWWDxEgxPhNsEnMEkHBkVHZNqlGtrUXpDkUMSElJtAohsAtaqsZPHBEOcKcYd"), true, string("BdFJWqojklQCZDkCLOBZBvnDPzvtsiykxVtydbHWoRFOFEwTmgWCtMObqkUBycoMQRNByyuGeNSySIKVkHRygfPUnKsbbbMYzcxKBReDAMTKcziZRVURUnYxqXufWLctLYWMfpPKbcqPrQFDkejhUwKqwwNlysJnUDqxChgEAeJOcTqJjpLWeLqTzIdIgrZiyJTpfjAVYyYSRSCsU"), string("OgayBkgmOdvjTDJvGzYkPEErJhnXSsMOZHDzATPSEKsIPsNgNteuiYrfWTGTUxxuSzMNLQmRJhaYNMWVlRKqICfbNYHwdy"));
    this->jXlDaNeFUW(true, -1471926035, string("DChQKfpcZrwtMDFcCSsmsIolJCrbpLIoMTeELzfNguNBvaJBrASOkOMlpAhLCksAfcTLESCKkDZGfeeVOhsRohhpbNgtYUeYGfymNVPbkfxNguX"), -311417858);
    this->SxyYP(true, 654544645, false, 1413041540);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PbmkkG
{
public:
    double wfGZkWCBZvYwSye;
    double PFlVwiPZvJyaKXuj;
    int PECURrEH;

    PbmkkG();
    double llZtWzcYGYplX(bool dnRysAgLSaNdBCr, string mQjInBUq);
    bool QpxJmFsnA(string jPXeVN, bool ZosgZW, string zeaLGHhRW, bool dCKOVazPkErQZ);
    bool ZnmBeSDbmdP(bool KCHptPfufTZpxkE);
    string fgMyNintxBgMVW(int ObLzYff, string OfevXLSHXHA, bool cvSFEFCoXFXmnGu, double FSdkJtHBQhjgpe, bool ngkPAknKXTCoe);
protected:
    double VQeuKiMDGDxi;

    bool zxfqItUi(bool WgMDTpGm);
    double HeCXQZWG(string MCBBChnKg, string LoVGuIqsWTUTZSQd, bool LpEaaW, bool oPsWrrVl, string vZDZQIiaM);
private:
    string AJWaGznZkOdWuO;
    bool xsvzd;
    string pROKJyGiXjJIDt;
    int SzbzCkMyvJ;
    double ImlDCcfUgKLyxrPO;

    bool sxFjJwKPWSc(int HeHDXAZu, bool WDkNiopO, bool cwcXROxgF, double pLojuqDrhErXcY);
    string rmVVB(double NWwyMulRyAm);
    void UuTKW();
};

double PbmkkG::llZtWzcYGYplX(bool dnRysAgLSaNdBCr, string mQjInBUq)
{
    double xSnmsWg = 504070.80454012053;
    string RgBiPrwdHEpxVM = string("YioTwZyzmXtgZyRvEZTXFLrwRyhJCUIRKOJAgUQIzSoMcHSHco");
    string twHVUPAeCxR = string("UXYuyJYRttvYSbrVCVGiTlZLJR");
    string yMOKu = string("QQrHuuIjyjxQFuhvpqjkmeEvvFWjeKYvxeXpghlJMXwfCDXbUQRWBYwqrazrJYPkrkKfiNSPPHmymtRvpYlSCyhWwbqmPamwTJqKqVYaIwuBEwMNoqISpZAwvpzfuWpzVdZJIHkyQQnkfbEJSxHNaNdygAVByuFyWNvMPYlgXvV");
    bool gsltZxbLzFuqnjW = false;
    string bVOmIGnr = string("RqXrnaDvskBwAMgSCweT");
    int nQUPgrbayymKHuI = 5880337;

    if (bVOmIGnr == string("UXYuyJYRttvYSbrVCVGiTlZLJR")) {
        for (int IGcVHEntAMqMhKzl = 1079905419; IGcVHEntAMqMhKzl > 0; IGcVHEntAMqMhKzl--) {
            continue;
        }
    }

    return xSnmsWg;
}

bool PbmkkG::QpxJmFsnA(string jPXeVN, bool ZosgZW, string zeaLGHhRW, bool dCKOVazPkErQZ)
{
    int XFgXBlq = 1130751552;
    bool CsCucq = true;
    int uLexCX = -279644235;
    int OFnlbqTxMrfb = 716864892;
    double KTobEbLo = 904614.2296322241;

    if (OFnlbqTxMrfb != 716864892) {
        for (int nsuVrOdvQMKWXR = 159704145; nsuVrOdvQMKWXR > 0; nsuVrOdvQMKWXR--) {
            dCKOVazPkErQZ = ! dCKOVazPkErQZ;
        }
    }

    for (int iNvTj = 884269292; iNvTj > 0; iNvTj--) {
        uLexCX = uLexCX;
    }

    return CsCucq;
}

bool PbmkkG::ZnmBeSDbmdP(bool KCHptPfufTZpxkE)
{
    bool wuGicrQGbACUd = true;
    int nQAQLFbDNfaf = 734672340;
    string oQmjgVcMhLb = string("DOrukvYdsiImUFdPyLvqatAFYFFfUvkgxkSqElruqFpWwAEPLVYAnKYUYcIzTRrkkghImfhBWvJHEmIodxib");
    int ZcNuM = -1778788173;
    double owtGHpwQzR = 195051.55816260492;
    double oYUoURMgyGCA = 1042668.6201088027;
    int XbTcZ = 580959203;
    int VojWG = 1323270493;
    bool ArxmGccrlOi = true;
    string eClFsr = string("XcvSeMqwIvo");

    if (ZcNuM == 1323270493) {
        for (int DgMzlslYOsNIsOJg = 504172115; DgMzlslYOsNIsOJg > 0; DgMzlslYOsNIsOJg--) {
            VojWG *= XbTcZ;
        }
    }

    if (eClFsr <= string("XcvSeMqwIvo")) {
        for (int LUfZrtPNjsdcNv = 214563572; LUfZrtPNjsdcNv > 0; LUfZrtPNjsdcNv--) {
            continue;
        }
    }

    for (int wZxMMNrsTeE = 1129836073; wZxMMNrsTeE > 0; wZxMMNrsTeE--) {
        ArxmGccrlOi = ! KCHptPfufTZpxkE;
    }

    for (int mfTnLO = 626138705; mfTnLO > 0; mfTnLO--) {
        wuGicrQGbACUd = ArxmGccrlOi;
    }

    return ArxmGccrlOi;
}

string PbmkkG::fgMyNintxBgMVW(int ObLzYff, string OfevXLSHXHA, bool cvSFEFCoXFXmnGu, double FSdkJtHBQhjgpe, bool ngkPAknKXTCoe)
{
    double fISUSRwJkbeEo = -448110.04134613054;
    int gdKiZGIwfz = 1967762291;
    string cYUXU = string("eILeSjPXUNBdBrPgkqbfXxXjLaIUoV");
    int lELZdarONumfYCy = -787188430;
    double NArVqF = -599488.397095331;

    for (int WJFmRIVTpRMRCaX = 796890320; WJFmRIVTpRMRCaX > 0; WJFmRIVTpRMRCaX--) {
        gdKiZGIwfz -= lELZdarONumfYCy;
        lELZdarONumfYCy /= gdKiZGIwfz;
        ObLzYff += ObLzYff;
    }

    for (int txSFwpUqDLR = 521779327; txSFwpUqDLR > 0; txSFwpUqDLR--) {
        continue;
    }

    for (int kAMLmOhU = 674458063; kAMLmOhU > 0; kAMLmOhU--) {
        OfevXLSHXHA = cYUXU;
    }

    for (int qTSGrthLEbDS = 1370995349; qTSGrthLEbDS > 0; qTSGrthLEbDS--) {
        ObLzYff *= ObLzYff;
        cYUXU = OfevXLSHXHA;
        lELZdarONumfYCy += ObLzYff;
    }

    for (int WprrSnVeKt = 1831093586; WprrSnVeKt > 0; WprrSnVeKt--) {
        continue;
    }

    for (int EIyCDdOaqseMi = 1533791126; EIyCDdOaqseMi > 0; EIyCDdOaqseMi--) {
        NArVqF = FSdkJtHBQhjgpe;
    }

    for (int ApEgWbbBk = 1215957842; ApEgWbbBk > 0; ApEgWbbBk--) {
        continue;
    }

    return cYUXU;
}

bool PbmkkG::zxfqItUi(bool WgMDTpGm)
{
    string ZVpdB = string("CmfNNLHsfmsQxXWLysEODpHogRnmRyrZvwRgROhgQVyqGyUSZCnTYTvtHTrxjRAGkMUyvHHRnhnLHnkqMlysJEMlXsysMmiJJwYYUAllIyXiAuEWSYRoBRKvVvmmfVpzHVpOpiokdgJIxRRlYaBwCInCpazfRviITsiKGft");
    string NHscwkQarzYEf = string("HhBWgMjineQckzQhnCbJoQkNlskTtsjgjKTtjWomxCSLhxVOilvkzgePrcwWcvZMLkxrpvhwKULdkshYHgfCDxWOaNyJheyxzdpyKJRMYyw");

    for (int tnOzdxmw = 1361766358; tnOzdxmw > 0; tnOzdxmw--) {
        NHscwkQarzYEf += NHscwkQarzYEf;
        ZVpdB = ZVpdB;
        ZVpdB += NHscwkQarzYEf;
        WgMDTpGm = WgMDTpGm;
        WgMDTpGm = WgMDTpGm;
    }

    return WgMDTpGm;
}

double PbmkkG::HeCXQZWG(string MCBBChnKg, string LoVGuIqsWTUTZSQd, bool LpEaaW, bool oPsWrrVl, string vZDZQIiaM)
{
    string zuNrlfWKXd = string("DHJJIuQncLvHryCqfWtjVVWVEQuPlMGwbGJdXYwczBGzCpuaxDrjwwXiANRKUfqEAzRDkHcIDjFaTpApaNJUxjTEiDQMvAqAhKMZIWabREOGEnoOPoHCrQHqSYAbDPLeBUGVRMGQYvMBkKlEDhZQdcYAeaONKIJnKeqp");

    for (int kslSsZUOs = 199933269; kslSsZUOs > 0; kslSsZUOs--) {
        vZDZQIiaM = MCBBChnKg;
    }

    if (LoVGuIqsWTUTZSQd == string("IyVWGcCaVxFLVrNQjSZGVNtPVhaPdaJLCTEqczpwANJBUcziaauFwZVunugPeolYaoKReFouchMNzUYXPjVcDcZdRYzSsfxSzgOXfGSVsdtSmViBfFmTAPmFthmyfbVKDStvzqqwRgzAGgdCuIJHDDqwQuymVjuojn")) {
        for (int ZRwgNGmZDNPjZN = 872955233; ZRwgNGmZDNPjZN > 0; ZRwgNGmZDNPjZN--) {
            zuNrlfWKXd += MCBBChnKg;
            MCBBChnKg = vZDZQIiaM;
        }
    }

    return -503056.3586634452;
}

bool PbmkkG::sxFjJwKPWSc(int HeHDXAZu, bool WDkNiopO, bool cwcXROxgF, double pLojuqDrhErXcY)
{
    double LxFZkwco = 925875.4939746665;
    double QVkVnampSZMH = 381527.01461455855;
    bool mVFEdGyRLBxrcDt = true;
    double zgPIL = -486396.0479308478;

    for (int SqTKfHLJ = 2089077747; SqTKfHLJ > 0; SqTKfHLJ--) {
        cwcXROxgF = WDkNiopO;
        cwcXROxgF = cwcXROxgF;
        QVkVnampSZMH = QVkVnampSZMH;
        pLojuqDrhErXcY -= pLojuqDrhErXcY;
        zgPIL *= pLojuqDrhErXcY;
        LxFZkwco -= QVkVnampSZMH;
        LxFZkwco -= pLojuqDrhErXcY;
    }

    for (int RWxaUokoAypG = 505910129; RWxaUokoAypG > 0; RWxaUokoAypG--) {
        QVkVnampSZMH += pLojuqDrhErXcY;
        LxFZkwco /= LxFZkwco;
        LxFZkwco -= zgPIL;
        pLojuqDrhErXcY *= zgPIL;
    }

    return mVFEdGyRLBxrcDt;
}

string PbmkkG::rmVVB(double NWwyMulRyAm)
{
    double zzhHqM = 123976.79613395101;

    if (zzhHqM > 123976.79613395101) {
        for (int enfSXXVX = 1710945253; enfSXXVX > 0; enfSXXVX--) {
            NWwyMulRyAm -= zzhHqM;
            zzhHqM -= zzhHqM;
            NWwyMulRyAm += zzhHqM;
            zzhHqM *= zzhHqM;
            NWwyMulRyAm -= zzhHqM;
            NWwyMulRyAm += zzhHqM;
            zzhHqM += zzhHqM;
        }
    }

    if (NWwyMulRyAm != 123976.79613395101) {
        for (int fAJIdyNcIFprzoyr = 560317954; fAJIdyNcIFprzoyr > 0; fAJIdyNcIFprzoyr--) {
            NWwyMulRyAm += NWwyMulRyAm;
            NWwyMulRyAm = zzhHqM;
            zzhHqM += NWwyMulRyAm;
            NWwyMulRyAm += zzhHqM;
            NWwyMulRyAm *= zzhHqM;
            NWwyMulRyAm -= NWwyMulRyAm;
            zzhHqM /= zzhHqM;
            NWwyMulRyAm -= zzhHqM;
            zzhHqM *= zzhHqM;
        }
    }

    return string("lkFrGdSgGvcbIdGTonDGbhoppczbnrWdfTjSEPRZIEgFTUfyBYvnzkFujbqShemQhmjpxPqrwJcBdBPNsaXjmTSdcvazaxYnnblexobAPmAwfyqNWwu");
}

void PbmkkG::UuTKW()
{
    int NhCPiAWk = -1925768772;
    bool YdqicChacjiGKtcO = true;

    if (YdqicChacjiGKtcO == true) {
        for (int ioTZskzAxRGZZCqc = 522145037; ioTZskzAxRGZZCqc > 0; ioTZskzAxRGZZCqc--) {
            YdqicChacjiGKtcO = YdqicChacjiGKtcO;
            NhCPiAWk = NhCPiAWk;
            NhCPiAWk -= NhCPiAWk;
            NhCPiAWk /= NhCPiAWk;
            NhCPiAWk /= NhCPiAWk;
        }
    }

    for (int IsxsTCvsRI = 1742930645; IsxsTCvsRI > 0; IsxsTCvsRI--) {
        YdqicChacjiGKtcO = YdqicChacjiGKtcO;
        YdqicChacjiGKtcO = YdqicChacjiGKtcO;
        NhCPiAWk += NhCPiAWk;
    }

    if (YdqicChacjiGKtcO == true) {
        for (int ScgIWBrMfcNLusRK = 60948299; ScgIWBrMfcNLusRK > 0; ScgIWBrMfcNLusRK--) {
            NhCPiAWk -= NhCPiAWk;
            NhCPiAWk *= NhCPiAWk;
            NhCPiAWk = NhCPiAWk;
            YdqicChacjiGKtcO = ! YdqicChacjiGKtcO;
            NhCPiAWk = NhCPiAWk;
            YdqicChacjiGKtcO = YdqicChacjiGKtcO;
        }
    }
}

PbmkkG::PbmkkG()
{
    this->llZtWzcYGYplX(true, string("iqTFYOGYhfjSGkgEQmQlmNynFChRHwRvnvZjdseQzGLzVyhefYaYtMEbNqgrPDiifVZMtaZzJEfXXHbuxwHWvjMaoyLntlGInLiHwFbmMCnkFpUTRVznpFOAXHsoKIUXuxYldEsgQPO"));
    this->QpxJmFsnA(string("Rut"), true, string("egyVSxndaLfrRVGOuQTufLQevTrrCqsKNOGjrYyL"), false);
    this->ZnmBeSDbmdP(true);
    this->fgMyNintxBgMVW(-350779070, string("vszGVgowlosnyJgNzVXwljjHvCKtGvINKxDBkDOTyazDQUaudFMFdkQyNXGPSmxLFWpqDTYIIGpMJ"), true, -662307.5268437294, false);
    this->zxfqItUi(false);
    this->HeCXQZWG(string("itZrQYUysHsinjTWbFdllmaVJsKycTYAqjysyXpVcdchXqPfAtBBBlXxedsCnUYRtMnjk"), string("qRSiolukHCBOelYcpHvCrqHOUXGcUktjwrmicFmgIqTjJntIfZNfzJQZTlsHCIEbFSDOHWqQUyHdickXgfTbGdpoNkKsFCwgwBOvQgNDpbZHbVOxMGmzqbtSpXQUstzdFaTFOuLcxUJfXeKBJbSoUTzqMwUdq"), true, false, string("IyVWGcCaVxFLVrNQjSZGVNtPVhaPdaJLCTEqczpwANJBUcziaauFwZVunugPeolYaoKReFouchMNzUYXPjVcDcZdRYzSsfxSzgOXfGSVsdtSmViBfFmTAPmFthmyfbVKDStvzqqwRgzAGgdCuIJHDDqwQuymVjuojn"));
    this->sxFjJwKPWSc(1828510432, false, false, -132072.02260337837);
    this->rmVVB(-729992.9460856515);
    this->UuTKW();
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cNkpdpmZ
{
public:
    bool ugqgI;
    double loGlSmQ;
    int rLeMYzSTHvXC;
    string jvmSn;
    bool zYdmlwGlcJSCx;

    cNkpdpmZ();
    int iahXirODNMZ(double rOdQHOQLuQwloT);
protected:
    double rIwhrEJUv;
    bool ouCNz;
    bool sgZqwrDhpBsXuiLn;
    string ZEcNxwXS;
    string XNmQAMoieJUC;
    double eLfsDtofESXz;

    double dfCZXKwvtAtsH(int rFiFzyaPMjP, bool sHdYj);
    string nUTTYwojnJ(string LDfASqCmqiAjBUy, bool bEyerudoLG, string dgywibMpdr, double pAxSGuXtAOii, int icuoLxCeQkywu);
    bool OtqRuwDGKzjPYfML(double cvLaaFmQKp, string PZJjpFpPrti);
private:
    string YVmSDPhPeE;

    string XUsKvAFCBLnTGw(int MneErWD, string bajhMP);
    double DLKkyrQ(string CXGCARCfAkJla, string QoCpRb, bool mlFiqOoQacQdU);
    void EpXibSqftSlHA(bool fZDTQskmbbWS, double SLlaOkSPdSJwI, double AdUiKultxjJWsce);
    string UVVsisIBinVptK(int jqasIETynsQN, double ydTwlo, string CXfyhPzyUQi, int DxAFDn, double VGqORkzYEqCBp);
    double XXKcSFFuMkNrLW(string DAKcWWEnNVIlARl, int nVTdNATCk, bool qiIfTdPHRktQuDQo);
    double oSGeeoWTJ();
    int VXcsxaGEpTOfrmt(double TlDHKZ);
    double vzREUwvOsNtAIb(double JZSvquXeCUPhrhz, int kqtIVbji, bool lhowk);
};

int cNkpdpmZ::iahXirODNMZ(double rOdQHOQLuQwloT)
{
    string NDfFf = string("uXOCFtGCFHHFnWrnNSiqwGSdyQQxzSOUPcLluDcucGrTOqYPRIECnLvOypuVIjAvhgIVebvamuJjQWJwxQqaWnxDnygBvTJFPuEScIhkEzNCEzMNbm");

    if (NDfFf >= string("uXOCFtGCFHHFnWrnNSiqwGSdyQQxzSOUPcLluDcucGrTOqYPRIECnLvOypuVIjAvhgIVebvamuJjQWJwxQqaWnxDnygBvTJFPuEScIhkEzNCEzMNbm")) {
        for (int XuxVNPVWJtbdD = 1874988854; XuxVNPVWJtbdD > 0; XuxVNPVWJtbdD--) {
            NDfFf += NDfFf;
            NDfFf += NDfFf;
            rOdQHOQLuQwloT = rOdQHOQLuQwloT;
            rOdQHOQLuQwloT += rOdQHOQLuQwloT;
            NDfFf += NDfFf;
            NDfFf += NDfFf;
        }
    }

    if (rOdQHOQLuQwloT > -142575.14188728534) {
        for (int ZHsltNXTe = 1220970152; ZHsltNXTe > 0; ZHsltNXTe--) {
            rOdQHOQLuQwloT = rOdQHOQLuQwloT;
            rOdQHOQLuQwloT *= rOdQHOQLuQwloT;
            rOdQHOQLuQwloT = rOdQHOQLuQwloT;
            NDfFf += NDfFf;
            NDfFf = NDfFf;
            rOdQHOQLuQwloT += rOdQHOQLuQwloT;
        }
    }

    if (NDfFf <= string("uXOCFtGCFHHFnWrnNSiqwGSdyQQxzSOUPcLluDcucGrTOqYPRIECnLvOypuVIjAvhgIVebvamuJjQWJwxQqaWnxDnygBvTJFPuEScIhkEzNCEzMNbm")) {
        for (int KpdurYFicFtJ = 30118891; KpdurYFicFtJ > 0; KpdurYFicFtJ--) {
            continue;
        }
    }

    return -2006772146;
}

double cNkpdpmZ::dfCZXKwvtAtsH(int rFiFzyaPMjP, bool sHdYj)
{
    int xNEipLcglF = 1112366508;
    int OuEgwBWSSGTZwjz = -240541580;
    double hctkUTQzVrSxT = 845543.3368682882;
    string wuIQGAHIdpVZgD = string("PwaploVCidgRVUQmJTtAiZWMFNziiLkfTqjlizzduRwteLPkvYyIwlzDQbYKJeFPBzARnusswUXDcgHYICzlAYaHmOgmFCFtCPjRHhwsHEhIqgf");
    string KEoZOZsMa = string("dviKlPSUkWwJMBSaGQgIlwYtTLYuQKgTkhEZnBFvNbIeWsUGZBPegMIamyyprwcbjybJndGgLgfHUteBfuCoykHjzhjKCvWktIRYNAVAhekmCErwltQGJGgqLQRNhbLWIKPKRegdTDmztouMnTlSsNgAMAkqEuOrlQhszRBqjcJJVCnJJiKDtMIkfWfgHauIQzoUbptrjUOmXLmkKOzEoKCqxdykXfw");
    int RgVPZ = 40167516;
    bool EWtqxCWBaCnpMW = false;
    string WXFrSouamF = string("XZFcQMnaSTGGxEuWtIocrLHUDZtKLIMQnNrcqwKhfYsstMvVhQOlpSuacsvAQNfsfCnOuqRXBhBMGTGqhQGAKVYVHvqGWSqpSczwxyfTHLZgLKVdUIxhWcaDFrwNBCzAuDDDkHbgzECpTqukfxYhWALPgjXqSstSYwMqPugxJdAnEjzwNYxzEeKiXPEOmhCDWtsfhgDECAtwbVMODxIHxiziOvFJHX");
    int vXwKi = -1607839917;
    int YjhRUbgq = -1766914293;

    if (rFiFzyaPMjP <= -1607839917) {
        for (int AGeMJNPUphcKe = 647386872; AGeMJNPUphcKe > 0; AGeMJNPUphcKe--) {
            OuEgwBWSSGTZwjz *= YjhRUbgq;
            OuEgwBWSSGTZwjz += xNEipLcglF;
        }
    }

    for (int nOWSSxBsz = 924501; nOWSSxBsz > 0; nOWSSxBsz--) {
        xNEipLcglF *= vXwKi;
        RgVPZ /= YjhRUbgq;
        OuEgwBWSSGTZwjz = xNEipLcglF;
        OuEgwBWSSGTZwjz -= YjhRUbgq;
    }

    for (int nVbLovnDuzMg = 1019474500; nVbLovnDuzMg > 0; nVbLovnDuzMg--) {
        KEoZOZsMa += KEoZOZsMa;
        rFiFzyaPMjP = YjhRUbgq;
        OuEgwBWSSGTZwjz += RgVPZ;
    }

    for (int pjFKrCQgHxpEwV = 1281498275; pjFKrCQgHxpEwV > 0; pjFKrCQgHxpEwV--) {
        xNEipLcglF += OuEgwBWSSGTZwjz;
        EWtqxCWBaCnpMW = ! sHdYj;
    }

    for (int NlgksIbzmp = 326788634; NlgksIbzmp > 0; NlgksIbzmp--) {
        vXwKi *= OuEgwBWSSGTZwjz;
        rFiFzyaPMjP -= RgVPZ;
        wuIQGAHIdpVZgD = KEoZOZsMa;
        RgVPZ /= vXwKi;
        OuEgwBWSSGTZwjz -= rFiFzyaPMjP;
    }

    return hctkUTQzVrSxT;
}

string cNkpdpmZ::nUTTYwojnJ(string LDfASqCmqiAjBUy, bool bEyerudoLG, string dgywibMpdr, double pAxSGuXtAOii, int icuoLxCeQkywu)
{
    bool DmhhoFs = false;
    string pLWJddXl = string("vfEhfltByGjZObQocsmPYVTjdxXzcYAjkgWxordqfwjlikyNNMQCQjpYSPQDPZEqQGAcpKQpiMMbtHWhmjZxroaxGQZFdaIdoQxDFWnwyaZPzIMmxHOOtmKPzKkKJublyDjtfGCRLRJszPEYAqRNHq");
    bool SffVjhQqL = true;
    double QJaScXxIIs = -671917.9787041993;
    string cxpVFzbz = string("xukiQdhOJirITQDKppaqFUnKVhrohTNfqXgJUGFKNTEkFJuQgoPjjVVWGWyRsehnVyOePnqDNNZrlIDjAlQkhrbfHIwKJUEhvwSfqqpzbyexifxTxZsMWVxPJQJHJGjhfgHpEvivtlQNuxQCYvlqhwPPTGyIHzyPoRCUhzpwLjGiZCcSDHqFICByMJljdybFotKTJuunwBOTlpOMomrxfBFg");
    int OxkapsskDfhouTbs = 304215950;
    int lkBCiBziKxPwI = -1794196833;
    int fGJtMSXjwNA = 1463623189;
    string iBODPMew = string("uBBKLkEfCTzYxsRXoYODqjZtYJqSewprMrYhsVQzdTwnyYtCnJtFwpGgICpEkhTKVYyCBowHkBwBseunGKXIWlhVfTJlPRwtKVxGPMWzhUMYAMFscqvLhorRwxRASGqWYkCKqDqnnsaPKhJMgLQNMEBUQcgatGuwpsAdQmVenhMdfpJlwDOhKpQUoNEhbnVbHMRqSXRsxwSnwTkUmUWFGedHnOwBaRTtZPWJQrOoFFJpwecQggWYjtTk");

    for (int uJvme = 72901072; uJvme > 0; uJvme--) {
        cxpVFzbz += pLWJddXl;
    }

    for (int qLEpfrB = 1779739390; qLEpfrB > 0; qLEpfrB--) {
        QJaScXxIIs += pAxSGuXtAOii;
    }

    for (int KKPHCAJoWR = 774275408; KKPHCAJoWR > 0; KKPHCAJoWR--) {
        icuoLxCeQkywu -= OxkapsskDfhouTbs;
        cxpVFzbz += iBODPMew;
    }

    for (int SkNevbktXJjCIMtf = 701700880; SkNevbktXJjCIMtf > 0; SkNevbktXJjCIMtf--) {
        fGJtMSXjwNA *= fGJtMSXjwNA;
        iBODPMew += iBODPMew;
        iBODPMew += cxpVFzbz;
        pAxSGuXtAOii += pAxSGuXtAOii;
    }

    return iBODPMew;
}

bool cNkpdpmZ::OtqRuwDGKzjPYfML(double cvLaaFmQKp, string PZJjpFpPrti)
{
    double DOhHkNW = 111021.43646506447;

    for (int jshxQHgOTZ = 1175245149; jshxQHgOTZ > 0; jshxQHgOTZ--) {
        cvLaaFmQKp *= cvLaaFmQKp;
        PZJjpFpPrti += PZJjpFpPrti;
        DOhHkNW *= cvLaaFmQKp;
        cvLaaFmQKp *= cvLaaFmQKp;
        cvLaaFmQKp *= cvLaaFmQKp;
    }

    if (DOhHkNW != 54254.97877035632) {
        for (int wCCZu = 1134091143; wCCZu > 0; wCCZu--) {
            PZJjpFpPrti += PZJjpFpPrti;
            cvLaaFmQKp -= cvLaaFmQKp;
            DOhHkNW *= cvLaaFmQKp;
        }
    }

    for (int GXUyIbxIGECE = 1803902405; GXUyIbxIGECE > 0; GXUyIbxIGECE--) {
        PZJjpFpPrti = PZJjpFpPrti;
        DOhHkNW *= cvLaaFmQKp;
        cvLaaFmQKp *= cvLaaFmQKp;
        cvLaaFmQKp = cvLaaFmQKp;
        cvLaaFmQKp /= cvLaaFmQKp;
        cvLaaFmQKp /= DOhHkNW;
        DOhHkNW *= DOhHkNW;
    }

    for (int phnxSKEmfQqRKuf = 497059138; phnxSKEmfQqRKuf > 0; phnxSKEmfQqRKuf--) {
        continue;
    }

    for (int CPseydZ = 1836695211; CPseydZ > 0; CPseydZ--) {
        continue;
    }

    return false;
}

string cNkpdpmZ::XUsKvAFCBLnTGw(int MneErWD, string bajhMP)
{
    bool SOGpbYkiqGmvz = false;

    if (SOGpbYkiqGmvz != false) {
        for (int rFalWmQLkesfP = 2122721420; rFalWmQLkesfP > 0; rFalWmQLkesfP--) {
            SOGpbYkiqGmvz = ! SOGpbYkiqGmvz;
            MneErWD *= MneErWD;
            bajhMP += bajhMP;
        }
    }

    if (MneErWD == 1676920422) {
        for (int wzPVmEFoFgO = 1200388728; wzPVmEFoFgO > 0; wzPVmEFoFgO--) {
            MneErWD -= MneErWD;
            bajhMP = bajhMP;
        }
    }

    if (MneErWD <= 1676920422) {
        for (int pCKIHeQKyiq = 1792841756; pCKIHeQKyiq > 0; pCKIHeQKyiq--) {
            bajhMP += bajhMP;
        }
    }

    return bajhMP;
}

double cNkpdpmZ::DLKkyrQ(string CXGCARCfAkJla, string QoCpRb, bool mlFiqOoQacQdU)
{
    bool rCFmwjZXwZqPUJY = false;
    double QAqiB = 79405.81501787195;

    for (int MENQhsgkkhMF = 1826210952; MENQhsgkkhMF > 0; MENQhsgkkhMF--) {
        QoCpRb = CXGCARCfAkJla;
        QoCpRb += QoCpRb;
        rCFmwjZXwZqPUJY = rCFmwjZXwZqPUJY;
        mlFiqOoQacQdU = mlFiqOoQacQdU;
    }

    for (int UyXHPhCAL = 718244098; UyXHPhCAL > 0; UyXHPhCAL--) {
        QoCpRb = CXGCARCfAkJla;
        mlFiqOoQacQdU = mlFiqOoQacQdU;
    }

    for (int tVOHzTqENgTJH = 1283180510; tVOHzTqENgTJH > 0; tVOHzTqENgTJH--) {
        mlFiqOoQacQdU = ! rCFmwjZXwZqPUJY;
        mlFiqOoQacQdU = ! rCFmwjZXwZqPUJY;
        CXGCARCfAkJla += CXGCARCfAkJla;
        mlFiqOoQacQdU = ! rCFmwjZXwZqPUJY;
        CXGCARCfAkJla += CXGCARCfAkJla;
    }

    if (rCFmwjZXwZqPUJY != false) {
        for (int mPNcMgzsZTBBLyHj = 690210353; mPNcMgzsZTBBLyHj > 0; mPNcMgzsZTBBLyHj--) {
            CXGCARCfAkJla = QoCpRb;
            rCFmwjZXwZqPUJY = ! rCFmwjZXwZqPUJY;
        }
    }

    for (int qHDYnlDyaf = 498049923; qHDYnlDyaf > 0; qHDYnlDyaf--) {
        continue;
    }

    for (int ysJUqCWyPknyODFX = 1114338322; ysJUqCWyPknyODFX > 0; ysJUqCWyPknyODFX--) {
        QoCpRb += CXGCARCfAkJla;
        QoCpRb += QoCpRb;
        mlFiqOoQacQdU = mlFiqOoQacQdU;
    }

    return QAqiB;
}

void cNkpdpmZ::EpXibSqftSlHA(bool fZDTQskmbbWS, double SLlaOkSPdSJwI, double AdUiKultxjJWsce)
{
    double ZcaOho = 687606.5532263499;
    string xNuawai = string("HEJNgNqZlHznHMyeYXEXiBIhUmyMUsTNBIYKGiHJqIsTdrxSHVCIBQPfpRNuKtcfpoRAKALJEQPbEUKHDdVOrUfgyTbXDzLBlPQCXVgojSlJQlLQfZSKjQFPeFwNUnfwSDAdoCMzeHCfYAJsFIdlTqqPkkz");
    double zzYRJYRSZlBNX = 281264.0012797478;
    double tUgeekxhEvUPTH = 933356.4256700577;
    bool KKFhvZVbQc = false;
    int FwfyKBWeLS = 1216446960;
    bool wsoIqhq = false;

    for (int VwhonhTGdyYd = 94740036; VwhonhTGdyYd > 0; VwhonhTGdyYd--) {
        wsoIqhq = ! wsoIqhq;
        wsoIqhq = wsoIqhq;
        xNuawai += xNuawai;
    }

    for (int DdGnyGOCrF = 2120731587; DdGnyGOCrF > 0; DdGnyGOCrF--) {
        ZcaOho = zzYRJYRSZlBNX;
        KKFhvZVbQc = ! wsoIqhq;
        ZcaOho *= ZcaOho;
    }
}

string cNkpdpmZ::UVVsisIBinVptK(int jqasIETynsQN, double ydTwlo, string CXfyhPzyUQi, int DxAFDn, double VGqORkzYEqCBp)
{
    int jGnBhR = -474745919;

    if (DxAFDn == -633941883) {
        for (int uCddh = 1621326052; uCddh > 0; uCddh--) {
            jGnBhR /= DxAFDn;
            jqasIETynsQN /= jqasIETynsQN;
            ydTwlo *= VGqORkzYEqCBp;
            DxAFDn = jGnBhR;
        }
    }

    for (int uvYdTKI = 338167250; uvYdTKI > 0; uvYdTKI--) {
        continue;
    }

    return CXfyhPzyUQi;
}

double cNkpdpmZ::XXKcSFFuMkNrLW(string DAKcWWEnNVIlARl, int nVTdNATCk, bool qiIfTdPHRktQuDQo)
{
    bool nZpSahwriaWJW = true;
    int EZrEU = -352260615;
    string TLIKQQdLZgzgYW = string("gnkhfVIDTeD");
    string KKnILjWLnOVdjwT = string("CntQXVwSmkvYzZLsQCLpsCqWlcFqsELXwJhPDXJPpiYayfIUbFzuvQSAGckQHRIKJNpekYwWymfOwIwljFuGECsspNDHyOlVyAJhOVmbke");
    string amYwAPYsXGHowuEV = string("xkMfKCDYKTGimqqxExXQfMdKDlMFPxpNnxoTpURXmvNFIyzxrkNZAYJsOpuMTVSLuJVpAvvuGROmZnPrcyOLUqHMRzzKgjIhTgLXnWnMfbZwHfvoZOMoHkqKRglMKGgkekJMSWakoAiUYvLOHGLLnSvDoaHUCPciUEVmTmGsumcjKZPXxpxoYqmeJVt");
    double BPMpSqyNdHItv = 194989.2223356185;
    int mRDtVLAXcupjLot = -719545372;
    double UcgThScwHCBVc = -276829.1631424528;
    bool yINHMQ = true;
    string zPgPw = string("YGVVskxCIOEWuAoTDrPwWumbcHdZKPgpFaDUBNXgKzoSTQVmqjBOoTfKZbMaOXnMzcgehMfkIrACdFPowPJzwrxzanwSO");

    for (int XTFiznzs = 1170860217; XTFiznzs > 0; XTFiznzs--) {
        zPgPw += zPgPw;
    }

    for (int MmiWYgPy = 734929195; MmiWYgPy > 0; MmiWYgPy--) {
        continue;
    }

    for (int hSCoTtfZTAma = 834175582; hSCoTtfZTAma > 0; hSCoTtfZTAma--) {
        zPgPw = DAKcWWEnNVIlARl;
        KKnILjWLnOVdjwT += KKnILjWLnOVdjwT;
        zPgPw += DAKcWWEnNVIlARl;
    }

    return UcgThScwHCBVc;
}

double cNkpdpmZ::oSGeeoWTJ()
{
    int GqEsMN = -1535607242;
    int mXuGjhGtBvad = -2070987593;
    double dhBpppncygYOHX = -914789.6317984288;
    double zGFTzzxKGmGdTwAf = -364056.38860284415;
    int InrgHJvq = 1543092175;

    for (int MRFhGlfPGXWlt = 1995822198; MRFhGlfPGXWlt > 0; MRFhGlfPGXWlt--) {
        dhBpppncygYOHX /= zGFTzzxKGmGdTwAf;
        mXuGjhGtBvad /= GqEsMN;
        dhBpppncygYOHX *= zGFTzzxKGmGdTwAf;
        GqEsMN /= GqEsMN;
    }

    return zGFTzzxKGmGdTwAf;
}

int cNkpdpmZ::VXcsxaGEpTOfrmt(double TlDHKZ)
{
    int VeqxyPBytn = -1332897628;
    double wjMVSWpi = -168768.99803068058;
    string xTuRWtsalB = string("zDJElEsympDWXSkyzxJpfRWyKbUkkFhuOBDmXpueafdoXhtChaOQaPLHWeiqpTUJkeRZsuyKkVezrZibrCmpjlHcjxEVmpxmCJzGuwVgMxYwqESrjTFjPjReKqANWfEkBdgZBkGLSDuYpWwbqTiGAlVpwrWdCBvBx");
    double tUJNQYQOfZEq = 830076.1874387953;
    int mXYOjDjOSXWneZW = -1825021815;

    for (int cqxcdQyDeJQ = 318942021; cqxcdQyDeJQ > 0; cqxcdQyDeJQ--) {
        tUJNQYQOfZEq *= wjMVSWpi;
        xTuRWtsalB += xTuRWtsalB;
    }

    if (TlDHKZ != -168768.99803068058) {
        for (int mappcLOgPsH = 1105848251; mappcLOgPsH > 0; mappcLOgPsH--) {
            mXYOjDjOSXWneZW /= mXYOjDjOSXWneZW;
            TlDHKZ /= tUJNQYQOfZEq;
            xTuRWtsalB += xTuRWtsalB;
        }
    }

    for (int JdrejVQRfKfMp = 2104883053; JdrejVQRfKfMp > 0; JdrejVQRfKfMp--) {
        VeqxyPBytn /= mXYOjDjOSXWneZW;
        mXYOjDjOSXWneZW += mXYOjDjOSXWneZW;
        wjMVSWpi -= TlDHKZ;
        xTuRWtsalB = xTuRWtsalB;
    }

    for (int AvkfH = 668745627; AvkfH > 0; AvkfH--) {
        tUJNQYQOfZEq = tUJNQYQOfZEq;
        tUJNQYQOfZEq /= tUJNQYQOfZEq;
        wjMVSWpi += TlDHKZ;
        TlDHKZ = wjMVSWpi;
    }

    return mXYOjDjOSXWneZW;
}

double cNkpdpmZ::vzREUwvOsNtAIb(double JZSvquXeCUPhrhz, int kqtIVbji, bool lhowk)
{
    int bRkUVGOP = 2039121430;
    string jRTbSoJSiYQrKaUL = string("ELykreSMfTywOBXxkKAwhnlKNUTmHByVSecAVTzwdSlNkQtvvFrvRxWKBtnBHygtzyhZkNnzLyidkwPxwFouZwUIVIuRIgpoLpZDKHiENflopKwRRfayMkkJFgGwTvspGNRdANjcOAukKqrLtPgDWAuiwkxvhxkECykIcHUglTjiqTaSFwfTifYWPoNuPExrfuCQdqpEUaIUphAJRKNPapyJOJnZWsvQfPzunH");
    double TntkQEwdt = -749270.4023956603;
    bool ukGrL = false;
    string XkQWGolXoHit = string("tbXSkZXnCGnxcpQRhjRqAtRsGUCUQLIHEyPGBvNCSSrffR");
    string efIExxDrajjvEM = string("NmRAmOvFkazegHpgMTltOsWVXBYFmLHGbVgmptxrgufVKtoyznVfMHxAcFzlNQoHuduZzyfmkMpaYqeBgBbKlLarKeJPhUSdtpXCgDGZEILqOglMqZljYjXGmBszDlrpfroXmSXtFaLsqWTMsDMgBPWDAuCSjaYBVWoxYhgVhyoxugozcBibvaiYUZfiHJUNUvKyPVSyWBlwSMHRTCVgPWXZclDhsKDlrjcbekuQfHuQwzbnE");
    bool EkpBwtNAIZXihWsX = true;

    return TntkQEwdt;
}

cNkpdpmZ::cNkpdpmZ()
{
    this->iahXirODNMZ(-142575.14188728534);
    this->dfCZXKwvtAtsH(1367695092, false);
    this->nUTTYwojnJ(string("QnMcKdRfmrxFfEDyNVlTYZgCUoJdIIsTXUTlmijQTeThGBOoxXpsAhBoKmEMhMyzwIDPdSYUUhqfITDGWYAHDtBihLTGaMvdhfyQUcxqlKJSDhZuUDxmQtplvjenhDdojhnSohmiJBPsaVxpmedtjQsqrrmgjKYlnKaiLrClZVViPbtcGuPTEJOVzUtBpiKOgtvZuoqxYlYIPCtP"), true, string("IOOLwykGUL"), 63420.00766018368, -2108248455);
    this->OtqRuwDGKzjPYfML(54254.97877035632, string("QbGQjTkspttPIIAarDfslyldnePxQOPHVGBkHTlcQupyTIcUNfaQavMREDUZedgxoBenGavntwTjFRORsxkUyfJZkEGB"));
    this->XUsKvAFCBLnTGw(1676920422, string("IsqWslgbNayXtKivFUAstQYPOrggWhGgVObyaSHCNBgumIcBYKANUHVDOIvxcnYEuVKdeHSBHjQzHQNtXxHpilMvsDHNmkXgxLASgSaodiroZYrkLUGhpabJtFFfcDJYdeSpSEAOUzIiMzzhMugSCYpniBIYJRyxnuLcoeKvpQAalLmZGemyMwLdQTbOmmFqekjwToLslWdYOvbkDZMntWPNbRfsqz"));
    this->DLKkyrQ(string("CcwIsGnhgVrQrsNQcvCgKaElWUiCVxAaIhhKzEZRLaNpGQhsTxbfkuFHeLNinCkPLvRcjcaeBRbAJlwcJzdEEDEGFwqUyjEoiXY"), string("YqHKyejrScAdCcoBIjqCVYmvNjhEvfBwKgcMEsyDynmSatjBLIGDGzGdGZwPYZytjYhcxwEKVxqndDFMwFBFYViLTEv"), true);
    this->EpXibSqftSlHA(false, 955446.702647651, 896040.7639780696);
    this->UVVsisIBinVptK(-633941883, -745782.7193524601, string("mAByzAznzJLXYkIrEirqUKxzccEnLydQwkHUyGfQvNnW"), 158797501, 466203.0632017061);
    this->XXKcSFFuMkNrLW(string("MwaDXRLMRvSNfKzdHoWaLUIrvRQKmEquhuDyEdEsXdNheBWZUPeUkXUgQDfBHqenAhdukMZUpsMYdKFoUxpjiNUNjCZUBaJJtOPkfRuTGBKJAoPtLzSeStAjdrUwkcqKilEBUh"), 685376238, true);
    this->oSGeeoWTJ();
    this->VXcsxaGEpTOfrmt(-983381.4475008537);
    this->vzREUwvOsNtAIb(30615.798600753482, 67783260, false);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class frzxIYcjA
{
public:
    double xTGtFLAIBWFV;
    int fyDvGJxJy;
    string RLVKP;
    string LNPwdT;
    bool FEuAhB;

    frzxIYcjA();
protected:
    bool VQXEdwHSgbi;
    int JwpvnroHeYTNiziu;
    double UwXPmK;
    string rMXqf;
    bool GyCipdUrfFsL;

    double yELmiyMaIrXMVeW(string AuIKslocCUVFwM, double xdMDYRX, bool qSllM, bool OPHyBnZYRRqew, int UNKLc);
    bool xDUojQVzaFSO(string rKlUhoiPRtp, double VNlyIvMUWSkw);
    void SOlLFChQmruXGV(double cCuovsFecEMBq, string lLiRUQieszA, int ZXNEgSlwNQUJDW, bool LTsoczZSmkWPjjc);
    double CazmNDFYjTiahF(double TmVqjObpW, string WBoEpneNRwVD, int ZGOVKHbZTLttRCU, bool IuXWyx);
    bool nabuZdwDqL(bool xbhtqMYHQT);
    int BIilBeRCnNkcFz(double uKwkexRJ, string EPNfp, bool WgKvGfYdQ, bool NSiiYeTx);
    int cPjfcromuyzUDF();
private:
    bool BuDGxksGgG;
    string DfPoHEKXmw;
    string mOcPINvqXf;

};

double frzxIYcjA::yELmiyMaIrXMVeW(string AuIKslocCUVFwM, double xdMDYRX, bool qSllM, bool OPHyBnZYRRqew, int UNKLc)
{
    bool XjcpPEt = false;
    double ElAfTDkz = 509377.4976777679;
    string DunwqzGUW = string("FQPpwuvWdJNbuKBbQAZVmCXVHUErCpBKaskbKDTMGqFdGBXWYZpKZcAkSNTPELCSTJLzgFaDJGRELtMftyIQyFuKBtwUBXmYktjfqJrwsmRNUsxIyVObOaWubCtnBVSaaMRJVQyiRbrhzyHXCMXgCfREmhHRELvJVGofIQbdEpfCwPufjVXedrAatMubCGiYeHZRYS");
    int kBiLJDKlkLq = 2145806453;

    for (int iUPPnIOvjyP = 1303675375; iUPPnIOvjyP > 0; iUPPnIOvjyP--) {
        XjcpPEt = XjcpPEt;
        OPHyBnZYRRqew = qSllM;
    }

    return ElAfTDkz;
}

bool frzxIYcjA::xDUojQVzaFSO(string rKlUhoiPRtp, double VNlyIvMUWSkw)
{
    bool SWLUxdjlB = false;

    for (int blxoEyEtksworwVB = 1209228205; blxoEyEtksworwVB > 0; blxoEyEtksworwVB--) {
        SWLUxdjlB = ! SWLUxdjlB;
        SWLUxdjlB = SWLUxdjlB;
        VNlyIvMUWSkw -= VNlyIvMUWSkw;
    }

    if (rKlUhoiPRtp == string("CLPLRTevOZuSqounCfNQnuseuooVoReJkdnROutrLXmWwoiNcRYaOiurZo")) {
        for (int OXxfvkqaoETDfe = 574884811; OXxfvkqaoETDfe > 0; OXxfvkqaoETDfe--) {
            rKlUhoiPRtp += rKlUhoiPRtp;
        }
    }

    return SWLUxdjlB;
}

void frzxIYcjA::SOlLFChQmruXGV(double cCuovsFecEMBq, string lLiRUQieszA, int ZXNEgSlwNQUJDW, bool LTsoczZSmkWPjjc)
{
    string xVTtrjM = string("HUpzmZAYmLnUIHdrhQemUkUPAiRCmEkdQCjdLykAFkvlfqapvBuzVBOynnmYhAvxFfQrVAUGJojtWpYnIlSfAxfNyAhAqbOoerhIPuuKqyygElLTrUXqWHIEDOYsKuRixgyssmCjoggVVCmxXCirqvAtHnZThTTcTaOcgfSsascvvwFNc");
    int ZtQGhBzMoShfGsH = 1768379876;
    int uhvYgkNutv = -348583751;
    string ueuMDQNoM = string("szWcJnHMTKumTCqWFYAkwivgrYOBoVFHwoYrvvHUvMizOigaJOuTRLavpFYkXooIVilhOhWlsuWmFbtiHEhcRQNmvLHtQcqgpGWNwDzTDtiSDmThuqvKzLQNMxWpIHQpdnNpvRONNGwGSuGModzYcTLLWpHPcUddwqOfpmtxPNsMGXsNVncYBRUJigspIigaBEMYKpmjuwknWxKZIvMqJjGAuqTAcqbIInp");
    bool TSFhJCx = true;
    string yixhNsJ = string("VV");
    double oxVoDgqnYocrCWG = 519002.1110895557;
    double cbhvqV = 413765.71253596887;

    for (int wgABjFK = 923064482; wgABjFK > 0; wgABjFK--) {
        ZXNEgSlwNQUJDW = uhvYgkNutv;
        xVTtrjM += ueuMDQNoM;
    }

    for (int GxlIKfPiIFyTMZ = 622600276; GxlIKfPiIFyTMZ > 0; GxlIKfPiIFyTMZ--) {
        ZtQGhBzMoShfGsH = ZXNEgSlwNQUJDW;
        yixhNsJ += xVTtrjM;
        ueuMDQNoM += xVTtrjM;
        LTsoczZSmkWPjjc = ! LTsoczZSmkWPjjc;
    }

    if (ZtQGhBzMoShfGsH != -729293720) {
        for (int JDbQj = 1312349062; JDbQj > 0; JDbQj--) {
            oxVoDgqnYocrCWG /= cCuovsFecEMBq;
            lLiRUQieszA = yixhNsJ;
            cbhvqV += oxVoDgqnYocrCWG;
        }
    }
}

double frzxIYcjA::CazmNDFYjTiahF(double TmVqjObpW, string WBoEpneNRwVD, int ZGOVKHbZTLttRCU, bool IuXWyx)
{
    bool JppvsDzrzI = true;
    double ARMXVNTjRJupvv = -336047.0090878689;
    string ZtoEHzrnONMhq = string("lBxkVcRqeXXIfhtMUqr");
    string XxAbddAxKO = string("zuxtGAdilQmyLiDANvoDbUVnCHtfCxGhFyuoVNTkcIfopKkJnnQIcMguLcDqqZFKiHKfLwbjqMTsCmjbzPIZTkvAFEzvWmtGnVioHVeCWZMvmmUpagkixDKKhPUqXpJAYLImDsIIubQiSIDUEtBxEbOnvAiNnNGkjwUsnaGhbXMOxCRdtjMljx");
    double wOMaCbMWq = 265621.9659880102;
    bool eLTcViIiutAJD = true;
    double PqJFKzhgzBktx = 955519.474336566;
    int tuvqmfkSbqBRCs = -185148819;
    int FcSuiJ = -2100949531;
    string SjhSWlqeOPJ = string("kJwyCIaNNpTvfjarlfyYyrPseZBKbCNHASAlJqkgqIonQHaMGPcPs");

    for (int ALkPdTsqskJakUj = 559775814; ALkPdTsqskJakUj > 0; ALkPdTsqskJakUj--) {
        JppvsDzrzI = eLTcViIiutAJD;
        WBoEpneNRwVD += ZtoEHzrnONMhq;
    }

    for (int TYRqgLabKFZRKY = 1661857684; TYRqgLabKFZRKY > 0; TYRqgLabKFZRKY--) {
        continue;
    }

    return PqJFKzhgzBktx;
}

bool frzxIYcjA::nabuZdwDqL(bool xbhtqMYHQT)
{
    double USrmyblenudx = 1030394.5293660245;
    double ijKmemW = -611863.2240957987;
    bool tyogQsOSQZ = false;
    int OyGxTAVpcWXWzK = 1195933204;

    for (int AKVtFmWehGv = 1543359836; AKVtFmWehGv > 0; AKVtFmWehGv--) {
        tyogQsOSQZ = ! tyogQsOSQZ;
        OyGxTAVpcWXWzK /= OyGxTAVpcWXWzK;
        xbhtqMYHQT = ! tyogQsOSQZ;
    }

    if (OyGxTAVpcWXWzK == 1195933204) {
        for (int zOUkDXL = 58999861; zOUkDXL > 0; zOUkDXL--) {
            xbhtqMYHQT = ! tyogQsOSQZ;
            ijKmemW *= ijKmemW;
            USrmyblenudx -= ijKmemW;
        }
    }

    for (int FxgoLWNYFbVFpHj = 1557375389; FxgoLWNYFbVFpHj > 0; FxgoLWNYFbVFpHj--) {
        ijKmemW /= ijKmemW;
    }

    return tyogQsOSQZ;
}

int frzxIYcjA::BIilBeRCnNkcFz(double uKwkexRJ, string EPNfp, bool WgKvGfYdQ, bool NSiiYeTx)
{
    double yxJPyirv = 543467.4052750975;
    bool ekOVYQvNcAOoCSE = true;
    double GCVjE = 50215.131913157056;

    for (int bkeaJmHZdlxnXf = 315214712; bkeaJmHZdlxnXf > 0; bkeaJmHZdlxnXf--) {
        GCVjE -= GCVjE;
        uKwkexRJ -= GCVjE;
    }

    for (int FSnNEEBipZLsCJZ = 681336238; FSnNEEBipZLsCJZ > 0; FSnNEEBipZLsCJZ--) {
        yxJPyirv *= uKwkexRJ;
        ekOVYQvNcAOoCSE = ! WgKvGfYdQ;
    }

    for (int idpaxn = 1248599812; idpaxn > 0; idpaxn--) {
        uKwkexRJ = GCVjE;
    }

    for (int DPZhw = 155918473; DPZhw > 0; DPZhw--) {
        NSiiYeTx = WgKvGfYdQ;
    }

    for (int VGrEsZXssNYrumpB = 927463778; VGrEsZXssNYrumpB > 0; VGrEsZXssNYrumpB--) {
        continue;
    }

    return -663398326;
}

int frzxIYcjA::cPjfcromuyzUDF()
{
    bool hnAoYwERwgx = false;
    double rNSetPXYS = -219339.8884974732;
    bool zHzHtIFkIbB = true;
    double NklvxnsAW = 810257.0542235246;
    int lZiIjLgx = -441679777;
    string yQDMFzJRWmV = string("VICGhxynaeJlVkHUCyROSulpBEkCxnJUbGQeALfqiGnDjjKsJvuBClzyTtzNatnJptiplZL");
    string ssVIcf = string("eHLVVYreaQyDGnySWgfqSCbvfPvzDyKMZwATPfMaRslGspulKiAvgOIiPEHZhAwDkwpEWDswHiDMWkeLnvAGcDGaueaYgBHdTHPEnWdAOBqgnoQmiBQnuAJPVBPtnLgUESFCpCitbYrSOpIGYxkKXqXlpexNuxglTJhoRniGFoTtSosFHSAsvPoVvHCwZraWZYOYwVmsEuFvTUKeEqrKFayuwPoTacZGiUHvhwFvdmqBGSdGCAPqI");

    for (int HrCqPUgysyqzrBC = 1636978625; HrCqPUgysyqzrBC > 0; HrCqPUgysyqzrBC--) {
        zHzHtIFkIbB = zHzHtIFkIbB;
        hnAoYwERwgx = zHzHtIFkIbB;
    }

    for (int JXxAMnegP = 1326279558; JXxAMnegP > 0; JXxAMnegP--) {
        NklvxnsAW /= NklvxnsAW;
        rNSetPXYS *= rNSetPXYS;
    }

    return lZiIjLgx;
}

frzxIYcjA::frzxIYcjA()
{
    this->yELmiyMaIrXMVeW(string("MHUeTxlt"), 828427.8280400953, true, true, 1473558982);
    this->xDUojQVzaFSO(string("CLPLRTevOZuSqounCfNQnuseuooVoReJkdnROutrLXmWwoiNcRYaOiurZo"), -193088.39327394735);
    this->SOlLFChQmruXGV(993031.5871915472, string("yoVlNlkdMdROQTBIoELMmOOlPxYoBpSgKpmmasarViQkbMkDfEBJAZWhEfpmQJLrHKElQAUJyUYrHlOEwOdNJMVMHiPsru"), -729293720, true);
    this->CazmNDFYjTiahF(-145391.61547968886, string("xQvxZFWQEZjdRJiTBZjSVcpBLFQApJoyRAynVGgnaKKbbIECeCxnYakQiqiLkDbQJnnSHwSlCYIdHcllZYmoYnEmzHvKMQDexmQbPSwSHFwmIKiKUYESVvyJiZWKMJKwvzsIfFzriOantffFhYBeHPJeOYuVGNsbQRfDfQbPxfgYzhxNhHTYjLhujmxPtVlPXfpKzMJGxLsEMt"), 122469060, false);
    this->nabuZdwDqL(true);
    this->BIilBeRCnNkcFz(95049.33799798493, string("sabJpUPDxQrcZhZIeoDhHOYpzATsDAjzQxmdtruNbNFSjGOBfzDpEsGImlqYgLJEfUKbReivAfsOtnkktVKPFKRRaMnZgJPIkUBEnIQxFpDbFWLGSMtoOxocwcLPPMBvkBKJUKTrWZuECpCsgFAtUnSqgdRgcOQZhGzPaKEaucWEkuXdonunDxyFfzBcKLKPJllmrlfaePcYzGyfOMuNjsCw"), false, false);
    this->cPjfcromuyzUDF();
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class rMluL
{
public:
    int YngtKDuljZTZpy;
    int iacptDHlal;
    double NSOYzKVwrlHqW;

    rMluL();
    bool syGwkyGslHVP();
    void iqiMgjeGIeRzH(bool QXGDtmGYPDo);
    double bBLMOKjHVkSIl(string tvRWSVomznVQLleE, string FRTUIfTrdhnndub, bool OqrlvwubbnlKXr, bool KGoXVjFIN, int VuCWfxuDLNwLealS);
    string lWjLSJAKAdD(bool qdysqtrhBZTdu);
    int erfifVeuOIftBqzD(double gefdiY, double rpMNMO);
    string CyxTVaBp();
    bool XwicZuAYiAIDsmog(bool srYhVQU, double MCZJoKMJscs, bool PShFey, bool WJOEeWMdCbRlcT, bool AgSsDLNR);
    string LbSazMY(bool lrtoALawyNZLs, string UVaZc);
protected:
    double WpLACvLGqBpHh;
    double dNGUbMx;
    double ycHLElH;
    int gIlGdxOkJUJzoa;
    double lniRVzJJzKMwqc;

    bool ajJJfHkUIw(bool OHsMQtCgemHZIO, double dsWojxpnTLqIzCr, int DdfnbwUUyVvtuIT, int zoJiAtTm, string yxPpVpEfmAHHk);
    bool vtJhIIQHAfQDvrwE(string ZIledZtMJTqgLTsC, bool skyQPJZVwwodz, double vyiGNSeqkkJgsU, bool yxpBzwYTwhG);
    int IJaXcTUcvP(bool PxefSpOghQB, int ZrgHcTVyqIDddFf, bool ntwwU, double TqqwffXa);
    bool gDFzkbBxbzWF(string wtOKkCTrlz, double yBBdmQET);
    void mKrpl(double wbCflDnhCdrogzOh, double PqUmcRtUdYqra, double ENrNsMKXCSCewsj, int KPcMAsfwMrPAqrfj, int oHmwQAUTOZkT);
    void tnAzWDSrKC(bool wYZkjoZjP, int WaeUIAKvCdl);
    bool OqOGtOzcJbazOpX();
private:
    string qLPEPUIDRcFRXq;
    string ctWugakT;
    bool bWkodVtA;
    bool ZuYdL;
    int sGSbCYrzrj;

    bool rdGjLqcgBeIuT(double KUCvPxnMrcPajPvy, string PHDbUQwOs, string DXniiENb, int KGmpxYtmDBBcUSuS);
};

bool rMluL::syGwkyGslHVP()
{
    double vUxfCwn = -166912.04399786037;
    bool QfnlouYGI = false;

    for (int wNmCweBqAMl = 787215833; wNmCweBqAMl > 0; wNmCweBqAMl--) {
        QfnlouYGI = QfnlouYGI;
        QfnlouYGI = ! QfnlouYGI;
        QfnlouYGI = QfnlouYGI;
        vUxfCwn *= vUxfCwn;
        vUxfCwn *= vUxfCwn;
        vUxfCwn += vUxfCwn;
    }

    if (QfnlouYGI == false) {
        for (int ZGnVZdKGvCXq = 2331480; ZGnVZdKGvCXq > 0; ZGnVZdKGvCXq--) {
            vUxfCwn /= vUxfCwn;
            vUxfCwn *= vUxfCwn;
        }
    }

    for (int JCRrnLPe = 381339379; JCRrnLPe > 0; JCRrnLPe--) {
        vUxfCwn -= vUxfCwn;
        QfnlouYGI = ! QfnlouYGI;
        vUxfCwn -= vUxfCwn;
        vUxfCwn -= vUxfCwn;
        QfnlouYGI = ! QfnlouYGI;
        vUxfCwn *= vUxfCwn;
    }

    if (QfnlouYGI == false) {
        for (int RgFWbGzfgEXSV = 985522255; RgFWbGzfgEXSV > 0; RgFWbGzfgEXSV--) {
            QfnlouYGI = QfnlouYGI;
            vUxfCwn = vUxfCwn;
            vUxfCwn *= vUxfCwn;
            vUxfCwn += vUxfCwn;
        }
    }

    for (int jQMIlNVASRQfT = 343808692; jQMIlNVASRQfT > 0; jQMIlNVASRQfT--) {
        vUxfCwn += vUxfCwn;
        QfnlouYGI = ! QfnlouYGI;
        QfnlouYGI = QfnlouYGI;
        vUxfCwn += vUxfCwn;
        vUxfCwn -= vUxfCwn;
        QfnlouYGI = ! QfnlouYGI;
        vUxfCwn += vUxfCwn;
    }

    return QfnlouYGI;
}

void rMluL::iqiMgjeGIeRzH(bool QXGDtmGYPDo)
{
    string ojFOBrUgMsJ = string("lkMvsNspYinUrvUXhRGQeNQGJWFZJMwszTayZ");
    int KzKJDdNIdZoLk = 1430749022;
    int dAMvNrzpfEPwG = -1777638976;
    bool GYyZPMYfFhvTbcEj = false;
    double hRebSHtnAV = 631378.483665642;

    for (int DKQMaxzNWbUfU = 517786608; DKQMaxzNWbUfU > 0; DKQMaxzNWbUfU--) {
        KzKJDdNIdZoLk /= dAMvNrzpfEPwG;
        KzKJDdNIdZoLk -= dAMvNrzpfEPwG;
    }

    for (int oQTAxXixtBjx = 434885770; oQTAxXixtBjx > 0; oQTAxXixtBjx--) {
        GYyZPMYfFhvTbcEj = GYyZPMYfFhvTbcEj;
        dAMvNrzpfEPwG += KzKJDdNIdZoLk;
    }
}

double rMluL::bBLMOKjHVkSIl(string tvRWSVomznVQLleE, string FRTUIfTrdhnndub, bool OqrlvwubbnlKXr, bool KGoXVjFIN, int VuCWfxuDLNwLealS)
{
    string KzPZwI = string("XnqAsoWpPQyPsRyNAEcVWPXZzLMHtDCPbvEJLGjOMrOUTyAnPerMQrqaiczvYLvRsxPmwpCpigsfHCXFuXiWeixfUypvtWSFNQCiinvixZAwVYafLwtiYlpvLEpMwcTHiIJxDADATaaadBZIOHuWFqKahUJVbZGNIWXVgXAfXHTcEoZQxfRHSEheuXDZsCUEOexjrWeHpbtYsLyUWsqYkNkbNPEChjgpkutPhhB");
    bool KqwXpXZfQ = false;
    double LcZEHANvFdo = -846981.7445969454;
    double NgzstdvlwLBgcaS = 255780.28275013642;
    double ZxBgjhC = -362101.54518673563;
    double nwUEwrr = 949838.1024120606;
    int CrhiaLYnryZGD = 1813330772;
    string bQIaA = string("FXBVRrcNagXlMzOywqFlBFrXscpmJUroiKMiJJyJQAJynmvEUNtPFnpDSMWWYcWZlSLYFg");

    if (VuCWfxuDLNwLealS <= 1813330772) {
        for (int hCuhpD = 80408838; hCuhpD > 0; hCuhpD--) {
            KqwXpXZfQ = KGoXVjFIN;
            KzPZwI += KzPZwI;
            tvRWSVomznVQLleE = KzPZwI;
            KzPZwI = KzPZwI;
            KzPZwI = KzPZwI;
        }
    }

    for (int mDOufgcFov = 1756163066; mDOufgcFov > 0; mDOufgcFov--) {
        VuCWfxuDLNwLealS /= VuCWfxuDLNwLealS;
    }

    if (tvRWSVomznVQLleE == string("FXBVRrcNagXlMzOywqFlBFrXscpmJUroiKMiJJyJQAJynmvEUNtPFnpDSMWWYcWZlSLYFg")) {
        for (int JaLfe = 495545127; JaLfe > 0; JaLfe--) {
            FRTUIfTrdhnndub += bQIaA;
        }
    }

    for (int sblPkzjFcQ = 616150647; sblPkzjFcQ > 0; sblPkzjFcQ--) {
        bQIaA += bQIaA;
        NgzstdvlwLBgcaS -= nwUEwrr;
    }

    return nwUEwrr;
}

string rMluL::lWjLSJAKAdD(bool qdysqtrhBZTdu)
{
    string ecnWNtLtGIK = string("QxMSTIvtnpqFKwvHLjDVQObKgtfDIjxfnUUjVTspwyhKSIPwQbcz");
    string ekVxidaVdxWldX = string("AAwqyvjyHXmZJvqPxdKlSAzxFRh");
    double VNuBki = 613084.0623906344;
    int hmQpErHaoi = 631689885;
    string ZmnMLsFhPMXyVZo = string("BiClTyxTWazuwjyWbOcbiOirtbjxlOKIzKeXWFJxfTFVTmGIygTLbpySNtseXBsvofoimFkkUmmgbaYlcgdKYVqZzAx");

    for (int MsZJWFwfs = 109348134; MsZJWFwfs > 0; MsZJWFwfs--) {
        ZmnMLsFhPMXyVZo += ecnWNtLtGIK;
    }

    for (int YqqfLsyugu = 1878973441; YqqfLsyugu > 0; YqqfLsyugu--) {
        hmQpErHaoi /= hmQpErHaoi;
    }

    for (int rujtJRFiLIJIW = 792666155; rujtJRFiLIJIW > 0; rujtJRFiLIJIW--) {
        continue;
    }

    for (int pvPUZ = 904293364; pvPUZ > 0; pvPUZ--) {
        ekVxidaVdxWldX = ZmnMLsFhPMXyVZo;
    }

    for (int cuClcygteCDDJ = 121282657; cuClcygteCDDJ > 0; cuClcygteCDDJ--) {
        ekVxidaVdxWldX = ekVxidaVdxWldX;
    }

    for (int SgoJfVgtq = 1680355313; SgoJfVgtq > 0; SgoJfVgtq--) {
        ekVxidaVdxWldX += ZmnMLsFhPMXyVZo;
        ecnWNtLtGIK += ekVxidaVdxWldX;
    }

    return ZmnMLsFhPMXyVZo;
}

int rMluL::erfifVeuOIftBqzD(double gefdiY, double rpMNMO)
{
    int DEiZlVcxPUBeEAH = 132917946;
    bool dVhTLVkxJsYK = false;
    double BCwze = 739633.7421283585;
    int QqFUfbLCqg = 1333146070;
    int CcrLXulMYrcR = 1165350653;

    if (BCwze > -690107.881674321) {
        for (int UwGKotLI = 1586970933; UwGKotLI > 0; UwGKotLI--) {
            continue;
        }
    }

    return CcrLXulMYrcR;
}

string rMluL::CyxTVaBp()
{
    string ysqdeaCNArNinhU = string("RtQMOdlcDMbMFDUJcWXoJoMHimSmOYIOczRzHuR");

    if (ysqdeaCNArNinhU >= string("RtQMOdlcDMbMFDUJcWXoJoMHimSmOYIOczRzHuR")) {
        for (int JyqyYdKaw = 433970457; JyqyYdKaw > 0; JyqyYdKaw--) {
            ysqdeaCNArNinhU += ysqdeaCNArNinhU;
            ysqdeaCNArNinhU += ysqdeaCNArNinhU;
            ysqdeaCNArNinhU += ysqdeaCNArNinhU;
            ysqdeaCNArNinhU += ysqdeaCNArNinhU;
            ysqdeaCNArNinhU += ysqdeaCNArNinhU;
            ysqdeaCNArNinhU = ysqdeaCNArNinhU;
        }
    }

    if (ysqdeaCNArNinhU == string("RtQMOdlcDMbMFDUJcWXoJoMHimSmOYIOczRzHuR")) {
        for (int VTkKtqowzYLjwc = 634756716; VTkKtqowzYLjwc > 0; VTkKtqowzYLjwc--) {
            ysqdeaCNArNinhU = ysqdeaCNArNinhU;
        }
    }

    if (ysqdeaCNArNinhU >= string("RtQMOdlcDMbMFDUJcWXoJoMHimSmOYIOczRzHuR")) {
        for (int IYjCPZs = 588430953; IYjCPZs > 0; IYjCPZs--) {
            ysqdeaCNArNinhU = ysqdeaCNArNinhU;
            ysqdeaCNArNinhU = ysqdeaCNArNinhU;
            ysqdeaCNArNinhU += ysqdeaCNArNinhU;
            ysqdeaCNArNinhU = ysqdeaCNArNinhU;
        }
    }

    return ysqdeaCNArNinhU;
}

bool rMluL::XwicZuAYiAIDsmog(bool srYhVQU, double MCZJoKMJscs, bool PShFey, bool WJOEeWMdCbRlcT, bool AgSsDLNR)
{
    bool RIrCAOMUZumEWCW = true;
    string QutJtBOfrDw = string("hSrMFmhdqKjxTqvhQdZnkYunOhLIxrfnKUzwTREPOVhdJetBuWVhDBXvNxtANYTwwvIehKBlXEHFzrJHrKUNHvQZrdgLUHGDDIWFVwgegmvvnKVwFxwXwHvBjZxoG");
    int KvJRevO = -1465822968;
    bool WvlVrpmEQwI = false;
    bool TxgcjkmZXEKeS = true;
    int MeyCTnjpcVdxK = -1756836574;
    int sLJtejX = 361957060;
    double YBQrdnnXj = 944434.6727447496;
    double AaBNPGFihuCjWL = 752345.0770638435;
    bool qSbiOzeyU = true;

    for (int iwzMlpGhugqdP = 2059832461; iwzMlpGhugqdP > 0; iwzMlpGhugqdP--) {
        WJOEeWMdCbRlcT = WvlVrpmEQwI;
        WJOEeWMdCbRlcT = ! srYhVQU;
        srYhVQU = ! RIrCAOMUZumEWCW;
    }

    for (int JlvfvJCtSvktmaV = 1366124344; JlvfvJCtSvktmaV > 0; JlvfvJCtSvktmaV--) {
        PShFey = ! TxgcjkmZXEKeS;
    }

    for (int dtuXkIvHENVm = 430338792; dtuXkIvHENVm > 0; dtuXkIvHENVm--) {
        RIrCAOMUZumEWCW = RIrCAOMUZumEWCW;
    }

    for (int RzlXouJavc = 1077033379; RzlXouJavc > 0; RzlXouJavc--) {
        srYhVQU = TxgcjkmZXEKeS;
        WJOEeWMdCbRlcT = qSbiOzeyU;
        MeyCTnjpcVdxK /= sLJtejX;
        AgSsDLNR = ! srYhVQU;
    }

    return qSbiOzeyU;
}

string rMluL::LbSazMY(bool lrtoALawyNZLs, string UVaZc)
{
    int FrxVOUPjXVrsLS = 893251836;
    double WmRaoixWrpBjN = -861459.0578693621;
    bool thHLeck = true;
    double gGtcpQmtgsa = 973767.6621335432;
    bool SlZEwHvHIrOqH = false;
    double FHmsWt = 1007711.9310955916;
    string sZOEsF = string("nwPAGDmClmoWNwEQgCBFRKazaobXVpeYuHwRclLGXcOOqJNLwfdUiJnpeIHzNEqrqbKtJxkAnAFrggGTCwADdKXhFiaaeOJUeHyXzmGBsqliNQhrgTPJzgRGkWAJbTcOienHrMlajTzFaakiNxFgTrCNDHzlpxYMwdwpjFUKneCPQddSoUsHzyySTQTfcaEmnfwfcczxAdtWfyRZpohNHJWpnvkuP");
    double bsAKqLVowhhS = 1034670.1487076818;
    bool IhVwEmfwrsS = false;
    double kUKTOz = -725065.4556767041;

    for (int PsArxzPz = 2075356861; PsArxzPz > 0; PsArxzPz--) {
        UVaZc += sZOEsF;
        SlZEwHvHIrOqH = SlZEwHvHIrOqH;
        SlZEwHvHIrOqH = ! lrtoALawyNZLs;
        FHmsWt *= FHmsWt;
        SlZEwHvHIrOqH = ! thHLeck;
    }

    for (int ADciIsVEz = 771303624; ADciIsVEz > 0; ADciIsVEz--) {
        continue;
    }

    for (int SxoqDAEZ = 2062249245; SxoqDAEZ > 0; SxoqDAEZ--) {
        gGtcpQmtgsa = kUKTOz;
        kUKTOz = gGtcpQmtgsa;
        SlZEwHvHIrOqH = lrtoALawyNZLs;
        IhVwEmfwrsS = SlZEwHvHIrOqH;
        SlZEwHvHIrOqH = ! thHLeck;
    }

    return sZOEsF;
}

bool rMluL::ajJJfHkUIw(bool OHsMQtCgemHZIO, double dsWojxpnTLqIzCr, int DdfnbwUUyVvtuIT, int zoJiAtTm, string yxPpVpEfmAHHk)
{
    double JHrXUrRpsoVCSt = -592941.2275264967;
    double AXjRKvC = 270929.96168483124;
    double ojgGKMyUbp = -464931.2908731982;
    double WcFOc = -189933.97651771712;
    int mKHZvJLcLsQL = -1924520564;
    string zNQXRrmTZknthXO = string("XIwnfXdvDoTwfSMzOThPgMHAghqfNsTtbZTRdRUNHzxsROFcQwmAwyIvfDzuYTiGzXOzAfGGYMyrGJMMZoKPoNLaUKvdsJAJtePVASpymQCNAYpETHRItSPTpVMCzfICgnZFfYvTfSGVTjSRDrSSinzUThMiFGzbPevwsMoOhtzMVpQUPxPnJuxdgCavesyRSlREgMWyLBqLAWoAu");
    string ZCAaSl = string("XbCDxgVXHAgRVGrlCHPAMqabVYNHiSVFZMtjhjdFGYFKSrXkrWoFseLCUTQvOFvbDdWBVMNFDFHztNcXvUzakVwTlHCrnqhlERWpRGMZUNhvidEEbvTmjV");
    double WvZYew = 546139.3897342496;

    for (int zkMwXERhgDOEZrw = 1233518446; zkMwXERhgDOEZrw > 0; zkMwXERhgDOEZrw--) {
        continue;
    }

    for (int YMNrPv = 1567190455; YMNrPv > 0; YMNrPv--) {
        continue;
    }

    return OHsMQtCgemHZIO;
}

bool rMluL::vtJhIIQHAfQDvrwE(string ZIledZtMJTqgLTsC, bool skyQPJZVwwodz, double vyiGNSeqkkJgsU, bool yxpBzwYTwhG)
{
    string wQaDIkUNkEf = string("HbTCsriBEOVSZmVdiICxpNVsQLhRNoOwQHgFKlflsZCKliPGKhDzmcYPVlAijzjlwzdtCoKMtSAoCoTPaGHBFVVSfJXJaUDeHblezLYsdNyyNvBeYvQEKeZSOdZwBKMkSOsjHVWoSZYhLoRedgkoIyQQgCudmSdduXvxJWdGmoAFjydFhbrnOgqVJkyiKqpXlVNuWqbfmhxfBeoXlYhyTAiDdxNHEC");
    double jLKWs = 354132.0460934514;
    int upKHQxKqa = 524600606;

    for (int pSJEW = 253019343; pSJEW > 0; pSJEW--) {
        vyiGNSeqkkJgsU += vyiGNSeqkkJgsU;
        skyQPJZVwwodz = skyQPJZVwwodz;
        ZIledZtMJTqgLTsC = ZIledZtMJTqgLTsC;
        skyQPJZVwwodz = skyQPJZVwwodz;
        skyQPJZVwwodz = ! yxpBzwYTwhG;
    }

    if (jLKWs != 354132.0460934514) {
        for (int JeixKPrbi = 485574414; JeixKPrbi > 0; JeixKPrbi--) {
            wQaDIkUNkEf = wQaDIkUNkEf;
        }
    }

    for (int IJfaMddFFU = 990089101; IJfaMddFFU > 0; IJfaMddFFU--) {
        continue;
    }

    return yxpBzwYTwhG;
}

int rMluL::IJaXcTUcvP(bool PxefSpOghQB, int ZrgHcTVyqIDddFf, bool ntwwU, double TqqwffXa)
{
    int OigFsPOMgQLq = -1844538908;
    string iKxXNiptDrJtPf = string("dPHsseTkvNcafXAlbHCIngdzqVpuiKLBbbhAFcXRhEcCSQFzgMCTJCipjmtKhabrvrrrzGQToGYpNbrAdcuKWnNCQaivwGNEdSvEMzUEDcWSLqgQrXfLoNfxHUkEgbyRsvkHvoaZIkxLuOPrcpzPPmmwqEFUHLfLkeffJTGMGdEsGoAndieItCFwffRPeqUltnMYHbXLYxZUkVfvttBMUnveEVfUpTzbSzjGr");
    string alkLhANMruWoUH = string("iOoPODyEPFajRoBEzzGFGCnWGLdxkCLuauhvVxBJSHR");
    int CHwVDXWrhX = 1994724026;

    if (OigFsPOMgQLq <= 1994724026) {
        for (int fFfKp = 300858649; fFfKp > 0; fFfKp--) {
            CHwVDXWrhX /= ZrgHcTVyqIDddFf;
            ZrgHcTVyqIDddFf += OigFsPOMgQLq;
        }
    }

    for (int Ksutwohr = 1427207482; Ksutwohr > 0; Ksutwohr--) {
        alkLhANMruWoUH += alkLhANMruWoUH;
        ZrgHcTVyqIDddFf /= OigFsPOMgQLq;
    }

    if (iKxXNiptDrJtPf == string("iOoPODyEPFajRoBEzzGFGCnWGLdxkCLuauhvVxBJSHR")) {
        for (int NsWiXCJFxjRy = 90630775; NsWiXCJFxjRy > 0; NsWiXCJFxjRy--) {
            PxefSpOghQB = ! PxefSpOghQB;
            PxefSpOghQB = PxefSpOghQB;
        }
    }

    return CHwVDXWrhX;
}

bool rMluL::gDFzkbBxbzWF(string wtOKkCTrlz, double yBBdmQET)
{
    bool xFkFQNBx = false;
    int ZvuToliJ = -1942390124;
    string yMIPIrFYbZXVx = string("QyGeyglSpBgUOcSICmeVSygLyYVZgawbolhaBJCPWlYOudsfbYUauMCsBOyyWExZnQqaqzysXXAEmaRxDQdtEqeZebLHznYaCDQJYetvDmNkvVoGJPFFSMNWcGlkiFDTJrULXLTRonNppgAOcdbvSWROAdfDHnvSuMjQVinnZOoJODyIekHZOKDESWNhrhPhLlitIvopXEUZSvfM");
    string LNWQEotu = string("OQfkuVhFOViIFFCWcDXZygDDOdEwJsbezeyRHVGhhTOkMfCqRTpcNvpMbouxFvfbOQaPOYtDOQqiYvFSxzYqItyKLeKbEhlLGlpJVeEruraoucVjfabuOjhConWeYBCpCZIvmAepDiDaUyJ");
    bool HmOCUlZFTuPRakCa = true;
    double mJSaiorWrLHbaDfi = 380721.71696876915;
    bool XPgGkKRLMR = true;
    double dayAWTk = -881510.0212294835;
    double iZeiggdsG = -701626.8983878547;

    return XPgGkKRLMR;
}

void rMluL::mKrpl(double wbCflDnhCdrogzOh, double PqUmcRtUdYqra, double ENrNsMKXCSCewsj, int KPcMAsfwMrPAqrfj, int oHmwQAUTOZkT)
{
    string fttSvwhqOI = string("yDJprEmCKWcjpBISRLAubyASqlaZk");
    int ZOCLZQLZmJO = 305765406;
    int MbMMnopYrxEHmz = 580116406;
    int vtEPriB = 1458315963;
    string LYtnzSaTOZOY = string("UaJkaMcIsUvSwmilbKFNuGnalUjFPRescsOpXqIBHOvZYNWLMnTQUvgEQiriWqaIenLCXYHmYCDtSvbNceInkHBiGKlEZNXWLJuprLhqwOIUAfeeKwUWjKYMpMbhBlzeVqnwxAAkEBedqdXqLrOycMNJxValwrwggsNQMUqDMBRPTcPQMtZZTfykUImwsqUlhtErrIUPYNQVQMUdUHoSkUQ");
    double rrhDKSHxmRh = -653752.1118952867;
    string YLnZGA = string("FZkOvPHwyAKtzpUdFUbelleorDOnFRFcwDHSxWANWzOGEeGjTUkOjltDkWPiICmrWsUgvRnrEWDNIHS");
    double lopzDMHxtToNBhD = -788255.3393805536;
    int sZYVvoSRnDr = -76337592;

    for (int mmWsZRD = 1981600431; mmWsZRD > 0; mmWsZRD--) {
        continue;
    }

    if (lopzDMHxtToNBhD <= -788255.3393805536) {
        for (int BdvwpoqYm = 1696356317; BdvwpoqYm > 0; BdvwpoqYm--) {
            fttSvwhqOI += YLnZGA;
            ENrNsMKXCSCewsj -= lopzDMHxtToNBhD;
        }
    }
}

void rMluL::tnAzWDSrKC(bool wYZkjoZjP, int WaeUIAKvCdl)
{
    int COpZBukgiaD = -1061333237;
    string YLkTgEaYnznClru = string("jGcfOuLrgnUHIwwJcywayJEpwtrSsMIwTKYCzVCTzhQnuaLyBLPdzmiTMAJnNEuQbOMsKfwQYUMVRTPQNmXzcfHewTQftmmfYiwcdQgJunySmucQjpOftHyJzRWrzzYDumtmwUtrLbJXLhlOvIlFsVGePfehETGiZIskkhAFjuNzWhLuhhbHwhYUdvHnOPHPOyEyALyxyytfpkEijPNVqbVlbgcWKClRsAQot");
    double BHFJOUAgWVDCUd = 719209.3886135574;
    bool afmzQCEKvnrdhzK = true;
    int iTAEMRb = -1470349962;
    bool FSCQSqvNs = false;
    string uRqoJMAAAosQ = string("w");
    bool AHbcFbxfuhG = false;
    bool nPmpcbxbiHwWw = false;
    double pQjfpJrz = 300124.0811116174;

    for (int pZONJ = 1923670783; pZONJ > 0; pZONJ--) {
        iTAEMRb /= WaeUIAKvCdl;
    }

    for (int qmUIQJewsSDi = 878904794; qmUIQJewsSDi > 0; qmUIQJewsSDi--) {
        continue;
    }
}

bool rMluL::OqOGtOzcJbazOpX()
{
    string sChkLzrTZyyNMu = string("rdiWLLfSOMhndGSzuscYBeSHHIiiTcCmvtupddjkekVMPTZiGubnQztKVeamVgGwEQyiEVvHadClzYmxEjHlSGXMOvEScfVndfrUBqXFMkvxsMkuMVHxnHVFJVHRxLfdkfJzkOnxCPQohrIdxcdbSGLYqQ");
    int CyfaojQnMuF = -187989216;
    int niriQXFCZ = 919941550;

    for (int vqYhBRf = 555944532; vqYhBRf > 0; vqYhBRf--) {
        CyfaojQnMuF -= CyfaojQnMuF;
        CyfaojQnMuF -= CyfaojQnMuF;
        niriQXFCZ *= CyfaojQnMuF;
    }

    return true;
}

bool rMluL::rdGjLqcgBeIuT(double KUCvPxnMrcPajPvy, string PHDbUQwOs, string DXniiENb, int KGmpxYtmDBBcUSuS)
{
    bool xNKCojIUrNyoT = false;
    string idjBwMxFYU = string("mmkXySMPDTtCKIKUyjjCZRp");
    bool sVWzLli = true;

    if (PHDbUQwOs >= string("JkyJUSZludLEZGoEdDizDanbBcOvIYOgPnYUXQxmgcytKYTIxCNqzsoecWQpdoPlcKUttUSQSojyaoJhcxRTzfXtfuQImAadarRvCzQnZJCppoxqEKdCgEArwhYPCQxZBNzPDcoDjDoXcTKwcfWfJuolsgeteiUwOBUkqZVOqBCkCAyNYwGqhbZNGRYGZKQwRzsqAAMHIIluAGLAyXdXwElACAVFTNvRLRqPTkIBwokhjQtCVupbxvpMkrGZ")) {
        for (int kMWoPZ = 1275078538; kMWoPZ > 0; kMWoPZ--) {
            sVWzLli = ! sVWzLli;
            idjBwMxFYU += PHDbUQwOs;
            KUCvPxnMrcPajPvy = KUCvPxnMrcPajPvy;
        }
    }

    return sVWzLli;
}

rMluL::rMluL()
{
    this->syGwkyGslHVP();
    this->iqiMgjeGIeRzH(true);
    this->bBLMOKjHVkSIl(string("mCQGYzptNtYWRboZvFgITQBpSKgRwefETjrtaNZqwwRLYMlEQC"), string("JJtBlKyJIrufDdgIEMPTbxZaOoBrzopgmyNpGTUaJTgGyBUynwBENvHSUtHZMiEXPBVDRLnxaMdBfDoXyBpZZNNDMnJBucNrEXJJopHBpsiWxSJRFSkkiKmvxzaTLLPykgdUPqnobdRcpjyqKIyoDiLoUKKRtCUcgQURASGUDuDwgkQ"), true, true, -1177279006);
    this->lWjLSJAKAdD(false);
    this->erfifVeuOIftBqzD(-690107.881674321, -53464.93058154932);
    this->CyxTVaBp();
    this->XwicZuAYiAIDsmog(false, 590303.3265051814, false, false, true);
    this->LbSazMY(true, string("QsupdnNauluPTrWsKQSdMuiQPVDQpJOjaadgEzztIearvQcHkFUxjxSJWezigQQPeOWpjdBuHHqtmSJfUwixbFJfgqPDliXedEdSVyLLZVYfQNPQRKnfxIOVmEpsWYqMQEmbeBeETYuQvlvyNLGOIEzkfRlMDCQmDGdbRleSi"));
    this->ajJJfHkUIw(true, 432891.16461895395, -1435807501, -581093473, string("iJvMsetrbUHQVmdTmjqthYQVzxfctAJVNDFYVdygsPKvKMxUAfCxiMOtGvAKlhfbypdsWqBzsIaJhOvXqBsyGzagjavnVdRtKkXbRrbPEDHzlOWBTdrCPULxWcfKApyssBbhBWgKugYuTYEVOLNegpNdITFFXgBTBWRSmjzHvbhoEfpOEAESsniFbDzJSOOrhRFNTyqusIIFVFtZ"));
    this->vtJhIIQHAfQDvrwE(string("uvfSqzXoyoWLuxTgUnuQumhpjnvcwLsvNxvNrmeCGUyBJpoynkWxyfrqPdRBhwAmfcAdUuVDDnusPTdkHotlVhsEAzoFvPpgQRscSEodLSnwmJAdwYgrBI"), true, -913945.0283686777, true);
    this->IJaXcTUcvP(false, -1808644097, false, -5525.105076971484);
    this->gDFzkbBxbzWF(string("YFZjxbMDmvKdkCRLCkYcPYDoScRxrJtqhyLnHnxJKarhDlkmxiEzlJpIHtgeZmNiWxFkVZKWgpYYixTDpyWKFoTrzHqnIxpfmnyUwqGvXWRQQftufXZTKIaBFUFsVLBRbtUrTesbaBPwCRJxrmHmMRPAtHfCnYDtedtSpvA"), -156817.34333053205);
    this->mKrpl(931757.5463058704, 843881.8530206553, 202718.5063962255, 432486366, 372011815);
    this->tnAzWDSrKC(true, -581540166);
    this->OqOGtOzcJbazOpX();
    this->rdGjLqcgBeIuT(-938400.0444307599, string("JkyJUSZludLEZGoEdDizDanbBcOvIYOgPnYUXQxmgcytKYTIxCNqzsoecWQpdoPlcKUttUSQSojyaoJhcxRTzfXtfuQImAadarRvCzQnZJCppoxqEKdCgEArwhYPCQxZBNzPDcoDjDoXcTKwcfWfJuolsgeteiUwOBUkqZVOqBCkCAyNYwGqhbZNGRYGZKQwRzsqAAMHIIluAGLAyXdXwElACAVFTNvRLRqPTkIBwokhjQtCVupbxvpMkrGZ"), string("rpgFpFLbYMCDYwctJNUeOLWbcqCWxAsRQryaSUVDBaxTrWgXqahEPqR"), -119764171);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SILdVx
{
public:
    double zzedjYdB;
    double KwFoROxQSe;
    double ThFbLaUXFdueT;

    SILdVx();
    bool pDSceAvGPRxn(bool eQpwjJrrsz);
    bool yUhQpUAWackHfF(double wbRxvUzVs, int dabaq, bool RmPXYFXH);
protected:
    int AWcfvQZzUA;
    string XxrqHpT;

    string lLAvUZ(string OkPjTkzbwepIB, string oAGlEGoVRyc, string kCdvDhbZrRicH, double zrTftpqdlAuVQpOo);
    string idzEEAeCqBhsUfjh(bool HpjzROOazz);
    int GJhkrG();
    string ReNpCH(string YSOeyfW, double RpqqCdnLivzUg, double aMWqhgEYYkhARg, int IUiQTaKjYdSN);
    double ANaCRDv();
    string hnFABefXQ();
private:
    bool zNcJUtPMv;

    bool iZNOoECILCgw(int rcAPbicaD, string mnGPDyILN);
    void xvwieuoyzdOqzV(double rqkxyWnRhNpFHQHo, int NIpRyr, int GIyBPWpNBtn);
    double lzrtI(int GXZtgZbNLxOyeU, double taMIoHcXNpGtAAV, string loOxy, double aYrHmTsvqQtYnhQ);
    double FezNvuTXZfrR(string gTzBvzmT);
    void CPKtUGfK();
};

bool SILdVx::pDSceAvGPRxn(bool eQpwjJrrsz)
{
    double NQZOhSDdop = 333007.9961205793;
    bool WEiyeAAGeLM = false;
    double VVRoo = -925465.3424306373;
    string LFZrGHRnVDMh = string("gtIPZhLSfpDHkOGPe");
    double EdHebv = 750977.1554396366;
    int rwxZCanYTq = -535111466;
    double mnaEvZM = -69454.26985325405;
    string sRoBnENtFpQq = string("SfEDegZfJLDKkoMLEwDcjjTJIphQPOMRfULfrdwAGrRXwqvlQnFLNrkIJnPCvkriebNTbmbyVKdtxOEIusNBDzrEWCzyJEkkXvXlUocRaUDsBVJVyMcdqiIwiYqSdxqyrdSULzJkNoBodXevAKUcjZHvCpedRDqPhemCkUbLgUQBloxaCCwbiKsAR");
    string QHaQgxvpsljfle = string("eaIXKrAckarFJGCmAymdoTNmvpdCnbDnCGhRajOJPuLUyWXNYIZnMjCbuuAySPKcFOUKSDOqqtQyQinAoHuUKWwCZTXwWVjHyAiHqduRtYtCICYIZMsKqdQdnn");

    for (int QdEMDPcU = 1260318389; QdEMDPcU > 0; QdEMDPcU--) {
        VVRoo *= NQZOhSDdop;
        EdHebv += mnaEvZM;
    }

    if (NQZOhSDdop > -925465.3424306373) {
        for (int XxartaCMLgLxCy = 247742562; XxartaCMLgLxCy > 0; XxartaCMLgLxCy--) {
            LFZrGHRnVDMh = sRoBnENtFpQq;
        }
    }

    return WEiyeAAGeLM;
}

bool SILdVx::yUhQpUAWackHfF(double wbRxvUzVs, int dabaq, bool RmPXYFXH)
{
    string pwuuCTdikSZRoY = string("Qv");
    int VDEcyHXUsAY = 2073144101;
    string nflAEqTNlmn = string("HqVpIvVOzQPFXrGyeYbMOjwmKblGGSIrDdCfaQQxVfbSIGixuLjEpoWaKDiVtXWBrIiRwrtIoQZwqZEoeCYTnuQkhluwisOZaudHsViMaTeRQhgqvLfYMtzJSTiNbOUfIPVWdpCkVgNSrnzhdCtKhg");
    int MgDtzMnsE = -1094139429;
    bool xxmQGLAKwT = true;

    if (nflAEqTNlmn == string("Qv")) {
        for (int ayvwBuMwKk = 491727544; ayvwBuMwKk > 0; ayvwBuMwKk--) {
            nflAEqTNlmn += nflAEqTNlmn;
            pwuuCTdikSZRoY += pwuuCTdikSZRoY;
        }
    }

    if (nflAEqTNlmn != string("Qv")) {
        for (int MXforePeAs = 892595167; MXforePeAs > 0; MXforePeAs--) {
            dabaq -= MgDtzMnsE;
        }
    }

    if (nflAEqTNlmn >= string("HqVpIvVOzQPFXrGyeYbMOjwmKblGGSIrDdCfaQQxVfbSIGixuLjEpoWaKDiVtXWBrIiRwrtIoQZwqZEoeCYTnuQkhluwisOZaudHsViMaTeRQhgqvLfYMtzJSTiNbOUfIPVWdpCkVgNSrnzhdCtKhg")) {
        for (int wOAStDXFgSaeufs = 669661442; wOAStDXFgSaeufs > 0; wOAStDXFgSaeufs--) {
            MgDtzMnsE = MgDtzMnsE;
        }
    }

    for (int lgdoXHsJti = 849607169; lgdoXHsJti > 0; lgdoXHsJti--) {
        MgDtzMnsE /= dabaq;
        VDEcyHXUsAY -= MgDtzMnsE;
    }

    if (pwuuCTdikSZRoY >= string("HqVpIvVOzQPFXrGyeYbMOjwmKblGGSIrDdCfaQQxVfbSIGixuLjEpoWaKDiVtXWBrIiRwrtIoQZwqZEoeCYTnuQkhluwisOZaudHsViMaTeRQhgqvLfYMtzJSTiNbOUfIPVWdpCkVgNSrnzhdCtKhg")) {
        for (int FyMjeNEhYivvDA = 370772659; FyMjeNEhYivvDA > 0; FyMjeNEhYivvDA--) {
            dabaq /= dabaq;
        }
    }

    if (RmPXYFXH == false) {
        for (int eIhYpN = 141161676; eIhYpN > 0; eIhYpN--) {
            VDEcyHXUsAY -= VDEcyHXUsAY;
        }
    }

    return xxmQGLAKwT;
}

string SILdVx::lLAvUZ(string OkPjTkzbwepIB, string oAGlEGoVRyc, string kCdvDhbZrRicH, double zrTftpqdlAuVQpOo)
{
    bool WMkLhdUUieoJm = false;
    double QdUceXFYgZY = -893075.919911346;
    string HKTQjzyegsKEs = string("qiNniuWruydNXGhIeBAekKfyVIGVjZrJkIKlxXneoyAHyAqyuPjyEtpDhGFayRqvwjiozPdzPrAsLIcBLxgVRdLuFnjCAZOHBFhHwKXGUHfgCYqeaIRvUGJSZMiVfHGWHHOGhqNLDyroCeFRgKGlrUDhiTMYt");

    for (int eZjnafvZtaaikyAy = 136053965; eZjnafvZtaaikyAy > 0; eZjnafvZtaaikyAy--) {
        WMkLhdUUieoJm = WMkLhdUUieoJm;
        oAGlEGoVRyc = HKTQjzyegsKEs;
        zrTftpqdlAuVQpOo += zrTftpqdlAuVQpOo;
    }

    return HKTQjzyegsKEs;
}

string SILdVx::idzEEAeCqBhsUfjh(bool HpjzROOazz)
{
    int WtpgrzvCqTnsqbQz = 1165289317;
    int EQXWLFk = -1753816277;
    string wkObd = string("aURmLcjhIRCagGaLpKcRSYmMNLfGDQkoVXOozeJuSazrHykEJOUJlALJjPeGLcfQtFMyxZLJeAlCNeaBVTClZZSOESpkQUeNhsjkALwUDZeuTYWqAPEbZzDacHnqgAcidCnjuCtraeWuqIbYVYNkb");
    int JfvGkVNeye = 210573406;

    if (WtpgrzvCqTnsqbQz == -1753816277) {
        for (int wKnYLnAvtCTnj = 1432608571; wKnYLnAvtCTnj > 0; wKnYLnAvtCTnj--) {
            WtpgrzvCqTnsqbQz -= WtpgrzvCqTnsqbQz;
            WtpgrzvCqTnsqbQz = EQXWLFk;
            JfvGkVNeye -= JfvGkVNeye;
            EQXWLFk /= JfvGkVNeye;
        }
    }

    return wkObd;
}

int SILdVx::GJhkrG()
{
    int FfQpmzVosu = 2140721644;
    double DgamRBngmL = 224220.1661711429;

    if (DgamRBngmL < 224220.1661711429) {
        for (int WCsbV = 2005012281; WCsbV > 0; WCsbV--) {
            FfQpmzVosu -= FfQpmzVosu;
            FfQpmzVosu /= FfQpmzVosu;
            DgamRBngmL *= DgamRBngmL;
            DgamRBngmL = DgamRBngmL;
            FfQpmzVosu *= FfQpmzVosu;
        }
    }

    for (int jhsrjtI = 2085915445; jhsrjtI > 0; jhsrjtI--) {
        DgamRBngmL -= DgamRBngmL;
        DgamRBngmL /= DgamRBngmL;
        FfQpmzVosu *= FfQpmzVosu;
        FfQpmzVosu += FfQpmzVosu;
    }

    return FfQpmzVosu;
}

string SILdVx::ReNpCH(string YSOeyfW, double RpqqCdnLivzUg, double aMWqhgEYYkhARg, int IUiQTaKjYdSN)
{
    int rFCjxcSsxOePwOqW = 1335393912;
    bool wTVFZcJeWSRzRyWD = true;
    bool IyDdGVlereZio = false;

    if (aMWqhgEYYkhARg < -341677.4664297711) {
        for (int zQXnF = 2019663319; zQXnF > 0; zQXnF--) {
            wTVFZcJeWSRzRyWD = ! IyDdGVlereZio;
            RpqqCdnLivzUg *= aMWqhgEYYkhARg;
            YSOeyfW += YSOeyfW;
            IyDdGVlereZio = ! IyDdGVlereZio;
            IUiQTaKjYdSN -= rFCjxcSsxOePwOqW;
        }
    }

    for (int cnuKkvNByLmaScHd = 810988094; cnuKkvNByLmaScHd > 0; cnuKkvNByLmaScHd--) {
        rFCjxcSsxOePwOqW -= rFCjxcSsxOePwOqW;
    }

    for (int JoMfXer = 1288684724; JoMfXer > 0; JoMfXer--) {
        wTVFZcJeWSRzRyWD = IyDdGVlereZio;
        RpqqCdnLivzUg /= aMWqhgEYYkhARg;
        rFCjxcSsxOePwOqW = IUiQTaKjYdSN;
    }

    return YSOeyfW;
}

double SILdVx::ANaCRDv()
{
    int kvFGAnVCqeC = 1441999498;
    int lXkeEBQRVH = 619663218;
    double PqRvL = -61031.82157921469;
    double SYBvuTTCyRVE = 182840.90529381795;
    double ncVEVSPnuPbCJ = -154068.59057652156;
    int OSinXYrGrUAuc = 1927568968;
    bool MCUHuKwy = false;
    string dGAUYkWaDpA = string("zTuBJKGhLVXRynsiUdLiwcCLqZWcNzbAShjUbuwYekmhuXYUjWLbDxenSHaswxedxwTKYhIZxorVvZLpYaTbedncdjpIkSYDFEQdMKjtKNBoJehWYZMZMKxOUbrMExHwUZWMvMLkiqkJPsCsfpAzuDvBvOyUDqqQfhPjyBVKaTQhgIiLyVkhal");

    if (PqRvL > -154068.59057652156) {
        for (int zzuABxUNHDSe = 2001400343; zzuABxUNHDSe > 0; zzuABxUNHDSe--) {
            lXkeEBQRVH = lXkeEBQRVH;
        }
    }

    for (int AaTlJ = 1575132039; AaTlJ > 0; AaTlJ--) {
        SYBvuTTCyRVE -= PqRvL;
        SYBvuTTCyRVE /= ncVEVSPnuPbCJ;
        ncVEVSPnuPbCJ /= SYBvuTTCyRVE;
    }

    for (int sSzrIatd = 1320331057; sSzrIatd > 0; sSzrIatd--) {
        OSinXYrGrUAuc -= lXkeEBQRVH;
    }

    return ncVEVSPnuPbCJ;
}

string SILdVx::hnFABefXQ()
{
    int dzefZKFrpImlIhkH = -218317090;
    double FYdJHjtLpcMqIpR = 667559.136166563;
    bool gSkjr = false;
    bool RYRsymfFJJZn = true;

    if (FYdJHjtLpcMqIpR != 667559.136166563) {
        for (int CxcQRntffUaWiAab = 1483061499; CxcQRntffUaWiAab > 0; CxcQRntffUaWiAab--) {
            gSkjr = ! RYRsymfFJJZn;
            RYRsymfFJJZn = ! RYRsymfFJJZn;
            RYRsymfFJJZn = ! RYRsymfFJJZn;
            RYRsymfFJJZn = ! RYRsymfFJJZn;
            dzefZKFrpImlIhkH -= dzefZKFrpImlIhkH;
        }
    }

    for (int yQpqOeWjy = 1302077286; yQpqOeWjy > 0; yQpqOeWjy--) {
        continue;
    }

    for (int bCNAhnmiULP = 1963420006; bCNAhnmiULP > 0; bCNAhnmiULP--) {
        gSkjr = ! gSkjr;
        RYRsymfFJJZn = ! RYRsymfFJJZn;
        dzefZKFrpImlIhkH -= dzefZKFrpImlIhkH;
        gSkjr = ! gSkjr;
    }

    if (gSkjr == false) {
        for (int OKalgxHQ = 1057148322; OKalgxHQ > 0; OKalgxHQ--) {
            dzefZKFrpImlIhkH = dzefZKFrpImlIhkH;
            RYRsymfFJJZn = RYRsymfFJJZn;
        }
    }

    if (gSkjr == true) {
        for (int JFYzn = 420229801; JFYzn > 0; JFYzn--) {
            continue;
        }
    }

    if (dzefZKFrpImlIhkH < -218317090) {
        for (int iWxcjHXWPfdMg = 353648481; iWxcjHXWPfdMg > 0; iWxcjHXWPfdMg--) {
            RYRsymfFJJZn = gSkjr;
            dzefZKFrpImlIhkH = dzefZKFrpImlIhkH;
        }
    }

    if (RYRsymfFJJZn == true) {
        for (int IqDTDusTHz = 1935780997; IqDTDusTHz > 0; IqDTDusTHz--) {
            RYRsymfFJJZn = gSkjr;
            gSkjr = ! gSkjr;
            RYRsymfFJJZn = ! RYRsymfFJJZn;
            gSkjr = ! gSkjr;
        }
    }

    return string("qWrwXTtydWcgEIqITYQgRCfTmnWezLxAnVFGkdDpwSwlUQwpwL");
}

bool SILdVx::iZNOoECILCgw(int rcAPbicaD, string mnGPDyILN)
{
    string TrQtcufDwbtRJ = string("sfzWiemZQctfiiTAUkitEfccBLJwEhmIdkIkPAegZtQSibbTNBivbSsPhKZzvUwNiCvIckMOnuuMeVrCxEDrzTlTvFNCqBlONIfvFJjBJZazANYQC");
    bool MkcFRhFiP = false;
    int ebPCZTqDo = 975815624;
    bool TwCrJ = true;
    bool DoGZEgmiVgABzK = false;
    double uHszbkZVxjIavKpF = -504802.15289483045;
    bool wbwWXNMfPODtOeIO = false;

    return wbwWXNMfPODtOeIO;
}

void SILdVx::xvwieuoyzdOqzV(double rqkxyWnRhNpFHQHo, int NIpRyr, int GIyBPWpNBtn)
{
    double BHshM = -917095.6939728612;
    int iWQAdyDRYMIdkA = -431453998;
    double iqVeRe = 741198.1126412271;
    string FdynCb = string("heOkshAjruiYEexuGPOVNqEFmwCCCVAIFSBZNZewhItkqWHYXXPaUALmlrkwpeaEzNnryzrjhLcrYnjCHyQBvsdziAKjIGlBsKGnYnMZmuhdEHaDQngVBLUTdnbvYqeCMkYcCPCSKKOhVRuKHvBLaLzRZHtcNzMSKdqQwlhFY");
}

double SILdVx::lzrtI(int GXZtgZbNLxOyeU, double taMIoHcXNpGtAAV, string loOxy, double aYrHmTsvqQtYnhQ)
{
    int xLxEXl = 808264791;
    bool bULXoucXZIGg = true;

    return aYrHmTsvqQtYnhQ;
}

double SILdVx::FezNvuTXZfrR(string gTzBvzmT)
{
    bool ntUrEhEPVdexwu = true;
    int hfZuiubmQATjmVXz = -1440137782;
    bool kxDJXsWgTZKx = false;
    double ZdYPIuyKsAbGZ = -206120.2270225743;
    string wmFaPcmLpVdPd = string("AyqwOtizESnGWmIODLKaqEkZxoHYLRxkIIqDqCsDopMILmJNDziSTByKQKysrfaDDSvwpabuUVeskXBYHbSapUwzSPuyYYuBOdDnlnfFiuaaUhKUrRhnMSZZGwmLPNQioOgbdezcLxjkqVbwtOqAAucBMWVgMjYf");
    int PEIkhfeeHtBKlQy = -273051457;
    int kalKHMCiw = -1103764334;
    int bWOnbgxRuLcUfD = 965886208;

    for (int ygpDEcmfYvS = 2000661245; ygpDEcmfYvS > 0; ygpDEcmfYvS--) {
        bWOnbgxRuLcUfD *= PEIkhfeeHtBKlQy;
    }

    for (int IIITUHNWndyYUhk = 656805913; IIITUHNWndyYUhk > 0; IIITUHNWndyYUhk--) {
        continue;
    }

    for (int IfrQWddolakb = 512478158; IfrQWddolakb > 0; IfrQWddolakb--) {
        continue;
    }

    for (int ybqIX = 170437839; ybqIX > 0; ybqIX--) {
        continue;
    }

    return ZdYPIuyKsAbGZ;
}

void SILdVx::CPKtUGfK()
{
    int TixxndOr = 1374552030;
    bool NVhgO = true;
    string oqEEucBtVQnA = string("rpsptotsgKJvMJVIKGUuPgvuJsUIFlfcFLtAlsmBDCejflMxenFTquHXpcmLNdiuCtQbWyMxWaUoMFmszBSNiFzJscnyTYUVCTQqWeRykweHhb");
    bool cXMyCQsztPhpq = true;
    bool UjGiVesPKoEg = true;
    double IaxNQRukpTbKUAJ = 205826.72212299565;
    string DKKBWJKpfk = string("igswOdhHSWYJtkZbVwwYoLFySILOOCBQoZmQAGGifUlOZQtvbdIYCkBKoufMMoPHpPYkBQAeiEjhkjolwJATiJwtJjAxDQqgeTTPKTBqEVLvdHcntqZDlQJDNuVkOmeCRztlDcdCYHEzZ");
    bool JxOFLvxRfVqE = true;
    string uREjNwQAFRkCS = string("ZjZEVyWCstFrOrzzGwKLsqoCfUeBDjdhqaYOHQqTmcFDAhLQvjIbXhYwsQksrsiMGmflsicKLSnkAxEiDDrnDJLakspKWJGjktknMgrPOoNCrbpFEDgrsjVJJJQlbkfpsfsTukkiGKFkCwhklMlKeHugbOywWfdgfRWeOXBXNjyduYVddJzSBszkivLSQkjlEjuiBhEwHAw");
    double psbylHpBOdjJ = -174139.16767957812;

    for (int EfQzqGI = 1322079103; EfQzqGI > 0; EfQzqGI--) {
        continue;
    }

    if (JxOFLvxRfVqE == true) {
        for (int qqprWODlixEmmb = 2020090837; qqprWODlixEmmb > 0; qqprWODlixEmmb--) {
            NVhgO = ! JxOFLvxRfVqE;
            oqEEucBtVQnA += oqEEucBtVQnA;
            cXMyCQsztPhpq = ! NVhgO;
            oqEEucBtVQnA += uREjNwQAFRkCS;
        }
    }
}

SILdVx::SILdVx()
{
    this->pDSceAvGPRxn(true);
    this->yUhQpUAWackHfF(-775578.1406501243, -141694046, false);
    this->lLAvUZ(string("gTHgcfSoQvVcbkdbHCZblQkKXgzeVSnUQgNHsHpltGTeSAAlzbTGFMjpuwzyUYTmfnxTOUsnfPyXkPIQjvDDqXYbeDuONDeAWrKmGeRYXUiECabqvOItdpoSIliyBhgMXRGBQYRepxMjFvOXhHfAhnKmHoWUhrwEixfIhICZqhpwDUoXcSrueSXZIzQnAhQj"), string("KOWJdtmAEUhDRWRRnftSCRlWRcdJAfShLkVyEkqqjuUOtIBpLVivQPiDTJtDBOsqConBSLmNhpTXjsGbOjaUJVaGHWxggBOVGmSTGOoqKElnCOyglblkgOMvsHUBQCfmWJLrbzxhAdJPtKIRSPXniVueQrEgmkcBequsMjrdKzZvzNBtWbVwNBjxPqwWsYsDpHBNMYOQsOXXtJNTHnLRsfUYHNMaQJrDfSPlehI"), string("IhpqAvVSbI"), 306546.8942675583);
    this->idzEEAeCqBhsUfjh(true);
    this->GJhkrG();
    this->ReNpCH(string("yszzKrxXquCCOFPLhcyVSCGQFqcyMcIvPADaQnGmAYdtMBxRQUCQblNEdsMIgDDvgzKiSgoeJAJgbwlWOSDgHauiCFtysGRtxjRMolaLzWSHmEdnYxmfEdSVDKZvXtIfsXjhuPceiBnRVeMYCuF"), -341677.4664297711, 836517.7400751372, -1286861932);
    this->ANaCRDv();
    this->hnFABefXQ();
    this->iZNOoECILCgw(-130891956, string("HUagTHqxzQvgTDyRqmWhaPPLRfWZADZMsNwiXDAmidYpnJrMOuLlDHVWDnVNNhwsfgXsipfdSACnmsxKUKsNWKXcoivFYvzfQYrfDFkxmsBNQzKJjyittyYyRDKRcygcqpkDgKEflZnXuuumCCFggttLqYFtRgltSXzHYytGNaG"));
    this->xvwieuoyzdOqzV(918207.8155347939, 192356521, 886038288);
    this->lzrtI(-679567436, 977898.0151895188, string("hovkgEeJbKvOXRsYmMEfroSuuBHZWUclrQiVLDHPdejfmGpVXuIgTcHRrQFhnxWZrbPMtvaHgRebCXlGulmGWXHgmWcYllQHFLAgXzjfrXdwdvLPFdffiYByhNdRbAHGxNyBgSOcrmqhDqCrUaNaakJUleoszmzWEshHNLDnfwXEUoLcXF"), -939597.3794516715);
    this->FezNvuTXZfrR(string("EfvfWDEYJnKQfWcQdWfrvnLhMQqppvPJTfvmHrjfSemCpAqYdlNgjEb"));
    this->CPKtUGfK();
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class TCWyRdgwCDdHQkL
{
public:
    double EucalpIcWHRYbKqY;
    string GzHxB;
    double RKpBEFaLWt;
    double lpFIJWUCWguM;
    int KFrzIKyHtMf;

    TCWyRdgwCDdHQkL();
    string ATZqBjtncyDLYp(string ahSVWFSYn, string qmGXNgIiSKhNWbKF);
    int KGuiDZoP(bool ecCOG, int zpaXSTS, double NLHSBTJAPkOLlT, bool GhHOyYyGyJyzouQi, string CuSSKtOksoSR);
    void fEbZJVwtVO(int ztQWHneN, bool VgHCPFKLiwX, bool jHolTyfR, string WSGYnxllBBaCLv, double tMrlTngeV);
    string PitSfMe(int rGMlzhQtSPmE, bool aseJe);
    int aQUomYir(bool UblteNaYca, string dGPExVaWpcJMJU);
    string eawmWIzO();
protected:
    double HEJNAPTOmxWz;

private:
    int hapBGacjpnBtJX;

    void DBYnIDRqTfcDzxC(bool zfBjmvxZHcn, string CfQiJ, bool FwrDrcM, double WOjpRmFrqLmxyNh, double cVXoClzIXva);
    double xaWqxCYxnCS(int qPwZJWdsPkxEA, string CYMudW, double VVYNcaeBFVYiga, double cFgWDGWyhHcfvpBe);
    string mSlrIxNzncbJnmP(double BoRqaLGvElK, string CLeYPYpOt, bool eoGaTl, int XcqvKfYokrVEcTJa, double OGkbHjtrqEaNx);
    bool KIisRINcqKqoM(string sPlUfVKNPNcqO, int NoOMcJPuHlh, double RNnyAW);
    int FkZLBzZzkT(string WOVtNcoLRK, int MJNIYgbhNSu);
    string ogsuWRtWHIm(string DPlXMlXytTMSJMIw, string GrrAhtGgsxfc, bool zSxwdwXzxdZB, string cdtIKIxFUpJFbTWY);
};

string TCWyRdgwCDdHQkL::ATZqBjtncyDLYp(string ahSVWFSYn, string qmGXNgIiSKhNWbKF)
{
    int gVCOmnghGrTaUR = 1555099870;
    bool Vewhd = true;
    bool sSjEQEzt = true;
    string cWxWD = string("sknVmjBXZxWEXAjSCSXcnyIbyLBWcqDyoGJKPwlWdyDVSnWGLSRabXfzqCYyWxbHSDOCcEQuRLgeDvlBLnqaDsNvDBbUdsHURLmdQeqUtMAaorWkJmDAJSNZzyJHMsAjpPJcyCiRECyWsTsDlkKMziMIbOxIfQCORslCoCxNNLPHJASQargcm");

    if (Vewhd == true) {
        for (int DbbZBrVDXrRs = 346918538; DbbZBrVDXrRs > 0; DbbZBrVDXrRs--) {
            cWxWD = cWxWD;
        }
    }

    for (int PIKbaJQzfqC = 886776925; PIKbaJQzfqC > 0; PIKbaJQzfqC--) {
        ahSVWFSYn += ahSVWFSYn;
    }

    return cWxWD;
}

int TCWyRdgwCDdHQkL::KGuiDZoP(bool ecCOG, int zpaXSTS, double NLHSBTJAPkOLlT, bool GhHOyYyGyJyzouQi, string CuSSKtOksoSR)
{
    bool dRrTowCSmc = false;
    string PKcTO = string("mCnDAlIDtzQvdqZDoBvgzQdHBpvMIXX");
    bool LQvynxtpbdaXKn = false;
    int aujzkEiPBLqWj = 1657830705;
    double EvjpNZmKMJxbMnyR = 203444.63848191831;
    string QWZzZGMRGUqYww = string("LODpUhVFGZvpEmuEYBlOdqyCGtOMDRYYIuZSnLxDHkodpdSckLGpIPrLdjlLoaFbZCnCikvQliMNrWrWsGjmbsgxDiedvWkBdpNmoWDtvPpWUCDMjJdRcYHuBmLGVA");

    for (int TzWvneXhaqWgAG = 503687607; TzWvneXhaqWgAG > 0; TzWvneXhaqWgAG--) {
        GhHOyYyGyJyzouQi = ! LQvynxtpbdaXKn;
    }

    for (int vFoCeHU = 1652824280; vFoCeHU > 0; vFoCeHU--) {
        LQvynxtpbdaXKn = ecCOG;
        zpaXSTS += zpaXSTS;
        LQvynxtpbdaXKn = ecCOG;
    }

    if (EvjpNZmKMJxbMnyR != 173123.32615181312) {
        for (int QuAWFTGiZYfC = 1603753955; QuAWFTGiZYfC > 0; QuAWFTGiZYfC--) {
            LQvynxtpbdaXKn = ! dRrTowCSmc;
        }
    }

    if (dRrTowCSmc == true) {
        for (int AstDXEXMp = 377343391; AstDXEXMp > 0; AstDXEXMp--) {
            aujzkEiPBLqWj -= zpaXSTS;
            CuSSKtOksoSR += PKcTO;
            zpaXSTS *= zpaXSTS;
        }
    }

    return aujzkEiPBLqWj;
}

void TCWyRdgwCDdHQkL::fEbZJVwtVO(int ztQWHneN, bool VgHCPFKLiwX, bool jHolTyfR, string WSGYnxllBBaCLv, double tMrlTngeV)
{
    bool paZALCEgzLjA = false;
    string DExTecPvyjq = string("mhvpImOLtSW");
    string WaUmwTBH = string("HxHCQTPjnxdzKCAjpfUZlKVnksbNCiGPvgYbqwYNUVanKaeqQGOmdHypdqXVuMHaVIgKRzNFVpXIibQbVOjhJmmUjZkqGQheRkQtFIFLpGNZrbIrSGfVzgTmgPeNAoXnHWCgPgsBnwsoADZiJhUsizYr");
    bool WQwkYXEN = false;
    bool jAbZlfXJDIJ = true;
    int ZNWRxVer = -372287874;
    bool ROoweddYSNA = true;
    double ykgQPtmOeQuhNW = 949202.043904188;
    string RgkCWxHXdRj = string("qKbVDThgrDmeoSuRmAaFhjWuTVUPvqzlDqaBXJdSkaVWXyLQhGjyrElpF");
    bool fTyIlxTrbK = true;

    for (int ncDyoGRurMnIP = 763165519; ncDyoGRurMnIP > 0; ncDyoGRurMnIP--) {
        WSGYnxllBBaCLv += WSGYnxllBBaCLv;
        ZNWRxVer *= ZNWRxVer;
        paZALCEgzLjA = ROoweddYSNA;
    }

    if (paZALCEgzLjA != false) {
        for (int KDaRlENaYZhGLvQ = 1061391236; KDaRlENaYZhGLvQ > 0; KDaRlENaYZhGLvQ--) {
            paZALCEgzLjA = ! paZALCEgzLjA;
            ykgQPtmOeQuhNW *= tMrlTngeV;
            WQwkYXEN = paZALCEgzLjA;
        }
    }

    for (int yHCPrVZaTYgYI = 1862033694; yHCPrVZaTYgYI > 0; yHCPrVZaTYgYI--) {
        jAbZlfXJDIJ = VgHCPFKLiwX;
        paZALCEgzLjA = WQwkYXEN;
        paZALCEgzLjA = ! ROoweddYSNA;
        paZALCEgzLjA = ROoweddYSNA;
    }

    if (ykgQPtmOeQuhNW > 949202.043904188) {
        for (int azLPDIig = 110397375; azLPDIig > 0; azLPDIig--) {
            WaUmwTBH += DExTecPvyjq;
            ztQWHneN += ZNWRxVer;
        }
    }

    if (DExTecPvyjq > string("avGAhelfWkDoVgcgRvZOMRcfUCmLniBsTzQebodUepTuxfuetUAgloluALjCpeiGocioGrlvjiPVpaJxyXPKgezhUduQlxvSnLEqzWWzcQMBLHmwKETdrraDtWHdabsKaJuvIsPSitRCMhAdfvib")) {
        for (int zykphxGa = 362479157; zykphxGa > 0; zykphxGa--) {
            WQwkYXEN = ! jHolTyfR;
            ROoweddYSNA = ROoweddYSNA;
        }
    }

    if (WSGYnxllBBaCLv == string("mhvpImOLtSW")) {
        for (int QvIdBfcVjPgutPKS = 1933487506; QvIdBfcVjPgutPKS > 0; QvIdBfcVjPgutPKS--) {
            ztQWHneN *= ZNWRxVer;
            ROoweddYSNA = ! paZALCEgzLjA;
            jAbZlfXJDIJ = ROoweddYSNA;
            WQwkYXEN = ! ROoweddYSNA;
            paZALCEgzLjA = paZALCEgzLjA;
            tMrlTngeV *= tMrlTngeV;
        }
    }
}

string TCWyRdgwCDdHQkL::PitSfMe(int rGMlzhQtSPmE, bool aseJe)
{
    double KvqKu = -45548.09122619445;
    bool ylFRKKjDxafC = true;
    string GCYoYBbKABBO = string("nv");

    if (ylFRKKjDxafC == true) {
        for (int hfIWyXAXFzDuIiDH = 1291466791; hfIWyXAXFzDuIiDH > 0; hfIWyXAXFzDuIiDH--) {
            rGMlzhQtSPmE -= rGMlzhQtSPmE;
            rGMlzhQtSPmE /= rGMlzhQtSPmE;
            KvqKu *= KvqKu;
        }
    }

    for (int NJffcO = 1651567050; NJffcO > 0; NJffcO--) {
        ylFRKKjDxafC = ! aseJe;
        ylFRKKjDxafC = ! ylFRKKjDxafC;
        aseJe = ! ylFRKKjDxafC;
        rGMlzhQtSPmE = rGMlzhQtSPmE;
    }

    return GCYoYBbKABBO;
}

int TCWyRdgwCDdHQkL::aQUomYir(bool UblteNaYca, string dGPExVaWpcJMJU)
{
    bool AIQdeZpzfhxXVxB = true;
    bool hcBCILueJYljifft = false;
    bool pNNtJMq = true;

    if (hcBCILueJYljifft == true) {
        for (int sFDRSZtksI = 793968170; sFDRSZtksI > 0; sFDRSZtksI--) {
            dGPExVaWpcJMJU = dGPExVaWpcJMJU;
            UblteNaYca = ! AIQdeZpzfhxXVxB;
            pNNtJMq = ! hcBCILueJYljifft;
            hcBCILueJYljifft = ! UblteNaYca;
        }
    }

    for (int BqNCkafsNcQZU = 265684772; BqNCkafsNcQZU > 0; BqNCkafsNcQZU--) {
        pNNtJMq = ! AIQdeZpzfhxXVxB;
        AIQdeZpzfhxXVxB = ! UblteNaYca;
        UblteNaYca = UblteNaYca;
        AIQdeZpzfhxXVxB = ! hcBCILueJYljifft;
        pNNtJMq = ! hcBCILueJYljifft;
        hcBCILueJYljifft = ! UblteNaYca;
        pNNtJMq = ! AIQdeZpzfhxXVxB;
    }

    if (pNNtJMq == true) {
        for (int DiCTeBpasmXsjt = 1228884031; DiCTeBpasmXsjt > 0; DiCTeBpasmXsjt--) {
            AIQdeZpzfhxXVxB = ! pNNtJMq;
            AIQdeZpzfhxXVxB = AIQdeZpzfhxXVxB;
            hcBCILueJYljifft = ! AIQdeZpzfhxXVxB;
            hcBCILueJYljifft = pNNtJMq;
            hcBCILueJYljifft = pNNtJMq;
            hcBCILueJYljifft = pNNtJMq;
        }
    }

    for (int OTeRjNBq = 833190387; OTeRjNBq > 0; OTeRjNBq--) {
        hcBCILueJYljifft = ! UblteNaYca;
        AIQdeZpzfhxXVxB = pNNtJMq;
    }

    for (int gUZONq = 1032227134; gUZONq > 0; gUZONq--) {
        AIQdeZpzfhxXVxB = UblteNaYca;
        AIQdeZpzfhxXVxB = ! pNNtJMq;
        dGPExVaWpcJMJU += dGPExVaWpcJMJU;
        AIQdeZpzfhxXVxB = AIQdeZpzfhxXVxB;
    }

    return -1359947287;
}

string TCWyRdgwCDdHQkL::eawmWIzO()
{
    string ZSPJHjTL = string("AsFcrXsJkqVkcgtGEVwQFgMipsSzkMyNPItUIVAcGUCavTbSILXSNdyyGcSlWuAvApiYFDhZLWRCtUsqHoyzVlDxDbKyGaqyUqirkMHSxlSldNtRXXvmNfaxvvgNhGwZGdUfqtsTVORkm");

    if (ZSPJHjTL > string("AsFcrXsJkqVkcgtGEVwQFgMipsSzkMyNPItUIVAcGUCavTbSILXSNdyyGcSlWuAvApiYFDhZLWRCtUsqHoyzVlDxDbKyGaqyUqirkMHSxlSldNtRXXvmNfaxvvgNhGwZGdUfqtsTVORkm")) {
        for (int syuVUlKtpRPil = 253199269; syuVUlKtpRPil > 0; syuVUlKtpRPil--) {
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
        }
    }

    if (ZSPJHjTL >= string("AsFcrXsJkqVkcgtGEVwQFgMipsSzkMyNPItUIVAcGUCavTbSILXSNdyyGcSlWuAvApiYFDhZLWRCtUsqHoyzVlDxDbKyGaqyUqirkMHSxlSldNtRXXvmNfaxvvgNhGwZGdUfqtsTVORkm")) {
        for (int AhHaDQPYs = 1157940322; AhHaDQPYs > 0; AhHaDQPYs--) {
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
        }
    }

    if (ZSPJHjTL >= string("AsFcrXsJkqVkcgtGEVwQFgMipsSzkMyNPItUIVAcGUCavTbSILXSNdyyGcSlWuAvApiYFDhZLWRCtUsqHoyzVlDxDbKyGaqyUqirkMHSxlSldNtRXXvmNfaxvvgNhGwZGdUfqtsTVORkm")) {
        for (int sFyDhqmjGK = 1674198635; sFyDhqmjGK > 0; sFyDhqmjGK--) {
            ZSPJHjTL = ZSPJHjTL;
        }
    }

    if (ZSPJHjTL < string("AsFcrXsJkqVkcgtGEVwQFgMipsSzkMyNPItUIVAcGUCavTbSILXSNdyyGcSlWuAvApiYFDhZLWRCtUsqHoyzVlDxDbKyGaqyUqirkMHSxlSldNtRXXvmNfaxvvgNhGwZGdUfqtsTVORkm")) {
        for (int unLxnWNdHjfHn = 849843316; unLxnWNdHjfHn > 0; unLxnWNdHjfHn--) {
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
        }
    }

    if (ZSPJHjTL > string("AsFcrXsJkqVkcgtGEVwQFgMipsSzkMyNPItUIVAcGUCavTbSILXSNdyyGcSlWuAvApiYFDhZLWRCtUsqHoyzVlDxDbKyGaqyUqirkMHSxlSldNtRXXvmNfaxvvgNhGwZGdUfqtsTVORkm")) {
        for (int ftCJPf = 907858147; ftCJPf > 0; ftCJPf--) {
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
        }
    }

    if (ZSPJHjTL < string("AsFcrXsJkqVkcgtGEVwQFgMipsSzkMyNPItUIVAcGUCavTbSILXSNdyyGcSlWuAvApiYFDhZLWRCtUsqHoyzVlDxDbKyGaqyUqirkMHSxlSldNtRXXvmNfaxvvgNhGwZGdUfqtsTVORkm")) {
        for (int tZBNaGmgM = 931626074; tZBNaGmgM > 0; tZBNaGmgM--) {
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
            ZSPJHjTL += ZSPJHjTL;
            ZSPJHjTL = ZSPJHjTL;
        }
    }

    return ZSPJHjTL;
}

void TCWyRdgwCDdHQkL::DBYnIDRqTfcDzxC(bool zfBjmvxZHcn, string CfQiJ, bool FwrDrcM, double WOjpRmFrqLmxyNh, double cVXoClzIXva)
{
    bool MIKAeSGyBZlzYOQ = true;
    int GQJFqYeNDcuQYaVP = -2374801;
    int YPTZANI = 605177229;
    bool qrmCQM = false;
    bool vGrLLHYhEQ = false;
    string UwOJeBTBQxQu = string("YZdLywKSpNwWOSRWqCgcjQhCoMZVPDzokTKoYGGYKDKANRPZZRpqxZwCUChIGpcNhJoOHamzvGhuAqxcWCIETAiEwbrELrYbbqwZlwzanDbRFyOEWknOoSNdlcGrrRXsnRTmIpA");

    for (int DafUiNfnecOzqR = 518526631; DafUiNfnecOzqR > 0; DafUiNfnecOzqR--) {
        FwrDrcM = ! qrmCQM;
    }

    for (int VOasbsYB = 1978690645; VOasbsYB > 0; VOasbsYB--) {
        FwrDrcM = ! zfBjmvxZHcn;
    }

    for (int sRslIT = 541845942; sRslIT > 0; sRslIT--) {
        CfQiJ += CfQiJ;
    }

    for (int cukFVuF = 474853153; cukFVuF > 0; cukFVuF--) {
        FwrDrcM = ! qrmCQM;
        FwrDrcM = FwrDrcM;
        qrmCQM = FwrDrcM;
    }
}

double TCWyRdgwCDdHQkL::xaWqxCYxnCS(int qPwZJWdsPkxEA, string CYMudW, double VVYNcaeBFVYiga, double cFgWDGWyhHcfvpBe)
{
    double UxbiJrWxIMnSi = -940602.2491739546;
    int PZOGy = 347745667;
    string YsJMf = string("xBGSQWrRxxNvOXLbozLyVxTDIvnEtPjBQIUiCardjdExWUcoMTrkrHxWcMNZyayAjmPhWwHAMPvyzwbeInQnUXRIWuYuScIHCWOOJLwQEkGMGAEjOngtxKFReJCfLPaILRVgPxifPyMSTtwPzPqDGVHVXaDRfcHrcUROTbFMQyFlwlJAeLrglHmHwv");
    string cQRhwKRoBsLi = string("pySasvMPCZqSnofJohzdEtIFdaxRgniwbExvwlOlDJYgVLpqZODmngieTrKOyaDWMKJeVlBltdyQvarvhNoAhvBLfrarESlrvGTlNRpEhTuhQPbpFqgrnbvfQsIOQDOCteYEdviiEKfpqcJRAecRdpHMwjsHYTTqyzcgtoCFscACXjZiIBepqunmNckbFZOtWdnwvIewAdHarIHMYISFEtCiHjFctTcvudzzqKQmJLm");
    string YhiYVYs = string("HIPefgpDDCMqRReUKOtdKKlwyEBEbRzoZKBVAVfwOkGEzuikzwCsGWccgefPVPgGTxQXVZtdwdjIsulaSEPCoxERZcTlxoPctKqEXHTufBhgulWOLqEtsFDLkTLzZEmYOeuTYxEjzXwVEMCveshgFCigJLwwKXEhLAFrMsYgwMUxyCWUptYpAGRFwwzcoCGKNWicJNgjjDEgmTTdWadlpbvSWIsRxfVfWz");
    string SvcFKUmojxUPoWD = string("QkhvFlDmbeGtvfjutaXqlTMEHDhEGbUODUbogHcqSayaQywqZvgsHkdrgsLpzXdkWXHoZkYRmPzuWlCjzvjjBkzqQmfRuGIGXCafoBcIwCFzvuwqGflDVdqnxgwubVYwIhPeIlyLuRtABnNxtindGPxkPUgLfYTLPqirFJjPLzQkoRrPTLURgvO");
    string ohheu = string("AEZgJvlIjsvdbcagiodBwcOoJEyDEVpdqPCHjwaSJvhCoaUtqSJzzwDviipNWAY");
    double QQGtrtEVXee = 175162.0241610388;
    double hkFmwRet = -131155.7807577163;
    int uLATFAqIUhK = 608871800;

    if (YsJMf < string("xBGSQWrRxxNvOXLbozLyVxTDIvnEtPjBQIUiCardjdExWUcoMTrkrHxWcMNZyayAjmPhWwHAMPvyzwbeInQnUXRIWuYuScIHCWOOJLwQEkGMGAEjOngtxKFReJCfLPaILRVgPxifPyMSTtwPzPqDGVHVXaDRfcHrcUROTbFMQyFlwlJAeLrglHmHwv")) {
        for (int whExmRxVwBYSHo = 491422799; whExmRxVwBYSHo > 0; whExmRxVwBYSHo--) {
            cQRhwKRoBsLi += YhiYVYs;
            YhiYVYs = SvcFKUmojxUPoWD;
            hkFmwRet -= cFgWDGWyhHcfvpBe;
            YhiYVYs += SvcFKUmojxUPoWD;
        }
    }

    for (int YOZuEmWZUWymaHA = 1763025064; YOZuEmWZUWymaHA > 0; YOZuEmWZUWymaHA--) {
        continue;
    }

    for (int HVkAj = 1523990140; HVkAj > 0; HVkAj--) {
        YsJMf += YhiYVYs;
        YsJMf = SvcFKUmojxUPoWD;
    }

    if (cFgWDGWyhHcfvpBe < -131155.7807577163) {
        for (int LgaLFSEBE = 1273371440; LgaLFSEBE > 0; LgaLFSEBE--) {
            UxbiJrWxIMnSi *= UxbiJrWxIMnSi;
        }
    }

    return hkFmwRet;
}

string TCWyRdgwCDdHQkL::mSlrIxNzncbJnmP(double BoRqaLGvElK, string CLeYPYpOt, bool eoGaTl, int XcqvKfYokrVEcTJa, double OGkbHjtrqEaNx)
{
    bool iRWsXoaHMoVvgPC = true;
    string RubkDJxCkwnt = string("MEdoQsJsRCvmeFPjUOhrmoWIJkFjtiGpKPlukSYbsNgzWpoVJYBVEqMHNLQPRXoMsEHZXUQiZUqheaEYBazjUrZEULYFTuUkSyeiukOPSYcvcyJXxhFEzfmbjTXpTHVhhtyEKjMlmNdSbbZDhqdoNsrRnQlhuZtKCqXaJwxdrEOtJXJevpcMJfilpIALXfXECCCNMjnOVEdzi");
    string whfLUicxj = string("TmsCmeIuIOvsizeHSQnPMenrugbsRCqMMfoMHYLXWEuXveMzKQXObHQlKqxosuNLGcOEClnLD");
    double xallRsazPmg = -20216.83373530374;

    for (int RUgpcEeoC = 1312768163; RUgpcEeoC > 0; RUgpcEeoC--) {
        continue;
    }

    for (int aYXzHORYwhixJ = 1783990387; aYXzHORYwhixJ > 0; aYXzHORYwhixJ--) {
        continue;
    }

    return whfLUicxj;
}

bool TCWyRdgwCDdHQkL::KIisRINcqKqoM(string sPlUfVKNPNcqO, int NoOMcJPuHlh, double RNnyAW)
{
    double HQUZrZsiOUY = -318600.7649293816;
    string hUdelpkjjQlHK = string("nIJjnXuzxHhZpnlZdoaOFqrJvRyGFldkwvgHniqXVLMKJEnmtFGtvQJPPxiWaeGgQHeocsSxTapLRdssnTmBHplzvFVgxEtqaJyv");
    int XBiUIni = 702096371;
    string bdhlqe = string("loUhaDLYCOdGNjXLHxLEKwrVXLLnONNrCQkDXTkjTufUkAaMETm");
    int RDuAFfz = 1214232003;
    bool OsCdcZVg = true;

    for (int STkdsLEn = 242245032; STkdsLEn > 0; STkdsLEn--) {
        hUdelpkjjQlHK += hUdelpkjjQlHK;
        bdhlqe += bdhlqe;
        bdhlqe += sPlUfVKNPNcqO;
        RNnyAW += HQUZrZsiOUY;
    }

    for (int wnFNPKWfZUrepSm = 2122175577; wnFNPKWfZUrepSm > 0; wnFNPKWfZUrepSm--) {
        HQUZrZsiOUY -= RNnyAW;
        XBiUIni /= RDuAFfz;
    }

    return OsCdcZVg;
}

int TCWyRdgwCDdHQkL::FkZLBzZzkT(string WOVtNcoLRK, int MJNIYgbhNSu)
{
    string vIaLOBoiPdriqH = string("oslFJZmUxDYQQsNheAdqzpKzMFlCdfqRbAragEuktYWqqyVYFngWaMQszdRaIxZakSkTxLytNifUPTSGdLVKCrJweFOHPpycqcdpiEwbZZYgVytPeVJqfkuGq");

    for (int siytgI = 1857466130; siytgI > 0; siytgI--) {
        WOVtNcoLRK += vIaLOBoiPdriqH;
        vIaLOBoiPdriqH += vIaLOBoiPdriqH;
        vIaLOBoiPdriqH += WOVtNcoLRK;
        WOVtNcoLRK = WOVtNcoLRK;
        WOVtNcoLRK += WOVtNcoLRK;
    }

    if (MJNIYgbhNSu >= 1919787446) {
        for (int ETngKMOMcL = 29210572; ETngKMOMcL > 0; ETngKMOMcL--) {
            vIaLOBoiPdriqH += vIaLOBoiPdriqH;
        }
    }

    return MJNIYgbhNSu;
}

string TCWyRdgwCDdHQkL::ogsuWRtWHIm(string DPlXMlXytTMSJMIw, string GrrAhtGgsxfc, bool zSxwdwXzxdZB, string cdtIKIxFUpJFbTWY)
{
    string slXEf = string("RAKcurTgQKZXXaNBIyeoWfNiGHQxxOWYDGGdZAlwvpNCWlTcUioQdFamYnNzvTWWVhfHNdihttazkmLaVeCpnniWGrllLWLYlZBOYDabhr");
    int btxfydllnGGk = 1989450976;
    bool OIbQPURJZUg = true;
    double XgexZpbSfg = -831640.0497279026;
    string CQZbtzzn = string("XCbkMsZrOAOzCjoCIYSzqWpxcTocfAq");
    bool BZbQYeyw = false;

    for (int meMSNXtXZEUj = 301018116; meMSNXtXZEUj > 0; meMSNXtXZEUj--) {
        cdtIKIxFUpJFbTWY = cdtIKIxFUpJFbTWY;
    }

    if (DPlXMlXytTMSJMIw > string("RAKcurTgQKZXXaNBIyeoWfNiGHQxxOWYDGGdZAlwvpNCWlTcUioQdFamYnNzvTWWVhfHNdihttazkmLaVeCpnniWGrllLWLYlZBOYDabhr")) {
        for (int hZMlUwJs = 880316850; hZMlUwJs > 0; hZMlUwJs--) {
            slXEf += slXEf;
            GrrAhtGgsxfc = DPlXMlXytTMSJMIw;
            cdtIKIxFUpJFbTWY = cdtIKIxFUpJFbTWY;
            zSxwdwXzxdZB = ! zSxwdwXzxdZB;
        }
    }

    for (int EJzDVBLrjYBoe = 1128437421; EJzDVBLrjYBoe > 0; EJzDVBLrjYBoe--) {
        continue;
    }

    return CQZbtzzn;
}

TCWyRdgwCDdHQkL::TCWyRdgwCDdHQkL()
{
    this->ATZqBjtncyDLYp(string("DgVMyMSJelUFnmnxwoagqzvmNQxHmhRaVOAGIZMhCdiKwXeYLEwHeecbZKeWUcBRSQNseiNvXoBvMSftWtIbdcPymDApnNpYCWTzkItbvgvbsFUqPLAjsoubCWyhMYJeLBVSEdrhngkXSHACZCqFNhFEaGkGh"), string("vgrAsVdvKBYEQkWFokGWdaLIWsdEAcpuFdPCiLvNbPBPVBgW"));
    this->KGuiDZoP(true, -209963191, 173123.32615181312, true, string("PrSglaWmxUKWRyETgawFaqyDteJrttbHFKAlBQixEPNcpcDNnfFpvPFtyrdioQNRDunLgkGHoaDPTmZjjqIFXZvSXRvwNfYDxXlNVNYaxokmfbq"));
    this->fEbZJVwtVO(1131077640, true, true, string("avGAhelfWkDoVgcgRvZOMRcfUCmLniBsTzQebodUepTuxfuetUAgloluALjCpeiGocioGrlvjiPVpaJxyXPKgezhUduQlxvSnLEqzWWzcQMBLHmwKETdrraDtWHdabsKaJuvIsPSitRCMhAdfvib"), 62829.70227675255);
    this->PitSfMe(1613933686, true);
    this->aQUomYir(true, string("riCoDmBsEyaIrMeMjIfnwbYKdEVWwTehJWZkTtAcZFfjcjByZaxOfBawxReqcsbBdztvMqrznMYEzDbUOfGZNAUInRSiLFUqbtadLssCzNBWyRWWWvEfPGLyaCvBdcrToysAwHBSKfLIIdRyOkesTXmUyGSOQNRVxIvGKPMwSApXxpjkXxASPOpjPDuTLQarbptNbfqFBdwgfYKriwhoVlibQjzHlsQGpAJJVmBbhjXuZcIOIzpwWweQYt"));
    this->eawmWIzO();
    this->DBYnIDRqTfcDzxC(false, string("qpAVHNyAnEnCaEBCmEYUXQnJaxPtTmlYEloASBjsSHOOtgOaasablBvwnROAMfHzeawEFKudwhIvVugTecBuEPqfKctzwrPqxKBEsdYPnNGqSzllMtHKtHCdOkSsFmFbiAVsbiLfgoekEZtQgDyPOPtDuPHWbGZHXqJbPaCKmcGnpggwsIjChwKPCemdEeKrHhRGgQfGiKBFgfjbjQZhXZrBpuaMZpwLsDoRfQJFBVYZ"), true, -713738.6584800724, 746566.0405432485);
    this->xaWqxCYxnCS(-1039675980, string("iAurjnOZSizyyKxBGgZhLApbvOpWpyDYcZNikihzXybjQkSknMAuAJZdFUGNXS"), -1047032.8352284402, 784513.5036690109);
    this->mSlrIxNzncbJnmP(756283.6555087025, string("nXsXuFRwvtJjTrJKmGEtbXCTtAuJBYljQjjCqsMVfsvCEVFCEyzMMuIftXAGTxNraDWOpVVHSApVWsrMtOUKKzyMcVyriMbtBPXNzFgZWUJhfZmSb"), false, -1325144391, -762963.9225861151);
    this->KIisRINcqKqoM(string("DgRZlLolPgeLKSdBqHkyAULTHdxxItLyvcuxbfvqYvJwGOfxnQBBtJUspWwvgtknPnyneLOxZAdUTeKFznQkIdAYfVAOuhlUPKEiOTvBjIkasoClfhYaeDUYJRuLBdgkGWqMWJIJemJLypVlKzvFpWPQxtoQULsjNtSJXBIJzOxUDDhOwYZnqKuJjrLyITLFIRHPRHkUkDBgcjEgLigojgxWPhXSbIOLIPStXiHOegdUlopx"), -126361614, 741393.3532943589);
    this->FkZLBzZzkT(string("VRnevZNqssetyQXkWBTpWckMkGaEFfivFxJRbZbGZsUyheOacCjnnxmcayNSCJpGwqbHzFihNvPreRMRmhNCRQIrCVfEJyErxmAwGPlixanBzlyhqxFSBPCVUhsVnbYpWJaVwmZawvWjbmWTNDxCSvIlSFSmYstDqKMudEZyjzvlMAlehvAuNkMy"), 1919787446);
    this->ogsuWRtWHIm(string("AmVzYEhUyybUclmWkVEevUBtJQnltdDROSOxgbuHadwxmYivjxHGOUNOgznQvmWzYdRFTibXjOiwSkPLsWvyXpmQndDDgoAyRYyJFwrtBThDHPsWOllnSnrCoIPiYsSNBkQyPibucxuYyoqJNZtfewpSuvagUIUkQdFluAdFcqBdgBvcPIyEcjAlteljHybDJtcnXdIKObZOryKwZrsvbVYyHZwKdpOsIQrsKWArfQfpnNnobCeqkTpCBfa"), string("VqqdKMxrrSdemzUe"), true, string("ISrBhXRpcNijTAGokAPUndnijtPIuEJMzfciYkluWjIZhlWUVyhiKOreOQNuaKAqHFMpsyNsaVMIUSKLSJKiOeyrpWRXVXybNKmnCIOuAIFBTao"));
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class oddtPeQxuPzHtc
{
public:
    bool BfBGppNgFdJ;
    bool PPUDDdHE;
    bool ELaqhustkOSmSGQ;

    oddtPeQxuPzHtc();
    string jSkUszzOOjDyp(double YNGiGjxxySVefd, double QwbXHD, int WYvWeecOsTtXfRjJ);
protected:
    double qIBOLXpgF;
    int kxgcIcwB;

    double SCNRL(double VSqbfi, string zpFmIcrNCSVsH, int yANqH, double szCZSQnKdyXtuA, string epbbEk);
    bool AQFwYoURoHNREeen(string ifsKf, bool okgAPn);
private:
    string GzHYPJzeIwIMvpC;
    double FzCIvPllAGfgFDy;
    bool sFgHeNN;
    string VfJHpKr;
    double FXaaJnEFZi;

    bool gwPEFfMK(int BLDGmiIWjbySLwe, string hzPLTgIFJIITWx);
    bool FnTOEphFcuo(int EjdNqU, string rtURfddranPyyLW, string ZcTvHhJNrtrxH, bool DVwIZDROjXMi, bool ambZamymMAZxSIQs);
    string cbowNqqnBeXbrcQ(string CYWnYinLdLyOTE, double VlbDVFJhRQEBvJRk, double XKopLdsSvStS, string BmnHBsLTFxBIqD);
    void fuTvRirJ(int OYOHgQncIKZzs);
    bool hbOcbBwNe(int qcmtuFrmANjxFS, int yApgtMf, string XKPQBFmQMV);
    string APoTakE(string FMOurvMANgtXpkB);
    int OmiRAEn(int sOtGX, string iQxadrFEmFcZzKD, double MHvJXzvO);
};

string oddtPeQxuPzHtc::jSkUszzOOjDyp(double YNGiGjxxySVefd, double QwbXHD, int WYvWeecOsTtXfRjJ)
{
    string JZiLwPVdJTBjqCT = string("TkrBOYBVmDVeRdsGURkIqgHVLnRtFbbraZMlVRGJehafIixhikbQDkHxQUozQeYrWnMwqYuRvZlozrtpBjfYCFBEDFYDZRpTslqmt");
    double EQvkGorlifRg = -65471.60636298003;
    string LmyezYhl = string("RFCOJOqmagXvUwXCSBtxRKQgLKQXERgIuoOgQpsemwlnnvSChjJnEozNlGYcpEiCVgBMzsRqxLjPgjXhYvOsqvzSUgvCBVycyBUcVIaZVzUsRm");
    bool pottlUpHLmgM = true;
    double lkuPVsvLxNqVLS = 69736.80764499072;
    string fLvZHvLfic = string("LtSEBbjrUsCpPVZPSmnvBnMppZWUQyTMoOlENkTFqpnhJPcJPmLnDeKQgluCFcmjyiTvuLzxMaKLPcGIODPLcCQJhXaJnGxSBkUCcdErvUIsEPkrmuuUgOvVOsktAYtPwQoOAEvpiDtKbjNzbSxXNqimNEaWTFdptyMfVoKYqwG");

    return fLvZHvLfic;
}

double oddtPeQxuPzHtc::SCNRL(double VSqbfi, string zpFmIcrNCSVsH, int yANqH, double szCZSQnKdyXtuA, string epbbEk)
{
    bool BabvfUdCitW = false;
    int OrdwOp = 2010162502;
    int FYqOU = 92204667;

    if (szCZSQnKdyXtuA <= -450213.4386382547) {
        for (int GnekhaV = 928817562; GnekhaV > 0; GnekhaV--) {
            zpFmIcrNCSVsH = zpFmIcrNCSVsH;
        }
    }

    return szCZSQnKdyXtuA;
}

bool oddtPeQxuPzHtc::AQFwYoURoHNREeen(string ifsKf, bool okgAPn)
{
    string GWuvVxvkxJshGUm = string("BhuBJRXZNxNOdAymGURbVjniYpuKHhLNqZfXWkJxUkaPwaZSsXvXOcYWzEnVBjgCQSwFkcrDABueRfjgoMAFwkMv");
    double EOQgDnbboMDJSXps = 53276.259361060984;
    string ywgFdmxvJjG = string("wZESjsCeozoPYAgkJBNaRurTsCSTjWuZZtlEcfvsdRGYuEucihBiMJEdcTATEDlJaDPfEgvRHxchQUDEBsnEfoaXEPYctJVzVAZNRpZbwWEqirqPMncFdXUKqjJNVWEXpjskkzppSbvQXFLKHHenrmYCHWRDeXsymXcudFZOFRWeSFkupfvTGepVUf");
    bool bZqvjDgTacAcf = true;
    int FugPPnwRvCoT = -697764979;
    string znHRTO = string("ZbvEuigyHDOuVDgFZneqczsQIyWTZnvzRPChosmqMojdCHXrrThNlOUjDOmcqHAJxRziPsVwkxOMcRoNVBMWgeKFijlpTNQnjoOHmURhtxkbNhWfMKFMFqRbmsQhcMKudPsXGfRfhItizkwazkQBbUzhlXmTYFICLmNAKljMNTQfNXQUgqwTfbZuiyqhUQkHHjouSrLbHAgnh");

    for (int MAxWoWiLwv = 901287050; MAxWoWiLwv > 0; MAxWoWiLwv--) {
        ywgFdmxvJjG += ywgFdmxvJjG;
    }

    for (int snOdjyNnkMxutPFY = 1587433199; snOdjyNnkMxutPFY > 0; snOdjyNnkMxutPFY--) {
        GWuvVxvkxJshGUm = GWuvVxvkxJshGUm;
        okgAPn = ! bZqvjDgTacAcf;
    }

    return bZqvjDgTacAcf;
}

bool oddtPeQxuPzHtc::gwPEFfMK(int BLDGmiIWjbySLwe, string hzPLTgIFJIITWx)
{
    double oCzjwJsPrkqEeZT = 487926.3203427367;
    string qWdwfWIlCm = string("xdmtinDJVowoZTDTvTaFhYQadTxrVwHDWEndKRqXiKvTwOYzDwebWeaxPyNESqAYjDpSgBCDmzhYpmUVpYkvaDlQZsXyxCsxTHuGPbDqXCfVVlkKKeeMamfoSEgxnnptCTKljEnqHhQGhvAzQSJKyTUcdIGxxR");
    double KwAaCtA = 974761.1658332511;
    string nDSSekMEstiPNVb = string("bLRWuPB");
    string ZOywK = string("ZvERqhSmBfxcJBTmCatXONvSIRZFKttjkbWTRrhvJToQzIqafjnJSTVwwAZfABgyuFiGrAgudZNeCKmTZQdxRwRVjIYZyczJSmQzykhGKMFdvkWrLNdrChFtxZsjXaYWBuPkLZyfCfBGWfjROsuf");
    int ryYYWCpv = -2039417923;
    bool vXnAu = false;

    if (hzPLTgIFJIITWx >= string("ZvERqhSmBfxcJBTmCatXONvSIRZFKttjkbWTRrhvJToQzIqafjnJSTVwwAZfABgyuFiGrAgudZNeCKmTZQdxRwRVjIYZyczJSmQzykhGKMFdvkWrLNdrChFtxZsjXaYWBuPkLZyfCfBGWfjROsuf")) {
        for (int ymiyPGscLORZv = 1323684100; ymiyPGscLORZv > 0; ymiyPGscLORZv--) {
            qWdwfWIlCm = hzPLTgIFJIITWx;
            nDSSekMEstiPNVb = ZOywK;
            qWdwfWIlCm = ZOywK;
        }
    }

    if (qWdwfWIlCm <= string("bLRWuPB")) {
        for (int MTNVBJ = 55308122; MTNVBJ > 0; MTNVBJ--) {
            ZOywK = ZOywK;
        }
    }

    for (int BFTrQwNbwb = 526487229; BFTrQwNbwb > 0; BFTrQwNbwb--) {
        ZOywK += nDSSekMEstiPNVb;
        ryYYWCpv = ryYYWCpv;
        ZOywK = qWdwfWIlCm;
    }

    return vXnAu;
}

bool oddtPeQxuPzHtc::FnTOEphFcuo(int EjdNqU, string rtURfddranPyyLW, string ZcTvHhJNrtrxH, bool DVwIZDROjXMi, bool ambZamymMAZxSIQs)
{
    double ydyGcvMgZZ = -706345.9894831805;
    bool iLtXBVxDxr = false;
    int OyJiLswIThjSol = 252560296;
    string mCktRAxgdnHV = string("oxkmgUDosjVYCLtbmEfLlEqbf");
    string BbtqbdoWPQZNZiz = string("XmUAVaqSKZXqLMCKfNAzLGnlfGipsyZDvdWKCbnOifxCsRTJKPKmTMncDkkVHNoBOtPjhNOoVcGhsWMMQaPxvImnSxWWRdgHywcifjMuKBgXqMkkcdUINoZIcTbUMLskaOlzQXtnOecPqmAWFbUIJRtGmLstHklGCAzVjajmKsKuopHkVSDpSIfKHidyObfmqYDqaPzTikCGPiuyqjrSuIkHtxLLnCKtvQoCSORgiVnObwA");
    bool IObXtiDWlQt = false;
    string IXkKQVQfQ = string("iiGYWdRadKoZCxtwKmKAdZJmdtuVTOOmnkWmDqlTMlQzsPAbXsgOWPmoPRySzVktaPRXFVqppmwBeTWgqrFlOQDvFhcdJCKuCSdXlErBxWMMhaQhdpgJUAmmkWyhyUWDqMRItJInrd");
    string btLSVrZCnfyMZbxu = string("aaAZrEObaPzZUREZOOYjjkSqwWLpcigPgMmCHshcQcFMj");
    string tIXwszO = string("yBkGbhPUIVlDPTVasdeOqlWmAFspmKAfjmWU");
    bool BNcfAVyi = true;

    for (int woUqjrQhtvjbvhX = 1739491776; woUqjrQhtvjbvhX > 0; woUqjrQhtvjbvhX--) {
        rtURfddranPyyLW = mCktRAxgdnHV;
    }

    for (int tcePwTlHwxnK = 1915038284; tcePwTlHwxnK > 0; tcePwTlHwxnK--) {
        mCktRAxgdnHV += BbtqbdoWPQZNZiz;
        BNcfAVyi = ! IObXtiDWlQt;
        ambZamymMAZxSIQs = ambZamymMAZxSIQs;
    }

    for (int PfxzpcBAudcylV = 103670758; PfxzpcBAudcylV > 0; PfxzpcBAudcylV--) {
        mCktRAxgdnHV = BbtqbdoWPQZNZiz;
    }

    if (BbtqbdoWPQZNZiz < string("bgYJrZYkXNXxOIfXJiNxlIbbnAdRBFNGzaUPISQIEELPavUArOgzuxqmoMKrLiePKdUEuyZJbeOOvxkCmpSNTxDpLeBIncYUxdscJEdPgFQDKbiSXrl")) {
        for (int GAbIPGfVcRsFEFr = 716573681; GAbIPGfVcRsFEFr > 0; GAbIPGfVcRsFEFr--) {
            OyJiLswIThjSol -= EjdNqU;
            OyJiLswIThjSol /= EjdNqU;
        }
    }

    return BNcfAVyi;
}

string oddtPeQxuPzHtc::cbowNqqnBeXbrcQ(string CYWnYinLdLyOTE, double VlbDVFJhRQEBvJRk, double XKopLdsSvStS, string BmnHBsLTFxBIqD)
{
    bool GmgnRl = false;
    double bBxfbgba = 604952.831385847;
    double cCWQoDbNtdGf = 144618.4829646846;

    if (CYWnYinLdLyOTE > string("CSjWLOKBhyeecTwaXMoiAlGvYojPPcQEqZxTedCqqdOYQylLrEzACAlVzSYFvwiGKVSQLAMrVrFMzTmJbelVUMSUPhQinooQybTvCFwfUlDZKQUClGpdZwTYOraSMraZTCoErlkvfPHkqqNVzWJLudfWlZMMiOxeWxkXMoplkVlbmQKDdCtKfqqjvvIqTNtrepltwMal")) {
        for (int oWVjI = 2100612028; oWVjI > 0; oWVjI--) {
            VlbDVFJhRQEBvJRk /= VlbDVFJhRQEBvJRk;
            CYWnYinLdLyOTE = CYWnYinLdLyOTE;
            cCWQoDbNtdGf -= cCWQoDbNtdGf;
        }
    }

    for (int QtsGUgl = 174580893; QtsGUgl > 0; QtsGUgl--) {
        cCWQoDbNtdGf += cCWQoDbNtdGf;
    }

    if (bBxfbgba <= 604952.831385847) {
        for (int lvCQlaOnFu = 490515028; lvCQlaOnFu > 0; lvCQlaOnFu--) {
            XKopLdsSvStS -= cCWQoDbNtdGf;
            bBxfbgba += VlbDVFJhRQEBvJRk;
            VlbDVFJhRQEBvJRk += VlbDVFJhRQEBvJRk;
            CYWnYinLdLyOTE = CYWnYinLdLyOTE;
            VlbDVFJhRQEBvJRk += bBxfbgba;
        }
    }

    if (VlbDVFJhRQEBvJRk == 144618.4829646846) {
        for (int EMDzyhgcnUatV = 213056908; EMDzyhgcnUatV > 0; EMDzyhgcnUatV--) {
            bBxfbgba = cCWQoDbNtdGf;
        }
    }

    if (cCWQoDbNtdGf >= -107676.64658876267) {
        for (int hNgJwTLLIUvEJWbY = 2124650448; hNgJwTLLIUvEJWbY > 0; hNgJwTLLIUvEJWbY--) {
            VlbDVFJhRQEBvJRk /= cCWQoDbNtdGf;
            GmgnRl = ! GmgnRl;
            XKopLdsSvStS -= cCWQoDbNtdGf;
            VlbDVFJhRQEBvJRk /= XKopLdsSvStS;
        }
    }

    return BmnHBsLTFxBIqD;
}

void oddtPeQxuPzHtc::fuTvRirJ(int OYOHgQncIKZzs)
{
    string eYQVl = string("FYDYuqFobAmSclxskZbwLDuRVFiMgMduVOMcVRkTMNqWbHDndRTkWbXSiKsNTdCQksvKrJddzfCWYbfiZqFgjLWhlVrlSmMpIgBggKkbOMEqFTAqJYYytampmNWUfSdSbMlbXFrsTcUsYxolOVohCzxFJtBcKiMRfGBKQFrqSHNehKTljrMDqsPWz");
    double ZMsyZgR = 587299.1555132851;
    double IxDNp = -404343.7509115041;
    double gJbuShFCZyrWVZ = -827191.7046249012;
    double pnkGSfiMoAFM = -587786.9914145748;
    string BkkDPYJUdzNkmwh = string("tZqBcxluRzvyWJpiTwnVcyBWkNahRKzvzmVgTbmEjvxhAmsbLfrtThBRMhrnJMtrodVVEEOqfXnhdLihmhYzJYEhECCvgjJgbiLqdpVSQOFbIzKjfsEopgHAPTovBbpcnSlcyLhWfpDSNskJkCqqnSYtiqNazERlEL");
    int IOxTpgCmMLevw = 531705813;
    int lqCae = 2032140260;

    if (OYOHgQncIKZzs < 841937560) {
        for (int nBvoZpjwcdM = 1754513654; nBvoZpjwcdM > 0; nBvoZpjwcdM--) {
            IOxTpgCmMLevw -= IOxTpgCmMLevw;
            pnkGSfiMoAFM += gJbuShFCZyrWVZ;
            IOxTpgCmMLevw /= IOxTpgCmMLevw;
            pnkGSfiMoAFM = IxDNp;
        }
    }

    for (int IrJlwuPlvcGwq = 398962303; IrJlwuPlvcGwq > 0; IrJlwuPlvcGwq--) {
        ZMsyZgR /= ZMsyZgR;
    }
}

bool oddtPeQxuPzHtc::hbOcbBwNe(int qcmtuFrmANjxFS, int yApgtMf, string XKPQBFmQMV)
{
    double uZwxWtBYAzAe = -664147.9551863766;

    for (int aLqIjScgf = 1497203063; aLqIjScgf > 0; aLqIjScgf--) {
        continue;
    }

    for (int jXbYu = 568085272; jXbYu > 0; jXbYu--) {
        qcmtuFrmANjxFS = qcmtuFrmANjxFS;
        XKPQBFmQMV += XKPQBFmQMV;
        yApgtMf /= yApgtMf;
        qcmtuFrmANjxFS += yApgtMf;
    }

    for (int wciBIpUJY = 653098534; wciBIpUJY > 0; wciBIpUJY--) {
        yApgtMf /= qcmtuFrmANjxFS;
    }

    for (int HsDmagqCEAFrbvA = 1021449658; HsDmagqCEAFrbvA > 0; HsDmagqCEAFrbvA--) {
        uZwxWtBYAzAe += uZwxWtBYAzAe;
        yApgtMf *= qcmtuFrmANjxFS;
        XKPQBFmQMV = XKPQBFmQMV;
    }

    return false;
}

string oddtPeQxuPzHtc::APoTakE(string FMOurvMANgtXpkB)
{
    double ufyTEVrTKDq = -78371.72272663609;
    int IQgTZm = 1547081251;
    string NosXR = string("kNAhTuyPaPXVqOoqJJUyvwUeHbKNeqUclBIrojFwISkKLJpdVI");
    double XShQjNAZUhqbfL = 82487.04038690511;

    for (int doMojqQpmGimFC = 2072530957; doMojqQpmGimFC > 0; doMojqQpmGimFC--) {
        IQgTZm /= IQgTZm;
        NosXR += NosXR;
    }

    return NosXR;
}

int oddtPeQxuPzHtc::OmiRAEn(int sOtGX, string iQxadrFEmFcZzKD, double MHvJXzvO)
{
    bool IWjHFrT = false;
    int QVmALLIG = 1142843632;
    bool loqlhj = false;
    bool xKgNkVjAG = true;
    bool OeDroSPLIocfpVt = true;
    int VmbICRYJliAK = 846505599;
    string JKpkBsai = string("lkhNuBznhYWjmTxVMJFMlGszzBHsWNcYcxEzVvpAIEXslbmqwuMFatuWIjgdUZpQPBqnHIZRcowfLAIstIRXUiyZsXZGHcGWyPrviGPKhbaxvrHJDuGWeGfmDajcOtsSATFXnxuiZRzQunahKjlIpFQFVluyImDVWRtEyOEDvLhYkYYfdXvYVluvBwrukyMXzkAKYuzzQoiVPaOHtNVuxylAVzXUIweIjXXqZusfw");
    bool OrkLskuNXzGuael = false;
    double gGwLbqRI = 809680.5872774239;
    double iQZlBcfkxDN = 415264.3969848363;

    for (int nZQvLDfevhN = 1650949400; nZQvLDfevhN > 0; nZQvLDfevhN--) {
        MHvJXzvO = MHvJXzvO;
        OeDroSPLIocfpVt = OeDroSPLIocfpVt;
        xKgNkVjAG = OrkLskuNXzGuael;
    }

    for (int vcaWgZzFef = 1357357937; vcaWgZzFef > 0; vcaWgZzFef--) {
        continue;
    }

    for (int jgvXc = 1681182736; jgvXc > 0; jgvXc--) {
        continue;
    }

    return VmbICRYJliAK;
}

oddtPeQxuPzHtc::oddtPeQxuPzHtc()
{
    this->jSkUszzOOjDyp(66769.47498781963, -3061.758728739594, 1913209476);
    this->SCNRL(-582101.9103450113, string("wlrTsIUKMVHEjGWQggVCdifkPoTi"), 2081771288, -450213.4386382547, string("ArdXSTiJDsoaxCFgZTDtMhJWqAQTXbASCdXtoIHGnYWxTylFZxpcVGMTEjNTIfQWQUKVPAHvsRGhooRlyJVsLvgoiuqPPHtXZNssHglUFOvDVujADRHdxGQofalOxJydbkwqVeJYnSTdwsvcwyDaMVMpKBaAqsvpOYKhNETprXvVuWrnwsGTuMFkmPZTfnlnX"));
    this->AQFwYoURoHNREeen(string("LqpOPumJBmJFbWZbMPnuLlSPpTtPtXIcxSGeVeDTvYtcqkreKlNpqFGIbfmAuvHupURurIflfyngTKlBLEzsODUuJvyYOcFWvEjQuAXoHFwNKcQUAxwWXdQhWLJYupHadHGuyvmtmrqXfXPJCYIgoGPCqqsRoBaYbEBPlfDxwFTwgAJeJSqewoBRLAbejYW"), false);
    this->gwPEFfMK(-930112393, string("LbpdXsJrtHognxlRnRtTWIsIZRxcFYcfTUmyGBnvWSBdKbxWdJpLrhMOlcMwaUFANQEXnvLemtVWSSpbSVwPYTAUokUcqLnATqFWmiZJfnmGseSoskxztOlAVFGHezGvIeyTmLlqKIUWsy"));
    this->FnTOEphFcuo(-1932200271, string("rdFgpEZqnMBqIrnCcDJuzhYZchKrhHvyKupMhUVxGSKwJxIBgdLSUKlJXQvpdsStFjeVEAvEmFjKYaagrNxbKYxGkgsphshjigTglXPtvtJiovUjKiIQMylmXuqyUrmXNOhAxygOXLhIqSXSM"), string("bgYJrZYkXNXxOIfXJiNxlIbbnAdRBFNGzaUPISQIEELPavUArOgzuxqmoMKrLiePKdUEuyZJbeOOvxkCmpSNTxDpLeBIncYUxdscJEdPgFQDKbiSXrl"), false, true);
    this->cbowNqqnBeXbrcQ(string("CSjWLOKBhyeecTwaXMoiAlGvYojPPcQEqZxTedCqqdOYQylLrEzACAlVzSYFvwiGKVSQLAMrVrFMzTmJbelVUMSUPhQinooQybTvCFwfUlDZKQUClGpdZwTYOraSMraZTCoErlkvfPHkqqNVzWJLudfWlZMMiOxeWxkXMoplkVlbmQKDdCtKfqqjvvIqTNtrepltwMal"), -286857.82002082106, -107676.64658876267, string("PQurICsTUbjWRvdIhIEBLrCbRjOTuBKHFQzcZqgDLVvnTKgyXosCAbKanbQNxaIczXAcqaczJRodmQgDDEii"));
    this->fuTvRirJ(841937560);
    this->hbOcbBwNe(1526636353, -1095705206, string("RHSPsHZIaYlUnFhjlkgHQsGyXpTPCXRHCEPYiJZrtVCWNFqlkiyVMEGgnJTzoAEWICEtQybWpxRqyTwvKIrNGmiUjeqeiSHwTDHuOOAahYNEovpgXkpbAdkBtfuqUhbcKxSAXfAUmFFciuqCasnnREHtZDZKxFNMIgWKXvkvwQERUMFrcGvIBIxoOjvHRIyUZOclBuXf"));
    this->APoTakE(string("zXrgYyAQCkFoiQzSMjKIPVbCfRBnhPqpZmKQkXcoGGkpjsUrXBdWBZuIDCuABPwmZfMTyvYhjfwwvUcNuGgvKiOSLINQVl"));
    this->OmiRAEn(-1233993320, string("cFqgwdfUqxJiAsspVsPWXKtjGsmWSyfxCWRgWuYeLgHRxsLNojVHASVrObSKJVyBJdSnhkkYNvZOzPuhPPHmHlbuVdeEjdvvsJSjgaqqqDReORHCXLCPRsKRDpWqlXuzfkTqsQytiglyivQHXbuRCrfPhWqhBWCXCfETngfBWvRaOClEXBWflyXXkbQCkGwoaDcacGpchIDvPhiZlYEjOrbroX"), 664953.3788715497);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class eZbaIwFIzwX
{
public:
    int hhiqddDBTCxFq;
    int wblThbTqtbPJ;
    int QcjhzkHs;

    eZbaIwFIzwX();
    double oZBSGpfZflGIYDi(int IZfXxpbIHqqg);
    bool LORkURxQGPv(double SokEUY, int FqRhLI);
    void wvBVDCv();
    bool XMsQwq(int YaSKMhBoMxOJ, string VjzTHOajlPz);
    int Gomrjqz(string cBUKTHWpEfTkuAVn, double FBIOzl);
protected:
    int kZigBEMlcWeWGziM;
    string zcddRjwOm;

    string MsjEuCBDhkJSNLi(double xZqDZYnxJ, string StdKkXjOd);
    bool mwsmEETCtICvdaS();
    int EYcOXceLJCMVPdbA(int UZrQHLcDSYxVmT, int sSBKZVDmtRlKUPG, int ONXKR);
    void juWmRHInVFAulwMl(double mcCKVm, bool QdncSrpqOxNgKvY);
    void DVIkFGiFXJjC(double iPGKXHFFA, int GTihRTsoiHPbsj, string tVBmbax, string mQfKDNslzysPrLg);
    void gSpPkwOrvcx();
private:
    string SqdVtaptXvY;

    void Gtkcrx(double afKJVHZdP, bool SsJqvRQRX, int YLyXX);
    bool WLeIc(double cFpleburTMZNEoo, string nlBsmJvCceZGeB, string xJNYkNl, string olTDPgKxhhigs);
    string MwioUgbQ(string WvTNWfRz, double iYUVPSbM, bool JVJXLLnItlvnPOkM, int kkrDk, double mSKzmVMVVtvAYKX);
    string ZEcGbOZOzqbhvbS(bool vFEgWAJcxbzep);
    string PlHsB(int fyzqdDW, string BvFwZOkYWtOduZH);
    bool uCPVzwmfSroFKuL();
    bool zBFctwC(bool zITijubU);
};

double eZbaIwFIzwX::oZBSGpfZflGIYDi(int IZfXxpbIHqqg)
{
    string GHKIPfHh = string("zoFuOEHMsouytz");
    int kOxgsmZ = 1988428312;
    string Ixtwn = string("VLWuRFpfXVQnvxDctXnyHbZECQkaVXZAJAZTcAToupHXeMArcPSSLGUWeZlHFgxmxXjTNHWQuwKpwqoBArZnoKmg");
    double glMhOvrvUdLSxOQn = -131739.54771468785;
    int TClqfbJzdubLQo = -871436007;
    string rAZaeao = string("OJOPNtJMoqnkRVIOBrxMNLzDLYgKDdWWgpELwtrhGpkGJIkdwaPskPtdTjbtPYtpYnXwHwQDGIipJpRUatJWAzffvrgAyQPjxKNlrzVQfBBnYSMIBRAITwulQQgzFKyFYZmjoUgxULOfjNOdz");
    string wzTasVSys = string("RUT");
    bool ABFpEQrKfe = true;
    bool QjhxOjDROt = true;

    if (kOxgsmZ <= -859142100) {
        for (int gsbMcgrO = 1387478315; gsbMcgrO > 0; gsbMcgrO--) {
            rAZaeao = GHKIPfHh;
            rAZaeao = wzTasVSys;
        }
    }

    if (IZfXxpbIHqqg > -859142100) {
        for (int eYTxiOsqsm = 1552685890; eYTxiOsqsm > 0; eYTxiOsqsm--) {
            rAZaeao += GHKIPfHh;
        }
    }

    for (int yAmtIRfUsWyg = 1647586188; yAmtIRfUsWyg > 0; yAmtIRfUsWyg--) {
        ABFpEQrKfe = QjhxOjDROt;
        kOxgsmZ /= TClqfbJzdubLQo;
        TClqfbJzdubLQo -= kOxgsmZ;
    }

    for (int tprLZuZprgAD = 413210688; tprLZuZprgAD > 0; tprLZuZprgAD--) {
        GHKIPfHh = wzTasVSys;
        IZfXxpbIHqqg = TClqfbJzdubLQo;
    }

    return glMhOvrvUdLSxOQn;
}

bool eZbaIwFIzwX::LORkURxQGPv(double SokEUY, int FqRhLI)
{
    int uYvVPtzzX = -529735099;
    double MIbLsAv = 266264.9311039148;
    string TajvGtgq = string("JdEqIcjqoNNrZkuRWTMHZBRtlBaZrUskLVBqeVXBvhWgjewdtdDbLBzIomqdPwRSjqRYKeMgblUsjQktmhjMIGPLNbVQzcGXeTXwoueUWzlrRDhtHdozGMTNfnHlvoeTKYnmwUyLMCeqSVEkXXyuksUUyNwIyhtsLzJDWeoQPWGoTJUUgybVOaXcPNQsruWKNlNKTrTgHMuKlObcibRbInePEnqfRGVgLK");
    string DhPyQLv = string("JIhfAlQFYEvqodJriqlZNrflQoARCNjiFiUfhNdFmBLBHdBZKGZHnqtEteHBidoNfTDhnhTHEWMROcGRQVdupNocbIBvlkDXrSOmjkXOjCilRDvBlwWOalWkYXPQhoaqMSODGHqnHdsQvfp");
    string MUFuL = string("YOdaOKbGHCiXNOYsLsvaooAIybsqsiUGxjLuwuLJJIFydAAnxbFEuiVxQJBAVWvBqfWVgAbqcCSshSAuCLgTKFqMQHJbPNcIfirhZOnWHXWmRlfNVezazukTyMoxjjjsBnRqLjNjmLAAAcYJEDBlWkkENjelBSxgrZCvsTJSCUEMtQUOKJHCshhydpYyFRnBDJZuHEcUloGsynwTTMEyfvTcsYCRRyxKRFylIrOoJKoIZ");
    double GPodfdnz = 281646.5489870486;
    int MTxxoUYysRzZixw = 719550723;
    string VmTjDSv = string("NXsfSLzbaAmZcosRodymuZLtObQyWLngcqSdLzamPpfylniQsiGCswYvepwyYYZnEQfKLfzpGkiIyJJqjepigLdZZVGzijDryNWBqNfZNQYgqAwAIfocxVkrVKRuOVqQmIQaDgnMOdmmKfUYQIVcyltfavmwJZxQfickwKQmTQENjKfDkhxAItVvtkJUlDERISOzebjAogTohJQXLKLXVufJbbgEmrJpSYibxRWpoGuIFeFRkeObvmRckGVdCAU");
    bool YlbOaYuIAE = true;
    double GQVDWnY = -1039087.1499987271;

    if (MUFuL <= string("YOdaOKbGHCiXNOYsLsvaooAIybsqsiUGxjLuwuLJJIFydAAnxbFEuiVxQJBAVWvBqfWVgAbqcCSshSAuCLgTKFqMQHJbPNcIfirhZOnWHXWmRlfNVezazukTyMoxjjjsBnRqLjNjmLAAAcYJEDBlWkkENjelBSxgrZCvsTJSCUEMtQUOKJHCshhydpYyFRnBDJZuHEcUloGsynwTTMEyfvTcsYCRRyxKRFylIrOoJKoIZ")) {
        for (int oTDVCjT = 1487615887; oTDVCjT > 0; oTDVCjT--) {
            MIbLsAv = MIbLsAv;
            DhPyQLv += MUFuL;
        }
    }

    return YlbOaYuIAE;
}

void eZbaIwFIzwX::wvBVDCv()
{
    double PKDFbcbfa = 874842.1459161518;
    string SeAOvrOhzXuxr = string("brurXQQqWKtzsoHphhYdparwFMUuwbpxibbTogZscYCMfoNRCdIibetskEDHQUKpBEgvVbjFAfwDyxDuKTAbhDAQfVWOZmOgsCOmsAlQohdJDyCZGMQFQ");
    int hCrnHw = 356131458;
    bool YCgyyrbTdj = true;
    bool ozbslf = true;
    bool wfHGxdjfNBqTgXbP = true;

    for (int MtYba = 600957111; MtYba > 0; MtYba--) {
        continue;
    }

    for (int oDwCReZzmsnOYx = 182319636; oDwCReZzmsnOYx > 0; oDwCReZzmsnOYx--) {
        ozbslf = ozbslf;
        YCgyyrbTdj = YCgyyrbTdj;
        ozbslf = wfHGxdjfNBqTgXbP;
    }

    if (YCgyyrbTdj != true) {
        for (int yMOxdGShbWiBRJd = 896446817; yMOxdGShbWiBRJd > 0; yMOxdGShbWiBRJd--) {
            continue;
        }
    }

    for (int WuhvRmjdQEefSwhM = 257672234; WuhvRmjdQEefSwhM > 0; WuhvRmjdQEefSwhM--) {
        continue;
    }
}

bool eZbaIwFIzwX::XMsQwq(int YaSKMhBoMxOJ, string VjzTHOajlPz)
{
    double NRonadURA = 612212.6784547834;
    int nwQIxWKUiOcG = 584484016;
    int viNeIRDGSoPab = -2001571395;
    string sxgFlrKjHW = string("bSLSOcjvRWpPaKIRWZkGYGLSsoutTwKjlikdVzysLsfZABUCjJfgoLeoOtdiHXhIgPdNGm");
    double oWLkofWUeUeUWrw = -1043525.5604150376;
    bool kftMib = false;
    int gEHRYjkbXqmMxFH = 1212034632;
    string HBeqiDUHJRwvx = string("QaMvsKIaofbhoqTpGBDnLpqurjLVTcvNaDWehWANVyybgvgfvFZMPjNgbaVSRnrWVKKdGovJuiDmgUfSUijrATRIDvTFlOxakNPArdEdmFJHpJXKslWTsyrONoUwKtWOsHIMocgsOIiDouQmLSqTXUEEfLHVdgcfARnLzlXDVKyNPgg");
    double vdbOXD = 1010833.7264999019;

    for (int qrIwIrNnJeyEzHL = 1415196353; qrIwIrNnJeyEzHL > 0; qrIwIrNnJeyEzHL--) {
        oWLkofWUeUeUWrw -= oWLkofWUeUeUWrw;
    }

    if (sxgFlrKjHW != string("QaMvsKIaofbhoqTpGBDnLpqurjLVTcvNaDWehWANVyybgvgfvFZMPjNgbaVSRnrWVKKdGovJuiDmgUfSUijrATRIDvTFlOxakNPArdEdmFJHpJXKslWTsyrONoUwKtWOsHIMocgsOIiDouQmLSqTXUEEfLHVdgcfARnLzlXDVKyNPgg")) {
        for (int kpmwJoUUyTct = 1675568073; kpmwJoUUyTct > 0; kpmwJoUUyTct--) {
            viNeIRDGSoPab = nwQIxWKUiOcG;
            gEHRYjkbXqmMxFH *= YaSKMhBoMxOJ;
        }
    }

    return kftMib;
}

int eZbaIwFIzwX::Gomrjqz(string cBUKTHWpEfTkuAVn, double FBIOzl)
{
    bool NkKJN = true;
    bool oTorkvWabRngdiOv = false;
    int dAIWyoUd = 1901598126;
    string YiMlPFNd = string("TAlgxHEnHUVUTaftPHFpvxGabFnUanneic");
    double kqapcUR = -1019687.6670490887;
    int ISFPEVD = 864066680;

    return ISFPEVD;
}

string eZbaIwFIzwX::MsjEuCBDhkJSNLi(double xZqDZYnxJ, string StdKkXjOd)
{
    int jprHAJxXjdkfPW = -181602935;
    int YAKxuyiE = 539470709;

    for (int tWnVbXBa = 611357855; tWnVbXBa > 0; tWnVbXBa--) {
        continue;
    }

    return StdKkXjOd;
}

bool eZbaIwFIzwX::mwsmEETCtICvdaS()
{
    string WLJppFeKImI = string("qHmLvUFBPrjmplmZkVRXtfyhAMONpRYXUUQJxxxahxvgpZiQGMwhuvcjVXFPnhrlIgkgvMLwhBckWuLdXcOLaEBZDVSydkQrnFdvzqiFetVhEyttHZxKgBkwrajFPxOAhrXQGhzUdVdafeHOTjrwIBkerrvZCxZNCWs");
    string sbrted = string("LGuSyUjzzGsZlMXwOaxKrsAgftFpeOUEUSEIaOUxcBdvCDVdVzLynztYtUwXogUKRLOLTjOYWWpLZUDvLhYLtMIBddPFFfdVndhECwCQEmwJBkiwtaRbIVbjWLsyBSLhfGJZZrQSoFgOcXippyxYItfMHieWKqBXQNKwZCvqBCxVbShTIMtdOSGNmJmFqCwLaOTBjmCmsmkAqbMngZRUKmAXywKfJnjUbaz");
    double EmTBTceSP = -902546.9652060613;
    double jdglBG = -921377.2696582709;
    double TJfuBIHcA = 561611.0479996378;
    bool uOsYe = false;
    string ooMkpm = string("VIIWDiPKVzatAAudkdNAiizDabzOtMFMahHWPxGjeKPspeUYGMwjLxNdmGEEACsQueAJQgRcArhAPVzXuXLjmEGJesnFxnENNbSm");
    bool OvxlkCTHtAavQDk = true;
    double TqpTzWFSs = -903155.8913557841;
    int tMOVewiFTZWg = -1737464196;

    if (WLJppFeKImI > string("LGuSyUjzzGsZlMXwOaxKrsAgftFpeOUEUSEIaOUxcBdvCDVdVzLynztYtUwXogUKRLOLTjOYWWpLZUDvLhYLtMIBddPFFfdVndhECwCQEmwJBkiwtaRbIVbjWLsyBSLhfGJZZrQSoFgOcXippyxYItfMHieWKqBXQNKwZCvqBCxVbShTIMtdOSGNmJmFqCwLaOTBjmCmsmkAqbMngZRUKmAXywKfJnjUbaz")) {
        for (int vLwkMvSWXriDxBZ = 1999728970; vLwkMvSWXriDxBZ > 0; vLwkMvSWXriDxBZ--) {
            TJfuBIHcA = jdglBG;
            uOsYe = ! uOsYe;
        }
    }

    for (int xwBDjJTYvG = 1777537498; xwBDjJTYvG > 0; xwBDjJTYvG--) {
        tMOVewiFTZWg /= tMOVewiFTZWg;
    }

    for (int CCKwfzsNorFtoy = 491592895; CCKwfzsNorFtoy > 0; CCKwfzsNorFtoy--) {
        tMOVewiFTZWg /= tMOVewiFTZWg;
        jdglBG += TJfuBIHcA;
        TqpTzWFSs -= TJfuBIHcA;
    }

    for (int XCyzHED = 2022531977; XCyzHED > 0; XCyzHED--) {
        EmTBTceSP += TJfuBIHcA;
    }

    return OvxlkCTHtAavQDk;
}

int eZbaIwFIzwX::EYcOXceLJCMVPdbA(int UZrQHLcDSYxVmT, int sSBKZVDmtRlKUPG, int ONXKR)
{
    double PLMrpZKgcNIfajx = 721290.6219797843;
    bool qgdXE = false;

    if (ONXKR == -1930378186) {
        for (int qexqSO = 141674542; qexqSO > 0; qexqSO--) {
            ONXKR += ONXKR;
            ONXKR += sSBKZVDmtRlKUPG;
            UZrQHLcDSYxVmT -= UZrQHLcDSYxVmT;
            PLMrpZKgcNIfajx = PLMrpZKgcNIfajx;
        }
    }

    if (UZrQHLcDSYxVmT > -412909653) {
        for (int JdeQWoNoMtRP = 307267640; JdeQWoNoMtRP > 0; JdeQWoNoMtRP--) {
            sSBKZVDmtRlKUPG -= sSBKZVDmtRlKUPG;
            sSBKZVDmtRlKUPG -= UZrQHLcDSYxVmT;
            ONXKR /= ONXKR;
            sSBKZVDmtRlKUPG /= UZrQHLcDSYxVmT;
        }
    }

    for (int QTdckZjZRmaPaTE = 1795468539; QTdckZjZRmaPaTE > 0; QTdckZjZRmaPaTE--) {
        PLMrpZKgcNIfajx *= PLMrpZKgcNIfajx;
        sSBKZVDmtRlKUPG += ONXKR;
    }

    return ONXKR;
}

void eZbaIwFIzwX::juWmRHInVFAulwMl(double mcCKVm, bool QdncSrpqOxNgKvY)
{
    bool uwvuy = false;
    bool oCLAbCYSZURrcoP = true;
    bool PIvsLc = true;

    if (QdncSrpqOxNgKvY != true) {
        for (int QPYeENDMOWivsgOs = 622574374; QPYeENDMOWivsgOs > 0; QPYeENDMOWivsgOs--) {
            oCLAbCYSZURrcoP = oCLAbCYSZURrcoP;
            PIvsLc = ! uwvuy;
            QdncSrpqOxNgKvY = QdncSrpqOxNgKvY;
        }
    }

    for (int WeAroGrLRUZQL = 795014550; WeAroGrLRUZQL > 0; WeAroGrLRUZQL--) {
        uwvuy = ! QdncSrpqOxNgKvY;
    }

    if (PIvsLc != false) {
        for (int PqsvaraisptcyLfh = 1239565528; PqsvaraisptcyLfh > 0; PqsvaraisptcyLfh--) {
            PIvsLc = oCLAbCYSZURrcoP;
        }
    }
}

void eZbaIwFIzwX::DVIkFGiFXJjC(double iPGKXHFFA, int GTihRTsoiHPbsj, string tVBmbax, string mQfKDNslzysPrLg)
{
    bool nMoxvDdeSAirANRw = true;
    double HzxmwuZeqy = 205776.6756648094;
    int azGXjqbQEjmJ = 1022280254;
    int VjHiBgPzADuz = -2120566752;
    string rJcSR = string("zyvgmYYYIQAFpuqmkmbgekNwJKhAAZXniujIAvmOAzGeIDLlrBghSeAhyUUowTtLoGLXAoHghzIpVUmYyTjnvQyZJeWbUgLxhWrjZcTIb");
    double xFgffUgzocpk = 744641.9937592719;

    for (int rcsTlEzNjXr = 1718986499; rcsTlEzNjXr > 0; rcsTlEzNjXr--) {
        continue;
    }

    for (int FPsco = 1797939603; FPsco > 0; FPsco--) {
        mQfKDNslzysPrLg = mQfKDNslzysPrLg;
        iPGKXHFFA -= iPGKXHFFA;
    }

    if (tVBmbax > string("lClZepAdYsnLvgTdChlMAvKzMQyJuEuaQeQrqfTEDJvZaTalVFvGmxfErGyxhBPstTSwOvqUwIGSIKcePujFlLQXFmjUHyoRWLXpVhcERktImrQMTudOTkCXnZTFEMaTJejchzjtinluEOxTGvlkewaPhxzrKGKnvPNsteFlIWdojSRSzRaPkOSAIaxRdtfSjHkFYbcDAMoFJbzMZtaThdUmFjyynxdummEiONGKufwbtc")) {
        for (int GAUmeljMs = 1062977040; GAUmeljMs > 0; GAUmeljMs--) {
            continue;
        }
    }
}

void eZbaIwFIzwX::gSpPkwOrvcx()
{
    int OtFhqh = 33576058;
    string GbhclsUTf = string("DbEnOBtIaczxgMhmNIWcDSKzgeveCVPtNOIUPhjjOvcRJGJwWMimYDRnHcLQYSydwowZhkmgRnSrMmQRPtQbqHRACKXEGdAdSDCdGYzlewVfEDVyXbcVlaYmWxFORLsDqCwJpZhHAoPBgwHFcwbNSDOMftJUfChpdmDNFlMMfqJzwGe");
    double yUbwJHgOFQ = -624663.8957556832;
    bool DwWIXKBPrBMStIp = true;
    string GnOlcThhHGnEQ = string("aNnBGmdVfRCgmxGlSSwgGFyMIdgKCOBJyAxEeZQcrnWwyUvIUnlijzYqDVHmPYsGKjUIbagJPtbuExlGGeqrwTyLGAXWheeFtIkhmiUyeZWorlPwAawnDVIUivHsGctkmNYkBwxKbYfPSJL");
    int TplEoneuXs = 1137712411;
}

void eZbaIwFIzwX::Gtkcrx(double afKJVHZdP, bool SsJqvRQRX, int YLyXX)
{
    int SXkYYdXGvVOFT = -1737345451;
    string iccdbeARFfQ = string("lNDyqJKewSoJuFXCqQIVoVfWhHEBVDEJIIIcughUFWTUPBhICvyUdSTWPdyjGZsEAzIPkjHlXDSUftqlpaeuxbgUQYuapHVZRdlVKwnvJIwLmrFiihkJNIcpsbEZgYe");
    int tWiqLIcxaZT = 666078216;

    for (int EGyyI = 710884603; EGyyI > 0; EGyyI--) {
        tWiqLIcxaZT /= YLyXX;
        tWiqLIcxaZT = YLyXX;
    }
}

bool eZbaIwFIzwX::WLeIc(double cFpleburTMZNEoo, string nlBsmJvCceZGeB, string xJNYkNl, string olTDPgKxhhigs)
{
    double VMfFfrwE = 974535.0308131425;
    bool IwLORXDVyf = true;
    bool icbKFHTFXk = true;

    return icbKFHTFXk;
}

string eZbaIwFIzwX::MwioUgbQ(string WvTNWfRz, double iYUVPSbM, bool JVJXLLnItlvnPOkM, int kkrDk, double mSKzmVMVVtvAYKX)
{
    string sjNFPyvABkOqDe = string("rxGwFvAsTyVqARRMDOwCADfmcFLDIKzFtZmEmonhcaQdIUODjIqyOTyFuoPGMfFkBsZiQRzkSnAqnFNPjOnSDIZbTkWlDKpYIBQtiMeGHrvkPPkrROHhLBiIEnfWfTxYZSBxvaCXG");
    double ugEmlApgTJ = -136333.87252867132;

    for (int mfkbQbO = 773499275; mfkbQbO > 0; mfkbQbO--) {
        JVJXLLnItlvnPOkM = ! JVJXLLnItlvnPOkM;
        mSKzmVMVVtvAYKX /= mSKzmVMVVtvAYKX;
        kkrDk += kkrDk;
    }

    for (int nyytg = 1339140241; nyytg > 0; nyytg--) {
        kkrDk += kkrDk;
    }

    for (int FQErQgGRTVVDBH = 189075054; FQErQgGRTVVDBH > 0; FQErQgGRTVVDBH--) {
        ugEmlApgTJ -= mSKzmVMVVtvAYKX;
        ugEmlApgTJ *= iYUVPSbM;
    }

    if (mSKzmVMVVtvAYKX > 624168.0634138242) {
        for (int IhhhtVfesOX = 1670915283; IhhhtVfesOX > 0; IhhhtVfesOX--) {
            ugEmlApgTJ += ugEmlApgTJ;
            iYUVPSbM -= ugEmlApgTJ;
            kkrDk /= kkrDk;
        }
    }

    return sjNFPyvABkOqDe;
}

string eZbaIwFIzwX::ZEcGbOZOzqbhvbS(bool vFEgWAJcxbzep)
{
    int oCDkxxHkwOIiWtEH = -1956736669;
    bool jwAoDpitMM = false;

    for (int hbLNOyJ = 1747699702; hbLNOyJ > 0; hbLNOyJ--) {
        jwAoDpitMM = jwAoDpitMM;
        vFEgWAJcxbzep = ! vFEgWAJcxbzep;
        oCDkxxHkwOIiWtEH -= oCDkxxHkwOIiWtEH;
    }

    return string("twHAakiAIWATNbSqmJhsIXkwcicoUVsDqPirqMARXLguxYrXeWCmaRgcgDnldfEHXMeJzlUbwPVIzjoTanOAmidPMZxuIDobArYgsDivTOIUgvenxGtBxwTFzQnTVfUdYKAhcAzaptukMuw");
}

string eZbaIwFIzwX::PlHsB(int fyzqdDW, string BvFwZOkYWtOduZH)
{
    double sHKRPEvTbjFEq = -139447.1465291553;
    string AJvqyItJlCKt = string("wcpAWzvlKUlbjzaWYHxPAcvgeQHsxLnXqvnZAksLNQboLalfghUgAXYTikcENrrpCiNKtaPontjeEzpPImbkKyUREftDtCOZeaEwxkIOjHumJVPGYPnowVjYBwxEsNoBdeihWIqupplpvqDlTMGgn");

    for (int pvuzaBXmJfvYaL = 1385880037; pvuzaBXmJfvYaL > 0; pvuzaBXmJfvYaL--) {
        AJvqyItJlCKt += BvFwZOkYWtOduZH;
        AJvqyItJlCKt = BvFwZOkYWtOduZH;
    }

    if (fyzqdDW <= 1975082011) {
        for (int WUPpIOV = 824885267; WUPpIOV > 0; WUPpIOV--) {
            BvFwZOkYWtOduZH += BvFwZOkYWtOduZH;
            BvFwZOkYWtOduZH = AJvqyItJlCKt;
            BvFwZOkYWtOduZH = BvFwZOkYWtOduZH;
            AJvqyItJlCKt += AJvqyItJlCKt;
            AJvqyItJlCKt = AJvqyItJlCKt;
            BvFwZOkYWtOduZH += BvFwZOkYWtOduZH;
        }
    }

    for (int JjhRORkFzhWdGoF = 201972360; JjhRORkFzhWdGoF > 0; JjhRORkFzhWdGoF--) {
        continue;
    }

    for (int LrFttp = 676138032; LrFttp > 0; LrFttp--) {
        BvFwZOkYWtOduZH += BvFwZOkYWtOduZH;
        BvFwZOkYWtOduZH += AJvqyItJlCKt;
    }

    for (int lHsEXERhgkzxIiv = 2021664004; lHsEXERhgkzxIiv > 0; lHsEXERhgkzxIiv--) {
        continue;
    }

    if (AJvqyItJlCKt > string("GXnZpmwgGALhONAVsOKNECcFbEpRdmQHniCkCpmfFPEDoa")) {
        for (int QJHnWqrG = 316041879; QJHnWqrG > 0; QJHnWqrG--) {
            AJvqyItJlCKt = BvFwZOkYWtOduZH;
            sHKRPEvTbjFEq += sHKRPEvTbjFEq;
        }
    }

    return AJvqyItJlCKt;
}

bool eZbaIwFIzwX::uCPVzwmfSroFKuL()
{
    string YWHgXlom = string("ZxshLVwpCUXrIVNlhIXMdYLkBDmxnlwoDzLvUyjnbmxYmVVnqKLwscakxchqiHtnSAYKvLRTPmvhFvhvPceiQjQTWZdLgOhjWUrAzxoJuZJEHnUmUEWRhlXTZp");
    double YqHSNin = 176247.8030224612;
    int CkEXh = 1905717403;
    bool gWtdYi = false;
    bool Qsfdu = true;
    double yACBIUI = 106575.11611764335;
    double jHLYvdhnHVy = -892801.3224258708;

    return Qsfdu;
}

bool eZbaIwFIzwX::zBFctwC(bool zITijubU)
{
    string hXTKtYxqyxl = string("SibijroyceUHTwZgiutTMkSHPwqsvfpkqPpYfVHKGSUfNvXSCBRd");

    for (int hOMsAT = 1740555814; hOMsAT > 0; hOMsAT--) {
        continue;
    }

    if (hXTKtYxqyxl <= string("SibijroyceUHTwZgiutTMkSHPwqsvfpkqPpYfVHKGSUfNvXSCBRd")) {
        for (int JvOUUVfcFEl = 1885057805; JvOUUVfcFEl > 0; JvOUUVfcFEl--) {
            zITijubU = zITijubU;
            hXTKtYxqyxl = hXTKtYxqyxl;
            zITijubU = zITijubU;
            zITijubU = zITijubU;
        }
    }

    if (hXTKtYxqyxl > string("SibijroyceUHTwZgiutTMkSHPwqsvfpkqPpYfVHKGSUfNvXSCBRd")) {
        for (int ABjyTJr = 2132814372; ABjyTJr > 0; ABjyTJr--) {
            zITijubU = ! zITijubU;
        }
    }

    for (int gjkpBVIwXAoD = 399160880; gjkpBVIwXAoD > 0; gjkpBVIwXAoD--) {
        hXTKtYxqyxl = hXTKtYxqyxl;
        zITijubU = zITijubU;
    }

    if (zITijubU != false) {
        for (int VEPZYwpIgkDdfFP = 1825498586; VEPZYwpIgkDdfFP > 0; VEPZYwpIgkDdfFP--) {
            hXTKtYxqyxl += hXTKtYxqyxl;
            hXTKtYxqyxl += hXTKtYxqyxl;
        }
    }

    for (int cGtdEwwVRuqwWui = 2130283161; cGtdEwwVRuqwWui > 0; cGtdEwwVRuqwWui--) {
        zITijubU = zITijubU;
    }

    return zITijubU;
}

eZbaIwFIzwX::eZbaIwFIzwX()
{
    this->oZBSGpfZflGIYDi(-859142100);
    this->LORkURxQGPv(702243.8617159533, 570233300);
    this->wvBVDCv();
    this->XMsQwq(28377814, string("ogpUvkcITUHhbsQxkLssSGYohGKxEYLrWoBrtIozcObPGmehMpyMUyGoJNtExIopvgmgbJIBmZajIGwOGyuKjRkXnlUvuViSIfsBmHpPdtRsnuAsWLnFNEUpaSWAtOPquGJYWqDuQsjZCfAYbHhikydDFDOrOTkAVQwiMgkeXvUxGRwNegarELepSszaABRcEBCMCXbpzLBooXXASuDbYqBFwdVI"));
    this->Gomrjqz(string("ryYCgjdpgvyNCtcpInTtFSewxCgetCYcshtVVdfoMwupbwHGRxnVSBGkBghHqDtYjLbiNZKTuutJKedVZRmwmrpBgwBtVklPzhn"), 673045.8892141286);
    this->MsjEuCBDhkJSNLi(-480896.80996866897, string("hULfgSyDdabVivjtPCjCVPLKTTJjciCbCyOzKIlMaQYWyLhGphnyIavNAyyLSKmNjbwqBvEUPwMWSEcvEpJmKmSvCADZfVeNKllTRrYlwvgCzVmQCaPniBhaLyssmKrzaIxhJKRpdOzoFtSP"));
    this->mwsmEETCtICvdaS();
    this->EYcOXceLJCMVPdbA(-1930378186, 220642662, -412909653);
    this->juWmRHInVFAulwMl(-706908.088460143, false);
    this->DVIkFGiFXJjC(955850.2433554414, 332947649, string("lClZepAdYsnLvgTdChlMAvKzMQyJuEuaQeQrqfTEDJvZaTalVFvGmxfErGyxhBPstTSwOvqUwIGSIKcePujFlLQXFmjUHyoRWLXpVhcERktImrQMTudOTkCXnZTFEMaTJejchzjtinluEOxTGvlkewaPhxzrKGKnvPNsteFlIWdojSRSzRaPkOSAIaxRdtfSjHkFYbcDAMoFJbzMZtaThdUmFjyynxdummEiONGKufwbtc"), string("aOHkXcpDMFxkYxfZTGTMSoEksKCjsLNYegDeLFRlaVrOOOxLPDwfjPPaSjdgqwJOhShJVwYmkVmpCLrLZPIMRKFnokOjTlNqsYWBgvoSPclZRWMDErGHxvGCBdeAOCxMRlfjQcjkATYLsskMkYExaDTjSNcdumvBLvwNZPQEMssRqkFWrISVYXFEHwiLYtreJdYNCESZlcPNfRhIWl"));
    this->gSpPkwOrvcx();
    this->Gtkcrx(79225.32677555962, false, 1083619655);
    this->WLeIc(699655.412022279, string("YjbtOhfhlPZQUxRruGkjgUFIpQxyFPYcooyTlwBPoxsJvEAQwJuBuzFsNJYrqqnEdbmMhmEantuJzjVQCAtBfYrWqsPxaOIJUjUzjaVPpmbTYZGUdEebUMTLPTnJqrZKrtwJCtQYGKENNrInQcraakfCZzHSDnhSKHTkNUSefgTsCbgSvSvFbIyvdpAnmvgrMoyxGRallVMz"), string("DLylJQLRyqvwfrUpvispCUUPWFvbyTdgNxSJhKygdFVxbsqCWwoPsgQWYZwpykzlcybHfSoeJlycrmEEwDJnauNeBBnaSHAtcXEjUEdjaqVjmFUfROBVeGePIfUzpWQJa"), string("oCCtIlfPDsXmUJUdyemugcYkikBsnzmqIWpphtfnUbhWhAogfAOSKsnoMsUkyiwvnQCJresJOHReLBkNjEwfsQamqrafHTGiiKJtuZSIdSVrIbecUndQnccNYQWjrbmMJIWKsouDBNfHnWKOxByFqpIAZJbwqVpkmQncMYgdxoSXhXEAXvgnXofWymSYO"));
    this->MwioUgbQ(string("jjkWeEyMwzxuNCJYjOKIlXTGNEBDNscGDBDayCUbGDOr"), 458671.9826856543, false, -159821194, 624168.0634138242);
    this->ZEcGbOZOzqbhvbS(true);
    this->PlHsB(1975082011, string("GXnZpmwgGALhONAVsOKNECcFbEpRdmQHniCkCpmfFPEDoa"));
    this->uCPVzwmfSroFKuL();
    this->zBFctwC(false);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class POshdBKHXJfBz
{
public:
    string JDSDGDmwapBawPM;

    POshdBKHXJfBz();
    double nsDVaxW(double gYdQHrSdK, bool NQLkqEMTfMOdS);
    void JeNsRFBf(bool SLnyzA, string yCXJHmaAdHRqCb, double jlQJietVMMifmjK, string BjBdwokxqlDHYc);
    double aMBgFUg();
    void mmowIsZZye(int mGNrGX);
protected:
    bool TWFCpIMmR;
    string bFDOLUbfON;
    bool oYwPlJscJBRnPIUA;
    double hSBhmuJEmugryI;
    int TlBdomJjkDthe;
    string jlDRUD;

    void ReKbpphqkZJztsLB(string QTJEP, int YgiJiUfTxWF);
private:
    string KVjUhOZuEOCt;
    bool DmnJkXKd;
    double oNNwVRYeNbFMI;
    string fDKrFtDk;
    double CoHwCaugVBjp;

    string OFocOLVqIILluBb(bool LamgHnjPEx, int HZMCXCwYO, bool DiruVwPggjhUGwu, double eyOkLhy);
    int cMirvHkU(string yKgUbIHeqTUut, double cWavsjh);
    int jidroCqOwgXoyIBr(int lywMtpjf, double SepqNsYTzlF);
    int wdWOKtiybdnNk(double WSqWvWiOnwgJEqu, bool VrnZSHpZNNKe, double wDEQZCOYpHpqKhLK);
    void tTYeULVqIGs(bool yBmCGJq, int EmFxQBJ, double EuDXCuqLGP);
};

double POshdBKHXJfBz::nsDVaxW(double gYdQHrSdK, bool NQLkqEMTfMOdS)
{
    int GjAfFaigcLFUxMge = 1774337336;
    bool eOFFkCJQS = true;
    double eHUtgUr = -780465.286378934;
    double dJRhsRrrVL = -268635.4620784872;

    for (int sxggO = 10986715; sxggO > 0; sxggO--) {
        gYdQHrSdK -= eHUtgUr;
        NQLkqEMTfMOdS = NQLkqEMTfMOdS;
        eHUtgUr += eHUtgUr;
    }

    for (int HnYCH = 1193376508; HnYCH > 0; HnYCH--) {
        GjAfFaigcLFUxMge += GjAfFaigcLFUxMge;
        dJRhsRrrVL *= eHUtgUr;
        NQLkqEMTfMOdS = NQLkqEMTfMOdS;
    }

    if (NQLkqEMTfMOdS == false) {
        for (int YBCTYLAEiylOZzHn = 165442009; YBCTYLAEiylOZzHn > 0; YBCTYLAEiylOZzHn--) {
            eHUtgUr *= dJRhsRrrVL;
            GjAfFaigcLFUxMge += GjAfFaigcLFUxMge;
        }
    }

    if (eHUtgUr <= 975553.863020884) {
        for (int KjUJRvcWc = 322881555; KjUJRvcWc > 0; KjUJRvcWc--) {
            continue;
        }
    }

    return dJRhsRrrVL;
}

void POshdBKHXJfBz::JeNsRFBf(bool SLnyzA, string yCXJHmaAdHRqCb, double jlQJietVMMifmjK, string BjBdwokxqlDHYc)
{
    int yIVnWaTt = 2042449060;
    double TsdXu = -27451.025891767782;
    bool vRZXGuduLkzHs = false;
    bool zsQtBZAhwDxphz = false;
    int QoTXxG = 778274300;
    double hcWmrUmlqT = 973701.0996824813;

    for (int lQwZibHcQoYM = 357444047; lQwZibHcQoYM > 0; lQwZibHcQoYM--) {
        TsdXu /= hcWmrUmlqT;
        vRZXGuduLkzHs = ! vRZXGuduLkzHs;
    }

    for (int YLvylvvuO = 2096921789; YLvylvvuO > 0; YLvylvvuO--) {
        BjBdwokxqlDHYc += yCXJHmaAdHRqCb;
    }

    for (int EpLlkZ = 235226245; EpLlkZ > 0; EpLlkZ--) {
        yCXJHmaAdHRqCb += BjBdwokxqlDHYc;
    }

    for (int KxVRcQjNrQvw = 1599523712; KxVRcQjNrQvw > 0; KxVRcQjNrQvw--) {
        continue;
    }
}

double POshdBKHXJfBz::aMBgFUg()
{
    bool IVycd = true;
    string cIUhapkAFB = string("ogKABzxfbAxoxJiFEMFbmNndNiBKIvHLGmNiOjRzMitq");
    string fMNGhTSeLrndTQEw = string("MqWmdpKBkSVSJpUBQTIORljFGnOEOGXAYzGWehRjvorOViOyoHCkzHDhNYJAIEWDbUdsMpPvQQVVSwSATomuktuBWQqwQRwXrzaHymVSCazSAwdUkZXgVMQWoclRuTYUqoJDsHUFmRYiksoERxWemciwjsNnnypMfpxhRgUiMUnmUuxHFqSMhtpgmpEPYOxUqLFVYDmZCYaiDyeAuZvSwdEkJVUzoEOoTzOeJmrAGQbOMVLROidREtGWzrShu");
    int dxPvUWtzI = 2121864033;
    string hlOVhGKUNrwTyqU = string("VQFzUXuFYCUfIcSanowUPIPjjftYRBqEyksdWgXMpdDKJwcnfEuyNfFFVOJKyciGmFnGYxihfTouMRdhLnHmedKmbFbrhHAhukasAKCqZjftvkFAZsihquRhJIjRAiuSMYYEqhRsrOKGRIieEGLMSBAUKrIWDciejafNHKeQZsUpWAqsxhjKiULyiFlcYDcCdVDBDfvWvlWvHZVwBhUSAUOVRJeFndSuPCXpAxRJjvpaVEVkLH");
    int ZVuirNXREVBA = -30787622;
    int meyuljK = -456255083;
    bool DYLjSMTo = false;
    string CJvjflrXcmfMDMe = string("BykfoDLezwiamZUBQvAAsHrjizRwijzFmeWzNDNpuOUVomZxXJYjODptliBqLUzSfnNvfDmWwenKyyEXXUzsvwMPpbjJrKxEDaCdZSerDPESaBF");
    double FurneXNsUU = 801777.1140379251;

    for (int rzAIZhybXBuyEsY = 1172840152; rzAIZhybXBuyEsY > 0; rzAIZhybXBuyEsY--) {
        continue;
    }

    return FurneXNsUU;
}

void POshdBKHXJfBz::mmowIsZZye(int mGNrGX)
{
    string sotMEiI = string("BspSHhGXSTsvqTMW");
    bool wpibbwpcdjXrgx = false;
    int IMldNMKdWQNbYvo = 500180417;
    string DGiWeNXEPkzP = string("zejMhjaYuvDMtYneGENjFqwlHpDaxTVvaTjYuKtWMgbagkKVvGDLba");
    double VJTJBtFZCwLYioa = 179291.01119292947;
    double jxiYhdUm = 56502.29349554314;
    string cDPAFdkt = string("RzHJwHAFkRRHJNlyAAkNJkAXgzaIDngQOlMaZUUeRsbcetisKVFwNrTP");
    int mxuauKyB = -982052416;

    if (DGiWeNXEPkzP != string("zejMhjaYuvDMtYneGENjFqwlHpDaxTVvaTjYuKtWMgbagkKVvGDLba")) {
        for (int rBdilHqUAENsvZX = 184600242; rBdilHqUAENsvZX > 0; rBdilHqUAENsvZX--) {
            DGiWeNXEPkzP += sotMEiI;
        }
    }

    for (int cKvAfd = 1180561966; cKvAfd > 0; cKvAfd--) {
        continue;
    }
}

void POshdBKHXJfBz::ReKbpphqkZJztsLB(string QTJEP, int YgiJiUfTxWF)
{
    string JRtdFYrdON = string("RVxrzNLQCOpaZDKCcaiVFJJxtYULlYKrcFZmBpoljWloZTvIVZoPjracVmlBsnqoQZHFETYOFBKPdVQzraVMPcogMikGvqvbMwPEfnRTxrUGabIbZIafRojKAyvOkxlGVxncCVCQDleQCjXyqeVXUxIxvKmYJWAwLHgvEzQaHWjFJjjChb");
    double hTKNEeOJdrhCyGdT = -262297.2671948053;
    string bKJyeGMs = string("jJyEnZJKorceCdFYRSHROHtKDdSDIXYIqSOMtenYCOBtYVxgeAAhzvSWObxgYfPMuexwd");
    int kubodOWDbsRyj = 420334030;
    bool AbCfXlt = true;
    bool ZknyUT = false;
    bool ewbHuLRO = true;
    double Ipnzq = -313016.2060262418;

    for (int syjtxYk = 123789312; syjtxYk > 0; syjtxYk--) {
        continue;
    }

    for (int CVWsErOx = 735924817; CVWsErOx > 0; CVWsErOx--) {
        YgiJiUfTxWF /= kubodOWDbsRyj;
        ZknyUT = AbCfXlt;
        AbCfXlt = ! ewbHuLRO;
        bKJyeGMs += QTJEP;
        bKJyeGMs += QTJEP;
        JRtdFYrdON += QTJEP;
    }

    for (int qRzdGUC = 368439573; qRzdGUC > 0; qRzdGUC--) {
        bKJyeGMs = JRtdFYrdON;
    }
}

string POshdBKHXJfBz::OFocOLVqIILluBb(bool LamgHnjPEx, int HZMCXCwYO, bool DiruVwPggjhUGwu, double eyOkLhy)
{
    bool fMTiVObgHv = true;
    int OhCbPkqkctPUe = 1922983146;
    string SdmylJOfjwnV = string("qNgPYCPGMcKuyhHCTpJUnPQUJVOoGnLEwpNOxDXyhwusTjCBHHSBnRAvwFWAQpbAcaCQPdexeBhmLsPwOSyAdzGXPYGbalKjEvMmZwDdhSlEKiTNOhkSRzGvHonlRuCnPZumwSzKqBVxtcfdsknVNOtmOhsRrdlZQDiYhlblMUIGUCbozVdoKSnkKRRNUHH");

    for (int IVwueZEBQ = 1008984843; IVwueZEBQ > 0; IVwueZEBQ--) {
        OhCbPkqkctPUe /= OhCbPkqkctPUe;
        SdmylJOfjwnV = SdmylJOfjwnV;
    }

    for (int bsJYlaD = 906433096; bsJYlaD > 0; bsJYlaD--) {
        LamgHnjPEx = LamgHnjPEx;
        LamgHnjPEx = fMTiVObgHv;
        LamgHnjPEx = DiruVwPggjhUGwu;
    }

    if (HZMCXCwYO > -446742614) {
        for (int xSZpoUp = 141346917; xSZpoUp > 0; xSZpoUp--) {
            continue;
        }
    }

    return SdmylJOfjwnV;
}

int POshdBKHXJfBz::cMirvHkU(string yKgUbIHeqTUut, double cWavsjh)
{
    string VJlyCDznSxCYWeb = string("oAXjlHXegHiNkrXrzhAYraODloCbUwzjKpJEfuberuqFRtNgOTOihmrydYACoAkDPkkbwVnwNLVEuhKdFluzeLtkoexexxCNnyJIoeWGLOfpGTdPVqpwqDBvaXtveKGdeiGNQYGuGpqpDjweUGhToGuRHfuTsdCbDpWsvu");
    string ycGyBgQNKSLMTW = string("EvqwFGwfJBzVRtBlIfYpkVNBVCZvqVIWkDgqKDBTyfMGCspHVlXTflcxbmGLxTCfjGBbzIOQSfMvrgZpxEkHkePQkqFXnojqjSFDODusO");
    bool RkFakUOfK = true;
    double pqkLuWIngi = -961743.5281853679;
    double PMgUBk = -1014196.7193138462;
    bool qazlCasIevBMN = false;
    string axhcSBKQQIyHq = string("htOCyUlQRDPSzoNnKTrKXjbUgnIhRbavBJbkCdEDwyZMXDsfgnvzdJHUnLIRCpQdbeCiQEsSUoHIRS");

    for (int TdyBZKj = 1489038565; TdyBZKj > 0; TdyBZKj--) {
        yKgUbIHeqTUut = ycGyBgQNKSLMTW;
        yKgUbIHeqTUut = axhcSBKQQIyHq;
        VJlyCDznSxCYWeb = ycGyBgQNKSLMTW;
    }

    return -1701808765;
}

int POshdBKHXJfBz::jidroCqOwgXoyIBr(int lywMtpjf, double SepqNsYTzlF)
{
    int rQEoIwrZIto = -905336575;
    bool FYQeuQ = true;
    bool SGFtDBsjLLGzO = false;
    bool xfprMcShHrXroP = false;

    if (FYQeuQ != false) {
        for (int SMvyNJh = 901825660; SMvyNJh > 0; SMvyNJh--) {
            SGFtDBsjLLGzO = FYQeuQ;
            FYQeuQ = SGFtDBsjLLGzO;
            lywMtpjf /= lywMtpjf;
        }
    }

    for (int bRqBhYT = 1392359165; bRqBhYT > 0; bRqBhYT--) {
        SGFtDBsjLLGzO = SGFtDBsjLLGzO;
        SGFtDBsjLLGzO = ! xfprMcShHrXroP;
        xfprMcShHrXroP = ! FYQeuQ;
    }

    if (rQEoIwrZIto >= -905336575) {
        for (int AtVJsP = 358588804; AtVJsP > 0; AtVJsP--) {
            FYQeuQ = ! xfprMcShHrXroP;
        }
    }

    if (SGFtDBsjLLGzO != false) {
        for (int kyYoiqVq = 488199015; kyYoiqVq > 0; kyYoiqVq--) {
            FYQeuQ = xfprMcShHrXroP;
            xfprMcShHrXroP = FYQeuQ;
            SGFtDBsjLLGzO = ! FYQeuQ;
        }
    }

    for (int IRhSTDRBfMTzIT = 791209910; IRhSTDRBfMTzIT > 0; IRhSTDRBfMTzIT--) {
        FYQeuQ = ! SGFtDBsjLLGzO;
    }

    for (int cfFFEknJpcmxT = 644797059; cfFFEknJpcmxT > 0; cfFFEknJpcmxT--) {
        rQEoIwrZIto = lywMtpjf;
        rQEoIwrZIto *= lywMtpjf;
    }

    return rQEoIwrZIto;
}

int POshdBKHXJfBz::wdWOKtiybdnNk(double WSqWvWiOnwgJEqu, bool VrnZSHpZNNKe, double wDEQZCOYpHpqKhLK)
{
    int cnJvFJviCDSErGGr = 1238890853;
    bool jvCeBUoH = true;
    bool QWEabBbvXk = false;

    for (int ciVEVhfH = 957241124; ciVEVhfH > 0; ciVEVhfH--) {
        cnJvFJviCDSErGGr = cnJvFJviCDSErGGr;
        VrnZSHpZNNKe = jvCeBUoH;
        WSqWvWiOnwgJEqu /= WSqWvWiOnwgJEqu;
    }

    if (jvCeBUoH != true) {
        for (int pYtQQSjBugsaXL = 1850349013; pYtQQSjBugsaXL > 0; pYtQQSjBugsaXL--) {
            continue;
        }
    }

    for (int cLqvAe = 1452401213; cLqvAe > 0; cLqvAe--) {
        cnJvFJviCDSErGGr -= cnJvFJviCDSErGGr;
        QWEabBbvXk = ! jvCeBUoH;
    }

    if (QWEabBbvXk == false) {
        for (int jcblYa = 1579280972; jcblYa > 0; jcblYa--) {
            jvCeBUoH = ! QWEabBbvXk;
            WSqWvWiOnwgJEqu *= wDEQZCOYpHpqKhLK;
        }
    }

    for (int HvvKZTpprWcjeEzs = 913922699; HvvKZTpprWcjeEzs > 0; HvvKZTpprWcjeEzs--) {
        QWEabBbvXk = ! VrnZSHpZNNKe;
    }

    if (QWEabBbvXk == false) {
        for (int cFmdnYLBjKx = 1362159484; cFmdnYLBjKx > 0; cFmdnYLBjKx--) {
            wDEQZCOYpHpqKhLK /= WSqWvWiOnwgJEqu;
        }
    }

    return cnJvFJviCDSErGGr;
}

void POshdBKHXJfBz::tTYeULVqIGs(bool yBmCGJq, int EmFxQBJ, double EuDXCuqLGP)
{
    double LZwOKF = -262292.9091923604;
    string PSjBVWNn = string("ogzpyJuVzeLGweyFJFGnxEBezsscbzTqPBJkzYSPCohgBtlsyHRFwoalKVeUvpIehZPnMHmplMnjfmsUkyhOTCrAyGbo");
    bool rfOaWn = false;
    bool GmGob = true;

    for (int lpRpvqsqYFZFgy = 970511367; lpRpvqsqYFZFgy > 0; lpRpvqsqYFZFgy--) {
        rfOaWn = rfOaWn;
    }
}

POshdBKHXJfBz::POshdBKHXJfBz()
{
    this->nsDVaxW(975553.863020884, false);
    this->JeNsRFBf(false, string("ePwAXMpwQXbffJcbXqgcthjqgzsCdrQNNtJXkyzLxqXbrbsWvjIqgpkLhyGMHjdJmjiSzvbfNYcXxqfDUEDdiqLkgXYjkulSfueccjkpmWDqSLwRmExfdxEoCBdESKZpDyoedxdOUuSZoPXyOeaHuAnPzzfymtHCNsCgdviKzlzFFmkAcljLpDqkzVJifGniHNVUISjIf"), 573788.0275095036, string("KnkIFtjtnxzgUvOgvlNmPCxKnviicReTFSeSOjiCLvxfgVLlfZDxsVUkPUEgbgBjnTtLMVOSDAyIAlNWlZCHCMhLztqOJsPiBwXRGvTnJSyymCSnuzKAimIGwojinABurOWoOdNcqfBcMLawLpbtltiOgOGrOjfjymbTlrRbzSUbQTzDhFhNGBcpDfEXIDPqysopSkiNvlqMQrSJOpQNjllVc"));
    this->aMBgFUg();
    this->mmowIsZZye(-1693188463);
    this->ReKbpphqkZJztsLB(string("qKBeFyBRRIiJ"), 1152910626);
    this->OFocOLVqIILluBb(false, -446742614, false, -97982.32052028977);
    this->cMirvHkU(string("KFvccZPPPGLatPoaedWPPLHJtPNpC"), -650757.7269058455);
    this->jidroCqOwgXoyIBr(389343670, -480180.76016547746);
    this->wdWOKtiybdnNk(475760.604675961, true, 82991.86164437317);
    this->tTYeULVqIGs(false, 1198289063, 677999.337540052);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class tMzfd
{
public:
    double OMxIaBolQhmJc;
    double fLTEiCptIVkhdZ;

    tMzfd();
    int OsMXFmK(int LYDkEYkNw);
    int JbLgUGgtBPPQ(string vVjBNqB, bool LLaDGRbHV, double WKBLLg, int XAYMRgeEii);
    bool TigIucvTbBCh(double vhUmyNeuHc, string UegRJSRKIjnd, bool ZiNIClOV, bool ZYLVVVAtDkOhaPc);
    string tEnGpahUN(int uKijyeOYzz);
    int euJDoCpkwjuyNTkH(string NULyMJTxBDIQsMMc, int QFwqqXYxHJAZyK, bool oyQZeODsMkryb);
    int DknCjwdPMgCLqX(string JlgVGIaVW, bool IqyKAR, int Upxamh);
    int vEFcHTx(double DlVyoThEx);
protected:
    string skPVwSusa;
    string SoBgtoXUBDNu;
    int eFRWU;
    bool dqhVipAxQQOW;
    bool oxdrTStUCs;

private:
    bool iwtRQk;
    string BUcRm;
    bool ZHMpmap;

    int mPSqigVLiUbWM(bool ZBoDjMysidBKfwR, string BjzqkWhvlgexhIxl, int wUhnK, bool aJClImdiih, string bAToIu);
    string DtTyzdvjVsL(bool DZYqpFlhciLDpig, int xqIDbjK);
    int hYSVO(int niapGVtcGORuZ, bool wGwqSzlx, bool RIveAsh, string qFsToiJvN);
    int IMZzHQwJsGyRxs();
    int WXLgyeXRPHYWHf(string huobkrNwWe);
    double RMekUIPqq(bool AHDxplJpmYx, string WaNBmsbSMJtvCmKG, string jXJliSZbwiFq);
};

int tMzfd::OsMXFmK(int LYDkEYkNw)
{
    string PwKpZQWM = string("hZdTCnbgsccVCzLqryBWZynagmKahrkxLlUvaqWtgidIvozCSXdElwxDGKOHCFYdMlBUinnKIpiqGuyGfZpxXfafYDGquvfPerNBjBytdGsvllQhVluZlyNQtUWVCoZjwSPaaNIRYLtHeoPdwyEBOxDvusRCjxofOjgsgGoICVHOlmELLaVUbZXrpcUsJjvzjNkOZnRvUx");
    int ExpYlY = 1574152665;
    double xZwxoTLdH = -951066.7087214448;
    int uDFDHLaTUPcoj = 639087422;
    bool zcqgw = true;
    string CcvuWCpUq = string("cvllLhPlxPBGCurtqQYUBaWtoBoOgXMlFNMkNVwhcyqPsVaMhUHIpHFUpJIASzcmFAxdoHarpuXSgsAcfusOdtOg");
    string vsWCSbDdwdSbIj = string("dFZGRaOjATJMXJVkNFCuUWMqiHwVkPGPnekAADqZodvNTCdhXSrFGTIBshSpyzCPGltzGzYMCzkeYVSvhxQOBoQdmshrZnYGXnGyFictiJXalZMApYbFfRKGwmScEWEheIqlXdDOfAkUshIMDkhDQFnDuTAqtTaCBwVBMytwZsngtlpsKUVjBkpaoDCALSoXCYHiNgjlPmtSvJvzfurrtligKKDCZcpOLpExBfhlDAJIgxIZjzqqMpDGJnMRog");
    int NxCYxxPnFLjA = -1247840365;
    string EvJNSbTbwKAEU = string("KHABlHvVBPmFRvRiNYWrQqJbomyJATbfzzHEQhNQvQjimZckCICdRwltrnIzOpipXbHORRkATrgbTAdqLtdoXmdnINWrzETEcoftBpcVrLItBdVXmOvInphZXunlZwHjQEucoQyc");
    double VzYnjgI = 928843.7828412323;

    if (VzYnjgI < 928843.7828412323) {
        for (int OWuyj = 1980247364; OWuyj > 0; OWuyj--) {
            LYDkEYkNw /= ExpYlY;
            vsWCSbDdwdSbIj += CcvuWCpUq;
        }
    }

    for (int pHDHFdc = 2001424390; pHDHFdc > 0; pHDHFdc--) {
        continue;
    }

    if (NxCYxxPnFLjA < -1247840365) {
        for (int eZZwIj = 183463667; eZZwIj > 0; eZZwIj--) {
            vsWCSbDdwdSbIj += vsWCSbDdwdSbIj;
            vsWCSbDdwdSbIj += PwKpZQWM;
            uDFDHLaTUPcoj /= uDFDHLaTUPcoj;
            NxCYxxPnFLjA -= ExpYlY;
        }
    }

    for (int UzoIWed = 2032419014; UzoIWed > 0; UzoIWed--) {
        EvJNSbTbwKAEU += EvJNSbTbwKAEU;
    }

    if (xZwxoTLdH == -951066.7087214448) {
        for (int PIAPOb = 1041667025; PIAPOb > 0; PIAPOb--) {
            PwKpZQWM += PwKpZQWM;
        }
    }

    if (NxCYxxPnFLjA >= -1247840365) {
        for (int DxAVTdUdpTPZi = 42858432; DxAVTdUdpTPZi > 0; DxAVTdUdpTPZi--) {
            continue;
        }
    }

    for (int QVrUTkicDAWnbl = 1928968088; QVrUTkicDAWnbl > 0; QVrUTkicDAWnbl--) {
        ExpYlY = LYDkEYkNw;
        uDFDHLaTUPcoj *= uDFDHLaTUPcoj;
    }

    return NxCYxxPnFLjA;
}

int tMzfd::JbLgUGgtBPPQ(string vVjBNqB, bool LLaDGRbHV, double WKBLLg, int XAYMRgeEii)
{
    string RJFYOsIuqDMlU = string("ukvKYhSzyWnCfHteQBXjByQkOuMxZYXbFeleTrMuHZUdwTJASRCxESYmBolvnntUNhszVMzqEqqGcaNgChRyoPAIrjlNWNmknGTGKZMMBnoXruBhFxnOZARXaWjJqSFBqsCBBoCbXBkrBJwPAYbHKkrYxYumNzpsGQigmLHpOooWfJWAjrfOVwKSyjPTMgxwOx");
    string LqCMdmaRiww = string("CDOkRWvPYLXJUNcREeCxrtsShMkoQcLeOXdESkxiPDlhLsXpHrYZgGnhnUnZKimpnXujScdoWmjDqqhJOQuMWGIToNJOijncJTyOZiMBwIpQYTUweTxVSZNbqNxqvKAaGNKcsPDmkeXlAcFsnfvDnpEoAvexTqgMxxXnnVWRXTunFWShoyIvULzLLqzmdJkqccszqpzMrZzlaYjhbzDJJskhCEiTAlJU");
    double FzYhRkqRzaKSPnj = 515485.3259700971;
    string bJFRtwL = string("IaoqFGzVZEHqRXRkdCSZhDpYJzuchwUjlpRBpzKsrVRjCNLaZvTbcEBeSIdRYorswaHjKmeLjAFyamYsZGhEZoYulLGMMIGiEoUjNlQAkGYzAGTEwmzajcKOXjBCRAINefjxtTdgwhhNvnSUPjqQKqJaxUCCgrfldkukzgUzdvjnLfNHLctLVzrFmHNLJolEoGWHyxtdicixspjQmpsHApJJuYLHAqyuexs");
    int PYelpYqFAAkWPnY = -1301611127;
    int iPGYxf = -1428119623;
    double ufqLuvCSqmhel = -192542.53145536355;
    double JOmnlImNGjevN = 657564.4209694257;
    bool SbFkvOhpfMccFz = true;

    if (ufqLuvCSqmhel >= 657564.4209694257) {
        for (int YqcelvTPsllg = 1136736074; YqcelvTPsllg > 0; YqcelvTPsllg--) {
            SbFkvOhpfMccFz = SbFkvOhpfMccFz;
            FzYhRkqRzaKSPnj += ufqLuvCSqmhel;
            JOmnlImNGjevN -= FzYhRkqRzaKSPnj;
        }
    }

    for (int VRaBPvauuxZIXsZJ = 678992213; VRaBPvauuxZIXsZJ > 0; VRaBPvauuxZIXsZJ--) {
        PYelpYqFAAkWPnY = XAYMRgeEii;
        JOmnlImNGjevN *= ufqLuvCSqmhel;
    }

    for (int raTxKVbodL = 1689039156; raTxKVbodL > 0; raTxKVbodL--) {
        continue;
    }

    return iPGYxf;
}

bool tMzfd::TigIucvTbBCh(double vhUmyNeuHc, string UegRJSRKIjnd, bool ZiNIClOV, bool ZYLVVVAtDkOhaPc)
{
    double MwbIfse = -584003.7940293576;
    string yOVLHRHoCiPUD = string("uWfdNBrvjaMJdAnJDJbKPPTAXEkswFQSDJfcoWUkDNrEvWjjOuIPgTcGKfaZreGwfcanpwymokllupORBnmimpaxNgEybuPMtsjMABYkKfDUMrywCOdImpAlvSdFqsFbiWECjxKSTSofdXiaMsXiD");
    bool BNdaFnl = false;
    bool PsCKpwIdAEe = true;
    double mDLybEZ = 1034327.289974549;
    bool IhGbhPKjtAs = true;

    for (int UjCyZCH = 1200933580; UjCyZCH > 0; UjCyZCH--) {
        IhGbhPKjtAs = ! BNdaFnl;
        vhUmyNeuHc -= mDLybEZ;
        PsCKpwIdAEe = ! ZYLVVVAtDkOhaPc;
        BNdaFnl = ! IhGbhPKjtAs;
    }

    for (int VtbuBTQVwZuu = 1574252465; VtbuBTQVwZuu > 0; VtbuBTQVwZuu--) {
        IhGbhPKjtAs = BNdaFnl;
    }

    if (IhGbhPKjtAs == false) {
        for (int reYljVqnxVep = 370704379; reYljVqnxVep > 0; reYljVqnxVep--) {
            mDLybEZ = MwbIfse;
            BNdaFnl = ZYLVVVAtDkOhaPc;
            mDLybEZ *= MwbIfse;
            BNdaFnl = ! ZYLVVVAtDkOhaPc;
        }
    }

    if (ZYLVVVAtDkOhaPc != false) {
        for (int odSRwYdltthLDYj = 1577321987; odSRwYdltthLDYj > 0; odSRwYdltthLDYj--) {
            continue;
        }
    }

    if (ZiNIClOV != true) {
        for (int HFfTnczCVkxNmU = 1515669325; HFfTnczCVkxNmU > 0; HFfTnczCVkxNmU--) {
            ZiNIClOV = IhGbhPKjtAs;
            BNdaFnl = ZYLVVVAtDkOhaPc;
        }
    }

    for (int LTibERNqyzMAcFt = 1981142306; LTibERNqyzMAcFt > 0; LTibERNqyzMAcFt--) {
        PsCKpwIdAEe = ! IhGbhPKjtAs;
        ZiNIClOV = BNdaFnl;
        UegRJSRKIjnd = UegRJSRKIjnd;
        IhGbhPKjtAs = ! ZiNIClOV;
        ZiNIClOV = IhGbhPKjtAs;
    }

    for (int xrmNyTNcpJKmaQ = 184729889; xrmNyTNcpJKmaQ > 0; xrmNyTNcpJKmaQ--) {
        IhGbhPKjtAs = PsCKpwIdAEe;
        vhUmyNeuHc -= mDLybEZ;
    }

    return IhGbhPKjtAs;
}

string tMzfd::tEnGpahUN(int uKijyeOYzz)
{
    double mxPuwZd = 560669.0567474446;
    double lUNJmKx = -270176.46111301903;

    if (mxPuwZd >= 560669.0567474446) {
        for (int IsuFTCPNyhSYJIX = 2118398092; IsuFTCPNyhSYJIX > 0; IsuFTCPNyhSYJIX--) {
            continue;
        }
    }

    for (int KSKuaKNnu = 60244849; KSKuaKNnu > 0; KSKuaKNnu--) {
        lUNJmKx = lUNJmKx;
        lUNJmKx += lUNJmKx;
        lUNJmKx *= lUNJmKx;
    }

    for (int KuMTOlc = 1283577713; KuMTOlc > 0; KuMTOlc--) {
        uKijyeOYzz *= uKijyeOYzz;
    }

    for (int BhpGnmSmQs = 1850409067; BhpGnmSmQs > 0; BhpGnmSmQs--) {
        mxPuwZd /= mxPuwZd;
    }

    return string("opPfUTXwAyLxUTrtoaWbWTaGmmiGVSgFndLFDyxxGxuhVpTacXUkVmyXBbswCPuvMcaqPsOuaxyyvVPUZMBPzSIVIhoxfLoaiPJssmSmRBIevWpGckobszVHoiSEEpySdjnRGlWbcO");
}

int tMzfd::euJDoCpkwjuyNTkH(string NULyMJTxBDIQsMMc, int QFwqqXYxHJAZyK, bool oyQZeODsMkryb)
{
    string OTNPAbJFMCXeTEn = string("eklraYPvrzEEFKrPnmyAxOfzH");
    string vdDlNOXNSiAFcYmA = string("sAqNBFAJJabxAQWXOHAhXfIrHrQzIXxxnZtZnmPfwXdGPJMcTVUDmyzLbkJCKmQQmZBPUlvxLowmlpgnnxvxFPtYzgCuWZasfljDYmMrXzExNPrRsKYixlTmMhpAmiDWFUbtbCaYMtkROquzOdONYAksAzmpBDRKwjiMlGyQDzRXAPeNpkcVukiVLycJHGNVBzRSPZuKLjgoVaAs");
    bool UqjysStoOqUDRAJ = true;
    string DKXHKAIKCCnuz = string("ZmihOsaquTPkDNCaWfUlNOCb");
    bool IidaPT = true;

    if (UqjysStoOqUDRAJ != true) {
        for (int VJFLFSUCtHNgTgE = 142643592; VJFLFSUCtHNgTgE > 0; VJFLFSUCtHNgTgE--) {
            UqjysStoOqUDRAJ = ! oyQZeODsMkryb;
            oyQZeODsMkryb = UqjysStoOqUDRAJ;
        }
    }

    for (int qEaYGzim = 544385017; qEaYGzim > 0; qEaYGzim--) {
        oyQZeODsMkryb = ! IidaPT;
    }

    if (UqjysStoOqUDRAJ == false) {
        for (int IIPqyKhpQY = 488015506; IIPqyKhpQY > 0; IIPqyKhpQY--) {
            NULyMJTxBDIQsMMc = NULyMJTxBDIQsMMc;
            OTNPAbJFMCXeTEn += DKXHKAIKCCnuz;
            vdDlNOXNSiAFcYmA = DKXHKAIKCCnuz;
            DKXHKAIKCCnuz = NULyMJTxBDIQsMMc;
            IidaPT = ! oyQZeODsMkryb;
        }
    }

    for (int RQUNLTDCxZaF = 576222608; RQUNLTDCxZaF > 0; RQUNLTDCxZaF--) {
        NULyMJTxBDIQsMMc += DKXHKAIKCCnuz;
    }

    return QFwqqXYxHJAZyK;
}

int tMzfd::DknCjwdPMgCLqX(string JlgVGIaVW, bool IqyKAR, int Upxamh)
{
    int VYISIloOoWQsKu = 71410693;
    int fQfTG = 794854853;
    int FmOduI = -1709484462;
    double FtDucnTHVwkXpt = -262245.82745240227;
    int CcgWkBbyfHCNQhG = -1256862076;
    bool LVDyaBiSqCJ = false;
    bool NhFgEvhm = false;
    int VyuZkgEPW = 1923906131;

    if (VyuZkgEPW > -107013767) {
        for (int tfPPfJ = 336436772; tfPPfJ > 0; tfPPfJ--) {
            fQfTG *= fQfTG;
        }
    }

    if (CcgWkBbyfHCNQhG > -1709484462) {
        for (int BkRDwxUXqNLXMu = 1255785500; BkRDwxUXqNLXMu > 0; BkRDwxUXqNLXMu--) {
            VyuZkgEPW *= Upxamh;
        }
    }

    for (int ynuqVW = 1423214608; ynuqVW > 0; ynuqVW--) {
        CcgWkBbyfHCNQhG *= CcgWkBbyfHCNQhG;
        VyuZkgEPW *= Upxamh;
    }

    for (int jVuVTwX = 999860369; jVuVTwX > 0; jVuVTwX--) {
        NhFgEvhm = ! NhFgEvhm;
    }

    return VyuZkgEPW;
}

int tMzfd::vEFcHTx(double DlVyoThEx)
{
    int stMsPSGJuGg = 775132681;
    string MKhDnzmCp = string("vLgyucTFtfEMVBsUPCBGnJbftRjCBBzumiXlBjHpeuOuosYqfbhTBNFntDfaYEXbdWpcNkwgDjjlFfRHLtlLnuBXqnPwpcjiPghHnTDVbNqgpwkHbhhbdcFHAIuzchBoKFcLTFuTGosngeXbdWKURoRhNpspOhgwDINLfrZoEeiAztxjjJNmaRycoobivuZNFavMKYtiXz");
    int yaaAsC = -886703790;
    string qSrJCpjMfByvhX = string("KFwzNcaoZlpsEWGOXBVhKKKVTPzCVOieVvRHvSejxUQnNHnibwAyKERtloJcxxYSbvUKFKcEfYILVYGGcbKyRrFrUAJIIwwPbMLAgixohxoFiOpBgVIfIUQTfusJDYHsWGyyyJJhcMhVNtaYAxeNLcvuCzURloSQhmCUBfwAmkcbgFaKxuvddshOTEnHmLIewpFUwpwrjRM");
    double CYQDfAeQFyMJ = -745751.5626842087;
    bool pGTAvBt = false;
    int dfKBirkxbDuqQNng = -750363448;
    int CJlGNfPXmVT = -784886891;
    int sZCln = -1564252403;

    return sZCln;
}

int tMzfd::mPSqigVLiUbWM(bool ZBoDjMysidBKfwR, string BjzqkWhvlgexhIxl, int wUhnK, bool aJClImdiih, string bAToIu)
{
    bool XsRxSILLFXOd = false;

    for (int pYVVBmwZRt = 1073748009; pYVVBmwZRt > 0; pYVVBmwZRt--) {
        continue;
    }

    for (int LSujBISQQwhrFxci = 375234212; LSujBISQQwhrFxci > 0; LSujBISQQwhrFxci--) {
        XsRxSILLFXOd = XsRxSILLFXOd;
        bAToIu += BjzqkWhvlgexhIxl;
        aJClImdiih = aJClImdiih;
        XsRxSILLFXOd = ! ZBoDjMysidBKfwR;
        aJClImdiih = ! XsRxSILLFXOd;
        bAToIu += bAToIu;
    }

    for (int dvgloWCRpJ = 149498042; dvgloWCRpJ > 0; dvgloWCRpJ--) {
        wUhnK -= wUhnK;
        BjzqkWhvlgexhIxl = bAToIu;
        ZBoDjMysidBKfwR = aJClImdiih;
        XsRxSILLFXOd = aJClImdiih;
        bAToIu += bAToIu;
    }

    for (int PNLSq = 915944096; PNLSq > 0; PNLSq--) {
        BjzqkWhvlgexhIxl = BjzqkWhvlgexhIxl;
        aJClImdiih = XsRxSILLFXOd;
        XsRxSILLFXOd = ! XsRxSILLFXOd;
        XsRxSILLFXOd = ZBoDjMysidBKfwR;
    }

    for (int jYHtSfwENKigzjSI = 823843851; jYHtSfwENKigzjSI > 0; jYHtSfwENKigzjSI--) {
        BjzqkWhvlgexhIxl += BjzqkWhvlgexhIxl;
        BjzqkWhvlgexhIxl += BjzqkWhvlgexhIxl;
    }

    if (aJClImdiih != false) {
        for (int vCGZoqWfxdJXqI = 330065443; vCGZoqWfxdJXqI > 0; vCGZoqWfxdJXqI--) {
            XsRxSILLFXOd = XsRxSILLFXOd;
            BjzqkWhvlgexhIxl = BjzqkWhvlgexhIxl;
            bAToIu += bAToIu;
        }
    }

    for (int PbPbKRtknl = 1611057797; PbPbKRtknl > 0; PbPbKRtknl--) {
        XsRxSILLFXOd = ! XsRxSILLFXOd;
    }

    return wUhnK;
}

string tMzfd::DtTyzdvjVsL(bool DZYqpFlhciLDpig, int xqIDbjK)
{
    string pognCxc = string("cYicUsIQZAqcGIdzMCJWphGmySHJPvW");
    int JudccVujEvzBkfyV = -397032671;
    bool QOHkPTDBbNHpG = true;

    if (xqIDbjK <= -696406835) {
        for (int xoLgJy = 1394445099; xoLgJy > 0; xoLgJy--) {
            xqIDbjK = xqIDbjK;
            DZYqpFlhciLDpig = ! QOHkPTDBbNHpG;
            QOHkPTDBbNHpG = ! QOHkPTDBbNHpG;
        }
    }

    for (int LqmVsStCkKJOuqT = 1504502293; LqmVsStCkKJOuqT > 0; LqmVsStCkKJOuqT--) {
        JudccVujEvzBkfyV *= xqIDbjK;
        JudccVujEvzBkfyV += xqIDbjK;
        DZYqpFlhciLDpig = QOHkPTDBbNHpG;
    }

    for (int vGojmuvgNjYacEA = 1427330604; vGojmuvgNjYacEA > 0; vGojmuvgNjYacEA--) {
        DZYqpFlhciLDpig = DZYqpFlhciLDpig;
        QOHkPTDBbNHpG = QOHkPTDBbNHpG;
        xqIDbjK /= xqIDbjK;
    }

    if (QOHkPTDBbNHpG != true) {
        for (int BpvYYJMBP = 1434165402; BpvYYJMBP > 0; BpvYYJMBP--) {
            JudccVujEvzBkfyV /= xqIDbjK;
            xqIDbjK *= JudccVujEvzBkfyV;
            JudccVujEvzBkfyV /= xqIDbjK;
            DZYqpFlhciLDpig = ! DZYqpFlhciLDpig;
        }
    }

    for (int aFHckXQoz = 1873553320; aFHckXQoz > 0; aFHckXQoz--) {
        continue;
    }

    return pognCxc;
}

int tMzfd::hYSVO(int niapGVtcGORuZ, bool wGwqSzlx, bool RIveAsh, string qFsToiJvN)
{
    int GJWDD = -1464672168;
    bool dQLuNrGEOl = false;
    double tdgvmwBDToA = 634289.5193059706;
    string xqfjEcApJgFdUeZ = string("kelJCcmcmilLaVfJnoZSAspPOfztJGejVdltDchWpnEKrtOcGRsNteVRwTqYlcCNRKGgQqXeLPMHtLAjRkLCcejgfNQyvfOeprpjjuMlIYIWczsegEUSxoYSUamraIUMysmUNiWDJgLXQJIPjxnOyCFCiTYvcixnycBFTxoZjGQtwLKQnEHBqzJAxHOjEXyrmejdGjAdegUxKkjFrB");
    double ZFphoTbvdVSU = 744444.3778207611;
    string pixzVw = string("XbWzmDQOCxUzDSdwI");
    bool tZYohV = true;

    for (int jgAQToZPgU = 2089764159; jgAQToZPgU > 0; jgAQToZPgU--) {
        dQLuNrGEOl = ! RIveAsh;
        dQLuNrGEOl = ! RIveAsh;
    }

    for (int kDhAxifs = 321024253; kDhAxifs > 0; kDhAxifs--) {
        RIveAsh = tZYohV;
    }

    for (int OPfGJpZXfNPwt = 31462331; OPfGJpZXfNPwt > 0; OPfGJpZXfNPwt--) {
        continue;
    }

    for (int wbWKs = 2035867532; wbWKs > 0; wbWKs--) {
        ZFphoTbvdVSU /= ZFphoTbvdVSU;
    }

    for (int AaCCWoEIM = 728322278; AaCCWoEIM > 0; AaCCWoEIM--) {
        wGwqSzlx = tZYohV;
    }

    return GJWDD;
}

int tMzfd::IMZzHQwJsGyRxs()
{
    bool sJcASRQzZBSNTi = false;
    int IkIYFETHwoQn = 2033419539;
    int wFUKNepLP = -745613213;
    double ZlaGMbcXzT = -387656.57373981713;
    bool MkfnPctkBtXga = true;
    int tCqRBVITOo = -1726492763;
    bool lcqKyeO = false;
    bool aIwqQWWtQgY = true;
    double JXPrW = 68825.74412648252;

    for (int bPoOyaruQHn = 519280900; bPoOyaruQHn > 0; bPoOyaruQHn--) {
        MkfnPctkBtXga = ! MkfnPctkBtXga;
        aIwqQWWtQgY = aIwqQWWtQgY;
    }

    for (int VVHEfHI = 1545671741; VVHEfHI > 0; VVHEfHI--) {
        wFUKNepLP /= wFUKNepLP;
    }

    if (wFUKNepLP < -1726492763) {
        for (int wQqwosPaAaCHGUt = 1387250704; wQqwosPaAaCHGUt > 0; wQqwosPaAaCHGUt--) {
            continue;
        }
    }

    return tCqRBVITOo;
}

int tMzfd::WXLgyeXRPHYWHf(string huobkrNwWe)
{
    int HmflFimW = 551364047;
    int rMqIPKQoagT = 2015439079;
    double XIjQIqFVrrtRt = -727765.7637510487;

    for (int pIWhFXmSCb = 222286330; pIWhFXmSCb > 0; pIWhFXmSCb--) {
        HmflFimW *= HmflFimW;
        XIjQIqFVrrtRt = XIjQIqFVrrtRt;
    }

    for (int wPGwrAEBj = 617856840; wPGwrAEBj > 0; wPGwrAEBj--) {
        HmflFimW *= rMqIPKQoagT;
        XIjQIqFVrrtRt /= XIjQIqFVrrtRt;
        HmflFimW /= HmflFimW;
        huobkrNwWe = huobkrNwWe;
        HmflFimW /= rMqIPKQoagT;
        HmflFimW /= HmflFimW;
    }

    for (int IiaoAH = 611860412; IiaoAH > 0; IiaoAH--) {
        HmflFimW *= HmflFimW;
    }

    return rMqIPKQoagT;
}

double tMzfd::RMekUIPqq(bool AHDxplJpmYx, string WaNBmsbSMJtvCmKG, string jXJliSZbwiFq)
{
    double JQhtKs = 171475.18567425854;
    string LWSTIkGzVcxkM = string("XvqIgKwPWzmetZdyaoYzcpFFPwXHNzyVSfSNlTxrJgWPtJzGLERKWNFwIsbwvSMbbFLNfAbMLYqRsIsDWCCrcXuNDRGLvajHTkhizTFgFxPAZYffPrlmFHGMYmzQFj");
    bool rMHxeepZu = true;
    double qggQJuuBIG = 659024.6578230755;
    double nUlZHcZuCiFFRJ = 807097.8579797643;
    int leJKwTLHTCpJGv = -1013943618;
    string SnLkCZjqv = string("pJzrDAUuKJVlHaXLXeYhznhcZQWMEuXLnGQRaxUZPqJSJRCPlNzmcwIiXFQkBllHKpgjTcUMVikBsvaBqPTHRZPyHLgIhkzxtahyXpMWqCNbAFvJZUlFypecHcgBVPGqJVLTNeDNXvQvxgPXUfxWwOBKjALlPSZFZYuvIfRLBphJztTUWezRWUvPQYrxUtVrBAuhMHqC");
    double JBvEFvMf = 718565.7383168101;
    int FUZeTQLXOr = 1083877807;

    for (int bmHmwyHJtZhO = 4734206; bmHmwyHJtZhO > 0; bmHmwyHJtZhO--) {
        WaNBmsbSMJtvCmKG = WaNBmsbSMJtvCmKG;
    }

    for (int UfimUcN = 525787272; UfimUcN > 0; UfimUcN--) {
        LWSTIkGzVcxkM += jXJliSZbwiFq;
        LWSTIkGzVcxkM += LWSTIkGzVcxkM;
        qggQJuuBIG /= JQhtKs;
    }

    if (jXJliSZbwiFq != string("XvqIgKwPWzmetZdyaoYzcpFFPwXHNzyVSfSNlTxrJgWPtJzGLERKWNFwIsbwvSMbbFLNfAbMLYqRsIsDWCCrcXuNDRGLvajHTkhizTFgFxPAZYffPrlmFHGMYmzQFj")) {
        for (int IjVsjZBhcdXfrH = 1671194670; IjVsjZBhcdXfrH > 0; IjVsjZBhcdXfrH--) {
            nUlZHcZuCiFFRJ *= qggQJuuBIG;
            AHDxplJpmYx = ! rMHxeepZu;
        }
    }

    for (int UUYZiqbldg = 1175099549; UUYZiqbldg > 0; UUYZiqbldg--) {
        qggQJuuBIG *= JBvEFvMf;
    }

    for (int IweAFufGzfMRDqRS = 215302263; IweAFufGzfMRDqRS > 0; IweAFufGzfMRDqRS--) {
        LWSTIkGzVcxkM += LWSTIkGzVcxkM;
        leJKwTLHTCpJGv = leJKwTLHTCpJGv;
        nUlZHcZuCiFFRJ -= JBvEFvMf;
        qggQJuuBIG += nUlZHcZuCiFFRJ;
        qggQJuuBIG /= JBvEFvMf;
        LWSTIkGzVcxkM = LWSTIkGzVcxkM;
    }

    if (leJKwTLHTCpJGv > 1083877807) {
        for (int xggqWAGOF = 2123647218; xggqWAGOF > 0; xggqWAGOF--) {
            LWSTIkGzVcxkM += SnLkCZjqv;
            JQhtKs /= JQhtKs;
        }
    }

    for (int mAtmgDULNj = 1525798697; mAtmgDULNj > 0; mAtmgDULNj--) {
        continue;
    }

    return JBvEFvMf;
}

tMzfd::tMzfd()
{
    this->OsMXFmK(279303870);
    this->JbLgUGgtBPPQ(string("vXlpfZWnMFWLnDaRrpBqeeNkyJRtALRAEi"), true, 138088.98644049856, -1881120874);
    this->TigIucvTbBCh(1029305.7092635134, string("RiwATxKQvBZWqmrFHqXmrfQncGXmBiFlUdHOfbkNYNAwZzRRUNvNyOxbWzcfmBBHyzqhiIYRKvTsbuajlbTroVMxDcyFqzvLELcdUARFGgxeKIlfmBoXokeuErsGSmRqIgZyYRJheFisZBXvyvYwpCVITbZQjastsvbXIDlXhlQnpKWgp"), false, false);
    this->tEnGpahUN(-652146599);
    this->euJDoCpkwjuyNTkH(string("wLcDeyLDtXnzwKuDInCdYrUZbosOMsueEqgQNAWLDcREfTVEcdwkSUXyjZcWCLVhopqGmnfMSgovUuIpLGFeeazHjZBaCiEgkt"), 209996512, false);
    this->DknCjwdPMgCLqX(string("otVVALhkwyzgrlzZKdKUJIHVlpfIBwhZVDUEQHifapLQQwwpVorHTGNTVlttMMoXGVtkgZYyDueVXoYJGzKOTDqontkuXEygrWoEtxBOGl"), false, -107013767);
    this->vEFcHTx(1027263.2939938838);
    this->mPSqigVLiUbWM(false, string("POCUmsQngyLlEOOtgitMmtJUkroJbgwcSZkCLcDZPMmUgeLTYmQUnpbUcyXYKPMhJRtUMYaIPrZYojgnZhKtaNbPXcBCHbVFEMQEXBxgeSSkNQtmVSahLIBkfhShybFrHWjAndraQeFPNFAYQEFlyINwYFhaGopStICLET"), 373703075, false, string("WlvFYzbmRTPEMkZYCkEwXomNWMyPFFNvrnwjLBjGvyABFvTpInthCsQiowRwzmMeHdkdrTOUNX"));
    this->DtTyzdvjVsL(false, -696406835);
    this->hYSVO(-1609195019, false, true, string("ENVxrHaqKGvlZhhamxmhVUnjGWjEcoEUpdQAVaAQarARdyhmNlSwQLcwtdzLRhifwBYXBdSAmhNxvMmhbbZpIIYFyQXjgvzcFIOFRpUlzoSuwwtXPcvgIKBAlVtpBBlPzXomlFzwCGaMJtyZeYmsYIdGsWlgqztRAkSOpdKGMVQbfPIBWuBAcdkNblBootXfYWCbpxSjERZnVJPRzvcpEHSJGVbOEkVZjZiozTM"));
    this->IMZzHQwJsGyRxs();
    this->WXLgyeXRPHYWHf(string("nhtABsnNQOgbzkyaPEJNKfFYwOLNSLUoBlsDSeJLLqmbjeeUsMuFNBolxIrUXjjOCJZXXgjuJuSekdnozGjGDYPTjKrdyBIkmnlLgQgdlQcNvclmzgkSm"));
    this->RMekUIPqq(false, string("EmvzNEzCkPqJOTAqgKsMqnCM"), string("rBZvbYIFswRLuqQmDQiYUoGfUTRQlhMhFqfKuzaHgSdphzPQxizYsrLyfkBOmgLaZfDcJBXrJdVNbonGxgvIfaGEcSnmGqOZkMXKnmGZNTgyBXTIWVpDymWq"));
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RKQdxumL
{
public:
    double JxtxKOohbYn;

    RKQdxumL();
protected:
    double pcrjLlcZH;
    bool EcTsR;

    void BWfbPIT();
    int Oipiqri(double qaDquTCJfYQyO, string tbyyL);
private:
    int WXhnTNeGSycDZcv;
    double qZgRsHRFIm;
    double MYYMImKUGtxWvAli;
    double iARgvWppBVoEEH;
    int uftVZpR;
    bool wCRSzHJTlWWunte;

    string KiEBNjzVmNGzwTD(bool mffbvyWkvoR, bool NCXbndhqIFjqzMgv, double oVmDORpLCfUVry, double HtsPTrGppOLq, double BhtklY);
    string MZWtScycV(int AhHLyPhxCcVvg, string SukPwiCRwwI, string kKDKXcuO, int uYGzKqHasd, bool DrVpJlTJsG);
    bool mXajcBFQFkWCtn(double VwjoKLqLyPd, double wctsgCwxqpEnIICK, int hqIZZa);
};

void RKQdxumL::BWfbPIT()
{
    string FEsoqyEvwKEjyEe = string("AikMHGaFDTlUTiQQZeULfTozomWkJfHqtQgfuZzTgXKIExMIybDhiAlqhRFomOOZkKliWLLPjVVqxHidztmFiCxrNeNEmUfEfuxCnOCXzKqwtSlrxNuhEJLWgIOSQMKqAxPdIAuSAQRUuoalUOcLuiNIGggNQcmPhfzMgyzuuKouyEEUPjZ");
    double bkJtvLnJCUSRkM = -479410.4265138526;
    int gFCRcQRcVefLDV = 949491674;
    int GXyPDEJvShM = 495721426;
    string znDPLXUkn = string("zExNBAZSYqoeUFEnNBfZgVihKcmtaudyHsNMCLYZhYRptuVIflHKDqyvxxwVpWEtrHZAOhCFKbnlJpGdzbSQMyfMPCcsZtQYBSLneiJBmuMfAEGBDFSADiiAYgVbmkBmCUBCNOzYFsqZIQswIWwLMBYTQUqkHynRWpKxBTaMdIRTLlVPCYkLGaGLeYfqitNGXxNcYGfEJgzlSPJkflE");
    double QdDqyPes = -211180.29746095478;
    string dTwVKzk = string("FAmyEQvrxhaQFNKxurgAElWHaLvkvhqMYTuFnZQVrSRuatawGXKYMzWsTnPCuEJxHGuyWtsdXUlMdnkmhAaklEtGMgSqOxOsjtOkkxfSIoxVVevNlxmSeSDUokwQhBNqXuFqdAdspcabiGzkZVkEvSOCtfEKqxq");
    int MUNSjFZiA = 412656901;
    int VegAY = -383069715;
    bool hDFlrrVvpJpo = true;

    if (GXyPDEJvShM <= 412656901) {
        for (int hovTGv = 1537669138; hovTGv > 0; hovTGv--) {
            MUNSjFZiA *= VegAY;
            FEsoqyEvwKEjyEe += dTwVKzk;
        }
    }

    if (MUNSjFZiA != 949491674) {
        for (int JdGGDPK = 1418911668; JdGGDPK > 0; JdGGDPK--) {
            MUNSjFZiA *= gFCRcQRcVefLDV;
            hDFlrrVvpJpo = hDFlrrVvpJpo;
        }
    }

    for (int IpVKRGJWvPWF = 843345599; IpVKRGJWvPWF > 0; IpVKRGJWvPWF--) {
        FEsoqyEvwKEjyEe = FEsoqyEvwKEjyEe;
    }
}

int RKQdxumL::Oipiqri(double qaDquTCJfYQyO, string tbyyL)
{
    int zzMHIBcbql = 11640711;
    string ruOFDaVLvD = string("wwufdzhfwyRMNEhjyNRMbgBSmpygssHIYivdnEGUAfokGmTufKJVfYqiTlUAMnYdNdJcYEnBMdfZvqDitagpYeHLzHUoIAWRNrOibfTjkHLZBheUQuQJSSPTFKrrSOgUXqsVlNBlquJBnPTwcETizXoMJgffkgiZWLJrJPIThTDycQTySATZrULG");
    int zQMkonUjoFlihk = 716963890;
    bool JBfET = false;
    double oXXnVVlXuxyVfy = -972018.022791911;
    int wzmmirxMtCpooX = -1574469650;

    for (int YGDdM = 503202835; YGDdM > 0; YGDdM--) {
        zzMHIBcbql -= zzMHIBcbql;
    }

    if (zQMkonUjoFlihk > 11640711) {
        for (int wDzyJupymoqf = 1532203551; wDzyJupymoqf > 0; wDzyJupymoqf--) {
            wzmmirxMtCpooX *= zQMkonUjoFlihk;
        }
    }

    return wzmmirxMtCpooX;
}

string RKQdxumL::KiEBNjzVmNGzwTD(bool mffbvyWkvoR, bool NCXbndhqIFjqzMgv, double oVmDORpLCfUVry, double HtsPTrGppOLq, double BhtklY)
{
    bool SVwSwqjHgwD = false;
    double RskNIsyI = -64290.08222336985;
    string hlUJQIqNnj = string("fTEttYlmqwkiuvAIWxIldXtnCVYsqnUzoUxvsjIsrvgaOEQiGFKWYqCoiXlHQssrxgoWnLbLkekLdYhVemaIGoykbSxRykmoRhYeKQHkbMptQdZFqsPkxnHPyOlYSfJCexyRSyFIoOkaXlQLnWtQWAnZQVNhnfQ");
    int gXBCYZqEXAGrSBpD = 1628288519;
    string kpedOHmL = string("lNIwdKkxqqtFYUJCqxZnnCfKWVuBTbfSGnbzWVZfqFdpFtxNNxoUhxnOevTHXKgCRblm");
    int gtZslkj = -2041196364;
    double HxnmsYFq = -883768.110250874;
    double Yeqrv = -913481.0147064257;
    string WEsQX = string("uwfTVZWuLKGtPNZlQ");

    for (int YgBLEISZ = 1969582630; YgBLEISZ > 0; YgBLEISZ--) {
        continue;
    }

    if (oVmDORpLCfUVry > -461595.91653964226) {
        for (int pHjyB = 1171497790; pHjyB > 0; pHjyB--) {
            continue;
        }
    }

    return WEsQX;
}

string RKQdxumL::MZWtScycV(int AhHLyPhxCcVvg, string SukPwiCRwwI, string kKDKXcuO, int uYGzKqHasd, bool DrVpJlTJsG)
{
    double IiTLLcmQMd = -971403.3254262616;
    bool VVKnqcTPFbLsOFn = true;
    string hbbJHCrJgT = string("ESasExabIuKhKjisVaPBCllHnisrFJHVJlZ");
    int xlUluSwkWL = -1771196921;
    int djuCqIAloYgh = -1748908003;
    bool YMQFWdXIKRX = true;
    string APaIRKjymbOVzC = string("HBGKtZsbHLDriQfRJeVkLODOIwAxCUHIHQSbMsInyztbRgpmMJyGgfbmXffhvssSJJToCPCxwVNgKUdcRSSMFHoFbDsGFVpHklbCeqMljhXFWdmulXJSCzBGDjNKVXWONtFEumYJliF");
    bool mmJeNrKBOebasml = false;
    int GnIxu = 1409246072;

    return APaIRKjymbOVzC;
}

bool RKQdxumL::mXajcBFQFkWCtn(double VwjoKLqLyPd, double wctsgCwxqpEnIICK, int hqIZZa)
{
    string GFWGCNiwWQQUXBNU = string("QcufXSQFoTJtFlMaRpeLBtqSwedwvhOdHeZQhyFucRSaLezWIsZBxlUhgMhrlDBuABUSaXXeHLOhgWRMETEKkKDoPKERCFRcCvrbybOsXOIptzInancJYEUDncEMhDcfbZhBIaehVznqcuzumLCEWwNIeuIpkTFVtUrYrYLNLBpIgDIqPXELQLmzEoPcnzzkxAMCzMdkyTGfJImKKOnTMDYTVPCgEMlhoeSEGVYzVEemTjBIE");
    double prksiVRVyxo = -42886.79259212129;
    bool HyyUvq = false;
    int pecZdtp = 855165300;
    int VWfnCjQGyxhQVPi = -518796943;

    for (int vvKHXd = 1818322391; vvKHXd > 0; vvKHXd--) {
        VwjoKLqLyPd += wctsgCwxqpEnIICK;
        hqIZZa += pecZdtp;
    }

    for (int nMDDaSeInGD = 1012579941; nMDDaSeInGD > 0; nMDDaSeInGD--) {
        GFWGCNiwWQQUXBNU = GFWGCNiwWQQUXBNU;
        prksiVRVyxo /= wctsgCwxqpEnIICK;
    }

    if (prksiVRVyxo != -643074.7276964722) {
        for (int jrjJoQtmDBAPggE = 1609508578; jrjJoQtmDBAPggE > 0; jrjJoQtmDBAPggE--) {
            VwjoKLqLyPd += VwjoKLqLyPd;
        }
    }

    return HyyUvq;
}

RKQdxumL::RKQdxumL()
{
    this->BWfbPIT();
    this->Oipiqri(-912406.9012465876, string("zTXIRCtm"));
    this->KiEBNjzVmNGzwTD(false, true, -461595.91653964226, 499697.8711431239, -800436.6422665997);
    this->MZWtScycV(1780086415, string("kVyDCVwhyhnwvLKNogWzKmhsTVMnsUznkhuKriTPpKKHjtmpNcXzONwDcWxHtrtgufxWNeOpafBjfVRqKQOPOpUObzSGPnvuExRwMCgmtcduHtxAnPQQJJdWdPcLiRrtRMccEejyVjBpToHPGpwyVJ"), string("oUBqXAQebSrmUTSSYPLRcgoAPalaluvOjTfhKCVQonkyTAymmQSzmpVQQsRJEgRpTAiBUjuCgKBegVDGeAWKStHJ"), -794269284, false);
    this->mXajcBFQFkWCtn(-643074.7276964722, -852623.3005795226, -226290499);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OpSGD
{
public:
    double DKLvhkae;
    string KVCOY;
    int NuOyT;
    double pQzYnenB;
    bool DOttaXnr;

    OpSGD();
    string oMDRPtavLswiRwy(int tjfJGevFhmFnuWQt, bool ZFIDjDhUk, string VVVrgHWfXIZTwS, bool OBakaN);
    double SsspjWOlqURoY(bool SqikZRFBjFdKZ, string jRsEQHmtqvHbHR, bool mLULesKKGeznTU);
    bool nWRLK(string LmIKiDKEod);
    int fbwrGqnPGrLpb();
    void gUKkjiPtkihyJVR();
    string DUmTldz(string tmScPCTlyVwQGMea, int anfsm, bool EzzWOMjRcQxgPoZ);
    bool IRcmaWmu(double vqCaiJTu, int KarLBVkyomN);
    bool lwPrfbGYxFNHVu(bool avxHQQly, double OXbflwisAlAJ, double CbEPKItKQeLGwgEi, string lEcMXsRiPWkzIXp, int egjUZcfVGadkvGF);
protected:
    int MGkbDLzcSrv;

    string HNFkjBxD();
    int uDZHuR(string ijUhWnkuZyEXiuyB, int aNRHHU);
    bool wHaffYdo(double SPVbdt);
    bool heAdLXDFh(int uupdd, int RFWqaByd, int xRdKkPEkJqdcvuSq);
private:
    string QewqA;
    bool dMJhSJ;
    bool fEhyKhuADDAzc;
    double BbmegivBN;
    int tvNxmfCZkPIbgw;
    string aRNtzWPLbrlW;

    string NWXgWecb(string hqQcgwr);
    string rrWITZXsPSU(double zzxIsTG);
    int YsjYFSHcPhM(bool ajFsbdQPceXo);
    double XinzSxebkO(double uGHhLsIOFgIF, int dvocDb);
    int EEgqQEE(bool jJfWSzEGHjDlFiyi, string sUDnekrOYsitAy);
    int ykhxSgolNR();
};

string OpSGD::oMDRPtavLswiRwy(int tjfJGevFhmFnuWQt, bool ZFIDjDhUk, string VVVrgHWfXIZTwS, bool OBakaN)
{
    string cUSgf = string("YNUYonNvwssLhRcIXrGmCHGNEIKmEgKAAXyKWQgRglLXivtodJsVudoKpGHPSZFopEJKNPLsbhCwqjMomEEcOogwjilzvfIFWLOFuzfY");
    double GaxgvrebUPN = 981454.5578287903;
    int JGlLgS = 1248473025;
    bool gyGzbv = false;
    bool DDHAXYbzCwYTgS = true;
    bool RkvKWhrOpTNNkULf = false;
    int fiwsbBMAxkJCk = -1988673576;
    double KUfuoKfGIvUYbn = 442958.42363677226;
    bool IHEpRqZLiWwry = false;

    if (ZFIDjDhUk == false) {
        for (int vHuOyXTJS = 1227469096; vHuOyXTJS > 0; vHuOyXTJS--) {
            ZFIDjDhUk = gyGzbv;
        }
    }

    for (int DOCwe = 493834083; DOCwe > 0; DOCwe--) {
        tjfJGevFhmFnuWQt -= JGlLgS;
    }

    for (int yVhLvffknxi = 302037401; yVhLvffknxi > 0; yVhLvffknxi--) {
        fiwsbBMAxkJCk += fiwsbBMAxkJCk;
    }

    for (int mVRHRzUCqfSAsLP = 353529644; mVRHRzUCqfSAsLP > 0; mVRHRzUCqfSAsLP--) {
        KUfuoKfGIvUYbn = GaxgvrebUPN;
        ZFIDjDhUk = ! gyGzbv;
        fiwsbBMAxkJCk /= fiwsbBMAxkJCk;
        OBakaN = ! gyGzbv;
    }

    if (GaxgvrebUPN > 442958.42363677226) {
        for (int ZvVCUTMbsbuYs = 345261280; ZvVCUTMbsbuYs > 0; ZvVCUTMbsbuYs--) {
            RkvKWhrOpTNNkULf = gyGzbv;
        }
    }

    return cUSgf;
}

double OpSGD::SsspjWOlqURoY(bool SqikZRFBjFdKZ, string jRsEQHmtqvHbHR, bool mLULesKKGeznTU)
{
    double suXwCHnhnZ = -66674.6481372948;
    bool uAHQT = false;

    for (int LLeBxTEdmsD = 571653452; LLeBxTEdmsD > 0; LLeBxTEdmsD--) {
        SqikZRFBjFdKZ = ! uAHQT;
        uAHQT = ! uAHQT;
        mLULesKKGeznTU = uAHQT;
        jRsEQHmtqvHbHR = jRsEQHmtqvHbHR;
    }

    return suXwCHnhnZ;
}

bool OpSGD::nWRLK(string LmIKiDKEod)
{
    bool DkQzxrNSUCygTc = true;

    for (int mfxyjOeqxqUbouZ = 1358076512; mfxyjOeqxqUbouZ > 0; mfxyjOeqxqUbouZ--) {
        DkQzxrNSUCygTc = ! DkQzxrNSUCygTc;
        LmIKiDKEod = LmIKiDKEod;
    }

    return DkQzxrNSUCygTc;
}

int OpSGD::fbwrGqnPGrLpb()
{
    double vlAVJdIFwfw = -905832.9420644589;
    bool YjqEnlnMBBFsd = true;
    string tBLatDxbgcsznlS = string("OmVFpxlKzOmpPHLyawCUFkurNdAWAZXAErnJRCwrYtuSzMcuKSUiJnQzfgqNPQgdCvvxoLTpWrLFCAqtMbERGpysKoYwKNFkkBfkeCRCwVYElBzdKxKFxdTxvkBNFdkJLeIqIigrsosWmJxxDEklREGzjNgaIUQnUlebgOUTIuaMxTPfcETodOnnJkJiBqSFcSvlfRMIknhApWGFVuRXbWaWhgGBhfK");
    string huPkOI = string("UPfmIxVPshPQNSOeWgQtgMosGEGzGiWpZigPHfnxMHFCfbwVLBeqBrkAFzKfsfLBbtnjnCODWXpWGpB");
    double loBPYC = 141738.79105323445;
    string kVxwAkDxpn = string("qOiTaywkGUbtlvctxdIURiGVHIQnJtPQzjEEhQcazlAmQVeHtnilIYCXwUaVfNLOQuJWrWPuVjsCJDdCDQiiVaJMgDOJPuUvpBlHHsuYIXEbqnVTnAKkNpYnioeACLGGlvHkQCeavhfOextIXbmwcuMVgyMdXdaCjmLoMlEYPNmuJBnsfTiCjOisrqYcURPSxmXEZXzNDTNxNUXIUzcRndfrNiJGGRvlnkCjCqxfPhfHJy");
    string acFSzCvaKgnPcXz = string("dXKEWEKugCujwcfJlJRmunvPkIdGoUKdXZDTuCfQjstKLXRsJeIcCUITfwjkFLBkVOpZFQxkFzjCGWVjKNdNVLGwwJYPLnOpsGGcmsxGorD");
    double PUblmUyUxinxFS = -608789.1263632941;
    string nMdfcOOqbZRAsagH = string("EMVbAxsOIRMfFlDgoMbDmQz");

    for (int sOyUCvYCjtPTLH = 2130711599; sOyUCvYCjtPTLH > 0; sOyUCvYCjtPTLH--) {
        kVxwAkDxpn += tBLatDxbgcsznlS;
        loBPYC += loBPYC;
    }

    if (kVxwAkDxpn < string("EMVbAxsOIRMfFlDgoMbDmQz")) {
        for (int lqPkzSzoIximOT = 924759497; lqPkzSzoIximOT > 0; lqPkzSzoIximOT--) {
            huPkOI = acFSzCvaKgnPcXz;
        }
    }

    for (int bpIeElWxNFEl = 11734595; bpIeElWxNFEl > 0; bpIeElWxNFEl--) {
        tBLatDxbgcsznlS += nMdfcOOqbZRAsagH;
        acFSzCvaKgnPcXz += kVxwAkDxpn;
        acFSzCvaKgnPcXz = tBLatDxbgcsznlS;
    }

    for (int XtiQtG = 943435009; XtiQtG > 0; XtiQtG--) {
        tBLatDxbgcsznlS += huPkOI;
        kVxwAkDxpn += kVxwAkDxpn;
    }

    if (tBLatDxbgcsznlS == string("dXKEWEKugCujwcfJlJRmunvPkIdGoUKdXZDTuCfQjstKLXRsJeIcCUITfwjkFLBkVOpZFQxkFzjCGWVjKNdNVLGwwJYPLnOpsGGcmsxGorD")) {
        for (int nNcfyme = 1871849009; nNcfyme > 0; nNcfyme--) {
            loBPYC *= loBPYC;
            PUblmUyUxinxFS *= PUblmUyUxinxFS;
            YjqEnlnMBBFsd = ! YjqEnlnMBBFsd;
            YjqEnlnMBBFsd = YjqEnlnMBBFsd;
        }
    }

    if (kVxwAkDxpn == string("qOiTaywkGUbtlvctxdIURiGVHIQnJtPQzjEEhQcazlAmQVeHtnilIYCXwUaVfNLOQuJWrWPuVjsCJDdCDQiiVaJMgDOJPuUvpBlHHsuYIXEbqnVTnAKkNpYnioeACLGGlvHkQCeavhfOextIXbmwcuMVgyMdXdaCjmLoMlEYPNmuJBnsfTiCjOisrqYcURPSxmXEZXzNDTNxNUXIUzcRndfrNiJGGRvlnkCjCqxfPhfHJy")) {
        for (int QwcSVtqxYOgAsrcU = 1136375645; QwcSVtqxYOgAsrcU > 0; QwcSVtqxYOgAsrcU--) {
            acFSzCvaKgnPcXz += acFSzCvaKgnPcXz;
            YjqEnlnMBBFsd = ! YjqEnlnMBBFsd;
            vlAVJdIFwfw *= PUblmUyUxinxFS;
            kVxwAkDxpn = acFSzCvaKgnPcXz;
        }
    }

    return 1778604949;
}

void OpSGD::gUKkjiPtkihyJVR()
{
    bool dgIMsNNMhrXj = true;
    bool ZQsNx = false;
    double XNPOn = -226352.11434714723;
    string gUfzfuf = string("UorQljGYBgAEAHROHHLjWBMsHqkasacbiRXpLReuggQNbRUndmjIAnxbPuAbbGDjzascaHzcCVcrolvlZfZpdGAFhCdZIokUUHfVfEOFyiyQRyiiAxJfkZwNjVMghdDxRBfgIemjRjTZHcm");
    bool eTeiUWpbXQut = true;

    for (int VkYuzF = 1542293108; VkYuzF > 0; VkYuzF--) {
        dgIMsNNMhrXj = eTeiUWpbXQut;
        eTeiUWpbXQut = ! dgIMsNNMhrXj;
        XNPOn -= XNPOn;
    }

    for (int FhXBkiW = 1256310561; FhXBkiW > 0; FhXBkiW--) {
        dgIMsNNMhrXj = eTeiUWpbXQut;
        gUfzfuf += gUfzfuf;
        eTeiUWpbXQut = ! dgIMsNNMhrXj;
    }

    for (int zMRdyVktQapUqQ = 914953629; zMRdyVktQapUqQ > 0; zMRdyVktQapUqQ--) {
        eTeiUWpbXQut = ! eTeiUWpbXQut;
        XNPOn = XNPOn;
        dgIMsNNMhrXj = dgIMsNNMhrXj;
    }

    if (eTeiUWpbXQut == true) {
        for (int omZCXKGPu = 1381417445; omZCXKGPu > 0; omZCXKGPu--) {
            eTeiUWpbXQut = ! ZQsNx;
            eTeiUWpbXQut = ZQsNx;
        }
    }
}

string OpSGD::DUmTldz(string tmScPCTlyVwQGMea, int anfsm, bool EzzWOMjRcQxgPoZ)
{
    bool QLxhBNA = false;
    double OGAzKytkwphPxU = 575450.7549098742;
    double iHVaUKocgWaOPNM = 840680.2049632949;
    int iEEWXcD = -303265169;

    for (int Rcshhaz = 1324964775; Rcshhaz > 0; Rcshhaz--) {
        continue;
    }

    return tmScPCTlyVwQGMea;
}

bool OpSGD::IRcmaWmu(double vqCaiJTu, int KarLBVkyomN)
{
    double HbZyjjWwhNN = 190086.74331517422;
    int NVTNlfUcuYDPwr = -1026250673;
    bool yfVDsLBSrT = false;
    int asNnFBTSV = -1111939588;
    double EKegIbynjAtorLw = -46791.81239530483;
    string mdVCwSo = string("vTnILerXedQswRGYysnjBvvkfIhDnkRYhCieibJcsjDnRqEoBiPeliFCWbNFWwydiNxDgzSjZSMYnsVUuJeXJRJMHlZWqdCGxsLruWtfDSjCJZxGUgeojYPeNUJFimlqHfCXjQLrxZnKXkvXLMfoyqXFWfoUENTBvLetpptAqqZrvmwrisFHKQstHTEvqRHcakEZBjiwUQWwFYUDXbBV");
    string LDnAxQtspjplxwSO = string("UvEOIQsWclmpCmoJrztVpyYZPESlLWUbXrzPsImKtwmwfxzARHLGWNzoJNDAYhoQmAopjqdtycNBdYL");
    string wARXzQwgSwRMeLp = string("seOpMTzCpAyOtVyczfqTLkMHhVkxFpjTtKiWVsbMqIpolKewfOZpXIWCwkdyBzixtBwxUBzKexCvIUsPCoeaaZzKTiJfazLlrswUjolraerWBcpOaWhVYJg");
    int cgeWEkujrjTsdhog = -868089613;
    string cYyrGXDA = string("JVSMcZnbdezskEoMEuvwVqmiLTNhLrQxjLYqEepcfolkDPiYbimacKcDNgmxXcdhzfnTnDWSMwpLBuHwMToeLWXJwWMIOeSosMYPtujMxhKsoYNFDiPKrYdlaNpREHqBOOKrRDqDOykABxftArlBqiqcVG");

    if (KarLBVkyomN > -558292514) {
        for (int DUQfp = 1989408645; DUQfp > 0; DUQfp--) {
            asNnFBTSV -= NVTNlfUcuYDPwr;
        }
    }

    for (int EQGpUfRdIf = 1496128705; EQGpUfRdIf > 0; EQGpUfRdIf--) {
        continue;
    }

    if (mdVCwSo != string("UvEOIQsWclmpCmoJrztVpyYZPESlLWUbXrzPsImKtwmwfxzARHLGWNzoJNDAYhoQmAopjqdtycNBdYL")) {
        for (int MFqagcpLTlsEczRs = 1553373260; MFqagcpLTlsEczRs > 0; MFqagcpLTlsEczRs--) {
            mdVCwSo += LDnAxQtspjplxwSO;
        }
    }

    return yfVDsLBSrT;
}

bool OpSGD::lwPrfbGYxFNHVu(bool avxHQQly, double OXbflwisAlAJ, double CbEPKItKQeLGwgEi, string lEcMXsRiPWkzIXp, int egjUZcfVGadkvGF)
{
    int AMgaPgducHMkn = 1921028245;
    bool iTwOVgmfRFqu = false;
    string VRvLujRZdekheYI = string("LWejrTNVmhumdyXxVKpeJZoZPNynzWuHoBAJGUmUlYnsNohkWqvZgWCDCimuHazQSOPYwwAdEwxiOzctCvtaHTPixoHrpTEaqxEgzKdFqyrAUDOxmQnCCcNhnjAIub");
    int bBjwuEOOgOXJbfzu = 162971687;
    int cjWHQLiSHX = 1996895829;
    int UxFCebNWxyvCQ = 276427106;
    bool ZuAqPyQ = true;

    for (int ugkzXRrSbdlb = 1662284452; ugkzXRrSbdlb > 0; ugkzXRrSbdlb--) {
        bBjwuEOOgOXJbfzu *= UxFCebNWxyvCQ;
    }

    for (int bQMHujayQOzokF = 1117789734; bQMHujayQOzokF > 0; bQMHujayQOzokF--) {
        bBjwuEOOgOXJbfzu /= egjUZcfVGadkvGF;
        AMgaPgducHMkn -= egjUZcfVGadkvGF;
    }

    if (ZuAqPyQ != false) {
        for (int bbmhMKHyyjEF = 1622744718; bbmhMKHyyjEF > 0; bbmhMKHyyjEF--) {
            AMgaPgducHMkn = egjUZcfVGadkvGF;
        }
    }

    if (iTwOVgmfRFqu == false) {
        for (int XQWpYxbYAkWz = 1197214939; XQWpYxbYAkWz > 0; XQWpYxbYAkWz--) {
            bBjwuEOOgOXJbfzu /= bBjwuEOOgOXJbfzu;
        }
    }

    for (int hIfzHgx = 1517413751; hIfzHgx > 0; hIfzHgx--) {
        lEcMXsRiPWkzIXp += lEcMXsRiPWkzIXp;
    }

    return ZuAqPyQ;
}

string OpSGD::HNFkjBxD()
{
    bool JjwxlpGScRFyjhwk = false;
    double EKicFETsIcEytj = 961519.6218538111;
    double ZVNrDwcpLs = 88208.2783614152;
    double kRQHOiXsCJu = 653114.1731193761;
    bool JODMB = false;
    int oImtvyKmJYWQh = 703778333;
    bool rmSLroWwxtkL = false;
    bool QwYnZQkzxJjeYUYX = true;
    bool wuPuCorXBivEFdf = false;

    if (JjwxlpGScRFyjhwk != false) {
        for (int EVaTnaoxr = 68055363; EVaTnaoxr > 0; EVaTnaoxr--) {
            JODMB = wuPuCorXBivEFdf;
            JODMB = JODMB;
        }
    }

    return string("AScBLdLwtHYBFtGgUwLzZZvVZGlvzEQZiraclJMxvTCNNYUbhxhZwhioslYMXeiESHYSuwjOwpocUfxZQPjXGgESefzOlpvvthGvJaCVNjrpmxkoErNCnIzRH");
}

int OpSGD::uDZHuR(string ijUhWnkuZyEXiuyB, int aNRHHU)
{
    int GNgBonsNhjiaC = -924097034;
    bool wskFTMqUdZVRf = false;
    bool HKgzTdtgEL = false;
    int UPjjQVPJidBkb = -260831002;
    bool VuWQcfqB = true;
    double OjfLobRj = 105399.68857929255;
    double SWnejdYw = 834968.4635825037;
    bool jmgSHvYHKmn = false;
    int fceqogfPuQHMO = 1313605433;
    bool CrVgxnO = true;

    for (int cwffuJ = 708058946; cwffuJ > 0; cwffuJ--) {
        aNRHHU = aNRHHU;
        VuWQcfqB = ! VuWQcfqB;
    }

    for (int DSWFeTeRcSY = 1000238222; DSWFeTeRcSY > 0; DSWFeTeRcSY--) {
        GNgBonsNhjiaC -= fceqogfPuQHMO;
        VuWQcfqB = ! jmgSHvYHKmn;
        wskFTMqUdZVRf = HKgzTdtgEL;
    }

    for (int OsnzBZ = 930155522; OsnzBZ > 0; OsnzBZ--) {
        continue;
    }

    if (UPjjQVPJidBkb > 1313605433) {
        for (int DNYRMirBa = 1217788402; DNYRMirBa > 0; DNYRMirBa--) {
            aNRHHU *= UPjjQVPJidBkb;
        }
    }

    return fceqogfPuQHMO;
}

bool OpSGD::wHaffYdo(double SPVbdt)
{
    string rnHXCCUZE = string("utxayTVRnDCrSoIOEvFmjcUJvEIecasBVHIAmFwUwPjvpTLdxUKeEalTQsfjZnKFslHWmegKpJhnOZQYDoauOMDjYPnvekdTEKVJHBrbKywmHOyXEhNWtUNcJiSRnBZPeskvqpHLYaVkpOSUQfCRmLwhhTchJORoZCMQOIlAzrKEUwinGcXXJKepEIPlhMizYbglReFmrMTdE");
    double uJfdaxuTTcO = -1033726.8831363944;

    if (uJfdaxuTTcO >= -1033726.8831363944) {
        for (int aLnjygZEK = 542776941; aLnjygZEK > 0; aLnjygZEK--) {
            uJfdaxuTTcO *= SPVbdt;
            rnHXCCUZE = rnHXCCUZE;
            SPVbdt *= uJfdaxuTTcO;
        }
    }

    return true;
}

bool OpSGD::heAdLXDFh(int uupdd, int RFWqaByd, int xRdKkPEkJqdcvuSq)
{
    bool iaHTYhhx = false;
    int ljwUOpwYAOOk = -822081766;
    string XboHsu = string("nypPHwOTVzZhbWtlznKeJVrcJxqbeRXpjqUOPoVvpsQpjLBHJJafLniuEhvTcWiwJZDWzcdNQPfDzRIcoIECpJjcsXMATjEOVXTTbZZhIJTmcCsWXzsLOFccOlmUrGhJ");

    if (RFWqaByd >= 680004535) {
        for (int qQqQykfGLkTsZ = 1178029137; qQqQykfGLkTsZ > 0; qQqQykfGLkTsZ--) {
            xRdKkPEkJqdcvuSq += xRdKkPEkJqdcvuSq;
        }
    }

    for (int IDwFSITmmqdjHe = 314014000; IDwFSITmmqdjHe > 0; IDwFSITmmqdjHe--) {
        uupdd += xRdKkPEkJqdcvuSq;
        xRdKkPEkJqdcvuSq *= RFWqaByd;
        XboHsu += XboHsu;
        uupdd += xRdKkPEkJqdcvuSq;
        xRdKkPEkJqdcvuSq -= RFWqaByd;
    }

    return iaHTYhhx;
}

string OpSGD::NWXgWecb(string hqQcgwr)
{
    int xTutHlRaROraK = 1721374541;
    bool aIWzGWCpy = false;
    double xuSHp = 236805.92065294975;
    string OuQHJuwBtriDR = string("SeOIOYdSPueUGvmeaQucmVzSgPglklkUmjeHIgdwcwTeqqQQNKmTviQtDUnfNvrNiGKSoykkNHOkazzOMlOmClfREbuPtMmTziBGyFwcDwQsfBhhcgUObrzrGHDZAAOELCIIBWkJUAxDWoNdUHeLimiRKiZENWfYQXRwplXdcWhgMIIYqKVPYVuPhlxeYYuZBuaNhfXhdztqRRwzDYKbmwVKNCTKAyBxqAHbdQZgTy");
    bool FwPugsPOmxmaaWK = true;
    int eYsQRAPwqbOPXdb = 326479787;
    string BkyENyzhP = string("JqFlwUrCgADkoNGimgqKSWQSLBjzOqxoJNJYdfJlhvZmlSXpWnt");
    bool UaESZgrrrHba = false;
    bool IQaDpplheRTpPv = true;
    string uMdjKKSQFnaHBx = string("fmtpIWLhUIjaFcxycAjONUvNsqNXsSKFCPGHUETMIzqTqC");

    if (IQaDpplheRTpPv != true) {
        for (int WnmDvOWGZtq = 530366531; WnmDvOWGZtq > 0; WnmDvOWGZtq--) {
            hqQcgwr = uMdjKKSQFnaHBx;
            FwPugsPOmxmaaWK = UaESZgrrrHba;
        }
    }

    for (int ERdXkeyf = 1715062751; ERdXkeyf > 0; ERdXkeyf--) {
        hqQcgwr = BkyENyzhP;
    }

    return uMdjKKSQFnaHBx;
}

string OpSGD::rrWITZXsPSU(double zzxIsTG)
{
    string SYfWVxADNfGx = string("zTfLinzAQDaGyICvixiDeKyVHlupyFMlXJcxHoNcuwrIoRBzATlDOTVJEHfhdvCZdwVXXPxJFeDOfRFzpZwZxCxgYjaxTDjCGKYYYBlvDxdARJDAUPUGSYjJYuKRaAfTKCVFm");
    double SLBRwXUUb = -826084.7348123392;
    double RpandU = -381693.5410293762;
    double mksFLHcoKO = 123684.44656407037;
    int HEcvOoikMFu = 334711012;
    double vkGHE = 412316.5850313944;
    int ifLAjVqRXjJm = 1909714883;
    int tHgGe = -1789215105;
    bool bNOwrv = true;

    for (int zMTvh = 623724553; zMTvh > 0; zMTvh--) {
        zzxIsTG /= vkGHE;
        zzxIsTG = SLBRwXUUb;
        SLBRwXUUb -= SLBRwXUUb;
        SLBRwXUUb += zzxIsTG;
        RpandU = mksFLHcoKO;
        tHgGe /= ifLAjVqRXjJm;
        bNOwrv = bNOwrv;
    }

    return SYfWVxADNfGx;
}

int OpSGD::YsjYFSHcPhM(bool ajFsbdQPceXo)
{
    bool qDBaBVfEVhrUx = true;
    double wnfSaEgPDa = -338510.9303672113;
    int ahqSlpq = 1458206918;
    string ZeXXkm = string("JWjBQkenettWEutcrlfHviqoiXAtrwblZEMPUPBqkZgOMCKkBIVmXluEcDtkaPhlWyVwoEAeFvqDQtbYCAhsiUcZIfulpzJxyHvFEaIbSlYoezcFyyPYFvlEDPcNcvBepUCNShpVIWJliDPWFmsnYDyazAXGenHqXNBugLrcWfPrhnbDQcgbQlZDAHJTrODdaSlUAntsAPtTYgQUtlbPGTYrJOqbpCdmlW");
    int ZyAdjvYKihFfooo = 783572208;
    double GpMmYi = 79419.93704551978;
    int RLFwV = -1991311471;
    int LQQWoc = 1748779430;

    if (LQQWoc > 1458206918) {
        for (int WFWZJLgmjymXiei = 1247166333; WFWZJLgmjymXiei > 0; WFWZJLgmjymXiei--) {
            ahqSlpq /= RLFwV;
        }
    }

    if (wnfSaEgPDa < -338510.9303672113) {
        for (int dUzrZE = 857774670; dUzrZE > 0; dUzrZE--) {
            continue;
        }
    }

    if (GpMmYi == 79419.93704551978) {
        for (int OxfBwOnJjbv = 390478979; OxfBwOnJjbv > 0; OxfBwOnJjbv--) {
            RLFwV = ahqSlpq;
            LQQWoc -= ZyAdjvYKihFfooo;
            ZyAdjvYKihFfooo *= RLFwV;
        }
    }

    for (int aaXLXkkmAVIBRPZY = 990738459; aaXLXkkmAVIBRPZY > 0; aaXLXkkmAVIBRPZY--) {
        ajFsbdQPceXo = ajFsbdQPceXo;
        ZyAdjvYKihFfooo += ZyAdjvYKihFfooo;
        qDBaBVfEVhrUx = qDBaBVfEVhrUx;
    }

    return LQQWoc;
}

double OpSGD::XinzSxebkO(double uGHhLsIOFgIF, int dvocDb)
{
    int CYAxJFMpjBEeXRM = 2085629998;
    int PRRrSLQH = -627302771;

    for (int cdLlhMwiUOL = 1850402157; cdLlhMwiUOL > 0; cdLlhMwiUOL--) {
        dvocDb = CYAxJFMpjBEeXRM;
        PRRrSLQH += PRRrSLQH;
        dvocDb = dvocDb;
        dvocDb = CYAxJFMpjBEeXRM;
        uGHhLsIOFgIF *= uGHhLsIOFgIF;
        CYAxJFMpjBEeXRM += CYAxJFMpjBEeXRM;
        uGHhLsIOFgIF += uGHhLsIOFgIF;
        dvocDb /= CYAxJFMpjBEeXRM;
    }

    if (CYAxJFMpjBEeXRM <= -627302771) {
        for (int iZcGNWqNJkWD = 1891300237; iZcGNWqNJkWD > 0; iZcGNWqNJkWD--) {
            CYAxJFMpjBEeXRM /= PRRrSLQH;
        }
    }

    if (PRRrSLQH <= -627302771) {
        for (int AEJAXLOwJyQHIxA = 1687808049; AEJAXLOwJyQHIxA > 0; AEJAXLOwJyQHIxA--) {
            dvocDb = PRRrSLQH;
            PRRrSLQH = CYAxJFMpjBEeXRM;
            CYAxJFMpjBEeXRM /= PRRrSLQH;
        }
    }

    if (PRRrSLQH != -627302771) {
        for (int eHeNlfrhZeiPRl = 1558537847; eHeNlfrhZeiPRl > 0; eHeNlfrhZeiPRl--) {
            continue;
        }
    }

    return uGHhLsIOFgIF;
}

int OpSGD::EEgqQEE(bool jJfWSzEGHjDlFiyi, string sUDnekrOYsitAy)
{
    string quLUVTGi = string("PbMSKfMw");

    if (sUDnekrOYsitAy >= string("niuEImaZOLpdxPrtaFErkHmdnKAfXJIddQPlsmiUhXfxOLVaOEn")) {
        for (int quzeuwnGkUZDR = 948206473; quzeuwnGkUZDR > 0; quzeuwnGkUZDR--) {
            sUDnekrOYsitAy = sUDnekrOYsitAy;
            sUDnekrOYsitAy += quLUVTGi;
            sUDnekrOYsitAy = sUDnekrOYsitAy;
            quLUVTGi += sUDnekrOYsitAy;
        }
    }

    if (quLUVTGi < string("PbMSKfMw")) {
        for (int gnwjtdeLLy = 966196729; gnwjtdeLLy > 0; gnwjtdeLLy--) {
            sUDnekrOYsitAy = quLUVTGi;
            quLUVTGi = quLUVTGi;
            quLUVTGi += quLUVTGi;
        }
    }

    for (int gLSonWLIwIWW = 1437758889; gLSonWLIwIWW > 0; gLSonWLIwIWW--) {
        quLUVTGi += quLUVTGi;
        sUDnekrOYsitAy += quLUVTGi;
        quLUVTGi += sUDnekrOYsitAy;
    }

    for (int kwSFSpgeJvLPURQm = 1847251910; kwSFSpgeJvLPURQm > 0; kwSFSpgeJvLPURQm--) {
        sUDnekrOYsitAy += quLUVTGi;
    }

    if (sUDnekrOYsitAy < string("niuEImaZOLpdxPrtaFErkHmdnKAfXJIddQPlsmiUhXfxOLVaOEn")) {
        for (int aHJKPfKuUJMNThQ = 495317133; aHJKPfKuUJMNThQ > 0; aHJKPfKuUJMNThQ--) {
            quLUVTGi += sUDnekrOYsitAy;
            sUDnekrOYsitAy = quLUVTGi;
            jJfWSzEGHjDlFiyi = ! jJfWSzEGHjDlFiyi;
        }
    }

    if (quLUVTGi > string("PbMSKfMw")) {
        for (int NTFqSgNOCHrW = 1451447683; NTFqSgNOCHrW > 0; NTFqSgNOCHrW--) {
            quLUVTGi = sUDnekrOYsitAy;
            jJfWSzEGHjDlFiyi = ! jJfWSzEGHjDlFiyi;
            quLUVTGi = quLUVTGi;
            quLUVTGi = sUDnekrOYsitAy;
            jJfWSzEGHjDlFiyi = jJfWSzEGHjDlFiyi;
            sUDnekrOYsitAy = sUDnekrOYsitAy;
            quLUVTGi += sUDnekrOYsitAy;
        }
    }

    if (quLUVTGi <= string("niuEImaZOLpdxPrtaFErkHmdnKAfXJIddQPlsmiUhXfxOLVaOEn")) {
        for (int gBBAaDsSVTkVaA = 1744991270; gBBAaDsSVTkVaA > 0; gBBAaDsSVTkVaA--) {
            quLUVTGi += quLUVTGi;
            quLUVTGi = sUDnekrOYsitAy;
            sUDnekrOYsitAy = sUDnekrOYsitAy;
            quLUVTGi += sUDnekrOYsitAy;
            quLUVTGi += sUDnekrOYsitAy;
            quLUVTGi = sUDnekrOYsitAy;
        }
    }

    return -707820538;
}

int OpSGD::ykhxSgolNR()
{
    bool tFuLTtjjvoti = true;
    int ZwgxHbwqbN = -1685385465;
    string GuavhHLJDJLzXNJ = string("hnznQCzVHRbjTGmxfGFBmqiKLQImXgYlAmYmZLfscpRmfUNQiFjaENEkHomKWYuPVnAtiZMNQJhhCKxWIaEvPstdVaMyRwOTIIsCOHVTwTpegeSJqDDzmnHbVEiNkkAVSOKirNIKacLNUZALuHJKNYHWJkOCVKOhSBCsmebLlYiQJkLRFHUnpwtCOoUeWCcapilFoRRaPROmOgtRWqhzEKYaLdobplXofLKTqqnYhntjrlFRIfDzbIuYKNrSoe");
    double bklyQ = 965274.4086681296;

    for (int XIRkpsAScDEVky = 1566561537; XIRkpsAScDEVky > 0; XIRkpsAScDEVky--) {
        continue;
    }

    for (int cUFhz = 1037574700; cUFhz > 0; cUFhz--) {
        tFuLTtjjvoti = ! tFuLTtjjvoti;
        ZwgxHbwqbN += ZwgxHbwqbN;
    }

    if (tFuLTtjjvoti != true) {
        for (int QSbknCFIKnaTlWWN = 1062896924; QSbknCFIKnaTlWWN > 0; QSbknCFIKnaTlWWN--) {
            ZwgxHbwqbN += ZwgxHbwqbN;
        }
    }

    for (int MrIUpGQAbmCv = 1536931610; MrIUpGQAbmCv > 0; MrIUpGQAbmCv--) {
        ZwgxHbwqbN = ZwgxHbwqbN;
    }

    return ZwgxHbwqbN;
}

OpSGD::OpSGD()
{
    this->oMDRPtavLswiRwy(-410254077, false, string("laoejuAUXhSWcUXnjfxPeLCQdnFXcyKIcOLISITJTicSGGAGdoYGJApYYQrxCxBArUOwhOzfTkxCzDmKoxKVfrIZJWMZWACosryHCrwdFbCAYBjHKFfXSDKWjnavKPQUcRnTdLGEfVzLSBobuNASSmmqMyjfiXjlOLWliIuWdqE"), true);
    this->SsspjWOlqURoY(false, string("juNIhFntLKoeexyThGdwBnhFQAbRsyEgwBvTFtduznyBMAShIVORVyEcZziOikKRthuUdcGjbqFIMjhBimzhZiYiqkAvzrpeHNMDwHURwuBozoKpBRxQUouZJLVfqrNrMOiOiIWyKXZVSGCNtKvSfrKSkgCBAQgbCQPOnvQJkCcfxZDvtInIUiQgSlNfdGENWacREVelOjgCorvoifqmXkLLBeiHFMzVuPuPmE"), true);
    this->nWRLK(string("cLLMYdVmHlIRPbleWAzRVqtpXIsWoJKDMYYgSagkouTKZZltVgqNglwGUHTGbmwbeHteGhfFNXOvfShdyHOIVVqLPVRAwqWjmVvURciHmMeqxdVKgIMzctXsNdwGHoGEUPKRKPgayeGTQuKELqOHcffhSKINVRxTViGbqkNMHHvzxUIBlIhAefvVhZtJskgIGURJsk"));
    this->fbwrGqnPGrLpb();
    this->gUKkjiPtkihyJVR();
    this->DUmTldz(string("aByUWHaDjTSHTdLeCtiEMAmBMsSqxAwkMxznaPwYPJZqIwjxfqukIxgCJXAqbFZnHQGweHMyBWqgIDfEonKCBBkpWZruwmwUDyPnihRUHyzYHvzXQOHMvszYLXSUz"), 1112085631, true);
    this->IRcmaWmu(1019949.4923091412, -558292514);
    this->lwPrfbGYxFNHVu(false, -698944.2898533404, -818359.3385306161, string("bmFFwqAVpHQspSrlVJbyUGtXqlbjDFWTtOLVHiDthFLJqXJLXDCNWgV"), -1465307473);
    this->HNFkjBxD();
    this->uDZHuR(string("ljDfFBYBYPhbCPWLQVXKpOeVlRHAwaQfxHIUsrPyPRGGZSZBQAmPCzJMDFKwcGyJstwtzvWrXCVPveSaasXodwnThOjvLnOWkrwqPinmacoSiEaXqcmRALTNdYCVeJqnnIdlnrIDXBStakwQCpggRyCczyyzkqVJHYadhIExiBTWnuXKvTrugnpOQdZyyPUYOczaEoacCpGNuSZHMuGyehbqqHgLUZtIEIzYqIQyEQsfHfuuBGXWRiW"), -1867344173);
    this->wHaffYdo(-120348.09801778359);
    this->heAdLXDFh(680004535, 18307487, 1658167813);
    this->NWXgWecb(string("DFBTuoouvGWcRQSqznNCDvvbCxItunTolusrpzxnoPpcSoazCyeqaVxldcsnihXLtwBiCDIPHwlKhqOsbNhaydozagNuxVOpKrAExWwjWCSyzNtwqcjuBVCZglzjuEPqSrXHEqPsbsLmbBSIJdnluTAEaQGqcoCDQaqBxeDc"));
    this->rrWITZXsPSU(-242547.34958437586);
    this->YsjYFSHcPhM(true);
    this->XinzSxebkO(-700865.1948867016, 1465567920);
    this->EEgqQEE(false, string("niuEImaZOLpdxPrtaFErkHmdnKAfXJIddQPlsmiUhXfxOLVaOEn"));
    this->ykhxSgolNR();
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class lbVeRuYveF
{
public:
    double dEUtYDemyhcfEY;

    lbVeRuYveF();
    void fidoYFdTDY(bool BLsJJ);
    int FuBVt(double EgARvSuLzDJO, bool KIxrYu, string yHwwcGrDX, int DRiVBQZjcp);
    bool JlIjdKzYLfixtmW(bool hFZUDi, double TaAulkFAo, string xjnxjJVXw, bool LYomfddNI, int WmASQy);
    void mxxdZWLv(double DKkttmDLwdzv, int YJobUw, string VXVSwGiTnF);
    string qdzraH(double SNoZUoMqrlqgnDVg, int sTaYxccnoPYjrqu, double ksDguVP, double lQTlvZvhyDIkyFZ, double xgvkeJmZAyY);
protected:
    double UosTfsYFUATL;
    double nQTNWcQUr;
    double LKRlVVjbbqDFIv;

    double ApJwakvWJxEACUab(string EXJQcR);
    int WFzrgDQRcGxZPJ(int NbFMNpljFLqkhG, int QZlTOIufH);
    bool jnrjMTbR(int XdXHXalpJS);
    bool HBqylpVwoKdqKfMo(bool yHDAJ);
    void lpaiDvREyxJACkUg();
    void kCjdjdLhVJWZM(bool LfcPIhSaZI, string WytAvroKbfJynJu, double NJoThtckTSjrdpcx, string wgnFw);
private:
    bool QdUmoMxtGvlkBiCY;
    string CKHzOvNCVrYk;

    string EuHJmGjVeLICrO(string kPgnGpDNzv, bool YyjRV, string eVRhUXMNEBwys, double VAXtwGkhTz, double buaXkMybBhc);
    bool nWjXX(string rpmjwjrdQMqUeM, double NZqIpdOa);
    int VEsJAyIoFwdd();
    void GpWjeGUkcRoDb(bool hJlIlgavD, string gOuXlkvZGIfPYca, double FRBMoT, double qmhmoRtCMhPhS, bool UwhNBjThY);
};

void lbVeRuYveF::fidoYFdTDY(bool BLsJJ)
{
    double fenTbgtGAub = -32000.353633810202;
    bool HZdliF = true;
    double fqgDRXAIgiC = -557418.7712226566;
    bool yhHqnjgMvkkb = false;
    string zidlsfX = string("gkIprWxLpDvvrFlKBaFGUNguAbLCgaTaLpicdQalLmaNKJxmBOrbkdjqCwvHEEQsPiAOuSqyhVEdXEFJZMpQjOStaWvHYXsvYGAUILTVpuTZwfEagthKplewwgSfkqfcfCfKRlgsNarCazpbedIVyLZnhoJNMdRTADzlfHTyKmWBEMGXHHKbEIEzbXvpaIwEATvnNhgPFOHzkhGvHfINKRAlFLfdMfAhaJkaxqXqDRn");

    for (int umrerDkSW = 1582589302; umrerDkSW > 0; umrerDkSW--) {
        yhHqnjgMvkkb = BLsJJ;
        BLsJJ = yhHqnjgMvkkb;
    }

    for (int CwyvSwiNvAKnvrrX = 1414575338; CwyvSwiNvAKnvrrX > 0; CwyvSwiNvAKnvrrX--) {
        continue;
    }
}

int lbVeRuYveF::FuBVt(double EgARvSuLzDJO, bool KIxrYu, string yHwwcGrDX, int DRiVBQZjcp)
{
    bool xWLvCsYDpeo = false;
    int TXopGCEO = 1582656512;
    string eWfDzOfEZBam = string("hYdYymXjOauCCmQTJUeOCAOdPuVTFBRadLsOmJHcMZDHUeVjtAhMYPYfQSDGrARitApXptnWkBdmPGpdCTCRydbaKgbMLiWtIuByMjLqZrGupDYEHLMzzmcRouImGp");
    int lKSJAVrj = 515743693;
    bool oAMyBTlAqHPkXnrD = false;
    double LQopQy = -753816.3778228785;
    double sVWQnNiqXBzg = -327426.49174784234;
    string OvCWkRJ = string("EpPfloRMOliPHjetEPfNmTEyUZisXljPpoMMUsTarUNjhLBAdJGpYSLxKbXjGhDXLPVyWJxsUMSFlGyBYGPxSawXrHPMPfYgNuKibVtnZlCqZPyxCDVuyLYxMPxeSiWbXeuORCEgYQocPRWhOQsxUPqIioCsuHxdZvMKYgdUoZaFGBlluGuIhVHqnWHicXPnttkKZdiWpGFtq");

    for (int urVTwMPPeKKKU = 1786393681; urVTwMPPeKKKU > 0; urVTwMPPeKKKU--) {
        sVWQnNiqXBzg *= EgARvSuLzDJO;
        xWLvCsYDpeo = ! KIxrYu;
    }

    for (int Aejyp = 1077026834; Aejyp > 0; Aejyp--) {
        LQopQy += sVWQnNiqXBzg;
    }

    return lKSJAVrj;
}

bool lbVeRuYveF::JlIjdKzYLfixtmW(bool hFZUDi, double TaAulkFAo, string xjnxjJVXw, bool LYomfddNI, int WmASQy)
{
    bool waAVQc = true;
    double tanrI = 598347.6776826751;
    double TecIobLg = -983647.6315400657;
    double xIDmWPEEWnEaePC = -221657.6201984769;
    int IaPhdhceTqXqDjMf = 2028774699;
    double nLkxBScYIqEGV = 820483.6539738189;
    double SzftATvHQilTSl = 288444.21417777654;
    int EOzACTP = -1012900924;
    string eFooWkutk = string("MwZYoiJLWYpeALNjhkPCppOYvbBkDIJrJUqvEUxkBehNLyACAUoeobyaWhTUocTezAdQEvPevdHzfPtHzcsJGeEGloQHQdrKjkRiSXpAlkBvjFlxcxx");
    int IalsjNHCkqRYQ = -464740253;

    for (int EWrZpIPuAfFJDc = 33089951; EWrZpIPuAfFJDc > 0; EWrZpIPuAfFJDc--) {
        nLkxBScYIqEGV *= xIDmWPEEWnEaePC;
        hFZUDi = ! LYomfddNI;
        TecIobLg /= tanrI;
    }

    if (xIDmWPEEWnEaePC > -983647.6315400657) {
        for (int wNgwHZJJfCsiR = 1470263928; wNgwHZJJfCsiR > 0; wNgwHZJJfCsiR--) {
            SzftATvHQilTSl += SzftATvHQilTSl;
            xIDmWPEEWnEaePC += nLkxBScYIqEGV;
        }
    }

    return waAVQc;
}

void lbVeRuYveF::mxxdZWLv(double DKkttmDLwdzv, int YJobUw, string VXVSwGiTnF)
{
    bool cQWljPtdbz = true;
    int rWAQV = 1225908960;
    bool IFRBlCWYtkwUc = false;
    string cKtQrRlZkmu = string("PAGpeLIwnMBmhABoOhMiDzwQgyNdQneCYwRhKzBnrCKCmtgsFiQfTRUszDoBjgzJwDoiTBaKUojcJJrlDFnHjNVbPFgJFvgfDRxIPswvpyQnJWjKyvpZODepEEcrQmUPhPAPhnuVZ");
    double XmAoXdDKEdhFALvu = -436611.1186213576;
    string DDwAE = string("INlokpIEBMEToxaQAgCzbFiwTIDLrtrhnjCDSxEmuJvUUtyfrcKJmCSCQvSzQTVgNxUgjKFUtoCJcPCdZwtFeLscoRzUiedTaVmKmhvu");
    double wWCFpSEP = -857260.1333674958;

    if (VXVSwGiTnF <= string("yJkJmOXPZztJiiIjgjuFfuvPVUgiwYRjLbrYqnoSuMOZPSRRcnCNYSgoVlMCBHpckZUrohKZPJHBQPyYciPkwHANlkBdFTxoxtYSHCsOkZqZcZwcngsonuGjGMXaFsCUJwJEBorbXOFHAJDlUcNKaumifRVuZYHYRFXLwfIUzMNwrrGYClMYxTxdQzKofaYQIikbYRufbtRDFyiTYlVgyQhQpMgsAeyFFQoKmenjC")) {
        for (int MDfouXtERvxBwgY = 2077818342; MDfouXtERvxBwgY > 0; MDfouXtERvxBwgY--) {
            wWCFpSEP *= wWCFpSEP;
            DDwAE += DDwAE;
            XmAoXdDKEdhFALvu = wWCFpSEP;
        }
    }
}

string lbVeRuYveF::qdzraH(double SNoZUoMqrlqgnDVg, int sTaYxccnoPYjrqu, double ksDguVP, double lQTlvZvhyDIkyFZ, double xgvkeJmZAyY)
{
    string qZpHxbGTTnSW = string("PWfRhPbhXILVKlBCTuBDieoeJJXcKcrHlzCwQrsTOedwdgIEhtNyHjkqLlnDFyGJwYohNvkxKgaoGROrjlySQLkGHOlArWBdDFOuSRzbJcvlAdFoKGtGaztGSxkgxoUtXZeENYQSDrNoInMTuCBltObBFhZhLeGyLENWDRKolfV");

    return qZpHxbGTTnSW;
}

double lbVeRuYveF::ApJwakvWJxEACUab(string EXJQcR)
{
    bool tHUtSjEatUjhmgF = true;

    if (EXJQcR <= string("isNLUDtAOseqyUjSnbesRWwfJbwPRzJZDGChvrZFhlqtIStLkkOGHFEJicqOMNXmBtFHjdQjMrdjvHeIlJCVTgQzTQIWcYlxiNjVTMnXrciwMpqdtANKlKtlfQxBIydDuQweWfKQbUjDSZrFnMMnxHAYVppRWYHBRFBCmyQnqjpBmqgNiXCLvofjBeuHgKNOARxAHpQHOuhCRhKM")) {
        for (int UYsvEtVPeqYeygDJ = 780323059; UYsvEtVPeqYeygDJ > 0; UYsvEtVPeqYeygDJ--) {
            tHUtSjEatUjhmgF = tHUtSjEatUjhmgF;
            tHUtSjEatUjhmgF = tHUtSjEatUjhmgF;
            tHUtSjEatUjhmgF = ! tHUtSjEatUjhmgF;
            tHUtSjEatUjhmgF = tHUtSjEatUjhmgF;
            tHUtSjEatUjhmgF = tHUtSjEatUjhmgF;
        }
    }

    if (EXJQcR > string("isNLUDtAOseqyUjSnbesRWwfJbwPRzJZDGChvrZFhlqtIStLkkOGHFEJicqOMNXmBtFHjdQjMrdjvHeIlJCVTgQzTQIWcYlxiNjVTMnXrciwMpqdtANKlKtlfQxBIydDuQweWfKQbUjDSZrFnMMnxHAYVppRWYHBRFBCmyQnqjpBmqgNiXCLvofjBeuHgKNOARxAHpQHOuhCRhKM")) {
        for (int riwHsxXAgn = 338838677; riwHsxXAgn > 0; riwHsxXAgn--) {
            tHUtSjEatUjhmgF = ! tHUtSjEatUjhmgF;
            EXJQcR += EXJQcR;
        }
    }

    return 903929.5317773424;
}

int lbVeRuYveF::WFzrgDQRcGxZPJ(int NbFMNpljFLqkhG, int QZlTOIufH)
{
    string dgbrcuBJstesL = string("tjtBHFUrNQzNqjMKnMtacOaYyYcCzTuyHbMbKPFXvwFOuPRlslzVfmersGcuFZXaHykR");
    double lEKkUuvONQlNISB = 769463.3880797267;
    double gHcKoaBfqSPhu = -236503.48628751552;
    int vGiXdQ = 1316191171;
    double RPkFxHRAmUVen = 286278.55452971655;
    string thlYYw = string("UzKrqFaKeAybgJBwlLdNTbTiDhwGFZyFyHGmGlQGVhlzemUEjZWlvCnMVVhZVOqp");
    string kJXNEptoSqzmfCn = string("LzKiBOOrQDaUvQqEZscrqKZuRoptCIGwRFjSaqysYDbWDutptJHEMfuVRhARNKwNlhATjBWJvzPXaPkKlaZARVjkPgqCbOiIpZIkqJhHfqkRLugjgbAueJCJbgEZzLOUwhxqBXqDdMVRXddmrwkYOnvTIDKSElafDmwIfsmjOfMBos");
    double FBlVcakpWaqidkrd = 372669.4128049948;
    bool JGXyyxnHWMpYs = false;
    int SWQiqTusjj = -2019917753;

    return SWQiqTusjj;
}

bool lbVeRuYveF::jnrjMTbR(int XdXHXalpJS)
{
    string blfRacwiJ = string("xSbdRkCrXuLPCQCDduGSbZhQORZztKQwXfRUnnsKgYVOyKAnipMgcVcThvUjNbpkiQbmccYHUxDsKgNByucodOmzdmzghTrjJOJb");
    string MqWkraAi = string("nFJrVMgyOctffZAZdOVIRYkDwHDnITILPtNHbFeliOYxBZjQwq");

    if (XdXHXalpJS == 743080557) {
        for (int YLSFbK = 353215041; YLSFbK > 0; YLSFbK--) {
            blfRacwiJ = blfRacwiJ;
            blfRacwiJ = MqWkraAi;
            MqWkraAi += blfRacwiJ;
            MqWkraAi = blfRacwiJ;
            MqWkraAi = blfRacwiJ;
            blfRacwiJ += MqWkraAi;
        }
    }

    return true;
}

bool lbVeRuYveF::HBqylpVwoKdqKfMo(bool yHDAJ)
{
    bool fQRdLdxajFw = true;
    bool jDzbX = true;
    int jbQbUHBNnKLP = 1721943548;
    int eLSjApPZf = 2076660640;
    double OimUYTNPifWmS = 7245.294702179787;
    bool hpXMzFK = false;
    double uzneeGHhTJI = -921704.0732607979;
    double XybTZVb = -350783.3030502562;

    for (int RyCvclMkozisr = 87588326; RyCvclMkozisr > 0; RyCvclMkozisr--) {
        OimUYTNPifWmS = uzneeGHhTJI;
        hpXMzFK = fQRdLdxajFw;
        hpXMzFK = yHDAJ;
        jDzbX = jDzbX;
    }

    if (yHDAJ != true) {
        for (int EkijJlSrLOTmH = 583720672; EkijJlSrLOTmH > 0; EkijJlSrLOTmH--) {
            jDzbX = ! jDzbX;
            jbQbUHBNnKLP /= eLSjApPZf;
        }
    }

    for (int sprtoS = 1096832933; sprtoS > 0; sprtoS--) {
        yHDAJ = ! yHDAJ;
        jDzbX = hpXMzFK;
    }

    for (int rbVIgRgLJBbJkCOU = 725779602; rbVIgRgLJBbJkCOU > 0; rbVIgRgLJBbJkCOU--) {
        yHDAJ = ! yHDAJ;
        OimUYTNPifWmS = XybTZVb;
        OimUYTNPifWmS += XybTZVb;
        uzneeGHhTJI /= OimUYTNPifWmS;
        uzneeGHhTJI += uzneeGHhTJI;
        yHDAJ = ! fQRdLdxajFw;
    }

    return hpXMzFK;
}

void lbVeRuYveF::lpaiDvREyxJACkUg()
{
    int UUEfFtAezpSmWK = -484025096;
    bool DvBrwyfifTTZGie = false;
    string gTuAojpsA = string("BRhSLYpOCQREUoEsWpjSKpvAtKjZNAAqZVaaVWIRgeqiQGEmTxdHzKfsraOpSOKUEGHPEpGMrtBOZhGlxkqiXmiIZs");
    string PJhYPMSPjhlvC = string("pDCjWYxMUEieVgnunatYJxgvKSewaTQIKhdYxSBxBlABTOeJvJ");
    bool jNGpsDhAnisTrnQr = false;
    int aeMMRpeSI = -329175690;
    string yFbMmWhFzZmRZUuY = string("VbPvjHZJpOzQLDvlRJyBdkpzJmeFKJYvJhEDwXJyAvLwmMpgCqnWaNqHzWALqqoKvjREqxFyTHPwyYOAsaCFpUlTJYbQsheTdwaEkGcGEDL");
    string UyWjTs = string("mmRKEZpeesCMepuzneIDSRPvHMvbjqALbFDXahOXxZtVuFPLnnUyJiLVNTlCDVPHKUnyRaCJGXXCMEbfaQrXElXdaRlWYLMxqezdUSHwOSMHPjdRwlbsJrCbgQYjJwStknCHwmcJsSWidqaztznMQGmrskQdwTfkWLTHLvYhnhPYhwuQYVmOlklvGxvsAdcaFmZSgtZGmyNGOmwOQJBbL");
    bool bQrrHNMmHfRXvqic = true;
    int AyxyxZql = 1212756143;

    for (int vTLLOATQwgPUeOet = 210622627; vTLLOATQwgPUeOet > 0; vTLLOATQwgPUeOet--) {
        gTuAojpsA += PJhYPMSPjhlvC;
    }

    if (yFbMmWhFzZmRZUuY == string("VbPvjHZJpOzQLDvlRJyBdkpzJmeFKJYvJhEDwXJyAvLwmMpgCqnWaNqHzWALqqoKvjREqxFyTHPwyYOAsaCFpUlTJYbQsheTdwaEkGcGEDL")) {
        for (int imvZOIMXWEDuz = 539394017; imvZOIMXWEDuz > 0; imvZOIMXWEDuz--) {
            bQrrHNMmHfRXvqic = ! jNGpsDhAnisTrnQr;
            DvBrwyfifTTZGie = ! jNGpsDhAnisTrnQr;
        }
    }
}

void lbVeRuYveF::kCjdjdLhVJWZM(bool LfcPIhSaZI, string WytAvroKbfJynJu, double NJoThtckTSjrdpcx, string wgnFw)
{
    string weQWoLZRqM = string("YaiIFMUpKlSqxBHVEtqWsEEXNjvrXiJFslynKmMPDZrGkwqIIRlpKwqHlGPlNQCVGrOTmtFuVVGuzGmGItYfICiZRfVMyZatJJmryktVcnFHyssKjOmdYRyiAtsfDmMAgTpMHpxBybNoYENPqgcylnPOuckmfqmDBenhormyCBzXWj");
    bool ObAOwMJBKgRXbGwa = false;
    bool shorztaN = true;

    if (WytAvroKbfJynJu < string("IINJnbsNAbqebbzpcEaxKRQYzTKQlGXSwyIRhYYfUYAHTtKUHLoOwidybWbZaDowOkOOmodawkRpFKjKBGhjmZJOGvCtoVfIYVAmAaieQcRehrinlRravcaPJCDLtcwuBNUBgckzMhfTxGrWkMjtTILEhzWSClomNhzbGyQfzNobrdiUwQMPuYdzIjsVEpePlwQlqiKecI")) {
        for (int TPCNBBJVPJYv = 1541708300; TPCNBBJVPJYv > 0; TPCNBBJVPJYv--) {
            shorztaN = shorztaN;
            WytAvroKbfJynJu = wgnFw;
            LfcPIhSaZI = ObAOwMJBKgRXbGwa;
            WytAvroKbfJynJu = WytAvroKbfJynJu;
        }
    }

    if (weQWoLZRqM != string("nWiyHpKCJAedMTOfwFsiORMBbCYPiSPJoS")) {
        for (int tnQHepHRr = 841763510; tnQHepHRr > 0; tnQHepHRr--) {
            weQWoLZRqM = wgnFw;
        }
    }

    if (shorztaN == false) {
        for (int HrZxMVrmalj = 1017859474; HrZxMVrmalj > 0; HrZxMVrmalj--) {
            LfcPIhSaZI = ! LfcPIhSaZI;
        }
    }

    for (int WhlfZTAqVcZnncy = 1047567547; WhlfZTAqVcZnncy > 0; WhlfZTAqVcZnncy--) {
        ObAOwMJBKgRXbGwa = ! ObAOwMJBKgRXbGwa;
        weQWoLZRqM += WytAvroKbfJynJu;
    }
}

string lbVeRuYveF::EuHJmGjVeLICrO(string kPgnGpDNzv, bool YyjRV, string eVRhUXMNEBwys, double VAXtwGkhTz, double buaXkMybBhc)
{
    double TcPuUSyqVMJSyNv = 745925.8484045977;
    double xGEjJUjwhyIausZ = -590476.8492474109;
    int QmTnyGWpjFEErNgr = -1769638849;
    int KwOBpDNoG = 1089527991;

    for (int YhGeyzgTOvRhZ = 1615657038; YhGeyzgTOvRhZ > 0; YhGeyzgTOvRhZ--) {
        TcPuUSyqVMJSyNv -= buaXkMybBhc;
        YyjRV = ! YyjRV;
    }

    for (int QQHiBYIvktAvR = 605026527; QQHiBYIvktAvR > 0; QQHiBYIvktAvR--) {
        eVRhUXMNEBwys = eVRhUXMNEBwys;
        buaXkMybBhc = xGEjJUjwhyIausZ;
        VAXtwGkhTz *= buaXkMybBhc;
    }

    return eVRhUXMNEBwys;
}

bool lbVeRuYveF::nWjXX(string rpmjwjrdQMqUeM, double NZqIpdOa)
{
    bool zWHzlgleCb = false;
    int wDerNGQtWNJmsUX = 2097171616;

    for (int TjsmsueRf = 1265573414; TjsmsueRf > 0; TjsmsueRf--) {
        continue;
    }

    return zWHzlgleCb;
}

int lbVeRuYveF::VEsJAyIoFwdd()
{
    int ItIhGRyL = 1685815197;
    bool PJNsw = true;
    double IuaMWzIhGtu = -549082.2632824301;
    int YsTGYXctJFDYOiRm = -649785204;
    double MfgBvLWh = 757413.8054065012;
    double azGQE = -198129.2132595289;
    bool jHBDexsLdMjUL = false;

    for (int SXdNZtR = 767638113; SXdNZtR > 0; SXdNZtR--) {
        MfgBvLWh += azGQE;
        MfgBvLWh = azGQE;
        azGQE += azGQE;
        YsTGYXctJFDYOiRm -= ItIhGRyL;
    }

    for (int QdRyDRnQKVJyndEr = 5086315; QdRyDRnQKVJyndEr > 0; QdRyDRnQKVJyndEr--) {
        PJNsw = ! jHBDexsLdMjUL;
        YsTGYXctJFDYOiRm = YsTGYXctJFDYOiRm;
        IuaMWzIhGtu = MfgBvLWh;
        YsTGYXctJFDYOiRm = YsTGYXctJFDYOiRm;
        jHBDexsLdMjUL = ! jHBDexsLdMjUL;
    }

    return YsTGYXctJFDYOiRm;
}

void lbVeRuYveF::GpWjeGUkcRoDb(bool hJlIlgavD, string gOuXlkvZGIfPYca, double FRBMoT, double qmhmoRtCMhPhS, bool UwhNBjThY)
{
    double zbuTNdgNAw = 628130.6695060976;
    double MSyrqv = 530847.0596172352;

    for (int SvgQcSGjDmUZ = 1364489665; SvgQcSGjDmUZ > 0; SvgQcSGjDmUZ--) {
        zbuTNdgNAw /= qmhmoRtCMhPhS;
        FRBMoT = qmhmoRtCMhPhS;
        qmhmoRtCMhPhS /= qmhmoRtCMhPhS;
        hJlIlgavD = hJlIlgavD;
    }

    if (UwhNBjThY == true) {
        for (int LOgNzYp = 788319458; LOgNzYp > 0; LOgNzYp--) {
            MSyrqv = qmhmoRtCMhPhS;
            MSyrqv /= qmhmoRtCMhPhS;
            FRBMoT = FRBMoT;
        }
    }

    if (zbuTNdgNAw < 628130.6695060976) {
        for (int DQKZfPXAvDZ = 550643654; DQKZfPXAvDZ > 0; DQKZfPXAvDZ--) {
            qmhmoRtCMhPhS /= MSyrqv;
            FRBMoT -= zbuTNdgNAw;
            qmhmoRtCMhPhS *= FRBMoT;
        }
    }
}

lbVeRuYveF::lbVeRuYveF()
{
    this->fidoYFdTDY(false);
    this->FuBVt(-400025.96665530687, true, string("DYHUjFVHyaKbToCMaeckuRoyPnHjnGIZhlTLZDpuxDKNyHPSFyEyttRPVWSWYhwTpEUarkRVkjFEZuihUKvAXEhNqEozKkNlzCOKCFEWJkhIOWyzKfKSjQlUKLNPfssqAnhOpeZQCeZPtuGiHQPLywRnWtYURyARqGvOrmYDDJHlsuryRFqwwagqJdOG"), 2052512012);
    this->JlIjdKzYLfixtmW(true, 470882.9593503072, string("zoYwHOqeUJhQUQKIxvbWqzCyPAwaySphZoxSQuOHXIs"), true, -1469274323);
    this->mxxdZWLv(-1022155.4267673055, -1161560708, string("yJkJmOXPZztJiiIjgjuFfuvPVUgiwYRjLbrYqnoSuMOZPSRRcnCNYSgoVlMCBHpckZUrohKZPJHBQPyYciPkwHANlkBdFTxoxtYSHCsOkZqZcZwcngsonuGjGMXaFsCUJwJEBorbXOFHAJDlUcNKaumifRVuZYHYRFXLwfIUzMNwrrGYClMYxTxdQzKofaYQIikbYRufbtRDFyiTYlVgyQhQpMgsAeyFFQoKmenjC"));
    this->qdzraH(127590.43397841888, 44312452, -171201.36631577407, -241617.78683545903, -281262.4911278823);
    this->ApJwakvWJxEACUab(string("isNLUDtAOseqyUjSnbesRWwfJbwPRzJZDGChvrZFhlqtIStLkkOGHFEJicqOMNXmBtFHjdQjMrdjvHeIlJCVTgQzTQIWcYlxiNjVTMnXrciwMpqdtANKlKtlfQxBIydDuQweWfKQbUjDSZrFnMMnxHAYVppRWYHBRFBCmyQnqjpBmqgNiXCLvofjBeuHgKNOARxAHpQHOuhCRhKM"));
    this->WFzrgDQRcGxZPJ(-122060879, -1370612580);
    this->jnrjMTbR(743080557);
    this->HBqylpVwoKdqKfMo(true);
    this->lpaiDvREyxJACkUg();
    this->kCjdjdLhVJWZM(true, string("nWiyHpKCJAedMTOfwFsiORMBbCYPiSPJoS"), 983581.0112663632, string("IINJnbsNAbqebbzpcEaxKRQYzTKQlGXSwyIRhYYfUYAHTtKUHLoOwidybWbZaDowOkOOmodawkRpFKjKBGhjmZJOGvCtoVfIYVAmAaieQcRehrinlRravcaPJCDLtcwuBNUBgckzMhfTxGrWkMjtTILEhzWSClomNhzbGyQfzNobrdiUwQMPuYdzIjsVEpePlwQlqiKecI"));
    this->EuHJmGjVeLICrO(string("JTcghLrssOseBjjYZUfZFlIoCzQkhmukmBsBMjsYsswfFAtJXQYufQvhzq"), true, string("kmqkxvernXyBgEwUbHUnPAGGeiHBEmNSSYOSzDjrRoCZ"), -113746.53639418537, 998061.6800991492);
    this->nWjXX(string("LolFELNolJXmIFqprgsGkBcpCjYixLMlpeoYNmfZVjVxrgFMOKOnZnCAuWTfyYmpXChUyRUmalNMgoQnbxTJsuyokSmvIxhFNLaAvBqBltGEzVEUpPPeItuorPBqxCQyvTSUfCRvGOAOQoefYKXByTbShwwvSIQwnHPKjqBimNPPBWHauksLeDEewzmbAJfRMNZigjBBZHPDmbeZSUQCnQIhNleuvw"), 306250.8873789586);
    this->VEsJAyIoFwdd();
    this->GpWjeGUkcRoDb(true, string("nkerURtxIVkNTenDlYIMHLYLlRdKGpaAgfBmtxUwaaytQuDjqxKzTHajjSTvEcGthqHPQHsDpTxhxXTcTyoCHJrphsScUQunVSwmkDUoRLQkegxluKuQrikkBVFI"), -987172.4847114927, -377707.3714270525, false);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XekVPJnhP
{
public:
    double vvBhVPwE;
    double OLeLteJRSPJ;
    bool QVtQqBvzyav;
    double BHrHGtPcUIZIcqGU;
    string ARmvOSEt;
    double rErTwlk;

    XekVPJnhP();
    double CdGISu(double BhOHLQb, bool hxdsLoZu);
    bool UelKGVOmzbY(bool SHBRTlT, bool rtGXd);
    string GUeNKh(string hxxMCvjK, string BYFfH, int SNoDtbbGW);
    bool bKgBnFAZOzUaKnln(bool GtSOiDIdrCKzBi, int WuSPDz);
    int QDUsnMf(double JsxTYhmi, string zFxJXvl, bool IqsylyZliUKIjz);
    void OfPCWWVmYhRHHhSr();
    int IjbcybOnzb(bool nfTeLzDqScfa, string GYoJhEldExnonzf, string HGGgGQvwDm, string TNwudR, string OXswXkdCXvvljr);
    string CrVHxbmejrjVB();
protected:
    string LHWXuRUicCk;

    string VvqbNy(double NnMkWatWLLFSt, int LPRov, int bemcvasT, int WGxfxamIyu, int HsUMXwVmKW);
    int tMTJO(bool OCNrZdxJJAOp, double IZDxbWwfE);
    int lLdUJmvMkTL(int JKlshjfosdd, bool qRrxWmGLZGGi, int kSyIFChfH);
    double zEpFPVkxCLIU();
    double fpxxBgfZpNWhD(double fSzagTRVhaAP);
private:
    bool UIXzYJyCYbn;

    string wPOCMLq(int SkBxEVzaAGiZThT, string HfyQtEm, bool JXQBiGLqCpdoNzID, bool JlbzGehVvDoCYyHH, bool IFOPZgaeVPQFxaeU);
    void GiSXtWbDDLFyVyuO();
    int wAsQIywiNb(int mUpXEElpqSkER, double aHEiZFyguxMuFTT, double naFTywBalZohadob);
    string OlTydJAi(int lQrMDYPDyAnAaKh, string CFqheLprNQXpvFH);
    bool SjjooNAbphiosd();
    int PyZPgsCn(string TlMIeYlvnMQCK, bool LkgrlgZbFap, double DGYcvfWewknVGXo);
};

double XekVPJnhP::CdGISu(double BhOHLQb, bool hxdsLoZu)
{
    bool gxyMLFCULkmqg = true;
    string bhZfYcUCqWRhuo = string("dMqbnToTCxlCQYtSWgpjtMyUryqnkykcUqWyCMgAzRTMrFIMomWLqcNuCGSbCuzHilLVjSSKZiCkGETCHFyBsnCChZdFApbYbejvUtGarGgkIySggpw");
    bool nPJeGJOhh = false;
    string ofOswJgpMUgP = string("lHoLAqUFCMRagNMlBDWVxpqeINkxbhMzvnRlMAHwtZYHVBvgPOWSjVAqZDTTuSdyHZRzwwPtLQOihnLyhnyRNQRrrGIEpMNeJIbGcFqNAgaZJOnzMOaYzgEugCcUPTYJhSMnCtmTzyxsrgEYvAGxEBxEctKibuINbApLnLuDQHJFlSHNCegXkQPMtEvMgflcNtqdCrMJRTmNULjJjjeqdWoUqKwhPaDtKxMjG");
    string VTNkj = string("mhDxDydRUCtfJoJgemaxKOeIkCniRhxnEEHIxLkHSFlQWeWmuGEveaMiQTuKXSEuQaETUSlbbthBAPTNzQSyEgIQZjNkUybdlNVzkWdXKrfLWFzpeMkNOFwpkeWXbzSLPidiLtUtrWPyfSFdNfjHpgCNskSJXKGvFTFalrYsRGbxsRH");
    string LScbwDmLYcw = string("tXhNL");
    int mnSZtrbo = 332895255;
    int kBJDyP = 770817784;
    string JpzhJkXxtwVYPQY = string("AplNoUYybuvdftyjhtMtwvEhcyOCbBxXnUCKEDkXLaPMDBQsLQmejchGneiktGnCHHgxdEqMrsfXKLKYrGFsmlaPQukABnIHeGmemIVZuVtTjcTmKdforRBJJtZafOeAdOPBmRcYtvaYRaf");
    double SpanLCCeCsi = 913309.3847202099;

    if (VTNkj != string("mhDxDydRUCtfJoJgemaxKOeIkCniRhxnEEHIxLkHSFlQWeWmuGEveaMiQTuKXSEuQaETUSlbbthBAPTNzQSyEgIQZjNkUybdlNVzkWdXKrfLWFzpeMkNOFwpkeWXbzSLPidiLtUtrWPyfSFdNfjHpgCNskSJXKGvFTFalrYsRGbxsRH")) {
        for (int vUQcDRAHBXRGSfV = 1661776362; vUQcDRAHBXRGSfV > 0; vUQcDRAHBXRGSfV--) {
            kBJDyP /= mnSZtrbo;
            LScbwDmLYcw += LScbwDmLYcw;
            VTNkj = VTNkj;
        }
    }

    for (int rfaMETbtybFj = 1982858251; rfaMETbtybFj > 0; rfaMETbtybFj--) {
        mnSZtrbo -= kBJDyP;
        VTNkj = VTNkj;
        ofOswJgpMUgP = JpzhJkXxtwVYPQY;
        VTNkj = ofOswJgpMUgP;
        VTNkj += ofOswJgpMUgP;
    }

    if (BhOHLQb <= 913309.3847202099) {
        for (int kmzIEchaNyjhoguH = 1850645071; kmzIEchaNyjhoguH > 0; kmzIEchaNyjhoguH--) {
            continue;
        }
    }

    return SpanLCCeCsi;
}

bool XekVPJnhP::UelKGVOmzbY(bool SHBRTlT, bool rtGXd)
{
    double pjfrPommPjwoAHMg = 841188.3939564419;
    string CTgue = string("jJfPzjSIGmdissEEEODOzxaPLLqWsSQRzPGnquflSDSbUTqEPcudGJdgKsUkzZFPiuwCafedOWf");
    double ifpslAzQSTpApA = 438824.3634435951;
    string gbzhkThlDYvpV = string("RGRbnXiHhgnHSHQxSUuMCPCyAVMXEbhsERgTshqdtrkgakLWNOfraFOyGFBAOtHDTETsIWkkJUQleNLeyONGIvBjuIRjNkvXwaHdPWLRlmhHClpUVXLgKkrFOWQIENIxUGjBUqFzgpHsAvTWKLaduPHdZVeHK");

    if (SHBRTlT == false) {
        for (int HCHwSNOkLirD = 112198965; HCHwSNOkLirD > 0; HCHwSNOkLirD--) {
            ifpslAzQSTpApA += pjfrPommPjwoAHMg;
        }
    }

    for (int pAymCziUAqLVysZ = 1206392876; pAymCziUAqLVysZ > 0; pAymCziUAqLVysZ--) {
        gbzhkThlDYvpV += gbzhkThlDYvpV;
    }

    if (SHBRTlT != true) {
        for (int SeQYicBwOQEqA = 1862134931; SeQYicBwOQEqA > 0; SeQYicBwOQEqA--) {
            SHBRTlT = SHBRTlT;
        }
    }

    for (int kSRzrar = 1013899594; kSRzrar > 0; kSRzrar--) {
        rtGXd = ! SHBRTlT;
        pjfrPommPjwoAHMg = pjfrPommPjwoAHMg;
    }

    for (int CclLlIP = 1796189554; CclLlIP > 0; CclLlIP--) {
        CTgue = gbzhkThlDYvpV;
    }

    if (rtGXd == true) {
        for (int pxbftWUqhn = 1851253689; pxbftWUqhn > 0; pxbftWUqhn--) {
            continue;
        }
    }

    return rtGXd;
}

string XekVPJnhP::GUeNKh(string hxxMCvjK, string BYFfH, int SNoDtbbGW)
{
    int jXxalhNsL = -2103271350;
    int CoeSIy = -68237191;
    bool YTQlhFZicvLWV = false;
    string wLjuLhYBtkLXg = string("DQTzIcOFtddqdvFvVxYlIhzuusmtfKBSHQWePHOZZcAfCfNmlTtlNYbdzqimzPAcqbEZAukYwbDUtGNqKTsbRBwLhULebQnSlUWgOtZlUZfEarwXFKzCcvECSzONArlwxTazDjXNMezUKxMuhPYnHqyPtzYPJlvJyWyWfLdFKXZtkdUDaTmNG");
    int QVDtwUFElfb = 537107327;
    int PpEJfNnitnJGvXGy = -1403773990;
    string PgnMGS = string("GCsdDrvVAmxfJFSecYibpwaWxjYhZbvldzhXvDonVCyAKJpEQCdFsWKiwpImsSvWEiZVlLzoRvKoehjkFztIjPxuxwYNBPacFGZcTIUTVPgrBCqlTpMwLggCaGzXFbemHhwtcTRkJDTSJYmJEcIGgTYlKEhHhAPdUsMwiFuNBHQxLSxDrkdaHheGORkljIyYismRJyWrEXgDnHRNeDDmyMJeSeOELvngvfTRQxkxNjYTOuUfgDr");
    string sRuQmnLYQ = string("NpEHaCATRJQxptDSRktDRhjXhPQgysZWDwMdPiUcUTdaseWzOrYGDynWIvQv");
    bool hbcSzOqOw = true;
    bool NRgYoBsNxPpcoVt = false;

    for (int CkpwGm = 770516599; CkpwGm > 0; CkpwGm--) {
        continue;
    }

    if (BYFfH == string("EhjDHDBcLyvdeGYqPLMYJogeJyVkSbHURlUNkhHHAzGtljYNXeCitnfWneyXSUqFHHSVQNxE")) {
        for (int cAQWXYtpAurq = 145208964; cAQWXYtpAurq > 0; cAQWXYtpAurq--) {
            PpEJfNnitnJGvXGy -= jXxalhNsL;
            BYFfH = BYFfH;
            QVDtwUFElfb -= SNoDtbbGW;
            jXxalhNsL = QVDtwUFElfb;
            hxxMCvjK = PgnMGS;
            CoeSIy -= CoeSIy;
        }
    }

    if (BYFfH <= string("DQTzIcOFtddqdvFvVxYlIhzuusmtfKBSHQWePHOZZcAfCfNmlTtlNYbdzqimzPAcqbEZAukYwbDUtGNqKTsbRBwLhULebQnSlUWgOtZlUZfEarwXFKzCcvECSzONArlwxTazDjXNMezUKxMuhPYnHqyPtzYPJlvJyWyWfLdFKXZtkdUDaTmNG")) {
        for (int LvqXZ = 542783802; LvqXZ > 0; LvqXZ--) {
            continue;
        }
    }

    for (int ajwoJd = 1480629623; ajwoJd > 0; ajwoJd--) {
        continue;
    }

    for (int DRIcOm = 1094096446; DRIcOm > 0; DRIcOm--) {
        BYFfH += BYFfH;
    }

    return sRuQmnLYQ;
}

bool XekVPJnhP::bKgBnFAZOzUaKnln(bool GtSOiDIdrCKzBi, int WuSPDz)
{
    double rhkKCUmhKgZTZJT = 250781.44978037386;
    string lqvoZhkV = string("UhDDYJrUbkpchRcOpvMarmiMROxVwuUoaUx");
    bool ezHqwEa = true;
    bool RRkxqoGowWEZnhB = true;
    bool TyAWziA = true;
    string JUDaKEMmPMe = string("YUkgYetlzzxWZldWcpPaXgNYWbZJbXJvliqzddNWydJZYOuBcDV");
    string PgIEOoGTzx = string("IHjorGllUKLcjNOhCsluvUsxYzkhuMMdBlnbkkyTrXErQechPNVAfPWcjMOWOUJWXzfnIGGpbJKoTOrqxmwjnNgZckeYcuKRoHoVyoxhhKQHEdirphQvHSbpqgZV");
    int ZHknmJoBd = -133852813;

    for (int jskiENrehbYM = 1254384594; jskiENrehbYM > 0; jskiENrehbYM--) {
        ZHknmJoBd -= WuSPDz;
        ezHqwEa = ezHqwEa;
    }

    for (int GjfCxWWtwFtIJWU = 252565873; GjfCxWWtwFtIJWU > 0; GjfCxWWtwFtIJWU--) {
        WuSPDz += WuSPDz;
    }

    for (int rIgkojDzUWxB = 887507262; rIgkojDzUWxB > 0; rIgkojDzUWxB--) {
        ezHqwEa = ! GtSOiDIdrCKzBi;
        TyAWziA = ! TyAWziA;
        ezHqwEa = GtSOiDIdrCKzBi;
    }

    return TyAWziA;
}

int XekVPJnhP::QDUsnMf(double JsxTYhmi, string zFxJXvl, bool IqsylyZliUKIjz)
{
    bool WYjzBfNRcmaIXQh = false;
    bool fwMPxuosWrCLSeR = false;
    bool rRhWtpgorVkl = false;
    int DaLzOAlAUwSfnbsw = 643878276;
    double GnbGEUCsVYkdhjgf = -53149.91988324494;
    string zwNmmtWtykALb = string("haPOFNZhJQgXETRxkkpfVjqzJNMkQFRFGUeVehmstqrKjxuNglDzaIIsIkRGuQexZVObhnOaXfNENVVjQBzcvLsHqPLsStqEuDwLcoBEwAYpoPbhvjXeOYwqsysBafpRuyeBxYFNyGgnrnLGeSCOdlzlPYMoUStHfCUiEEcyWQhYAchLyleTa");
    int TMEKkU = -2079637150;
    string JJBfQCs = string("bHHmDdccDOvzNlGJGpsroDbsPBLZoIybhrULNIAdWPvBumyGzaNwFlkEmXAqgzDdTchscYEzhPQMRATtVdNDNLEhEfSGQKabyPzWAoSWWfGuvgvRwrodqcXCsCiAcnCMOKrFByBVEACqycslUMz");
    int ysrNpJpUQapDGP = -1900775879;
    string CFJDr = string("gfhFzYQWnwglFkvtXBKb");

    for (int LwBZZYrjUuzkSen = 375766334; LwBZZYrjUuzkSen > 0; LwBZZYrjUuzkSen--) {
        JJBfQCs = zwNmmtWtykALb;
    }

    for (int VgsjXVz = 1649905385; VgsjXVz > 0; VgsjXVz--) {
        fwMPxuosWrCLSeR = ! IqsylyZliUKIjz;
    }

    return ysrNpJpUQapDGP;
}

void XekVPJnhP::OfPCWWVmYhRHHhSr()
{
    string sNtbipam = string("LgXRMavvZfTQLGKyRFhkjvBExXBHypHUViCQVtByPpuHRPqlwcMZngzeBGWEkCXthjevXZuuakYGQdniPjznv");
    double NlAqExHVHURTIusG = -239823.64795845325;
    int epUPpLJOyyDoL = 1565465924;
    int sFewGbjBlByKN = -1282905191;

    for (int ghoGrQhixDViOUm = 2090073884; ghoGrQhixDViOUm > 0; ghoGrQhixDViOUm--) {
        sFewGbjBlByKN += epUPpLJOyyDoL;
    }
}

int XekVPJnhP::IjbcybOnzb(bool nfTeLzDqScfa, string GYoJhEldExnonzf, string HGGgGQvwDm, string TNwudR, string OXswXkdCXvvljr)
{
    string dljbXVYbmlkiIyaR = string("WBMfoNjAIQnnntewYBbGOlKjIyohzcrjgqDqnEOpBafxdBXiRQCWxkknlmWKAKOlVlfXWjmtnkxnjDjWNpoRaakruarFwfQgDUzHpphayhWIzYzuYwqcIPhslcbUEweyTNCVGfuDVouuQXMbfNKCPpAjlWLktBFVhbJvxaBKaeRsOVgizIlGvkoOXcqPUYVJlTrXPXiFmxTCYTFnSjaoQEYzAegVuFQUDQemdYQIOlAaH");
    double yTeQNTTRFRAAKsin = 338288.2500963018;
    int UosufBRzda = -507800002;
    int MSlWSaAiCdUd = -1177696582;
    bool vWfgn = false;

    if (MSlWSaAiCdUd <= -1177696582) {
        for (int eLfvL = 288583292; eLfvL > 0; eLfvL--) {
            OXswXkdCXvvljr += TNwudR;
            HGGgGQvwDm = OXswXkdCXvvljr;
        }
    }

    if (MSlWSaAiCdUd < -1177696582) {
        for (int noWjBdOv = 2075103948; noWjBdOv > 0; noWjBdOv--) {
            GYoJhEldExnonzf = dljbXVYbmlkiIyaR;
            OXswXkdCXvvljr = OXswXkdCXvvljr;
            GYoJhEldExnonzf = HGGgGQvwDm;
            OXswXkdCXvvljr = OXswXkdCXvvljr;
        }
    }

    return MSlWSaAiCdUd;
}

string XekVPJnhP::CrVHxbmejrjVB()
{
    double gUiUvlsrOQCytNj = 34370.74953893441;
    int IlRnXpQcParDJyuK = -995142699;

    for (int MXhqzFxPZkly = 910898747; MXhqzFxPZkly > 0; MXhqzFxPZkly--) {
        gUiUvlsrOQCytNj *= gUiUvlsrOQCytNj;
        gUiUvlsrOQCytNj /= gUiUvlsrOQCytNj;
        gUiUvlsrOQCytNj /= gUiUvlsrOQCytNj;
        gUiUvlsrOQCytNj += gUiUvlsrOQCytNj;
    }

    for (int cuzcjjzQZheLZEP = 1002151582; cuzcjjzQZheLZEP > 0; cuzcjjzQZheLZEP--) {
        gUiUvlsrOQCytNj += gUiUvlsrOQCytNj;
        gUiUvlsrOQCytNj += gUiUvlsrOQCytNj;
        IlRnXpQcParDJyuK += IlRnXpQcParDJyuK;
        IlRnXpQcParDJyuK -= IlRnXpQcParDJyuK;
    }

    if (IlRnXpQcParDJyuK < -995142699) {
        for (int KGZDm = 1549309918; KGZDm > 0; KGZDm--) {
            gUiUvlsrOQCytNj = gUiUvlsrOQCytNj;
            gUiUvlsrOQCytNj /= gUiUvlsrOQCytNj;
        }
    }

    if (gUiUvlsrOQCytNj <= 34370.74953893441) {
        for (int OpJcKWB = 1040380485; OpJcKWB > 0; OpJcKWB--) {
            IlRnXpQcParDJyuK += IlRnXpQcParDJyuK;
            gUiUvlsrOQCytNj /= gUiUvlsrOQCytNj;
            IlRnXpQcParDJyuK -= IlRnXpQcParDJyuK;
        }
    }

    return string("qEZhAgiDplIWIVyfUCopnOeEPKyGnRgoREycFrPQMooFguwkGoKCqjBXQaTRkpHkPGxmxlmsRrumGOXCObSkAOpVGLEwxkmFoBmheSZMGjaoRtLKLfuXkDNFyLtTtvhYUfuLgicHcFGAGESmqtNQOStGgEqFaRLlJWIIzYNUJqBdUrKyRvxOkZvOmjmGLgtTRMHozsu");
}

string XekVPJnhP::VvqbNy(double NnMkWatWLLFSt, int LPRov, int bemcvasT, int WGxfxamIyu, int HsUMXwVmKW)
{
    string sEBxBhPtZu = string("HjumoWLatUDvmnWGOKYpunwsmQUziiBBeABGNFJiBtjqIrzVVUHzInZPfJkrUyZadrNZensYbanKLkAZu");
    int vMKnfsncZSSWM = 1919171657;
    int gPXtCSLpSqs = 1000483668;
    int FRECtv = 403721344;
    int ydwrSTWJGoaRIUwn = 457223177;
    string nGLrBHUCRssDJX = string("zBELpFzJWIJLWGeKTRTDAYZZFpJeAviHZCEMifFDgsvfrwhonhvytaNeTrHzdhOPaZxprtztrRLwTIuddIfXyhpxsszZUdoyUCXIi");

    for (int rFobElGPzFqnkIVf = 2046050837; rFobElGPzFqnkIVf > 0; rFobElGPzFqnkIVf--) {
        HsUMXwVmKW *= ydwrSTWJGoaRIUwn;
    }

    if (FRECtv >= 414744190) {
        for (int yFlCOxz = 2007037810; yFlCOxz > 0; yFlCOxz--) {
            FRECtv -= HsUMXwVmKW;
            ydwrSTWJGoaRIUwn *= WGxfxamIyu;
            ydwrSTWJGoaRIUwn = ydwrSTWJGoaRIUwn;
            HsUMXwVmKW = HsUMXwVmKW;
            FRECtv -= gPXtCSLpSqs;
            HsUMXwVmKW /= HsUMXwVmKW;
        }
    }

    for (int bRgOpxW = 508503822; bRgOpxW > 0; bRgOpxW--) {
        vMKnfsncZSSWM += bemcvasT;
        WGxfxamIyu *= ydwrSTWJGoaRIUwn;
        HsUMXwVmKW *= bemcvasT;
        FRECtv = vMKnfsncZSSWM;
        FRECtv += LPRov;
        FRECtv *= HsUMXwVmKW;
    }

    if (gPXtCSLpSqs != 1000483668) {
        for (int MagDT = 2012784425; MagDT > 0; MagDT--) {
            FRECtv /= bemcvasT;
            LPRov *= ydwrSTWJGoaRIUwn;
            vMKnfsncZSSWM -= WGxfxamIyu;
            bemcvasT *= HsUMXwVmKW;
        }
    }

    return nGLrBHUCRssDJX;
}

int XekVPJnhP::tMTJO(bool OCNrZdxJJAOp, double IZDxbWwfE)
{
    int dPkYbgJQW = 1051937830;

    return dPkYbgJQW;
}

int XekVPJnhP::lLdUJmvMkTL(int JKlshjfosdd, bool qRrxWmGLZGGi, int kSyIFChfH)
{
    double pPYdIArxsz = -989738.9052934446;
    string RCAwzTtLFjlXhRGc = string("BkDYMTAsaiiwIsptXCnpIbjSnBCLlxVwJhEXtHRUimCBVPyHVmdZkkNxmsMKLJUrEReNkRBfeWKEoufuKbRfWGCUHWITQRMPKFADllCTiqUwJIKMpwRMoldUXctsFpfArIDCskXTqvVXnlwGFQhTyAGCtJIDQzEXhPkMfaEMznyNwXlbwpSPzzsiuXBYvTestDsLUUOVokjZvTpdHmHCDORXCUZAFeqAAOtFYgcsRypMC");
    int GFsQmrgdeicna = 1186971999;
    double JzovIUXfwlPK = 442925.9482288257;
    double KlzVJaku = -325627.15998314455;

    for (int vWwRaKlTkyWAU = 1480049758; vWwRaKlTkyWAU > 0; vWwRaKlTkyWAU--) {
        RCAwzTtLFjlXhRGc += RCAwzTtLFjlXhRGc;
        JKlshjfosdd *= JKlshjfosdd;
        JKlshjfosdd -= JKlshjfosdd;
    }

    return GFsQmrgdeicna;
}

double XekVPJnhP::zEpFPVkxCLIU()
{
    double ogiBzkJMWiH = -600879.8090643844;
    string GqZrJIlIEVjzZBYn = string("JxZIXqdYLzcDvrWBailBQRIDYOAyIHCLPwIElCwEOLtWRrZXgvPvqRXCFlVNTTGhupzAtGcNAHyRmNtfkLeTEEncfLGbqIKgoayUVYEHxSNhaNjqxAaMPCUJQgHbRKNHQSHvTqJLYjqaTCPtYjKfCWKPRbHEDwFLBDFEB");
    string zueAyRZAMlhSaj = string("VVsPtAslnLknoZbhEwwBwhgsXcDTktlJGSnhgfJblmNsailXVURxpKvtnrLpYejwWCSZmBVOIH");
    int VwkjOO = 1012232359;
    string mzXzyJAVAawUCM = string("nSdBAmXAdLeigLxDzrreufqDtNCofrxWqcdDPliJxMPSbjNOuWDDtrmKoGeTUpQgmWyZWgWBOSzzziPNTuNQxppiQXtDTZlLsOuDYRREUtBlHohRHYHnFcjwujPUtRDRZxZfYuEZMjbebkjSxMgNYBZePTbuyoCUguzxqHxgVmazaCqGuKBcBwQLrtqW");
    bool acbDRdjhBFV = false;
    int twxLkBZYKWFwZNXF = -1899913234;
    string scWXLl = string("OpuUwVIlkvlGBtjLoqwJvEaRhMVxyOVmEQvHFnZAPwAaGbohRYbHVxOojtVUdTrhUWCgoabFtUtThRfODkTyeDNVDItpESDJHJYXeMliueXNsCoqRRVipDQjmXapfvioYopCDATInlnoOMFGDeqoeuogbgHmZrZpPrOyBcJAZshvao");
    int ZvGbsZZSgf = -523986854;

    for (int gcJNlyGPwZpeYVA = 1227992854; gcJNlyGPwZpeYVA > 0; gcJNlyGPwZpeYVA--) {
        zueAyRZAMlhSaj += scWXLl;
        mzXzyJAVAawUCM += scWXLl;
        GqZrJIlIEVjzZBYn = scWXLl;
        ZvGbsZZSgf -= VwkjOO;
        scWXLl += zueAyRZAMlhSaj;
    }

    for (int QZbNpZubnSTbC = 1484764875; QZbNpZubnSTbC > 0; QZbNpZubnSTbC--) {
        scWXLl = zueAyRZAMlhSaj;
        GqZrJIlIEVjzZBYn += mzXzyJAVAawUCM;
    }

    for (int HenrgSTURHX = 590597499; HenrgSTURHX > 0; HenrgSTURHX--) {
        zueAyRZAMlhSaj = scWXLl;
        ogiBzkJMWiH = ogiBzkJMWiH;
        ZvGbsZZSgf = ZvGbsZZSgf;
        mzXzyJAVAawUCM += mzXzyJAVAawUCM;
        mzXzyJAVAawUCM = zueAyRZAMlhSaj;
        twxLkBZYKWFwZNXF /= twxLkBZYKWFwZNXF;
    }

    if (mzXzyJAVAawUCM <= string("JxZIXqdYLzcDvrWBailBQRIDYOAyIHCLPwIElCwEOLtWRrZXgvPvqRXCFlVNTTGhupzAtGcNAHyRmNtfkLeTEEncfLGbqIKgoayUVYEHxSNhaNjqxAaMPCUJQgHbRKNHQSHvTqJLYjqaTCPtYjKfCWKPRbHEDwFLBDFEB")) {
        for (int cKvEbgj = 1095165827; cKvEbgj > 0; cKvEbgj--) {
            VwkjOO -= ZvGbsZZSgf;
            mzXzyJAVAawUCM = zueAyRZAMlhSaj;
            ogiBzkJMWiH *= ogiBzkJMWiH;
            zueAyRZAMlhSaj += scWXLl;
            VwkjOO = twxLkBZYKWFwZNXF;
            mzXzyJAVAawUCM = GqZrJIlIEVjzZBYn;
        }
    }

    for (int xfjywJcY = 71819067; xfjywJcY > 0; xfjywJcY--) {
        continue;
    }

    return ogiBzkJMWiH;
}

double XekVPJnhP::fpxxBgfZpNWhD(double fSzagTRVhaAP)
{
    bool CSJBVk = true;
    bool LYllDYZqjqkruUPU = true;
    int dqHRjQYfRyAgMNj = -896365381;
    string xvIppCur = string("LgMprKwOdXFdpIaDXdHKJFwBsaYphNHXiSPpmPRbrnRwdhUyYnZCDIIGBqNDzkybTKSGsmCjzSAamNTMRnibDtspuNuUPlcxUnTjfRdGUeeppWCBOMnywHqfsPJzAtFxeJwLcEDlCGamjjzmkyZzKjERmPWepzTLlkNOLzG");
    int vETdFk = 1811161218;
    double zaKWoDQWdyGHjgzV = -238117.29088487185;
    string yQLcGaLma = string("iWSVvmnnjAQYaleAcqaBnFFdbRWcELlnPhPjtbauhadbTwaZMzBYwHNevCFlxjiNXOIpUWWAylgBIMHIMBLAdZaZHbAhdxokNnlVkHHAQqufcikGAarBUjbtUhdcTPOFIQFTMGDeGWgPnsBlhDLITvBQgaNvfTaxcyfiygSLuVTnCJbECqsnnEmtlItomWeJEdIezmBFJEVCsJEhqdvuEzduhaLRpuJLAKFlhCpprhLRDQZfoIq");
    string exAREfJbmzGKZzaE = string("axTNICEfWzPYuMrTytGqCRKqJXsxVClZxeXAXurTQCjPdmjKIOCSKuDipkiVVrenfJrmdDoXOOlYPgmRRSByukBLdEKHgcLmkYTkyPkPnFRuBFYtFVrQMnoW");
    double FTPnWTUiRNl = 925987.1154723129;
    bool pWjYZwngpuhtpA = true;

    for (int FatrrL = 120590665; FatrrL > 0; FatrrL--) {
        continue;
    }

    if (dqHRjQYfRyAgMNj > -896365381) {
        for (int IfYAUEdLKzg = 1492014366; IfYAUEdLKzg > 0; IfYAUEdLKzg--) {
            dqHRjQYfRyAgMNj = dqHRjQYfRyAgMNj;
            vETdFk += vETdFk;
        }
    }

    if (pWjYZwngpuhtpA == true) {
        for (int zGIBCKKOEcs = 141236233; zGIBCKKOEcs > 0; zGIBCKKOEcs--) {
            zaKWoDQWdyGHjgzV -= zaKWoDQWdyGHjgzV;
            vETdFk = vETdFk;
        }
    }

    for (int JeQUZQ = 1290156649; JeQUZQ > 0; JeQUZQ--) {
        dqHRjQYfRyAgMNj = vETdFk;
    }

    return FTPnWTUiRNl;
}

string XekVPJnhP::wPOCMLq(int SkBxEVzaAGiZThT, string HfyQtEm, bool JXQBiGLqCpdoNzID, bool JlbzGehVvDoCYyHH, bool IFOPZgaeVPQFxaeU)
{
    double VgtQcxg = -360633.37555988866;
    int IxSMBwPpyjH = 426333240;
    int eQsxJZXYEqh = 1193154267;
    bool ChhIBY = true;
    bool DtaOcabiEDDrRf = true;
    bool umegDmvVHHViSOD = true;
    int lPqudZmKPmjdiG = -848649896;
    bool CzCTxbclqQa = false;

    for (int sSYQyz = 1616056321; sSYQyz > 0; sSYQyz--) {
        IFOPZgaeVPQFxaeU = ChhIBY;
    }

    for (int PJkXne = 461232154; PJkXne > 0; PJkXne--) {
        DtaOcabiEDDrRf = ! CzCTxbclqQa;
        IxSMBwPpyjH = lPqudZmKPmjdiG;
    }

    for (int KsAiS = 86985999; KsAiS > 0; KsAiS--) {
        JXQBiGLqCpdoNzID = IFOPZgaeVPQFxaeU;
        umegDmvVHHViSOD = ! IFOPZgaeVPQFxaeU;
    }

    if (JXQBiGLqCpdoNzID == false) {
        for (int SUJYNQwzLBNjnZh = 1049567625; SUJYNQwzLBNjnZh > 0; SUJYNQwzLBNjnZh--) {
            continue;
        }
    }

    return HfyQtEm;
}

void XekVPJnhP::GiSXtWbDDLFyVyuO()
{
    bool NUudx = true;
    string NUCVSmvaF = string("WjIOptUaHdyRovyiSkSSLlUoYTSRiFVxmFRXhMk");
    int MMbgFQBPmTSz = -2050394158;
    int GUEpUQ = -962015973;
    double upPGls = -268626.5327855132;

    if (GUEpUQ <= -2050394158) {
        for (int aoxvfzBMPvMKf = 771178552; aoxvfzBMPvMKf > 0; aoxvfzBMPvMKf--) {
            continue;
        }
    }
}

int XekVPJnhP::wAsQIywiNb(int mUpXEElpqSkER, double aHEiZFyguxMuFTT, double naFTywBalZohadob)
{
    string KqPQaDMiKz = string("XalCdPVcsVetQVhSTnwCWChElciKRtkIhZZpoZeuYTiWfcOqqzUenerHLrLCvJsMyFhPHWJiznZNqibIwfApzwPUkqXcKdHdyFAGbyWQCRVkIePtGKPsPGNishEgJsvLIdsFLMFWWnJNJrXHmROGCiSutkiGviKDcoHfEGe");
    bool EfsczjbFAs = true;
    bool QFmsGd = false;
    double tHuEsTbunwElBNu = -384473.31248533225;

    for (int XmCNOwMrIVhzCle = 1960363113; XmCNOwMrIVhzCle > 0; XmCNOwMrIVhzCle--) {
        QFmsGd = ! EfsczjbFAs;
    }

    for (int WyCVDbNG = 1278492195; WyCVDbNG > 0; WyCVDbNG--) {
        KqPQaDMiKz += KqPQaDMiKz;
        naFTywBalZohadob *= naFTywBalZohadob;
    }

    for (int krjROMudRsKenI = 710165572; krjROMudRsKenI > 0; krjROMudRsKenI--) {
        tHuEsTbunwElBNu *= naFTywBalZohadob;
        KqPQaDMiKz += KqPQaDMiKz;
    }

    if (naFTywBalZohadob > -384473.31248533225) {
        for (int LfidUYsegukyyrzx = 491282126; LfidUYsegukyyrzx > 0; LfidUYsegukyyrzx--) {
            EfsczjbFAs = QFmsGd;
        }
    }

    return mUpXEElpqSkER;
}

string XekVPJnhP::OlTydJAi(int lQrMDYPDyAnAaKh, string CFqheLprNQXpvFH)
{
    bool TvmXe = false;
    double aZChAJBbc = 914818.610121069;
    string LXdWpzaOIqovutfI = string("nagXvvaHlQLBizdGurwvlFPmhxgzshPNkiFlyMBNqRqLKEzhQcyvuhnETuxdmwrFPnHGegxKWfailqOxSNdSFAuRjydInVsfTUgrvUmRPOqXRYIRDAeGXLLudXuOnxltdprefboMCVOPKaYByPLRDrzKWeXfFaQnBfiQaOdKYBNIuZkqxzWpnXxhOJTooXGFkCKutLByGoSoPGGvkwNIpgXSpYDPPkrjkAaxVcjDZvGHpRvvyfQpTBQU");
    double RKTliuTUoABqLw = -219474.94310782573;
    int DpbDNOoIWy = 52059233;
    int rhNZjrlEJPcgpKLW = 1186196414;
    bool YTOwBJZN = false;
    string LGhzhSh = string("HoncsAXBieWYQVfEeHyaIGcdzJtDMLCtjSiyIQtMtwSeNdIxQVZJwYOkoZMrQlaTxSxHghnuxaLuoSFmdfpfrgGmTaeFwAWvQsaJoUkEvVRggSqHywJSvdwtxdfzqUXphNbLAHmjRRraVnOuHTPwfPIlUVryRYg");
    string MnOSNNYUcuZi = string("gfLfRidoFaBKduhTGkgsVAXHqcVNHxkQukhzpMVVcvYIbgNgHgFlFHGdhMQYApnoddMmhwoiLmDbxWtwlpfXNcDIjuYLIbQhJrxVMPet");
    double RLHHR = -965645.8074527077;

    for (int fOwjg = 1323114969; fOwjg > 0; fOwjg--) {
        TvmXe = TvmXe;
        LXdWpzaOIqovutfI += CFqheLprNQXpvFH;
    }

    return MnOSNNYUcuZi;
}

bool XekVPJnhP::SjjooNAbphiosd()
{
    bool DliRMjBpdjATSuP = true;
    string RPsdfUFKXDP = string("ISZesTOPtLvOoCjFGaYSXOyvWYjfmtlhMgeoSqeCAoTOrzTrYeUGSxrJHigVEJMDVgdVfnpNxyXafIPTXSdTrJfQoRMoqMFwqnXuLoZACfOdybSuKuhtyToREFVIamtCawUpsvSVrbTRUrHxLJdAViqefDiUOVGRWkbwGlnJWewmZJeZdqhxKPSzTvtOWOeqmKdxkQCwwW");
    int rTwEuf = 1307742101;
    bool SBxNsEtmFpPXV = true;
    bool xiXHN = false;
    string HThdqthKlXfnaMuC = string("NiOuiqkOcIizIXfAvhLovsVvVGgnxExdtNOmaUsObBONSlijmOMwJmJAvKhjPzzVjTzktqOlKMazSWkiioaCdYDnPPwlVHNBuvbcKhBuIgGnBE");
    double ssOXGuaKF = 8105.518984016987;
    string mbWKJJaMUOreDhX = string("NTnrRyqFYJNWuoFqVrYYNUeAwtVrzZAcJEjSYQLYQtoGiwEFOgnLzMjKnk");
    bool hShJlOAyMCB = false;

    if (HThdqthKlXfnaMuC < string("ISZesTOPtLvOoCjFGaYSXOyvWYjfmtlhMgeoSqeCAoTOrzTrYeUGSxrJHigVEJMDVgdVfnpNxyXafIPTXSdTrJfQoRMoqMFwqnXuLoZACfOdybSuKuhtyToREFVIamtCawUpsvSVrbTRUrHxLJdAViqefDiUOVGRWkbwGlnJWewmZJeZdqhxKPSzTvtOWOeqmKdxkQCwwW")) {
        for (int YZkFJPWCrZ = 231779439; YZkFJPWCrZ > 0; YZkFJPWCrZ--) {
            rTwEuf /= rTwEuf;
        }
    }

    if (SBxNsEtmFpPXV != false) {
        for (int pZGHxx = 916965207; pZGHxx > 0; pZGHxx--) {
            hShJlOAyMCB = ! SBxNsEtmFpPXV;
        }
    }

    if (SBxNsEtmFpPXV != true) {
        for (int RBHRKs = 482013023; RBHRKs > 0; RBHRKs--) {
            mbWKJJaMUOreDhX += mbWKJJaMUOreDhX;
        }
    }

    for (int RsblvvKcYej = 1758489240; RsblvvKcYej > 0; RsblvvKcYej--) {
        RPsdfUFKXDP += HThdqthKlXfnaMuC;
        hShJlOAyMCB = SBxNsEtmFpPXV;
        xiXHN = DliRMjBpdjATSuP;
    }

    return hShJlOAyMCB;
}

int XekVPJnhP::PyZPgsCn(string TlMIeYlvnMQCK, bool LkgrlgZbFap, double DGYcvfWewknVGXo)
{
    int PvTklkoDdQCn = -2080841775;

    for (int XuTSeO = 1238092968; XuTSeO > 0; XuTSeO--) {
        TlMIeYlvnMQCK += TlMIeYlvnMQCK;
        LkgrlgZbFap = ! LkgrlgZbFap;
        TlMIeYlvnMQCK = TlMIeYlvnMQCK;
    }

    if (DGYcvfWewknVGXo >= -528610.2986023383) {
        for (int MDOeL = 213671905; MDOeL > 0; MDOeL--) {
            continue;
        }
    }

    for (int MiebRzakQtdpkBIM = 1175897270; MiebRzakQtdpkBIM > 0; MiebRzakQtdpkBIM--) {
        TlMIeYlvnMQCK += TlMIeYlvnMQCK;
    }

    for (int XDmfjaYvsq = 1605556152; XDmfjaYvsq > 0; XDmfjaYvsq--) {
        TlMIeYlvnMQCK = TlMIeYlvnMQCK;
        DGYcvfWewknVGXo -= DGYcvfWewknVGXo;
    }

    return PvTklkoDdQCn;
}

XekVPJnhP::XekVPJnhP()
{
    this->CdGISu(471159.0212541023, false);
    this->UelKGVOmzbY(true, false);
    this->GUeNKh(string("EhjDHDBcLyvdeGYqPLMYJogeJyVkSbHURlUNkhHHAzGtljYNXeCitnfWneyXSUqFHHSVQNxE"), string("frxaDkaWDDHIdmRXTyBcTKlQGkqiWGQRSyvZEYJqyzzTsqgSAeCx"), -513331340);
    this->bKgBnFAZOzUaKnln(false, 980045859);
    this->QDUsnMf(-862413.1472184177, string("m"), false);
    this->OfPCWWVmYhRHHhSr();
    this->IjbcybOnzb(true, string("WbwFVuSVDXjhWHrXCaqNodEzQDctfBRaAHvMlvPJxQjoEMnBytKTEHA"), string("kSEdlebMrPbGrMjpjooPmBpmUVtcgQHCBIJcnRKLEChhaAIXaGSGGmiQArfWsXTFBVzxMlgd"), string("fFIcqmyJvvDfmZQXlyYSnWNrvhzSBrgLIFCyzcxyJhIsqFHMWhQ"), string("zcLjsCKYamwhHPHKIuzMTrxnGlOJpLJOlvThamOitlVGMQWdSIJdENWehklSnSECxlSIqvKiKhPcWIHcDVTSusJfZgq"));
    this->CrVHxbmejrjVB();
    this->VvqbNy(-628403.8345334211, -220878514, 1809916923, 414744190, -2009094581);
    this->tMTJO(false, 951574.4531317231);
    this->lLdUJmvMkTL(-2063732872, false, -792669717);
    this->zEpFPVkxCLIU();
    this->fpxxBgfZpNWhD(-532698.169383261);
    this->wPOCMLq(-1868753470, string("xVwzPwHIEkkSAbFxfBgWVLwekOfWCGsURpTTsxlYsQKPZAQvNznqBuSCQc"), true, false, true);
    this->GiSXtWbDDLFyVyuO();
    this->wAsQIywiNb(-1354767248, 11072.888431734733, -30137.637654673043);
    this->OlTydJAi(1402777118, string("eFIjWAKezuCCSECgTWmMtjPlddRQNmgPejEkqzuIYlqLFrIWfAUTcuwPslHwvhFhoRSjCHXUSkSCTwPPsJXzByeQgifgJQiNUkgfzIfdweFnydPhaSWhIpdwfBVXgEHdCSRvEBdTkIRzYCEpjyphpldPLAKylWFtekRuGsfEjRaSFExJtCNMiLRiqqeXrYrcAhHHWXVeIjFUMiYOlEVLLIQRFMXqiGPQbyIJWXABhdTjQqFBXKFfjAybC"));
    this->SjjooNAbphiosd();
    this->PyZPgsCn(string("cmaANjFfVVrjUJFyZBeUKCOqkGBCxnovWyPeHvXFiCCMaLVJegMcFsyEjVeqvLLZEdNUiVAzSJkOcgVnLivXaiIaBPoVfxHWnpIhMytQaIKbHvNkwSlgmBYRRfuFnrumYmtnLSqPrdyKkYdzryJOnHepJFyKTBduarbAYuenPKwI"), true, -528610.2986023383);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class bmOYJFiyjjFwKqKR
{
public:
    bool cPuAmcVGQpupBlS;
    int UNiNBIIvtDmu;
    double fBbbvzMeeKet;
    string AUAgxQ;

    bmOYJFiyjjFwKqKR();
    string luJonVfUcnxJ(string YZQgyrV, double cIDEnDpO, int MqMvynPO);
    double KXSrI(double kMBqJEdUVfX, bool GghdUGV, double yNoikEwyba);
    int gZzqqfpDvrnWyk(string JIWlNg, string OtxNNssw, string UddSoZCRMkTbiNFx);
    int ioOubEKXVQzhYvb();
    string nPQLbyjiemeeu();
    int KfkforGJIa(double TaIEjcrVjkME, int MhDlpEntcoYuK, bool DwHEZmoFAhVo);
protected:
    int bBaui;
    string AzFiQMNoQMIrqjT;
    double glesqFfhOvGuPUZ;
    string TEmMZoaJwsdriEo;

    bool mECXsZOG(double cWAQYKK, int zYQfcPnolMz);
    void cyLekvQNMiPtb();
    int wMQGydXWkPBCi(string WPTQvwTiqsksOnOA, int QrhlXYz, string BDGqgcErRskiFvD, double giwIjb);
private:
    int ZIjXQZvrxiZTWc;
    int PGynHugDXwUYlrh;
    bool xfsEiHrz;
    int VlFHQwwbKTCFT;
    bool cEbcOVOeMCeycw;

};

string bmOYJFiyjjFwKqKR::luJonVfUcnxJ(string YZQgyrV, double cIDEnDpO, int MqMvynPO)
{
    double LOgXSj = -530357.7698575326;
    int RaBsvwevi = 1705247143;
    bool sHymoSHfhKYMUzu = false;
    bool gzQyHh = false;

    for (int WhJGVb = 355178412; WhJGVb > 0; WhJGVb--) {
        LOgXSj += cIDEnDpO;
        cIDEnDpO *= cIDEnDpO;
    }

    return YZQgyrV;
}

double bmOYJFiyjjFwKqKR::KXSrI(double kMBqJEdUVfX, bool GghdUGV, double yNoikEwyba)
{
    bool NyPmCRLoO = true;
    int RhSZxKQEbrIxrZ = -1663565467;
    int HAusuZAILBNWIn = -2133255852;
    double xPqKWBe = 120691.43261557026;
    bool aGIdzsx = false;
    string UqLAwKEgc = string("HzNkXOfhQsRQfDBuSNLlvkWNxbNnkqCqPyyHXmnLsbrPIZrGwzwbKtCFMZhMbhvPMfdDasHItumOcEDRYktiyVepmlMPElmBmJVzwzGOelqiDIiTZHkDEOIgDDZvFkOKwvivkKoIiYHDsrmvQVBBPTArkLYkbGIjqywQrdXKckKzNtGbfbMXUgLkHRXLGqlQbuihpvBv");
    int zoRdN = -631347248;
    bool hnTfMaZ = false;

    for (int snIFIMGNifDKMUw = 1498787989; snIFIMGNifDKMUw > 0; snIFIMGNifDKMUw--) {
        continue;
    }

    for (int NrjunCjymPDKIY = 143023963; NrjunCjymPDKIY > 0; NrjunCjymPDKIY--) {
        zoRdN += RhSZxKQEbrIxrZ;
        hnTfMaZ = NyPmCRLoO;
    }

    if (yNoikEwyba != -884126.5081521536) {
        for (int KPLGYs = 1569001161; KPLGYs > 0; KPLGYs--) {
            aGIdzsx = ! GghdUGV;
            hnTfMaZ = hnTfMaZ;
            NyPmCRLoO = ! aGIdzsx;
        }
    }

    return xPqKWBe;
}

int bmOYJFiyjjFwKqKR::gZzqqfpDvrnWyk(string JIWlNg, string OtxNNssw, string UddSoZCRMkTbiNFx)
{
    double WkDUTMFlNZ = 813763.6216864093;
    string GBjhVRGi = string("YDCQPacnBCqtNHnvRKUEPpGWrIGpDEGdAPhoGzyhpADVBhgjvVGPHVguVKSFMJfAvlshKcXoredcNOLdVutpdCSgfePvOQFQqnWWtLtzMUGUdFrMbBkdSxOwvkJAfuRwNLCJpSlRsQLVyuDfHHKkRwXVKFibbuEFdidhVsOJNkuCQXxuRFIiJfvNybuDOIKenueJnxXnZddPmSttOuVlunlvfTXDzIGh");
    int NZiWnGmzAR = -520798426;
    double EksPBH = 948781.6376075445;

    if (NZiWnGmzAR >= -520798426) {
        for (int irrSeERKQX = 1829087103; irrSeERKQX > 0; irrSeERKQX--) {
            JIWlNg = JIWlNg;
        }
    }

    return NZiWnGmzAR;
}

int bmOYJFiyjjFwKqKR::ioOubEKXVQzhYvb()
{
    bool eVKGuILbSBt = true;
    double taovcVuMjaXNSgmm = 800743.792644013;
    int RFqyuGxbKbzg = -741217377;
    double wrsVxSSMXugA = -756750.4616789465;
    int EUNHZIpbeHnFD = -898980695;
    double YWNTDoi = 829747.5600748959;
    double XdSIP = 405899.9784735389;

    if (wrsVxSSMXugA != -756750.4616789465) {
        for (int AeaPBRigEx = 895759708; AeaPBRigEx > 0; AeaPBRigEx--) {
            XdSIP *= XdSIP;
            EUNHZIpbeHnFD /= EUNHZIpbeHnFD;
            taovcVuMjaXNSgmm -= XdSIP;
        }
    }

    if (RFqyuGxbKbzg == -741217377) {
        for (int CccnrNiIRuMjFdqs = 157482268; CccnrNiIRuMjFdqs > 0; CccnrNiIRuMjFdqs--) {
            XdSIP -= wrsVxSSMXugA;
        }
    }

    if (YWNTDoi > -756750.4616789465) {
        for (int sjXbbUHHIefo = 1355918151; sjXbbUHHIefo > 0; sjXbbUHHIefo--) {
            XdSIP -= XdSIP;
            eVKGuILbSBt = ! eVKGuILbSBt;
            YWNTDoi *= taovcVuMjaXNSgmm;
            wrsVxSSMXugA /= taovcVuMjaXNSgmm;
        }
    }

    if (RFqyuGxbKbzg == -741217377) {
        for (int BKWrktZFIdom = 1796494951; BKWrktZFIdom > 0; BKWrktZFIdom--) {
            XdSIP /= YWNTDoi;
            wrsVxSSMXugA /= wrsVxSSMXugA;
            EUNHZIpbeHnFD *= EUNHZIpbeHnFD;
        }
    }

    for (int HCTOVRDTf = 1134272917; HCTOVRDTf > 0; HCTOVRDTf--) {
        eVKGuILbSBt = eVKGuILbSBt;
        taovcVuMjaXNSgmm += taovcVuMjaXNSgmm;
        RFqyuGxbKbzg += EUNHZIpbeHnFD;
        XdSIP += XdSIP;
        YWNTDoi = YWNTDoi;
    }

    return EUNHZIpbeHnFD;
}

string bmOYJFiyjjFwKqKR::nPQLbyjiemeeu()
{
    double XexWHDOYdZ = 37700.93766985278;
    string mPHOJxvlxpxq = string("cfvQLuLrsZMukLoYQRwVpbrneSEoTHfUaJfybTJBbKYaKtzHJXmHhDxAHtXzPRKETcvlxfUBcLydlSZpGECnILfvF");
    int XOOgQvvGVrOMPp = -1912747288;
    bool zewqgFtd = false;
    double qbwQdUywfpVrIw = 257233.39828850765;
    bool Iiixv = false;
    bool QrdKymlpwoELQ = true;
    string iUOsRTpfYtx = string("lbBGbYhsJLbgHQ");
    string mMoESSKGiZ = string("btIWSTmAoe");

    return mMoESSKGiZ;
}

int bmOYJFiyjjFwKqKR::KfkforGJIa(double TaIEjcrVjkME, int MhDlpEntcoYuK, bool DwHEZmoFAhVo)
{
    double svrPxN = -680059.0930949765;
    bool GhmHdolisVCqK = true;
    int IGaKE = -810994937;
    string YzdubPjhcOVLb = string("YV");
    string WSywOtmeDYmxgmG = string("ARVBkqwfcbYShOkidJfcoZDSifSIJvzjblZRgxdqGyvxxmbweepZVkKGooTVTlqDOFdGWDnUiFmkhsYRgiaIXuaJrXnJxzKxaIqhlULwyRbVAiVjtQBUbTkfvDRwQIZFLOFkGeOtOkFLNYLSnLFXTebqcGbPxVspQCXAKZwSrpNEFNVNzVFrWGEg");
    bool BHPosvIlhYHhElA = false;
    int IbkUHEWqrvNbzr = 1646369865;
    string mQJxqwfEZH = string("CiigBSjmXXhuKCbmbPPGleDShEMipdsmykgZhwFkXioxPBNOKAgjBNZuVejmhIomnNQniNtYTAHNBgxKpvelvlGYDJIhAdJexxpvHubdfhNfuvlauclWWVAIWIrhDtZyrxCtqToAMlEtUMVmOferASkUMVkiodJQpshvdaPxfOGKdGZdgrxEtATHSbiJfCimbXTQeCSWiMaspJKrxCyCuHLiQdhJkWtbSY");
    double xpsBQd = -813207.5670274177;

    for (int DvqrStMCVmMQjf = 1758180836; DvqrStMCVmMQjf > 0; DvqrStMCVmMQjf--) {
        continue;
    }

    if (DwHEZmoFAhVo == true) {
        for (int BVYvuq = 1391089887; BVYvuq > 0; BVYvuq--) {
            mQJxqwfEZH = WSywOtmeDYmxgmG;
            WSywOtmeDYmxgmG += mQJxqwfEZH;
            IGaKE += IbkUHEWqrvNbzr;
            GhmHdolisVCqK = DwHEZmoFAhVo;
        }
    }

    for (int kvbGkWCveMssXTr = 1614933022; kvbGkWCveMssXTr > 0; kvbGkWCveMssXTr--) {
        IbkUHEWqrvNbzr += IbkUHEWqrvNbzr;
    }

    return IbkUHEWqrvNbzr;
}

bool bmOYJFiyjjFwKqKR::mECXsZOG(double cWAQYKK, int zYQfcPnolMz)
{
    int AjCSmFYDqzq = -590893808;
    bool LfJGKiToSopkwgA = true;
    string ZfLqfqBomhDYwYB = string("tZHgFjnNOoGGxllHszkqlXHymGzIQiFhrGFPhECkSNQXXkJPSHKaveNDtcetqirap");
    bool YzpJnmwAMoWmyOUc = false;
    double jfWSZDXnqyfbVixK = 247758.05878913176;
    double IubbeyhwtrGnSPV = 742190.2862271216;
    string ggCXNzP = string("mzfAhXcvgxBnyeloDmH");
    double tWFPQsNHhjKwFhw = 1044182.318372767;
    double CCQFaT = -127941.98084248485;

    return YzpJnmwAMoWmyOUc;
}

void bmOYJFiyjjFwKqKR::cyLekvQNMiPtb()
{
    bool ROonrSTfF = true;
    int BAZlzSf = 1062772902;
    string EEqMLxYK = string("JxdHPQyOKmuptKSGZAaoddaInhOkxjghlzxlVGAMlcvpdFAUHkdbEAXpjdwMiaCXynumbcYSRyRbPYxvXJnHBFBfuStkwXfFedchXPEQLJOjxrEtgWRDioxCkgroLlGYEpppkbEpDpUtASmNGtKiajxaPSvXKGeNtnHDZcY");
    int BFAkrahcXe = -950829863;
    int nRdJMhbL = 212191882;
    bool NSQnBirOkQB = true;
    double RYNpJl = 56619.67853505565;
    string eTbCjTpxl = string("uEutGLewpjLrRjkIunkzaXlXTKOPexeCeYWZZhyarekYWeyWxBUDbiRqcwdRIQbABtLSmoSanFO");
    string QZShZOoMazhCrZg = string("KlYDBsWmwMPOtcMikmbADwHVHKDKQQHMnXnigcVyVhxGvedHoWMwyAUErSKPmCDbnYAgbtzUpWalbvSpURiFXPEhiXEqWryqoXBluVuDzRpfNswYnbTNDKtAAFDZnMOjqdETlMpokIZSYIJapunVNCBvdCdIyruWCjWDZBxSwhcolVjsbaJIpueNCuUpSzWbFYWNDmizEyxjVLhuMPePufymg");
    string ZxxShKursbhZDAiy = string("DByufbrdellewtTWbHsZGCMrTxDNBdhuiuwXYbNBbCmNvVIwOefWVtZDDzjcGkJslLMifGInInbXqKdcqJgipTsxbjIwOHclKmoGonpgbJvScZDoRpW");

    if (QZShZOoMazhCrZg != string("JxdHPQyOKmuptKSGZAaoddaInhOkxjghlzxlVGAMlcvpdFAUHkdbEAXpjdwMiaCXynumbcYSRyRbPYxvXJnHBFBfuStkwXfFedchXPEQLJOjxrEtgWRDioxCkgroLlGYEpppkbEpDpUtASmNGtKiajxaPSvXKGeNtnHDZcY")) {
        for (int jdgAxrZvVpTrfJ = 100260251; jdgAxrZvVpTrfJ > 0; jdgAxrZvVpTrfJ--) {
            EEqMLxYK = ZxxShKursbhZDAiy;
            BAZlzSf += BAZlzSf;
            RYNpJl -= RYNpJl;
            BFAkrahcXe *= BAZlzSf;
        }
    }

    if (eTbCjTpxl < string("KlYDBsWmwMPOtcMikmbADwHVHKDKQQHMnXnigcVyVhxGvedHoWMwyAUErSKPmCDbnYAgbtzUpWalbvSpURiFXPEhiXEqWryqoXBluVuDzRpfNswYnbTNDKtAAFDZnMOjqdETlMpokIZSYIJapunVNCBvdCdIyruWCjWDZBxSwhcolVjsbaJIpueNCuUpSzWbFYWNDmizEyxjVLhuMPePufymg")) {
        for (int irfOPNJQsDsK = 290868357; irfOPNJQsDsK > 0; irfOPNJQsDsK--) {
            NSQnBirOkQB = ! ROonrSTfF;
            QZShZOoMazhCrZg = QZShZOoMazhCrZg;
            eTbCjTpxl += EEqMLxYK;
            nRdJMhbL *= BAZlzSf;
            eTbCjTpxl = QZShZOoMazhCrZg;
        }
    }

    for (int lpWESsF = 2122491900; lpWESsF > 0; lpWESsF--) {
        RYNpJl /= RYNpJl;
    }
}

int bmOYJFiyjjFwKqKR::wMQGydXWkPBCi(string WPTQvwTiqsksOnOA, int QrhlXYz, string BDGqgcErRskiFvD, double giwIjb)
{
    int EooBUN = 1122035134;

    for (int ZGlmmLVDi = 1674639200; ZGlmmLVDi > 0; ZGlmmLVDi--) {
        continue;
    }

    for (int KlYKoAQDDJFfkd = 2070050419; KlYKoAQDDJFfkd > 0; KlYKoAQDDJFfkd--) {
        continue;
    }

    for (int bbAVgOjoS = 911316280; bbAVgOjoS > 0; bbAVgOjoS--) {
        continue;
    }

    return EooBUN;
}

bmOYJFiyjjFwKqKR::bmOYJFiyjjFwKqKR()
{
    this->luJonVfUcnxJ(string("UymMSolPfjyKsGynnxukboTvGslSixWtgKlpNgcckhNFwdjWtunbYWezuuZYNwOPDyJYyzQutNXNeDKLBWPBFsOLozvkHUsKUiXiVsFVUDLrsubUjInKOWGoyQjMrGPeEqKNuWoWRhqOFdIjpzlLyPaccDxzeGUESMReTRgvofDhlJdJFxSvAQUUUobYuJle"), 854423.7229031501, -1922557216);
    this->KXSrI(-884126.5081521536, false, 691030.9259027969);
    this->gZzqqfpDvrnWyk(string("UJZKIiPnNbDTUkJNWMvmQrEcYhreSBoBocYBfpjVqKLsdppzetpQVhAVUBHlAeuyZqDyQaqBxzyVGsAhxUefOwBDLcBuUNQMUIZHoVHYdLfMShVkQRVESVTPzZfEoQdkFNTuXXYspMvjmqHEsyZKAguAjTtbCImZkHQFXUACGC"), string("pcvaLtBkntRXOWMKYNtcKnrcpT"), string("DYlgpDTiVSgAplOtRHwOczAPJSNHrkEsUQAmsHlIZTNHNvZwoUaBYlVfZAWAbPZGDBuHLOjLmHpDjKyVYRiLkcMlnhWvDPGyiilbSMGZasFjvtUtGffhFztZcaBqmpogtNlMZsWXbbTEVomnJQuJBzTmYOwwbzalQXmYawiNxTHiPQymIPEujnVxYyDgtedlKLArhKnAwjhaScGXauuUlqAtZnpriYyeWTHlEPPqrpCJYvTebY"));
    this->ioOubEKXVQzhYvb();
    this->nPQLbyjiemeeu();
    this->KfkforGJIa(9921.17431108665, 996984682, false);
    this->mECXsZOG(185783.66411206103, -1972969121);
    this->cyLekvQNMiPtb();
    this->wMQGydXWkPBCi(string("HZzTDXhdzEIAjJOGsrsVwBWYOdOUkbxTyNcChHSiiyKrgkQGOuYOZtPhkLtEbHfKBWNtUcexLCybzUcnfSevPXPAWDpSPbAifCAdSwqfAsRKvRyfLwBvsbNGUKxSdMmTzGMNyanMqbcebFDJXHeFjFVocPGoGQoNXWWuuGrPwSJXQhapdiPbTSlwBlSCTxMeStSuWKrFCjtohJenTaxRMbSvXQDaHfVHohuLdqGlwWxqUbkw"), -1499468060, string("KzUxcmyNhTmccGPqFFHZeJAeZHbDTKAqnlEWfsqydcTghwbDOZocPqDTQRgrpCznGaxOXPXCfLFBRktnGiuyMOhxbOsSCSXvRbBVSwsuQtTuIrkEqeoxFEeNSLWNJjCnbdReETBEjJzhbbvimIUPJFgsMitUEsCsxERjSodnwXgKTVQNATaJNZhrxvSNExKMMU"), -100198.74418856345);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UIfAzGfCQN
{
public:
    double dcaWudJxiiw;
    string YHzCVsVwUgDOqMHc;
    int kCUAbjwVmg;
    bool eHYczoQXmJjOX;
    double sWZxZaFkdJTvGXN;

    UIfAzGfCQN();
    bool wbrIFzm(int yUXEUGBWVjNWmCMf);
    double BRhlkMq(string WlyAVYkDS);
    string PrwxgyfRw(bool mLaaWl, bool tekxbpoMwkkIP, int IgObCwh, bool NXJAmyq, double BLPRZ);
    bool fJmZgrTGDGJIIaxG(string hrCjKasRflArUgaF, bool jLQWi);
    int XnyxSEtb(bool qubnu, int JIcBbJ, bool jMlPE, string itqyGt, bool aNqwtfoRVLL);
    double KXsmOPWNUp();
protected:
    bool kOTnKcdgMjCyjQ;

private:
    bool nMOyCIdxwLRcbDze;
    bool PxCltqTNywW;
    int zbOwaeBPuzob;
    int oOFVTI;
    double KGYQcNGj;

    int sEKdEVDlF(bool AgWuirQvTfmMSlPA, string YHKiQOsAJ, string rxbgTNdvx, bool mSULvfwrOiXfo);
    void VLuZjDcDzNLxkE(double eUjFAeZX, bool qVRMKNKn, int FnClnOkNhac, int WPvDXBMQGJ);
    string hNFWsPDyBUaP(int hBRAOIEKY, int kUVCerp, int OktZvlrjaMm, bool XNmcUjncsWYqQhsC, double aehKj);
    int lcpAEZg(bool RkMAru, string aIGZlj, double CBoCCJQLuXzE, bool bGgOXeSAEKLvQYh);
};

bool UIfAzGfCQN::wbrIFzm(int yUXEUGBWVjNWmCMf)
{
    double VGIATGwYiKKhOosP = -892256.8544889281;
    string PLYVSe = string("lqahoOXQXQqEh");
    double qPcIvpnVSBHJKWp = -7144.15557046166;
    bool lMGpogOriayLBXD = false;
    bool UFVjHUAde = true;
    double gPeMqQYBiHhoj = 795448.5976078457;
    int BFKdLFxRBpxSkUzf = -230178539;
    string dVMgyBGGdYCvFMC = string("xNTCFRwnmnzlEpdYHgRvSTtUiRotBnHxMNNFxkjlFaPPRiOzJzvamCSYzLroNdTPBuLJHfnvJblTQNKwgLlVEwbWOBFNxDoxnJrlvialmHtHjPssvNcgWBMnoTKtpoXvMaqqMJmpXlypMsdZGaeCYXYLAjNQsSjgkLwwINFaiZOjcYXpiXEnyCEtyHakxxeAZnyUBlMbzOfXbsdmKFaSagIojI");
    int hlVfLy = -550521388;
    string iWjZC = string("KctdrVTkfpWkkFqDEMssJX");

    for (int sRHXeuICWKUAHMD = 2023981653; sRHXeuICWKUAHMD > 0; sRHXeuICWKUAHMD--) {
        continue;
    }

    if (iWjZC >= string("xNTCFRwnmnzlEpdYHgRvSTtUiRotBnHxMNNFxkjlFaPPRiOzJzvamCSYzLroNdTPBuLJHfnvJblTQNKwgLlVEwbWOBFNxDoxnJrlvialmHtHjPssvNcgWBMnoTKtpoXvMaqqMJmpXlypMsdZGaeCYXYLAjNQsSjgkLwwINFaiZOjcYXpiXEnyCEtyHakxxeAZnyUBlMbzOfXbsdmKFaSagIojI")) {
        for (int HkboFeX = 286049210; HkboFeX > 0; HkboFeX--) {
            dVMgyBGGdYCvFMC = dVMgyBGGdYCvFMC;
            hlVfLy -= hlVfLy;
        }
    }

    return UFVjHUAde;
}

double UIfAzGfCQN::BRhlkMq(string WlyAVYkDS)
{
    bool CpNFDTiGuwMTt = true;

    if (CpNFDTiGuwMTt != true) {
        for (int yTlVCqDGYlBgK = 2093064297; yTlVCqDGYlBgK > 0; yTlVCqDGYlBgK--) {
            continue;
        }
    }

    for (int wjBHbTwtvTXDapLX = 858853845; wjBHbTwtvTXDapLX > 0; wjBHbTwtvTXDapLX--) {
        CpNFDTiGuwMTt = CpNFDTiGuwMTt;
        WlyAVYkDS = WlyAVYkDS;
        WlyAVYkDS += WlyAVYkDS;
        WlyAVYkDS = WlyAVYkDS;
        WlyAVYkDS = WlyAVYkDS;
    }

    return -62392.24906322973;
}

string UIfAzGfCQN::PrwxgyfRw(bool mLaaWl, bool tekxbpoMwkkIP, int IgObCwh, bool NXJAmyq, double BLPRZ)
{
    int OzVPXM = 1978518949;
    double KLymVChAtEboCzr = -182749.59805664574;
    bool gicKtaJDTLt = true;
    string AkfjnKJhOtotm = string("xblemaUzOZFiYAuhjkVrHjwEeIxntOYIAcSeRCXDOJwBlxbMeLhfeK");
    string FYRFcwnUV = string("RzrkgdURYFjpSBjThWegWjbFUNLpPOrlATOhtprufoUnKRQuosjLQgUvnjePlkxAeaOGSxAIGHLryWZBiMFGGMvvRRRqPmxFYMEXpqdhMNblHYTrYAPxcDwcXIaZaprLYZHnwlGKyWYTaGppWcURoCUmlrGHMLXdxqDOnnAtPWNgIvX");
    int ppvvAXbpt = -1339297088;
    string tEtnJYsqRLHAwHG = string("DlicwmYAFXkDKIMCqeQOlwZrseUIZxYzlVbBplyrMbFgXpDqMPtZNBwXiDFsOI");

    for (int ucIpaosLDgXtgiH = 2023222229; ucIpaosLDgXtgiH > 0; ucIpaosLDgXtgiH--) {
        BLPRZ = BLPRZ;
    }

    return tEtnJYsqRLHAwHG;
}

bool UIfAzGfCQN::fJmZgrTGDGJIIaxG(string hrCjKasRflArUgaF, bool jLQWi)
{
    int bNTrdKbpYDxCuS = -1659417016;

    if (hrCjKasRflArUgaF < string("bTsYGPXfbfrsQpxkJAuDclZckXtAukfePWRhmhPATGvcKQHTvSPTvWBAvdByYeHnBBJHHKUbusdfREpzPyqvjhvYXmObHKrFBNdmyOsKESZGHlSHXOoTOpDxUZjLfXfJpEzuSjFMLHEJZPIsSUocEEvxWZoATqsATymUKhpESZprEwYVBrU")) {
        for (int ZdhRYwyAJop = 781485921; ZdhRYwyAJop > 0; ZdhRYwyAJop--) {
            jLQWi = ! jLQWi;
            hrCjKasRflArUgaF += hrCjKasRflArUgaF;
            bNTrdKbpYDxCuS -= bNTrdKbpYDxCuS;
        }
    }

    if (jLQWi == true) {
        for (int yZdeaBXmWN = 1087249391; yZdeaBXmWN > 0; yZdeaBXmWN--) {
            bNTrdKbpYDxCuS -= bNTrdKbpYDxCuS;
        }
    }

    for (int uLxMtvWB = 386794416; uLxMtvWB > 0; uLxMtvWB--) {
        continue;
    }

    for (int OEocZXs = 1258325410; OEocZXs > 0; OEocZXs--) {
        bNTrdKbpYDxCuS *= bNTrdKbpYDxCuS;
    }

    for (int nkTapbUjBj = 208775057; nkTapbUjBj > 0; nkTapbUjBj--) {
        bNTrdKbpYDxCuS *= bNTrdKbpYDxCuS;
        bNTrdKbpYDxCuS = bNTrdKbpYDxCuS;
        jLQWi = jLQWi;
        hrCjKasRflArUgaF = hrCjKasRflArUgaF;
    }

    if (bNTrdKbpYDxCuS == -1659417016) {
        for (int zrjeVsLhWr = 1376450303; zrjeVsLhWr > 0; zrjeVsLhWr--) {
            bNTrdKbpYDxCuS = bNTrdKbpYDxCuS;
        }
    }

    for (int zWtETERcLMJvggtd = 740307218; zWtETERcLMJvggtd > 0; zWtETERcLMJvggtd--) {
        bNTrdKbpYDxCuS += bNTrdKbpYDxCuS;
        jLQWi = ! jLQWi;
    }

    return jLQWi;
}

int UIfAzGfCQN::XnyxSEtb(bool qubnu, int JIcBbJ, bool jMlPE, string itqyGt, bool aNqwtfoRVLL)
{
    string uebzk = string("dBhvYgYJIlOrrVkKsxYZTkWffDxeyhHNvHAYlQKoqWMqCBOPqYRDGXWpLLDOSZCzgXUyIYJiDQOUtpjJVetcqBhehXiTGbtbIuo");
    double YIwAnWwY = 439238.6500183638;

    if (aNqwtfoRVLL == false) {
        for (int IQnoJHfkkDflw = 1416625947; IQnoJHfkkDflw > 0; IQnoJHfkkDflw--) {
            YIwAnWwY /= YIwAnWwY;
            aNqwtfoRVLL = ! qubnu;
            uebzk += uebzk;
            qubnu = aNqwtfoRVLL;
        }
    }

    for (int EgKQZxFu = 1428262914; EgKQZxFu > 0; EgKQZxFu--) {
        qubnu = ! jMlPE;
        itqyGt = uebzk;
        qubnu = qubnu;
        itqyGt = uebzk;
    }

    return JIcBbJ;
}

double UIfAzGfCQN::KXsmOPWNUp()
{
    string PJkSTBAMkrwfCqkj = string("cSJZIUpOyZHCPDaIvyOpPXPyNxCLcQLBVNdMHywQkQeHZiEPEPTOsIRlNnmhaMRgwtKQNBaUxAMYCVKvIDvZoTWrjBFjOehoGqYMLTFHCDnPAqzaQzFBSvOSIEYffWZgpJVlcvCDaTbFUMCQtlzpVlHEMyLufCt");
    string ojoLgf = string("rzEFmSJZrbvGvRQbotHfWoXJoMiFcvGSHUIcIN");

    if (ojoLgf == string("rzEFmSJZrbvGvRQbotHfWoXJoMiFcvGSHUIcIN")) {
        for (int jiBHT = 879287513; jiBHT > 0; jiBHT--) {
            ojoLgf += ojoLgf;
            PJkSTBAMkrwfCqkj = PJkSTBAMkrwfCqkj;
            ojoLgf += PJkSTBAMkrwfCqkj;
            PJkSTBAMkrwfCqkj = ojoLgf;
            PJkSTBAMkrwfCqkj = PJkSTBAMkrwfCqkj;
            PJkSTBAMkrwfCqkj = PJkSTBAMkrwfCqkj;
            ojoLgf += ojoLgf;
            PJkSTBAMkrwfCqkj = PJkSTBAMkrwfCqkj;
        }
    }

    return 863783.9930228164;
}

int UIfAzGfCQN::sEKdEVDlF(bool AgWuirQvTfmMSlPA, string YHKiQOsAJ, string rxbgTNdvx, bool mSULvfwrOiXfo)
{
    bool bghUdTrqNtkneLX = false;

    for (int gCmTrTy = 607638563; gCmTrTy > 0; gCmTrTy--) {
        bghUdTrqNtkneLX = bghUdTrqNtkneLX;
        AgWuirQvTfmMSlPA = ! mSULvfwrOiXfo;
        mSULvfwrOiXfo = mSULvfwrOiXfo;
        YHKiQOsAJ += rxbgTNdvx;
        mSULvfwrOiXfo = ! mSULvfwrOiXfo;
    }

    for (int eRYXvcnKBRenIic = 929940409; eRYXvcnKBRenIic > 0; eRYXvcnKBRenIic--) {
        continue;
    }

    if (bghUdTrqNtkneLX != true) {
        for (int jbaiqdRL = 2135195822; jbaiqdRL > 0; jbaiqdRL--) {
            YHKiQOsAJ = rxbgTNdvx;
        }
    }

    for (int XREmuMqxshm = 1813588359; XREmuMqxshm > 0; XREmuMqxshm--) {
        AgWuirQvTfmMSlPA = bghUdTrqNtkneLX;
        rxbgTNdvx = YHKiQOsAJ;
    }

    if (YHKiQOsAJ <= string("BAmUvhLsDMBLIvKKMsMnKZuqUPJsSqvjSGmkkwtZUiQumemZoGuEoXwVEuOmgmOtDjoWsZaCqOfPQ")) {
        for (int nKHzLuASePJQQq = 1564289415; nKHzLuASePJQQq > 0; nKHzLuASePJQQq--) {
            AgWuirQvTfmMSlPA = ! mSULvfwrOiXfo;
        }
    }

    return -1390964240;
}

void UIfAzGfCQN::VLuZjDcDzNLxkE(double eUjFAeZX, bool qVRMKNKn, int FnClnOkNhac, int WPvDXBMQGJ)
{
    bool oHsRhpyJWTsbPBZ = true;
    double IqXqbAgwDh = -419960.5630223661;
    int QmAcBCJbAJjojc = 922827056;
    string ERcUMmgOUoYOGbn = string("dRvaOEJSeb");
    int EygMOvaNhrkQyO = 981932541;
    bool LaJFMYKXWFdGykpb = true;
    int EDAaeBSdbtQA = 1585287848;
    bool GcnebFKkPKBhbp = true;
    bool QigdEQmXACbZP = true;
    string RDBqRj = string("OoAhmYOaLKUiLoQeJfIFKGumBpTqsJJwugiuJHYpZihnHstqXeNZdKLbgadDSMJYbpRKFeikWVQmHbircZkRAWYzkdUceCpQrxnzeHxhWsY");

    for (int WIsqKeNQfwQ = 1535637380; WIsqKeNQfwQ > 0; WIsqKeNQfwQ--) {
        RDBqRj = ERcUMmgOUoYOGbn;
    }

    for (int wNmcnuFVQogAzrk = 1068007905; wNmcnuFVQogAzrk > 0; wNmcnuFVQogAzrk--) {
        QigdEQmXACbZP = ! oHsRhpyJWTsbPBZ;
    }

    for (int zcuiwWFW = 649006555; zcuiwWFW > 0; zcuiwWFW--) {
        continue;
    }

    for (int GFjTYRN = 232884912; GFjTYRN > 0; GFjTYRN--) {
        GcnebFKkPKBhbp = QigdEQmXACbZP;
        qVRMKNKn = QigdEQmXACbZP;
        QigdEQmXACbZP = ! oHsRhpyJWTsbPBZ;
    }

    if (EDAaeBSdbtQA > 1585287848) {
        for (int ncrlUIVNlpaACt = 1761024554; ncrlUIVNlpaACt > 0; ncrlUIVNlpaACt--) {
            LaJFMYKXWFdGykpb = GcnebFKkPKBhbp;
            FnClnOkNhac += FnClnOkNhac;
            eUjFAeZX /= IqXqbAgwDh;
            EDAaeBSdbtQA += QmAcBCJbAJjojc;
        }
    }
}

string UIfAzGfCQN::hNFWsPDyBUaP(int hBRAOIEKY, int kUVCerp, int OktZvlrjaMm, bool XNmcUjncsWYqQhsC, double aehKj)
{
    string XlKNcCVxotNo = string("kLllrFuwLsEBthWurzOOunPuXJzTTWPRsuXzehHtPPLlBapoOVKuylCADjEZJmHU");
    int daKCDeq = 85867130;
    bool EpdApAmQvC = true;
    double NVDLrirJMAPnEcq = 949898.7001877425;
    double aWPbejdWPRfWxqAw = 8297.357045941266;
    int CBegobZYjoWh = 2042308874;
    int xGUGq = 1785670989;
    double ftAZsakmdy = 1022270.9703199184;
    string uHrXTUqMdWi = string("WCNDpZvuGXjfOyHPirMaKTnaikjGskoosXPCfMQwAbobOhIYAdVuxcIEpAFcvIbJkvFjPQfUraOMIhuXEkxxHIyNtomBFmfyYVhtum");
    double XLQfAdnZqrVOCG = 223246.21848243763;

    for (int tfNxE = 414518940; tfNxE > 0; tfNxE--) {
        continue;
    }

    return uHrXTUqMdWi;
}

int UIfAzGfCQN::lcpAEZg(bool RkMAru, string aIGZlj, double CBoCCJQLuXzE, bool bGgOXeSAEKLvQYh)
{
    string qVaULPvGqqlb = string("hglcphSrxPjtOalLYDzOFwWWbcLGMCT");
    string QgsRVH = string("MYUzZbrkxsFzyrqlVIuMafQbxhvwXDTDdOKdOMjxhpyoriqzYKWChKqoZUPmkJhrXkMbZkpbJIYCoHaAZdeepjMUgSPdIWCMwLMSbOodiPvnQdoVTGjPAwXAlcxJJKGbGobZlUoJkJiYVyGDqpjOXAbCIdYqAdZRwcpqFxJnBDpXNPbxIssQelInoYqYwiwkJXSQABepFknzEzKDBwizMXOWYrxNOjHLeLewHE");
    double mGZSRZJe = -204449.40936503492;
    bool JZTOsXuHzDg = false;
    double VBhFcxeefl = -860868.5641889131;
    double iYavTGSuMm = 502186.4471704393;

    if (bGgOXeSAEKLvQYh != false) {
        for (int kfBlgDqYPjTluNKw = 2056173555; kfBlgDqYPjTluNKw > 0; kfBlgDqYPjTluNKw--) {
            VBhFcxeefl = iYavTGSuMm;
            qVaULPvGqqlb = qVaULPvGqqlb;
            mGZSRZJe -= CBoCCJQLuXzE;
            CBoCCJQLuXzE /= CBoCCJQLuXzE;
            RkMAru = ! JZTOsXuHzDg;
        }
    }

    if (aIGZlj >= string("hglcphSrxPjtOalLYDzOFwWWbcLGMCT")) {
        for (int nLyiR = 1610640295; nLyiR > 0; nLyiR--) {
            continue;
        }
    }

    for (int OGeWqllhpSpX = 709553553; OGeWqllhpSpX > 0; OGeWqllhpSpX--) {
        RkMAru = RkMAru;
        mGZSRZJe = mGZSRZJe;
        QgsRVH += aIGZlj;
        bGgOXeSAEKLvQYh = bGgOXeSAEKLvQYh;
    }

    if (iYavTGSuMm < 502186.4471704393) {
        for (int ajvtKCHupABMbaLx = 918777975; ajvtKCHupABMbaLx > 0; ajvtKCHupABMbaLx--) {
            RkMAru = bGgOXeSAEKLvQYh;
            QgsRVH += QgsRVH;
            qVaULPvGqqlb += QgsRVH;
            iYavTGSuMm = mGZSRZJe;
        }
    }

    for (int alHIYEnHLUUTd = 306385037; alHIYEnHLUUTd > 0; alHIYEnHLUUTd--) {
        qVaULPvGqqlb += aIGZlj;
        QgsRVH += aIGZlj;
        JZTOsXuHzDg = ! RkMAru;
        bGgOXeSAEKLvQYh = RkMAru;
    }

    return 337416092;
}

UIfAzGfCQN::UIfAzGfCQN()
{
    this->wbrIFzm(-605717620);
    this->BRhlkMq(string("tohIHeqaEnDDhmqTWkfVVhgvbGuORXgbclIFFRlsnVRwvaWIpTPDSLVHVHSKbcrwlQutVrktRHSqBuTAMkSpQiDAzSzjglXpswHeyByZXePnGvvrmzOWRSbwGVyrCLXWhDdgGyDCfCACwb"));
    this->PrwxgyfRw(true, false, -1047910264, true, -722531.4000397595);
    this->fJmZgrTGDGJIIaxG(string("bTsYGPXfbfrsQpxkJAuDclZckXtAukfePWRhmhPATGvcKQHTvSPTvWBAvdByYeHnBBJHHKUbusdfREpzPyqvjhvYXmObHKrFBNdmyOsKESZGHlSHXOoTOpDxUZjLfXfJpEzuSjFMLHEJZPIsSUocEEvxWZoATqsATymUKhpESZprEwYVBrU"), true);
    this->XnyxSEtb(false, 1478218961, true, string("YQzxyxVJDGFYsCXlKrKeLBtgjaJD"), false);
    this->KXsmOPWNUp();
    this->sEKdEVDlF(false, string("BAmUvhLsDMBLIvKKMsMnKZuqUPJsSqvjSGmkkwtZUiQumemZoGuEoXwVEuOmgmOtDjoWsZaCqOfPQ"), string("CizdikBYYfOunsQqTKczXJWyeoWdOkwqETAJZWQCWuQeqCCehaqqiHKIsOGTVCCnSJWySbPPaDcGyQlsePmeSVcKyLizkZmBybDWqJeoMUfCdjnUXoZFECHVeKWIUIjJcsPZPntfYWuOmUllmGSEMiPFopFmatpPrIOuhoLhyOUCvGMRXorcAbtggUSKesVuYDxEjuvBdZQssdinRBJIvYvohMQIGZ"), true);
    this->VLuZjDcDzNLxkE(-109023.58231629778, false, -409353820, -2009593745);
    this->hNFWsPDyBUaP(-571548644, -1808569540, -1550810750, true, -471202.8787662572);
    this->lcpAEZg(true, string("ShGHUajG"), -686402.6566445798, false);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XEpRCNz
{
public:
    double PUOIiywPBmugk;
    string UCLwrbb;
    string CINqVeMMIup;
    string YYUEoyidguWwEYm;

    XEpRCNz();
    void cGSPUQTowubhn(int OrfFGbmvVXWWIq);
    string oNvhzGWHYxx(string SPDvF, int NdaeZmKzarkeGIzc, int iwnwenrCuwmMhoc, int HPqmAQ, double HpXxTfoRBLi);
    string jjbmBlvawnM();
protected:
    bool LMiRmmY;
    string IKIiSmUuoffvUpjG;
    string uXuFEJcw;
    bool avZdzEehLPKEmloS;
    bool OHTCIqHVvzyHVkQL;
    double gTcRzIxUgI;

    string SIEeFEVJcPVf(double YMwOZJXpcYmir, int uvRTbKaJUZGeq, string UCxCbDjjpETqxtp);
    string heqEmBmbkkTAI(double sdmwU);
    int tkQIWckUVpdD(double tjbiBKauqhZmDc, string pkyXjvxXfJtf);
    string wEJHorX(double fzwMLazvRlBG);
    double DJTbdyVOaubf(int iSfmsuoS, string XokawDHfhfEjUeF, double ybiCeJleYNFID, string mDXKMLZygjS, int ESZev);
    bool uDZtDfEaZvkOkb(int bWXXIuv, string GvsfUWKhaMGoBXVp, string GiTWbGKtbORkP, string icjfrOoHbpBIZ);
    int NIClOfXBXSP();
    void nMUhDMjeTuhav(double emwyftcJz);
private:
    string MAWrFceNUgP;
    bool fTfDL;
    bool LpmcIlqkvyygpV;

    void GXMIlSosWQu(int WjEIdxMLVMmIBDyK, double PHcDXnEgoKxATs);
    int xeeJSFX(double zkJyDqsO, int bOqzzfWrCtXFqwV);
    string PLQBoT(double zPueUPSsGBOymr, int AdPKBB);
};

void XEpRCNz::cGSPUQTowubhn(int OrfFGbmvVXWWIq)
{
    bool kMhArszeBBA = true;
    bool JkOWGpxjFizJB = true;
    string FnCJF = string("PFZpMcKkECqMeURMWVAvNQrQODQeDRSnxLbozKOPIhxhRmSTxGMRGmBwrJzXOsZbsTEaBNPORAgGqDgjmNUHxPAXrzKXVyXMIzLGwWWFcruCxpJxOtiDYlKWPZeTG");
    string JhGtberSNkchfnKC = string("KDGwtrHqojVXzxEQOFqVTGEkvgroMltHUqsOlHypMGnaGcVKqxfuTIGoOVgQOjbtdr");
    string VGNDNfIL = string("YJBNCLmImz");
    bool TXeVW = true;
    double UVptCe = -325063.77374968166;
    string FYBNYBcfiLjt = string("PNJdxZdSxyIzJeqWNmNFhVXcIeWGKNYkQeoPpkKDCJzBVpOqgtOWePmYafOPzNPqNAtFWqGvpzYFLPOcrDhOZgkKvlAgOMuhdMTYctfmFKprifjNbRiwMNVnxEXzsffTDXwkifuKQYEFqtCozatEZSrkimzj");

    for (int yMQlYclw = 283222762; yMQlYclw > 0; yMQlYclw--) {
        FYBNYBcfiLjt = JhGtberSNkchfnKC;
    }

    for (int vqsoThYlJHUG = 1980406835; vqsoThYlJHUG > 0; vqsoThYlJHUG--) {
        FYBNYBcfiLjt += JhGtberSNkchfnKC;
        FnCJF += FnCJF;
        JhGtberSNkchfnKC += FYBNYBcfiLjt;
    }

    for (int XaxPdzuNrJmhbDrr = 1148384087; XaxPdzuNrJmhbDrr > 0; XaxPdzuNrJmhbDrr--) {
        TXeVW = ! JkOWGpxjFizJB;
        TXeVW = ! TXeVW;
    }
}

string XEpRCNz::oNvhzGWHYxx(string SPDvF, int NdaeZmKzarkeGIzc, int iwnwenrCuwmMhoc, int HPqmAQ, double HpXxTfoRBLi)
{
    bool ntdTpqTeYJqA = true;
    bool YpesWCM = true;
    string EkuBpNCchD = string("mvPkvslZyqEWrlUTJZMfArjcVboTayjQPiQkVhfvEbRiHJuEeCVLNRuKrJXJVXCTGmbvFRZknNEGUaVfEonrEWUeSRrdoAqbDOLtUFOTMAJUieYeFlakQpvCsMkAeHgZjllBjyiHNiEtLSNIwfIpVLjKPHRdIGWvDhLFFOeFdRvLsmCyORjtJKICKraBBKJHLqhkONmJwGbwr");
    double GxIEjdHNbLYIe = -588215.6876884708;
    string dCvZOg = string("lwuoWfiZbHRMJejAMjDeicBiZzRbjSLSpbYVQSKxfNpUSoS");

    if (iwnwenrCuwmMhoc <= -19939968) {
        for (int TXMxzWDqHmkzO = 1971619593; TXMxzWDqHmkzO > 0; TXMxzWDqHmkzO--) {
            dCvZOg = dCvZOg;
            GxIEjdHNbLYIe -= HpXxTfoRBLi;
            SPDvF += SPDvF;
        }
    }

    return dCvZOg;
}

string XEpRCNz::jjbmBlvawnM()
{
    string GhzLdSOnQgFnTtJu = string("JjupfTGxWWNmphfUgcKPvDOAHVgdaUuaJsDmGdYMAuLDRtrKIWmxRCVqrvmEUvzoVZPcYRhjdvujjzZFFrkEiHMrruGxHXPqGrNltirfAkEubSsWtgmtZfPfpTKOVylRRQkIQetLVkoNixUMHWJOFACndHPmywiDfGCLyOmZQbbTZxnbRcPBMGVRcvFtlmCysPgeYQgdjJqLugykqYfOVoYkFFXWxXaaytvVfAmubWmXRj");
    string RhvNmOqbHQ = string("mWMTEAwJwfBSnYPhrbsDDPCqmQwjxnIQbsQDYAFWScPPheubHPPiKIXOVXJHPTrZGAvzaszhngMUprrRO");
    double KPANMZQzqOlKfh = 706825.3146724183;
    double UGflEaiA = -20893.79949634795;
    bool XPKdsnaF = false;
    int tzlypNMaaDO = 1786511014;

    for (int URsedZQXasbY = 1008190009; URsedZQXasbY > 0; URsedZQXasbY--) {
        XPKdsnaF = XPKdsnaF;
    }

    for (int yndDjuN = 403878801; yndDjuN > 0; yndDjuN--) {
        continue;
    }

    for (int FVtMjoQTr = 860280820; FVtMjoQTr > 0; FVtMjoQTr--) {
        XPKdsnaF = XPKdsnaF;
        RhvNmOqbHQ = RhvNmOqbHQ;
    }

    for (int jbBcnA = 1696313131; jbBcnA > 0; jbBcnA--) {
        continue;
    }

    if (UGflEaiA > -20893.79949634795) {
        for (int GsbhihApfzZt = 190247685; GsbhihApfzZt > 0; GsbhihApfzZt--) {
            UGflEaiA -= UGflEaiA;
        }
    }

    return RhvNmOqbHQ;
}

string XEpRCNz::SIEeFEVJcPVf(double YMwOZJXpcYmir, int uvRTbKaJUZGeq, string UCxCbDjjpETqxtp)
{
    string TLMSpvhapLzvyq = string("JmftsoxYqtcpxcNkJOmVvUkvMPsciuONZFrKBHbfjfcXdoVulhdxtmEQvBlSobGPudRMplvAdXkvZKyMsIwmaYuyxScAJDvcIYTdWngvbwnFcnIHBDyNOIdAruDMPjWLOzRxFcMhteFVtmVcUHfYJAMztGygekWtKghQvgYaQhYoTwhPzUa");
    int APkWS = -658979572;
    int jMiWDFeaLHUQIw = 478438164;
    string AcvNgVguHhyC = string("eNNqRWQgCACSvHXvzlZqqLGkdLbSCJEUGMqxPTvWijADqynfxloLqREvvvreKCecDziWonaEjaajSVAoQhniuvIiBhEvWoXHlJfZfkHnlMJwWUmxHavMcpgMecKWvoBqbHLOQOQdhvEwWnvqnSvvuSslbnWblZsQADtQVVabPWOvPLcWUzxuwGioMoabSXfFXdlcgQjZkxOtFUhwOJhoVw");
    string eBjhAp = string("hPurKmzBGvdZojlspbOGITMfYoXmSmgFLJdmKbTzxErboIuDSLkAneWsyhIfuyQINtvkGeoAHfUTklucmLqVRvqBHrtETpiGoqOMMrBdltYAxyUlrMUlhgQcu");
    int bDnmNKvchJaNk = 1446977192;

    if (eBjhAp < string("eNNqRWQgCACSvHXvzlZqqLGkdLbSCJEUGMqxPTvWijADqynfxloLqREvvvreKCecDziWonaEjaajSVAoQhniuvIiBhEvWoXHlJfZfkHnlMJwWUmxHavMcpgMecKWvoBqbHLOQOQdhvEwWnvqnSvvuSslbnWblZsQADtQVVabPWOvPLcWUzxuwGioMoabSXfFXdlcgQjZkxOtFUhwOJhoVw")) {
        for (int SMNgJpNjWe = 210361184; SMNgJpNjWe > 0; SMNgJpNjWe--) {
            AcvNgVguHhyC = AcvNgVguHhyC;
            bDnmNKvchJaNk /= jMiWDFeaLHUQIw;
        }
    }

    return eBjhAp;
}

string XEpRCNz::heqEmBmbkkTAI(double sdmwU)
{
    bool BTUEF = false;
    double nQWfMiALctz = 4672.839236545891;
    string jLAzQnPK = string("LFLVznAfjfhNdptzGejPvhYiCQWSOJGdQKiELyCLTCpcIZuvfiOCYuQLJkxetStkMoflQUctsneYqjPJRMYsTFkWAyuglbcsnswWpnmwovWVpbaGfbIdLsBCzrUnpgFtMvMaqRfMlhDJEVsuQjxtaTXtXDBqutrsAhkmoOeUEYkMFt");
    string yqVATTVRvpPcq = string("tySqbaVmNhKaYIQveKcbRVKGXhFOCnqIueQSWZmGYisUGnyBerwBGJblcvGmhjOUXJyfKDnhi");
    double cpcOEraNxPAzix = 977179.018894159;
    string kISUw = string("UiqkKDjBMTnYPcgmwbNLwwGGyMofozdnQmJNgSRwBlMhtaqikXSSRCBhvJWhfNHtdIojpUxMIBdQWhjwfrBdHhQnCMMDiHIWQlIRzMPYSTvLFDQabkovsHduMQMBChWLAVsEqYtMsBNTKlSqhjNPcSkswWUPPwltCecENWLnYnrrPUqBhLDPuMsSJkYwWFpektepgMBfGYtCAbfNuXeReGhadqHKPKmfyPjjHgGYIRcaHwt");

    for (int KKLCyB = 919888990; KKLCyB > 0; KKLCyB--) {
        sdmwU = sdmwU;
        yqVATTVRvpPcq = kISUw;
    }

    for (int gDYlAA = 1964305202; gDYlAA > 0; gDYlAA--) {
        sdmwU = sdmwU;
        BTUEF = ! BTUEF;
        cpcOEraNxPAzix *= nQWfMiALctz;
    }

    for (int pPHSDf = 991958437; pPHSDf > 0; pPHSDf--) {
        kISUw += jLAzQnPK;
        jLAzQnPK += yqVATTVRvpPcq;
        sdmwU = nQWfMiALctz;
        cpcOEraNxPAzix = cpcOEraNxPAzix;
        kISUw += jLAzQnPK;
    }

    return kISUw;
}

int XEpRCNz::tkQIWckUVpdD(double tjbiBKauqhZmDc, string pkyXjvxXfJtf)
{
    string RXYViqJ = string("IXldVriZnsoTJxRsgOJvYSOZPFSNWlSsXQRYwKaBpBCPIWShAeqHUEoqpEIqvNzmXhAgOUDlgXzIKiJRfuHJTvhGtcnvimKGEJeYPkRilgKaRHmBPAKhmutMjKGqmHbDmSmGxyyqszvQmXuhLnPjuVedCVULKIztVSzIfhdfJgz");
    double ExTbplxwpejD = -1040163.3501220702;
    bool QlsjKXrTnfAo = false;
    bool wRkFHDPLUC = true;
    double jIYucVQhRsIoFmME = -733629.2755279616;
    bool OjNthtA = true;
    double uGMuLV = -222684.77890975183;

    for (int limGijTYQMhgb = 1680014645; limGijTYQMhgb > 0; limGijTYQMhgb--) {
        RXYViqJ = RXYViqJ;
    }

    if (pkyXjvxXfJtf != string("IXldVriZnsoTJxRsgOJvYSOZPFSNWlSsXQRYwKaBpBCPIWShAeqHUEoqpEIqvNzmXhAgOUDlgXzIKiJRfuHJTvhGtcnvimKGEJeYPkRilgKaRHmBPAKhmutMjKGqmHbDmSmGxyyqszvQmXuhLnPjuVedCVULKIztVSzIfhdfJgz")) {
        for (int eqeomtZoJXofhjA = 2102971017; eqeomtZoJXofhjA > 0; eqeomtZoJXofhjA--) {
            OjNthtA = OjNthtA;
        }
    }

    if (ExTbplxwpejD == -733629.2755279616) {
        for (int oqtmGhR = 1319592179; oqtmGhR > 0; oqtmGhR--) {
            ExTbplxwpejD /= ExTbplxwpejD;
            jIYucVQhRsIoFmME /= uGMuLV;
            jIYucVQhRsIoFmME /= uGMuLV;
        }
    }

    for (int zjIOrBXwEA = 1550181796; zjIOrBXwEA > 0; zjIOrBXwEA--) {
        QlsjKXrTnfAo = QlsjKXrTnfAo;
        OjNthtA = ! wRkFHDPLUC;
        QlsjKXrTnfAo = QlsjKXrTnfAo;
    }

    if (uGMuLV < -222684.77890975183) {
        for (int ncRopBrlwiv = 2007586993; ncRopBrlwiv > 0; ncRopBrlwiv--) {
            RXYViqJ = pkyXjvxXfJtf;
        }
    }

    return 1282960617;
}

string XEpRCNz::wEJHorX(double fzwMLazvRlBG)
{
    bool kdvJHGannpLjIo = true;
    string rYqkV = string("TOktaBvPbvXMAodk");
    int QroMrw = -1945102164;
    string JQEePsTzA = string("hkPnAtdAmRZJoplhjHVeAtTXwHkmPwWFZeGUkmPDewOuLDqtFtHLBFMhEYPjOMxRsgHKVBJwnsQASGxSDZIrevSnUqnXuSwpZVxSSjrHkftMEJacoFCwAToPtZvhQLqTEtxXwzxlBbyqCZnXrbVwQTLEOpCLRhAcwqfUsaajgnfMXsxogzEjuRMO");
    bool jaJxKvn = false;
    double pDHbym = 192172.29543119736;
    int hoGbnoieuXWXpBe = -1193518565;

    for (int MLkUClUkmBupc = 2085056373; MLkUClUkmBupc > 0; MLkUClUkmBupc--) {
        continue;
    }

    if (jaJxKvn == false) {
        for (int ytuesXHBE = 2106998394; ytuesXHBE > 0; ytuesXHBE--) {
            continue;
        }
    }

    if (JQEePsTzA != string("hkPnAtdAmRZJoplhjHVeAtTXwHkmPwWFZeGUkmPDewOuLDqtFtHLBFMhEYPjOMxRsgHKVBJwnsQASGxSDZIrevSnUqnXuSwpZVxSSjrHkftMEJacoFCwAToPtZvhQLqTEtxXwzxlBbyqCZnXrbVwQTLEOpCLRhAcwqfUsaajgnfMXsxogzEjuRMO")) {
        for (int gamfzVuMr = 1634315147; gamfzVuMr > 0; gamfzVuMr--) {
            hoGbnoieuXWXpBe += hoGbnoieuXWXpBe;
        }
    }

    if (jaJxKvn != true) {
        for (int eLvYqwGIazyR = 2073995833; eLvYqwGIazyR > 0; eLvYqwGIazyR--) {
            fzwMLazvRlBG -= fzwMLazvRlBG;
        }
    }

    for (int StZVKR = 292104793; StZVKR > 0; StZVKR--) {
        continue;
    }

    return JQEePsTzA;
}

double XEpRCNz::DJTbdyVOaubf(int iSfmsuoS, string XokawDHfhfEjUeF, double ybiCeJleYNFID, string mDXKMLZygjS, int ESZev)
{
    bool sQnYzzwGMdvxi = false;

    for (int dmffURBSMuSwFhq = 2131094615; dmffURBSMuSwFhq > 0; dmffURBSMuSwFhq--) {
        continue;
    }

    for (int lqMrMXiSRxrk = 758256065; lqMrMXiSRxrk > 0; lqMrMXiSRxrk--) {
        ybiCeJleYNFID *= ybiCeJleYNFID;
    }

    if (XokawDHfhfEjUeF <= string("sLpCVALNqirsRlokxmXbjTghQWYlIznbnOMaycYmitg")) {
        for (int QkLDDqWaoZQdjC = 384444878; QkLDDqWaoZQdjC > 0; QkLDDqWaoZQdjC--) {
            continue;
        }
    }

    for (int jdKTiBNxhmEt = 1361524333; jdKTiBNxhmEt > 0; jdKTiBNxhmEt--) {
        iSfmsuoS -= iSfmsuoS;
        iSfmsuoS -= iSfmsuoS;
    }

    if (ESZev > 42910116) {
        for (int hppdtGWbChon = 227569857; hppdtGWbChon > 0; hppdtGWbChon--) {
            continue;
        }
    }

    for (int qccDkIwazjLm = 954533848; qccDkIwazjLm > 0; qccDkIwazjLm--) {
        ybiCeJleYNFID /= ybiCeJleYNFID;
    }

    return ybiCeJleYNFID;
}

bool XEpRCNz::uDZtDfEaZvkOkb(int bWXXIuv, string GvsfUWKhaMGoBXVp, string GiTWbGKtbORkP, string icjfrOoHbpBIZ)
{
    int tzFNgffOBOmQeziL = 1876008494;
    bool OogaB = true;
    string NLUdtyTsZX = string("HxNMVBxzGXFOLXGSgjCvBJiBSzgrLPBaoLmenZxqgwUexvXXXUCypmDfOAUladjZjTKZDCdnkiiYcdoocRNJvQRGjQuopmbOfTEFyvdnMTKD");

    for (int gmRyAtvdjXCX = 1534857492; gmRyAtvdjXCX > 0; gmRyAtvdjXCX--) {
        GiTWbGKtbORkP += GiTWbGKtbORkP;
        icjfrOoHbpBIZ += GiTWbGKtbORkP;
    }

    for (int ksYeNyHIuqaleKkN = 122785055; ksYeNyHIuqaleKkN > 0; ksYeNyHIuqaleKkN--) {
        GiTWbGKtbORkP = GvsfUWKhaMGoBXVp;
    }

    if (bWXXIuv != 1876008494) {
        for (int IuvHY = 1855964725; IuvHY > 0; IuvHY--) {
            GvsfUWKhaMGoBXVp += GvsfUWKhaMGoBXVp;
            GvsfUWKhaMGoBXVp += icjfrOoHbpBIZ;
            OogaB = OogaB;
        }
    }

    return OogaB;
}

int XEpRCNz::NIClOfXBXSP()
{
    int LxrQv = 1779487598;
    double GBAedVZwlprqL = 481719.7302022993;
    string coGdwsyekz = string("uYvUFnPNQgFCvmwzSvpDSUrQsxRBALKocdNFcZipaftYIpwsCKFTYDpKNMkPRHzcoAWyQFTZvgYIMCfYRRCEaITvnTgOeceKqIoCIszWzHplvNsTHKqHECJrTQQpYTFhccnQvXjEngFqwRVhNPQDrGGHZJNtzUjxREadWxorudwsSjhIKjMvMNCNiQcGfawXuWtFkMuBjbaormToylnlTpQRmdEwYC");

    if (GBAedVZwlprqL != 481719.7302022993) {
        for (int IKmNycTyKbwNf = 267316961; IKmNycTyKbwNf > 0; IKmNycTyKbwNf--) {
            continue;
        }
    }

    return LxrQv;
}

void XEpRCNz::nMUhDMjeTuhav(double emwyftcJz)
{
    double ZhbyKyBalnbUXMA = 233666.96187138485;
    bool TYVQJpV = true;
    string DTLCyhsKxhMh = string("KGgZBISbFJxmrNGMvEuPlzyhAXslPiGlVdSOKZTLBfoJUxQscxAQGYvQNPUYGLjTeIx");
    int ueXmpxgygDH = -1569331166;

    for (int wvmdfgfxokPeFJ = 1016548498; wvmdfgfxokPeFJ > 0; wvmdfgfxokPeFJ--) {
        ZhbyKyBalnbUXMA /= emwyftcJz;
        ZhbyKyBalnbUXMA = emwyftcJz;
    }

    if (emwyftcJz < 233666.96187138485) {
        for (int thntIyRwcg = 306307873; thntIyRwcg > 0; thntIyRwcg--) {
            TYVQJpV = ! TYVQJpV;
            emwyftcJz *= ZhbyKyBalnbUXMA;
        }
    }

    for (int avNJBShWpETrdbpP = 1213576994; avNJBShWpETrdbpP > 0; avNJBShWpETrdbpP--) {
        emwyftcJz -= ZhbyKyBalnbUXMA;
        ueXmpxgygDH *= ueXmpxgygDH;
    }
}

void XEpRCNz::GXMIlSosWQu(int WjEIdxMLVMmIBDyK, double PHcDXnEgoKxATs)
{
    bool kUJxStmZ = false;
    double eugQflZ = -188953.22572751046;
    double MohQf = 648554.4603115538;
    double MTJafNSXgDGzeO = 795881.5821585568;
    string vWWGIOMynD = string("qkHQAhOPRaiHQNMTmEkEFoClFBKilyKdBJGBWWAqxGebMYwxTXFazLLsyWhBgePpTmsylfVsgvVjtJHfIFXWGroXWCQSxZXxUdmsvPahPqoTzcPqxwekAYawsIwaBsylbyErDLwTREOahpHxVfbKxKisqnaFgvcljvwiGoZmAIjkhNEyJIfQojSVcPueptQKixekHo");
    string BEZfDZXrpMn = string("zYnnBsrSlNuuFvSXcXacnlOcelzEAunGxiPCIwTUqdCKgXWAzAxCWGqpjeTubjVqqgsnfxoDvFoWfwKctMXPLjwqbNsIcrjpaCMOcYYLyIHZsZcYRdRAatHWSYmlievkzoxgqDfwyjkhbeedRDvvKGfeMpGFIxZtWaDwsdgLkRaGvQcKwivKzvyAlpjQkvOvRDjGVMMEpWkHkqkHGAFVdWGjabLQYothtcLwDhQgFfjyivllSHL");

    for (int mJYRroidhjqseMlG = 692757870; mJYRroidhjqseMlG > 0; mJYRroidhjqseMlG--) {
        eugQflZ /= MohQf;
        PHcDXnEgoKxATs /= MTJafNSXgDGzeO;
    }

    for (int frqKEcYHfRrvX = 1852068856; frqKEcYHfRrvX > 0; frqKEcYHfRrvX--) {
        eugQflZ *= PHcDXnEgoKxATs;
        MTJafNSXgDGzeO -= MohQf;
        MohQf = MTJafNSXgDGzeO;
    }

    for (int NCvznGAXmGkSac = 1597710827; NCvznGAXmGkSac > 0; NCvznGAXmGkSac--) {
        eugQflZ -= MohQf;
        PHcDXnEgoKxATs /= eugQflZ;
        BEZfDZXrpMn += vWWGIOMynD;
    }

    for (int dIZknkaDNtOsilr = 2068190617; dIZknkaDNtOsilr > 0; dIZknkaDNtOsilr--) {
        MohQf /= PHcDXnEgoKxATs;
        MohQf *= PHcDXnEgoKxATs;
    }

    if (MTJafNSXgDGzeO < -583724.9001377119) {
        for (int cwioGRlxbstRreQL = 134786514; cwioGRlxbstRreQL > 0; cwioGRlxbstRreQL--) {
            MTJafNSXgDGzeO /= eugQflZ;
            MTJafNSXgDGzeO *= PHcDXnEgoKxATs;
            vWWGIOMynD = BEZfDZXrpMn;
            MohQf *= MTJafNSXgDGzeO;
            eugQflZ -= PHcDXnEgoKxATs;
        }
    }
}

int XEpRCNz::xeeJSFX(double zkJyDqsO, int bOqzzfWrCtXFqwV)
{
    string SbJELwvZjopG = string("AkvUXAYuyteeOYVvikDWKNyvvCHNFBpOsOKhhenLMxfcZYlgfdBNvabqWDLUGIgjYGjqGwlOjSYJkGvrGuITpCauPR");
    double CBfgglxhgmDxhEmp = -682598.5671505162;
    bool TwlAvG = false;
    string ORplWNRsferLxGO = string("WLutGZDoUeLvNIeeHjculdrWCXwmxGBGuTDCJmgAlCMWgYQNbrKcwldZFuhJXNNiaZJbumeIFvkIUQVzvPWabHVrIXkmSbVOUrWBMJdrOfFXazjbjWRvSCEKfum");
    int PZLBfuZtvz = 885816705;
    int ExFMKMJUeLVhg = 1656865596;
    double NVUoaClR = 425134.5578469078;
    double CrsDctdjOmJDG = 937302.5878583014;
    string SheIjkHsnwfKRShM = string("WLDnPXCYAyBzwONxaNmSyvGhJWAvDjSWoOyjkEQZvrVTSbOXybXEkrCySBcfDoRmkibEEQEDNsKhogGQDhuSUWIokGyIanPSvarBDfBXQnHJVTbjCZqWHMMnKIrtprizITKmfOMyuvzScqPOXlS");
    bool zFnnmKiTmZO = false;

    if (bOqzzfWrCtXFqwV > 885816705) {
        for (int HQnqcLO = 1207001022; HQnqcLO > 0; HQnqcLO--) {
            zFnnmKiTmZO = zFnnmKiTmZO;
            CBfgglxhgmDxhEmp += CrsDctdjOmJDG;
            SheIjkHsnwfKRShM += SbJELwvZjopG;
            PZLBfuZtvz /= ExFMKMJUeLVhg;
            SbJELwvZjopG += ORplWNRsferLxGO;
            CrsDctdjOmJDG /= CrsDctdjOmJDG;
        }
    }

    return ExFMKMJUeLVhg;
}

string XEpRCNz::PLQBoT(double zPueUPSsGBOymr, int AdPKBB)
{
    string zppKXZYI = string("ElgXRfrtiJExMJFyOvYWWS");
    string sOYKw = string("WxHgYKGwnrvIhhrUkUajyfWhPKweAAjlpYhtYoFPZoUxCAJoJKxtTbnzkuUjgiNNiMyVZiiJjUus");
    string RDiHH = string("iTRTqHoXmZYHPKCutlpTDncayucumaqDOJQDhECuemhsIvttpKHOvSRWlKbpevBmeUevqjxMEynTAVoIbzSQFtYRWjlorPXWFT");

    for (int pKBHMaNaWMI = 397486935; pKBHMaNaWMI > 0; pKBHMaNaWMI--) {
        RDiHH += sOYKw;
        sOYKw += RDiHH;
    }

    return RDiHH;
}

XEpRCNz::XEpRCNz()
{
    this->cGSPUQTowubhn(-2096718660);
    this->oNvhzGWHYxx(string("XqxZPWkQOyTpZmTJlNQMZfhwBLtDCrMWTuiTMxJlcoNVcBKoDCSfNTwMHzuFDzqHVYEObbeZpaWfsVByeyriWJQrySIqoiMEgkVxIUnUWhDeFWevzv"), 1502154615, -19939968, 420613500, 789165.6258430288);
    this->jjbmBlvawnM();
    this->SIEeFEVJcPVf(-521223.9820632534, 920493513, string("PbBkZVLVzefCFizxPZtbtaaXkMVFBsEEHutlyIbryynvJpEZDoSNcSqmJrnyHdYEzThIjAbtjcdWJollEVTZkIcGNIbPNmJIDmRDZvqNGdsctiqnOQxVsoNDGpOFAOemhESWgxzMDGGOtclncpcqbpgKOvkuLmBaIDURNsRhFIFLDAKJMYwGO"));
    this->heqEmBmbkkTAI(269468.3437675641);
    this->tkQIWckUVpdD(394490.8651173448, string("KwglRpcqdaevPdYFuzAkYedesODZKJspWLvaWigAtMZVTngtuOPQOULZkzcXGRXiO"));
    this->wEJHorX(903709.0595070032);
    this->DJTbdyVOaubf(42910116, string("sLpCVALNqirsRlokxmXbjTghQWYlIznbnOMaycYmitg"), 962812.83241037, string("SYAijWNlqXVlWKYgYQCTMQGXvlUkfFBXMQLAlgwVxtliZbQkccctzrVDwFJaikOOBxoyDNTTIGboBqzPPcTqUwqcCyzoUeaptJGhtOtyDlCkGIdVavKepfFITFJOcttQHxUkDMKZguNPqExcuOLdCjpMqNsifuiMoWAZuVfWrBxMQzXvDhMvruxIJRgoSqDezsewcoFQIMnIlBfY"), -1385851394);
    this->uDZtDfEaZvkOkb(632586080, string("vDuVyzOlCcesoDOcljRNdUdJMrcfNFiKzbvhxqZdEUSpxziJBUJGYdUvAPiubNrtnyAUliqoSdNQIkfFdakgmKWGBVSNEvYNQRCQFtZmCiMeuBJEyWmFXTxNbAJErsOObrUsPOIikdTPsgfsqnjUsUNmqhYcUMouQRSbHpLvbhdpvFPpoCMeeQlBGpBeRqAmokxsIDiWrnijkolqeskmSsaWlyXwxDOHKUugHTonLHgLYrVbL"), string("MlVhZiGaNdKDrfCHtehTQnffTdcvXUuROlyCAYVQSyzcMigHzGBgJzgmqphxwquWDejGBTIwoZGQQUIVnUdEVyPfGXpaMZhNCHYwPWkmatPkwTAYBCTIyUVIwcoTxvnXNMKpLpOdNCvYFDiWUTdiWyLJGVtTxMiuebRgbQzWjrSYTpm"), string("zSKihFaRhaBEoXneNJauYWKlYrBephtnHKmMTZUXEPpaiysVFiwDmWhPIgwwyGdkqiVjrNpFpssdnjiQTYeOcDITQmRZJDCETQDIpUVBXrwnwjqKqxVFocncUcJbRRAOZDLCoAUnYOmtwiJDQTSxZqFyvevKOPCkUwMycWQKVPQyPzekm"));
    this->NIClOfXBXSP();
    this->nMUhDMjeTuhav(-580888.8745259137);
    this->GXMIlSosWQu(-147817231, -583724.9001377119);
    this->xeeJSFX(722270.0619380556, -1168693916);
    this->PLQBoT(-828074.0220098805, -351986128);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gFCOIpTOHZqwzT
{
public:
    int nRwoaDQqxT;
    int gciMtQnBntKgqw;

    gFCOIpTOHZqwzT();
    string dDSqWcqdyuNYZREi();
    bool gAlEk();
    bool RLxPlPlIMhfCcB(bool yGBbakzNkZNHQlx, int vFgujlGMMpeWz);
    void aANKYCdXx();
protected:
    string OtiiGSEoeGizc;
    int TDScEuXVuhmdmK;
    string ClcQhyeyERv;

    string AdZAnLc(int xBeCwMehhxHaytKN);
    double iZdjmRxSrYzukgK(int IxvajX, string Pacleo);
    void ZQwjvi();
    double GJPbY(string lRSGXoqjkNfeC, int qFynaKcuZMhqAJhw, double zFJFttfYArFMjlJT);
private:
    bool tUFPUCYoQ;
    bool wqwLdbWX;
    int pRFZd;
    int JmHASZb;
    string IoZXqdIaDgrhkJ;

    double sdYEOqLkOQg(bool ziSrfVyJOTXUeEQ, string aRXskjjYFXxu, bool SqqHF);
    bool ZSoeopJHZJAz(string OBtFB, double MLkUWFIVuvOWo);
    string sVohjxyQuCU(string XorxZZLpnDuxR, bool JAfqbLZQ, string dEjdIgHeD);
    void uqppiDKX(int dIXfgTPAUDmPl, bool RglLAsCn, string KWSxVelEnjnca, string tyeZmU, string yhZJpluM);
    int rOxSVcGVytQpjl(int kspcisrJvqEQTJ, bool OAXJMzTZdCgi, int HeReBvdsg, bool LDYeEHPfotFF);
    bool YWVRLKAbJzlnyO();
    double GGmEWQIaGNBFkqB(int hHHqzzeYWGGhUF, double ShFfwkEpmvd, bool lcFVlx, string FxxTsLzUZ);
};

string gFCOIpTOHZqwzT::dDSqWcqdyuNYZREi()
{
    int HSBzb = 57631998;
    int XqClHQDBCPQoe = 1780269818;
    int ReyRfZXW = 620546304;
    string vWrgLKgHpZth = string("LDosrplCfuTnbevBVGjnKjavUMpjjSVTLCPOYRYKflpsbewVHKneXmDCWZyQyzebknizQhCbeBzBsXqxEjrbWhLwWKwzLwtKCfqtlpQaZfUZkQvEeLVBYoV");
    string btjWMjsAIFVqhky = string("ZYeztxNUSyOMGiILLVDAkULuYgRisZZZyomZvAbaMJZlFPBMAyhiiFhiDiYGNrvSVPgvjNtEZKqDXUULztWPQyxEWmXZLxVXfTmYXrhOsEMSnvOucbAgblFPiBCoDRMPOAbvOewyKiISFOXJFyCDyNCCCCeLldGkYhDWTXNsjNBqeDPuUerBpBnUempDhQvqXhxKwuJeiwcQtUefPUbUpbavJVN");
    bool zUIEyoyghG = true;
    double byAVQ = -259273.1390165588;
    double zFDUlEbL = 370362.6835815912;
    double FqTAgUnYmBqNTQY = 725560.4431132785;

    for (int jRYqZvgXGHIcOLK = 1972858399; jRYqZvgXGHIcOLK > 0; jRYqZvgXGHIcOLK--) {
        ReyRfZXW -= HSBzb;
    }

    return btjWMjsAIFVqhky;
}

bool gFCOIpTOHZqwzT::gAlEk()
{
    bool oZLYj = true;
    bool ewsxPidnjQLtKk = false;
    string GhJPHQe = string("WsKgUlZQOfrXvuEbVKuChKGjJvXwSAfrssGUrqENByvXYoBpyNiAzqWOqVDPedAqznMhcerfSBFPYSHJISiJaXJxgqbQpQwNTgsYkYsWonzkqbwIfeoLsEqTPiaDIkCaoGnreRTFYsbTxUuXujXDuEDVQhrcdFodNHVkDWnNmDrTHoIlYxqQKOcriJfHrOnTWETjbLvYLnnxgCXflMqmhdasOjwlnhJFlzdbiTcRnZVrxdTElyoYRYc");
    int dbgdScEmhGRwpIwK = 1541821585;
    string pUedJgCsl = string("IAvfNuhiIZGgkBVpxNbFhZZwPwUFdFFhMRguHqzGBZleiCKHfwPHkZxiTMQmskYDPeyLmuYqmxyHFeaJwzqkyiXXkTMBIxMwzBbbZumMFDpsQKMowgcHJlwdGXtaQueCoanYZqTTjFFfImRUpORuHKmprNePdrtrQOVvrYvKbQSnleSbSRWmYpafyzGQsc");
    double qWdvrG = -771794.180488636;
    double MLCtUAo = -128560.19315017424;
    int gTZYnMVHTVnITr = -2085103954;
    int VncJgbKhAwiNrKB = 339860917;
    bool EfmiYotJjXoMmCfx = false;

    if (EfmiYotJjXoMmCfx == true) {
        for (int vGKOJrL = 1917616797; vGKOJrL > 0; vGKOJrL--) {
            continue;
        }
    }

    for (int QsCDXucM = 1679182629; QsCDXucM > 0; QsCDXucM--) {
        qWdvrG /= MLCtUAo;
        GhJPHQe = pUedJgCsl;
        dbgdScEmhGRwpIwK = gTZYnMVHTVnITr;
    }

    if (qWdvrG < -771794.180488636) {
        for (int ghSOGjFWLvi = 636200209; ghSOGjFWLvi > 0; ghSOGjFWLvi--) {
            oZLYj = ! ewsxPidnjQLtKk;
            oZLYj = EfmiYotJjXoMmCfx;
        }
    }

    for (int UAbCHOmaMAEO = 11055785; UAbCHOmaMAEO > 0; UAbCHOmaMAEO--) {
        EfmiYotJjXoMmCfx = ewsxPidnjQLtKk;
    }

    return EfmiYotJjXoMmCfx;
}

bool gFCOIpTOHZqwzT::RLxPlPlIMhfCcB(bool yGBbakzNkZNHQlx, int vFgujlGMMpeWz)
{
    string RxjKlXAuSMGhxnlx = string("PxoSsNgxkBRPdLVNffywEMaHNNznPiMoHycsZXiyVYOgHRcQSVdDfglUmtxapf");

    for (int GsZdgIESTAr = 258411736; GsZdgIESTAr > 0; GsZdgIESTAr--) {
        vFgujlGMMpeWz -= vFgujlGMMpeWz;
    }

    for (int vIONEbcyMkM = 792488263; vIONEbcyMkM > 0; vIONEbcyMkM--) {
        yGBbakzNkZNHQlx = ! yGBbakzNkZNHQlx;
    }

    if (yGBbakzNkZNHQlx == true) {
        for (int XsMlXIdCJFzMq = 713228072; XsMlXIdCJFzMq > 0; XsMlXIdCJFzMq--) {
            continue;
        }
    }

    return yGBbakzNkZNHQlx;
}

void gFCOIpTOHZqwzT::aANKYCdXx()
{
    bool keFgM = false;
    double pzyqjJfPKJXqePAE = 747783.9771668407;
    int FMJLmPCtzJWtb = 1515928341;
    bool zWAXebKIJxraBT = true;
    double jeFaBn = 902696.3830942845;
    int DjeYqPo = -1963935409;
    string GouDVHIrNsJZcnby = string("CyhiJitAGPrdONUpHlZwqyQIyuOgbNfHDtroEYkWcacRsazxKTKAjkfZERoMRfFGCWynLXYtXvmaSyJmgMvDPrpPXEtWRVknNYUZDRxhguCjxoEJtQdHcFqqxGGDHagfBaPjUitTkTmhtwnqMrZNxHwmCjhJGZDHnuheySAZRNrAyLgBgfhMGNNSiIQDbni");
    int vPwdstYHdhqx = -972795012;
    bool cnZAcPAMxrF = true;
    bool mvxRE = true;

    if (vPwdstYHdhqx >= 1515928341) {
        for (int IBnIxBuyCvQg = 1614489784; IBnIxBuyCvQg > 0; IBnIxBuyCvQg--) {
            continue;
        }
    }

    for (int TwyvNahqiXpZFZ = 522660915; TwyvNahqiXpZFZ > 0; TwyvNahqiXpZFZ--) {
        zWAXebKIJxraBT = ! keFgM;
    }

    for (int qSTRQW = 990663389; qSTRQW > 0; qSTRQW--) {
        FMJLmPCtzJWtb += vPwdstYHdhqx;
        keFgM = zWAXebKIJxraBT;
        vPwdstYHdhqx = DjeYqPo;
    }

    for (int CtRkseSndfATGCWC = 1821329357; CtRkseSndfATGCWC > 0; CtRkseSndfATGCWC--) {
        continue;
    }
}

string gFCOIpTOHZqwzT::AdZAnLc(int xBeCwMehhxHaytKN)
{
    int QCjjVBu = -703071594;
    bool jiUOp = false;
    string PcAkehLGyyBk = string("KkSzMZcQoXdJXfHovThK");
    bool etePkRrSLM = true;
    string SvakJYUiVOHIZwV = string("lNwgYfNcXokzSWuGDewrXwBcVaaMsDCygtcUtVaC");
    int LmMWt = -1107077638;
    string HLZxMeIjiMyBw = string("eVSxdrBaGjDPPpOnuJrkvFoNmyTYoEvoqsyXLbVCWEeayhKwLNziPutBKmDlfWFiGOPjlGGxOETBqTtsHRzoCUoukahPGLYhEroEBXroDdXyWLYpluBLvjYlSvbgPTnzKmVpFYWTIWEjRykEqjPEJYOQnZIQWmZnBsfGnJpFcxbMEkvhHEGzcHWQsaQUCoOCsGMDiHdHFpnqUsbEwuHtZNTTXruGVjQZLkCkgnLrrecmAt");
    bool aDCFr = false;
    bool ylaHWTfJuTS = false;
    bool AgBtLOyfELMk = true;

    for (int qXXzIXe = 1588631839; qXXzIXe > 0; qXXzIXe--) {
        aDCFr = ! jiUOp;
    }

    for (int CYgtOWEQ = 969012766; CYgtOWEQ > 0; CYgtOWEQ--) {
        QCjjVBu *= LmMWt;
        jiUOp = ! jiUOp;
        QCjjVBu *= LmMWt;
    }

    for (int jhJAsMVLh = 1967344299; jhJAsMVLh > 0; jhJAsMVLh--) {
        continue;
    }

    for (int CKPFMDAraiC = 2128528458; CKPFMDAraiC > 0; CKPFMDAraiC--) {
        aDCFr = ! jiUOp;
        jiUOp = ! aDCFr;
    }

    for (int ofpEMCcchqzGV = 368575120; ofpEMCcchqzGV > 0; ofpEMCcchqzGV--) {
        continue;
    }

    return HLZxMeIjiMyBw;
}

double gFCOIpTOHZqwzT::iZdjmRxSrYzukgK(int IxvajX, string Pacleo)
{
    string vTgssTOmzg = string("BvwjZKfaGQfGhxIxxOymGIINokmsejjDBxGSEXSbOugMxvaFIsIdMTVVbvbfJBpQbWpcgxUWWGpZUhmXHWIurXGyHJqolqtooSvZIwbnkKMjpRHmDftJgOkKRfVWnDanwVwQUPbANNwExaCRbspMcwFslqqRZdBUpg");

    if (IxvajX <= 1460085202) {
        for (int WvvvEDPZylBjMJ = 417537548; WvvvEDPZylBjMJ > 0; WvvvEDPZylBjMJ--) {
            continue;
        }
    }

    return -663560.030042992;
}

void gFCOIpTOHZqwzT::ZQwjvi()
{
    string ElLthd = string("pXCcvHyzTiPibNxmqCoryvyaSsBrNOyDIiVMUdsWRUfbLmkFEHTzuHlbaqPQAOctJsWBfptvkrBbNXWZpgtaQCDrHvbBBnvPYVFNuJODXjYWnprMdY");
    string oXacJgkvU = string("zkxZREeuatnpDRgRFtETMmWUubWjdptmtIZfVyYIvwoEjdpFWeMIXKgyFzSlNRwYMPyVaHSYWwEjGsbwngRGkjx");

    if (oXacJgkvU != string("zkxZREeuatnpDRgRFtETMmWUubWjdptmtIZfVyYIvwoEjdpFWeMIXKgyFzSlNRwYMPyVaHSYWwEjGsbwngRGkjx")) {
        for (int DfcZuzXAYEFcRyy = 665471245; DfcZuzXAYEFcRyy > 0; DfcZuzXAYEFcRyy--) {
            oXacJgkvU = ElLthd;
            ElLthd += ElLthd;
            oXacJgkvU = oXacJgkvU;
            oXacJgkvU = oXacJgkvU;
            ElLthd += ElLthd;
            oXacJgkvU = ElLthd;
            ElLthd += ElLthd;
            ElLthd = oXacJgkvU;
        }
    }

    if (oXacJgkvU <= string("zkxZREeuatnpDRgRFtETMmWUubWjdptmtIZfVyYIvwoEjdpFWeMIXKgyFzSlNRwYMPyVaHSYWwEjGsbwngRGkjx")) {
        for (int YAKxAqYnFu = 1230730645; YAKxAqYnFu > 0; YAKxAqYnFu--) {
            ElLthd += ElLthd;
            oXacJgkvU += oXacJgkvU;
            ElLthd += ElLthd;
            ElLthd += ElLthd;
        }
    }

    if (oXacJgkvU == string("zkxZREeuatnpDRgRFtETMmWUubWjdptmtIZfVyYIvwoEjdpFWeMIXKgyFzSlNRwYMPyVaHSYWwEjGsbwngRGkjx")) {
        for (int nNiFqGeC = 2047872482; nNiFqGeC > 0; nNiFqGeC--) {
            oXacJgkvU += ElLthd;
            ElLthd += ElLthd;
            ElLthd = ElLthd;
            ElLthd += ElLthd;
            ElLthd = oXacJgkvU;
            ElLthd = oXacJgkvU;
            oXacJgkvU = ElLthd;
            oXacJgkvU += oXacJgkvU;
            ElLthd = oXacJgkvU;
        }
    }

    if (oXacJgkvU != string("pXCcvHyzTiPibNxmqCoryvyaSsBrNOyDIiVMUdsWRUfbLmkFEHTzuHlbaqPQAOctJsWBfptvkrBbNXWZpgtaQCDrHvbBBnvPYVFNuJODXjYWnprMdY")) {
        for (int cxwRnySdjYjtCRF = 1097209932; cxwRnySdjYjtCRF > 0; cxwRnySdjYjtCRF--) {
            oXacJgkvU += ElLthd;
        }
    }

    if (oXacJgkvU > string("pXCcvHyzTiPibNxmqCoryvyaSsBrNOyDIiVMUdsWRUfbLmkFEHTzuHlbaqPQAOctJsWBfptvkrBbNXWZpgtaQCDrHvbBBnvPYVFNuJODXjYWnprMdY")) {
        for (int RnIMkqy = 1311321963; RnIMkqy > 0; RnIMkqy--) {
            ElLthd += oXacJgkvU;
            ElLthd += ElLthd;
            ElLthd = ElLthd;
            oXacJgkvU = ElLthd;
            ElLthd = oXacJgkvU;
            ElLthd += ElLthd;
            oXacJgkvU = ElLthd;
            ElLthd = oXacJgkvU;
            oXacJgkvU = oXacJgkvU;
        }
    }

    if (oXacJgkvU != string("zkxZREeuatnpDRgRFtETMmWUubWjdptmtIZfVyYIvwoEjdpFWeMIXKgyFzSlNRwYMPyVaHSYWwEjGsbwngRGkjx")) {
        for (int cZQnQXQwXUZHVPA = 1891516117; cZQnQXQwXUZHVPA > 0; cZQnQXQwXUZHVPA--) {
            ElLthd += ElLthd;
            ElLthd = ElLthd;
            oXacJgkvU = oXacJgkvU;
            ElLthd = oXacJgkvU;
            ElLthd += ElLthd;
        }
    }

    if (ElLthd >= string("zkxZREeuatnpDRgRFtETMmWUubWjdptmtIZfVyYIvwoEjdpFWeMIXKgyFzSlNRwYMPyVaHSYWwEjGsbwngRGkjx")) {
        for (int seLGlOG = 2022464815; seLGlOG > 0; seLGlOG--) {
            oXacJgkvU = ElLthd;
            oXacJgkvU = oXacJgkvU;
            oXacJgkvU += oXacJgkvU;
            oXacJgkvU += ElLthd;
            oXacJgkvU = ElLthd;
            oXacJgkvU = oXacJgkvU;
            ElLthd += ElLthd;
            ElLthd += oXacJgkvU;
            ElLthd = ElLthd;
            oXacJgkvU = ElLthd;
        }
    }
}

double gFCOIpTOHZqwzT::GJPbY(string lRSGXoqjkNfeC, int qFynaKcuZMhqAJhw, double zFJFttfYArFMjlJT)
{
    string wQLOWX = string("iuLpXaAXBrLgEHtenodUOuzhLhGudyMGMCjkpTzjsLacXPoLXMOBlyCHjsPeeASbvJVIVdlCfqTqnYbOCX");
    double VfdZZf = 982084.3564715985;
    int TvlPSTP = 1404524859;
    string RLQkAQWEiu = string("WvMxdgNThnNzZbrFJscQOi");
    double WGcDVajacaxg = 667798.8332584583;

    if (lRSGXoqjkNfeC != string("iuLpXaAXBrLgEHtenodUOuzhLhGudyMGMCjkpTzjsLacXPoLXMOBlyCHjsPeeASbvJVIVdlCfqTqnYbOCX")) {
        for (int RUJJsOtlqxN = 1040301293; RUJJsOtlqxN > 0; RUJJsOtlqxN--) {
            qFynaKcuZMhqAJhw = qFynaKcuZMhqAJhw;
            lRSGXoqjkNfeC += lRSGXoqjkNfeC;
            WGcDVajacaxg = VfdZZf;
            wQLOWX += lRSGXoqjkNfeC;
            RLQkAQWEiu += lRSGXoqjkNfeC;
        }
    }

    return WGcDVajacaxg;
}

double gFCOIpTOHZqwzT::sdYEOqLkOQg(bool ziSrfVyJOTXUeEQ, string aRXskjjYFXxu, bool SqqHF)
{
    string uVBVvWWZ = string("CvpWgVteMmXfzOqdeGtcAFqBpNVOOTTczXvFQByBkcYunHqaXfIqtHsRIcMzySnHlgcECMnqEHAXtGUppIoRAsQTnSMypDNFOVGxiPuwiOifalMTmbrStLZavPeGaPGbGzqPIlouCaCjspWvYMZDCXsGhnhzoShrfcGFrxQASTEFvHNnUQsfnwECLtuhtHw");
    string IdTtOwdRRKmbaXJ = string("wEjNBQoofKqTXCIILMEiYoGVHgThXDbtxhunelxTFtxNPOeUslzsDCOyKWXUvilyOiRJWZJaInFfEsLIOpszwNIvjltlHQZBxlxveKAUkRhNkhhFtBHymIEEZLeIlqrgCcKopSVZoGvNBudmvfTEylQmZnddBwzBqwdEaHERyFLHmRriozdwlxezxsUuNuHVzgQcxDYQL");
    bool bzqUOKRHgLf = false;
    int JnGSOgYNzzIg = -572921838;
    int HGruSxGQZdcEcVCv = -1702720341;
    int ydDmJ = 1294786701;
    bool JPBwWKHL = true;

    if (bzqUOKRHgLf == true) {
        for (int ZcyMhCvK = 1055587593; ZcyMhCvK > 0; ZcyMhCvK--) {
            aRXskjjYFXxu = uVBVvWWZ;
        }
    }

    if (bzqUOKRHgLf != true) {
        for (int GPkbxkSv = 66347939; GPkbxkSv > 0; GPkbxkSv--) {
            IdTtOwdRRKmbaXJ = aRXskjjYFXxu;
            SqqHF = ! ziSrfVyJOTXUeEQ;
        }
    }

    return -480502.3175435837;
}

bool gFCOIpTOHZqwzT::ZSoeopJHZJAz(string OBtFB, double MLkUWFIVuvOWo)
{
    string mfCmh = string("zhTAdtwKpRybBtafPvUEANHfgqoxmHphWmvcQmPRYraRVLDiZHrMGSUdGjiBrCuQDQ");
    double yMarTAAXYKGyTPOd = 1043164.1943065365;
    double ceuUYAXmVFc = 688416.19319163;
    bool iajbCS = true;
    int SwTbVetcnUuv = 1296807391;
    bool JfFIYcctRn = false;
    int XEiTCi = -140188169;
    double FRWsOFEcJwtmg = -917481.8530357948;

    for (int AjcZysDcsOnhXaiT = 516284431; AjcZysDcsOnhXaiT > 0; AjcZysDcsOnhXaiT--) {
        continue;
    }

    for (int AsLWSdqXReSRbol = 1315519222; AsLWSdqXReSRbol > 0; AsLWSdqXReSRbol--) {
        continue;
    }

    return JfFIYcctRn;
}

string gFCOIpTOHZqwzT::sVohjxyQuCU(string XorxZZLpnDuxR, bool JAfqbLZQ, string dEjdIgHeD)
{
    int LFMsoZja = -30592222;
    string CCdpZlHqPFclgR = string("hgBFvAoGrXeidbvjeclVUxjMuJqsWIfvoSuujNrckGhHkCpgVnZdrch");

    for (int oKMxJpRS = 943074355; oKMxJpRS > 0; oKMxJpRS--) {
        CCdpZlHqPFclgR += dEjdIgHeD;
        XorxZZLpnDuxR = dEjdIgHeD;
        dEjdIgHeD += XorxZZLpnDuxR;
        XorxZZLpnDuxR += XorxZZLpnDuxR;
        dEjdIgHeD = XorxZZLpnDuxR;
        LFMsoZja = LFMsoZja;
    }

    if (XorxZZLpnDuxR < string("DkdlysruhVYebHBULpikTOGIWGyLdisumMQBVoFSaDXhUysGLrlKDZzMpjNTflIMmPSSQKLnWthqudjEuUgXEYDpCddbvraibVDIKkhGkThhCAofYMUmkbaiDwYOoKWHvZrWSULcTevDUeZTiEkgCTphTBHvAmfDwOXKbhrHNrZXTTwZcANgEcXOgLTsCoQOsufBCxzKLPJrsqEQclxhstxprP")) {
        for (int DjAegAK = 1871942470; DjAegAK > 0; DjAegAK--) {
            JAfqbLZQ = JAfqbLZQ;
            CCdpZlHqPFclgR += CCdpZlHqPFclgR;
            dEjdIgHeD += CCdpZlHqPFclgR;
            CCdpZlHqPFclgR = XorxZZLpnDuxR;
            LFMsoZja -= LFMsoZja;
        }
    }

    for (int GApYFNDqBhhj = 585027137; GApYFNDqBhhj > 0; GApYFNDqBhhj--) {
        dEjdIgHeD = CCdpZlHqPFclgR;
        XorxZZLpnDuxR = dEjdIgHeD;
        XorxZZLpnDuxR += dEjdIgHeD;
        CCdpZlHqPFclgR += CCdpZlHqPFclgR;
        XorxZZLpnDuxR = CCdpZlHqPFclgR;
        XorxZZLpnDuxR += CCdpZlHqPFclgR;
    }

    return CCdpZlHqPFclgR;
}

void gFCOIpTOHZqwzT::uqppiDKX(int dIXfgTPAUDmPl, bool RglLAsCn, string KWSxVelEnjnca, string tyeZmU, string yhZJpluM)
{
    int PeJCuD = -449356535;
    int XRIbaWq = 1355972866;
    int qSzOjlXllxAqsf = 1320825321;

    for (int lPptu = 559586030; lPptu > 0; lPptu--) {
        KWSxVelEnjnca += KWSxVelEnjnca;
        PeJCuD -= qSzOjlXllxAqsf;
        dIXfgTPAUDmPl += XRIbaWq;
        XRIbaWq /= XRIbaWq;
        yhZJpluM = yhZJpluM;
    }
}

int gFCOIpTOHZqwzT::rOxSVcGVytQpjl(int kspcisrJvqEQTJ, bool OAXJMzTZdCgi, int HeReBvdsg, bool LDYeEHPfotFF)
{
    int VljTyVzclY = -422467663;

    for (int IUOkjrkchXOYECCU = 809086394; IUOkjrkchXOYECCU > 0; IUOkjrkchXOYECCU--) {
        VljTyVzclY += VljTyVzclY;
    }

    return VljTyVzclY;
}

bool gFCOIpTOHZqwzT::YWVRLKAbJzlnyO()
{
    bool yGplDnwPvUQfYGg = true;
    string ohlFobWmPXz = string("HVrZQoRKYPoOMwlSuTVUhcKJZoYrcUUFazmvNiiZodoXWZXxoASrLMsDhxZYGMXIcuPdkzpPzRAPwhHXLrnNutMOPNonshmlWyGydTCXgvDNyeQHlVNyCXbnnvKYJSHBrqiScJwFWKAshTBJNSqOaIZpwvwfsZlYuxtGXXqPrclyQLjPJjMcnMBLdfoAJnyANpLHLVVcQCkWpIH");
    string fVkqsBwYuJFx = string("zjlkvkTeKcmbsRUAfzrVGoIzNKEUvjzrOQwrQiTbSnkOCAHJCXyWpxBKrKGJLAlryYDdVIWWdxOLfeJLDUrsEthJUdYVaMLdpao");
    double OovepMRQZeJ = -728128.444909867;

    for (int xDIHpWI = 1790113211; xDIHpWI > 0; xDIHpWI--) {
        fVkqsBwYuJFx = ohlFobWmPXz;
    }

    for (int MicIp = 821147700; MicIp > 0; MicIp--) {
        ohlFobWmPXz = ohlFobWmPXz;
        ohlFobWmPXz = ohlFobWmPXz;
        yGplDnwPvUQfYGg = yGplDnwPvUQfYGg;
    }

    if (fVkqsBwYuJFx < string("HVrZQoRKYPoOMwlSuTVUhcKJZoYrcUUFazmvNiiZodoXWZXxoASrLMsDhxZYGMXIcuPdkzpPzRAPwhHXLrnNutMOPNonshmlWyGydTCXgvDNyeQHlVNyCXbnnvKYJSHBrqiScJwFWKAshTBJNSqOaIZpwvwfsZlYuxtGXXqPrclyQLjPJjMcnMBLdfoAJnyANpLHLVVcQCkWpIH")) {
        for (int UdszUUyeEODQWzJD = 959839428; UdszUUyeEODQWzJD > 0; UdszUUyeEODQWzJD--) {
            ohlFobWmPXz = ohlFobWmPXz;
            ohlFobWmPXz += fVkqsBwYuJFx;
            yGplDnwPvUQfYGg = ! yGplDnwPvUQfYGg;
        }
    }

    for (int JdERliarMKZc = 1541346322; JdERliarMKZc > 0; JdERliarMKZc--) {
        yGplDnwPvUQfYGg = ! yGplDnwPvUQfYGg;
        fVkqsBwYuJFx = fVkqsBwYuJFx;
        OovepMRQZeJ *= OovepMRQZeJ;
        ohlFobWmPXz += ohlFobWmPXz;
        ohlFobWmPXz = fVkqsBwYuJFx;
    }

    return yGplDnwPvUQfYGg;
}

double gFCOIpTOHZqwzT::GGmEWQIaGNBFkqB(int hHHqzzeYWGGhUF, double ShFfwkEpmvd, bool lcFVlx, string FxxTsLzUZ)
{
    int XpBiwnFhDGIcBfmL = 1687167698;
    bool FnwjO = true;
    int BycGVjnzCx = -1368625140;
    int ogIQcz = -876125341;
    double SMKBNgirAt = 248004.26544010674;
    string GYnPGDrPdoCWXlt = string("jlasOyBfbALRNWuHhlYbIJAuzwHfAFQSdyCHSOLXkISpiCfugMrLyBIZafwlaSEZFxrCFPtTgITFGFwpnkOCbPbMzqlTYDCkOPHQwyvJBLRgYopwXbewvtDoXadmIfQrhMrhAWBIQUEcxCXVcSvldrRwXLnqiYGFffUSLXwKZZtzwXallRw");
    int zmuyDjzXT = 1076828838;

    for (int qVHhyrh = 2055911964; qVHhyrh > 0; qVHhyrh--) {
        continue;
    }

    for (int HbWDHBbLc = 1224621352; HbWDHBbLc > 0; HbWDHBbLc--) {
        hHHqzzeYWGGhUF = BycGVjnzCx;
    }

    for (int TSKkbwnSUPm = 338166088; TSKkbwnSUPm > 0; TSKkbwnSUPm--) {
        ogIQcz -= ogIQcz;
    }

    if (zmuyDjzXT == -1368625140) {
        for (int gCvsFtLVQG = 96405767; gCvsFtLVQG > 0; gCvsFtLVQG--) {
            GYnPGDrPdoCWXlt = GYnPGDrPdoCWXlt;
        }
    }

    return SMKBNgirAt;
}

gFCOIpTOHZqwzT::gFCOIpTOHZqwzT()
{
    this->dDSqWcqdyuNYZREi();
    this->gAlEk();
    this->RLxPlPlIMhfCcB(true, -819888242);
    this->aANKYCdXx();
    this->AdZAnLc(1819529335);
    this->iZdjmRxSrYzukgK(1460085202, string("cRYZblfbnIhiqxcDlOOUTuWebXswxUtoeZLfRqMftHckYyMBpzmVnLinjkozYkjaEeeSEfjrzjLiERdYamOskpCpgflBpzThulELmYTlxzLWgmWuWCTIkmMEhdIOlThIHZjakxZFKDwcBZRqraRClgE"));
    this->ZQwjvi();
    this->GJPbY(string("kZwOVXhygeJbMFFRXBnCfHOSWlKsntJbAZRyjZzXCpEIzebXFwyafZxUGhePfjHlFkrsZrPrAuPyshSnikU"), -1032144564, 849051.7560640089);
    this->sdYEOqLkOQg(true, string("eVjXYuzVavvGknOKnhMSeYosdwgsdXWnmsZAQkCsRjWIqKsNZFEOuJWKpVgrQRcwfZUCdDOuSAOBhzKaLJeImawYIFdDQnjetxNXuVRQYeSZPeDlAGmkUv"), true);
    this->ZSoeopJHZJAz(string("vglDrfHTYwPKhhexVstAMxvctphHUVrCcfyygjPqPHqgvRVZLtmfgxJprlq"), 618578.2463742903);
    this->sVohjxyQuCU(string("DkdlysruhVYebHBULpikTOGIWGyLdisumMQBVoFSaDXhUysGLrlKDZzMpjNTflIMmPSSQKLnWthqudjEuUgXEYDpCddbvraibVDIKkhGkThhCAofYMUmkbaiDwYOoKWHvZrWSULcTevDUeZTiEkgCTphTBHvAmfDwOXKbhrHNrZXTTwZcANgEcXOgLTsCoQOsufBCxzKLPJrsqEQclxhstxprP"), true, string("VfYbCeGcbXRqAopRzinXpIQltzeawCwZhWdUgmJmiUGQSZYdiTuUKbiwMEzhczMgyCuZKHvhsVIwzipsijjE"));
    this->uqppiDKX(1584766431, true, string("VBAaUNKJoPJhnMjBEwjKGWMTZhpRnXvpDbjTbQmHhRfopcDwPoWeVSarDQHordDVWPJliKYbVLJQxLF"), string("QgmDXKPkIruVlTRvEdjXuXoDEIYAOIkcjkRixcxTLRyWdRmmyfQNwPjlyCyKGPIMXiMxFdrSMZwoeCZVzMhvzGGHGDPfOEkRGANzjIyiDstpLkmqGDOCUwhMeekiqbaRMMUsVtOkhOlsXxaleKtImRIPTvfmZQwmPJLQzMzVicXXEspkaPhzaLxvahrtFBEgQTD"), string("HlUzrLhVJVSfdLXmBkCEwnVzCqOmaJEzytpDuPWrCoTNFRAFUEnQbZuTEMsBBhrOzGehPPyFWCXUvYohkCpYbPFMguDvsrneJGAFPzuEjGwAtDSQpbWhLnglrJrDOrhhBVzFZYUYwGMfcJDrSSpCudOBIYtwCpVPEBdoAlvFl"));
    this->rOxSVcGVytQpjl(1799087901, true, 137139322, true);
    this->YWVRLKAbJzlnyO();
    this->GGmEWQIaGNBFkqB(2000495537, 285195.49426548684, true, string("IfThYpdWeZqSWlEIGlTbPxHWDuXzsfBZvLhKANWnMeDijgIaWIdYQLZkqVfnykyzpxYqarMyLVpPJpEuYqlMRlmzElSwMgbfLQVzUuhYryRqwE"));
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GQeLZlrfVC
{
public:
    double yQZbITze;
    double MMUWYeubublA;

    GQeLZlrfVC();
    bool wOSaVzxjkBlB(string oHtVyefdKJldg, string LgZbOcnfrQebudKi, string nvvOeJye);
    void euFoMsDgDGXhV(double SIevckMrlPjFlYB, double zYSBDA, string nSTlAkUnGhZ, int jRgItntOIaq, string rwfVhDwEKqLMGI);
    int PmaFkAWRHHEX(double sGrhpAHAXHoLZk, bool XJqkhbWlOgLyCmUb, bool wPTbRQfjhLIPb, double tdeGf);
    void tfpArHxv(double EgSoIcFONty, bool gaBrSLBsdtqXRO);
    int MDSYCcQPvamqzqo();
protected:
    double hWidHdfZkfRGZL;
    string SOOnNHtg;
    string EqTun;
    double MlgfUY;
    int rMqhGk;

    string QiVlASdOuUmJBd(bool KhOWoEWjcPnwm, double fthywLpKgl);
    void teQbg();
    string sEGuhvhX(string DDYGzZFCHf, double QqiqPQKdI, bool XaFUeZzrnYLnT, int KJUlOXfczF);
    void iOYASy();
    int LtzhRTAAbv(string sUYavCIVMlabT);
    void vFAhn(string BzWhOFPcMKHrxEx, bool WNTJYEwnwCBC, double IOIjpiJrQHeIgrO);
    double NLJbMEpPyAr(double ZvmJjLIqzkc);
    int WcCPvWDU(double NJFOZuYJ);
private:
    string GNHLzul;
    int ulnEbEF;

    string aPBRBvrxBjxc(int pdABlwroJfzt, bool poiJK);
    string lKLyDCL(bool gLnGDWrtkaIEToxz, bool lEurFxaSdnl, double bmepeatYzefmOcY, double WZWjoiM);
    double xIFCQtxQYzxq(bool ufwtlInHepOkateh);
    bool daVTgLgKyRPmT(string mADNkjaRX, int tqPKUsHiW, double iWgTcqzYAQNEmYTc);
    void BCNdwjsGMrC(double gVIgUliBvQCrQMoP, double DHwsKXvPA, string ekabCOZveNEnCy, int VZfxYHdv, bool PImnaMuXDZDtkyC);
    string DNEtJEeigkhycyN(bool oWJspVqd, int uAafMzdA, bool DhYAOBE);
    void qoHhXUulDBadb(double GEITIWLdmmIKfhz);
    int WfQhFQdCF(string rpkCzeiDJlytO, string kckmwoXpwu, bool DUcYzsRf, double XMjRWHILBxiNVGVh);
};

bool GQeLZlrfVC::wOSaVzxjkBlB(string oHtVyefdKJldg, string LgZbOcnfrQebudKi, string nvvOeJye)
{
    double LDnNkVQqNf = 1038900.2460269071;
    int TOJmbQ = 1909368991;

    for (int SlgSK = 2129112829; SlgSK > 0; SlgSK--) {
        LgZbOcnfrQebudKi = nvvOeJye;
    }

    for (int WfgngArVddm = 1871394436; WfgngArVddm > 0; WfgngArVddm--) {
        nvvOeJye = nvvOeJye;
    }

    return true;
}

void GQeLZlrfVC::euFoMsDgDGXhV(double SIevckMrlPjFlYB, double zYSBDA, string nSTlAkUnGhZ, int jRgItntOIaq, string rwfVhDwEKqLMGI)
{
    string kcvVBAhWsowBIiYY = string("sTmDkuTsEZyfhnEtJpYDFipgYlHKHWjvJBonuzkyLnyoGdlitTNUdzmq");
    string oFdQzhQfLo = string("jcCHIGntudhOOgIZdEvHOOAlFpRCgrCuijveCBFyfeeT");
    int qtVyTAmESjbL = -2037856264;

    if (zYSBDA < -160338.2997666268) {
        for (int MpIqWzuYPIQbH = 758354188; MpIqWzuYPIQbH > 0; MpIqWzuYPIQbH--) {
            rwfVhDwEKqLMGI += oFdQzhQfLo;
        }
    }
}

int GQeLZlrfVC::PmaFkAWRHHEX(double sGrhpAHAXHoLZk, bool XJqkhbWlOgLyCmUb, bool wPTbRQfjhLIPb, double tdeGf)
{
    double UmfacwKJKDeViL = 555819.5535026017;
    string PCTYrXCgstYvf = string("UWlMTrtjZFabYdJREsIBmntGIQzCyqZUNTeniDftSkvZujDKOpViClcOvCnjltuzYsnqCCUqgIfHlbcaahMMSRYnxxmZmUmvtVuwdklRbEcmljFxIawLGtYkiWuNDvdmjcBismyJmXRsTjShahONhsUfTvtik");
    string QXhooaZ = string("OUCkiDLWoksToIPPTx");
    string HsSJOVMeAEM = string("upQzEWyFcrDfjTSogoBdHHTdlQ");
    int jCycg = -1344642276;

    for (int drCdJhjRGuUpkgAc = 1407497725; drCdJhjRGuUpkgAc > 0; drCdJhjRGuUpkgAc--) {
        HsSJOVMeAEM += PCTYrXCgstYvf;
    }

    return jCycg;
}

void GQeLZlrfVC::tfpArHxv(double EgSoIcFONty, bool gaBrSLBsdtqXRO)
{
    double sMBqHdSqJt = -660662.0067918723;
    int eBdlr = -2123895441;
    string SIoskbU = string("lKozZtaunrvyBlQGSIxRt");
    string RIlrmlgrQdrVi = string("ljGeAUBoPRuCPeynFgWTQVgEoviRkwTpVuSWxmxPLpoTNkhIxXPoSnTlSQbivCIlfKjkOKYKQLlkNvGydnfrXjcgnvJpsHrbtzIsIRdlVhryN");
    bool TheFuGrTvmehJd = false;
    string TSLxdrHAEvnRuDT = string("EXPXlbcyniEIHIQHmwslBxPEhwbsydEroONVHFUXjZDniOfNCvQpPicjjagGjSpStnQiKmnBU");
    bool dCXeTNe = false;
    string yLPjnAGR = string("LmlCEPhKctAQAvGqSJdmlsxxPzVNrXgoaJQrEOfqnNzELMVaydNtQcMimZntdfUDT");
    double CCWbKL = -470903.0665928447;

    for (int qEqgHmUnrkqES = 1198189096; qEqgHmUnrkqES > 0; qEqgHmUnrkqES--) {
        SIoskbU = yLPjnAGR;
    }
}

int GQeLZlrfVC::MDSYCcQPvamqzqo()
{
    double CnqGXvsA = 1004702.6720364984;
    string WQrrYnYEqjVkbxY = string("IVcZWOHhiZDgAbEUlBgDEjYlBBRZHDWFHDpNGYmnAzgYHkhwwJpeeIXSIQURFeOcfZIaXFFGEparcQlEJLugkZaFqTiCegrdNCxbcCRIhlUWNSmUJPowCpLsQrtdXPBXXeMvBNRzYPKpPSiOUuWwCawvYiNJjaLxVTlDJqJTwWfQPMGiBdymNRxsNDjxyuAviStLPcuvgzlCTpdpstHpHPJVLvzynHDKPufPgR");
    double PVtvg = -127169.05176692136;
    int dOhcINoSl = 1610777363;
    int XMYTYfsCskY = -1507761857;
    double XbDNWrKICl = -995645.2764953383;
    double DkpSxzE = 268005.02239721554;
    int kclhGRvljJ = 294453996;
    double oARKXtEZNAnEzthv = 959473.9589098885;

    if (oARKXtEZNAnEzthv < 268005.02239721554) {
        for (int hxUCo = 174484393; hxUCo > 0; hxUCo--) {
            XbDNWrKICl += DkpSxzE;
            CnqGXvsA *= PVtvg;
        }
    }

    if (WQrrYnYEqjVkbxY != string("IVcZWOHhiZDgAbEUlBgDEjYlBBRZHDWFHDpNGYmnAzgYHkhwwJpeeIXSIQURFeOcfZIaXFFGEparcQlEJLugkZaFqTiCegrdNCxbcCRIhlUWNSmUJPowCpLsQrtdXPBXXeMvBNRzYPKpPSiOUuWwCawvYiNJjaLxVTlDJqJTwWfQPMGiBdymNRxsNDjxyuAviStLPcuvgzlCTpdpstHpHPJVLvzynHDKPufPgR")) {
        for (int IqJhCncD = 517339608; IqJhCncD > 0; IqJhCncD--) {
            WQrrYnYEqjVkbxY += WQrrYnYEqjVkbxY;
        }
    }

    for (int APwPDJtWwSPdsL = 772477617; APwPDJtWwSPdsL > 0; APwPDJtWwSPdsL--) {
        CnqGXvsA *= DkpSxzE;
    }

    return kclhGRvljJ;
}

string GQeLZlrfVC::QiVlASdOuUmJBd(bool KhOWoEWjcPnwm, double fthywLpKgl)
{
    string XGpTVYusthvhkwu = string("HDvxddMypiykXEqOVLUjEieLMtQeoNsYEALkIkmDKxgMHRiyPIZmNdoHxnwEuhjDorYmFaIuDDyjNMBQtsgdgZfhBfYlDRkAZuLazrIMMSxFaJvHQUoZgxNMKMUSyGhKMqRuJEhydyQghJBESdwDtwPkwTpZHrYEMHzWXFURyra");
    bool YKtXCvrjhM = true;

    for (int PYmSjMWm = 1783226419; PYmSjMWm > 0; PYmSjMWm--) {
        KhOWoEWjcPnwm = ! YKtXCvrjhM;
        XGpTVYusthvhkwu = XGpTVYusthvhkwu;
        YKtXCvrjhM = YKtXCvrjhM;
        YKtXCvrjhM = ! KhOWoEWjcPnwm;
    }

    if (YKtXCvrjhM == true) {
        for (int GkCWIdM = 124753957; GkCWIdM > 0; GkCWIdM--) {
            fthywLpKgl -= fthywLpKgl;
            KhOWoEWjcPnwm = YKtXCvrjhM;
            XGpTVYusthvhkwu = XGpTVYusthvhkwu;
        }
    }

    if (KhOWoEWjcPnwm == false) {
        for (int UWARCcW = 29469720; UWARCcW > 0; UWARCcW--) {
            XGpTVYusthvhkwu += XGpTVYusthvhkwu;
            YKtXCvrjhM = KhOWoEWjcPnwm;
        }
    }

    if (YKtXCvrjhM != false) {
        for (int HFOPbyLHKuon = 1058800143; HFOPbyLHKuon > 0; HFOPbyLHKuon--) {
            KhOWoEWjcPnwm = ! KhOWoEWjcPnwm;
            XGpTVYusthvhkwu = XGpTVYusthvhkwu;
        }
    }

    for (int rZJmDBjDiiuNvXG = 927433990; rZJmDBjDiiuNvXG > 0; rZJmDBjDiiuNvXG--) {
        continue;
    }

    return XGpTVYusthvhkwu;
}

void GQeLZlrfVC::teQbg()
{
    bool QfuhZLYfo = false;
    double MSXQOrlKVxzeVgp = 506944.07207217906;
    int MZine = -535045358;
    string plliMCi = string("HGTbGHJrYZKpKponHtczzaOldyOzLfgdgSKHcmgqpox");
    int zvYIADuFUCAqg = -7928200;
    bool FtARVGZQXVQ = true;
    double bMjLeEG = 954006.8722193927;

    if (MSXQOrlKVxzeVgp != 954006.8722193927) {
        for (int ZnYTIMbHtQzAEQs = 1419218748; ZnYTIMbHtQzAEQs > 0; ZnYTIMbHtQzAEQs--) {
            MSXQOrlKVxzeVgp /= bMjLeEG;
            MZine += zvYIADuFUCAqg;
            QfuhZLYfo = QfuhZLYfo;
            FtARVGZQXVQ = ! FtARVGZQXVQ;
        }
    }

    for (int NpkPlE = 747282088; NpkPlE > 0; NpkPlE--) {
        MZine += zvYIADuFUCAqg;
        FtARVGZQXVQ = FtARVGZQXVQ;
    }

    for (int VOdebwRCYe = 2045775737; VOdebwRCYe > 0; VOdebwRCYe--) {
        zvYIADuFUCAqg += zvYIADuFUCAqg;
        bMjLeEG /= bMjLeEG;
    }

    for (int KTBLsFV = 1088866696; KTBLsFV > 0; KTBLsFV--) {
        continue;
    }
}

string GQeLZlrfVC::sEGuhvhX(string DDYGzZFCHf, double QqiqPQKdI, bool XaFUeZzrnYLnT, int KJUlOXfczF)
{
    int JZGXRooj = 468174086;
    string UeFXVZ = string("BcVfqFDAHxtzGHkyceuzWUgbhwpPYsSzThpnAkKuxlwewDvcsyoEkOhAtZyhGETKQySOAwajsBnQofSYXsXRvNPEevbRGqpKcCmHfYUjZmWS");
    string TJdJWb = string("VLJmTTGcNBDrDIoGdFvtxaMSTABWTPIrezsDFuELDNeCtGBNE");
    bool XyEGdGaac = false;
    double eWnSeDPaOtji = -984092.4992301788;
    string uNLzUYHSet = string("PXeQuilspGEFQIkIULWdgsIZDpAqKCCbhtGbfnafJqTDqhKFHioECatymyHlTafMKwSOUjMZaYJvGNvXPTOYxTKnyxPFkdVMrsGwvGTdEOpOquJqDYyYFlOKWZYEKjTrwwncGUoyTPztiPwSRBsLDdMXkTCzBdxfkcYGLUAqlxHSHgwBBkEiSkmgNdRkVklQKzTdyzbxPmRjjDUWXHGEebBFwOgVaaaehEtTgKGmRI");
    string CVhvXtplOWW = string("BeaIgTyexrVhXOpFFcAQbacNhxJgmlMRDEfxVoEXzzAlKdzGavbkVFSHkcyLAkXCquymdqmJeygVXoFmEDWvFKyhJmBjwwTOANetPAVsQXQidyzIPyMfwKRvxCXUHboPFDfjjbGZVlVqOMmEWAdDe");

    for (int AWOogdhwfKv = 441178809; AWOogdhwfKv > 0; AWOogdhwfKv--) {
        uNLzUYHSet += UeFXVZ;
    }

    for (int cuWtFh = 1921722770; cuWtFh > 0; cuWtFh--) {
        CVhvXtplOWW += TJdJWb;
    }

    for (int UCNYUhc = 1856968715; UCNYUhc > 0; UCNYUhc--) {
        continue;
    }

    for (int ywAXDogINtTAXPdn = 1559648984; ywAXDogINtTAXPdn > 0; ywAXDogINtTAXPdn--) {
        DDYGzZFCHf += CVhvXtplOWW;
        XaFUeZzrnYLnT = ! XaFUeZzrnYLnT;
        XyEGdGaac = ! XaFUeZzrnYLnT;
        DDYGzZFCHf += UeFXVZ;
    }

    for (int RTGIRFzxgki = 311757825; RTGIRFzxgki > 0; RTGIRFzxgki--) {
        DDYGzZFCHf += CVhvXtplOWW;
        uNLzUYHSet = TJdJWb;
    }

    for (int XlivJEC = 534000775; XlivJEC > 0; XlivJEC--) {
        TJdJWb += CVhvXtplOWW;
        CVhvXtplOWW = DDYGzZFCHf;
    }

    return CVhvXtplOWW;
}

void GQeLZlrfVC::iOYASy()
{
    double mnUFkAAHptklu = -926384.0667606863;
    bool tpSLDuSKY = false;
    int QkgNAZAkU = 558166313;
    string NPAOScPzMHw = string("M");
    bool DecApQt = true;
    string BEriajKEfOtI = string("RWcgirRwRVQvxTudEQLlJD");
    double SwilEOwb = -55329.22968740688;
    string JcfuREYb = string("EBUkbehdefmjcRnoUaNrrVVKYplJuecFTwoRaTqMKlsAxDnpFCLoqOqzZykSdsQROJxstlmDyVSXrvIBDHpghJhRdKGX");
    bool KpNgE = false;

    for (int KaWWAUIgH = 1192871374; KaWWAUIgH > 0; KaWWAUIgH--) {
        continue;
    }
}

int GQeLZlrfVC::LtzhRTAAbv(string sUYavCIVMlabT)
{
    string LJktw = string("QrYJNLUMHSeIpoYlBsRKBFKdSQOiKDLgweGXzREMGEIMTeKJDBWRBvYwTArWIlXSpfIpHagUUinqWoWPXxzUcBEgjkEDMPiVLKBDraPStCQYOrBYeWRvzAVNWVfgYINcfmwBNPjLNAffeqlodgxBJKwDCMcviAxzIw");
    int eSEfKCdSWEk = -1893809372;
    int xUCKzhfeQNogqh = -681188085;
    bool XHECrQ = false;
    double zpLZqPiBJiieR = 387717.81172813533;

    for (int UlUNZuBGgHbBod = 1319973831; UlUNZuBGgHbBod > 0; UlUNZuBGgHbBod--) {
        continue;
    }

    if (XHECrQ != false) {
        for (int zhxnsXuOj = 1072184241; zhxnsXuOj > 0; zhxnsXuOj--) {
            LJktw += sUYavCIVMlabT;
            zpLZqPiBJiieR -= zpLZqPiBJiieR;
            sUYavCIVMlabT = sUYavCIVMlabT;
        }
    }

    for (int ALiow = 976063772; ALiow > 0; ALiow--) {
        eSEfKCdSWEk -= eSEfKCdSWEk;
        eSEfKCdSWEk /= xUCKzhfeQNogqh;
    }

    for (int qCgbNCj = 2088163869; qCgbNCj > 0; qCgbNCj--) {
        eSEfKCdSWEk = xUCKzhfeQNogqh;
    }

    return xUCKzhfeQNogqh;
}

void GQeLZlrfVC::vFAhn(string BzWhOFPcMKHrxEx, bool WNTJYEwnwCBC, double IOIjpiJrQHeIgrO)
{
    string uBBlkyeybxxmNXy = string("MWJVZmv");
    bool vKEmYNVvg = true;
    double AvwzDpjJOuyTdT = -878795.6763449522;
    double VhgPjpdlBCb = 846231.9123022713;
    int BdKMIMFx = -1933512899;
    string GBtnnQmzux = string("cNPayMsiRrtabXOJuzadENfUsGdsqdwtZxqGQzqUCooTDNtRbgWbMgDYCaYOeeteuKUTLdiOeAOPNMnbuvvaFoeVzLfHWKLrduaeBIrDeIAtEkRlWifhgkxJwAWRfwAVmobJlxmpTVREZCXbfxiwhGfzmbEynBcECHOCyOrGNOxQ");
    bool dVJhcAlXnO = false;
    double DhpcTDzEld = -679941.3921543373;
    bool xSaarQVaOXYDxO = true;
    int HqzvH = -640299455;

    for (int ABPWKl = 1682292602; ABPWKl > 0; ABPWKl--) {
        AvwzDpjJOuyTdT = IOIjpiJrQHeIgrO;
    }

    if (DhpcTDzEld > 846231.9123022713) {
        for (int JuNtXLDQmgxudlLg = 392450216; JuNtXLDQmgxudlLg > 0; JuNtXLDQmgxudlLg--) {
            AvwzDpjJOuyTdT += VhgPjpdlBCb;
            IOIjpiJrQHeIgrO = IOIjpiJrQHeIgrO;
        }
    }

    if (WNTJYEwnwCBC == true) {
        for (int QFTlKrX = 1296763660; QFTlKrX > 0; QFTlKrX--) {
            xSaarQVaOXYDxO = ! xSaarQVaOXYDxO;
            DhpcTDzEld -= AvwzDpjJOuyTdT;
            VhgPjpdlBCb -= IOIjpiJrQHeIgrO;
        }
    }
}

double GQeLZlrfVC::NLJbMEpPyAr(double ZvmJjLIqzkc)
{
    string qUvVDGEItBS = string("hKPAvWDExxjIisXIQYdZylHGskjhGEYTnpMLLonEmMAXEpXmEElUSOnYfUdRXFYKDePJOQolqiOOqYwvhvuHhGKwOpqnATqNvIQECMdzYMEVtvjWAYIrdGfoBAcZHXzlqNZCLhtSbTyZUkBtcrxQKUkjTXdfopTJFmPKyggjIgVshkbTGCSRUszwqn");
    int dcSIbu = 478798086;
    string MuBgCHHLxvDAs = string("sutRaDrAEzBsnFcRPlYQMhcBBsJPbOfWYWXMFCuVGTBHemBBUqcJBKCptmtJCwEnqjRopmbGRYySCISNCXnGqTAyifjqnIZypklKdXWmPBbhohqPTKidtnBZFPazWGlFSDKPKIqRGbPNVPyZNswgyrZokNKqBpnflqBYS");
    int lYvtPDL = 120262827;

    if (ZvmJjLIqzkc <= -14678.894495599321) {
        for (int gElYcTHaGsmWvD = 1971702432; gElYcTHaGsmWvD > 0; gElYcTHaGsmWvD--) {
            lYvtPDL += lYvtPDL;
        }
    }

    for (int LsHbsokM = 925142716; LsHbsokM > 0; LsHbsokM--) {
        continue;
    }

    for (int iQQvmZGMN = 352148899; iQQvmZGMN > 0; iQQvmZGMN--) {
        dcSIbu += lYvtPDL;
        MuBgCHHLxvDAs = qUvVDGEItBS;
        lYvtPDL += dcSIbu;
        MuBgCHHLxvDAs += qUvVDGEItBS;
        ZvmJjLIqzkc = ZvmJjLIqzkc;
        lYvtPDL *= lYvtPDL;
    }

    return ZvmJjLIqzkc;
}

int GQeLZlrfVC::WcCPvWDU(double NJFOZuYJ)
{
    bool duBulYGlMy = false;
    int oNAPDeDM = 1522035780;
    bool UVUxRSQWRi = false;
    bool aMOppViK = false;
    bool zVOAhprGheFSrbY = false;
    bool XlayKhdHOGkDxkE = true;
    bool tGNsJgqlFyu = false;
    string fwwMjOeW = string("piSOwxBdLBBOGnpsFOhgHgBGOzFmrLQsXfhdQZGeVSOOBGITPUPOqRwhhebvyCVjghzukUTjDXHJtzMn");
    double CsstbyuK = -601124.4395190158;
    double nAQBSfWFm = -1004621.4008999612;

    if (CsstbyuK > -1004621.4008999612) {
        for (int xBzTVf = 97179551; xBzTVf > 0; xBzTVf--) {
            UVUxRSQWRi = ! UVUxRSQWRi;
            UVUxRSQWRi = ! duBulYGlMy;
        }
    }

    for (int gSODUbQmmYJs = 951270980; gSODUbQmmYJs > 0; gSODUbQmmYJs--) {
        oNAPDeDM /= oNAPDeDM;
        aMOppViK = zVOAhprGheFSrbY;
    }

    return oNAPDeDM;
}

string GQeLZlrfVC::aPBRBvrxBjxc(int pdABlwroJfzt, bool poiJK)
{
    int BtiaVIdFpqncE = -1721567693;
    bool XXTscobNlmEr = true;
    double nCCSHCpLWQ = 388656.3651193465;
    double mabTjzRmicmd = -127580.19761672016;
    int fNTpRXAzRyEnJOX = -694782706;
    bool BUEIEx = false;
    string CovBSQHHZiDEQPiY = string("evRwkCLEwXLjNfVkPDkGMujROFSBSftRSXBKImgIsobGGIKvrJaAyHGXLDjNRVbhSBDDvoCBexHkQZWCjhrnFNyvRLRIeZCVQESSIV");
    bool crAnscUmg = false;
    double tjiHbtoXnDNqLpS = -749664.3760187346;

    for (int qbOjUk = 2135679209; qbOjUk > 0; qbOjUk--) {
        BUEIEx = ! BUEIEx;
    }

    if (poiJK == false) {
        for (int TkKtD = 844447123; TkKtD > 0; TkKtD--) {
            XXTscobNlmEr = BUEIEx;
        }
    }

    for (int xMoWFCoKUYRVELnm = 965571135; xMoWFCoKUYRVELnm > 0; xMoWFCoKUYRVELnm--) {
        tjiHbtoXnDNqLpS *= mabTjzRmicmd;
        XXTscobNlmEr = ! XXTscobNlmEr;
    }

    if (mabTjzRmicmd < -749664.3760187346) {
        for (int jQzDt = 2013764960; jQzDt > 0; jQzDt--) {
            crAnscUmg = BUEIEx;
            nCCSHCpLWQ /= nCCSHCpLWQ;
        }
    }

    for (int IDIxnSdOWkXtcdCC = 1537258962; IDIxnSdOWkXtcdCC > 0; IDIxnSdOWkXtcdCC--) {
        mabTjzRmicmd -= tjiHbtoXnDNqLpS;
        mabTjzRmicmd += nCCSHCpLWQ;
    }

    for (int uVqRzmDfJiJN = 239867063; uVqRzmDfJiJN > 0; uVqRzmDfJiJN--) {
        BUEIEx = poiJK;
        fNTpRXAzRyEnJOX += BtiaVIdFpqncE;
    }

    return CovBSQHHZiDEQPiY;
}

string GQeLZlrfVC::lKLyDCL(bool gLnGDWrtkaIEToxz, bool lEurFxaSdnl, double bmepeatYzefmOcY, double WZWjoiM)
{
    double NQGUGLducjGBk = 495305.29430320713;

    return string("ZCeDWZctVEiIkcJLZCuYxMgOqZyufZQhBotXUzDacDqqzkgxUglPWRfGvvWVRCRMJuhooFUUZuTGfgdqHeRfcPhVsbiOLlXdUlLKsODnWneiFSMigZcVJrYQxrwnDskzSGGlhuLbI");
}

double GQeLZlrfVC::xIFCQtxQYzxq(bool ufwtlInHepOkateh)
{
    int kbsDbQHdiSBlCU = 156720864;
    string tILtWbfmrMfijo = string("krBthHddswXMRWXIlzne");
    double tvvOlpCUOTl = -1044930.2385513308;
    string bwZKOqqbBZMP = string("MigciGHldlwwubfIbVWMTRLzjWOhInnpgzAVNjxETfEyonKfubHOMEsOfPKGOVJCxtZYlsmKjLsTjWSEusFQCDkdNXnUvvgJYFcIEkOnmONvnRDBhWzlzLIqShevlbyKUuwzxeppYLJkyoulNWWLLOZmTDdyxSDnLdLpREVnjCzxdlL");

    if (tILtWbfmrMfijo < string("MigciGHldlwwubfIbVWMTRLzjWOhInnpgzAVNjxETfEyonKfubHOMEsOfPKGOVJCxtZYlsmKjLsTjWSEusFQCDkdNXnUvvgJYFcIEkOnmONvnRDBhWzlzLIqShevlbyKUuwzxeppYLJkyoulNWWLLOZmTDdyxSDnLdLpREVnjCzxdlL")) {
        for (int KtasaxFubcaBZTnC = 2125299784; KtasaxFubcaBZTnC > 0; KtasaxFubcaBZTnC--) {
            continue;
        }
    }

    if (tvvOlpCUOTl >= -1044930.2385513308) {
        for (int bQRKRu = 1284898299; bQRKRu > 0; bQRKRu--) {
            kbsDbQHdiSBlCU += kbsDbQHdiSBlCU;
            tILtWbfmrMfijo += tILtWbfmrMfijo;
            tILtWbfmrMfijo += bwZKOqqbBZMP;
            tvvOlpCUOTl *= tvvOlpCUOTl;
            tvvOlpCUOTl *= tvvOlpCUOTl;
        }
    }

    return tvvOlpCUOTl;
}

bool GQeLZlrfVC::daVTgLgKyRPmT(string mADNkjaRX, int tqPKUsHiW, double iWgTcqzYAQNEmYTc)
{
    double kRnbrgOQe = -731126.0598209924;
    string TemfeYxByWMb = string("SjOstGARYOZEVJmGCuxDAjbyCxbCxKWpQNBQmlwyxQydYeHFoCKrBJCxdVkqkFnancToQtbPTwyiugMfHHwAePCIStzyhBGKKThRkfseeqebNskSVWZsuauXFBasLDbhCzGRyejiDmORNJZwMgTopQqLfooExnIhiaThsRZLQaaavzOoicnBrPRCZbQLlEspNrPLVbZuVMU");
    string YTYJNGgdYc = string("TlCYDXnaAcWtDIrZFvsMXJtfDRLUgsrinMimJIJEAjjBtnNxyeBQTiwsNxrmBpqfKoogsKLXnSQtUknLMLRggDyI");
    int GFqHvNdCszMss = -1861391142;
    int XWznwKLkzLLU = 560231933;
    int lDrjIobiKYgcS = 1669865430;
    string JdUzVEb = string("miOxEOEZbfGkwUguHIerQdAJbzjcIOmIQd");

    if (JdUzVEb < string("miOxEOEZbfGkwUguHIerQdAJbzjcIOmIQd")) {
        for (int nDKOcQNO = 1792360945; nDKOcQNO > 0; nDKOcQNO--) {
            lDrjIobiKYgcS -= lDrjIobiKYgcS;
            mADNkjaRX += YTYJNGgdYc;
        }
    }

    for (int NeYgonFVQhLqEdBn = 644225348; NeYgonFVQhLqEdBn > 0; NeYgonFVQhLqEdBn--) {
        YTYJNGgdYc += TemfeYxByWMb;
        TemfeYxByWMb += mADNkjaRX;
        XWznwKLkzLLU += XWznwKLkzLLU;
        lDrjIobiKYgcS += tqPKUsHiW;
        YTYJNGgdYc += mADNkjaRX;
        XWznwKLkzLLU *= tqPKUsHiW;
        iWgTcqzYAQNEmYTc = kRnbrgOQe;
    }

    for (int cCbBvklPhi = 1323885942; cCbBvklPhi > 0; cCbBvklPhi--) {
        YTYJNGgdYc += TemfeYxByWMb;
        iWgTcqzYAQNEmYTc /= iWgTcqzYAQNEmYTc;
    }

    return true;
}

void GQeLZlrfVC::BCNdwjsGMrC(double gVIgUliBvQCrQMoP, double DHwsKXvPA, string ekabCOZveNEnCy, int VZfxYHdv, bool PImnaMuXDZDtkyC)
{
    bool FHUahCnjkb = true;
    int vHEWK = -1586493844;
    bool vvKxNOh = true;
    bool BvhylQZcYiJhDANv = true;
    int VszFBBFMREGfBh = -2064298990;
    bool qbwtJXjrsvrYKcn = true;
    int jOcstw = -1102707221;
    double lVUnjxYTtZF = 489819.1500346515;
    bool QnYzwxcZADRu = true;
    double EZxtLzNFtPfwqE = 1028771.407136386;
}

string GQeLZlrfVC::DNEtJEeigkhycyN(bool oWJspVqd, int uAafMzdA, bool DhYAOBE)
{
    bool uTyZiYXn = false;
    string cMWIrsrXgdLx = string("MFYCFZEIZVvACfYRdLqRyzMwPlpMjvMbUelDePdzBvvBBrgDeToErIVfJDtuqhzSafbtkCuA");
    double yFtUjexflFnZWgEg = 326670.664928993;
    bool KrsJe = false;

    if (uTyZiYXn == false) {
        for (int HsjVanyDvLrZEK = 1694293147; HsjVanyDvLrZEK > 0; HsjVanyDvLrZEK--) {
            continue;
        }
    }

    if (DhYAOBE != false) {
        for (int GYPUxccjrpZsiPo = 997189326; GYPUxccjrpZsiPo > 0; GYPUxccjrpZsiPo--) {
            uTyZiYXn = ! uTyZiYXn;
            oWJspVqd = ! uTyZiYXn;
            uTyZiYXn = DhYAOBE;
        }
    }

    for (int sLiLi = 1653398517; sLiLi > 0; sLiLi--) {
        KrsJe = uTyZiYXn;
        DhYAOBE = ! oWJspVqd;
        yFtUjexflFnZWgEg += yFtUjexflFnZWgEg;
    }

    if (uTyZiYXn == false) {
        for (int RekNUzIgAYU = 994684785; RekNUzIgAYU > 0; RekNUzIgAYU--) {
            oWJspVqd = ! KrsJe;
            DhYAOBE = KrsJe;
        }
    }

    if (uTyZiYXn != false) {
        for (int NnpXaizEldBna = 2146380273; NnpXaizEldBna > 0; NnpXaizEldBna--) {
            oWJspVqd = KrsJe;
            KrsJe = ! oWJspVqd;
            DhYAOBE = KrsJe;
            yFtUjexflFnZWgEg += yFtUjexflFnZWgEg;
            KrsJe = ! oWJspVqd;
        }
    }

    return cMWIrsrXgdLx;
}

void GQeLZlrfVC::qoHhXUulDBadb(double GEITIWLdmmIKfhz)
{
    double epopvV = -765051.0319563883;
    bool cXYqO = false;

    for (int zfRnMxRAGa = 96546326; zfRnMxRAGa > 0; zfRnMxRAGa--) {
        cXYqO = ! cXYqO;
        epopvV += GEITIWLdmmIKfhz;
        cXYqO = ! cXYqO;
        cXYqO = ! cXYqO;
        cXYqO = ! cXYqO;
    }

    for (int oyCFkO = 493651362; oyCFkO > 0; oyCFkO--) {
        cXYqO = cXYqO;
        GEITIWLdmmIKfhz = GEITIWLdmmIKfhz;
        epopvV += GEITIWLdmmIKfhz;
        GEITIWLdmmIKfhz = GEITIWLdmmIKfhz;
        epopvV = epopvV;
        epopvV -= GEITIWLdmmIKfhz;
    }

    for (int nRRtLjMd = 428689407; nRRtLjMd > 0; nRRtLjMd--) {
        GEITIWLdmmIKfhz *= GEITIWLdmmIKfhz;
        epopvV *= epopvV;
        epopvV = epopvV;
    }

    for (int KKhAVlTipB = 17392014; KKhAVlTipB > 0; KKhAVlTipB--) {
        GEITIWLdmmIKfhz /= GEITIWLdmmIKfhz;
        GEITIWLdmmIKfhz += epopvV;
        GEITIWLdmmIKfhz += epopvV;
    }
}

int GQeLZlrfVC::WfQhFQdCF(string rpkCzeiDJlytO, string kckmwoXpwu, bool DUcYzsRf, double XMjRWHILBxiNVGVh)
{
    bool iHtrPRN = false;
    int vXqmqAZZox = 282306226;
    string HdlIMXqdWyJIpQ = string("AwkfhPzNfjJLgffNFGeSONXFlamKWoMBDfvUpSUVPtHdMZJlbHWwoEwZyQggTlJaZxHdsEUOOcBPbxJSDkvMDBGJUyebQoytxpfcHjBVzxacetynOryceUuHFvkhpkpgSXFmSPoKbrcpzvYEXIP");

    if (iHtrPRN != true) {
        for (int oqVQU = 1056609032; oqVQU > 0; oqVQU--) {
            vXqmqAZZox += vXqmqAZZox;
            iHtrPRN = ! DUcYzsRf;
            kckmwoXpwu += rpkCzeiDJlytO;
        }
    }

    return vXqmqAZZox;
}

GQeLZlrfVC::GQeLZlrfVC()
{
    this->wOSaVzxjkBlB(string("IabIJTEXzSktBrOJbFFDqUoLDAVUidnbtrgsHLSYgYhJQSquErtiUXqBBhSDEuclCAXVpWqkxEJcoTfctKVNDlihupmLoUnTdMpnJCBXfpfafpLOCPQbxuPaJqvuKDwHprHirsyaqTMLpdvBWGaXykmeSkBvpIjLVjOPbNBHJGLMvEEMfjTBrrxLjHhLSsJcoffX"), string("IhsjMNpuyFpHkLpvVUyvFncUxNpeevNlTNAeniNseCLeHPwGSfRVOGbkmaOoWTQXhWZNQSQEpUOnQRmNQfEECOBIcdGGPptAEjDvvwFNTiPzUrwYJKbBeKRfqGQHdigVixJxkTowtyYkynqDWbmeSoQMEKWxNmXuBOIuyaGKhOVDvUnFBQeEheuTezjySApQABKnQtdsPMlFWxDIHgfSsQhIrfJceYvFtAXjFRUIqTtZ"), string("fYlIhUqQuYNxmfpKbePvzSVvQIhsqCnrsWqGYwpkdVfgAeuaBKAWFFUrGBxdsTjGoDGHvmxGXEkCbJOlUZICmkOjOHWSrdTbsuqBfqkCRaVexnFpcyMKRFcsZRVIEJuzRLWHFXEHPdpaMEFUvXabQJryYn"));
    this->euFoMsDgDGXhV(-160338.2997666268, -39200.33131253133, string("NlSXpWbZUmnVFFapORdRLpevZUeusRwBItlTCfyifiECUEmRysJTvoryslrxuGYLuYyEtzwwQLgmFIhkSzMGPiKdnkBeFyRLOntHGIAYRvQQQYpVMPtkYvpJAYjoVmUeZeiNvZzjNlCGeFwaJwHyVfERqYWwQwj"), -1541381437, string("rQepDcUazyBRRuSDfvZUkksxXyvJBdbharYeghNjFtssIHbFKahYHDvnOGHUisrmVDtpOPGgySFSifMJbASUgZaFJDGj"));
    this->PmaFkAWRHHEX(260839.17241436857, true, true, -627972.2861590078);
    this->tfpArHxv(-216608.36240586388, true);
    this->MDSYCcQPvamqzqo();
    this->QiVlASdOuUmJBd(false, 644538.4497297915);
    this->teQbg();
    this->sEGuhvhX(string("jDLfmhvcsKNuWutHVSZGauLylJSoOftwyOIKFHSYpBgAMwHzzSTldLXtSxerCwvEJimjIUqpVnibgNwWVNDGoNqGRuiKuCMSpJdfToskUFUneORavVbnbwRvqVXcvBBaOkRTwFbuXOtSCSMrpqofHaPQLBpJPndUdcUaePylXOTqieVAVQFsTAcTXhtNGQPEysBjbjdNTvcHZchdzONEeYUjIebOxwjMujXTI"), 560276.085097802, true, -1437711213);
    this->iOYASy();
    this->LtzhRTAAbv(string("RsJqWFsTKVfBNhHctECqsRVCvKMGMfzkZMXZNQXtLaImJKimqysBjVYwyzEbtCNMsEBorswVtApBkhnAReJQpRyuFBTjMahAScZOVXiicKpErhufQOrcgYXxbPLXSWXcehWLowKCbZLSduIdaPgsEWmBYtWzvlfDDFAvBYXjiAcJbJfegDyAj"));
    this->vFAhn(string("giLVeJrQRHahtJmlDhFoEFkBUtTxilEzgsvKyEbLAfaCFVVjWjVZirJrYdQxwIPBqDoMXWsPcembACWMhiAslulNQvsWoeWBpgVqprVjxbkiPVVVYgvPKCZFzqfhbGxetNDZnzzVNWWtyWnstsssrbWCZSqTqRZLAkDtEcIqPNbWJlFkvnznozzIHIGGyyDDllVztCOWavbZoxrfkKARaOxaNpKgIIqfDILSyaXTA"), false, -12864.473234314775);
    this->NLJbMEpPyAr(-14678.894495599321);
    this->WcCPvWDU(-365159.17857386725);
    this->aPBRBvrxBjxc(-1155295830, true);
    this->lKLyDCL(false, true, -643264.9833421364, 855401.8599783905);
    this->xIFCQtxQYzxq(false);
    this->daVTgLgKyRPmT(string("MzwAGDVvPUyyyxcoleLmPsiMYFngrdZyGaWxZCetUgNGqgMjOvYXvqCedhYzDKudjPVOOnSidNFOHRMpgJGwyVmRqeBheDIJgaTMloswnppnJhrZVDjvbjVfOYCByvbtuoTIakaQODNWZzAwHKhARkcNuGZbokTcBTxvpwMDNnCfpsVDSsztujwISsgmiZwSzhhtPnnDeEsHAAtyWsOpMIdSJbrEQkrVsMZrGIiyTJelOSHWEsDJ"), -165623262, 558068.7073301153);
    this->BCNdwjsGMrC(-645121.7807887399, 221872.86342137514, string("uFpHfarwurRNAyot"), 1172288256, false);
    this->DNEtJEeigkhycyN(false, 1688241402, false);
    this->qoHhXUulDBadb(-640538.5437343784);
    this->WfQhFQdCF(string("gCMLhHbgWccoLEqduyQDOrSvAQSsrdkdSKDyFzTZfIYtbNqOWtfPwIpuqnaYgDxcXERneQEjRUeqAXjDzDBFnSexJJJcKGTzGxYdJbpkdrOTqLfdokiaJnxMpTAmGunZzbzpCmnWAsYuXsQxhlKnYUdHdbzAFxrlovNtLAjptVTdPrGh"), string("otqFvPbsUtjPEaoMYHZItHndSpHyoNDIhtgidHictGRqJJZesZaVvMReiRukKpHOzqRQqoknFVKZVPBaxqzqupBfqzGEzsGfMPTKxXLprUwoZZqDadkLPobLGnfrTnwcczRWUjAKzqrWqlqDyBiQeFTqextY"), true, 262863.11715751915);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NaVLFpr
{
public:
    string HbGBHk;
    double aEgGeVypfs;
    int JyAQZlizpVHb;
    int URuJrvrOEzeq;
    int GIoZVe;

    NaVLFpr();
    string QYDVXwaqxW(bool rBLoxqpnkyt, double gJYxvshc, bool RQfpQoBlZQcic);
    string kOWJMHusgy(double IQvFfFKWYwK, string SeSIgK);
    double yjelhEQuMvwEgBYD(double oHHkcNuTdyrboiRn, bool uJGqTkWvIII, int PfTTaEQXzEBTJdCK);
protected:
    double JVAiZOlmkQnG;
    int trRomMJcmMiqApch;
    int QVHpfoUebo;
    double WnKKNMCwrOG;

    bool DCXucoE(bool xtkqCtOHATQ, double XbmpPWGZxDiVd, bool DrBIJspcPMNutcGf, double lPJsRLnOS, string agydduSC);
    bool RIjIqeVjrqxSRh();
private:
    double UwQUmAZggCF;
    bool tWMNKmbXXHRuFT;

    bool FVbIuwn(double aIDRhRaExuHNNj, string RYydGhreLgjnnC);
    int lmtYWHooRFHKl(bool KOPYwFJeygRQlE, string FZCDhlcNNw, bool eyJPypFUmK, bool xWANFbyHrgXEtr, string uYRJSAUTUGBr);
};

string NaVLFpr::QYDVXwaqxW(bool rBLoxqpnkyt, double gJYxvshc, bool RQfpQoBlZQcic)
{
    double CiItetnsiPA = -669235.771867373;
    bool IkciP = true;
    bool rNFZWsFwC = true;
    string pqHMqYNBQAnk = string("kQqxgVQiEhNyvMjdKmEZJLkUwrokapewprbIBaDnkGdAGxFafUKTTbcwQyILz");
    double RhLrQesHzqbLKf = -43189.32521554781;

    return pqHMqYNBQAnk;
}

string NaVLFpr::kOWJMHusgy(double IQvFfFKWYwK, string SeSIgK)
{
    double OKPjjCCLdjqS = 695515.13719155;
    bool jhkgLbc = false;
    double hjonQlRHIMxOKYxs = 614191.5683698539;
    int GsJHd = -237648521;
    double PCzPsF = 100588.49484405457;
    double wnrquDguW = 571545.0155300505;

    for (int etRnPyYnUsotDwsb = 1606286381; etRnPyYnUsotDwsb > 0; etRnPyYnUsotDwsb--) {
        PCzPsF -= PCzPsF;
        IQvFfFKWYwK -= OKPjjCCLdjqS;
        wnrquDguW = OKPjjCCLdjqS;
        hjonQlRHIMxOKYxs *= IQvFfFKWYwK;
    }

    for (int SbSgTkPggCX = 923626196; SbSgTkPggCX > 0; SbSgTkPggCX--) {
        IQvFfFKWYwK -= OKPjjCCLdjqS;
    }

    return SeSIgK;
}

double NaVLFpr::yjelhEQuMvwEgBYD(double oHHkcNuTdyrboiRn, bool uJGqTkWvIII, int PfTTaEQXzEBTJdCK)
{
    double FQTkhMlXxKe = 492128.0367012558;
    int rJcJvBoCahtUkPI = -827387937;
    int nJYzEHWLrnzc = -1114326813;
    bool yHQkGA = true;
    int NnhKgWBLKppxlq = -235172861;
    bool HSDyKnsixguqpl = false;
    int XozMWsnXddkErL = 1411002621;
    int zkZofTSFxv = 1513505763;
    bool ITusqPR = true;

    if (ITusqPR == true) {
        for (int yNClxmdNDrBuoNf = 1199575104; yNClxmdNDrBuoNf > 0; yNClxmdNDrBuoNf--) {
            PfTTaEQXzEBTJdCK *= nJYzEHWLrnzc;
            HSDyKnsixguqpl = uJGqTkWvIII;
        }
    }

    for (int GWrcqehbvUgMBlG = 277514745; GWrcqehbvUgMBlG > 0; GWrcqehbvUgMBlG--) {
        PfTTaEQXzEBTJdCK -= rJcJvBoCahtUkPI;
    }

    return FQTkhMlXxKe;
}

bool NaVLFpr::DCXucoE(bool xtkqCtOHATQ, double XbmpPWGZxDiVd, bool DrBIJspcPMNutcGf, double lPJsRLnOS, string agydduSC)
{
    bool HHvqPosEXyVpk = true;
    bool JUIwlWCtZl = true;
    string ocXyO = string("wWygyQdAiaIrzSdXVQSHzgcDQoQIltauBPOKTKxEmzBIMAeyjXfObWsGUuKfDenVQCvdmbgHOdFbhYXDEIEnoRcoXUsGubpuccfrbeLokFsNjMohxACyhvgsVsXwijKWHPPjTTMxGllWMMiJgDnppzgGKJeRRrxQthvpNSteEaLqxrpnfRknTCwYixwzinAjZjCFHOHXxx");
    int wzLBiqtRgHgtFXG = 827969281;
    string wcuAtwKcjAOOp = string("qvJToGrCKOrqGzmfHgEEbophaWOGEKoElwRhJJBlnMyWbSwwNVqEhowWXHWXGFCVbFJLkprDBaOmNXnnQYURtiGRXiVVrnbgXgVbqrenNgXzxKSlaMCihQWkkJCGeZtKGj");
    int cxZhPODZTXDNX = -1870371544;
    int qGHzi = -2060580368;
    double BFwBDkTLpqKLlIGC = -300329.86299210537;

    if (JUIwlWCtZl != true) {
        for (int TFsfJZnRSUJFVYB = 1653380934; TFsfJZnRSUJFVYB > 0; TFsfJZnRSUJFVYB--) {
            lPJsRLnOS += lPJsRLnOS;
            wcuAtwKcjAOOp += wcuAtwKcjAOOp;
            agydduSC = agydduSC;
        }
    }

    return JUIwlWCtZl;
}

bool NaVLFpr::RIjIqeVjrqxSRh()
{
    string HvIvaKGZwjlAM = string("sADFqMMLGLsUIBQqwaQHLcQEsLRkvTnfyMwfDIfXJrGaCzdaVivwXcTBEmLDBJcgxAswzaZbEBiMtRJwcsooNxhcIJYTeYvKWWTjBlSssIGCOjsVzfrRMsGhoQbScqBXaWfTVNZMCQCVBaWMTIPIMf");

    if (HvIvaKGZwjlAM < string("sADFqMMLGLsUIBQqwaQHLcQEsLRkvTnfyMwfDIfXJrGaCzdaVivwXcTBEmLDBJcgxAswzaZbEBiMtRJwcsooNxhcIJYTeYvKWWTjBlSssIGCOjsVzfrRMsGhoQbScqBXaWfTVNZMCQCVBaWMTIPIMf")) {
        for (int XINNgUsnGhBMP = 1955065687; XINNgUsnGhBMP > 0; XINNgUsnGhBMP--) {
            HvIvaKGZwjlAM = HvIvaKGZwjlAM;
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
        }
    }

    if (HvIvaKGZwjlAM == string("sADFqMMLGLsUIBQqwaQHLcQEsLRkvTnfyMwfDIfXJrGaCzdaVivwXcTBEmLDBJcgxAswzaZbEBiMtRJwcsooNxhcIJYTeYvKWWTjBlSssIGCOjsVzfrRMsGhoQbScqBXaWfTVNZMCQCVBaWMTIPIMf")) {
        for (int iNMpdEeovZm = 247369657; iNMpdEeovZm > 0; iNMpdEeovZm--) {
            HvIvaKGZwjlAM = HvIvaKGZwjlAM;
        }
    }

    if (HvIvaKGZwjlAM < string("sADFqMMLGLsUIBQqwaQHLcQEsLRkvTnfyMwfDIfXJrGaCzdaVivwXcTBEmLDBJcgxAswzaZbEBiMtRJwcsooNxhcIJYTeYvKWWTjBlSssIGCOjsVzfrRMsGhoQbScqBXaWfTVNZMCQCVBaWMTIPIMf")) {
        for (int NJXhzCPcUdtwIMtJ = 2002643688; NJXhzCPcUdtwIMtJ > 0; NJXhzCPcUdtwIMtJ--) {
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
        }
    }

    if (HvIvaKGZwjlAM == string("sADFqMMLGLsUIBQqwaQHLcQEsLRkvTnfyMwfDIfXJrGaCzdaVivwXcTBEmLDBJcgxAswzaZbEBiMtRJwcsooNxhcIJYTeYvKWWTjBlSssIGCOjsVzfrRMsGhoQbScqBXaWfTVNZMCQCVBaWMTIPIMf")) {
        for (int knAPWzKVRx = 838995589; knAPWzKVRx > 0; knAPWzKVRx--) {
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
            HvIvaKGZwjlAM = HvIvaKGZwjlAM;
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
        }
    }

    if (HvIvaKGZwjlAM < string("sADFqMMLGLsUIBQqwaQHLcQEsLRkvTnfyMwfDIfXJrGaCzdaVivwXcTBEmLDBJcgxAswzaZbEBiMtRJwcsooNxhcIJYTeYvKWWTjBlSssIGCOjsVzfrRMsGhoQbScqBXaWfTVNZMCQCVBaWMTIPIMf")) {
        for (int BwCaMAzLr = 329219071; BwCaMAzLr > 0; BwCaMAzLr--) {
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
            HvIvaKGZwjlAM += HvIvaKGZwjlAM;
            HvIvaKGZwjlAM = HvIvaKGZwjlAM;
        }
    }

    return false;
}

bool NaVLFpr::FVbIuwn(double aIDRhRaExuHNNj, string RYydGhreLgjnnC)
{
    int IqcjUy = 139040134;
    int tccRrG = 2018393535;
    int FTznzgLo = 978785933;
    string dUDmWYeNcINMk = string("mCWqqaZpjKdNKscBBxXHMDjjSTXhuPfslYOWQtdEmwUxJwEoDhkIinppUoUTbkCHSZwbRBsxbGLFRiBHnMufJxwapeVwXleYWhJTPSsBMeQiGeNKgurydzOVtJvnZWymgrxETeAxqVglYRFAbefshQmTPzPWBlofXgJpWaATMqRVZQSyikDAnxQSmOnikRxoyToGvgouewWvzLNcaTfVZUWUYjzSbUJpyarwTELJPjTpmfuPSmIyAVMKeIANX");
    double zEDzTBz = -940197.4843544293;
    bool CdlAjQpasPEww = false;
    string NIWEDDRYP = string("U");
    bool zCVsz = false;
    string XDfSFFPWnjqOLK = string("kHGRMPPkqVXCaTLaDQAKZvNEDEAGZkKe");
    int hpxiz = -1247612712;

    for (int WlBhaFkm = 24510618; WlBhaFkm > 0; WlBhaFkm--) {
        NIWEDDRYP = XDfSFFPWnjqOLK;
    }

    for (int CEGwUVO = 1731894146; CEGwUVO > 0; CEGwUVO--) {
        NIWEDDRYP += XDfSFFPWnjqOLK;
    }

    if (zEDzTBz < -215457.2949343266) {
        for (int ekSktiGH = 431489068; ekSktiGH > 0; ekSktiGH--) {
            dUDmWYeNcINMk = dUDmWYeNcINMk;
            tccRrG *= FTznzgLo;
        }
    }

    if (zCVsz != false) {
        for (int gXDLrAdpniqwI = 1643996267; gXDLrAdpniqwI > 0; gXDLrAdpniqwI--) {
            hpxiz += hpxiz;
        }
    }

    if (zCVsz == false) {
        for (int bFDbYHCFysMpH = 2051984494; bFDbYHCFysMpH > 0; bFDbYHCFysMpH--) {
            hpxiz /= tccRrG;
        }
    }

    for (int ONwtJYCCPHDjLuBB = 1600619104; ONwtJYCCPHDjLuBB > 0; ONwtJYCCPHDjLuBB--) {
        RYydGhreLgjnnC = NIWEDDRYP;
        dUDmWYeNcINMk = RYydGhreLgjnnC;
    }

    for (int uIrnJHdGdxrAuXpP = 974257429; uIrnJHdGdxrAuXpP > 0; uIrnJHdGdxrAuXpP--) {
        continue;
    }

    return zCVsz;
}

int NaVLFpr::lmtYWHooRFHKl(bool KOPYwFJeygRQlE, string FZCDhlcNNw, bool eyJPypFUmK, bool xWANFbyHrgXEtr, string uYRJSAUTUGBr)
{
    double MqtRUZnoEztVaHm = -997311.4944352513;
    bool WJRHSWEDJUPfQCi = false;

    for (int ZWlaJtd = 744110342; ZWlaJtd > 0; ZWlaJtd--) {
        KOPYwFJeygRQlE = ! xWANFbyHrgXEtr;
    }

    return 1027865796;
}

NaVLFpr::NaVLFpr()
{
    this->QYDVXwaqxW(false, 97353.39142904748, false);
    this->kOWJMHusgy(-896078.261205074, string("PwXYMZnWRhRGgOqimNTgkzgbWnUUWvzSjhsqzRbcoTRBPwMTDLJXDIKQxXjFkLPd"));
    this->yjelhEQuMvwEgBYD(-239563.99801851826, false, 633926186);
    this->DCXucoE(false, 462169.25730340113, true, -880716.1666698141, string("XuqbiqHkSAVzhTkfiNUSVHAANvMUEcUUIkwaDxNtbdkbpXdmSuhWHGftkJjunqvJCQMGbLrLFRssUU"));
    this->RIjIqeVjrqxSRh();
    this->FVbIuwn(-215457.2949343266, string("mgIIzBbLBSFgUUrBXKMuhmKUThzBpopQygBnDdjBlbdUKynjTGGeRsxozNzcFSnUMxKpEeZRWqfVUhXWminKHFJAuluFAr"));
    this->lmtYWHooRFHKl(true, string("kqktpv"), false, true, string("fRYcImFveYWXwMoXZqWNqneebCYBcbGzDPcbEgQxpAAeRLPCSDEeHsEkGwGEUNlEOEiplhgOGhCPZCxvdxolgpzQkwokqPMUCPPuRVAuAbhMfTKbSMGfaqjbBWTkxqjajburgwtIbkxxHfnyfXRRWOwIgktqtXHUHJmGIguJMhbO"));
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vrnXftFge
{
public:
    bool gKJhAD;
    string hEEKjnQ;
    double KdABoGQdjDp;
    double nEYzOijxGgobN;
    int BdYVmmqKjVQtyI;
    double TDgfpkY;

    vrnXftFge();
protected:
    double OCeaHkNAHnkz;

private:
    string XkTgzDh;
    string cEYckGtkIXKBexM;
    string AnXOGdBmFoFLszM;
    int pJxVfLpNlevmO;
    double GIHiuQET;

    double GJoJEjXd(bool LHVnUwDfu, bool YtRXLPiDbszRS, double XQCVBCCXqvMd);
    double WTgQlAkMPUw(int DqZiMXkLckOj, string HtCXgqUTE, double qRtNMyrAANCT);
    bool pIDQGwIvCMUMG(string Vhaajv, bool uXFkaLGkj, string TdjtPvSlumPqp);
    void bIvRfuFLx(double ISBLEiUZJVbbDjd);
    string vsMecVqk();
    void ZvguVizXlsQtg(double RDoWKljbAeIij, string IDNLtFv, bool vbwcyLaxQnsrr, double OTVPdvncC);
    void MIVBptApAkdBY(string kigZIElNmxaMVcma, bool eyHZrKOvTwHuvL, string dsMOPysB, string XqLUtrwKz);
    int JTTVCeP(double yGryYXohSYUwsE, bool tpKjxVrwFOOmD, double tTARElaHFRxIAjCT);
};

double vrnXftFge::GJoJEjXd(bool LHVnUwDfu, bool YtRXLPiDbszRS, double XQCVBCCXqvMd)
{
    int ZhtyCLxAmgWIDJWb = 1143884602;
    bool HClaIejr = true;
    bool grRkoJKrq = true;
    double Seqmcalh = -916111.9802328143;
    string KTwrz = string("yJjDEHEzJRvrkmRfSaaqxkvjAhMfzzjBfXkogWOXTDmlyrfGjmSUmAsFyfklMbvLfOPOVxAhyLqvNAZOWWCHjwiDmVRqhAgWaLVJEXwNzKqfDvfxSbFrhVemHwNpRKoEIwWxNlRhDHbcbgjVGKovefbxqFbsjJtdebWbljHoaJInntbgRJyXHtnDwgyoXIqKeKdZsePLPGsTChCwgOMExbRcOImUwOunsrvtWNuKDAiOlGSoefKPFGDsoxvPDk");
    double YKlxvFPy = 349897.8856270683;

    if (ZhtyCLxAmgWIDJWb >= 1143884602) {
        for (int jEQTtDUMiSSRlpQ = 480078193; jEQTtDUMiSSRlpQ > 0; jEQTtDUMiSSRlpQ--) {
            grRkoJKrq = ! HClaIejr;
        }
    }

    for (int lpyMlGbYwU = 1074113619; lpyMlGbYwU > 0; lpyMlGbYwU--) {
        LHVnUwDfu = LHVnUwDfu;
        YKlxvFPy = XQCVBCCXqvMd;
        YKlxvFPy += XQCVBCCXqvMd;
    }

    for (int yvlwOzcDcZJVyb = 1592214637; yvlwOzcDcZJVyb > 0; yvlwOzcDcZJVyb--) {
        continue;
    }

    return YKlxvFPy;
}

double vrnXftFge::WTgQlAkMPUw(int DqZiMXkLckOj, string HtCXgqUTE, double qRtNMyrAANCT)
{
    string yuvxYC = string("FjdLjqFxUzUJlFwSgonodiAOVBYYQwoMsULmfNfKnO");
    int vnVlCPaElo = 2122898736;
    double WOLkVSYYF = 424208.17524808616;
    int jHxnhowBrWWKDhT = -1341559352;
    bool DIDjVC = true;
    double pvzErCSaTgcJ = 501247.4281461826;
    bool wdNKYCLdRBS = false;
    bool lOMBhiDYcJDyq = true;

    return pvzErCSaTgcJ;
}

bool vrnXftFge::pIDQGwIvCMUMG(string Vhaajv, bool uXFkaLGkj, string TdjtPvSlumPqp)
{
    bool uUIPcITy = true;
    int wAGtLyOitDhyzvc = -1925076393;
    int cOqfKKcWxvksEiVk = -1581279063;
    double xLOcmCIOSmQdBS = 1026848.898912756;
    bool utRvoyNbB = false;
    double fXCJDycU = -797420.5689730464;
    string RnbhfRgJauhm = string("FfpjTuZIrrdbMPILhDAAqWWSXHXNZawnwmpljWwPTccNwGhpemCXzeOKqgpnfmlzFzCUDjVVpXZcPbEnAZwxZOIybrVeZTsPKEyenRxgytOzeEyueQXGhdGRGBhRiTcddQROujvEyqnnxfgtmlIlbzpQxbGIehLMhYKwfcghQOlDbfmHpcaSNhpiPLCQOUlssTmIBWVKXNVEkCXGAjKLHjIsEScKkmt");
    bool FnQYPtQyD = true;
    string qphwFJPSAcRMn = string("zLtDsMUAyPiyeADLMnqGpVTyhyUFXTjSWApDqJmNcDoETgpgOPoTeFDWlMREGAtnUyWLXXlQqkNGqUUqBG");
    bool YZTiK = false;

    if (uXFkaLGkj == true) {
        for (int bCyKpfmOpZ = 75084503; bCyKpfmOpZ > 0; bCyKpfmOpZ--) {
            FnQYPtQyD = FnQYPtQyD;
            xLOcmCIOSmQdBS += xLOcmCIOSmQdBS;
            FnQYPtQyD = uXFkaLGkj;
        }
    }

    for (int eLsYwROjWQo = 1703249110; eLsYwROjWQo > 0; eLsYwROjWQo--) {
        wAGtLyOitDhyzvc -= wAGtLyOitDhyzvc;
        uXFkaLGkj = uXFkaLGkj;
    }

    for (int mkiFoKA = 1997327598; mkiFoKA > 0; mkiFoKA--) {
        continue;
    }

    return YZTiK;
}

void vrnXftFge::bIvRfuFLx(double ISBLEiUZJVbbDjd)
{
    bool VNctFokPEtOhCb = true;

    for (int CovhWHcYfhoaDnB = 1352958274; CovhWHcYfhoaDnB > 0; CovhWHcYfhoaDnB--) {
        VNctFokPEtOhCb = ! VNctFokPEtOhCb;
        VNctFokPEtOhCb = VNctFokPEtOhCb;
        ISBLEiUZJVbbDjd += ISBLEiUZJVbbDjd;
    }
}

string vrnXftFge::vsMecVqk()
{
    bool AiIRtpgeGznVT = true;
    bool LaXWUrnE = false;
    double XvmFih = -802179.7833062728;
    string kKIQqNNcgMiH = string("NnFoMiyaanIRJdlxYYCLPULGfaUTEJCKNwNbuUluUBxdVInBvNvvdyxoKQpiUovHgJPyxuXCEKNPKwikRXWzwuyvzQAmXrFGeZiRXMaLgTGmFAxTXIJzdYQtCMerTBrSUwSPtlIOuUBgyGRfGoJttBEisBrFsLSgZHTPYQSFgcOLXctqGjqxN");
    int TSTKCPJnFkrvnAOw = -1997536799;
    double FoEBK = 237484.9459824998;
    bool IHCCHb = false;
    double ZFDpZnkucg = -786028.2711697007;
    string VeveVHJP = string("SnKAwfUJHAvffLcDAgWKVnAJWYofiERAxgPFBarPIdEziZzoLGoWRmfEScahPLbhXDxNUDPHWdNnefInWmPDteflXzry");

    if (FoEBK < -786028.2711697007) {
        for (int AYaRCE = 1566500530; AYaRCE > 0; AYaRCE--) {
            IHCCHb = IHCCHb;
            VeveVHJP += kKIQqNNcgMiH;
            AiIRtpgeGznVT = ! LaXWUrnE;
            FoEBK *= ZFDpZnkucg;
        }
    }

    return VeveVHJP;
}

void vrnXftFge::ZvguVizXlsQtg(double RDoWKljbAeIij, string IDNLtFv, bool vbwcyLaxQnsrr, double OTVPdvncC)
{
    string pVOBtACkILfAjQj = string("fjbxprdtAsvjpk");
    string RVeRs = string("bLnwINccjSeMvqLngdbaDHEPWPVBaiDrfKftqSwaKWwufESxYZqSCqGrelxEcsFynZDQdutqrfBnYUvtCbshHpFYXeroFplVnRzMldiObQqIDRCpmNUpsiTvNnoMlSnqiwRKIcXGnqEqKeszXxdFcQEuqrIFJisfFgQNpgbAODLkQMDNzHkcyGLWEEJlR");
    bool XDDnz = false;
    string aZBZjcXgnuhsllq = string("zprGAVCLHSkZbKoWEcudNyyLEuvbJRxHOciwIdVvBEuTaaNTqarqhlUqAcLqFaklDYDfhpoHUDNANHleZcDE");
    int jJnBUpw = -150363316;
    bool orDMPswbDXnn = false;
    double CwZILyNCmEn = -249689.18629722766;
    double nCZmsNWZdOKXWqJE = 738029.8322742576;
    bool mCKDFIydaUcIqMs = true;
    int gXtRxsYl = -503741364;

    if (OTVPdvncC < 336924.25291546475) {
        for (int EMViIGrpRixYRuOv = 89096850; EMViIGrpRixYRuOv > 0; EMViIGrpRixYRuOv--) {
            continue;
        }
    }

    for (int BKkDfhtcGPlYxP = 2019321469; BKkDfhtcGPlYxP > 0; BKkDfhtcGPlYxP--) {
        continue;
    }

    for (int GMUXpyhXiBMRYMnj = 23069007; GMUXpyhXiBMRYMnj > 0; GMUXpyhXiBMRYMnj--) {
        vbwcyLaxQnsrr = ! vbwcyLaxQnsrr;
        CwZILyNCmEn *= RDoWKljbAeIij;
        XDDnz = ! XDDnz;
    }
}

void vrnXftFge::MIVBptApAkdBY(string kigZIElNmxaMVcma, bool eyHZrKOvTwHuvL, string dsMOPysB, string XqLUtrwKz)
{
    double sREpvgGo = -136147.75641736845;
    string wIsQegtleEY = string("vOoVxggUopoMpJLCdWwZetckcWIoYKkGYvqEaEKypscPlwDBZGVMtzhvYNIzKkTZNtxqYPNHmjNWlbK");
    double jyhnPbnkayuvCxl = 707642.4411479471;
    int ElUTsh = -1681469657;
    bool FuTijDKybL = false;
    int HxFJzrMSvieB = 881283159;
    string MKMrBcpXAG = string("PWCovOHHlCHUqboyWqzQaZtOGTwckgElHPCikyKrRfBibn");
    bool BTyDM = false;
}

int vrnXftFge::JTTVCeP(double yGryYXohSYUwsE, bool tpKjxVrwFOOmD, double tTARElaHFRxIAjCT)
{
    int pWLDFkTGep = 1277329296;
    int XJprUlKaLyctnpX = 1775869137;
    double ppklei = -834274.0180833497;
    double SLyjdQwKIH = -163472.18951855332;
    double hWOclgPfe = 465353.3611637595;
    double JhAWOLfYZDDJC = -451969.0209818381;
    int rzGUeFZLnjzg = -305513630;
    int wDLgLfB = -163213175;
    bool TUYayXcHs = false;

    if (yGryYXohSYUwsE >= 791830.5715165675) {
        for (int wlwdbqg = 996401609; wlwdbqg > 0; wlwdbqg--) {
            yGryYXohSYUwsE *= ppklei;
            wDLgLfB /= pWLDFkTGep;
            tpKjxVrwFOOmD = tpKjxVrwFOOmD;
        }
    }

    for (int DVCPFte = 1402309429; DVCPFte > 0; DVCPFte--) {
        ppklei = tTARElaHFRxIAjCT;
    }

    if (JhAWOLfYZDDJC >= 791830.5715165675) {
        for (int wfyliMrtmuOYrtNN = 867253575; wfyliMrtmuOYrtNN > 0; wfyliMrtmuOYrtNN--) {
            JhAWOLfYZDDJC /= SLyjdQwKIH;
        }
    }

    return wDLgLfB;
}

vrnXftFge::vrnXftFge()
{
    this->GJoJEjXd(false, true, -264992.10805748607);
    this->WTgQlAkMPUw(-1420871359, string("mSFOhjEPzPMiaDJTOIhdsztVnQahEAsPntviKRtXsBJhdbsiwBfDXKjSqRvJRpmJuZjsIRxkaRtOAesGBTNtBhZgIHgVbnsQTLqIsQRCUXiJEYtMGUCzFMFLZLGInAMZDrvUEIgyzpjJjcivxRPTXZPDHPRdxysOjlKNEsCoJzjEeYKDHTXLAlgQrEvWIEoimVxWzdXB"), -660500.9771384367);
    this->pIDQGwIvCMUMG(string("yfGHCBwMbXQxllCWXzntDbMHfooRdCbDHyYoAWNPpbsUIOuuGUJfwDBQWHtwKtqJcgCPjqKeVRyNZniegkxyrhnXMFSPmpcBhXTESRRphQQXAQCGlwmKXPPsBWwLIRmUIiWjUeRMpxEkybmKkEOrIEJuRfAtaXsmWRYbPJiBPwLFCg"), false, string("NyQibOGwDbxQTmCuOlKhePCNzMTbAgOiyPgSCKIoHOxdtcfglasgBhpovDpKhGlgN"));
    this->bIvRfuFLx(-922133.6386015639);
    this->vsMecVqk();
    this->ZvguVizXlsQtg(336924.25291546475, string("FetaTMpfHAjqcbjMmvdASzjeKCFUZwCHmWqCKzoDlleGQCAKHYtssOVjWHrZVGMAtofKlqjsMBzB"), true, -895671.5117739275);
    this->MIVBptApAkdBY(string("SvdjWxchGQUegNcHmlQzEyTVbdQxBQePytVTupAUJhoJRoIuMHSPqlRmYlrqJDpazsuhfwsoZJxwjNPigbmlqVEjErBVJULNVeYHynqzqcwXypJCCQuYlKtTjYcZcpsiUIZtDWroeZoxUcyenRLLWktuPYSUromBNsQOsUGRjzMSWsJRyVwpEskoNViDodvaclhKxfWmXybvNTxHraTXuPwnXvpRMFaLdWhjgWJMssMuFEDvYwyDY"), false, string("rOwOMqkfMswjSXnyHhcOeXHAbdonYmrYhmdYFdpdadlUTyVkJyzzBGnCSmRDPUhXjvyYsrbsKpODDEyrMXrmHIRYpNGZwPJCCDH"), string("dylBatwyNbkMwkqMmYDXxEcdCihZXXIGrKoVqDVPNDluMwTttBxJUsePIIAFhBiCjMzZJVOlvEFaeugFEGvbXRqGgskJsjPqPFfSuZGAwceSpDcIZhDHlLVIKKBAeBrNlHhUoHWbYNydtdzLaFUzNaCqs"));
    this->JTTVCeP(-578078.5870676372, false, 791830.5715165675);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class sclPDSSd
{
public:
    string jhuKtvfJfZ;
    string whUymuQ;
    double UOycfhArcol;
    string jjMqztoY;

    sclPDSSd();
protected:
    string FMOrebICZTU;
    bool zOcSfstLNxdwoyKv;
    bool LCobyiPgXuqPdnq;
    bool fzGxCkendjamSeE;

    double GPXmXqa(bool ogYWcAPpuNrtVR);
    void eYgFlfkbWzwKGz(double JNkDwAZbGSJdXrol, double lHvrjxk);
    int nYkJEgcvjopcFygK();
    bool ELDrRNgtnw(bool kTuAoiomvGTnkb, double xqBbrqluZgnAKNN, double bNkvtG);
private:
    string wTFGOsMNPlxQBnlk;
    string XHnqvAznQCtYzsG;
    int krJVPREtpyLHp;
    bool ialJw;
    string jQMIBPjgUqJyZepX;
    double tJjZkOSC;

    string bClLJeiPwS(bool ZbzfKa, double bShkIQeO, int hwceAmIUIjsXtE, string yMkiIEXUOkcFp, int ZwLHOsGpemrT);
    double dNTzl();
    bool ryUBNGVvl(bool CoLiXMaqhR, double dmLXGcIbjgGnlwfk, int POsOcmpCXzRG);
    double LqdEFIbaULMWxTB();
    int jbvfEwIzQqIjA(double hGHxfm);
    void XNQgyNViBcC(string hvHJlErg, string EojQTfaLJQ, int sHManGBJVuKwvLqw);
    string hfLcRDuvCvBL(bool FhKGOayE, double fniGgQKhJtbfGDbD, int skBeduJVGuy, bool BqyojDpVnB);
};

double sclPDSSd::GPXmXqa(bool ogYWcAPpuNrtVR)
{
    int viXEKUjwPxdt = 861073229;
    double QPQRMirKIJxyvklM = -62782.66876189908;
    int kjLhzjzxoONGMku = -1931125858;
    double pzsLzIzk = -580654.289239783;
    string XTIyEWH = string("fGSSvwDFWRBWPOPquBKPfgbEZNUVzSUNsisBtQqzF");
    double pNCLYMg = 350590.1517540859;
    string sSvHKCjtsfeWu = string("aNxxYePijCKyuOJieoSVLDnzPPlPkijcOBoiCjIMGmamUUJxwkuHMpOiwMgRVDAICbRtuiVKLYXpMycVFYTvSpTZjqLiHtYQAFBtmLdQiPjovMCZKBHQgFbWXL");
    bool KqjhcxuQr = false;
    bool YZOlIblPbkJhTA = true;

    return pNCLYMg;
}

void sclPDSSd::eYgFlfkbWzwKGz(double JNkDwAZbGSJdXrol, double lHvrjxk)
{
    int BWcYwsByrvWdT = 6775057;
    string lxGaBHBNMJNdvQ = string("rkqPOxLQVNxlxtoTonsOTFvKpXUCSccCvkFXoDdNCRjaXQVaMAsVSZbuihpwJCLVFVulrUSKdxwEFznCUHueQKfQJKnWaIoThZiLcwOYiJnOeGjHNSZnUKTmMGFNLuNsSVeXwEUTfYDBRTTnqbuGvoupDBMmdjmlycftrOeRPfoYgnbYipoftNyPqFPLYNMaXbwbsLDgvpUrqNlbyK");
    double VVFwy = 222145.73024320847;
    string QupDAPv = string("tzLKpOcGVVLClEj");
    int nAdXihBU = -2105778840;
    bool LWnfMEICeebihm = true;

    if (lHvrjxk < 222145.73024320847) {
        for (int AXTDoEiM = 1376974802; AXTDoEiM > 0; AXTDoEiM--) {
            continue;
        }
    }

    for (int mYQFkXAPzQOGTFxC = 518091034; mYQFkXAPzQOGTFxC > 0; mYQFkXAPzQOGTFxC--) {
        JNkDwAZbGSJdXrol /= JNkDwAZbGSJdXrol;
        QupDAPv += lxGaBHBNMJNdvQ;
        QupDAPv += lxGaBHBNMJNdvQ;
    }

    for (int zzfqbQs = 1260302211; zzfqbQs > 0; zzfqbQs--) {
        continue;
    }

    for (int HZOoNyexs = 1979493281; HZOoNyexs > 0; HZOoNyexs--) {
        LWnfMEICeebihm = ! LWnfMEICeebihm;
    }
}

int sclPDSSd::nYkJEgcvjopcFygK()
{
    string DeezhuAK = string("PrjxTvYOIdW");
    double YgGtAD = -804197.2423066196;
    int dNrRHaPprTJ = -498628625;

    for (int lkSFWHWSGLaZCK = 1130195838; lkSFWHWSGLaZCK > 0; lkSFWHWSGLaZCK--) {
        YgGtAD += YgGtAD;
    }

    return dNrRHaPprTJ;
}

bool sclPDSSd::ELDrRNgtnw(bool kTuAoiomvGTnkb, double xqBbrqluZgnAKNN, double bNkvtG)
{
    double UfNdIBjdWKMJ = -347118.4188748895;

    for (int GvONXw = 538501049; GvONXw > 0; GvONXw--) {
        bNkvtG -= bNkvtG;
        UfNdIBjdWKMJ -= UfNdIBjdWKMJ;
        UfNdIBjdWKMJ *= UfNdIBjdWKMJ;
        UfNdIBjdWKMJ -= UfNdIBjdWKMJ;
        kTuAoiomvGTnkb = ! kTuAoiomvGTnkb;
    }

    for (int GBntwKsOBvwgyrb = 1381016685; GBntwKsOBvwgyrb > 0; GBntwKsOBvwgyrb--) {
        kTuAoiomvGTnkb = kTuAoiomvGTnkb;
        UfNdIBjdWKMJ /= xqBbrqluZgnAKNN;
        UfNdIBjdWKMJ /= bNkvtG;
    }

    return kTuAoiomvGTnkb;
}

string sclPDSSd::bClLJeiPwS(bool ZbzfKa, double bShkIQeO, int hwceAmIUIjsXtE, string yMkiIEXUOkcFp, int ZwLHOsGpemrT)
{
    bool ZQXphVCUzTUEPu = true;
    string bmHIJfw = string("QwaetaaEJHDAjgoyllLxoIVxTdIgWYHdExzkCVfbYxBWrNCKKedCxbNzqwEmTABLaBNiEQzrsrzhcqpRIQnrEmJRxoqBdZXasQFaEPiXfAdFGDvRYCvIGXxHFNsNWQYTNsYcwEEarBvxupvjkEWSOrWGpthEHUQWjnmvidzydWeccrMaJjSCzesEeMxcIkUwXMZfSbVdywWWqouasdCTeddbrHUlGnJVNluSJKxxTkFucNdumnesjm");
    bool iHNDXMIjjwPRVlL = false;
    double ZSLFTPgThTIg = 254833.66716370688;
    int PyvNFHBfv = 775976072;
    bool svERcgdSE = false;

    if (ZwLHOsGpemrT <= 454207791) {
        for (int ypfjtxPgjPuNYq = 1629921620; ypfjtxPgjPuNYq > 0; ypfjtxPgjPuNYq--) {
            bShkIQeO += bShkIQeO;
        }
    }

    for (int WwLsNue = 1413341174; WwLsNue > 0; WwLsNue--) {
        continue;
    }

    return bmHIJfw;
}

double sclPDSSd::dNTzl()
{
    double BehakP = 405804.6031523146;

    if (BehakP == 405804.6031523146) {
        for (int WdTckdeTbZ = 1722311636; WdTckdeTbZ > 0; WdTckdeTbZ--) {
            BehakP -= BehakP;
            BehakP = BehakP;
            BehakP /= BehakP;
            BehakP -= BehakP;
        }
    }

    if (BehakP >= 405804.6031523146) {
        for (int zyAYNakXCyNPk = 1515105409; zyAYNakXCyNPk > 0; zyAYNakXCyNPk--) {
            BehakP = BehakP;
            BehakP /= BehakP;
            BehakP += BehakP;
            BehakP /= BehakP;
            BehakP += BehakP;
        }
    }

    if (BehakP <= 405804.6031523146) {
        for (int lFCNVTzy = 333944379; lFCNVTzy > 0; lFCNVTzy--) {
            BehakP *= BehakP;
        }
    }

    return BehakP;
}

bool sclPDSSd::ryUBNGVvl(bool CoLiXMaqhR, double dmLXGcIbjgGnlwfk, int POsOcmpCXzRG)
{
    string GGSUixAlkZV = string("oEMFhZtGTdWgmmJEBRHURhtFQXXVeUvlHXG");
    int FKUQEAJ = 1541286981;
    int ubvPjrPgdk = -925348841;
    double VgIhjwWfrnJcwn = 689318.4052138758;
    bool cgsiHBK = false;

    if (VgIhjwWfrnJcwn != 990250.4415559926) {
        for (int EKWTVjghskdve = 1992371311; EKWTVjghskdve > 0; EKWTVjghskdve--) {
            dmLXGcIbjgGnlwfk /= VgIhjwWfrnJcwn;
            ubvPjrPgdk -= FKUQEAJ;
            POsOcmpCXzRG += FKUQEAJ;
        }
    }

    for (int wWhqEjFvXoTRpE = 1464320561; wWhqEjFvXoTRpE > 0; wWhqEjFvXoTRpE--) {
        continue;
    }

    if (cgsiHBK != true) {
        for (int yXmGZWMsO = 798262489; yXmGZWMsO > 0; yXmGZWMsO--) {
            dmLXGcIbjgGnlwfk -= VgIhjwWfrnJcwn;
        }
    }

    for (int MVXLtQ = 1784355365; MVXLtQ > 0; MVXLtQ--) {
        continue;
    }

    if (cgsiHBK != true) {
        for (int aaYJRArETXrEl = 323815528; aaYJRArETXrEl > 0; aaYJRArETXrEl--) {
            dmLXGcIbjgGnlwfk += VgIhjwWfrnJcwn;
            POsOcmpCXzRG /= ubvPjrPgdk;
        }
    }

    return cgsiHBK;
}

double sclPDSSd::LqdEFIbaULMWxTB()
{
    string MFjrWjjZCzVLqq = string("YfiPjqABlVuDxOhPvwQfeILUKGUHEDpziwzrvjNNVbWHTKXQIyEtpQWL");
    int wMygDHQisJSUpCI = 1096229611;

    if (wMygDHQisJSUpCI >= 1096229611) {
        for (int JXBSwNiNHlLwfA = 389339627; JXBSwNiNHlLwfA > 0; JXBSwNiNHlLwfA--) {
            wMygDHQisJSUpCI -= wMygDHQisJSUpCI;
            MFjrWjjZCzVLqq = MFjrWjjZCzVLqq;
        }
    }

    if (wMygDHQisJSUpCI < 1096229611) {
        for (int hZuwQ = 840593677; hZuwQ > 0; hZuwQ--) {
            MFjrWjjZCzVLqq = MFjrWjjZCzVLqq;
            MFjrWjjZCzVLqq = MFjrWjjZCzVLqq;
            wMygDHQisJSUpCI *= wMygDHQisJSUpCI;
            wMygDHQisJSUpCI = wMygDHQisJSUpCI;
        }
    }

    return 1022073.3994256688;
}

int sclPDSSd::jbvfEwIzQqIjA(double hGHxfm)
{
    string VrHTVEvD = string("IdEwUuyOAwICgaIaaVHsgwsVhSGsfixyDONsQOfBMcjEpJpEnKVQaIcpQvByIuBGMWonANfXvhLXcqIYcDyNHOXxJhaZlRBKUSbucftxtQDraVwellMqxEvdTWWFF");

    for (int YWcBHmzIKK = 1685234104; YWcBHmzIKK > 0; YWcBHmzIKK--) {
        hGHxfm -= hGHxfm;
        VrHTVEvD += VrHTVEvD;
        VrHTVEvD = VrHTVEvD;
    }

    if (VrHTVEvD > string("IdEwUuyOAwICgaIaaVHsgwsVhSGsfixyDONsQOfBMcjEpJpEnKVQaIcpQvByIuBGMWonANfXvhLXcqIYcDyNHOXxJhaZlRBKUSbucftxtQDraVwellMqxEvdTWWFF")) {
        for (int IfEIz = 1461321664; IfEIz > 0; IfEIz--) {
            VrHTVEvD += VrHTVEvD;
            hGHxfm += hGHxfm;
            VrHTVEvD = VrHTVEvD;
            hGHxfm /= hGHxfm;
        }
    }

    return -1216130909;
}

void sclPDSSd::XNQgyNViBcC(string hvHJlErg, string EojQTfaLJQ, int sHManGBJVuKwvLqw)
{
    double pYgnspkLa = 487491.5647596019;

    if (EojQTfaLJQ >= string("RbcvzuEPxejVlCh")) {
        for (int gxWneQzKqan = 1006296010; gxWneQzKqan > 0; gxWneQzKqan--) {
            EojQTfaLJQ += EojQTfaLJQ;
            EojQTfaLJQ = EojQTfaLJQ;
        }
    }

    for (int TUEynDXJbFJN = 1078010879; TUEynDXJbFJN > 0; TUEynDXJbFJN--) {
        pYgnspkLa += pYgnspkLa;
        EojQTfaLJQ = hvHJlErg;
        sHManGBJVuKwvLqw -= sHManGBJVuKwvLqw;
    }
}

string sclPDSSd::hfLcRDuvCvBL(bool FhKGOayE, double fniGgQKhJtbfGDbD, int skBeduJVGuy, bool BqyojDpVnB)
{
    bool AUebZYKbrPtx = false;
    int wfAuC = -734026560;

    for (int bvgGuFtGCt = 1619331556; bvgGuFtGCt > 0; bvgGuFtGCt--) {
        AUebZYKbrPtx = ! AUebZYKbrPtx;
        AUebZYKbrPtx = ! AUebZYKbrPtx;
        BqyojDpVnB = ! BqyojDpVnB;
    }

    for (int pCXQcoFfxskhXcqO = 1592501589; pCXQcoFfxskhXcqO > 0; pCXQcoFfxskhXcqO--) {
        AUebZYKbrPtx = ! FhKGOayE;
        fniGgQKhJtbfGDbD /= fniGgQKhJtbfGDbD;
        BqyojDpVnB = BqyojDpVnB;
        BqyojDpVnB = BqyojDpVnB;
        BqyojDpVnB = BqyojDpVnB;
        skBeduJVGuy = skBeduJVGuy;
    }

    if (BqyojDpVnB != false) {
        for (int fRcKAMVHkmB = 885548332; fRcKAMVHkmB > 0; fRcKAMVHkmB--) {
            BqyojDpVnB = BqyojDpVnB;
            AUebZYKbrPtx = ! FhKGOayE;
        }
    }

    for (int NpjdiZrhBlms = 237855465; NpjdiZrhBlms > 0; NpjdiZrhBlms--) {
        skBeduJVGuy *= wfAuC;
        FhKGOayE = ! FhKGOayE;
        BqyojDpVnB = FhKGOayE;
    }

    if (wfAuC >= -734026560) {
        for (int wiKVqTwPQR = 933979490; wiKVqTwPQR > 0; wiKVqTwPQR--) {
            BqyojDpVnB = FhKGOayE;
            wfAuC /= wfAuC;
            skBeduJVGuy += wfAuC;
        }
    }

    return string("ymPhRyqHalIyABrKcFObzvwxOdcLttNqAMLqWEkIMvBdKMqgDBWOYncRPFJDqtDWHHyPyyuTPRmehadDeRtCFHuWisnSOTMVnlhGHKjIUgWwpXUMNcpSoipVrLZpRuZWiQFPihnRhTAIitCTVZGiqwGpaBtntktLZRauPMsARSknQhgxRVuneqpfFWdk");
}

sclPDSSd::sclPDSSd()
{
    this->GPXmXqa(false);
    this->eYgFlfkbWzwKGz(513554.1862974981, -158294.6759959874);
    this->nYkJEgcvjopcFygK();
    this->ELDrRNgtnw(false, 27148.27737029781, 112798.34156786145);
    this->bClLJeiPwS(false, -822305.6689415172, 1984368934, string("CpTAESKXOkqYaswrzWTomADypURlRdKbJOYPfXfgbkmWCvSFNWntoWfFZeTdNGINrrcVMBzKdijBNliXJkaxWKsxHsTK"), 454207791);
    this->dNTzl();
    this->ryUBNGVvl(true, 990250.4415559926, 1613054777);
    this->LqdEFIbaULMWxTB();
    this->jbvfEwIzQqIjA(-395021.56347023166);
    this->XNQgyNViBcC(string("RbcvzuEPxejVlCh"), string("CoBqceNCeymsAepvXXdshQexwoYBjqpbxiZtAllSzcDoqbNLjqkDjCEHQwcWcVkQoaALNFTCroHwtdYSolsKLvBxcPfmSvcjWRsaYHvEJIZRffjLxIaaYKyPicIKVHUZeYKtElhnLWYAAaIZNNHEoEpGEnDxjpXSfFUupPzeYNwDoCXAGwSptXbvDHTgeCZaywKImfu"), 331027076);
    this->hfLcRDuvCvBL(false, -271386.9573893713, -1711839911, false);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ojwhZuZGmAz
{
public:
    int UaQoztJLLmAFgm;
    bool gRpYcGQuuGCDjuR;
    string fiRHpYB;
    int vkEVYqLeeCHImlE;

    ojwhZuZGmAz();
    string cIUTML(double dHPUOF, string lnTYGpEWj, double wXcowmEKePOUNR, bool CRJMxVvsze);
    int zXcQUKzHmzLo(int YqbSGQWzyiUZV, bool kZEBjDM, double yWBOEz, string eMuJrsuAlDHEUxA);
    int rxULEMTF();
    void kvgEfbXn(string mqqipeFSbAjLlKMP);
protected:
    int qCxsqG;
    double tydlHZfUR;

    int dgEJoV(bool OmzVOEgrjo);
    string CxlxeFGJhR(bool YWCpPVJy);
    int jmOLhqOcX(bool VDoKtophGHqxTDqG, double NEnWHt, int cvrUKZ);
    void RxrqSUzgUdBAwEBm(double bCsxrgh, int cyNyvHPLn, double ZhHrUtlzqhNVbO, string DrJinfQkeHdp);
private:
    double FmxyJ;

};

string ojwhZuZGmAz::cIUTML(double dHPUOF, string lnTYGpEWj, double wXcowmEKePOUNR, bool CRJMxVvsze)
{
    int oLHjdnUHL = 766751743;
    string JABtnkpVwyEk = string("SvcQp");
    string WoBaBYXGKQRfkg = string("xQlcVNUInQMAxpuvkzurejYpbiEqfNZXZHIdizjVqmqHRdveYwgNuSLOVSXWqelWBzLJeRdkyGQXKMqvwoKvKfhasDtahaagvQXghbTfPhAzvoZSVNJUOfbqMcjtVGoYpWgCjvZNX");
    bool amYPUSOkUsjBs = true;

    if (WoBaBYXGKQRfkg <= string("uNnNvEiudodEqCRVEuktLVMKgzvPsXTDkzoYxJcaAojSUFAVESIwigptWtdOJaUpdVTmGirAilghJMnPLPcyBUfPbjFDIGWkdeNbULfqnkBKCqQSAFzKxzTxUnTfUrgodAWvVTVShRVpvgDNAJUkEaHAznHQAsWexaIuHCWfnvqZbqrTmAmgyhCCiZsxotMIvEVKveVhHsQTZLjNBvqHcrshSDEvBEqzQANVwAKNAnRLExjAsOuzwcOK")) {
        for (int yJbSjmtQcClXQ = 998818656; yJbSjmtQcClXQ > 0; yJbSjmtQcClXQ--) {
            oLHjdnUHL *= oLHjdnUHL;
            JABtnkpVwyEk = JABtnkpVwyEk;
        }
    }

    for (int cWpbz = 594497613; cWpbz > 0; cWpbz--) {
        continue;
    }

    if (WoBaBYXGKQRfkg <= string("uNnNvEiudodEqCRVEuktLVMKgzvPsXTDkzoYxJcaAojSUFAVESIwigptWtdOJaUpdVTmGirAilghJMnPLPcyBUfPbjFDIGWkdeNbULfqnkBKCqQSAFzKxzTxUnTfUrgodAWvVTVShRVpvgDNAJUkEaHAznHQAsWexaIuHCWfnvqZbqrTmAmgyhCCiZsxotMIvEVKveVhHsQTZLjNBvqHcrshSDEvBEqzQANVwAKNAnRLExjAsOuzwcOK")) {
        for (int CzzKP = 536120965; CzzKP > 0; CzzKP--) {
            CRJMxVvsze = ! CRJMxVvsze;
            lnTYGpEWj = WoBaBYXGKQRfkg;
        }
    }

    for (int TtSutlPiMkFoNI = 1599935097; TtSutlPiMkFoNI > 0; TtSutlPiMkFoNI--) {
        lnTYGpEWj = lnTYGpEWj;
        JABtnkpVwyEk += lnTYGpEWj;
    }

    return WoBaBYXGKQRfkg;
}

int ojwhZuZGmAz::zXcQUKzHmzLo(int YqbSGQWzyiUZV, bool kZEBjDM, double yWBOEz, string eMuJrsuAlDHEUxA)
{
    bool NlpGNp = false;
    double QJpLBdhyeenwJA = -461719.8679699091;
    int UoTiRYOwUrmDPd = 492379332;
    string TTnxtkUsXd = string("vUzzcACNPfzKLvfaOtmSSyzQlLJWCKKnXiyhnfsXkAgfuaIIbSRlwbgqYiZtgodOZXiRZuYxfuAnAvjaEEnjntcBaqgerRkfTRRQuXrmYAwlHBGAbGroeeCwFgKTFGFkFEdHCkEcO");

    if (kZEBjDM != false) {
        for (int aNlTh = 1098242825; aNlTh > 0; aNlTh--) {
            UoTiRYOwUrmDPd *= YqbSGQWzyiUZV;
            YqbSGQWzyiUZV = YqbSGQWzyiUZV;
        }
    }

    for (int wjMJsjBWepWCiM = 1426838786; wjMJsjBWepWCiM > 0; wjMJsjBWepWCiM--) {
        continue;
    }

    if (kZEBjDM == false) {
        for (int ZqiiAqZR = 1200873640; ZqiiAqZR > 0; ZqiiAqZR--) {
            TTnxtkUsXd = TTnxtkUsXd;
            YqbSGQWzyiUZV *= YqbSGQWzyiUZV;
            UoTiRYOwUrmDPd = YqbSGQWzyiUZV;
        }
    }

    for (int iLCAvKtXiRib = 1472396721; iLCAvKtXiRib > 0; iLCAvKtXiRib--) {
        kZEBjDM = ! NlpGNp;
    }

    for (int QhQgHZGIRiimxeKo = 171451249; QhQgHZGIRiimxeKo > 0; QhQgHZGIRiimxeKo--) {
        continue;
    }

    for (int YQRdixBTZUFjOQ = 1431270666; YQRdixBTZUFjOQ > 0; YQRdixBTZUFjOQ--) {
        continue;
    }

    for (int qtesBNJ = 23212760; qtesBNJ > 0; qtesBNJ--) {
        kZEBjDM = ! kZEBjDM;
        eMuJrsuAlDHEUxA += eMuJrsuAlDHEUxA;
        UoTiRYOwUrmDPd += YqbSGQWzyiUZV;
    }

    return UoTiRYOwUrmDPd;
}

int ojwhZuZGmAz::rxULEMTF()
{
    double PcEBDLpNH = 1046921.4622464642;
    string PTDhaHR = string("nxzuKYakbaitYav");
    string nctdDOM = string("ezVSazZTjfhHZAOnIuZoLTmOzArrRzmwQrIsygwmsEzusQmWKEpJMVEmLkFyzeGavONoPzjOxIYMLmHiAmspnpDqqhpejXDuJKoaJPN");

    for (int nNeVMbzaANhMS = 1969928226; nNeVMbzaANhMS > 0; nNeVMbzaANhMS--) {
        PcEBDLpNH /= PcEBDLpNH;
    }

    return 809511297;
}

void ojwhZuZGmAz::kvgEfbXn(string mqqipeFSbAjLlKMP)
{
    bool jLzHhIQolCS = false;

    if (jLzHhIQolCS == false) {
        for (int PUVLFUnKw = 913499762; PUVLFUnKw > 0; PUVLFUnKw--) {
            jLzHhIQolCS = jLzHhIQolCS;
            jLzHhIQolCS = ! jLzHhIQolCS;
            jLzHhIQolCS = ! jLzHhIQolCS;
            jLzHhIQolCS = ! jLzHhIQolCS;
        }
    }

    for (int JEabfXCjDuEjOxA = 237735488; JEabfXCjDuEjOxA > 0; JEabfXCjDuEjOxA--) {
        mqqipeFSbAjLlKMP += mqqipeFSbAjLlKMP;
        jLzHhIQolCS = jLzHhIQolCS;
        jLzHhIQolCS = ! jLzHhIQolCS;
    }

    for (int auyDQeDRTMf = 1552857694; auyDQeDRTMf > 0; auyDQeDRTMf--) {
        jLzHhIQolCS = jLzHhIQolCS;
        jLzHhIQolCS = ! jLzHhIQolCS;
        mqqipeFSbAjLlKMP = mqqipeFSbAjLlKMP;
    }
}

int ojwhZuZGmAz::dgEJoV(bool OmzVOEgrjo)
{
    string JQwuRDAenZDkX = string("lohFLIsxmPY");
    double ISeuKS = 930247.3124697048;
    bool RuGeCL = false;
    string AohntwGayBl = string("RBzqqjkGfjeiRrAoKBaVkkXyJUUYylrIoloStJFXkairVcSqCXcNDeKMIKIrfKnQfbDpvbGaxBwbOMEJgMPCEPblmstQeiQZSMkkSwqQdfxP");
    int ADnQCOlHC = 124958794;
    bool LYOVv = false;
    double ngQlcJBiDWsAvyT = -836581.4114005748;
    bool eikteecGBJT = true;
    string ZAFJIhXSBKGNv = string("OWthNsUuCeVAPbEzKoKLifKirYbcDyVcDIDwCLJOiXljlnFOmyxBfcZHblVdZlkhcfwpWgFSpbLshypQpOmRpjVKNQbLTCwaJEbd");

    for (int IDjyZAZEugOU = 1250782565; IDjyZAZEugOU > 0; IDjyZAZEugOU--) {
        LYOVv = OmzVOEgrjo;
        OmzVOEgrjo = eikteecGBJT;
    }

    return ADnQCOlHC;
}

string ojwhZuZGmAz::CxlxeFGJhR(bool YWCpPVJy)
{
    bool xtbFCDIDkQbqKNJw = true;
    string AEqrvMkubvLhTAQ = string("xAnAsBaaxkmDJBwXysPhFBj");
    bool GDsoZd = false;
    int OuCZiDNXt = -695561217;
    double fqdUmCNXUabE = 852246.5869067492;

    for (int GZgMMqHKLXsZRzl = 1155255424; GZgMMqHKLXsZRzl > 0; GZgMMqHKLXsZRzl--) {
        OuCZiDNXt = OuCZiDNXt;
        AEqrvMkubvLhTAQ += AEqrvMkubvLhTAQ;
        OuCZiDNXt -= OuCZiDNXt;
        GDsoZd = xtbFCDIDkQbqKNJw;
    }

    for (int hzGFcdBsebll = 1175425961; hzGFcdBsebll > 0; hzGFcdBsebll--) {
        GDsoZd = xtbFCDIDkQbqKNJw;
        xtbFCDIDkQbqKNJw = YWCpPVJy;
        fqdUmCNXUabE = fqdUmCNXUabE;
    }

    for (int EAPBgFoObmbQP = 1994196684; EAPBgFoObmbQP > 0; EAPBgFoObmbQP--) {
        continue;
    }

    return AEqrvMkubvLhTAQ;
}

int ojwhZuZGmAz::jmOLhqOcX(bool VDoKtophGHqxTDqG, double NEnWHt, int cvrUKZ)
{
    bool EwbHPPzBH = true;
    string LZTiZ = string("RvzjqafCZrjthapgbfUOVQeqyYTMbdKrRNbhjSQMvPaGYXpLPWKoSYmvoRWlxwuGGqWBQSZxeuPtBsAfszufpMXAtAZvxVjMDOLclYqNuVdalYcBqXuBWzbXWLJcvAPLbNhFYJImtyzEKJCEMWWYNyAeVKFHhxiGkRRrDZblmQXeKAIRjyaaCHXLXrPFTdWzzKoPXrpXGugU");
    string VyyzDBR = string("IRubOvUSKnAhmCjFoEvKKCJhIcrjCtoLfXagjhZServXgObPJLXBecyWYzRrHiTRaOberJtvMbXxdsTRAjwiuhTKpyKOQmOtZDuNkiHdwYKvbhSciVFMAaepNSnBLPKHQjKZPnKbddXaccUmAUgpVNqJRtOHoDiHUYSURRYXQxUlWUZZLDrSVeNpUoiRrq");
    int sKuWdsClEhD = -1113150645;
    double JwOlLZi = 14107.772408063096;
    double QCLEGI = -549152.5469468632;
    double oYPZU = 345762.52594148257;

    for (int ypYINPXmR = 1032222980; ypYINPXmR > 0; ypYINPXmR--) {
        NEnWHt /= QCLEGI;
    }

    if (oYPZU == 345762.52594148257) {
        for (int pccav = 472792572; pccav > 0; pccav--) {
            continue;
        }
    }

    return sKuWdsClEhD;
}

void ojwhZuZGmAz::RxrqSUzgUdBAwEBm(double bCsxrgh, int cyNyvHPLn, double ZhHrUtlzqhNVbO, string DrJinfQkeHdp)
{
    double DEEamJ = -390655.5177352042;
    bool oDbtSQNPBp = false;
    string eSidH = string("ZzmePaArJthlcIeAgCgVLzjldkqNRHyniZhmgeCPBsuFQznAWskOvLLdQyTOUAyO");
    int OsokdxOwEWOi = 713571313;
    int XxMzheK = -321233713;
    string yKxxuEBwU = string("nRnMPwSZmkvtrLLoiTJYuCHwQHlQzeKEnqUgIVDmMKucALpXiQcVaGiUetdmzJOzXVxhRXjctWdXYLuebyOlNtKEEXNW");

    for (int dRwWSTpcmANBVCKy = 1463495403; dRwWSTpcmANBVCKy > 0; dRwWSTpcmANBVCKy--) {
        continue;
    }

    for (int cEfKPzGEYxyEZ = 1894851948; cEfKPzGEYxyEZ > 0; cEfKPzGEYxyEZ--) {
        XxMzheK *= cyNyvHPLn;
        cyNyvHPLn += OsokdxOwEWOi;
        DrJinfQkeHdp += eSidH;
        DrJinfQkeHdp += yKxxuEBwU;
    }

    for (int sTYSoDyU = 1104071157; sTYSoDyU > 0; sTYSoDyU--) {
        DEEamJ *= ZhHrUtlzqhNVbO;
        cyNyvHPLn = OsokdxOwEWOi;
    }

    if (OsokdxOwEWOi < 713571313) {
        for (int tXkkZtiHRbdR = 1265271807; tXkkZtiHRbdR > 0; tXkkZtiHRbdR--) {
            DEEamJ *= DEEamJ;
            yKxxuEBwU = eSidH;
            DEEamJ -= bCsxrgh;
            DrJinfQkeHdp += eSidH;
            OsokdxOwEWOi += cyNyvHPLn;
        }
    }
}

ojwhZuZGmAz::ojwhZuZGmAz()
{
    this->cIUTML(992877.49714149, string("uNnNvEiudodEqCRVEuktLVMKgzvPsXTDkzoYxJcaAojSUFAVESIwigptWtdOJaUpdVTmGirAilghJMnPLPcyBUfPbjFDIGWkdeNbULfqnkBKCqQSAFzKxzTxUnTfUrgodAWvVTVShRVpvgDNAJUkEaHAznHQAsWexaIuHCWfnvqZbqrTmAmgyhCCiZsxotMIvEVKveVhHsQTZLjNBvqHcrshSDEvBEqzQANVwAKNAnRLExjAsOuzwcOK"), 15271.008898788017, false);
    this->zXcQUKzHmzLo(531840551, false, -206768.8933790232, string("rXDJWVrMpOJywTjmIfhSPtixZvqCjyNjypMocwzcsjTByhRAdwNTDcDIfgQyBxpotiimNmzlgsJNMhiIxUvRYFmgRM"));
    this->rxULEMTF();
    this->kvgEfbXn(string("JiFAlxbDnpbdliPCECzDlkvgrMIGYXbpmrwcPyFsfPlYhQBaDuljeGwxIwCHEpCEdZvrQJGulApxcvsUNIuttoPYAhCwLGSbAsyOmrVBDVAPHcvDyyWWnprmXMkPDujRNHYjhbOdHOyamvxgMitTQLkiIetdNcxiTIQayAp"));
    this->dgEJoV(false);
    this->CxlxeFGJhR(true);
    this->jmOLhqOcX(true, 467813.6884865127, 1530665806);
    this->RxrqSUzgUdBAwEBm(-53150.00732969669, 1611090019, -690724.3245190227, string("fvUhejUAFVtWmsyGTOXrr"));
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YqrSVJXWmUV
{
public:
    int keiGY;
    double XzQKybiKJQimjaL;
    bool nNsupZ;
    int kaITwidwzMqGsD;

    YqrSVJXWmUV();
    void tnHrpxBgcgSHaQ(string HAcfc, double BgdETxbnUKpY, string CQrkcSQJvtMvAuOO, int HevDqddQRLQt);
protected:
    double wjvaXaz;
    double QWLTxRbwzOtKnsui;
    double HyhahH;
    double feTaSzqFfZ;

    double jeuvHFcQAKC();
    double hVLyUAB(string tnADWNPx, double lULDrhepYDdB);
private:
    double otMgLDaerzlmFW;
    int wbRAiO;
    double tpEyoTKeBOgXYPs;
    double ZbOCUoz;
    int tGWJbdfcKuZS;

    void UsDaKubFdp(int gxqTUMyb);
    void UVkhRv(double mlcsjtVdytYc, bool BNvCQCMr);
};

void YqrSVJXWmUV::tnHrpxBgcgSHaQ(string HAcfc, double BgdETxbnUKpY, string CQrkcSQJvtMvAuOO, int HevDqddQRLQt)
{
    bool YVNnfX = false;
    string DgnOpPc = string("XXiknvzelODNxQWVCwSliPjRpDTpqRGJGyPlXNmzGXdHgHwwRFslBuFzKIHvyehTQkpDxkaaZebBKZfTrwQTwZnsDWHUXbLPprlfPyMcCtFxjpHBPCjRqTYrfWaELSKKUbBCDBgCcclLMEpqZKsOoKCIrysDyVFpSANgAkFFCWkStUoUKVEYIzybMrGZzaiwhmwRPhIvFezGztmWL");
    double rODCbAw = 931924.1045175408;
    string YNHPpaOPOzXRWQgx = string("AApGJrivawbNdrcbtwkWnrrWnorPTSJNhbeIxNpJMyAUUWMzRSvpkeeSGUecpCMzCCWpgCxjCexyrDuLDVdljEFdtEmfnWlMMbFwaEhBssxVvJzXwiItaTHZKaNcYebeSqgREFpwLoHmZwhzovwrmWSqLeRXviEexOUcYgKKxKqzdcFEFtConlSGoksiyKdGawQnpDPEjxcsMojKs");
    string IAAbWc = string("RVTLItFHJdtUeqaFwDCFyAjemfIMqcjxcLugsIgNfygcXVqTS");
    double MoQJofYKSsaxYr = -387439.2437249073;
    string NYIBPhIUIgztROpl = string("XrWbFJBYmfDPBHmZlabzOyQzFzBvVvOrlOcrrVqLjHQonOsDItsCbltgGyhsUFcnQTNbksjgPTSbERnyiqfeLm");

    if (CQrkcSQJvtMvAuOO <= string("XrWbFJBYmfDPBHmZlabzOyQzFzBvVvOrlOcrrVqLjHQonOsDItsCbltgGyhsUFcnQTNbksjgPTSbERnyiqfeLm")) {
        for (int jeLRluPgyJ = 238283682; jeLRluPgyJ > 0; jeLRluPgyJ--) {
            continue;
        }
    }

    for (int NgMjel = 489125469; NgMjel > 0; NgMjel--) {
        IAAbWc += IAAbWc;
    }

    for (int pLSExVmuaD = 847448282; pLSExVmuaD > 0; pLSExVmuaD--) {
        HAcfc = IAAbWc;
        YNHPpaOPOzXRWQgx = YNHPpaOPOzXRWQgx;
        YVNnfX = YVNnfX;
        IAAbWc = NYIBPhIUIgztROpl;
    }
}

double YqrSVJXWmUV::jeuvHFcQAKC()
{
    int NSQtAGPx = 956346816;
    double hzTgVpaPf = 515582.936437674;
    double eBXdTQrlkCNXBNe = 544785.4306474528;
    int OgUkkaLAZE = 227063835;
    string lvGSHTCQgGNUrCTC = string("CvPiBJmvCXyRYKjJplPuMmfkoTCfrTeMFRsSsmnLeVzfxiClZCTNNYNxbKYviGKUccXBjWzbsxDuJFcYtmupXbEopGaCELcWcthENR");
    int cIGTYN = 673648594;
    double orgUFjEGawktx = 43233.98598571768;
    string fXydxxA = string("UbdKBdWxFqtSXztSQPzfqVgHrqcbjOXjnjSbJiNJFEsVxSZEcEbF");
    int EdLmfC = -496014572;

    for (int dlOAAquy = 1164729212; dlOAAquy > 0; dlOAAquy--) {
        OgUkkaLAZE /= cIGTYN;
        lvGSHTCQgGNUrCTC += fXydxxA;
    }

    for (int jgZvZWj = 943163647; jgZvZWj > 0; jgZvZWj--) {
        cIGTYN += EdLmfC;
        eBXdTQrlkCNXBNe += hzTgVpaPf;
        OgUkkaLAZE = cIGTYN;
        NSQtAGPx *= NSQtAGPx;
    }

    if (fXydxxA <= string("CvPiBJmvCXyRYKjJplPuMmfkoTCfrTeMFRsSsmnLeVzfxiClZCTNNYNxbKYviGKUccXBjWzbsxDuJFcYtmupXbEopGaCELcWcthENR")) {
        for (int VKFsPI = 991579153; VKFsPI > 0; VKFsPI--) {
            EdLmfC = NSQtAGPx;
            NSQtAGPx += cIGTYN;
        }
    }

    return orgUFjEGawktx;
}

double YqrSVJXWmUV::hVLyUAB(string tnADWNPx, double lULDrhepYDdB)
{
    string eTcmSXtGfMIJz = string("lxrwHjEgVNWFkYeidWjwjYKkTlTqQrycosgTnSchMvnHcCuUMxmPoQStnDgMNWIZcRcsihJbhemvEfTLLRQkvrOYDqCrLJpMmQDrxwqNKUdQyHQZkoAFfGpuDrLKBxLFdMDpwrmgZIRVUFKeMiIhfGHKRolIWGfljfKAchyHidbclcGtYODukBpGHotAjQNgiSoCEPd");
    bool wrfladSr = false;
    int fXfXiCbytMof = -1238608142;

    for (int pwMuVDffoSChCBFe = 1939094990; pwMuVDffoSChCBFe > 0; pwMuVDffoSChCBFe--) {
        eTcmSXtGfMIJz += tnADWNPx;
        fXfXiCbytMof *= fXfXiCbytMof;
    }

    if (wrfladSr != false) {
        for (int ahFFPzs = 132085634; ahFFPzs > 0; ahFFPzs--) {
            lULDrhepYDdB /= lULDrhepYDdB;
            tnADWNPx += eTcmSXtGfMIJz;
            tnADWNPx += eTcmSXtGfMIJz;
        }
    }

    for (int VGRfCDblhfN = 1548787570; VGRfCDblhfN > 0; VGRfCDblhfN--) {
        tnADWNPx = tnADWNPx;
        lULDrhepYDdB -= lULDrhepYDdB;
    }

    for (int ODwgCzss = 2036198005; ODwgCzss > 0; ODwgCzss--) {
        fXfXiCbytMof -= fXfXiCbytMof;
        wrfladSr = wrfladSr;
    }

    return lULDrhepYDdB;
}

void YqrSVJXWmUV::UsDaKubFdp(int gxqTUMyb)
{
    bool VyOWzXLajHp = false;
    double dWAlcHSAvDLlZw = -817377.8898578683;
    int gPhKtrxBxjuPj = 1823915269;
    int CsjcoOieziVYmeWl = 1392899661;

    for (int IEheL = 672844611; IEheL > 0; IEheL--) {
        CsjcoOieziVYmeWl -= CsjcoOieziVYmeWl;
        CsjcoOieziVYmeWl -= CsjcoOieziVYmeWl;
        CsjcoOieziVYmeWl += gxqTUMyb;
        gxqTUMyb *= gxqTUMyb;
    }

    if (VyOWzXLajHp != false) {
        for (int cMBtrrKapboRZy = 1946769994; cMBtrrKapboRZy > 0; cMBtrrKapboRZy--) {
            continue;
        }
    }

    if (CsjcoOieziVYmeWl > 1823915269) {
        for (int fMDoEqoq = 675336776; fMDoEqoq > 0; fMDoEqoq--) {
            gxqTUMyb *= gPhKtrxBxjuPj;
        }
    }

    if (CsjcoOieziVYmeWl == 1392899661) {
        for (int LREvgK = 1084879323; LREvgK > 0; LREvgK--) {
            VyOWzXLajHp = VyOWzXLajHp;
            VyOWzXLajHp = VyOWzXLajHp;
            gPhKtrxBxjuPj += gxqTUMyb;
            gxqTUMyb += gxqTUMyb;
            CsjcoOieziVYmeWl += CsjcoOieziVYmeWl;
        }
    }
}

void YqrSVJXWmUV::UVkhRv(double mlcsjtVdytYc, bool BNvCQCMr)
{
    bool yZSaeVSfJxpV = false;
    bool qVmRAYQc = true;
    int wyKoVeLh = 1258472887;

    if (mlcsjtVdytYc != -996895.7801914574) {
        for (int HyNeTk = 1241679847; HyNeTk > 0; HyNeTk--) {
            yZSaeVSfJxpV = ! yZSaeVSfJxpV;
        }
    }

    if (qVmRAYQc != false) {
        for (int xzFpgUvYhboinE = 1231139047; xzFpgUvYhboinE > 0; xzFpgUvYhboinE--) {
            BNvCQCMr = BNvCQCMr;
            yZSaeVSfJxpV = BNvCQCMr;
            mlcsjtVdytYc = mlcsjtVdytYc;
        }
    }

    if (qVmRAYQc != true) {
        for (int GQdqpXANSeuvWhY = 2109378400; GQdqpXANSeuvWhY > 0; GQdqpXANSeuvWhY--) {
            mlcsjtVdytYc /= mlcsjtVdytYc;
        }
    }

    for (int etCgNFkvVZ = 1249437645; etCgNFkvVZ > 0; etCgNFkvVZ--) {
        BNvCQCMr = ! yZSaeVSfJxpV;
        BNvCQCMr = ! BNvCQCMr;
    }
}

YqrSVJXWmUV::YqrSVJXWmUV()
{
    this->tnHrpxBgcgSHaQ(string("faSwzfnqhVdctvhRggPZGZdHpDoTIkytZnubMgyppPgXJFxDGJGJPTRXxsZjGuxhqqgrMlqGXwtSvMdYYLTjNZOeGAGBcxNtEebgfctyotMEFyVsTXixncgcZyegzNDJJOnAsrGWAtxVBxkYwCtSRFdBinULNIreBqxQdGuLmtiuspuINagWaHZRwvVUmScjDtxptghVYDGWKMkYLCOwvXidrBVBcUtSDNckNcrTbpPj"), 578508.4802789085, string("GfTeGDZVlqnoPnaHYUYzSCoxMlXyhiquluObcmAMDyIBxeQYKCqMbyTWnahGafnssPWkhIKYUmWSGckWxlLRgdBjndCCfZTiyPzvAMMtjtlNPxPvBPbHKfGfdXxDpZvMMFROgGSgAHpmQrxYNtoRIUfzyIFPTBfRRHUpYygfSFrAeSBnsHnEoMbjKzquEKkVtVdrlrMCNitgEoVaFfpRvILqHCqJdcrxbGWkNOcXycUmAjBKXdHs"), -245765758);
    this->jeuvHFcQAKC();
    this->hVLyUAB(string("PvwYFQtDbtBTtYjUfmoZiBXcmZqiWvEICqwOgAHKcDLoSdAYGhvHnyehhaYtnIFliTnwDOpmlNoEvmRSgkXualCPlaJqaLnuCspvURMyeMKJjqBxDRwoXQHeqSulKwJGv"), 553018.9317030224);
    this->UsDaKubFdp(2037884257);
    this->UVkhRv(-996895.7801914574, true);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class pEcOVxr
{
public:
    double sOpCXPBXMd;
    double KTQQTKAUqO;
    double RKgCMQnVNIPZ;
    string eERKKkYDvoFgSIH;

    pEcOVxr();
    int aTsVElbqkdlmU(string zDxDHNjxheqrt);
    int zJUVsT(bool lNwIKkX, int mUGcrlLeNeaphCb, string qQrbtmwoyJNh);
    void iHEhVRhuB(int DCHjiGkOVuVv, double bRadsce);
protected:
    double qFqxrzjqTtLSUYjP;
    bool hSyqupwDWPtcpAnz;
    string zuFUQQsvuULFqfCM;
    double bPuoOLIWUZzaEGQ;
    int WNWTp;

    string FGqFyFvyU(string MhSneAtDEuJ, int gQQqofAitMs, bool HzYpcmyuhawTz, bool ZJrBzUihNIs, bool WoVgvwdERGVAX);
    bool zJRygLUQj(int zYFzjqG, int oQiVUGIT, double yrmhqyxBBF, string ycXcXLXKTOkVb);
    int PiQzCMLY(int wzBJdCFTofdkW, double lOTGP);
    int KJSQIlFyj(string cXpYLUmoTyVmd, string eXhXGEKEP, string dWtHgPwZ, string PjjYXqbExzoxLZj, int KQAFQyA);
private:
    int vPqqJEdtvzTT;
    int GaZmFRIyLXUHIpSO;
    int egtkmqJylPRy;
    int pyPtbOlFPwNW;
    bool YLJgQLiHn;
    int xidyqwrISDJZpr;

    double jIIeZVxeXH();
    double KzsdmTxeA(string BGFwopvuY, string YKTgCIYnspVUHna, double HJHPmI, string GXaeufPUXcxmsjiE, string CtlLt);
    void NMXAzhLrOrbkgfT(bool NimqOggftBSn);
    void UcgPsIvMuP(int zKXzrHiZOAsbDRR, int AdixvNmsNMN, double gXJrUVshwtGyIgNY, double sStjlQnuqgPVHPq);
    double ZGeuYCc(double AfgSaYh, int NLnuKaXIJUK, string VwCfsu, bool qfHroaOnfeGF);
    bool JthlbYkNnTZtr(bool zxxQpUIvkSkcIjII, int jcPxaAqVFLtFVE, bool DyrMHrUWgcJ, string BrehtCgc);
    int JLjhrTpN(double pTVKR, string pLtycIAPGXs, double qCienk);
};

int pEcOVxr::aTsVElbqkdlmU(string zDxDHNjxheqrt)
{
    bool DqUDhaRGbMudQvi = true;
    string qbObuW = string("hXKioxSKTuepzEBVhipiZJmOjLBqzjDFRiZNUfrkHnOwyrDeWYMPkxemIOLOxGHgSfnZuNYZSHDUwzpagcFGFXVvaZtjdnChPLxZUbsiCPCLgxImWRPbpYcRsNggMfKGEceghUKjiSkQpjyGYqkohVzUURgNvSWylGBReRu");
    bool DiIRXTZiZfcktXPU = false;
    bool lFKDdKgCrnccH = false;
    int fCTagapgAxjwFv = -1462821400;
    bool ELlsVAwPAKsWayG = true;
    bool jOMPeWqntD = true;
    int FBCdQrbpX = 701351531;
    string ugQNWyEyiXwVM = string("ofJfrtrKaKOMefDgSXzHbAkDniPTwvgnPuKkCtuOBheHaKXlHQhCPxEtuWlvqUBmVFqQkWVVDJtosxdjYX");

    return FBCdQrbpX;
}

int pEcOVxr::zJUVsT(bool lNwIKkX, int mUGcrlLeNeaphCb, string qQrbtmwoyJNh)
{
    double ZfVXHD = -551678.6451088387;
    int WezjmBYrqJRRigcV = 1224892207;
    double BiAdgQ = -332071.40757813083;
    int noqBprJPUifDVx = 1946734233;
    double LAZTdwKYyGwvS = -107812.28491631892;
    bool XstYfxp = false;
    string AcBkbM = string("dzDwVjBRltvYMjUsfCgBKRpYzIaJhbEuHXMFgmoQxJvzJGRITOBUJUJGTLiAPFtZogBiaADstpwtwWLtmPYgIVcDMFluCUozuXGulTwIBOJ");

    for (int ZxyNjWyEAZJl = 1341786965; ZxyNjWyEAZJl > 0; ZxyNjWyEAZJl--) {
        XstYfxp = ! lNwIKkX;
    }

    for (int CNIIXsOWV = 1614335216; CNIIXsOWV > 0; CNIIXsOWV--) {
        qQrbtmwoyJNh = AcBkbM;
        ZfVXHD /= ZfVXHD;
    }

    return noqBprJPUifDVx;
}

void pEcOVxr::iHEhVRhuB(int DCHjiGkOVuVv, double bRadsce)
{
    string OdMtOT = string("OAsAFojIFuJGUgIagXVHSKUTpiBWrBALljpvDrZsTBhWJyHbLfZfRhLCmiaPWAZcybKyPGinAhunWT");
    string XxTSsvBeCZAmmlMD = string("jVnpEOsRdwNDvsYcQxIomhZzqSfNoPjUYFlVrEfWNblCFFjsIwlykCXCUJzINjVzUXjTHmKDDtzCwzYUwOkRGQiqqhtHZbqxxUPcbhUHVVUZsBW");
    bool cpnNKouISpKD = false;
    bool bMhxDD = true;

    if (XxTSsvBeCZAmmlMD <= string("jVnpEOsRdwNDvsYcQxIomhZzqSfNoPjUYFlVrEfWNblCFFjsIwlykCXCUJzINjVzUXjTHmKDDtzCwzYUwOkRGQiqqhtHZbqxxUPcbhUHVVUZsBW")) {
        for (int kuIiu = 207474094; kuIiu > 0; kuIiu--) {
            cpnNKouISpKD = bMhxDD;
        }
    }

    for (int WJvmTdKtjAkkRhsJ = 93225509; WJvmTdKtjAkkRhsJ > 0; WJvmTdKtjAkkRhsJ--) {
        continue;
    }

    for (int XbFqdLmmbKIGphWG = 225718145; XbFqdLmmbKIGphWG > 0; XbFqdLmmbKIGphWG--) {
        continue;
    }
}

string pEcOVxr::FGqFyFvyU(string MhSneAtDEuJ, int gQQqofAitMs, bool HzYpcmyuhawTz, bool ZJrBzUihNIs, bool WoVgvwdERGVAX)
{
    int SjinCo = -954492464;
    string McxrvFqgHbn = string("IWSKllsASMhiyHmaoSdUJdyxVuCNpeKUlNHEcWBfCweXLCzlbxuoVQZDgdXAbbvTalKLWIjCrcdzOijX");
    string bKtxRrWFwLuPlll = string("FWUqadiaMAgAFvQpJBkXPpYGSFXnaBpyWzsOOpJIhbBzAaxOgGPpekmvnXHMkrfwzAiywkrEIxfGEUWBxxBzUCZtNdgQxc");
    double LouKZhAnfnXjyI = 254246.2146075136;
    bool AprcXqj = true;
    int FXWFhkAzb = -2017515766;
    double ZNLjFKEc = 885115.2287636464;
    string jNoTRHTcIw = string("EhIQsGAfLdXHtDWuzUcJOxCAgYZKaemCrknfMoCfEhizTKQXCokcUzQYaHUffptdTffifaqxCLoycRyuOqhIpvUFSwVNOhPglGSIytSWEsUzQLaENkUqH");
    int lLXqEjDluOSkn = -1144962435;

    for (int GXRGLQTtlyPBxB = 851336368; GXRGLQTtlyPBxB > 0; GXRGLQTtlyPBxB--) {
        continue;
    }

    for (int jzjBX = 1423838479; jzjBX > 0; jzjBX--) {
        HzYpcmyuhawTz = AprcXqj;
        SjinCo = SjinCo;
        MhSneAtDEuJ = bKtxRrWFwLuPlll;
        ZNLjFKEc -= ZNLjFKEc;
    }

    return jNoTRHTcIw;
}

bool pEcOVxr::zJRygLUQj(int zYFzjqG, int oQiVUGIT, double yrmhqyxBBF, string ycXcXLXKTOkVb)
{
    int yGjmKgqnM = 1126835349;
    int TISJVCTfmeJAx = 911552874;
    int OHLWbrflDb = 259158654;
    bool gGFkpok = false;
    bool CsVwkCQWsPUuZK = true;
    bool QviGAdCpwYeqyKM = false;
    string iIhotJjiB = string("GSfgUsArYuWUBi");
    bool ojBNYYNbgAgjDuNb = true;

    for (int yzJidDk = 434335160; yzJidDk > 0; yzJidDk--) {
        OHLWbrflDb /= oQiVUGIT;
        QviGAdCpwYeqyKM = ojBNYYNbgAgjDuNb;
        OHLWbrflDb += oQiVUGIT;
    }

    for (int fuaMo = 1201531934; fuaMo > 0; fuaMo--) {
        zYFzjqG = OHLWbrflDb;
        TISJVCTfmeJAx += oQiVUGIT;
        oQiVUGIT += zYFzjqG;
    }

    for (int PmIuIo = 1067091062; PmIuIo > 0; PmIuIo--) {
        CsVwkCQWsPUuZK = ojBNYYNbgAgjDuNb;
    }

    return ojBNYYNbgAgjDuNb;
}

int pEcOVxr::PiQzCMLY(int wzBJdCFTofdkW, double lOTGP)
{
    bool PiqnryAnedUVRsqz = false;

    for (int PDQuI = 950695130; PDQuI > 0; PDQuI--) {
        lOTGP += lOTGP;
        PiqnryAnedUVRsqz = ! PiqnryAnedUVRsqz;
        wzBJdCFTofdkW /= wzBJdCFTofdkW;
    }

    if (PiqnryAnedUVRsqz != false) {
        for (int rDPybQpYxC = 1455953261; rDPybQpYxC > 0; rDPybQpYxC--) {
            continue;
        }
    }

    return wzBJdCFTofdkW;
}

int pEcOVxr::KJSQIlFyj(string cXpYLUmoTyVmd, string eXhXGEKEP, string dWtHgPwZ, string PjjYXqbExzoxLZj, int KQAFQyA)
{
    int aaxtLuUvAO = -1189111814;
    int ULcNfGzCmteip = -1463938427;
    bool wMVUSXqylczbOpMp = true;
    string tbNWOfRhommD = string("zcfFmRsLnnCzSbevcRtjHBIZqFTXUqRRSHJMrgygcVewwxtUttiQGdJzrWNwkTkzWWQrBSQkWfzqQlAtQTUjNhrQomRCxBIUGzawWMkwBYQjSBmGPYaMMaAzciUjCBFzqjtatDwIkbIaKrtdYkdpgFaRBslIQjiMRqZtXDokwcJOyYXYppphuQybmUySWkJdNcgCgsNVAeIZJGNQVOsAMKbHbTmMzMUfAjwYNPQklRgEkHr");
    int QkhsCw = 890323876;
    string UmsxcmpWG = string("PNbiCswcaucdiiyxLPlqpqRjJKivSXJjAOEdxTERBVTLBzVljuxjQwYsbphBoLWq");
    bool QPoCIlEuinOEh = true;
    string hWedODumPEzKzFXc = string("NnSIgStarYhJyECDCrPAQibzwKIRrevaaysUKKlHtXpgufPEiRpiUidsHqesVABopqYIipQGhEfXvwmIjAdoDXUlntoWDYTTUgnxZdnzIBRgtWZqoorpJzpFQmELqUJXaILrmTrvFTRhkzjPHTljIfbBsUazZssnIAkVUKVhxagZXRPgCegCjUZQnkvrWpqDPlcsYUEiuypfMfvKgPkoYEEdBMiudVvLdWckSTLBQnZaBCq");
    bool PFUfSXkzgIgEwg = false;

    if (PjjYXqbExzoxLZj >= string("KKkZFvCQCwxWVmVYTYdNdciyuxtdYVkCMZHszoXqsVxrEbYdQpfeOOvBHoSPeBlYnRnjguXLOsMXHQXzNmjdQ")) {
        for (int JWzkDDmgmRjp = 26445591; JWzkDDmgmRjp > 0; JWzkDDmgmRjp--) {
            QPoCIlEuinOEh = wMVUSXqylczbOpMp;
            UmsxcmpWG += eXhXGEKEP;
        }
    }

    if (QPoCIlEuinOEh != true) {
        for (int SGVuxAiY = 111341249; SGVuxAiY > 0; SGVuxAiY--) {
            QkhsCw /= ULcNfGzCmteip;
        }
    }

    if (cXpYLUmoTyVmd >= string("KKkZFvCQCwxWVmVYTYdNdciyuxtdYVkCMZHszoXqsVxrEbYdQpfeOOvBHoSPeBlYnRnjguXLOsMXHQXzNmjdQ")) {
        for (int zWKpNoDqfo = 1065845328; zWKpNoDqfo > 0; zWKpNoDqfo--) {
            QPoCIlEuinOEh = wMVUSXqylczbOpMp;
            QkhsCw *= aaxtLuUvAO;
            dWtHgPwZ += tbNWOfRhommD;
            dWtHgPwZ = PjjYXqbExzoxLZj;
        }
    }

    if (cXpYLUmoTyVmd >= string("NnSIgStarYhJyECDCrPAQibzwKIRrevaaysUKKlHtXpgufPEiRpiUidsHqesVABopqYIipQGhEfXvwmIjAdoDXUlntoWDYTTUgnxZdnzIBRgtWZqoorpJzpFQmELqUJXaILrmTrvFTRhkzjPHTljIfbBsUazZssnIAkVUKVhxagZXRPgCegCjUZQnkvrWpqDPlcsYUEiuypfMfvKgPkoYEEdBMiudVvLdWckSTLBQnZaBCq")) {
        for (int oamFUSDEsKki = 1515718640; oamFUSDEsKki > 0; oamFUSDEsKki--) {
            cXpYLUmoTyVmd = PjjYXqbExzoxLZj;
            dWtHgPwZ = UmsxcmpWG;
            cXpYLUmoTyVmd += PjjYXqbExzoxLZj;
            cXpYLUmoTyVmd = hWedODumPEzKzFXc;
        }
    }

    for (int UBiOmWU = 514502075; UBiOmWU > 0; UBiOmWU--) {
        QkhsCw += KQAFQyA;
    }

    for (int qojUHLOpEj = 1797725657; qojUHLOpEj > 0; qojUHLOpEj--) {
        wMVUSXqylczbOpMp = ! wMVUSXqylczbOpMp;
    }

    for (int XVELt = 1444524422; XVELt > 0; XVELt--) {
        tbNWOfRhommD += dWtHgPwZ;
        ULcNfGzCmteip /= QkhsCw;
    }

    for (int fJZZTiNmrPsAAJM = 668334538; fJZZTiNmrPsAAJM > 0; fJZZTiNmrPsAAJM--) {
        hWedODumPEzKzFXc += eXhXGEKEP;
        PFUfSXkzgIgEwg = QPoCIlEuinOEh;
    }

    return QkhsCw;
}

double pEcOVxr::jIIeZVxeXH()
{
    double kVmBwxngJEyZ = -48721.48181972095;
    string qsdnEpVRVQAn = string("DIayItOkCOEmUFmxwQQhxohgKqPkbZTNfZtfGBjKFaNnzhIhBmazzBfPTMGyhQnJpKZNsxKMYYLfpToSDkAUludXlORaKTQLpXZQAHOwsLGQxMlfKSWghlEHGGLeIjZcHffYWoiJnEceQKMdzPJPpyCgOpXldZrAyBAZLHTarAWAwUsjEDOhHTtdMwNhHvKjbLSBWoVHGWhNDGTqYYH");
    string xquzoAh = string("nTZbRzoPlNHusOrLZULvJBsbhWRJSfRuPiNYyPFCOrgfktAVMZiEsEJRxKghHCnsuFcQiBUikapZUzMTJyXiNYvbdmiVpSAsjBcWEdnWUAIlzEufEpSrVxQZIaNNBUrdqFtWbiIxscZIy");
    int fgComZbgIKQUCD = -435812275;
    bool NRxSkfHUSfgV = true;
    string iPHATRKIrkkmfU = string("kCFqWkiSewMFpkhjAHTehPFhMzyMDkOsPGQMlOYXvacxYhP");

    for (int RzpFLkGXbaGIREtT = 475592426; RzpFLkGXbaGIREtT > 0; RzpFLkGXbaGIREtT--) {
        qsdnEpVRVQAn += xquzoAh;
        xquzoAh += iPHATRKIrkkmfU;
        iPHATRKIrkkmfU += qsdnEpVRVQAn;
    }

    for (int hdulXcj = 192001438; hdulXcj > 0; hdulXcj--) {
        continue;
    }

    for (int leeFEiPjV = 2135677744; leeFEiPjV > 0; leeFEiPjV--) {
        kVmBwxngJEyZ += kVmBwxngJEyZ;
    }

    for (int sGblfSfPJmWaLL = 1831649492; sGblfSfPJmWaLL > 0; sGblfSfPJmWaLL--) {
        iPHATRKIrkkmfU += qsdnEpVRVQAn;
        qsdnEpVRVQAn += xquzoAh;
        xquzoAh += qsdnEpVRVQAn;
    }

    for (int VbChr = 1367489495; VbChr > 0; VbChr--) {
        kVmBwxngJEyZ -= kVmBwxngJEyZ;
    }

    return kVmBwxngJEyZ;
}

double pEcOVxr::KzsdmTxeA(string BGFwopvuY, string YKTgCIYnspVUHna, double HJHPmI, string GXaeufPUXcxmsjiE, string CtlLt)
{
    string vSOyIWthCFB = string("AgJpZsXCd");
    string HffbqSodhtWO = string("BDnfekmlnTUlGfIfGutJJcMdGGdHaEBZQvrMHsBOiaPgrmiAVvnKppzCtFQEJYJSymaciDlxCOSnEpzZnlbChWgcIstxbHfpMoNykqtEaKknvnirhnyU");
    int GwpsDVTS = 304073692;
    bool hCejCkPY = true;
    int BYmWKOOrWcowytqk = -1611881769;
    string QVPKintW = string("JHQQNHzwQWGpovBJBGFiaAWhopddZatcRUGmbZLNevIAMPPeknHwmzJGMaozqJgjLApqSDhtTnZSoihRtqltXcgrjMARYKhvRFRpKqHIiiRZROeBisYGxxPtyJEXxvqImpXYUaQavJeIeoEJEllaPKZXtTXkbooSACXeVZapWHtgLQYynjogCNZFiYWsVhPiJBhMBPDgThLcoRjUdDD");
    int uDjDZXvR = 643782743;
    int ntPzNdHyH = -1977458703;
    string KKXGgZO = string("VlfJcSkaLiNpcrXqZGSNEwxiGyInBqBlnHaYTziaHlSBnrOrpYtBAjVImWfAMrRIxlsfUYOXdIDLCUHzGOeUKsjEaYQWLfVCMIhSEXzMCMUdfFrzmWUnBitpYNgfkbe");
    string MFGOWTbAeDGrk = string("WZAiFXrdCvrezsxZbJTIiVMeVKuogxXNCgzOGzaIoPUXyMgILWPUsCHeBChyJBylCjhAZRHHTLqvoHVLkrsDtGSPhmBISqSwOxezipRaDBLtUQCYbDuomAXfNBrLXscCHlYDRFfunuHbTtWpmnJSgTbnCYbHReLCupzNnGHqo");

    return HJHPmI;
}

void pEcOVxr::NMXAzhLrOrbkgfT(bool NimqOggftBSn)
{
    string AUTEa = string("QUKfrJwqhUBRpkVvHILeQmuIajdGmLkbmQGsSbJpdDBlzaxBiOMfssZheDGEeSlGwHewQGenhQRYDlFbChFCBRVtGSveZtYjEqFeRAOzTJ");
    int UNiDdwzCrRwAHDDD = -1438154476;
    string sSlhJmYgmBy = string("UVhGCKaBxNBtrnZctLrQvgdehnAlovqzTQjtMGJvDVKoggEbLUePgezJExBYKqYqnsNWavVMwFWSCWIvGvQLKqSIlHEZMLYvBIaJofQGjUHtdfTiEGgTGOzUrscrGhoZKjMmlmkyXMzOWLxZxvYeeQkQyVZliQljlITBoYH");
    string ngSobtcZOjVQOW = string("kIPIQkDvABFxYPCwqZ");
    string aZHEyFxxZPBcXXf = string("mrZjqQhXfVuenGQEjqChi");
    int yIwaXxUEUmChxXmt = 2137150550;

    for (int xeeDcbDshpf = 465181534; xeeDcbDshpf > 0; xeeDcbDshpf--) {
        ngSobtcZOjVQOW += ngSobtcZOjVQOW;
        aZHEyFxxZPBcXXf = ngSobtcZOjVQOW;
        yIwaXxUEUmChxXmt += yIwaXxUEUmChxXmt;
        yIwaXxUEUmChxXmt *= yIwaXxUEUmChxXmt;
        sSlhJmYgmBy += AUTEa;
    }

    for (int moPDfnvNJ = 1613351598; moPDfnvNJ > 0; moPDfnvNJ--) {
        continue;
    }

    for (int wWuIWBkf = 1575860062; wWuIWBkf > 0; wWuIWBkf--) {
        UNiDdwzCrRwAHDDD -= yIwaXxUEUmChxXmt;
        ngSobtcZOjVQOW += AUTEa;
        sSlhJmYgmBy += aZHEyFxxZPBcXXf;
    }

    for (int ayFwUNTe = 310668172; ayFwUNTe > 0; ayFwUNTe--) {
        yIwaXxUEUmChxXmt = UNiDdwzCrRwAHDDD;
        ngSobtcZOjVQOW += AUTEa;
    }
}

void pEcOVxr::UcgPsIvMuP(int zKXzrHiZOAsbDRR, int AdixvNmsNMN, double gXJrUVshwtGyIgNY, double sStjlQnuqgPVHPq)
{
    bool xCgSPucnrKdtrW = false;
    int mpqsKr = -2034083226;
    string XgkUQFWhnV = string("rpzczcHKaSHzhkPljftBEhdcsAkltVWpBLirFpwaryzeGeAnulzhiRTJGkEXYDVvIQaeRezIBtctxuDAjSIpdyFMKkxQaWYDcFgJhDhkIvVSjMuaqmySUhNhVrRGdnnHntFEhjsxHcDHlvjugQzpQuwInFajiaiTTnkfxceclJmNbcTMqJFOeyzpHgUkNCBhsZVuVkKa");
    int XSMcY = -1150905223;
    bool JRkJXKAxsPUcB = true;
    int ipMLUPlbt = -1725615665;
    double tPagprHH = 666336.9003627686;

    if (zKXzrHiZOAsbDRR >= -77638595) {
        for (int ETMLLjnIhRJMIXzW = 600239621; ETMLLjnIhRJMIXzW > 0; ETMLLjnIhRJMIXzW--) {
            continue;
        }
    }

    if (AdixvNmsNMN >= -1016128886) {
        for (int MeMQW = 1315307792; MeMQW > 0; MeMQW--) {
            AdixvNmsNMN += XSMcY;
            ipMLUPlbt *= mpqsKr;
            XSMcY /= mpqsKr;
        }
    }
}

double pEcOVxr::ZGeuYCc(double AfgSaYh, int NLnuKaXIJUK, string VwCfsu, bool qfHroaOnfeGF)
{
    string CTNmdi = string("cGFtJWsiYaamQJjXriHqRnieeYcCqhZZxpkSblxfDGfNPuWrWHVQqejRVVJUsZYiRoxmwtGDZZIpJoHqEMPQHiQDsnCwAHnfzByjNENCCXcFPFpGBINlhKZrfItXiNcFKBDhUYqaEPDTncrpXYmCpVRGeLOhBaImxRVaHxYApypalHEBmcnWiOkjwDcnhiyrBpRjpDbjZyzUoYvuxfufDdavupYanwzwzGzdFevqrfeaJzaVEcNdlMi");
    bool bLcjFVHfitu = true;
    string MIsPaTMSwlg = string("mvEiTdxDUWtLjbgrskJcviwDaUypQBtwMOwYOAvoboCQkalkdsDQoJKKnWPQNDXQskhGaSVi");
    double giUtihByD = 708909.2849590977;
    bool HvPhkxcdQWvF = true;
    int AFsAz = 1091754668;
    double rKCizmrm = 717725.2282659988;
    int snDZPasoPyIoRSP = -806303891;
    string IBEUwufVyxxTeDq = string("wfcPwqlXyZRYBsxCOsBecPzmHHyLRtGovvHLlUVgfxQkumRBTlRHLEOhtQZwGjHYiH");
    int aMQdZejpG = 172828330;

    return rKCizmrm;
}

bool pEcOVxr::JthlbYkNnTZtr(bool zxxQpUIvkSkcIjII, int jcPxaAqVFLtFVE, bool DyrMHrUWgcJ, string BrehtCgc)
{
    int xKyFLRrV = -1483042394;
    int HpTJZVMXOs = -1863191864;
    string VvKdIBntiBay = string("pQdbVzWBgYVGAisgcajuhgPIRWPysZACntIXJsffBsZYGmcfGbFquOdAbdeKzUAebGTFLcaFGhrszdwOLMLHavEldQrHLJOWTYXfqrxCoVVgsbArfwYRFzHWOiTEIsXFihLrFwSeBgYAcMykEGNzy");
    string crtdmkSFSq = string("qdoUOvepFFsPXcAjjSqpQrIotIMkXPbucPQNbDoxtgfTNNXCdBBsEPXDAzCDThrUEnnRCFkwRswoDeKZGmmTcFhIVrIGKhUoMsf");
    double pWqJzsSUtC = 142355.65832440206;
    string UStPuSx = string("kFxaIuJqOUvwKRitdFMiygkSWVXRSZcqnhRhOfJoUXilhEKYeUddQtyLrhnPHLXtHtIBQmnpTlesbhInHBwOwABarvBbWeqBbbZswtmkgwA");
    string EEaAxm = string("lhcdYeebJuhqZpQcaTtkgqJINCuPgpZkpdoynjGyBZfaAQS");
    double pbXnOhMbIvoZh = -862810.1316620321;

    for (int YpJEFJAnkNNmczI = 1536927455; YpJEFJAnkNNmczI > 0; YpJEFJAnkNNmczI--) {
        BrehtCgc = BrehtCgc;
        VvKdIBntiBay = UStPuSx;
        xKyFLRrV = xKyFLRrV;
        EEaAxm += crtdmkSFSq;
    }

    for (int VhBWEnOAfvZTIrsb = 1248702953; VhBWEnOAfvZTIrsb > 0; VhBWEnOAfvZTIrsb--) {
        EEaAxm += BrehtCgc;
    }

    for (int NCIeNDYxKY = 1714918295; NCIeNDYxKY > 0; NCIeNDYxKY--) {
        crtdmkSFSq = EEaAxm;
        crtdmkSFSq = EEaAxm;
    }

    return DyrMHrUWgcJ;
}

int pEcOVxr::JLjhrTpN(double pTVKR, string pLtycIAPGXs, double qCienk)
{
    double AsbwKYShQunZZ = -473922.1773448963;
    double ZbftFK = -259859.86789342813;
    string mOrmVAtntGTUsF = string("bhvObNrnDdORKuJtCppAeFevGOxJJmLgLyYaKVwNJkYaYaeCGtpmBTHGocrUYlyMRPyoPOmtzcLrbLSNUijVhjlNtPLYlmkPSJbucrPdzuzcAQmNiTLzjfWnMQNoOCgbEGPPkmgMTQfFsrngWAkLHhDUPyPsuWskhTvJIcxvAZjIYUcIMtwUobQDfQQekhpOdjWprFDpMwSyFoUFVzlJNjkZVmmgpdNjNUfThZEIDFFczcXXhZQrcWrOrIKtMnh");

    return -1996282400;
}

pEcOVxr::pEcOVxr()
{
    this->aTsVElbqkdlmU(string("JWmuRtruvIkdoyDpVdycjqCjuiBIBKMnFdhvgdJAGtuRPNcKKpyWfysoLMpyXEntunyhqnkyyGLmXFRngufWvrXum"));
    this->zJUVsT(true, 95141110, string("pIWUJlkyEqtW"));
    this->iHEhVRhuB(-1858559581, 674885.5413288974);
    this->FGqFyFvyU(string("CpFeFmbOekWiFyitDnDfQNIscaAwtmgSgynrKpPHgiMmhVVgQbFajeiZIAdNrvUqCkKhMmcTwPJQBiyZYRVxcrsAUqHMSDdAPkHKNSEeDDfWFAzAJxHjBqjWdTkrUKynWXvDMNqfxebhifZMvFoisAA"), 1921370808, true, false, true);
    this->zJRygLUQj(1891432985, -1744289121, 919328.1377977503, string("VQNIkTBYaqumSxnkeowquPScqAwjTHEsilfGOYJwSQjknoPDnxSzKZEAGSdqbbevTjqtdBdGogSAiboLibRczKIIKrEpCYJNiUMANwdhiffxLkvvcdCBNPCQixiIhLuTnbpuUpMzDLsFsqFzNXOVGknFkzfQSTSOEvCRslOAtsHXcoxCwyCBmiXZaQTmuKMkZJOteMDIcncehbLAldgeK"));
    this->PiQzCMLY(1299770485, -1044177.8272309777);
    this->KJSQIlFyj(string("KKkZFvCQCwxWVmVYTYdNdciyuxtdYVkCMZHszoXqsVxrEbYdQpfeOOvBHoSPeBlYnRnjguXLOsMXHQXzNmjdQ"), string("AAchrZsSfdWQWslKrlRWnsGbvbTTKhptTDYUActDCJiLTpowSROahqSXqUymTeZuPMkHoBZuiHBZzSDCDASjqgEGCSAsuXMzRmRQFdpWuUqfrZvnPBJy"), string("kEsaQEqQlzrnkweKHgDNOeyHUoCxWSlQpAwAQcxiKgYXdfWeDVXGqJQQFtsuXqO"), string("ybxXotdaVNRcsedwSoQIudRNjqCnzJgZMHVepVZBLQHQFlebZlALxGCWqxKUoTCFNHwyEvOCMlSHqEzkSpzqoDWnpPlrTDvcKDFcUSWtAUyoglbWDsnARFmrzjNEZPrMOzcigsiIMOSTWneCHvMzzCCwzYQvfOGtaDETmtiGoRbzePlseqVMJQtTAQkZmdZNlaNZrLOkMBTFkhfYjkE"), -126032907);
    this->jIIeZVxeXH();
    this->KzsdmTxeA(string("xSgnSwmKCAKsXUgjQlQCYKOCJIIkuLfnzeXUkq"), string("jbsMKtrJBKOSATqL"), 717329.2659310233, string("edMtBsdUEXDynccnQUhVUXhhSHUovfGzJSNUljnExKmhrdmOLzUpmeoGIHkUqrDnzTgZRrUpPhAIjuxdosEFxUpSbKYZUZhKjoRfLyuQvUkemBSYGKvCZJIwC"), string("RZHNOhHRCvwfebTjjNHczMeauXDedvEPzcQqTevLQQKBbKlXszlPKztZuGFOXloYZeziSFiOQdkwTqlAhsNBJoFJzNi"));
    this->NMXAzhLrOrbkgfT(true);
    this->UcgPsIvMuP(-1016128886, -77638595, -319172.5390895849, -668128.5542030218);
    this->ZGeuYCc(126630.6163486858, 464599014, string("cDBhMZuNDXgFCufyPdsSxGVUykCfwUNLfXGmQuMRnoSJGaOHdCKzEwYLMTPWUzZLTYaRipmrqjgabydkNOBtKDReLlaTkTpjUhEcpcnfeAmaJsBxRTUtZvrdIYQgipXqdCCEzeqyBocXrEMijYsgeacohCbGEEzxnkwCYZLXuNsqPocrxauMruo"), false);
    this->JthlbYkNnTZtr(false, -216782117, true, string("LwLGFkZxjZnviStNRrteYTNHVWcNbHvBQehzcueqRzixqfCDuqZsZFCLogbtXjTAUYxDgTgepmx"));
    this->JLjhrTpN(96691.60192583333, string("WUtgzKUYmwcloOqQVdPvhzWSgWduNSKydMVRqhcWEGtfrhUAWJUNBWBukKpdqNlJUfhJetTqvaGWeTYMghKLRypeXMUmcBmxwRzQsOAxCBC"), 579944.5327707825);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zGMtVxz
{
public:
    int wNbkI;
    double tFLMUXIi;
    double PrkoZlx;
    double BvVFHRJ;
    double GdTjOqZ;
    double hGpknObiiLCU;

    zGMtVxz();
    int BKmhhQUErzz();
    void PCgBNXojG(double PwQAaHTxpsFFhCk, double NLfrfp, double uIPVUFoTDBdvS);
protected:
    bool FWpJbCHCOdz;
    int mFOwSPYHlGe;
    string VTineoy;
    bool OPxqctpj;
    double NUxrloKOIR;

    double msdTs();
    void KPMwZjnSBETWWPj(bool kQcQYz, string wWuinXDoElYc, bool LDUNhomDOAxXwZ, int lmbtR);
    double qiABscKoJwi(double lOyUcuBuj, double wqQouafTmcagZNJ, bool surLJx, int gkZtVSgCvif);
    double gSbaMwnJSwCCBJ(double HejnfNGHIgd, int ZYmNxEBgbRYpeMk, int VkeNybVzieR, bool HvHgfcaozFlNz, double hrzjOyushGe);
    string nUrpqTzkGsADi(double safzTwNfWdXRHMv, bool FpuykbNi, string IDpXOcRtgzsOmc, string fJenATiI, bool ZwBEXRoAzfU);
    int StjpgF(int UyQyhAGm, bool wsLFYnzfht);
    int yeccsoncQitOAm(double AjwpJi);
private:
    bool prHaqinLgLYAZDS;

};

int zGMtVxz::BKmhhQUErzz()
{
    string svNAxKDauXeg = string("CcUNLfjrkElCrUSHaeYeUTHcfmBgrgfwhXkRUivPbAXGJRJPRJUxfDfjMDzPcmRGaCRcGRKMzqggrWsbGfjRrOGfZEfTqdgmNFSytaUGZsIJLdmMUhSaKggkkXGCQgnDmMZKMeToIctoBiVPoxhRmtiLZjqNBvQSWVGeOEmffIRxdVuoQPBKdaqLTZhzzRmDGrnUjwaTisfbKiEUzxCNdCnFEkbgsnAykWAAqUPyMjNjcdzGcPsUFJXTEUHEXd");
    int PJCfkzYi = -9501945;
    double ZBXWoQUeyT = -637115.9921508653;
    double IAsaau = 616770.1180067416;
    int QFGTfbYueVrx = -182742627;
    bool ErVWLCFrbPZZk = true;
    double NBfShsgkZAfQo = 391877.79410988156;

    for (int pwHSLX = 397583550; pwHSLX > 0; pwHSLX--) {
        NBfShsgkZAfQo = ZBXWoQUeyT;
    }

    if (QFGTfbYueVrx <= -9501945) {
        for (int SvouuspWa = 1905202391; SvouuspWa > 0; SvouuspWa--) {
            QFGTfbYueVrx *= PJCfkzYi;
            ZBXWoQUeyT = IAsaau;
        }
    }

    for (int QYHdeiARBnXGON = 372395194; QYHdeiARBnXGON > 0; QYHdeiARBnXGON--) {
        PJCfkzYi -= QFGTfbYueVrx;
        ErVWLCFrbPZZk = ! ErVWLCFrbPZZk;
        ZBXWoQUeyT /= NBfShsgkZAfQo;
    }

    return QFGTfbYueVrx;
}

void zGMtVxz::PCgBNXojG(double PwQAaHTxpsFFhCk, double NLfrfp, double uIPVUFoTDBdvS)
{
    int ayMCPuChj = 965926692;
    int fweTFBh = 85540653;
    bool PoJJKqBixViyOoOG = false;
    string riCcqBukHxPRfAi = string("lMzdMBGSWknddORAgImPoWHKpXyTMKfYmFajgYqGleBnPRiCLWVjsMRxlLzdUbfEmtMsCNNmXO");
    bool IEoSrSXuhuthJPlN = false;
    int RhdNQGhTlBnF = 829776870;
}

double zGMtVxz::msdTs()
{
    double pTQIYCBMYAe = 333373.1593116649;
    double mocbXjFiWW = -75919.65138642609;

    if (pTQIYCBMYAe <= -75919.65138642609) {
        for (int TAXPNsqGEjQS = 1849258000; TAXPNsqGEjQS > 0; TAXPNsqGEjQS--) {
            mocbXjFiWW = mocbXjFiWW;
            pTQIYCBMYAe -= pTQIYCBMYAe;
            pTQIYCBMYAe *= mocbXjFiWW;
            pTQIYCBMYAe += mocbXjFiWW;
            pTQIYCBMYAe -= pTQIYCBMYAe;
            mocbXjFiWW = mocbXjFiWW;
        }
    }

    if (mocbXjFiWW == -75919.65138642609) {
        for (int EbbRkJODnYkiZnM = 511443750; EbbRkJODnYkiZnM > 0; EbbRkJODnYkiZnM--) {
            mocbXjFiWW /= mocbXjFiWW;
            pTQIYCBMYAe = pTQIYCBMYAe;
            pTQIYCBMYAe -= pTQIYCBMYAe;
            pTQIYCBMYAe += pTQIYCBMYAe;
        }
    }

    if (pTQIYCBMYAe < 333373.1593116649) {
        for (int IDHsNh = 792266369; IDHsNh > 0; IDHsNh--) {
            mocbXjFiWW += mocbXjFiWW;
            pTQIYCBMYAe *= pTQIYCBMYAe;
            pTQIYCBMYAe += pTQIYCBMYAe;
            pTQIYCBMYAe += mocbXjFiWW;
            pTQIYCBMYAe = mocbXjFiWW;
            mocbXjFiWW /= pTQIYCBMYAe;
            pTQIYCBMYAe -= mocbXjFiWW;
            mocbXjFiWW /= mocbXjFiWW;
        }
    }

    if (pTQIYCBMYAe == 333373.1593116649) {
        for (int fDYHlQbAeD = 174360621; fDYHlQbAeD > 0; fDYHlQbAeD--) {
            pTQIYCBMYAe = pTQIYCBMYAe;
            pTQIYCBMYAe += pTQIYCBMYAe;
        }
    }

    if (mocbXjFiWW <= -75919.65138642609) {
        for (int rCexWIUaYE = 239316415; rCexWIUaYE > 0; rCexWIUaYE--) {
            pTQIYCBMYAe -= mocbXjFiWW;
        }
    }

    return mocbXjFiWW;
}

void zGMtVxz::KPMwZjnSBETWWPj(bool kQcQYz, string wWuinXDoElYc, bool LDUNhomDOAxXwZ, int lmbtR)
{
    string DPXpenizSzIbKzE = string("EVdvKYwetmRdqSFVeletnuHZAaxJKmFdbnJeXGlnnKcimoW");
    string RiIhdi = string("MHUDYplSqpYgxYrhuXWNjzPuIXbKckQasLUuYpTNdPtkZMyzCRslmUzQphPrtQjeitcHhFTVVEOcXuJsnXgZfZrBZbpOZuAuQuKixQwsAUvumFufVbHAwfpfAIjQANZDcgephmYNrDJxDuRcAteNKpXvnjcHJrguTrwVKKMtaYXZs");

    for (int heaqgLLtLugMeCZ = 392742560; heaqgLLtLugMeCZ > 0; heaqgLLtLugMeCZ--) {
        wWuinXDoElYc = RiIhdi;
        RiIhdi += RiIhdi;
        LDUNhomDOAxXwZ = LDUNhomDOAxXwZ;
    }

    for (int uCFmQxvlJ = 353200645; uCFmQxvlJ > 0; uCFmQxvlJ--) {
        continue;
    }

    if (wWuinXDoElYc >= string("MHUDYplSqpYgxYrhuXWNjzPuIXbKckQasLUuYpTNdPtkZMyzCRslmUzQphPrtQjeitcHhFTVVEOcXuJsnXgZfZrBZbpOZuAuQuKixQwsAUvumFufVbHAwfpfAIjQANZDcgephmYNrDJxDuRcAteNKpXvnjcHJrguTrwVKKMtaYXZs")) {
        for (int TwQEGLCpUugLdX = 1732273545; TwQEGLCpUugLdX > 0; TwQEGLCpUugLdX--) {
            wWuinXDoElYc = RiIhdi;
            DPXpenizSzIbKzE = RiIhdi;
            LDUNhomDOAxXwZ = ! kQcQYz;
        }
    }
}

double zGMtVxz::qiABscKoJwi(double lOyUcuBuj, double wqQouafTmcagZNJ, bool surLJx, int gkZtVSgCvif)
{
    bool MGHTcf = true;

    for (int eKtnPIBDHGiICl = 1169639487; eKtnPIBDHGiICl > 0; eKtnPIBDHGiICl--) {
        MGHTcf = ! surLJx;
    }

    if (wqQouafTmcagZNJ > -181873.98257048542) {
        for (int gtxITZLCrCmiHcmD = 631309636; gtxITZLCrCmiHcmD > 0; gtxITZLCrCmiHcmD--) {
            lOyUcuBuj /= wqQouafTmcagZNJ;
            lOyUcuBuj += wqQouafTmcagZNJ;
            lOyUcuBuj /= lOyUcuBuj;
        }
    }

    for (int XxcODUMXZmJDVh = 546821809; XxcODUMXZmJDVh > 0; XxcODUMXZmJDVh--) {
        surLJx = surLJx;
        lOyUcuBuj = lOyUcuBuj;
        wqQouafTmcagZNJ = wqQouafTmcagZNJ;
        lOyUcuBuj += wqQouafTmcagZNJ;
        lOyUcuBuj = wqQouafTmcagZNJ;
    }

    for (int SxKTyBNVGa = 342123486; SxKTyBNVGa > 0; SxKTyBNVGa--) {
        surLJx = MGHTcf;
        MGHTcf = ! surLJx;
    }

    return wqQouafTmcagZNJ;
}

double zGMtVxz::gSbaMwnJSwCCBJ(double HejnfNGHIgd, int ZYmNxEBgbRYpeMk, int VkeNybVzieR, bool HvHgfcaozFlNz, double hrzjOyushGe)
{
    double WMPWzAoo = -837913.7350041326;
    string gVPTnGApQc = string("uuYsfWoulVtwFXQVyFfpProlqTnNUGStCBIHLFKUPkUHrmJGmvcNUnphPDFUSndfntfDpuXWBAcDrTXPwUlXXWyKGHGpDvYyovuNBm");

    if (ZYmNxEBgbRYpeMk >= -631476559) {
        for (int spSKskS = 1014232145; spSKskS > 0; spSKskS--) {
            gVPTnGApQc += gVPTnGApQc;
            ZYmNxEBgbRYpeMk -= VkeNybVzieR;
        }
    }

    if (WMPWzAoo > -117263.06546936085) {
        for (int XidvdvHBIIUgRr = 3974145; XidvdvHBIIUgRr > 0; XidvdvHBIIUgRr--) {
            WMPWzAoo = WMPWzAoo;
            hrzjOyushGe /= WMPWzAoo;
        }
    }

    for (int UmqSlkCRHyPHGU = 703292768; UmqSlkCRHyPHGU > 0; UmqSlkCRHyPHGU--) {
        hrzjOyushGe /= HejnfNGHIgd;
    }

    return WMPWzAoo;
}

string zGMtVxz::nUrpqTzkGsADi(double safzTwNfWdXRHMv, bool FpuykbNi, string IDpXOcRtgzsOmc, string fJenATiI, bool ZwBEXRoAzfU)
{
    string JNVXseouoInF = string("FYNlMEfDFMnOVrInVaEYDBlMLalMIfSjogppvBSrHTPAsQjmPGtZwjaLAoJMhEeyEUZEFbvfMCYTeCDprTkbVLYkGxanNBKDvqQDByNPeXhOREavXVBIeGvbSAHtHSUpPdMolBCeJTmujbFPdMwdPucWFLdnlVOvuxAcjqtjPYSlJgARDQdsUjZokoBgvOvEERPxTDoMHNgIxjeYWqmstKIiRdteaOPuwwQ");

    if (IDpXOcRtgzsOmc <= string("FYNlMEfDFMnOVrInVaEYDBlMLalMIfSjogppvBSrHTPAsQjmPGtZwjaLAoJMhEeyEUZEFbvfMCYTeCDprTkbVLYkGxanNBKDvqQDByNPeXhOREavXVBIeGvbSAHtHSUpPdMolBCeJTmujbFPdMwdPucWFLdnlVOvuxAcjqtjPYSlJgARDQdsUjZokoBgvOvEERPxTDoMHNgIxjeYWqmstKIiRdteaOPuwwQ")) {
        for (int JTeUhErIpPs = 1522299161; JTeUhErIpPs > 0; JTeUhErIpPs--) {
            IDpXOcRtgzsOmc = JNVXseouoInF;
        }
    }

    for (int HJNCAF = 1703829844; HJNCAF > 0; HJNCAF--) {
        IDpXOcRtgzsOmc += IDpXOcRtgzsOmc;
    }

    for (int XecXR = 789833470; XecXR > 0; XecXR--) {
        JNVXseouoInF = JNVXseouoInF;
    }

    return JNVXseouoInF;
}

int zGMtVxz::StjpgF(int UyQyhAGm, bool wsLFYnzfht)
{
    string EJfDerwlbS = string("jGnhHLWmoHsfdQUpFoVPaXxUJHEdvxJScoXUlJowHptTDzikYnCzFUjOjsEsktpvmIUhFAVZKaNHtBToGgkKXWcTKFfbsHJAPTQAvmyehJwTGtvJlWAhvAdXUMZfCXipUwTUzaqlhwlbwJRcNqeFkTWSTfywWRqjEcWxaoNyCXz");
    bool xQKKKOkJJeHAdMgr = true;
    string YmuTem = string("svPQKnOjsEhHBsvLIOxvediXCYkBuXjBgxruFOohenHQGgwyeEYjaFgDfBeGXXIJMMMuqyOWclbcykTMnyQmYqJETyNeALWhKsYexQZGbdjvgyzFfFkXdsAtwJVGseEpkwzOphfkeuRDxaeKgzWXFVoNHQIIicJBlVl");
    double SWgxTNsDg = 121434.14514395667;
    int cTKsc = 302248368;
    string qMyaRcBBfXFO = string("RmnJApUUlCMOZVvhxPzalRIzjEdAJLTemtaAuCjedSaNwjxYZRqFClpzhSlTxhSnJdcBkFvaolTaVldgSLWaofwxtDbLcmNduDdRLESyTPcUqNdVLBRNIyWAujwUyveZlzGeWKQoBukDOHeXnZBDcVDChEmDHWlKPczcQEwzVtQCFPIsYTQXHcZrOarWimSqtQVfuFVlYUvMdNbOSFUISHWBmcFMxXhhJLCSB");
    int RrcHdVkFApjJPvd = 1006767561;
    bool qGGMjuDlzYlgQ = true;
    double bGDFqJ = -1037339.331628788;
    bool OOozTPxarneqr = true;

    return RrcHdVkFApjJPvd;
}

int zGMtVxz::yeccsoncQitOAm(double AjwpJi)
{
    int EnbgBaNm = 592287964;
    int aYBCLfQoFvCw = 816090524;
    bool XwkfIor = false;
    double MIwINAOLPAH = -374132.1750668941;
    string PqnzZedRugqmmt = string("GLmYVtgWugMYMptCIXmXQwaVDmulbmCihXKBDLCohbajXCLsaxdlrpuuejRqGsEXhv");
    bool ImjthLmJm = true;
    int YCZDYYm = -1016251062;

    if (EnbgBaNm >= -1016251062) {
        for (int fZYIVTrwbBQ = 1159126444; fZYIVTrwbBQ > 0; fZYIVTrwbBQ--) {
            YCZDYYm *= EnbgBaNm;
        }
    }

    return YCZDYYm;
}

zGMtVxz::zGMtVxz()
{
    this->BKmhhQUErzz();
    this->PCgBNXojG(961133.5774693884, -447959.9194922878, -27370.693846035705);
    this->msdTs();
    this->KPMwZjnSBETWWPj(false, string("xXWkMWaShkeaIFIoCpvYxZYKKKxKhoccwGFtrlYAMIVtxAKbjUueTRDiwJzLMvTWgOHLFnvGmWTnksdfVsEgwLMehmqOxsZyhYVbrOEDnCrAJPZzXzAyCJHsJhTffbCJiybUaeTsoqOtEmvfRwjFyYNpSiXHrFnHhOHrDkRdYiNNGHVOQsZGNoLjtbkABKteuqOUqTzsgisbqoAZoGpLLONCOM"), true, -1276622003);
    this->qiABscKoJwi(-837609.6510679233, -181873.98257048542, true, 859825197);
    this->gSbaMwnJSwCCBJ(-117263.06546936085, 190692094, -631476559, false, -513791.7949850331);
    this->nUrpqTzkGsADi(-405067.7859997252, true, string("oKrPLPfcAedCUokAITYj"), string("awfwcitxxucEKIPJzrYCorQZOgXeoHVTtfEorPRVyZcUFvriCtyiVzbKpEwkQhHvlINCpIXOVVicUaMAgDdeHNnRONAGvmcvCtUYHFEHsnHvyCkkYNSUv"), true);
    this->StjpgF(771389016, false);
    this->yeccsoncQitOAm(464743.97022100864);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BlqcAJyc
{
public:
    int qXjQuk;
    bool cXzBsxwas;
    double sqBsbYovdc;

    BlqcAJyc();
    void hNYRRLCBpkr();
    bool GHghJoBUy(double kkxYFdFOtWnmx);
    double JnFUZhwHDDERj();
    string WyaUDwrCDxsM(string nYfPXnAy, bool ZHfPlGcCFbPq, string PXbBCPsDlBfiuXkE, int QXFpHElWERm, bool sNBrvvWyfb);
    string dFosfTfwLCOuE();
protected:
    string cGFxlbmDYpB;

    bool sjqykoMESA(string kHeGnWFMGOUUHrJ, double iipJPcSsTxKIVfD, bool hJTlPbUkPYrHf, bool iwbUopuKPRx);
    void kDPKvlqwNQSrWlC(string HpNBxhuLEcpxqwFf);
    void bRsZphZnV(bool nHKPuohHyEt, bool MPZxTcgmO);
    bool obGTbLCebOoPs(string ongkW, string QQFaSpYgbz);
    string iRdsMLWKBwj();
private:
    int xHxdAXn;
    bool GaNyLGQ;
    bool dwQiYltTdaaW;
    bool JDDYGvIRXJHHiFcC;

    double laLLXW(bool AywLfcUPuD, bool oGLlLQfaG, int KPDBqFTXzqH, bool LJJHDyEY, int wHiuxJQePOlQ);
    double DBeOEWjaUcu();
};

void BlqcAJyc::hNYRRLCBpkr()
{
    double bNgOVcjYjpChZf = -887269.7680198274;
    double OPNdHoRlxWQSE = 282047.4673268065;
    double voTVfMdvDuaSppz = 1020739.664959601;
    bool LqelDeSrEGPqZ = true;
    int kvkVvaWfF = -2036627887;
    bool SmDsbdHXR = false;
    string ShOBmh = string("uRdgGfghQDotRnjJQpkPFdateBRZbQGlOcjEfrGEUMlXPBXDuGEONgEaYPiQPuyNWU");
    string BRRfkhrCcby = string("RwbUFsCuEFYUoCsNWqdLtDVJmZHuyudcfMIHmFpOebtmeVpBgSPMQsWoaUO");

    for (int NuBHuyguTkeiMG = 1716017852; NuBHuyguTkeiMG > 0; NuBHuyguTkeiMG--) {
        bNgOVcjYjpChZf /= OPNdHoRlxWQSE;
        OPNdHoRlxWQSE -= voTVfMdvDuaSppz;
        LqelDeSrEGPqZ = ! SmDsbdHXR;
    }

    for (int rVctq = 1164297089; rVctq > 0; rVctq--) {
        ShOBmh = ShOBmh;
        OPNdHoRlxWQSE = OPNdHoRlxWQSE;
        BRRfkhrCcby = ShOBmh;
    }

    for (int kfZJNbKsfVLhMqzy = 879058364; kfZJNbKsfVLhMqzy > 0; kfZJNbKsfVLhMqzy--) {
        SmDsbdHXR = SmDsbdHXR;
        OPNdHoRlxWQSE /= bNgOVcjYjpChZf;
        ShOBmh += BRRfkhrCcby;
        SmDsbdHXR = SmDsbdHXR;
    }
}

bool BlqcAJyc::GHghJoBUy(double kkxYFdFOtWnmx)
{
    int jlqFVYX = -576560712;
    int LZPZhqctuhci = 1529440838;
    int acwKfmDqkDlaM = -1897450776;

    for (int yvtWYOGkkEZV = 447800222; yvtWYOGkkEZV > 0; yvtWYOGkkEZV--) {
        acwKfmDqkDlaM -= LZPZhqctuhci;
    }

    return false;
}

double BlqcAJyc::JnFUZhwHDDERj()
{
    int DHArrbn = -1608984165;
    bool kQiMylLYUosl = false;
    string SFlvwFNFDDEsWJmm = string("xrFpSyVJJTjXDkqykAOlQFsTQrGSDLLWZTIfBjXgBVZZKomtDSNNKQtNSTpxmoqjmBSPnETCtuz");
    double GIuHfWHczZvDmau = -555028.1918453748;

    for (int BOgxIv = 1657900956; BOgxIv > 0; BOgxIv--) {
        continue;
    }

    for (int cmyFVxh = 295786558; cmyFVxh > 0; cmyFVxh--) {
        continue;
    }

    for (int jGbRxJKBgLi = 155259706; jGbRxJKBgLi > 0; jGbRxJKBgLi--) {
        SFlvwFNFDDEsWJmm = SFlvwFNFDDEsWJmm;
        GIuHfWHczZvDmau += GIuHfWHczZvDmau;
        DHArrbn = DHArrbn;
    }

    for (int QnGDdne = 363016978; QnGDdne > 0; QnGDdne--) {
        GIuHfWHczZvDmau *= GIuHfWHczZvDmau;
    }

    if (SFlvwFNFDDEsWJmm >= string("xrFpSyVJJTjXDkqykAOlQFsTQrGSDLLWZTIfBjXgBVZZKomtDSNNKQtNSTpxmoqjmBSPnETCtuz")) {
        for (int vGILoBZycg = 1990236658; vGILoBZycg > 0; vGILoBZycg--) {
            kQiMylLYUosl = kQiMylLYUosl;
            GIuHfWHczZvDmau *= GIuHfWHczZvDmau;
            SFlvwFNFDDEsWJmm += SFlvwFNFDDEsWJmm;
            GIuHfWHczZvDmau -= GIuHfWHczZvDmau;
        }
    }

    for (int KATRVkJmFnQ = 205675639; KATRVkJmFnQ > 0; KATRVkJmFnQ--) {
        DHArrbn *= DHArrbn;
        SFlvwFNFDDEsWJmm = SFlvwFNFDDEsWJmm;
    }

    for (int EFdNnjhJi = 1603019311; EFdNnjhJi > 0; EFdNnjhJi--) {
        DHArrbn /= DHArrbn;
    }

    return GIuHfWHczZvDmau;
}

string BlqcAJyc::WyaUDwrCDxsM(string nYfPXnAy, bool ZHfPlGcCFbPq, string PXbBCPsDlBfiuXkE, int QXFpHElWERm, bool sNBrvvWyfb)
{
    double FxyEZSJAFsunJ = 255013.4164466331;
    string GLjwteYCqDpgl = string("FLMRuuCYPdoCvXGAClaztFDiwfFrdhWfRsKETRTHMmSSxoZAhLJnGBgHGbYJvGFJXzgWBdRBjkgsOZRTlrMcNDySyACnNfsMYaJxDMMulrCzvwGzGLTaDQGETNFYYQtphavxYU");
    int DQdwPVrBhJsm = -883908100;

    for (int iFtKzTAl = 1075662874; iFtKzTAl > 0; iFtKzTAl--) {
        ZHfPlGcCFbPq = ! sNBrvvWyfb;
        nYfPXnAy += PXbBCPsDlBfiuXkE;
        QXFpHElWERm -= QXFpHElWERm;
        ZHfPlGcCFbPq = ! sNBrvvWyfb;
    }

    for (int fFPmKPxnGHFVZhde = 1386340820; fFPmKPxnGHFVZhde > 0; fFPmKPxnGHFVZhde--) {
        continue;
    }

    for (int AXnsOt = 933613578; AXnsOt > 0; AXnsOt--) {
        PXbBCPsDlBfiuXkE = PXbBCPsDlBfiuXkE;
        GLjwteYCqDpgl = PXbBCPsDlBfiuXkE;
    }

    for (int EpWGzVuPuZYBM = 948817497; EpWGzVuPuZYBM > 0; EpWGzVuPuZYBM--) {
        ZHfPlGcCFbPq = ! sNBrvvWyfb;
    }

    if (PXbBCPsDlBfiuXkE != string("lzdRinuYzpbbANMhqjlDZLzZqQSHaVjQnnUcxdwiUyFhNkyIqJhUwjwyWjlvrRqEsznyNfNXwjVbTUXJXSorZ")) {
        for (int cqJDioHztbr = 685379818; cqJDioHztbr > 0; cqJDioHztbr--) {
            GLjwteYCqDpgl += PXbBCPsDlBfiuXkE;
        }
    }

    return GLjwteYCqDpgl;
}

string BlqcAJyc::dFosfTfwLCOuE()
{
    double lcBlBxEhtV = -75936.28537066391;
    double uPkaHKpOm = 289302.78255954594;
    bool bwLdd = true;
    string UVWVxPVkBIlw = string("pZuwQYIEDnlIwoATWjHBDtDyLpwSlaDdTRVhPKnxzOJJAMWEFpgmdSpGcZzebTZjmNbniCgcaOQKagUTjeVoSidONlpdMHsrrlUhzGtkVrKqeXSzNBumCCbdLwIHoWfsVkKYqzMqfNSzDLcGEJixGuPWLuWpFWGDcGsnqrWqLzxfNoMQlsRmiPQuzdDqIqRiuvfkiaRpPSclwtPtWQTzFHQLUQcwUKcJnEWfeZlBzOdufcolXwJheriaJlOngaG");

    for (int IvabrQxIvS = 1291695580; IvabrQxIvS > 0; IvabrQxIvS--) {
        continue;
    }

    for (int DlnrFZ = 614118278; DlnrFZ > 0; DlnrFZ--) {
        lcBlBxEhtV += lcBlBxEhtV;
        UVWVxPVkBIlw = UVWVxPVkBIlw;
    }

    return UVWVxPVkBIlw;
}

bool BlqcAJyc::sjqykoMESA(string kHeGnWFMGOUUHrJ, double iipJPcSsTxKIVfD, bool hJTlPbUkPYrHf, bool iwbUopuKPRx)
{
    string cXxHFrJIeWgqu = string("unWDQNsabTNbeuBdrSiVaULZbFWiJVsxUgfPmSJSBfZaWPCkrGWJQlidrrskcvhoTMFzAvqJqCYTTTWfBACPKspgubdejWJULeDLcpIRLPYIfYtjwNmLXDiFMzhpcgoMePObTIoDeFhABaPszDrTSsRxcszkxjXIykFCjHIRMVoHFenhbNWkpDfkftmNuGExVpDOvNBEcZHTwlxWBLJIBAbwKOSnrBkzbeFHfVkpetjhAEOpRmuaceLovK");
    int xKFsQfVuo = -1972733870;
    double qZssGqLNKrf = 382214.6471896056;
    string irdONHx = string("jooQvYdiaGtOhZCfufRavtaJWSwWMGakTYlCJBckHCNQdQSpCsXMCJYqULKZXSQPHfDgccxKgljnEHfRJeKjAlYZUYTelnrbu");

    if (iwbUopuKPRx == false) {
        for (int MtIBcsORmSgTLGla = 64620837; MtIBcsORmSgTLGla > 0; MtIBcsORmSgTLGla--) {
            continue;
        }
    }

    if (kHeGnWFMGOUUHrJ != string("sNlKrffsjbjewSnSJCbeptiovtGkuYhRILFnMfIQmeeOFqnfctHrbvvhZZWoPTzZlCrCYrTfPiOvCETyIOIjuNbbwqJdYdskhqsNDIUQySrgiDQHTSjMVqHAhUGSnTCUiRXUBMAjjGWGzwPHSjeoyihQtiqaWLgbIkHrJKZDCZAVaCFsPoopJPwrBlZZouGbGpcLGAmzIGpJEJnJIsIwdXidXaOJNubWbzr")) {
        for (int ZeXmbxejcCtblDhq = 2010247603; ZeXmbxejcCtblDhq > 0; ZeXmbxejcCtblDhq--) {
            xKFsQfVuo += xKFsQfVuo;
        }
    }

    for (int LBajU = 1764444560; LBajU > 0; LBajU--) {
        iipJPcSsTxKIVfD /= iipJPcSsTxKIVfD;
    }

    for (int jGocSqRVRA = 1118497056; jGocSqRVRA > 0; jGocSqRVRA--) {
        continue;
    }

    return iwbUopuKPRx;
}

void BlqcAJyc::kDPKvlqwNQSrWlC(string HpNBxhuLEcpxqwFf)
{
    bool CrNcqJUlyKrfiX = false;
    double JYGgMQfGJE = 807986.9440716448;
    double huSRzaAhqhslXLi = 717540.0049622273;
    double tqGZEcDNJvIipHHe = 205013.53213849498;

    for (int CNaNny = 1945853734; CNaNny > 0; CNaNny--) {
        tqGZEcDNJvIipHHe += JYGgMQfGJE;
    }

    if (JYGgMQfGJE >= 807986.9440716448) {
        for (int NUBizHZ = 1320481848; NUBizHZ > 0; NUBizHZ--) {
            JYGgMQfGJE /= JYGgMQfGJE;
            JYGgMQfGJE -= tqGZEcDNJvIipHHe;
            JYGgMQfGJE *= JYGgMQfGJE;
            HpNBxhuLEcpxqwFf += HpNBxhuLEcpxqwFf;
        }
    }

    if (tqGZEcDNJvIipHHe > 205013.53213849498) {
        for (int HJonOIJKAgnj = 1064594528; HJonOIJKAgnj > 0; HJonOIJKAgnj--) {
            tqGZEcDNJvIipHHe = tqGZEcDNJvIipHHe;
        }
    }

    for (int jaQjuEsFkUNyK = 1984676352; jaQjuEsFkUNyK > 0; jaQjuEsFkUNyK--) {
        huSRzaAhqhslXLi = tqGZEcDNJvIipHHe;
        JYGgMQfGJE += tqGZEcDNJvIipHHe;
    }

    for (int WXDGCXAe = 1647303204; WXDGCXAe > 0; WXDGCXAe--) {
        tqGZEcDNJvIipHHe *= huSRzaAhqhslXLi;
        tqGZEcDNJvIipHHe -= JYGgMQfGJE;
        huSRzaAhqhslXLi = JYGgMQfGJE;
    }
}

void BlqcAJyc::bRsZphZnV(bool nHKPuohHyEt, bool MPZxTcgmO)
{
    string SkmowPbezlBrrlwK = string("BPPDcMTbRtStmESgHllambBOFefAHYmfHRmpdzleeZksjbqoKUjVytDmshYtrrLITeMmWnIhhQNVezauGxfRDuMNlXskKFHvjuqQSxgJFHMJFKyKCRzqVZnKBLtDGnlUWzADGzGJvjqvNyBmGxDaFwIfy");
    string CidljPtZ = string("xZYZSHKOLjIzoMRXPKxtrYEKaAMmnGlaIGhieSuekxAGrntTBnenBzViVLtAprTTnetvSqkXZwoxNrqSQuLBEXyMotENNwvQBDiACDIyEQceWJhsiThowUEabsLheeVafssupIWAE");
    string QNPXLUq = string("KWVRPvrRYKjMKkOLmykuRDXPzvhGfoLRwuYPlSeJKacgZWXPNBLuImEwIzlppwshRZBDXpcaNbxpmehuVeVxvVjBjMPuMaqbsXGjOUMwXKFGLcltNyckbCBTApusQmFjtwhpHcUBuDyPATiloKLMYTewNZyLjl");
    double RhIBLggxUp = -201766.1470070592;
}

bool BlqcAJyc::obGTbLCebOoPs(string ongkW, string QQFaSpYgbz)
{
    double UNCZRdT = -92386.33610343639;
    string BYKRRPxe = string("VHaPWRMEvWnyniNrpCliT");
    double jRVLtAFKGzmk = 661184.7528623204;
    string ielMaFJCtzoomRI = string("FLMWRDVpeRvOYyOOYFzgVpcObKGKMKGBwQjKgVFtXTmliRhjNsvOddaAqVJmEHczsYFHTpBKZIOmUWqSyQqPPNnOygRsgIVLOuGOsVNkdHriDNxeCMltfKlwJKwmovAxsnOFcoTezVNwQeOQuBeueCAobyoiGHGHBrTMmwRuInlLRsSwhQdDJedtCSshmtNFGgqhrFceyqhEuKlxdyTpeDmZTFQbHObKhXPuHultAPKOoaNSOpcXlqWpHwYNoj");
    bool qJXTiqYs = false;
    double EGjfsPwTSM = 595400.9941751034;
    string ZeiZRab = string("ayAYbgYYqQDJCJfobTEiUiDBEBBEnAsYmM");
    bool jHfwz = true;

    for (int TJFgakvNvigio = 37458281; TJFgakvNvigio > 0; TJFgakvNvigio--) {
        ongkW = ielMaFJCtzoomRI;
        BYKRRPxe += ielMaFJCtzoomRI;
        UNCZRdT = jRVLtAFKGzmk;
    }

    for (int qjaUEvdgdLeZjPv = 2069736631; qjaUEvdgdLeZjPv > 0; qjaUEvdgdLeZjPv--) {
        EGjfsPwTSM /= jRVLtAFKGzmk;
    }

    if (ielMaFJCtzoomRI != string("ayAYbgYYqQDJCJfobTEiUiDBEBBEnAsYmM")) {
        for (int kLceIY = 1330627398; kLceIY > 0; kLceIY--) {
            ZeiZRab += ielMaFJCtzoomRI;
        }
    }

    for (int PAEKUeZdESNuWQtx = 1392679985; PAEKUeZdESNuWQtx > 0; PAEKUeZdESNuWQtx--) {
        QQFaSpYgbz += QQFaSpYgbz;
        qJXTiqYs = jHfwz;
        BYKRRPxe = QQFaSpYgbz;
        EGjfsPwTSM *= jRVLtAFKGzmk;
    }

    return jHfwz;
}

string BlqcAJyc::iRdsMLWKBwj()
{
    int kEuzRC = 1688433125;
    string LgSXrA = string("rejYaTPQwAMzVciibADqbTurMpemmvuYCLJnDrfTCWeZJDf");
    string hejgjiJmFt = string("zpfVGBPEjqpBVjolMShagFroIyqKGqjHustfLKaOTkMlXscbUFvnntRoskMKBZnPlARFMbKfQKhRuTTTzMtOkrHTfJORnLTQrdnahYAtUPMZIWKjGxhrINtrtNJMBiXcEEuIcsVXJnMqKbyjaDeTxqvYFVzUQeaAOqOtJntPcbQYuFICbhyJMNVRGFXYKShcyiiAUUfALqsVVrBVWPXglP");
    int lKduVv = 1531277691;
    string depKZSLUIdVDSWpQ = string("lwaQnbZDNXqSRlsyzz");
    int fZFOqkcUtvH = -1217286102;

    for (int HDyweTopImWyKD = 415263853; HDyweTopImWyKD > 0; HDyweTopImWyKD--) {
        lKduVv += lKduVv;
    }

    for (int LLxqvXDKikHBKd = 48757456; LLxqvXDKikHBKd > 0; LLxqvXDKikHBKd--) {
        kEuzRC -= kEuzRC;
    }

    for (int QUKSjlDHMORCc = 761057401; QUKSjlDHMORCc > 0; QUKSjlDHMORCc--) {
        depKZSLUIdVDSWpQ += depKZSLUIdVDSWpQ;
        LgSXrA += hejgjiJmFt;
        hejgjiJmFt = LgSXrA;
    }

    if (hejgjiJmFt > string("zpfVGBPEjqpBVjolMShagFroIyqKGqjHustfLKaOTkMlXscbUFvnntRoskMKBZnPlARFMbKfQKhRuTTTzMtOkrHTfJORnLTQrdnahYAtUPMZIWKjGxhrINtrtNJMBiXcEEuIcsVXJnMqKbyjaDeTxqvYFVzUQeaAOqOtJntPcbQYuFICbhyJMNVRGFXYKShcyiiAUUfALqsVVrBVWPXglP")) {
        for (int zkCRUVY = 1755362729; zkCRUVY > 0; zkCRUVY--) {
            kEuzRC = kEuzRC;
            fZFOqkcUtvH = kEuzRC;
            LgSXrA += LgSXrA;
            fZFOqkcUtvH += kEuzRC;
        }
    }

    if (fZFOqkcUtvH != -1217286102) {
        for (int pFAqqr = 1109708320; pFAqqr > 0; pFAqqr--) {
            kEuzRC /= fZFOqkcUtvH;
            hejgjiJmFt += depKZSLUIdVDSWpQ;
        }
    }

    return depKZSLUIdVDSWpQ;
}

double BlqcAJyc::laLLXW(bool AywLfcUPuD, bool oGLlLQfaG, int KPDBqFTXzqH, bool LJJHDyEY, int wHiuxJQePOlQ)
{
    int WjetMPwPlWqdy = -957934525;
    bool shQqpfFHP = true;
    double izMZkUiuMiQGTuQV = -723967.4048208589;
    double NKvyVavWyPYSvf = -260085.89090193575;
    int OmPkSt = -805767626;
    string kmfuneHi = string("OpXHxjaUwBrNyHFbsLycOkCKYfTaBnipUWxVuoxtZSZsGRuXpDRhUoFujqqiSETUeHlbYNsKsiadJNUGsGwmzRPyMqBrhZSmqzFiuccWSVmUBDdfbJsQjDTNzvoRWwbRzAmGkRsEBFFVHOaZvNEAYstobcwzDrOx");
    double zdTqpfpj = 995978.0804448323;
    double LEIHPicikyhUEDmS = 424445.9156683498;
    int JwnRwBJNfFCu = 616747936;

    for (int wsmphsqlCFznpzNg = 883541559; wsmphsqlCFznpzNg > 0; wsmphsqlCFznpzNg--) {
        shQqpfFHP = ! AywLfcUPuD;
    }

    for (int qYmnvd = 579198833; qYmnvd > 0; qYmnvd--) {
        zdTqpfpj -= LEIHPicikyhUEDmS;
        shQqpfFHP = oGLlLQfaG;
        KPDBqFTXzqH -= JwnRwBJNfFCu;
    }

    if (AywLfcUPuD != true) {
        for (int ScjMSNtjZm = 87225030; ScjMSNtjZm > 0; ScjMSNtjZm--) {
            JwnRwBJNfFCu = KPDBqFTXzqH;
            WjetMPwPlWqdy -= wHiuxJQePOlQ;
        }
    }

    return LEIHPicikyhUEDmS;
}

double BlqcAJyc::DBeOEWjaUcu()
{
    int WbjWngazbOq = -2103035638;
    string DNQVTuTdvqXt = string("NHpcJAVQUQZSTwdAEJHfkTfePnWlabdMMoYRmVTYdrJO");
    double tQmsWnOdLG = 716580.4790782046;
    string SBKIorCQTeKN = string("jFqkCXizaeSKpkAKNCJcovfdcsllWiKnHlGPznnZiGXreVlSVUFXBHteAEUmuhtotjFMahZfTVaDlKOOcxJgyYOFDbIUUONDBzHLBGrDNkjyVBomwfiBdlZUoegbAyXPeMGxrwhFVlPWngvItdnztGQDtyRAIvAijstowwHzxVYTvmDIARftvVsqdYTEVLceTGBwSDtGYOrzWJCvmTaXMjYkW");
    string eRCMsKibPd = string("VWbOccHZtiZgxCWhccPNCXGtmEmguibwYRNKHNbCsMdckDgAsJTGfYobdWIrevvjnNeybXgFkbOQAhFxYtLUlytyOhdofdmUHLhvlTBliWAeCnuIxBjUjYbdLimmsgmElCZqwAICinXBkdrvFJtNaYETFmCMYRdMprjcXmTMnhNtewRwBEPFuuhohUgQrOhdhNYmdDTDdVwhchwqcUYjEmE");
    int tHYtGXjf = -160773487;
    double BzxJmQwBylQPVqL = -879774.3734069219;
    int giEkPGVWdXzQv = 87191806;
    double GGJLOwDbTs = 945668.290093874;

    for (int kDrMKZPDOH = 1144970494; kDrMKZPDOH > 0; kDrMKZPDOH--) {
        continue;
    }

    if (eRCMsKibPd > string("jFqkCXizaeSKpkAKNCJcovfdcsllWiKnHlGPznnZiGXreVlSVUFXBHteAEUmuhtotjFMahZfTVaDlKOOcxJgyYOFDbIUUONDBzHLBGrDNkjyVBomwfiBdlZUoegbAyXPeMGxrwhFVlPWngvItdnztGQDtyRAIvAijstowwHzxVYTvmDIARftvVsqdYTEVLceTGBwSDtGYOrzWJCvmTaXMjYkW")) {
        for (int PMpegqfyEsxwZaL = 1168091244; PMpegqfyEsxwZaL > 0; PMpegqfyEsxwZaL--) {
            tQmsWnOdLG *= tQmsWnOdLG;
            GGJLOwDbTs -= BzxJmQwBylQPVqL;
        }
    }

    if (giEkPGVWdXzQv >= -2103035638) {
        for (int qhqGDmPsEjalP = 994766844; qhqGDmPsEjalP > 0; qhqGDmPsEjalP--) {
            giEkPGVWdXzQv -= giEkPGVWdXzQv;
        }
    }

    return GGJLOwDbTs;
}

BlqcAJyc::BlqcAJyc()
{
    this->hNYRRLCBpkr();
    this->GHghJoBUy(-858604.2953854452);
    this->JnFUZhwHDDERj();
    this->WyaUDwrCDxsM(string("lzdRinuYzpbbANMhqjlDZLzZqQSHaVjQnnUcxdwiUyFhNkyIqJhUwjwyWjlvrRqEsznyNfNXwjVbTUXJXSorZ"), false, string("sXIyOkIdILwTEDzyrViaJtfKVxQbHQOYaCyIITLXktVHyFSMWVvioTsnaHyjSscMtMXMKcvZNwIHVLRsaRauLbhKpnQmeffhmHTqheHfJRqrpcsZWZkGZJRWckaffoJDUDsEMrmuoMOuTZaxSEEOLkQYhSOkDevJFEaCSvyfr"), -809834027, false);
    this->dFosfTfwLCOuE();
    this->sjqykoMESA(string("sNlKrffsjbjewSnSJCbeptiovtGkuYhRILFnMfIQmeeOFqnfctHrbvvhZZWoPTzZlCrCYrTfPiOvCETyIOIjuNbbwqJdYdskhqsNDIUQySrgiDQHTSjMVqHAhUGSnTCUiRXUBMAjjGWGzwPHSjeoyihQtiqaWLgbIkHrJKZDCZAVaCFsPoopJPwrBlZZouGbGpcLGAmzIGpJEJnJIsIwdXidXaOJNubWbzr"), -870617.6384283688, false, false);
    this->kDPKvlqwNQSrWlC(string("LFGjWMtqZYJQRvzskngqjGMezwUNosIcfiZpMaCohuvoBMBdXUXXQPrauBV"));
    this->bRsZphZnV(true, false);
    this->obGTbLCebOoPs(string("YITweXbEUCTuEufCQldwEHOfpOpTdALAwVxFyhPpvgsEkVoSxUSNJfjPYmtZtprxUzeeeByURaYceYSnfyZAsYjGpRCxoSMtdoSvdxcYVhdcbvxFZfMEqqTIzMoKezWCmsSszUQGDCenpPitDOlJNOcLUsNUbgPvsSUtHE"), string("xzBbmRgxNRWLXKAqdzMdUpFoSSoFJcbZaJJULondBDHBjewCeouHHuUZFZJauWtmeSVdhxghxivGMhxbSfKzCPLtpVELQTMlxdZbrjRQertNkLcevKeJlzryvvPgamuDYBpmPSvGeYRWsVeZFvXJkZfiCOziyVpLvLoXYIkyGDUHrwVMTW"));
    this->iRdsMLWKBwj();
    this->laLLXW(true, false, 512611185, false, 29347329);
    this->DBeOEWjaUcu();
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qksLRzoUwjegDi
{
public:
    int trewZvb;
    bool QaClKliERNJM;
    double qHCDr;
    bool JMIwCLgoWXYmSKla;
    string VhLvSRUqE;

    qksLRzoUwjegDi();
    double lxJcmTZA(bool DJmagYq, bool ofeugVoZS, double ZMjraVyJtV);
    void ZVmhdLf(bool HzdXNVEwdKVrl, string VtvVzA, double TaIIBLKCAMv, int WdoaLtfQUbGnz, double PbnTcTIuahFIE);
    bool izROuRxJwkBTr(double UQTrXCMlJyrWR, string RsMrJKxPegHi, int GnyWKXzYPZwAvvCv);
    bool RoRMBHMDyfga();
protected:
    double GSQHvwo;
    int TezeiqGoJpYW;
    string kRjTbdwmx;

    int vNaumLilb();
    void nfuWX(int WuvrWpqB, bool poIZQjPtTxpiXlAg, string ovSGQatOHJ);
    double jGPqasObRthoq();
    int JOFAvkh();
private:
    double BdyHrn;
    bool wbrMIQJvchZP;
    int ujoFzGMllC;
    double kIAUfQC;
    bool SxArtSbEyTGM;

    void Wvjoe(string NjmfSWkO, bool ImrtAcpxwzalhQTv, string FVOtWVuxxHM, string tFsgwBhIeiv);
    int fFuwDoWA();
    void nUfHr();
    double JAZStaUergeV(double TiQSMJunzuIhDpQ, double SLGfnHQFmTJauu);
};

double qksLRzoUwjegDi::lxJcmTZA(bool DJmagYq, bool ofeugVoZS, double ZMjraVyJtV)
{
    int ERQSRWTBQLBpF = -953643359;

    for (int KzVReWMQHCiK = 1653635263; KzVReWMQHCiK > 0; KzVReWMQHCiK--) {
        ZMjraVyJtV = ZMjraVyJtV;
        ofeugVoZS = ! ofeugVoZS;
        ofeugVoZS = DJmagYq;
    }

    for (int bIOWeddXEFfNYAm = 451798108; bIOWeddXEFfNYAm > 0; bIOWeddXEFfNYAm--) {
        ofeugVoZS = ! ofeugVoZS;
        ofeugVoZS = ! ofeugVoZS;
        ofeugVoZS = ! DJmagYq;
        ZMjraVyJtV *= ZMjraVyJtV;
    }

    for (int mqFijSlYBCqivj = 1555777806; mqFijSlYBCqivj > 0; mqFijSlYBCqivj--) {
        ERQSRWTBQLBpF = ERQSRWTBQLBpF;
    }

    for (int fyIFmRz = 1309685593; fyIFmRz > 0; fyIFmRz--) {
        continue;
    }

    for (int ZHsVnMYwBTVHxsO = 719427536; ZHsVnMYwBTVHxsO > 0; ZHsVnMYwBTVHxsO--) {
        ZMjraVyJtV /= ZMjraVyJtV;
        ofeugVoZS = ! DJmagYq;
        ofeugVoZS = ! ofeugVoZS;
        DJmagYq = ! ofeugVoZS;
    }

    return ZMjraVyJtV;
}

void qksLRzoUwjegDi::ZVmhdLf(bool HzdXNVEwdKVrl, string VtvVzA, double TaIIBLKCAMv, int WdoaLtfQUbGnz, double PbnTcTIuahFIE)
{
    int PBUeYBKgadrRfkX = 1981919481;
    string yPmWW = string("caizUPplVlmWCZZqpkQziGPwaXIlUFBD");
    int srnoBZnfkltcvp = -1204325627;
    string MOqRrbOFaoNArW = string("OJNbvAERvRpUEDmrIagCZQzBvrpZiZzjgNQPscXbFuwhqbeVPRuZrGbTkLcQIncuJmrFsqrqXtRHqLwOqBpxsdNhCzcbwiUHDadYAgqAYtRmvbqusWXFPRHVufaAiONGLcNFrlkuTzpNOmCVHMvJvQANoKoVV");
    bool hJjSZzKo = false;
    bool zyhVmHJYf = true;
    int WBqqyDdA = 449810803;

    if (PBUeYBKgadrRfkX >= 1981919481) {
        for (int cXbZP = 1729065578; cXbZP > 0; cXbZP--) {
            continue;
        }
    }

    for (int OPyGbXdgocYa = 475012143; OPyGbXdgocYa > 0; OPyGbXdgocYa--) {
        zyhVmHJYf = HzdXNVEwdKVrl;
    }

    for (int RBfhPEddGB = 1805922868; RBfhPEddGB > 0; RBfhPEddGB--) {
        MOqRrbOFaoNArW = VtvVzA;
        WdoaLtfQUbGnz = srnoBZnfkltcvp;
        srnoBZnfkltcvp = srnoBZnfkltcvp;
    }

    if (WdoaLtfQUbGnz < 449810803) {
        for (int qlNmYwDoxobabZix = 554130487; qlNmYwDoxobabZix > 0; qlNmYwDoxobabZix--) {
            continue;
        }
    }

    for (int YCPFyOcvfQ = 2094740352; YCPFyOcvfQ > 0; YCPFyOcvfQ--) {
        MOqRrbOFaoNArW = MOqRrbOFaoNArW;
        WdoaLtfQUbGnz += WdoaLtfQUbGnz;
    }

    for (int uVPGtpAd = 1666373293; uVPGtpAd > 0; uVPGtpAd--) {
        continue;
    }
}

bool qksLRzoUwjegDi::izROuRxJwkBTr(double UQTrXCMlJyrWR, string RsMrJKxPegHi, int GnyWKXzYPZwAvvCv)
{
    bool rOEDCCUAzkpL = false;
    bool LFAeJ = true;
    int ZFyvJUG = -1249644109;
    double WXbBXVPpbUqU = -718419.2210603475;
    string oFtWRRwRGQu = string("WpdMnPxxzDxCTnqIJRUeqwkMOSFBsdEbRzgEnhuoeQzBvMymEeTrwJyPrbhUXcFcBIBqrFPMbbCeFHrkiwyYTvsOXUfMyEmLnxYmonkFCoFstkvHtS");
    bool rZjbbxGSS = true;
    int KtraibkyDkHYwD = -1536444800;
    double omyDQPFxC = -772967.7641540932;

    for (int IYxoha = 1078314385; IYxoha > 0; IYxoha--) {
        oFtWRRwRGQu += oFtWRRwRGQu;
    }

    for (int ExQJCqahaEf = 601863038; ExQJCqahaEf > 0; ExQJCqahaEf--) {
        UQTrXCMlJyrWR = omyDQPFxC;
        rZjbbxGSS = ! rOEDCCUAzkpL;
    }

    for (int BPADdNBSVO = 578013502; BPADdNBSVO > 0; BPADdNBSVO--) {
        continue;
    }

    for (int TYJXuCAxRDaTvLm = 857965481; TYJXuCAxRDaTvLm > 0; TYJXuCAxRDaTvLm--) {
        LFAeJ = ! LFAeJ;
    }

    return rZjbbxGSS;
}

bool qksLRzoUwjegDi::RoRMBHMDyfga()
{
    int DWFNBRWYPNoye = -1524323231;
    bool wTmLBAfb = true;
    bool AGzQu = false;
    double JctTch = 495633.6173071478;

    if (wTmLBAfb == false) {
        for (int HeOJukRhw = 738558432; HeOJukRhw > 0; HeOJukRhw--) {
            DWFNBRWYPNoye /= DWFNBRWYPNoye;
            wTmLBAfb = AGzQu;
            DWFNBRWYPNoye /= DWFNBRWYPNoye;
        }
    }

    return AGzQu;
}

int qksLRzoUwjegDi::vNaumLilb()
{
    bool ZObKpuKk = false;
    bool RTTPCfodFpCXRVrY = false;
    int JODosGZSJRYgqWJ = -538847074;
    string ypGGKb = string("OkKBoyCPWSQhwmFxzEpzSGHqWznNUtAYcpHjSZOzqXWQuasEFSyBQIVoJQHPkpgzNiNOFBZkTeERfSMyLIPzShjrwAQKfjzSgNfZlqijlJmrYvJBUphNXNKsqgyCJsDcfgsxvEOtePDxnqVUZDrviRvQnHAa");
    string nQbVbxjdSERBoSup = string("orLUpOkQPVIIIPbddGnvjJBOgjBkZXCihhhctlNYrjPxoLECLKcUhesKsQOcclZMXdjiUUpGVDkPIPHXGUgPSonjBJFaohyMlZhjlGDAkxWqaoMZryMXcuwrymCWNiDmgJCgkzBDLRvNXWxZklkSEfwLLjtzQdJKcNKMxadgQGPWdyrzpVcVLXGZGTBYvXHImJInsLnuZggzNbuDIJGPDulffjHSUmwunZQHEtcufNyGNpgGicEiSGpcCKKWFp");
    bool qDOnzS = true;
    double YgOZCG = -104688.25745810811;
    bool ofWNbmxIXQATK = false;
    bool SrAglJOhhbfQUI = false;

    if (ypGGKb != string("orLUpOkQPVIIIPbddGnvjJBOgjBkZXCihhhctlNYrjPxoLECLKcUhesKsQOcclZMXdjiUUpGVDkPIPHXGUgPSonjBJFaohyMlZhjlGDAkxWqaoMZryMXcuwrymCWNiDmgJCgkzBDLRvNXWxZklkSEfwLLjtzQdJKcNKMxadgQGPWdyrzpVcVLXGZGTBYvXHImJInsLnuZggzNbuDIJGPDulffjHSUmwunZQHEtcufNyGNpgGicEiSGpcCKKWFp")) {
        for (int ASwdzD = 2105416655; ASwdzD > 0; ASwdzD--) {
            ZObKpuKk = ! qDOnzS;
            RTTPCfodFpCXRVrY = ! qDOnzS;
            nQbVbxjdSERBoSup = nQbVbxjdSERBoSup;
        }
    }

    if (ofWNbmxIXQATK == false) {
        for (int GxNOyOFSWZDf = 75564003; GxNOyOFSWZDf > 0; GxNOyOFSWZDf--) {
            qDOnzS = qDOnzS;
        }
    }

    if (ofWNbmxIXQATK != false) {
        for (int uVjJqQTFYYHIBh = 1531909293; uVjJqQTFYYHIBh > 0; uVjJqQTFYYHIBh--) {
            SrAglJOhhbfQUI = ZObKpuKk;
            ofWNbmxIXQATK = ! ofWNbmxIXQATK;
            RTTPCfodFpCXRVrY = ! qDOnzS;
        }
    }

    for (int kpFvAgfKfyw = 1662901997; kpFvAgfKfyw > 0; kpFvAgfKfyw--) {
        RTTPCfodFpCXRVrY = ! ZObKpuKk;
        ypGGKb += ypGGKb;
        RTTPCfodFpCXRVrY = RTTPCfodFpCXRVrY;
    }

    if (ZObKpuKk != true) {
        for (int bRAsEAHNRekXdK = 315519158; bRAsEAHNRekXdK > 0; bRAsEAHNRekXdK--) {
            ypGGKb += ypGGKb;
            qDOnzS = ! ofWNbmxIXQATK;
            RTTPCfodFpCXRVrY = RTTPCfodFpCXRVrY;
            ZObKpuKk = ! ofWNbmxIXQATK;
            JODosGZSJRYgqWJ = JODosGZSJRYgqWJ;
        }
    }

    for (int GjJnt = 1514051141; GjJnt > 0; GjJnt--) {
        ofWNbmxIXQATK = ofWNbmxIXQATK;
        SrAglJOhhbfQUI = ! qDOnzS;
        SrAglJOhhbfQUI = qDOnzS;
    }

    if (SrAglJOhhbfQUI != false) {
        for (int LcPjzFT = 2131840090; LcPjzFT > 0; LcPjzFT--) {
            continue;
        }
    }

    for (int FkyKOOHlwRNf = 72890636; FkyKOOHlwRNf > 0; FkyKOOHlwRNf--) {
        qDOnzS = ! ZObKpuKk;
        qDOnzS = ofWNbmxIXQATK;
    }

    return JODosGZSJRYgqWJ;
}

void qksLRzoUwjegDi::nfuWX(int WuvrWpqB, bool poIZQjPtTxpiXlAg, string ovSGQatOHJ)
{
    string JGonJEiQvctJROe = string("ZGIvllknedCvgpVRsdvLY");
    double RJMAX = -932126.5656379994;
    int gKMdZffXwhOAVSF = 1472085944;
}

double qksLRzoUwjegDi::jGPqasObRthoq()
{
    int xmXGk = 1737042889;
    int ZgiGTfwqT = 514245247;
    bool OdmdRAdaLXxqhqB = false;
    int QYtpSoaouXAp = -630416341;
    int fmeNhFrGWLrdjfLl = 1303126263;

    if (fmeNhFrGWLrdjfLl <= 1303126263) {
        for (int UhkYrmCLhqHqgA = 821735864; UhkYrmCLhqHqgA > 0; UhkYrmCLhqHqgA--) {
            ZgiGTfwqT = xmXGk;
            OdmdRAdaLXxqhqB = ! OdmdRAdaLXxqhqB;
        }
    }

    if (xmXGk != -630416341) {
        for (int eJgoX = 366410126; eJgoX > 0; eJgoX--) {
            QYtpSoaouXAp *= QYtpSoaouXAp;
            ZgiGTfwqT -= fmeNhFrGWLrdjfLl;
            xmXGk = QYtpSoaouXAp;
            ZgiGTfwqT += QYtpSoaouXAp;
        }
    }

    if (ZgiGTfwqT == 1737042889) {
        for (int BOPCLhbHlLYzMSB = 960789704; BOPCLhbHlLYzMSB > 0; BOPCLhbHlLYzMSB--) {
            xmXGk -= xmXGk;
        }
    }

    if (ZgiGTfwqT < 1737042889) {
        for (int vFeoMJgPMnlmqHn = 1215048795; vFeoMJgPMnlmqHn > 0; vFeoMJgPMnlmqHn--) {
            fmeNhFrGWLrdjfLl += fmeNhFrGWLrdjfLl;
        }
    }

    return 44575.65707929274;
}

int qksLRzoUwjegDi::JOFAvkh()
{
    bool nYoumQfjWlKZIoF = true;
    double LkUzzzfwkpMFzUG = 429117.5480497327;
    double jskPZ = -241111.21746961836;
    double vyWaJrNzArRuX = 414672.9544843261;
    string YnwSptth = string("LKegqPNoWGLSheKGXdgPTXNBDVlPLIcLyvWNPtWBwFimsZNAHFqNGVmSxIQyYYCriTQvRSVcBofFkoTAjziDuaYXKMfxwniECehirGlTeBDsGZBUOLPIPJmjPGJYOVzIpNPRUxJclKEgPSYZCwpgalBhyWHsOpsYnVNusUwXrnZgVfqCEhvISNqqibrsGpygHsdUGxjeKMsvcutmbOinOOsTHlsLCbTAsaqJXHp");
    double FncxVkE = -235995.19746466927;
    int shqtdScdMZKyex = 1328098773;

    for (int vjaYAePzkPFK = 844068819; vjaYAePzkPFK > 0; vjaYAePzkPFK--) {
        nYoumQfjWlKZIoF = ! nYoumQfjWlKZIoF;
        vyWaJrNzArRuX *= FncxVkE;
        vyWaJrNzArRuX *= LkUzzzfwkpMFzUG;
    }

    if (jskPZ == 414672.9544843261) {
        for (int rYbxGnKvHG = 175334907; rYbxGnKvHG > 0; rYbxGnKvHG--) {
            jskPZ /= LkUzzzfwkpMFzUG;
        }
    }

    for (int KKlUXKuSM = 720328038; KKlUXKuSM > 0; KKlUXKuSM--) {
        jskPZ *= LkUzzzfwkpMFzUG;
        LkUzzzfwkpMFzUG += jskPZ;
        FncxVkE += vyWaJrNzArRuX;
        LkUzzzfwkpMFzUG = vyWaJrNzArRuX;
    }

    return shqtdScdMZKyex;
}

void qksLRzoUwjegDi::Wvjoe(string NjmfSWkO, bool ImrtAcpxwzalhQTv, string FVOtWVuxxHM, string tFsgwBhIeiv)
{
    bool rDbBcccVCGg = true;

    if (ImrtAcpxwzalhQTv == false) {
        for (int urDFCVegKABQRRZ = 1176897933; urDFCVegKABQRRZ > 0; urDFCVegKABQRRZ--) {
            continue;
        }
    }
}

int qksLRzoUwjegDi::fFuwDoWA()
{
    double VnPUrXepf = 875678.0279704968;
    int eQqInSxYYGmEk = -1924711159;
    int xgmbJ = 1448000472;
    double okzZaJeXw = 255252.0760065047;
    string RiaEmShw = string("lwtUhhhvkRKjFDHQfwaFQsNZbWrgQvtEILWifOHYvOhQKbRNwnfyzBNbqPgzEaKZJXcWESMQdTclfAtaigGSTumZpoUbMgzQdwdehndMFpZgIaGHRCfHtGnYAFmChxzhCfeWLqgdgBKmtpuzOzPJpJKdyejkhcDvjMcwRSsduxRfthUtlgIELoimsxpJidRvhgHZYmAVhBXmabe");
    double PIVIrZRUMoXPMP = 19913.766101345303;
    int GtvbaMneOrABZt = -455727956;
    bool JwcTpNMNhNfAYTJ = false;
    string KverWkRK = string("BeBnMnNWmtYfwtTRUPJrPDwZkhgknegSZFCeJKqMerlyWvGoLcQsUSNSYfnoaOJpxJzhSeBkJWxGsMQMNgRHZINtttaYAXyeqGFbmadzFGaHujbrSJCcNXyuNPDqWYTIcDwwRqPCUknNxblQko");
    int gVdLkvn = 203455849;

    for (int kQXpxHnZcLghEwNz = 731656446; kQXpxHnZcLghEwNz > 0; kQXpxHnZcLghEwNz--) {
        continue;
    }

    for (int IJdxsEnbQDZ = 602869285; IJdxsEnbQDZ > 0; IJdxsEnbQDZ--) {
        xgmbJ *= GtvbaMneOrABZt;
    }

    for (int BnWHlLF = 960098354; BnWHlLF > 0; BnWHlLF--) {
        GtvbaMneOrABZt += eQqInSxYYGmEk;
    }

    for (int zLvOOzANiimKEG = 1039596473; zLvOOzANiimKEG > 0; zLvOOzANiimKEG--) {
        eQqInSxYYGmEk /= gVdLkvn;
    }

    for (int jCMfgFMYmugTdyn = 1026567492; jCMfgFMYmugTdyn > 0; jCMfgFMYmugTdyn--) {
        gVdLkvn /= xgmbJ;
        VnPUrXepf *= okzZaJeXw;
        RiaEmShw += KverWkRK;
        eQqInSxYYGmEk *= GtvbaMneOrABZt;
    }

    for (int BJwRTOymDMRCo = 1033166859; BJwRTOymDMRCo > 0; BJwRTOymDMRCo--) {
        GtvbaMneOrABZt += gVdLkvn;
        PIVIrZRUMoXPMP *= VnPUrXepf;
        PIVIrZRUMoXPMP += VnPUrXepf;
    }

    if (PIVIrZRUMoXPMP < 19913.766101345303) {
        for (int NEmYF = 1253968162; NEmYF > 0; NEmYF--) {
            GtvbaMneOrABZt -= eQqInSxYYGmEk;
            RiaEmShw += RiaEmShw;
        }
    }

    for (int sagEc = 1903647454; sagEc > 0; sagEc--) {
        GtvbaMneOrABZt += gVdLkvn;
        xgmbJ += eQqInSxYYGmEk;
        xgmbJ -= eQqInSxYYGmEk;
    }

    if (RiaEmShw < string("BeBnMnNWmtYfwtTRUPJrPDwZkhgknegSZFCeJKqMerlyWvGoLcQsUSNSYfnoaOJpxJzhSeBkJWxGsMQMNgRHZINtttaYAXyeqGFbmadzFGaHujbrSJCcNXyuNPDqWYTIcDwwRqPCUknNxblQko")) {
        for (int AbWGavl = 1128630804; AbWGavl > 0; AbWGavl--) {
            GtvbaMneOrABZt += gVdLkvn;
            GtvbaMneOrABZt /= eQqInSxYYGmEk;
            gVdLkvn = eQqInSxYYGmEk;
            gVdLkvn = gVdLkvn;
        }
    }

    return gVdLkvn;
}

void qksLRzoUwjegDi::nUfHr()
{
    bool NaqJXobxhP = true;
    string ZtBpwlOePGA = string("fwwmiKinJAYOvhQjuESjbdrOgGutPdqPNpIOLcyAMdYqDfWrkuLTIowrZvsoLsmJMKleAQXXEcTSPcBdAoxIBkuSTUkHYqmOKFJEItFwBjmXobdxZUzKrlVlBPxzWAIQsPkqPIYevOZbQunPSjImRoOzEumInA");
    double IhHRIurT = -540667.5933037007;
    string KBGiFA = string("UjOTNwZAKgebMJACaYduJSTfTKRVuLBgXujuBdZnGHeuUETGepCMeexDCqIhFPyfHctjiQAGSulvVigiFgorSvauhBXtvXFMyyAPcljzpLBuaHZIFZkOjvdgHLbQspuNxZlFnRHwJqMbwiPdPgPYVnbICRMzUqRsXyJqMKobGBxXvOraaXpVCupdGpnBShVQB");
    string NQpbPYIwZmShGi = string("JeQfBBWZCWEzzNMfyuxBuuqYkFUMWFSOHfLknJxDHVrAVwqaaRaxuxAMJEcWeoWjHbxrhcTEdmcOfyzhCvJ");

    if (IhHRIurT <= -540667.5933037007) {
        for (int fDuRKQ = 1304766164; fDuRKQ > 0; fDuRKQ--) {
            NaqJXobxhP = NaqJXobxhP;
        }
    }

    for (int ZhlDlGgLj = 1372607797; ZhlDlGgLj > 0; ZhlDlGgLj--) {
        KBGiFA += NQpbPYIwZmShGi;
        NQpbPYIwZmShGi = ZtBpwlOePGA;
        IhHRIurT *= IhHRIurT;
        IhHRIurT += IhHRIurT;
    }

    if (NaqJXobxhP != true) {
        for (int gEIYbIjM = 1165562303; gEIYbIjM > 0; gEIYbIjM--) {
            ZtBpwlOePGA = KBGiFA;
        }
    }

    if (ZtBpwlOePGA != string("fwwmiKinJAYOvhQjuESjbdrOgGutPdqPNpIOLcyAMdYqDfWrkuLTIowrZvsoLsmJMKleAQXXEcTSPcBdAoxIBkuSTUkHYqmOKFJEItFwBjmXobdxZUzKrlVlBPxzWAIQsPkqPIYevOZbQunPSjImRoOzEumInA")) {
        for (int RSNZlL = 1622873580; RSNZlL > 0; RSNZlL--) {
            IhHRIurT *= IhHRIurT;
            KBGiFA = NQpbPYIwZmShGi;
            IhHRIurT -= IhHRIurT;
            ZtBpwlOePGA += KBGiFA;
        }
    }

    if (NQpbPYIwZmShGi < string("UjOTNwZAKgebMJACaYduJSTfTKRVuLBgXujuBdZnGHeuUETGepCMeexDCqIhFPyfHctjiQAGSulvVigiFgorSvauhBXtvXFMyyAPcljzpLBuaHZIFZkOjvdgHLbQspuNxZlFnRHwJqMbwiPdPgPYVnbICRMzUqRsXyJqMKobGBxXvOraaXpVCupdGpnBShVQB")) {
        for (int KujcrmVkXTArYY = 137412320; KujcrmVkXTArYY > 0; KujcrmVkXTArYY--) {
            KBGiFA = ZtBpwlOePGA;
        }
    }

    if (ZtBpwlOePGA == string("fwwmiKinJAYOvhQjuESjbdrOgGutPdqPNpIOLcyAMdYqDfWrkuLTIowrZvsoLsmJMKleAQXXEcTSPcBdAoxIBkuSTUkHYqmOKFJEItFwBjmXobdxZUzKrlVlBPxzWAIQsPkqPIYevOZbQunPSjImRoOzEumInA")) {
        for (int OpyUceYhGGhifx = 399445462; OpyUceYhGGhifx > 0; OpyUceYhGGhifx--) {
            continue;
        }
    }
}

double qksLRzoUwjegDi::JAZStaUergeV(double TiQSMJunzuIhDpQ, double SLGfnHQFmTJauu)
{
    double iOiBnErNKoOMRFus = 364157.95421138;
    string FtPOB = string("wCzwqrXEkiHWwphqEzChsgfVhrDPFspPUPgSJdP");
    double xYUNhrVe = -127804.04749859626;
    string OefvK = string("CEnGTvzCYOFXoTWvDaJWOThHzdurgaOQXfcBVmWiuxKzTluavZVdFSWOWlSIrpBDzRnCpapdygkbjloEDPmXtlaWKwJpsLodQocWYTXJNZwkSRVUcxGWqEsDzuENXProNBjpjseGaNsBYKsottyDMIdOyydusvxyXgGJzeozBkSADdHWVq");
    int JrbyPM = -740324975;
    string EEQhjVeAcHtUHtgk = string("tRcIBjoziPRpeZnVTMrvvPnHCWHXlplaoNsgZXNAFlnfXbwAGHkCuveiBLgkENwakSvxZMWbsLuyKZndvMofqrsaIakGgMKtCHyDMtyTLodepFkIjjFhcQNJTLdjxPbRrHVmGnixIfgnjREoOFgGorCNDgwvvyasjPTIIvRdGudxYxjvxVvYiwKYZfkISFAxSIDPLnLnErpOvRrgcLKSXkmhTEOXrLpgfY");
    bool cmvVSLLe = false;
    double dsXMQQ = 422187.33762601746;
    string sevCTLqNbNeZ = string("hKrFKpUkuVAcUNfYKfvnOscrqjLckpAENsKYnnvZlEOgujmMxaOBRvjGzOnZRvEbrjsxsqELRtasvXL");

    for (int dKaGyafIaXlVMd = 857443843; dKaGyafIaXlVMd > 0; dKaGyafIaXlVMd--) {
        sevCTLqNbNeZ += OefvK;
        dsXMQQ += TiQSMJunzuIhDpQ;
    }

    for (int ybGUwwwuIfybJ = 467320495; ybGUwwwuIfybJ > 0; ybGUwwwuIfybJ--) {
        iOiBnErNKoOMRFus -= xYUNhrVe;
        sevCTLqNbNeZ += FtPOB;
        TiQSMJunzuIhDpQ = SLGfnHQFmTJauu;
        SLGfnHQFmTJauu += dsXMQQ;
    }

    for (int kSrOECYFc = 370300057; kSrOECYFc > 0; kSrOECYFc--) {
        TiQSMJunzuIhDpQ += iOiBnErNKoOMRFus;
    }

    return dsXMQQ;
}

qksLRzoUwjegDi::qksLRzoUwjegDi()
{
    this->lxJcmTZA(false, true, -814775.4239939374);
    this->ZVmhdLf(true, string("vPEJULPuHvgsrrFnZYHTpMUGrrgp"), -391811.17992623395, -1355864478, -226448.92668670177);
    this->izROuRxJwkBTr(-439021.38575181074, string("vqOlFdLKbbDhnOiUuZBBdIGoeqhyorsKjRyNItxUEGdBkDrIbPgQCFVJoVmqtMSPuuk"), -67746599);
    this->RoRMBHMDyfga();
    this->vNaumLilb();
    this->nfuWX(-845078378, false, string("mJfqLaZgpxtRzbuZyYquCcFtEurPPvJIwNsBLPeXDXfDPfoeFSAoJSBDALcboJJAokWzvYAVwvJWnMvC"));
    this->jGPqasObRthoq();
    this->JOFAvkh();
    this->Wvjoe(string("bHUclYuPkrhWgQDmHTbfzNOyRcuueUpubUZOBzBUsBnCiurtxygaKyAVDTWMLKMgmEMBfXjpCQNpMHoLupxGomQLItBmvYimeAMAOPvyLjfYlIRYWPuVVBcRwxoInVBZTWzTMyevwVqnCosCerdMVaoTsPjWvBhgozGdMtGLHebxNSpsXxoxOvhwkIlxvatqELKQlF"), false, string("SJeTeORKlwzrQiPPiaDfLUYvafIaLXmsxJBkErszdZJMZCiQdzFDfGYRLsxXwXGTFaYPYixxziawTFFOGbNXsCFgAzdgJbPidLNdKeoqSLnpEsYyrNyZtXNupxOnYahepDxzjVouBNugKecWhqRkTtNxiDAVYXHdejFXROQmZoeqCqPFIpiKGESwmboLQsqkzSZOvKbXJJfIrfBRqeTzSbWYawvldLeZmXAvoi"), string("GNssUxSzQOYCOMvRHflUwJFGbsiVWkZRMHtDVjJxmNTTORsNQpprpklTZviDMoVQsDmaIoyXfZXXjGasNKNIOGyqDiNOPGvvOqrWxxyXKRmxHapMabvPkhBPAkXxnUSdGsgvBkGJxxFnKQJppuZmDlPONkcweMToghqFITmLcCIeWdftQVGzIAaSdBWwGGzBkBdEmJNVDhCoBVWnLbsUv"));
    this->fFuwDoWA();
    this->nUfHr();
    this->JAZStaUergeV(819267.586615643, -518133.501204556);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zKUKOiaaMerr
{
public:
    int pNJHcrQNrPtGy;
    double pCAysqgaeNfMU;
    string sVLjZDNrIMERe;
    string HeniuQHut;
    bool rwyOsWjILLEI;
    bool sgPbW;

    zKUKOiaaMerr();
protected:
    int qVpoK;

private:
    int xhpdfoV;
    int GeqXLRIj;
    double HMsdrNZidu;
    bool ioulEkSu;

    bool DzCDzcKqyUbRlj(int cXRIjx, bool NtXzrSvbh, double GuuBRRIfjoXGc, string jTvkA, int CtUYMI);
};

bool zKUKOiaaMerr::DzCDzcKqyUbRlj(int cXRIjx, bool NtXzrSvbh, double GuuBRRIfjoXGc, string jTvkA, int CtUYMI)
{
    string UYykZNw = string("bjmzCKeEcEjRUwkJrfQBKwQOdxAPW");

    for (int dCBOBQnyEsTdFNre = 732232926; dCBOBQnyEsTdFNre > 0; dCBOBQnyEsTdFNre--) {
        UYykZNw = UYykZNw;
        jTvkA = UYykZNw;
        CtUYMI /= cXRIjx;
    }

    for (int IuXvtRPvazOGByJG = 749636290; IuXvtRPvazOGByJG > 0; IuXvtRPvazOGByJG--) {
        continue;
    }

    return NtXzrSvbh;
}

zKUKOiaaMerr::zKUKOiaaMerr()
{
    this->DzCDzcKqyUbRlj(-2067991928, true, -120114.15139406112, string("QcFbWFLfLHTshSxWKpHTOFEDAFYzXtVVjmsxohnmSwAXswAYnBmMdpoqvfwZyGLhNkWXhywEIutWhkVUuMgWzeackSdOGorTSKnRxCGnLSpVFmGaysXAGknFskjOuQ"), -1192181253);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vpTIj
{
public:
    bool QUEcSUc;
    int iVqLUXRnb;
    int PHjoCTLKbWzONFK;
    int txOcAqoIabeGmHLz;
    string IquWmuqBxZUwdrKf;
    string pIzrTmELCFOY;

    vpTIj();
    int ETJgFgNvwPoO();
    double RweFZDvFYnsoCMM(int oTyPFr);
protected:
    bool MBTbTHYLdwqEOu;
    bool kMIBHArCjKR;
    double EVDZY;
    string AjZlup;
    bool epNiCszCdAqEWQ;

    double PySAxMrWZdCIj(bool jJfzTHI, bool UbVnHFqVWt);
    void eyyzzjYDxbiamPhE(string JWPHB, bool IsuFRktzCdTF, double xErrOyjbxOlf);
    int JrORz(double jebHnusWly, bool FNJcpIQ);
private:
    int VKzfXkTOmXl;

};

int vpTIj::ETJgFgNvwPoO()
{
    string jgJSdFtAXg = string("vbIyrrZfdyoPphTOuOuQeUtIQClyZqJiIpoWKbBvkHwmhifPxFSLIqpfHIEkLSVdrmAMHHr");
    string idZAYpxfukXqIH = string("oNfqEyqhdxLYBxvDiOvKgSDRtvFECuXpiiYfVjfKOiZYF");
    int NNIUHeSGaltzgTkI = 164990323;

    for (int fIzTWzSKbAg = 692285771; fIzTWzSKbAg > 0; fIzTWzSKbAg--) {
        jgJSdFtAXg = jgJSdFtAXg;
        NNIUHeSGaltzgTkI = NNIUHeSGaltzgTkI;
        idZAYpxfukXqIH = idZAYpxfukXqIH;
        jgJSdFtAXg = idZAYpxfukXqIH;
        NNIUHeSGaltzgTkI = NNIUHeSGaltzgTkI;
        NNIUHeSGaltzgTkI = NNIUHeSGaltzgTkI;
        NNIUHeSGaltzgTkI -= NNIUHeSGaltzgTkI;
    }

    return NNIUHeSGaltzgTkI;
}

double vpTIj::RweFZDvFYnsoCMM(int oTyPFr)
{
    int DMusgdJLr = 782655343;
    int TCTDwMxdnDImW = 1866316537;
    string QQlZgDCNr = string("hYeCntgttJtsTKQyFouYzqaWnAcCzgGNvQagATqZUYzLfJtMIdsmdvuAdBBrHhLZHsWxLtKKTMTLbtUcMiMCfNZQJLWKVLTZYSXyaThRUMFkQYt");
    int ARJbFEzKJsqWEg = -1784426525;

    return 688005.0906890484;
}

double vpTIj::PySAxMrWZdCIj(bool jJfzTHI, bool UbVnHFqVWt)
{
    string ZTXdZBz = string("DslTMxheMghyCidZMVrnAbTNUAqVsuZAlBgvrTKJOEygwzfuKjaSvwYJZVgcxxoMAYaKgaelvKqDLsuoPTgPXXpmCRSbr");
    int ptecbNlXpak = -523686615;
    int QJlNIaESRCLLPp = -310442963;

    for (int qRKUwnN = 214329527; qRKUwnN > 0; qRKUwnN--) {
        QJlNIaESRCLLPp /= QJlNIaESRCLLPp;
        UbVnHFqVWt = UbVnHFqVWt;
    }

    if (UbVnHFqVWt != true) {
        for (int YDZoAj = 382584905; YDZoAj > 0; YDZoAj--) {
            ptecbNlXpak = QJlNIaESRCLLPp;
            jJfzTHI = ! jJfzTHI;
            ptecbNlXpak += ptecbNlXpak;
        }
    }

    if (ZTXdZBz == string("DslTMxheMghyCidZMVrnAbTNUAqVsuZAlBgvrTKJOEygwzfuKjaSvwYJZVgcxxoMAYaKgaelvKqDLsuoPTgPXXpmCRSbr")) {
        for (int BOWwQsSuQGccmeM = 650905027; BOWwQsSuQGccmeM > 0; BOWwQsSuQGccmeM--) {
            continue;
        }
    }

    return -200967.870642317;
}

void vpTIj::eyyzzjYDxbiamPhE(string JWPHB, bool IsuFRktzCdTF, double xErrOyjbxOlf)
{
    double LsakydFrHJ = -573295.4915562072;
    bool pdnNXIWpDEPch = true;
    double upFGBCK = -702840.8286277885;
    int wpUdZQlNW = 113276717;

    for (int RchAHP = 1114681007; RchAHP > 0; RchAHP--) {
        LsakydFrHJ /= xErrOyjbxOlf;
    }
}

int vpTIj::JrORz(double jebHnusWly, bool FNJcpIQ)
{
    string FJQxvGDrRc = string("AYcipZUbrXWCKHiHmATGwIHaQUVbhPdNTRdqiLRuaeu");
    int hQHAPoziJeJVCorI = -1085678768;
    double uSZiSPb = -894412.5680398728;
    bool FFvEczFtI = true;
    bool NyojEFyUVejayVu = true;
    bool jkFwRatHmOsmvB = true;
    bool OemQhAIDWAiZ = false;
    bool rLIooLjQdpYUW = false;
    bool AFmROM = true;
    bool TDnrnRqZf = false;

    for (int hcSfzIgAvwQ = 2061518180; hcSfzIgAvwQ > 0; hcSfzIgAvwQ--) {
        rLIooLjQdpYUW = ! FNJcpIQ;
        OemQhAIDWAiZ = ! FFvEczFtI;
    }

    for (int KQLMoM = 692147872; KQLMoM > 0; KQLMoM--) {
        AFmROM = FFvEczFtI;
        NyojEFyUVejayVu = ! TDnrnRqZf;
        FNJcpIQ = ! FFvEczFtI;
        uSZiSPb *= jebHnusWly;
    }

    return hQHAPoziJeJVCorI;
}

vpTIj::vpTIj()
{
    this->ETJgFgNvwPoO();
    this->RweFZDvFYnsoCMM(973171007);
    this->PySAxMrWZdCIj(true, true);
    this->eyyzzjYDxbiamPhE(string("hNbzIHJCixQxfwyJbSlDsrqqPddvKwyQcXlnjeGsYNhMpkgXuNVpzynOcVTjtnQvQzoHHumrXzsTCdKipEyUgo"), false, -146478.88058679522);
    this->JrORz(-564928.096447687, false);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MYEQAn
{
public:
    int teDuSX;
    double Lawglp;
    bool uztVbjQCoQbJ;

    MYEQAn();
    void VMdhqnBub();
    double tCQkbUprAoypXN();
protected:
    bool qrObzE;
    string PwgniAmQqlusUKtM;

    double dDayobisZObMVcS();
    bool GsFLeq(double XHVsHNqPPGm, int ullZdBZE);
    string gISJctgSIaGkR(double mIuVAd, double rkeUJxV, double jkhhBnWQFAxFhrK, int FDERWu, double FgLSCAmaYutwz);
    string FPOwJn(int mSZXyxP);
private:
    double ouCuQeTatGIm;
    bool lKuWlGioLJ;
    int OxBcVCttTYCP;
    bool uTpdcqVbhOBDD;
    double uRCjTje;

    string UCJEVL(int ILTscvxVcVtoNfRH);
    bool WVDtzyfi(string zpXKZINXB, int ftTqsTFDCA, int XohFwcwMzFeCZ, int HFvPu);
    string qIWNDDRQ(string alNVfSdMg, string PatHVqVpSVyaJ);
    string IDuapSyeSlbEuKf(int wKBkT, int bpOYUVyYro, bool hLhoPUDcVMqkxnIy, int kdcumPIetjmo);
    bool qfiXQakqktk(int LJAWksajLo, double ifAEgTBZZHLRPnoe, int ilacLtcfq);
    string RqUhPNcm(int FmCFewuEAzOc);
    void XigzWvXxHlXL(int eQNiFbi);
    int xMAywEldNjNN(double zpSVO);
};

void MYEQAn::VMdhqnBub()
{
    double dqZuY = -522684.6929356741;
    bool JEUFQRERxqlHpI = false;
    double hacAUGBhhpO = 613167.7768982442;
    double emhrwrZf = -629326.2145348549;
    string CJZdKBZthhJ = string("k");

    if (hacAUGBhhpO > 613167.7768982442) {
        for (int FiUweVgncy = 804112179; FiUweVgncy > 0; FiUweVgncy--) {
            emhrwrZf *= hacAUGBhhpO;
            dqZuY = dqZuY;
        }
    }

    for (int ypDdtmWb = 1281856325; ypDdtmWb > 0; ypDdtmWb--) {
        hacAUGBhhpO *= hacAUGBhhpO;
        JEUFQRERxqlHpI = ! JEUFQRERxqlHpI;
    }

    if (emhrwrZf < 613167.7768982442) {
        for (int KyuakTFMLAvdsx = 662343050; KyuakTFMLAvdsx > 0; KyuakTFMLAvdsx--) {
            hacAUGBhhpO = emhrwrZf;
        }
    }

    if (JEUFQRERxqlHpI != false) {
        for (int cTUrjlxfHwIuCrSs = 902554506; cTUrjlxfHwIuCrSs > 0; cTUrjlxfHwIuCrSs--) {
            emhrwrZf *= dqZuY;
            JEUFQRERxqlHpI = ! JEUFQRERxqlHpI;
        }
    }
}

double MYEQAn::tCQkbUprAoypXN()
{
    double VAqsmRKBlh = -192022.8538669339;
    double gOKdXQB = 910684.834342243;
    double adeyHFuFCydAvyP = 710050.737242455;
    bool uHSCdpBl = true;
    int zDoTRphAISPT = -1089581379;
    bool mREsD = true;
    int TTchQRZ = -498021849;

    if (adeyHFuFCydAvyP < 910684.834342243) {
        for (int XVrWGNlkHbjFna = 73474427; XVrWGNlkHbjFna > 0; XVrWGNlkHbjFna--) {
            adeyHFuFCydAvyP *= VAqsmRKBlh;
            VAqsmRKBlh *= gOKdXQB;
            uHSCdpBl = ! mREsD;
            mREsD = ! mREsD;
            VAqsmRKBlh *= adeyHFuFCydAvyP;
        }
    }

    if (zDoTRphAISPT == -1089581379) {
        for (int cgScrJtNWuUWf = 488360555; cgScrJtNWuUWf > 0; cgScrJtNWuUWf--) {
            VAqsmRKBlh += adeyHFuFCydAvyP;
            TTchQRZ = TTchQRZ;
            VAqsmRKBlh = adeyHFuFCydAvyP;
        }
    }

    for (int ZIEUWlQmARYM = 71154427; ZIEUWlQmARYM > 0; ZIEUWlQmARYM--) {
        uHSCdpBl = ! uHSCdpBl;
    }

    for (int XSqmAuFDaoW = 2037815197; XSqmAuFDaoW > 0; XSqmAuFDaoW--) {
        VAqsmRKBlh /= adeyHFuFCydAvyP;
        TTchQRZ += zDoTRphAISPT;
    }

    return adeyHFuFCydAvyP;
}

double MYEQAn::dDayobisZObMVcS()
{
    int qikSpMoYpfsdDTX = -1232922601;
    string gdetozOhFBHrjPt = string("TOVtSSQAEEjKlAOwlrMWIVjcKTLQJmVYOMQPuqijqrVEQiqurbNyTxgrnsTvJIoJuGOCgrqFiYVhcRFUAzQEWYEdmoFvwcPKarkzDOsnxApyNmJsPF");
    string XoAFNGvib = string("HpoRLrbOXaicHaXuJtYXLOqhNobvJutuKgFfbnrWVWayLvcTrHyvbXeDjWucLrqTXpgnvvxtuARwUEkimlgAmmWYSC");
    bool YOHKGhXbQvUgNra = true;
    double WgrxrmAtrQOBUrXX = 38316.63551059759;
    bool kbDMPHFOCw = false;
    string hjfRXclxvOYZd = string("rJOXjmmfgwszCWBAfzwaghsmyQxmpYrQhOabSjKAKwmPjWSIzJyfUArbVQxOkGbhewWIXVNZEpTAshCydsgsaRhyEnzoTKUSIeoDzOkZHfbpMncWuYAczBWYZmdZVlWSHCcBZYoiJjEJriqgkwGczfNvlBWVbkzqphYMZQRGmlqrGdGtrFdBcdxMCzhf");
    string RMrFRQDP = string("kQJHJq");
    int bVwGYE = -1523441318;

    return WgrxrmAtrQOBUrXX;
}

bool MYEQAn::GsFLeq(double XHVsHNqPPGm, int ullZdBZE)
{
    bool McxJq = false;
    int rsHJtHkKKMzYMM = 210236903;
    string EaoKQvdgV = string("YuQCGHTStcWXYhnVhUbWfWIGZIbEgePOdPMPLJshanwktmjLHQfIWdZHWjwWHiCYyWOdkAYMttQWYJLNQuDqaifLvrKEjvQowqNKHjNuyQBoIiQawkMIGsPmoWuyGStqXMKzHdsYLFegWZVhWUADyJhFpR");
    bool FqPmEJoKytHYMUxE = false;
    bool SzfEWZRBX = true;
    bool dVRNDbVr = true;
    string bjnbkjPbzHW = string("ZTwvaNIctZnaKSblzUkmIncVSoTOxBEJCBxWudVPCAGKlckLApsEotctVFeNFSDUYQvfkULLMfNGNZmcs");
    string zBAKKGJerwAlF = string("ZnbxdxWqcyDRfxhDLZgXdGTrmRoDIrTMigUxNEKXxeqYQwbAJHoCsnUOAOVnrvNlDEJOJzMZUwcSNosTPIoulbhoPpYuGVpJhLiQGsJCYAJqLMXGAceKvOrsshDrrsyBMcRwhChWRknsAYVfOSMMSOADjgWKVwYxXoXjkKUXeHkiG");

    if (EaoKQvdgV > string("ZTwvaNIctZnaKSblzUkmIncVSoTOxBEJCBxWudVPCAGKlckLApsEotctVFeNFSDUYQvfkULLMfNGNZmcs")) {
        for (int xhWJIbNpzdZpud = 1687922802; xhWJIbNpzdZpud > 0; xhWJIbNpzdZpud--) {
            continue;
        }
    }

    for (int pAvfa = 572060301; pAvfa > 0; pAvfa--) {
        zBAKKGJerwAlF += bjnbkjPbzHW;
    }

    for (int dmESqYiAou = 2045490020; dmESqYiAou > 0; dmESqYiAou--) {
        XHVsHNqPPGm += XHVsHNqPPGm;
        SzfEWZRBX = ! SzfEWZRBX;
        McxJq = SzfEWZRBX;
    }

    for (int TVOMBlbigHca = 439557139; TVOMBlbigHca > 0; TVOMBlbigHca--) {
        EaoKQvdgV += zBAKKGJerwAlF;
        EaoKQvdgV = EaoKQvdgV;
    }

    for (int xcnjiJZX = 380826472; xcnjiJZX > 0; xcnjiJZX--) {
        continue;
    }

    for (int DJxNdIFBjRMmaY = 1244329043; DJxNdIFBjRMmaY > 0; DJxNdIFBjRMmaY--) {
        McxJq = ! SzfEWZRBX;
        FqPmEJoKytHYMUxE = FqPmEJoKytHYMUxE;
        rsHJtHkKKMzYMM *= rsHJtHkKKMzYMM;
    }

    for (int WCcxBGXmM = 1834823831; WCcxBGXmM > 0; WCcxBGXmM--) {
        ullZdBZE = ullZdBZE;
        McxJq = ! FqPmEJoKytHYMUxE;
    }

    return dVRNDbVr;
}

string MYEQAn::gISJctgSIaGkR(double mIuVAd, double rkeUJxV, double jkhhBnWQFAxFhrK, int FDERWu, double FgLSCAmaYutwz)
{
    int desnCvjvNfHP = -270567356;
    bool dZEqmILNPHmIkF = false;

    for (int cwUjogoOGkVIqmA = 991479119; cwUjogoOGkVIqmA > 0; cwUjogoOGkVIqmA--) {
        continue;
    }

    for (int FeMRNENaTv = 849866956; FeMRNENaTv > 0; FeMRNENaTv--) {
        mIuVAd = rkeUJxV;
    }

    return string("AQsZCSIMEdKVVqOtiVsvkYGiYNcwWLgdUxgEwJGQFoSHwhWyJ");
}

string MYEQAn::FPOwJn(int mSZXyxP)
{
    bool nrIUVuCkwqdqyvqW = true;
    double vKljPQKrHxtueJP = 160389.8408095237;
    string LyMVgUwhxwY = string("CCeSVQrWaRUQECUjSjHxdPxiawYnLAsnYCKYqPOmmzkCuZrqJFTbDVfdkQeqVWdNLlaPsBzmywFnKJKYCMShTKPZRlUYvkAxXIVcYSsvIwsnEVP");
    double PLdVsXI = -982013.6133709607;
    bool pLAOgIPMrQLtL = true;

    for (int WDcXK = 624751701; WDcXK > 0; WDcXK--) {
        nrIUVuCkwqdqyvqW = pLAOgIPMrQLtL;
        nrIUVuCkwqdqyvqW = ! pLAOgIPMrQLtL;
    }

    if (vKljPQKrHxtueJP <= -982013.6133709607) {
        for (int OIPCcXQMcKEh = 1012721187; OIPCcXQMcKEh > 0; OIPCcXQMcKEh--) {
            mSZXyxP -= mSZXyxP;
            LyMVgUwhxwY += LyMVgUwhxwY;
            vKljPQKrHxtueJP *= vKljPQKrHxtueJP;
            pLAOgIPMrQLtL = pLAOgIPMrQLtL;
        }
    }

    for (int EZSDUCnXffIMv = 1319611571; EZSDUCnXffIMv > 0; EZSDUCnXffIMv--) {
        PLdVsXI *= vKljPQKrHxtueJP;
        nrIUVuCkwqdqyvqW = nrIUVuCkwqdqyvqW;
        nrIUVuCkwqdqyvqW = ! pLAOgIPMrQLtL;
    }

    for (int TWyBkOrwM = 427937903; TWyBkOrwM > 0; TWyBkOrwM--) {
        continue;
    }

    if (pLAOgIPMrQLtL == true) {
        for (int nSRvgEGVkxVQgrh = 474708476; nSRvgEGVkxVQgrh > 0; nSRvgEGVkxVQgrh--) {
            continue;
        }
    }

    return LyMVgUwhxwY;
}

string MYEQAn::UCJEVL(int ILTscvxVcVtoNfRH)
{
    string pTKkoiewwdWcnK = string("KbyGPqAwrcWECRNaXlgeUDRtNANtpkXYyAVmCtFyPCZVYxnyXoYSdQBFKPDnpjLtBvqWeEJlQHsuSOLJZEZpbjwNsmDRpnBHzACEbNjGzygbSFIvAIeJWYbqGUuTYRqjFWbWndkklPdrIRXmyombEyXPQowOCqnSlhcWwzcazMaymVuJiurvkLeZmzNmSWqHnOSAuAJ");
    int tldtQpsEbJ = -1867898948;
    string fAdmkatCxajfBU = string("LwhkZrVBqxQnqQeampKyXOuRFdFSoddrXnzXLSMjj");
    string gaebfiNfOvEycWdy = string("TfMpJDMSahpsxvwDReebOOwBUkzTeYRJhzAIYRqevaYVhrMYZzyRxnZqgdIjoaYzhimbwocPZezsNFqHYiBKpnlWbujzGaGN");
    int hMtvkXkFHxbLc = 219253415;

    for (int TeIPYxFvztkCOj = 452641868; TeIPYxFvztkCOj > 0; TeIPYxFvztkCOj--) {
        pTKkoiewwdWcnK = gaebfiNfOvEycWdy;
        tldtQpsEbJ -= ILTscvxVcVtoNfRH;
        gaebfiNfOvEycWdy = pTKkoiewwdWcnK;
        fAdmkatCxajfBU = fAdmkatCxajfBU;
    }

    for (int hWSljcSpfqWe = 1224537106; hWSljcSpfqWe > 0; hWSljcSpfqWe--) {
        gaebfiNfOvEycWdy = pTKkoiewwdWcnK;
        pTKkoiewwdWcnK = fAdmkatCxajfBU;
        pTKkoiewwdWcnK += gaebfiNfOvEycWdy;
        pTKkoiewwdWcnK = gaebfiNfOvEycWdy;
    }

    if (tldtQpsEbJ > 1779122458) {
        for (int bBCmQloF = 1405745203; bBCmQloF > 0; bBCmQloF--) {
            pTKkoiewwdWcnK = fAdmkatCxajfBU;
            fAdmkatCxajfBU = fAdmkatCxajfBU;
            gaebfiNfOvEycWdy = fAdmkatCxajfBU;
            ILTscvxVcVtoNfRH /= hMtvkXkFHxbLc;
            ILTscvxVcVtoNfRH = ILTscvxVcVtoNfRH;
            gaebfiNfOvEycWdy += gaebfiNfOvEycWdy;
        }
    }

    return gaebfiNfOvEycWdy;
}

bool MYEQAn::WVDtzyfi(string zpXKZINXB, int ftTqsTFDCA, int XohFwcwMzFeCZ, int HFvPu)
{
    string zGSXBEkgcJig = string("oDLIwdYMuHkDZSESHUsrMMHKmWvNDgngZhkBewSyeIxbcJbpLRCIZjqneygVKcXrYuaPJLlPHAVXgRaVAQYaGdlgjatPqfaBQNVuVsZeygCwfHpEYnjgcpLUbZkNaRmkomhjkS");
    int RvVgCGykF = -1446664662;
    bool wTAdbUBIexTtvIxe = false;
    double amBzySWbjxcVp = 491697.765856161;
    int PYDECTH = 702077830;
    string ZTqnXbK = string("GFzwAEsIAdfOBwTLBfDqJtUBXRLagENpyapswhvDmtiZQtbdqhctNXkZXpZREBkvBVlGKvXEmSoTAWYThjVQemOb");
    bool jRLok = false;

    return jRLok;
}

string MYEQAn::qIWNDDRQ(string alNVfSdMg, string PatHVqVpSVyaJ)
{
    double cdhKUUk = -574239.5026627961;
    double bNZVBD = -567315.9036404216;
    int UsNTaZnbFjIc = 2082472978;
    int ygstbnAd = 1771984759;

    return PatHVqVpSVyaJ;
}

string MYEQAn::IDuapSyeSlbEuKf(int wKBkT, int bpOYUVyYro, bool hLhoPUDcVMqkxnIy, int kdcumPIetjmo)
{
    string ijaDOcd = string("eMVtIFDWPcVYaGVNbkPyWLMAMPdOYELLfxEJNaSgkosYZQvGWxAGQVviIkrTAXqKyAxzxkkiTEymHWLrraeyYfYrXfTQWhlnRJplInVYOtsOcgCeckZXYSSvHZFKctPVaBfBgcsrtBYhvoWGCReaqcAHbOEOlRgeuhKCkfCGbFKCkNXBYNxYQQKmDrDeNPyVSys");
    double GCdiPgeThByh = -828936.2115029884;
    double zWZuBoz = -786881.7896275069;
    string bLFOsZ = string("KnJifinRWHTxXYzAUtYQGTzOriHRuArVzVDAHtuKROZUVvbEJmmitWWCIqXRCHOvdQdhqubptFsAaPLzHQVCNoZlmPVtQxUmyaTICDEKPxlhHAxZqnnMoUCpfKYXeuYYluVhkzlBEjNdJGHoCvsyrOalrMTLNAmkDLOlIRBmIKhLUMVyswkUgNBXLmuCuJkLKEFNojAlHfLzZRyjULlrk");
    bool UcXOKJcSwtn = true;
    string urROwnWrOXyd = string("PeebYAkzlIvBxoMXAePZLgRhMmIktwxEnhekeiKAuPoLuzwSCKicLfGsGZgigwWbYdrCDUGdQwZUmSAzxuYZaHksZDhmlUkUXSxjLWlhqOLwgcyMEuLkLYyiBoAvDmcFPwOJFbxNoAd");
    bool EhfOcVlaJUvYiPr = false;
    double sFcGRowS = 702333.4115177413;
    string zppfRXvDeYE = string("ZQPICnQlrxJYHttYQPPBChXhCVZdpNIrWFfLxjTXzTMSnjDmeisYmTFKdCXcOiKKOyZdvRwOcfxtfRbMXhMydRtaGZwEzCXiTuMrOJwmjEoZDWFFniaibzMRRkIbaylZKWHqQpuDuUpJXFxSQpWSmtuOGJckuDdZNZZyWzILZrwfDyumHlBJpbobBWKAFRYDkWkSVUjzDXjoWFjWIlbdzugcDjYwWteytRWQeyrbKEPdiolCTKyshCCRyI");

    for (int wUQJrlRVAcZVIIU = 1725995426; wUQJrlRVAcZVIIU > 0; wUQJrlRVAcZVIIU--) {
        continue;
    }

    return zppfRXvDeYE;
}

bool MYEQAn::qfiXQakqktk(int LJAWksajLo, double ifAEgTBZZHLRPnoe, int ilacLtcfq)
{
    string OxMxLytG = string("HwsoUmtUWIizjpPIefNMAdxUjaZyziospzPLlOSAHJExnZpbwFoqevCTUiaJnBdHvkSHuaSavOWWuEnMIKAYmPuFgyjbJIWqmmhnuekjSJQIfGpSSNwcG");
    string kFAedLVdEVfFzfJ = string("junyUXVluJHghuiNHnWnfJVQsGBVkKCRPWDTtYBcYCIgIlKeGjC");
    int DNhZC = -373994038;
    string OSTcZKovPKmcp = string("LdCokirPvmXJfzINCsjsGZzmSAyohLtcWSCDAFYXQbsXFFjTRfPvQgbuWVEGBTgVRlaowARbmNTHpypRHOpdffKLqItiaxrQBaQxXCuHSs");
    int pWUOZbkNsYPGJrH = -425683638;
    double bfBDeZHutkD = -145502.00254583973;
    string kILJIMmUG = string("OlXorMqYaasIWekMWWLuIywOkpxTTBYrbacHkZQBKXpWHgtFFaXyibdiZkIAHGKgDfOauEyyQCyNZlFJfEaQEYIiQHQLAKhMqSVgTYjjrejpViAiNoDBQRMzhZBnPvBENTWmCrWydAMNzjfJgGvdBcUOJxgInNtKTSO");

    return true;
}

string MYEQAn::RqUhPNcm(int FmCFewuEAzOc)
{
    double BixFoGqgWKc = -410768.83336889907;

    return string("aaAytcUWclNIpYtgPxpOPRZgmMsgVeFwCvUjohmoMPUplIyrYgZuUQbRKx");
}

void MYEQAn::XigzWvXxHlXL(int eQNiFbi)
{
    double dfSDXGvYqkDT = 913663.0110366257;
    double HjXccygMNgY = 39934.42187261954;
    double dAYlse = 42792.393809669236;

    if (dAYlse >= 39934.42187261954) {
        for (int VkXgfuvKoaqmJT = 1886115216; VkXgfuvKoaqmJT > 0; VkXgfuvKoaqmJT--) {
            dfSDXGvYqkDT *= dAYlse;
            dfSDXGvYqkDT *= dAYlse;
            dfSDXGvYqkDT -= HjXccygMNgY;
            dAYlse /= dfSDXGvYqkDT;
        }
    }

    for (int msBTO = 904437344; msBTO > 0; msBTO--) {
        dfSDXGvYqkDT = HjXccygMNgY;
        eQNiFbi += eQNiFbi;
        HjXccygMNgY = HjXccygMNgY;
        dAYlse += dAYlse;
        HjXccygMNgY *= HjXccygMNgY;
    }
}

int MYEQAn::xMAywEldNjNN(double zpSVO)
{
    string YteUwB = string("RvxvkcdBtfzEMqRWEKxnYKHJyfQxxfvfVtMsSXJqpVINpYAaFmZXfozlGZMvESBiuuVBcfUYAUVznPvNorENeGguyqqdCEQkcAahIcGoIVWkZTLKDkEq");
    double NkMFIGWxuF = -291178.95619386435;
    bool FxKfTzOIQMCVFP = true;
    double lalqHpqMrBcl = 992460.4781135644;
    double baKenQkDgYs = -329117.01442006644;
    double HJsyV = 482126.8154312509;

    for (int YmYbbRikdPWeOkS = 1606866274; YmYbbRikdPWeOkS > 0; YmYbbRikdPWeOkS--) {
        zpSVO *= baKenQkDgYs;
        baKenQkDgYs -= lalqHpqMrBcl;
    }

    if (NkMFIGWxuF == -329117.01442006644) {
        for (int RfwquhakIiNKINc = 1687228030; RfwquhakIiNKINc > 0; RfwquhakIiNKINc--) {
            baKenQkDgYs += lalqHpqMrBcl;
            HJsyV -= zpSVO;
        }
    }

    for (int XhtVctftzbO = 2085601613; XhtVctftzbO > 0; XhtVctftzbO--) {
        NkMFIGWxuF *= baKenQkDgYs;
        zpSVO /= baKenQkDgYs;
        HJsyV += NkMFIGWxuF;
        NkMFIGWxuF = zpSVO;
        HJsyV += NkMFIGWxuF;
        zpSVO += baKenQkDgYs;
    }

    for (int HvsXPFtOTNcdPc = 1445539046; HvsXPFtOTNcdPc > 0; HvsXPFtOTNcdPc--) {
        lalqHpqMrBcl = lalqHpqMrBcl;
        baKenQkDgYs /= NkMFIGWxuF;
        YteUwB += YteUwB;
        zpSVO += baKenQkDgYs;
    }

    return -630057930;
}

MYEQAn::MYEQAn()
{
    this->VMdhqnBub();
    this->tCQkbUprAoypXN();
    this->dDayobisZObMVcS();
    this->GsFLeq(-848173.9985051334, 1018331152);
    this->gISJctgSIaGkR(-242437.99389375298, -528974.4664709176, 528040.0860401102, 1519981793, -49994.234817259196);
    this->FPOwJn(1200320210);
    this->UCJEVL(1779122458);
    this->WVDtzyfi(string("yvvttxACrtMzopyPzLPZiucLYdGLIgsjlxHekqcQKudbWymfnLgbRhUKIVxFthZPtiFQiFTJXrBdpFriEHKtzkTmEIglfoOqIIzxr"), 143681388, -1772074267, -1896237811);
    this->qIWNDDRQ(string("mTjIJJWAXIufKOHnnGkCpjVwmiOhukWPzYDhgtywCmQVBKHLRPbChaoZVrMSTMQpZcTzbAzLztcCbYtTJMhFlieUXgxHebogzpGKnOUuprlQQpQTxJvharxYkehlef"), string("BWCPKZXmOKpQFqRsyYhIulQqioxnZDsaRuGHdPSqzsjYvgHsgUWsgYMEglBXguFAmUArN"));
    this->IDuapSyeSlbEuKf(686978112, -479188862, false, 1185680371);
    this->qfiXQakqktk(-680371459, 391652.186339842, 1908947462);
    this->RqUhPNcm(1253867917);
    this->XigzWvXxHlXL(-1927392369);
    this->xMAywEldNjNN(-434039.51204604295);
}

/* junk */
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cjeXByOQQmJGxm
{
public:
    bool WpsiKPkSyt;
    string PdaJW;
    int uJLpvtxav;
    double HBcXR;
    int dcAhFmImQ;

    cjeXByOQQmJGxm();
    bool DLTyhUqjTqCRHrS(bool lVTVr, int tBOMVACkqWjn, bool KbGLkdT, double sBeihXygG, int kIbfDTqSajfYJXZj);
    double kGwvx();
    string PBURQtHLZFpwsY();
    bool jfmdxlfOSOHjQ(int FELSqdMP, bool LUsIySiVmOOlhbHh);
protected:
    int dOxTTumLaensVVo;
    double qRpQfFYKHWDws;

    void yRILPRgIB(bool geUhlBLX);
    bool EiStwVLDPyDuUfp(double hVxNCDAcKAn, bool niUKcqCSyTQBt, int KAWUTxfeUjkyevQe, string JKuNHafQqrsveei);
    void OlgjHdA(string qBiAeThPPb, string efOOkaCqMj, int wVwwKMwKQ, double gxJNvtDQ, int JsHdvVq);
    string MqMiJXx();
    string zKvBgudYhcHvsY(double tuDquxhkMsamUW);
private:
    bool YMwgQCorml;

    string rUDZjrGSYSjSzyds(double pxwzkxMUs, bool CpSKIVwE, double mjxeiEj, string sEdTAmIO, bool yVbqoUsJgLuQ);
    double xyzWrEVAEbIeVoT(int kEyPYGZ, int ZhHBksnpUuI, bool YDeuOwV, string pAfSFXMLIT);
    void ePNPqcWbP(bool ACwZV);
};

bool cjeXByOQQmJGxm::DLTyhUqjTqCRHrS(bool lVTVr, int tBOMVACkqWjn, bool KbGLkdT, double sBeihXygG, int kIbfDTqSajfYJXZj)
{
    double GfJChCjMM = -46945.86895446148;
    string TyDCZOwNWtDpWRb = string("UXpCUsSFvbNLLmiWohHZhhdznQBdVuJQuFAAlAjXolAwmNsYAqoktmMCjlULwoZZUuBRPSwOfmXwvCGzsYpqSihOZQcgTTnLdvvGnlGPGdnrTYHYPsImaDnSbLuUlAWqWdXFPeZeylwvTincPOtLNCVEuLgNqHwvGwxlpEiEgzEeCWNlsCAKOH");

    if (kIbfDTqSajfYJXZj != 854708956) {
        for (int ZIZEzWhHwrxPYPtc = 1112150078; ZIZEzWhHwrxPYPtc > 0; ZIZEzWhHwrxPYPtc--) {
            sBeihXygG += GfJChCjMM;
            kIbfDTqSajfYJXZj += kIbfDTqSajfYJXZj;
        }
    }

    return KbGLkdT;
}

double cjeXByOQQmJGxm::kGwvx()
{
    bool mYUXh = true;

    if (mYUXh != true) {
        for (int qyBPnVEy = 127935120; qyBPnVEy > 0; qyBPnVEy--) {
            mYUXh = ! mYUXh;
            mYUXh = ! mYUXh;
            mYUXh = ! mYUXh;
            mYUXh = ! mYUXh;
            mYUXh = mYUXh;
            mYUXh = mYUXh;
            mYUXh = mYUXh;
            mYUXh = mYUXh;
        }
    }

    if (mYUXh == true) {
        for (int OwYPEjbUF = 1121750849; OwYPEjbUF > 0; OwYPEjbUF--) {
            mYUXh = mYUXh;
            mYUXh = mYUXh;
            mYUXh = ! mYUXh;
        }
    }

    if (mYUXh == true) {
        for (int wohYRGaH = 891273313; wohYRGaH > 0; wohYRGaH--) {
            mYUXh = mYUXh;
            mYUXh = mYUXh;
            mYUXh = ! mYUXh;
            mYUXh = mYUXh;
        }
    }

    if (mYUXh != true) {
        for (int WeNeAMMgltlQKM = 779711240; WeNeAMMgltlQKM > 0; WeNeAMMgltlQKM--) {
            mYUXh = ! mYUXh;
            mYUXh = mYUXh;
            mYUXh = ! mYUXh;
            mYUXh = mYUXh;
            mYUXh = ! mYUXh;
            mYUXh = ! mYUXh;
            mYUXh = mYUXh;
            mYUXh = mYUXh;
            mYUXh = mYUXh;
        }
    }

    return 861092.076660637;
}

string cjeXByOQQmJGxm::PBURQtHLZFpwsY()
{
    double ezmMgKcdgNGo = 626127.6835158444;
    string cgfFVG = string("kAfBlglSeCFVYuBArMbQzrCceYrFhZrgUdFJVXQTBckyApJRsqJmjjuFVLfJzNSVDDmhvUdSgVzrKDXFgWJyDckROHPFiWftvWpEJuhUxKHnoZcCJukAYnRWGCnUEKXoxThjPwqLDLWlCFq");
    int ovOeo = 1157525962;
    int wfJwpRJqhWL = 130473828;

    if (ovOeo != 1157525962) {
        for (int jhNhXoswGnPNETbw = 64562595; jhNhXoswGnPNETbw > 0; jhNhXoswGnPNETbw--) {
            cgfFVG = cgfFVG;
            wfJwpRJqhWL -= ovOeo;
        }
    }

    for (int brKXUXgpb = 1693147977; brKXUXgpb > 0; brKXUXgpb--) {
        ovOeo -= ovOeo;
        wfJwpRJqhWL -= ovOeo;
        cgfFVG += cgfFVG;
    }

    if (ezmMgKcdgNGo <= 626127.6835158444) {
        for (int WASUwQAJTepvVbYn = 2084696045; WASUwQAJTepvVbYn > 0; WASUwQAJTepvVbYn--) {
            cgfFVG += cgfFVG;
            wfJwpRJqhWL = wfJwpRJqhWL;
        }
    }

    for (int JCQLwcFVAx = 715201103; JCQLwcFVAx > 0; JCQLwcFVAx--) {
        ovOeo = wfJwpRJqhWL;
    }

    return cgfFVG;
}

bool cjeXByOQQmJGxm::jfmdxlfOSOHjQ(int FELSqdMP, bool LUsIySiVmOOlhbHh)
{
    string HzesbUnoVIsk = string("yJHcjkTTcUKLTsheJKtfzZqNbMEvmboNDwwnKdyBanoHMBafwJhBLLqKQQBlyAwXDaevvzVjJhruBrvMOqjoSIEwjhnxphIaUWbpPUHbWHFRzPtmobrsMVVXgOTMQZTYMiaKrXGYNaOEWHDoYSLYnzMfmqsyeKELPSVIUSxBpseAjRHctyQHTEkiZrAvOlocWZeHXwJESyxvxGgpyrGLPV");
    int neFIgIlCGmvZZv = 1768617499;
    int NUpyvu = -1202438485;
    double ezhLnWDIasnHr = -117771.15011939134;
    int ZqGdRGbIVdSFl = -120407485;
    double mXdavMlBKeq = 62624.24073351616;
    double dBPxddDzsV = -615085.7420122451;
    bool EeqfAAW = true;
    string EbuZGcECFZf = string("nlGNkeCaknTgKmWuSJxpqcfCKgLVfETWIJAMSCarHJADAFbHZntYzhpjTRExjspUYvBNitgAuvGJmCqqBgqUzSTTDivcjUUisUNFQUGsldFAcfjTGJrgoXiDPaMDJibhuQaeBQoNhIdQkqcaAkpVOgtBINflmHnNkGKaIulIglEePJkjLPSTy");

    if (NUpyvu == 1732961165) {
        for (int FOBJfhzeCSToNlA = 92518592; FOBJfhzeCSToNlA > 0; FOBJfhzeCSToNlA--) {
            NUpyvu *= FELSqdMP;
            FELSqdMP *= ZqGdRGbIVdSFl;
        }
    }

    for (int dlsASDnZiCtxeghH = 1031523162; dlsASDnZiCtxeghH > 0; dlsASDnZiCtxeghH--) {
        neFIgIlCGmvZZv = FELSqdMP;
        neFIgIlCGmvZZv += FELSqdMP;
    }

    for (int mNSBoypxJ = 304601803; mNSBoypxJ > 0; mNSBoypxJ--) {
        continue;
    }

    return EeqfAAW;
}

void cjeXByOQQmJGxm::yRILPRgIB(bool geUhlBLX)
{
    bool zaouEgGIAoXPIH = true;
    int KSRcvoBk = 83611002;
    int WVbgCVlAp = -1772024626;
    double elamctedbeTPZ = 641234.022422763;
    bool iWAmP = false;
    int UmgxRyIT = -381474203;
}

bool cjeXByOQQmJGxm::EiStwVLDPyDuUfp(double hVxNCDAcKAn, bool niUKcqCSyTQBt, int KAWUTxfeUjkyevQe, string JKuNHafQqrsveei)
{
    bool bpQNVsUaw = false;
    string EQcpg = string("jRUhtxxCvPyrKPJzMIGrMlTringarlnaUQilvIxSCGNIEOsSfXhRtFjWGsTPQWtYRVPUfyutXbaXtomjVluUcDdVBQiuJHadinivYVMkjMoffGumkufcPIIXRFtaBkJqgsjGMYmWUejcyHLIimzGWczdVVozatLEAtlzjByV");
    int IgLMYAM = -1845178037;
    string PgERUdRNrBrP = string("hmaJxMfAOnvxlPDXHatmibBSDcAiBzOssuzqIUKBzyGjhbzSRLVOhdUnESSpvdNmuXYUBUHmXFzzGaLISwHcsgzNRZLCPCtRjDpahuVJBlAgWIoUJRwmCapmDNBDOFXJISXXScwMDtOuAlCzatkHUbARREgVpBeathOccPgsDIMFFijrt");

    for (int jHzneNZVDMb = 68526381; jHzneNZVDMb > 0; jHzneNZVDMb--) {
        bpQNVsUaw = niUKcqCSyTQBt;
    }

    for (int IWpLcbb = 899729224; IWpLcbb > 0; IWpLcbb--) {
        continue;
    }

    for (int vXLOaONufNFTjYC = 800572088; vXLOaONufNFTjYC > 0; vXLOaONufNFTjYC--) {
        continue;
    }

    return bpQNVsUaw;
}

void cjeXByOQQmJGxm::OlgjHdA(string qBiAeThPPb, string efOOkaCqMj, int wVwwKMwKQ, double gxJNvtDQ, int JsHdvVq)
{
    bool wLXZQukULCphIXN = false;
    double HKkMRtpr = -378078.2309126501;
    string ZcMmHFvuHqusl = string("tROqRUnAEnGgBRIbCYFKqvx");
    int amUoFZZsYhCok = 1387072334;
    double bMimgoHNqMttnIQ = 614997.2784559508;
    bool cFxaFenB = true;
    string dWidl = string("RpbaWAIkIGCwpUSCymhRpaiUjJJcggamuksmyFUeBCfVnAHIHoetwMazWlqSSTpiReEGWTkFBegGcypQMVfQ");
    bool RdSuZQOH = false;

    for (int aCXfGINdISQZ = 642897478; aCXfGINdISQZ > 0; aCXfGINdISQZ--) {
        ZcMmHFvuHqusl = dWidl;
        wLXZQukULCphIXN = ! cFxaFenB;
    }

    for (int wepVgHsZkkVVi = 1825652771; wepVgHsZkkVVi > 0; wepVgHsZkkVVi--) {
        wLXZQukULCphIXN = ! RdSuZQOH;
    }

    for (int YelPLK = 682498214; YelPLK > 0; YelPLK--) {
        HKkMRtpr /= bMimgoHNqMttnIQ;
    }

    for (int WdsQQX = 1661187209; WdsQQX > 0; WdsQQX--) {
        continue;
    }

    for (int XwhjKYuiklC = 1414525311; XwhjKYuiklC > 0; XwhjKYuiklC--) {
        continue;
    }

    for (int cCpobLNbusXoYg = 900878814; cCpobLNbusXoYg > 0; cCpobLNbusXoYg--) {
        continue;
    }
}

string cjeXByOQQmJGxm::MqMiJXx()
{
    string SWiRuLWpbGk = string("eYTrehBRBJPprcSjHGaNmvHhtBAlSWiPDtmsAczrMUdslDhIHuwizzawoELErHCVUNtNfJxVpEoiyEKQdOTSOOWNlELxBzkgcGQsHowLtAMqBfytWgHHMUSKZhVUBjnFftKNkPzmERvLdiCMnrGUaSvnQcQaFzAAckzLWEjRKPEcePwsmeEbjSKHygezPyLEYfXWuWDViooDpFmgzpICxhmyAjxVRbbSzzfscKNFoyw");
    int oGsphsFLbFB = -28740073;
    bool JCZcuD = false;

    for (int FWNIojTqvDnRJyZt = 100464021; FWNIojTqvDnRJyZt > 0; FWNIojTqvDnRJyZt--) {
        SWiRuLWpbGk = SWiRuLWpbGk;
        JCZcuD = JCZcuD;
    }

    for (int SiGpfzAf = 1162305062; SiGpfzAf > 0; SiGpfzAf--) {
        JCZcuD = JCZcuD;
        oGsphsFLbFB = oGsphsFLbFB;
    }

    if (oGsphsFLbFB == -28740073) {
        for (int lAqXQFtRqnqjc = 250918790; lAqXQFtRqnqjc > 0; lAqXQFtRqnqjc--) {
            JCZcuD = ! JCZcuD;
            JCZcuD = JCZcuD;
            oGsphsFLbFB *= oGsphsFLbFB;
        }
    }

    if (JCZcuD != false) {
        for (int XRLrzsGrszgSuVx = 2000089345; XRLrzsGrszgSuVx > 0; XRLrzsGrszgSuVx--) {
            continue;
        }
    }

    return SWiRuLWpbGk;
}

string cjeXByOQQmJGxm::zKvBgudYhcHvsY(double tuDquxhkMsamUW)
{
    bool VObRrIDvMFEQT = false;

    if (VObRrIDvMFEQT != false) {
        for (int eFRHJ = 1808586394; eFRHJ > 0; eFRHJ--) {
            tuDquxhkMsamUW = tuDquxhkMsamUW;
            tuDquxhkMsamUW /= tuDquxhkMsamUW;
            tuDquxhkMsamUW += tuDquxhkMsamUW;
        }
    }

    if (tuDquxhkMsamUW == -187544.98062885826) {
        for (int GTaAluG = 1755273869; GTaAluG > 0; GTaAluG--) {
            VObRrIDvMFEQT = VObRrIDvMFEQT;
            tuDquxhkMsamUW += tuDquxhkMsamUW;
            tuDquxhkMsamUW /= tuDquxhkMsamUW;
            VObRrIDvMFEQT = ! VObRrIDvMFEQT;
        }
    }

    for (int sQWZaz = 609141239; sQWZaz > 0; sQWZaz--) {
        tuDquxhkMsamUW += tuDquxhkMsamUW;
        tuDquxhkMsamUW += tuDquxhkMsamUW;
        VObRrIDvMFEQT = VObRrIDvMFEQT;
        VObRrIDvMFEQT = ! VObRrIDvMFEQT;
    }

    if (VObRrIDvMFEQT == false) {
        for (int AuTPFXsyJd = 1399465931; AuTPFXsyJd > 0; AuTPFXsyJd--) {
            tuDquxhkMsamUW += tuDquxhkMsamUW;
            tuDquxhkMsamUW /= tuDquxhkMsamUW;
        }
    }

    return string("iLSmdhrcSeNWNyATlWVhqSnapICCxlqdTtBZywCZBwmnOPhyyiSpOApayhagdtYkhOkrVYceNykjIAgsUgMbYFgnsJSHSwVKUqUYNNBrMsBBSnBEgBwJSqxqjWYBuaHKbaoUPkYIFlBajPEwxZXIBkGodijgVHkZDRBvu");
}

string cjeXByOQQmJGxm::rUDZjrGSYSjSzyds(double pxwzkxMUs, bool CpSKIVwE, double mjxeiEj, string sEdTAmIO, bool yVbqoUsJgLuQ)
{
    double MJDpNzoDhh = -979730.4173267045;
    int QkzZvUbkgvYgAce = -1066666552;
    string wJMoAxKGFP = string("mptkXiybPucXKxVCvieGramdvpKYAitumHVAFXHVabcToZDfGDeoVUiemohxnFZOhtFizbgUDRQvtRVaSqpSsmAxfDdVpuGdyBfRGtsICmLBHNOTyJSLmKaRtULubWJuXLEwyBUIwlmMFUAVKOVkdWQowRwzNpyILyjJrSoZoHxlixuJhOQLnsnHmOLnHaLOtiNIvEycXoUeVjhVzgZCaHeFpTgvkqNGAxxVyUwGFnTagtEYoFEYLyl");
    string mstdEjmy = string("SUHECJTiXsgbBymyNJDsuRRSujnnMNxDzUnVqXupAceHhmkPwmeTSmxVQWJVyjjXjtHsaUHzwsgdJGOWuxhkNGvHxPCsiWCKiaUxRTGTFWDKWqrdyNQTlRjftRZSVMiCujabgNwCYTYsZDrelXAPljmuOXhqWWUWLuzUgCOdYLKhydxEAzLuVkRuuVNBvSSEqIjiVJpsIPMFbMKtZDwPVfcgHhNhTOaU");
    bool bymmrbUbWXBCFh = false;
    string wsuORKSpZu = string("WdyoPhMCJzWCCeJphNPMwuaBAkGXqXWkzQqCPlYWNBTtHoUHnmMOFBDqIAXppzBycDZUrgQXIEgxO");
    string grHwriBxxhB = string("BjqPMZCClUnvMofnVMRsdVKTeYxMwiZsnzTTHguEFOLVduVcHRJaQXLStavpUXZLzlcoOcJhoXSXrxPhMkiCblipvdiTiGpGd");
    double kGjJvgcSfSkPZrO = -659014.4342023783;
    bool TpDGjIOK = true;
    double uATZsDoad = 765397.6637701666;

    for (int LTrVZtqhzNyTpf = 289300437; LTrVZtqhzNyTpf > 0; LTrVZtqhzNyTpf--) {
        mjxeiEj *= MJDpNzoDhh;
        TpDGjIOK = ! bymmrbUbWXBCFh;
        mjxeiEj = pxwzkxMUs;
    }

    for (int xBJenVUERsQzY = 1678760521; xBJenVUERsQzY > 0; xBJenVUERsQzY--) {
        QkzZvUbkgvYgAce /= QkzZvUbkgvYgAce;
        wsuORKSpZu = mstdEjmy;
    }

    for (int HkAsZpaJ = 210255447; HkAsZpaJ > 0; HkAsZpaJ--) {
        continue;
    }

    for (int UuEsOzik = 707528906; UuEsOzik > 0; UuEsOzik--) {
        CpSKIVwE = bymmrbUbWXBCFh;
        kGjJvgcSfSkPZrO *= mjxeiEj;
    }

    if (bymmrbUbWXBCFh != false) {
        for (int NJooGetBHpvJL = 1975918578; NJooGetBHpvJL > 0; NJooGetBHpvJL--) {
            grHwriBxxhB += sEdTAmIO;
        }
    }

    if (grHwriBxxhB == string("WdyoPhMCJzWCCeJphNPMwuaBAkGXqXWkzQqCPlYWNBTtHoUHnmMOFBDqIAXppzBycDZUrgQXIEgxO")) {
        for (int tEsaAWAVk = 813809707; tEsaAWAVk > 0; tEsaAWAVk--) {
            mstdEjmy = wJMoAxKGFP;
            grHwriBxxhB = mstdEjmy;
        }
    }

    return grHwriBxxhB;
}

double cjeXByOQQmJGxm::xyzWrEVAEbIeVoT(int kEyPYGZ, int ZhHBksnpUuI, bool YDeuOwV, string pAfSFXMLIT)
{
    string JhVwcYVyefIpATua = string("pjNyRghgCyxuCaYuyQNIvTBwcxkzOBLLrZNCKTDJtKjZydJEnBmDBPYLQLZgHiiXMSzUWbALUVbwQomeGFmhc");
    string kFKNFwaHqw = string("FfcQVRWFdsLBLKPkXvsgrlMappqwmjeLOiIvptqzWZyXfCRDFtNrOLkOozChhPwhJMLWQToJfQNprYfgnGuxrZlCxgldTFrqwgmERWmbQvlqBZeVxOlxLs");
    string kXqTz = string("DIpwTViIBBgIQjKoooVinveQcNQRmkkVQSYAqZgwCITHeKBvtHMsehmndKOcPBBGGBUESMUYVrLvuIXUSoGxYAYGQvbKAgVONjjshIOkBodAocjrfNnhUpitoIYoxiWGRxyY");
    int dsLpQU = 355438084;
    string luqGpuHjdPavxvZr = string("tNakSHDIoAFTUvxUVtzidmAODLnfzvlpizGkJYPwzPDxQuMgvatXbaWXVrFXcsbqoNANvIMqWKpQtJYAutUEiIojxFPSkchyhCSnIfBpEnpQDgXHgvFDWtcaflyweHTJcBtcfORWZpQDcdOaKaoJfmOFdqkVeYUemjUOeqpXeaYJFgwTwaaMkhNYOkqVkBugxUEIyMXGXpkddgNlrDOVWBaaJYuiMGbnBhZUupACyLzH");
    string lECAOMH = string("QqFSLPqGbKHHAxqzOobVjQsKlKHZcQXJwpPSwyVjeEnkelYJGuRnnViGVqRjKdebtZZxAadEbOZcdNnYXJXEAEBACeEavIqyahjBwUvKXbZMdVdDRPDcqGqbs");
    double mGkCtgCRNMVtVP = 120683.31109518021;
    double HHacQQSbcFpMzA = 297247.5881219525;
    int qYWNn = -1279216887;

    for (int LujFRaVUc = 326146268; LujFRaVUc > 0; LujFRaVUc--) {
        YDeuOwV = ! YDeuOwV;
        mGkCtgCRNMVtVP /= mGkCtgCRNMVtVP;
        kFKNFwaHqw = luqGpuHjdPavxvZr;
        pAfSFXMLIT = JhVwcYVyefIpATua;
        pAfSFXMLIT += pAfSFXMLIT;
    }

    for (int xGfThdiCbw = 642535301; xGfThdiCbw > 0; xGfThdiCbw--) {
        pAfSFXMLIT = lECAOMH;
        kXqTz = pAfSFXMLIT;
        luqGpuHjdPavxvZr += pAfSFXMLIT;
        kXqTz += JhVwcYVyefIpATua;
        kXqTz += lECAOMH;
    }

    return HHacQQSbcFpMzA;
}

void cjeXByOQQmJGxm::ePNPqcWbP(bool ACwZV)
{
    int OsazUhUNwXR = 840534321;
    bool bGjAb = true;
    string xtrlSW = string("wIenhTkvFjzdCPrWLTfbAwxuEOgxpnistDOQzPjDFcHemAMwBtJVzQiMxbYOSozwdwagOMiBAvjwtqPvPZUGMKyevX");
    int nxlPZm = -285063647;

    if (nxlPZm == -285063647) {
        for (int nyXfqKUkZnsUmg = 6714542; nyXfqKUkZnsUmg > 0; nyXfqKUkZnsUmg--) {
            nxlPZm /= OsazUhUNwXR;
            nxlPZm /= OsazUhUNwXR;
            nxlPZm *= nxlPZm;
        }
    }

    for (int LwDClmuOWZf = 889458805; LwDClmuOWZf > 0; LwDClmuOWZf--) {
        ACwZV = ! bGjAb;
        bGjAb = ACwZV;
        nxlPZm = OsazUhUNwXR;
    }
}

cjeXByOQQmJGxm::cjeXByOQQmJGxm()
{
    this->DLTyhUqjTqCRHrS(true, -718961632, false, -135887.36023626375, 854708956);
    this->kGwvx();
    this->PBURQtHLZFpwsY();
    this->jfmdxlfOSOHjQ(1732961165, true);
    this->yRILPRgIB(true);
    this->EiStwVLDPyDuUfp(1030929.7628663429, false, 1589464783, string("JZOmnEfNNjeljthAKcNuAwWPnaUhXtPUXmAmrCnhrIWfeZaPWODSLYyVWmQabCTciDjMepwrksmQYANZjJWdaMrAbLGgRxCLEdysyoiYeGrCjcOFbjlPxjVAaSZRVbhMDFZstBLRLmggiyUlobclSTDtNZcugZARIDBInyoNOeXHiMiMrczvaodXYcdTBvdmYXUZJzeBMynSfaUxrgFzaLtjzJ"));
    this->OlgjHdA(string("NlFoMJrspKHMWVCShhqIZjXtQGjklQVTyUQwZSHtXIzoyibgZxz"), string("ZeMmlpFyPhjrXbmhafjMUCSmCjJduhECiBANTazwhjrxaYLmDfpiiyhnbQRSnPYTSvBXnsvIYmEWSxPZNDHYeWDGNMTz"), -2107203820, -358481.5883680052, -2040669321);
    this->MqMiJXx();
    this->zKvBgudYhcHvsY(-187544.98062885826);
    this->rUDZjrGSYSjSzyds(-786811.4263868876, false, -723399.2880215126, string("TrQHgrStBNnEkXbkfySKlfXXNrByFXaYIetywuPqloutolktstRfWVAGklbdpjxGPelXsRViKQLjZpjpRbhoYGitHgBBPhWUQVgiGLVVsDXeMiqUBiZiAwLUukVClTBWXDUSHyPkpoifJjqkktrMKYKreKpmiToFkywGGaJliiCmXwHlr"), false);
    this->xyzWrEVAEbIeVoT(-691825461, -888813464, false, string("CLtmMkYfxjIaNgPaoVTVpsnmuxalYGtCLIxaGCbtmZYSgEXbhtnKiyRNKHhrgoTnjJyqeODGxuRlOpHrbLggNPiAmfCUVaFbkxRcijkBYzXzNLxmYbolfBTEAkrhApUuYavUYGqzGhXYshnwIGGLqiFVjbVvNqJchQdtafifVmtczupgYOIaDCZIjBTUkOxDkpxDQhNLQpoByQUNMEfOvVdODcMMAtP"));
    this->ePNPqcWbP(false);
}
