use std::time::Instant;
use tokio::sync::oneshot;
use tokio::time::{sleep, Duration};

fn report(label: &str, msg: String) {
    println!("[{}] {}", label, msg);
}

async fn count(
    n: usize,
    interval: Duration,
    label: &'static str,
    started: oneshot::Sender<()>,
) -> f64 {
    let start = Instant::now();
    report(label, format!("before loop {:.1}", interval.as_secs_f64()));
    let _ = started.send(());

    for i in 0..n {
        sleep(interval).await;
        report(
            label,
            format!("slept {:.1} | i: {} < n: {}", interval.as_secs_f64(), i, n),
        );
    }

    let elapsed = start.elapsed().as_secs_f64();
    report(label, format!("after loop {:.1}", elapsed));
    elapsed
}

#[tokio::main(flavor = "current_thread")]
async fn main() {
    println!("begin");

    let (started_a_tx, started_a_rx) = oneshot::channel();
    let (started_b_tx, started_b_rx) = oneshot::channel();

    let task_a = tokio::spawn(count(
        2,
        Duration::from_secs_f64(1.0),
        "task a",
        started_a_tx,
    ));
    let _ = started_a_rx.await;

    let task_b = tokio::spawn(count(
        3,
        Duration::from_secs_f64(0.6),
        "task b",
        started_b_tx,
    ));
    let _ = started_b_rx.await;

    let total = task_a.await.unwrap() + task_b.await.unwrap();

    println!("end");
    println!("total: {:.1}", total);
}
