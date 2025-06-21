#![forbid(unsafe_code)]
#![warn(clippy::all)]
mod ordered_locking;

use ordered_locking::{LockProof, Mutex};

fn main() {
    let mut unlocked_ctx = LockProof::unlocked_state();
    let a: Mutex<u32, 4> = Mutex::new(32);
    let b: Mutex<u32, 5> = Mutex::new(32);
    let c: Mutex<u32, 6> = Mutex::new(32);

    let (mut guard_a, mut a_ctx) = a.lock(&mut unlocked_ctx);

    let (mut guard_c, mut c_ctx) = c.lock(&mut a_ctx);

    *guard_a += 1;

    *guard_c += 1;

    drop(guard_a);
    drop(guard_c);

    let (mut guard_b, mut b_ctx) = b.lock(&mut unlocked_ctx);
}
