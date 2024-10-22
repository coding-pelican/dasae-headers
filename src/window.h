#include "assert.h"
#include "primitive_types.h"


typedef struct Window Window;
struct Window {
    const wchar* title_;
    u32          width_;
    u32          height_;
    f32          refreshRate_;
    bool         usesRefreshRate_;
};
const wchar* Window_defaultTitle       = L"Terminal Window";
const u32    Window_defaultWidth       = 640;
const u32    Window_defaultHeight      = 400;
const f32    Window_defaultRefreshRate = 31.25f;

Window Window_make(const wchar* title, u32 width, u32 height) {
    return (Window){
        .title_           = title ? title : Window_defaultTitle,
        .width_           = 0 <= width ? width : Window_defaultWidth,
        .height_          = 0 <= height ? height : Window_defaultHeight,
        .refreshRate_     = Window_defaultRefreshRate,
        .usesRefreshRate_ = true,
    };
};
