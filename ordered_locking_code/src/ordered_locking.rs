use std::{marker::PhantomData, sync::MutexGuard};

/// Proof that `L` is the greatest Mutex held by this thread/task.
///
/// Pass into [`Mutex::lock`] to prove you can acquire the lock.
pub struct LockProof<'a, const L: i64> {
    /// Tells the compiler that this [`LockProof`]
    /// lives for lifetime `'a`.
    _dummy: PhantomData<&'a ()>,
}

impl LockProof<'_, { i64::MIN }> {
    /// Returns a proof that you're holding no locks.
    ///
    /// Only call at the start of each concurrent task/thread
    /// when you're actually holding no locks.
    pub fn unlocked_state() -> Self {
        Self {
            _dummy: PhantomData,
        }
    }
}

/// Mutex which enforces a locking order.
pub struct Mutex<T, const L: i64> {
    mutex: std::sync::Mutex<T>,
}

impl<T, const L: i64> Mutex<T, L> {
    /// Create a new ordered mutex in the unlocked state.
    pub fn new(t: T) -> Self {
        Self {
            mutex: std::sync::Mutex::new(t),
        }
    }

    /// Lock the mutex.
    pub fn lock<'a, const PREV_L: i64>(
        &'a self,
        _proof: &'a mut LockProof<PREV_L>,
    ) -> (MutexGuard<'a, T>, LockProof<'a, L>) {
        const {
            assert!(
                PREV_L < L,
                "Mutexes must be locked in strictly increasing ID order."
            )
        }
        (
            self.mutex
                .lock()
                .expect("Mutex can't be locked because it was poisoned."),
            LockProof {
                _dummy: PhantomData,
            },
        )
    }
}
