#include <dh-main.h>
#include <dh/HashSet.h>
#include <dh/ArrList.h>
#include <dh/heap/Sys.h>
#include <dh/heap/Arena.h>
#include <dh/io/stream.h>

T_alias$((Behaviour_Params)(struct Behaviour_Params) {
    var_(id, usize);
    var_(name, S_const$u8);
});
T_alias$((Behaviour)(struct Behaviour) {
    var_(id, usize);
    var_(name, S_const$u8);
});
$static fn_((Behaviour_init(Behaviour_Params params))(Behaviour));

T_alias$((Food)(struct Food) {
    var_(name, S_const$u8);
});
T_use_prl$(Food);
$static fn_((Food_init(S_const$u8 name))(Food));
T_alias$((FoodEatable)(struct FoodEatable) {
    var_(ctx, const Behaviour*);
    fn_(((*eatFoodFn)(const Behaviour* ctx, Food food))(void));
});
$static fn_((FoodEatable_eatFood(FoodEatable self, Food food))(void));

T_alias$((PelicanLike)(Behaviour));
$static fn_((PelicanLike_behaviour(Behaviour_Params params))(PelicanLike));
$static fn_((PelicanLike_foodEatable(PelicanLike* self))(FoodEatable));

T_alias$((Skill)(struct Skill) {
    var_(name, S_const$u8);
});
T_use_prl$(Skill);
T_use$((Skill)(ArrList));
$static let_(Skill_own, Skill) = { .name = u8_l("Own") };
$static let_(Skill_c, Skill) = { .name = u8_l("C") };
$static let_(Skill_cpp, Skill) = { .name = u8_l("C++") };
$static let_(Skill_csharp, Skill) = { .name = u8_l("C#") };
$static let_(Skill_zig, Skill) = { .name = u8_l("Zig") };
$static let_(Skill_rust, Skill) = { .name = u8_l("Rust") };
$static let_(Skill_go, Skill) = { .name = u8_l("Go") };
$static let_(Skill_python, Skill) = { .name = u8_l("Python") };
$static let_(Skill_unity, Skill) = { .name = u8_l("Unity") };
$static let_(Skill_godot, Skill) = { .name = u8_l("Godot") };
$static let_(Skill_git, Skill) = { .name = u8_l("Git") };
$static let_(Skill_vscode, Skill) = { .name = u8_l("VSCode") };
T_alias$((SkillSet)(struct SkillSet) {
    var_(languages, S_const$Skill);
    var_(engines, S_const$Skill);
    var_(tools, S_const$Skill);
});
T_use_prl$(SkillSet);

