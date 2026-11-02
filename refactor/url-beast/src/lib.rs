use axum::{
    Router,
    routing::{get, post},
};
use std::sync::{Arc, Mutex};
use std::collections::HashMap;
use crate::{handlers::{index, create_record, redirect, health_check}};

pub mod models;
pub mod handlers;

pub const HOST: &str = "http://0.0.0.0:6767";
pub const BASE: &str = "127.0.0.1:6767";
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
