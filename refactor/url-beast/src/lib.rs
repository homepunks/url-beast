use crate::handlers::{create_record, health_check, index, redirect};
use axum::{
    Router,
    routing::{get, post},
};
use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use tokio::signal;

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

pub async fn shutdown() {
    let ctrl_c = async {
        signal::ctrl_c().await.expect("failed to listen for ctrl+c");
    };

    #[cfg(unix)]
    let term = async {
        signal::unix::signal(signal::unix::SignalKind::terminate())
            .expect("failed to listen for sigterm")
            .recv()
            .await;
    };

    #[cfg(not(unix))]
    panic!("No. Just no.");

    tokio::select! {
        _ = ctrl_c => "got ctrl+c",
        _ = term => "got sigterm",
    };
    println!("shutting down...");
}
