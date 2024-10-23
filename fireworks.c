/* use anyhow::Result;
use crossterm::style::Print;
use crossterm::terminal::{ Clear, ClearType };
use crossterm::{ cursor, execute, terminal };
use pixel_loop::canvas::CrosstermCanvas;
use pixel_loop::input::{ CrosstermInputState, KeyboardKey, KeyboardState };
use pixel_loop::{ Canvas, Color, HslColor, RenderableCanvas }; */


#include "src/assert.h"
#include "src/color.h"
#include "src/floats.h"
#include "src/primitive_types.h"


f64 Rand_Random_f64() { return (f64)rand() / (f64)RAND_MAX; }


typedef struct Particle Particle;
struct Particle {
    f64   position[2];
    f64   speed[2];
    f64   acceleration[2];
    f64   fading;
    f64   lifetime;
    Color color;
    u32   dimensions[2];
};
#define Particle_(...)                                 ((Particle){ __VA_ARGS__ })
#define Particle_make(_x, _y, _width, _height, _color) (      \
    Particle_(                                                \
            .position     = { (f64)(_x), (f64)(_y) },         \
            .speed        = { 0.0, 0.0 },                     \
            .acceleration = { 0.0, 0.0 },                     \
            .fading       = 0.0,                              \
            .lifetime     = 1.0,                              \
            .color        = (_color),                         \
            .dimensions   = { (u32)(_width), (u32)(_height) } \
    )                                                         \
)
Particle* Particle_withSpeed(RefT(Particle) self, f64 x, f64 y) {
    Assert(self);
    self->speed[0] = x;
    self->speed[1] = y;
    return self;
}
Particle* Particle_withAcceleration(RefT(Particle) self, f64 x, f64 y) {
    Assert(self);
    self->acceleration[0] = x;
    self->acceleration[1] = y;
    return self;
}
Particle* Particle_withFading(RefT(Particle) self, f64 fading) {
    Assert(self);
    self->fading = fading;
    return self;
}
bool Particle_IsDead(RefT(Particle) self) {
    Assert(self);
    return self->lifetime <= 0.0;
}
void Particle_Update(RefT(Particle) self) {
    Assert(self);
    if (Particle_IsDead(self)) { return; }

    self->speed[0] += self->acceleration[0];
    self->speed[1] += self->acceleration[1];

    self->position[0] += self->speed[0];
    self->position[1] += self->speed[1];

    self->lifetime -= self->fading;
}
void Particle_Render(RefT(Particle) self) {
    Assert(self);
    if (Particle_IsDead(self)) { return; }
}

/* impl Particle {
    pub fn render<C : Canvas>(&self, canvas : &mut C) -> Result<()> {
        if self {
            .lifetime <= 0.0 {
                return Ok(());
            }
        }

        // @HACK: PixelLoop with CrosstermCanvas does not support proper alpha
        // blending at the moment. Therefore we calculate the coler against a
        // given base (black) and the lifetime as opacity and apply it.
        let render_color = Color::from_rgb(
            (self.color.r as f64 * self.lifetime) as u8,
            (self.color.g as f64 * self.lifetime) as u8,
            (self.color.b as f64 * self.lifetime) as u8,
        );

        canvas.filled_rect(
            self.position .0.round() as i64,
            self.position .1.round() as i64,
            self.dimensions .0,
            self.dimensions .1,
            &render_color,
        );
        Ok(())
    }
} */


