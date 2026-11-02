use crate::handlers::{create_record, health_check, index, redirect};
use axum::{
    Router,
    routing::{get, post},
};
use std::collections::HashMap;
use std::sync::{Arc, Mutex};

pub mod handlers;
pub mod models;

pub const HOST: &str = "0.0.0.0:6767";
pub const BASE: &str = "http://127.0.0.1:6767";
pub const CODE_LENGTH: usize = 6;

pub type Store = Arc<Mutex<HashMap<String, String>>>;

pub fn init() -> Router {
    let store: Store = Arc::new(Mutex::new(HashMap::new()));
    Router::new()
        .route("/", get(index))
        .route("/health", get(health_check))
        .route("/new", post(create_record))
        .route("/{code}", get(redirect))
        .with_state(store)
}
