// TODO: Port to dh-engine

#include "dce/dce.h"
#include "dce/dce_array.h"



typedef struct _Ball {
    Circle transform;
    Vec2   velocity;
    Vec2   acceleration;
    float  mass;
    size_t id;
} Ball;

Array(Ball);
Array(Vec2);
PtrArray(Ball);
PtrArray(Vec2);

BallArray balls;
Ball*     selectedBall = NULL;

void Ball_Create(float x, float y, float radius) {
    Ball this = {
        .transform    = MakeCircleXY(x, y, radius),
        .velocity     = V2.new(),
        .acceleration = V2.new(),
        .mass         = radius * 10.f,
        .id           = BallArray_Size(balls)
    };
    BallArray_Push(balls, this);
}

void Ball_CreateCirc(Circle circ) {
    Ball this = {
        .transform    = circ,
        .velocity     = V2.new(),
        .acceleration = V2.new(),
        .mass         = circ.radius * 10.f,
        .id           = BallArray_Size(balls)
    };
    BallArray_Push(balls, this);
}

void Ball_Initialize() {
    balls = BallArray_Create(32);
    // Debug_Watch(balls.array->size);

    // Add 16 Random Balls
    for (int i = 0; i < 16; i++) {
        Ball_Create((float)(rand() % Screen_Width()) - Screen_WidthHalf(), (float)(rand() % Screen_Height()) - Screen_HeightHalf(), (float)(rand() % 15 + 2));
        // Ball ball = BallArray_Top(balls);
        // Debug_Watch(ball.id);
        // Debug_Watch((int)ball.transform.position.x);
        // Debug_Watch((int)ball.transform.position.y);
    }
    // Debug_Watch(balls.array->size);
}

void Ball_Finalize() {
    BallArray_Destroy(balls);
}

const float velocityTolerance = 0.01f;