typedef struct Firework Firework;
struct Firework {
    Particle* rocket;
    Particle  effect[125];
    i64       effectCount;
    ColorHSL  effectBaseColor;
};
Firework Firework_new(i64 x, i64 y, Color effectBaseColor) {
    Firework firework = {
        .effectCount     = 0,
        .effectBaseColor = ColorHSL_fromRGBA(effectBaseColor),
    };
    Particle* const rocket = (Particle*)malloc(sizeof(Particle));
    rocket[0]              = Particle_make(x, y, 1, 3, Color_white);
    firework.rocket        = rocket;
    Particle_withSpeed(firework.rocket, 0.0, -2.0 - Rand_Random_f64() * -1.0);
    // .with_speed(0.0, -2.0 - rand::random::<f64>() * -1.0)
    Particle_withAcceleration(firework.rocket, 0.0, 0.02);
    return firework;
    /*
    pub fn new (x : i64, y : i64, effect_base_color : Color)->Self {
        let rocket = Some(
            Particle::new (x, y, 1, 3, Color::from_rgb(255, 255, 255))
                // Rocket flies upwards with gravity pulling it down.
                // Initial speed slightly randomized.
                .with_acceleration(0.0, 0.02),
        );

        Self {
            rocket,
                effect : vec ![],
                         effect_base_color : effect_base_color.as_hsl(),
        }
    }
    */
}
void Firework_Update(RefT(Firework) self) {
    if (self->rocket) {
        Particle* const rocket = self->rocket;
        Particle_Update(rocket);

        if (-0.2 <= rocket->speed[1]) {
            for (i64 i = 0; i < 25; ++i) {
                i64 const      x      = (i64)rocket->position[0];
                i64 const      y      = (i64)rocket->position[1];
                i64 const      width  = 1;
                i64 const      height = 1;
                ColorHSL const color  = ColorHSL_from(
                    self->effectBaseColor.h,
                    self->effectBaseColor.s + (Rand_Random_f64() - 0.5) * 20.0,
                    self->effectBaseColor.l + (Rand_Random_f64() - 0.5) * 40.0,
                );
                Particle particle[1] = { Particle_make(x, y, width, height, ColorHSL_toRGB(color)) };
                Particle_withSpeed(
                    particle,
                    (Rand_Random_f64() - 0.5) * 1.0,
                    (Rand_Random_f64() - 0.9) * 1.0
                );
                Particle_withAcceleration(particle, 0.0, 0.02);
                Particle_withFading(particle, 0.01);
                self->effect[self->effectCount++] = particle[0];
            }
            self->rocket = nullptr;
        }
    }

    for (Particle* particle = self->effect; particle < self->effect + self->effectCount; ++particle) {
        Particle_Update(particle);
    }

    /* pub fn update(&mut self) {
        if let {
            Some(ref mut rocket) = self.rocket {
                rocket.update();

                if rocket {
                    .speed .1 >= -0.2 {
                        // Rocket has reached its peak and is now exploding.
                        // Create a bunch of particles to simulate the explosion.
                    for{
                        _ in 0..25 {
                            let x      = rocket.position .0 as i64;
                            let y      = rocket.position .1 as i64;
                            let width  = 1;
                            let height = 1;
                            // Randomize color based on the base color of the rocket. using the hsl form
                            // of the color.
                            let color = HslColor::new (
                                self.effect_base_color.h,
                                self.effect_base_color.s + (rand::random::<f64>() - 0.5) * 20.0,
                                self.effect_base_color.l + (rand::random::<f64>() - 0.5) * 40.0,
                            );

                            let particle = Particle::new (x, y, width, height, color.into())
                                               .with_speed(
                                                   (rand::random::<f64>() - 0.5) * 1.0,
                                                   (rand::random::<f64>() - 0.9) * 1.0,
                                               )
                                               .with_acceleration(0.0, 0.02)
                                               .with_fading(0.01);
                            self.effect.push(particle);
                        }
                    }
                    self.rocket = None;
                    }
                }
            }
        }

        for{
            particle in& mut self.effect {
                particle.update();
            }
        }
    } */
}
void Firework_Render(RefT(Firework) self) {
    if (self->rocket) {
        Particle_Render(self->rocket);
    }
    for (Particle* particle = self->effect; particle < self->effect + self->effectCount; ++particle) {
        Particle_Render(particle);
    }
}
static bool Firework__DeadsAllEffect(RefT(Firework) self) {
    for (Particle* particle = self->effect; particle < self->effect + self->effectCount; ++particle) {
        if (!Particle_IsDead(particle)) {
            return false;
        }
    }
    return true;
}
bool Firework_IsDead(RefT(Firework) self) {
    return self->rocket == nullptr && (self->effectCount == 0 || Firework__DeadsAllEffect(self));
}

typedef struct State State;
struct State {
    Firework fireworks[16];
    i64      fireworksCount;
    u32      width;
    u32      height;
};
State State_new(u32 width, u32 height) {
    State state          = { 0 };
    state.fireworksCount = 0;
    state.width          = width;
    state.height         = height;
    return state;
}

int main() {
    State state = State_new(160, 100);
}

/* fn main() -> Result<()> {
    let(terminal_width, terminal_height) = terminal::size() ? ;
    let width                            = terminal_width;
    let height                           = terminal_height * 2;

    let mut canvas = CrosstermCanvas::new (width, height);
    canvas.set_refresh_limit(120);

    let state = State::new (width as u32, height as u32);

    let input = CrosstermInputState::new ();

    pixel_loop::run(
        60,
        state,
        input,
        canvas,
        | e,
        s,
        input,
        canvas | {
            let width = canvas.width();
            let height = canvas.height();

            if input{.is_key_pressed(KeyboardKey::Q) {
                // @HACK until we refactored PixelLoop to allow for a clean
                // exit.
                let mut stdout = std::io::stdout();
                execute!(
                    stdout,
                    Clear(ClearType::All), // Clear all on screen
                    cursor::MoveTo(0, 0),  // Reset cursor position
                    Print("\x1b[!p"),      // Soft terminal reset (DECSTR)
                    Print("\x1bc"),        // Full terminal reset (RIS)
                )?;
                crossterm::terminal::disable_raw_mode()?;
                std::process::exit(0);
            }}

            // eprintln!("Active fireworks: {}", s.fireworks.len());

            // Remove dead fireworks.
            s.fireworks.retain(|f| !f.is_dead());

            // Add a new rocket with with 5% chance.
            if rand{::random::<f64>() < 0.05 {
                let x = (rand::random::<u32>() % width) as i64;
                let y = height as i64;
                let effect_base_color = Color::from_rgb(
                    rand::random::<u8>(),
                    rand::random::<u8>(),
                    rand::random::<u8>(),
                );

                s.fireworks.push(Firework::new(x, y, effect_base_color));
            }
}

            for{ firework in &mut s.fireworks {
                firework.update();
            }
}

            Ok(()) },
        | e,
        s,
        i,
        canvas,
        dt | {
            // RENDER BEGIN
            canvas.clear_screen(&Color::from_rgb(0, 0, 0));

            for{ firework in &s.fireworks {
                firework.render(canvas)?;
            }
}

            // RENDER END

            canvas.render()?;

            Ok(()) },
    )
        ? ;
    Ok(())
} */
