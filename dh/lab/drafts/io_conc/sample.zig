const std = @import("std");

const Io = std.Io;

const Sys = struct {
    io: Io,
};

fn report(label: []const u8, comptime fmt: []const u8, args: anytype) void {
    std.debug.print("[{s}] " ++ fmt ++ "\n", .{label} ++ args);
}

fn count(sys: Sys, n: usize, interval: Io.Duration, label: []const u8) f64 {
    const start = Io.Timestamp.now(sys.io, .awake);
    report(label, "before loop {d:.1}", .{secs(interval)});

    for (0..n) |i| {
        Io.sleep(sys.io, interval, .awake) catch {};
        report(label, "slept {d:.1} | i: {d} < n: {d}", .{ secs(interval), i, n });
    }

    const elapsed = start.durationTo(Io.Timestamp.now(sys.io, .awake));
    report(label, "after loop {d:.1}", .{secs(elapsed)});
    return secs(elapsed);
}

fn secs(duration: Io.Duration) f64 {
    return @as(f64, @floatFromInt(duration.toNanoseconds())) / @as(f64, @floatFromInt(std.time.ns_per_s));
}

pub fn main() !void {
    var debug_allocator: std.heap.DebugAllocator(.{}) = .init;
    defer _ = debug_allocator.deinit();

    var threaded: Io.Threaded = .init(debug_allocator.allocator(), .{});
    defer threaded.deinit();

    const io = threaded.io();
    const sys: Sys = .{ .io = io };

    std.debug.print("begin\n", .{});
    var task_a = Io.async(io, count, .{ sys, 2, Io.Duration.fromSeconds(1), "task a" });
    defer Io.cancel(io, task_a) catch {};
    var task_b = Io.async(io, count, .{ sys, 3, Io.Duration.fromMilliseconds(600), "task b" });
    defer Io.cancel(io, task_b) catch {};

    var total: f64 = 0;
    total += task_a.await(io);
    total += task_b.await(io);

    std.debug.print("end\n", .{});
    std.debug.print("total: {d:.1}\n", .{total});
}