T_use$((Food)(ArrList, ArrList_appendFixed));
T_alias$((SWEngineer_Cfg)(struct SWEngineer_Cfg) {
    var_(gpa, mem_Alctr);
    var_(hash_ctx, P_const$HashMap_Ctx);
    var_(behaviour, Behaviour);
    var_(hobby, S_const$u8);
    var_(main_domain, S_const$u8);
    var_(core_skills, SkillSet);
    var_(special_skills, S_const$u8);
});
T_use$((S_const$u8)(HashSet));
T_use$((S_const$u8, ArrList$Skill)(HashMap));
T_alias$((SWEngineer)(struct SWEngineer {
    var_(behaviour, Behaviour);
    var_(hobby, HashSet$S_const$u8);
    var_(main_domain, S_const$u8);
    var_(core_skills, HashMap$1S_const$u8$2ArrList$Skill);
    var_(special_skills, HashSet$S_const$u8);
    var_(favorite_foods, ArrList$Food);
    var_(hash_ctx, P_const$HashMap_Ctx);
    var_(gpa, mem_Alctr);
}));
T_use_E$($set(mem_E)(SWEngineer));
$attr($must_check)
$static fn_((SWEngineer_init(SWEngineer_Cfg cfg))(mem_E$SWEngineer));
$static fn_((SWEngineer_fini(SWEngineer* self))(void));

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    var_(heap, heap_Sys) = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var_(arena, heap_Arena) = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    let hash_ctx = HashMap_Ctx_default();

    let own = Skill_own;
    let c = Skill_c;
    let cpp = Skill_cpp;
    let csharp = Skill_csharp;
    let zig = Skill_zig;
    let rust = Skill_rust;
    let go = Skill_go;
    let python = Skill_python;
    let unity = Skill_unity;
    let godot = Skill_godot;
    let git = Skill_git;
    let vscode = Skill_vscode;

    var dev_dasae = try_(SWEngineer_init(l$((SWEngineer_Cfg){
        .gpa = gpa,
        .hash_ctx = &hash_ctx,
        .behaviour = PelicanLike_behaviour(l$((Behaviour_Params){
            .id = compHash("coding-pelican"),
            .name = u8_l("Gyeongtae Kim"),
        })),

        .hobby = u8_l(
            "📸 Photography\n"
            "🏃 Running & Jogging\n"
            "🗣️ Learning languages\n"
        ),

        .main_domain = u8_l("Game Engine Development"),

        .core_skills = l$((SkillSet){
            .languages = NT_s$(8, Skill, { own, c, cpp, csharp, zig, rust, go, python }).as_const,
            .engines = NT_s$(3, Skill, { own, unity, godot }).as_const,
            .tools = NT_s$(3, Skill, { own, git, vscode }).as_const,
        }),

        .special_skills = u8_l(
            "🐢 Proud owner of an epic tech neck\n"
            "🦖 Cheers every time code execution speeds up by 0.01 seconds\n"
            "🏆 On a record-breaking caffeine-free streak (since 2022-09-05)\n"
        ),
    })));
    defer_(SWEngineer_fini(&dev_dasae));

    let favorite_foods = &dev_dasae.favorite_foods;
    try_(ArrList_appendFixed$Food(favorite_foods, Food_init(u8_l("🍕Pizza"))));
    try_(ArrList_appendFixed$Food(favorite_foods, Food_init(u8_l("🐔Chicken"))));

    let eatable = PelicanLike_foodEatable(&dev_dasae.behaviour);
    for_(($rf(0), $s(favorite_foods->items))(idx, food)) {
        io_stream_println(u8_l("{:uz}: {:s}"), idx + 1, food->name);
        FoodEatable_eatFood(eatable, *food);
    } $end(for);

    // output:
    // 1: 🍕Pizza
    // 'Gyeongtae Kim' swallows '🍕Pizza' without chewing!
    // 2: 🐔Chicken
    // 'Gyeongtae Kim' swallows '🐔Chicken' without chewing!
    return_ok({});
} $unguarded(fn);

fn_((Behaviour_init(Behaviour_Params params))(Behaviour)) {
    return (Behaviour){ .id = params.id, .name = params.name };
};

fn_((Food_init(S_const$u8 name))(Food)) {
    return (Food){ .name = name };
};
fn_((FoodEatable_eatFood(FoodEatable self, Food food))(void)) {
    return call((self.eatFoodFn)(self.ctx, food));
};

fn_((PelicanLike_behaviour(Behaviour_Params params))(PelicanLike)) {
    return Behaviour_init(params);
};
$static fn_((PelicanLike__eatFood(const PelicanLike* ctx, Food food))(void));
fn_((PelicanLike_foodEatable(PelicanLike* self))(FoodEatable)) {
    return (FoodEatable){ .ctx = self, .eatFoodFn = PelicanLike__eatFood };
};
fn_((PelicanLike__eatFood(const PelicanLike* ctx, Food food))(void)) {
    io_stream_println(u8_l("{:s} swallows {:s} without chewing!"), ctx->name, food.name);
};

