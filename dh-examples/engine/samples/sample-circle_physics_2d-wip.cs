using System;
using System.Collections.Generic;
using System.Numerics; // Vector2용

public class Ball
{
    public Circle transform;
    public Vector2 vel;
    public Vector2 acc;
    public float mass;

    public const float MassScalerByRadius = 10.0f;
    public const float VelocityTolerance = 0.01f;
    public const float DragCoefficient = 0.8f;

    public Ball(float x, float y, float radius)
    {
        this.transform = new Circle { Center = new Vector2(x, y), Radius = radius };
        this.vel = Vector2.Zero;
        this.acc = Vector2.Zero;
        this.mass = radius * MassScalerByRadius;
    }
}

public class BallManager : IDisposable
{
    public List<Ball> balls;
    public Ball? selected_ball;
    private Random rng;
    // private Allocator gpa; (C#은 GC가 관리하므로 생략 가능하나 의미상 남김)

    public BallManager(Random rng)
    {
        this.balls = new List<Ball>(32);
        this.selected_ball = null;
        this.rng = rng;
        // .gpa = gpa (GC가 자동 처리)
    }

    public void Dispose()
    {
        balls.Clear();
    }

    public void ReplaceAllRandomly(int count)
    {
        balls.Clear();
        if (balls.Capacity < count) { balls.Capacity = count; }
        for (int i = 0; i < count; i++)
        {
            balls.Add(new Ball(
                rng.GetRangeFloat(-Global.WindowResX / 2, Global.WindowResX / 2),
                rng.GetRangeFloat(-Global.WindowResY / 2, Global.WindowResY / 2),
                rng.GetRangeFloat(2.0f, 7.0f)
            ));
        }
        foreach (var ball in balls)
        {
            ball.mass = ball.transform.Radius * Ball.MassScalerByRadius;
        }
    }
}

public static class Global
{
    public const float WindowResX = 1920f;
    public const float WindowResY = 1080f;
}

public struct Vector2
{
    public float X;
    public float Y;
}

public class Random
{
    public float NextDouble()
    {
        return (float)rng.NextDouble();
    }
}

public class List<T>
{
    public List(int capacity)
    {
        this.capacity = capacity;
    }

    public void Add(T item)
    {
        items.Add(item);
    }

    public void Clear()
    {
        items.Clear();
    }

    public void Remove(T item)
    {
        items.Remove(item);
    }
}

public struct Circle
{
    public Vector2 Center;
    public float Radius;
}

public static class Extension
{
    public static float GetRangeFloat(this Random rng, float min, float max)
    {
        return (float)rng.NextDouble() * (max - min) + min;
    }
}
