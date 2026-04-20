#include <dh-main.h>
#include <dh/io/stream.h>

/* clang-format off */
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var dev_dasae = SWEngineer_init(l$((SWEngineer_Cfg){
        .allocator = allocator,
        .behaviour = PelicanLike_behaviour((Behaviour_Params){
            .id   = StrCompHash_create(u8_l("coding-pelican")).value,
            .name = u8_l("Gyeongtae Kim"),
        }),

        .hobby = u8_l(
            "📸 Photography"
            "🏃 Running & Jogging"
            "🗣️ Learning languages"
        ),

        .main_domain = u8_l("Game Engine Development"),

        .core_skills = l$((SkillSet){
            .languages = { own, c, cpp, csharp, zig, rust, go, python },
            .engines   = { own, unity, godot },
            .tools     = { own, git, vscode },
        }),

        .special_skills = u8_l(
            "🐢 Proud owner of an epic tech neck"
            "🦖 Cheers every time code execution speeds up by 0.01 seconds"
            "🏆 On a record-breaking caffeine-free streak (since 2022-09-05)"
        ),
    }));
    defer_(SWEngineer_fini(&dev_dasae));

    let favorite_foods = &dev_dasae->favorite_foods;
    try_(List_append$Food(favorite_foods, Food_init(u8_l("🍕Pizza"))));
    try_(List_append$Food(favorite_foods, Food_init(u8_l("🐔Chicken"))));

    let eatable = PelicanLike_foodEatable(dev_dasae->behaviour);
    for_(($rf(0), $s(favorite_foods->items))(idx, food)) {
        io_stream_println(u8_l("{:uz}: {:s}"), idx + 1, food->name);
        FoodEatable_eatFood(eatable, food);
    } $end(for);

    // output:
    // 1: 🍕Pizza
    // 'Gyeongtae Kim' swallows '🍕Pizza' without chewing!
    // 2: 🐔Chicken
    // 'Gyeongtae Kim' swallows '🐔Chicken' without chewing!
    return_ok({});
} $unguarded(fn);
/* clang-format on */