T_use$((Skill)(
    (P, ArrList), (O, P, ArrList),
    ArrList_init, ArrList_fini, ArrList_appendWithin
));
T_use$((Food)(ArrList_init, ArrList_fini));
T_use$((S_const$u8)(HashSet_init, HashSet_fini, HashSet_put));
T_use$((S_const$u8, ArrList$Skill)(
    HashMap_init, HashMap_fini, HashMap_put,
    HashMap_ValIter, HashMap_valIter, HashMap_ValIter_nextMut
));
T_use$((u8)(mem_Delim, mem_TokzIter, mem_tokzUnit, mem_TokzIter_next));
fn_((SWEngineer_init(SWEngineer_Cfg cfg))(mem_E$SWEngineer) $guard) {
    let gpa = mem_Alctr_ensureValid(cfg.gpa);
    let hash_ctx = cfg.hash_ctx;
    let behaviour = cfg.behaviour;

    var hobby = expr_(HashSet$S_const$u8 $guard)({
        var set = try_(HashSet_init$S_const$u8(hash_ctx, gpa, 32));
        errdefer_($ignore, HashSet_fini$S_const$u8(&set, gpa));
        var tokz = mem_tokzUnit$u8(cfg.hobby, u8_c('\n'));
        while_some(mem_TokzIter_next$u8(&tokz), tok) {
            try_(HashSet_put$S_const$u8(&set, gpa, tok));
        };
        $break_(set);
    }) $unguarded(expr);
    errdefer_($ignore, HashSet_fini$S_const$u8(&hobby, gpa));

    let main_domain = cfg.main_domain;

    var core_skills = expr_(HashMap$1S_const$u8$2ArrList$Skill $guard)({
        var set = try_(HashMap_init$1S_const$u8$2ArrList$Skill(hash_ctx, gpa, 3));
        errdefer_($ignore, HashMap_fini$1S_const$u8$2ArrList$Skill(&set, gpa));

        let skill_keys = A_from$((S_const$u8){
            u8_l(fieldName$(cfg.core_skills, languages)),
            u8_l(fieldName$(cfg.core_skills, engines)),
            u8_l(fieldName$(cfg.core_skills, tools)),
        });
        let skill_vals_list = A_from$((S_const$Skill){
            cfg.core_skills.languages,
            cfg.core_skills.engines,
            cfg.core_skills.tools,
        });
        for_(($a(skill_keys), $a(skill_vals_list))(key, vals)) loop_defer {
            var list = try_(ArrList_init$Skill(gpa, vals->len));
            errdefer_($ignore, ArrList_fini$Skill(&list, gpa));
            for_(($s(*vals))(val)) {
                ArrList_appendWithin$Skill(&list, *val);
            } $end(for);
            try_(HashMap_put$1S_const$u8$2ArrList$Skill(&set, gpa, *key, list));
        } loop_deferral $end(for);

        $break_(set);
    }) $unguarded(expr);
    errdefer_($ignore, {
        var iter = HashMap_valIter$1S_const$u8$2ArrList$Skill(core_skills);
        while_some(HashMap_ValIter_nextMut$1S_const$u8$2ArrList$Skill(&iter), val) {
            ArrList_fini$Skill(val, gpa);
        };
        HashMap_fini$1S_const$u8$2ArrList$Skill(&core_skills, gpa);
    });

    var special_skills = expr_(HashSet$S_const$u8 $guard)({
        var set = try_(HashSet_init$S_const$u8(hash_ctx, gpa, 32));
        errdefer_($ignore, HashSet_fini$S_const$u8(&set, gpa));
        var tokz = mem_tokzUnit$u8(cfg.special_skills, u8_c('\n'));
        while_some(mem_TokzIter_next$u8(&tokz), tok) {
            try_(HashSet_put$S_const$u8(&set, gpa, tok));
        };
        $break_(set);
    }) $unguarded(expr);
    errdefer_($ignore, HashSet_fini$S_const$u8(&special_skills, gpa));

    var favorite_foods = try_(ArrList_init$Food(gpa, 8));
    errdefer_($ignore, ArrList_fini$Food(&favorite_foods, gpa));

    return_ok({
        .behaviour = behaviour,
        .hobby = hobby,
        .main_domain = main_domain,
        .core_skills = core_skills,
        .special_skills = special_skills,
        .favorite_foods = favorite_foods,
        .hash_ctx = hash_ctx,
        .gpa = gpa,
    });
} $unguarded(fn);

fn_((SWEngineer_fini(SWEngineer* self))(void)) {
    ArrList_fini$Food(&self->favorite_foods, self->gpa);
    HashSet_fini$S_const$u8(&self->special_skills, self->gpa);
    using_(var iter = HashMap_valIter$1S_const$u8$2ArrList$Skill(self->core_skills)) {
        while_some(HashMap_ValIter_nextMut$1S_const$u8$2ArrList$Skill(&iter), val) {
            ArrList_fini$Skill(val, self->gpa);
        };
        HashMap_fini$1S_const$u8$2ArrList$Skill(&self->core_skills, self->gpa);
    };
    HashSet_fini$S_const$u8(&self->hobby, self->gpa);
};
