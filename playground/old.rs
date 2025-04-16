#![forbid(unsafe_code)]
#![warn(clippy::all)]

use std::{
    collections::{BTreeMap, BTreeSet, HashMap},
    sync::{Arc, Condvar, Mutex},
};

type Token = u64;

#[derive(Debug, Default)]
struct Element {
    value: i64,
    is_locked: Option<Token>,
}

#[derive(Debug, Default)]
struct WrappedElement {
    elem: Mutex<Element>,
    unlocked_notifier: Condvar,
}

#[derive(Debug)]
enum Request {
    Read {
        indices: BTreeSet<usize>,
    },
    Write {
        token: Token,
        values: HashMap<usize, i64>,
    },
}

#[derive(Debug)]
enum Response {
    ReadReply { values: BTreeMap<usize, i64> },
    WriteSuccessful,
    InvalidRequest,
}

#[derive(Debug, Default)]
struct ServerState {
    data: Arc<Vec<WrappedElement>>,
    next_token: Arc<Mutex<u64>>,
}

impl ServerState {
    /// Creates a new server with `len` elements
    fn new(len: usize) -> Self {
        Self {
            data: Arc::new((0..len).map(|_| WrappedElement::default()).collect()),
            next_token: Arc::new(Mutex::new(0)),
        }
    }

    fn handle_request(&self, request: Request) -> Response {
        match request {
            Request::Read { indices } => {
                // Decide on a token for this transaction
                let mut next_token = self.next_token.lock().unwrap();
                let current_token = *next_token;
                *next_token += 1;
                drop(next_token);

                // Lock and read all the requested index-value pairs
                let mut values = BTreeMap::new();
                for i in indices {
                    // Return an error if the requested index is non-existent
                    let Some(element) = &self.data.get(i) else {
                        return Response::InvalidRequest;
                    };

                    // Wait until no transaction has this item "locked"
                    let elem_guard = element.elem.lock().unwrap();
                    let mut elem_guard = element
                        .unlocked_notifier
                        .wait_while(elem_guard, |elem| elem.is_locked.is_none())
                        .unwrap();

                    elem_guard.is_locked = Some(current_token);

                    values.insert(i, elem_guard.value);
                }
                return Response::ReadReply { values };
            }

            Request::Write { token, values } => {

            }
        }

        Response::InvalidRequest
    }
}