int main(void) { // NOLINT
    dce_Initialize();
    srand((unsigned int)time(NULL)); // NOLINT
    Ball_Initialize();

    BallPtrArray colliders = BallPtrArray_Create(32);
    Debug_Watch(colliders.array->size);
    BallPtrArray collidees = BallPtrArray_Create(32);
    Debug_Watch(collidees.array->size);

    getch();

    while (dce_IsRunning()) {
        Screen_Clear(DCE_BLACK);

        BallPtrArray_Destroy(colliders);
        BallPtrArray_Destroy(collidees);
        colliders             = BallPtrArray_Create(32);
        collidees             = BallPtrArray_Create(32);
        size_t collisionCount = 0;

        // Update Ball Positions
        ForEach(Ball, ball, balls) {
            // Add Drag to emulate rolling friction
            ball->acceleration = V2.mul(V2.negOf(ball->velocity), 0.8f);
            // Update ball physics
            V2.addTo(&ball->velocity, V2.mul(ball->acceleration, Time_DeltaTime()));
            V2.addTo(&ball->transform.position, V2.mul(ball->velocity, Time_DeltaTime()));

            // Wrap the balls around screen
            ball->transform.position = V2.wrap(ball->transform.position, V2.make(-Screen_WidthHalf(), -Screen_HeightHalf()), V2.make(Screen_WidthHalf(), Screen_HeightHalf()));

            // Clamp velocity near zero
            if (AbsOf(V2.LengthSquaredOf(ball->velocity)) < velocityTolerance) {
                ball->velocity = V2.new();
            }
        }
        ForEach_End();

        ForEach(Ball, ball, balls) {
            ForEach(Ball, target, balls) {
                if (ball->id == target->id) {
                    continue;
                }
                // Collision has occurred
                if (!Circ_IntersectsCircle(ball->transform, target->transform)) {
                    continue;
                }
                BallPtrArray_Push(colliders, ball);
                BallPtrArray_Push(collidees, target);
                collisionCount++;

                // Distance between ball centers
                float distance = V2.Distance(ball->transform.position, target->transform.position);

                // Calculate displacement required
                float overlap = 0.5f * (distance - ball->transform.radius - target->transform.radius);

                // Displace Current Ball away from collision
                V2.subTo(&ball->transform.position, V2.div(V2.mul(V2.sub(ball->transform.position, target->transform.position), overlap), distance));

                // Displace Target Ball away from collision
                V2.addTo(&target->transform.position, V2.div(V2.mul(V2.sub(ball->transform.position, target->transform.position), overlap), distance));
            }
            ForEach_End();
        }
        ForEach_End();

        for (size_t i = 0; i < collisionCount; ++i) {
            Ball* collider = BallPtrArray_GetAt(colliders, i);
            Ball* collidee = BallPtrArray_GetAt(collidees, i);

            // Distance between balls
            Vec2  distVec  = V2.sub(collidee->transform.position, collider->transform.position);
            float distance = V2.LengthOf(distVec);

            // Normal vector
            Vec2 normal = V2.div(distVec, distance);

            // Tangent vector
            Vec2 tangent = V2.makeSkew(normal);

            // Dot Product Tangent
            float dpTanCollider = V2.Dot(collider->velocity, tangent);
            float dpTanCollidee = V2.Dot(collidee->velocity, tangent);

            // Dot Product Normal
            float dpNormCollider = V2.Dot(collider->velocity, normal);
            float dpNormCollidee = V2.Dot(collidee->velocity, normal);

            // Conservation of momentum in 1D
            float momentumCollider = (dpNormCollider * (collider->mass - collidee->mass) + 2.0f * collidee->mass * dpNormCollidee) / (collider->mass + collidee->mass);
            float momentumCollidee = (dpNormCollidee * (collidee->mass - collider->mass) + 2.0f * collider->mass * dpNormCollider) / (collider->mass + collidee->mass);

            // Update ball velocities
            collider->velocity = V2.add(V2.mul(tangent, dpTanCollider), V2.mul(normal, momentumCollider));
            collidee->velocity = V2.add(V2.mul(tangent, dpTanCollidee), V2.mul(normal, momentumCollidee));
        }



        // ===== Update =====
        Vec2 mousePos = Input_GetMousePosScreenToWorld();
        if (Input_GetMouse(DCE_BTN_LT).isPressed || Input_GetMouse(DCE_BTN_RT).isPressed) {
            selectedBall = NULL;
            ForEach(Ball, ball, balls) {
                if (Circ_IsPointInCircle(ball->transform, mousePos)) {
                    selectedBall = ball;
                    break;
                }
            }
            ForEach_End();
        }

        if (Input_GetMouse(DCE_BTN_LT).isHeld) {
            if (selectedBall != NULL) {
                selectedBall->transform.position = mousePos;
            }
        }

        if (Input_GetMouse(DCE_BTN_LT).isReleased) {
            selectedBall = NULL;
        }

        if (Input_GetMouse(DCE_BTN_RT).isReleased) {
            if (selectedBall != NULL) {
                selectedBall->velocity = V2.mul(V2.sub(selectedBall->transform.position, mousePos), 5.f);
            }
            selectedBall = NULL;
        }

        if (Input_GetKey(DCE_ESC).isPressed) {
            dce_Exit();
        }



        // ===== Render =====
        // Draw Balls
        ForEach(Ball, ball, balls) {
            Screen_DrawCircle(ball->transform, DCE_WHITE);
        }
        ForEach_End();
        // Draw static collisions
        for (size_t i = 0; i < collisionCount; ++i) {
            Ball* collider = BallPtrArray_GetAt(colliders, i);
            Ball* collidee = BallPtrArray_GetAt(collidees, i);
            Screen_DrawLine(collider->transform.position, collidee->transform.position, DCE_RED);
        }
        // Draw Cue
        if (selectedBall != NULL) {
            Screen_DrawLine(selectedBall->transform.position, mousePos, DCE_BLUE);
        }
    }

    BallPtrArray_Destroy(colliders);
    BallPtrArray_Destroy(collidees);
    Ball_Finalize();

    return 0;
}
