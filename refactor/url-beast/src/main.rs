use axum::{routing::{get, post}, Router, Json, extract::State, http::StatusCode};
use serde_json::{json, Value};
use tokio::net;
use anyhow::Context;

use std::{
    sync::{Arc, Mutex},
    collections::{HashMap, hash_map::Entry},
};

use url_beast::models;

const HOST: &str = "0.0.0.0:6767";
// const BASE: &str = "127.0.0.1;6767";
const CODE_LENGTH: usize = 6;

type Store = Arc<Mutex<HashMap<String, String>>>;

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let store: Store = Arc::new(Mutex::new(HashMap::new()));

    let app = Router::new()
        .route("/health", get(health_check))
        .route("/new", post(create_record))
        .with_state(store);
        
    let ln = net::TcpListener::bind(HOST).await
        .with_context(|| format!("failed to bind {HOST}"))?;
    println!("Up and running on {}", HOST);

    axum::serve(ln, app).await.context("server error")?;
    Ok(())
}

async fn health_check() -> Json<Value> {
    /* TIL: axum::Json responses default to status code 200 */
    Json(json!({ "status": "ok" }))
}

async fn create_record(
    State(store): State<Store>,
    Json(req): Json<models::NewLinkRequest>,
) -> Result<(StatusCode, Json<models::NewLinkResponse>), (StatusCode, Json<Value>)> {
    let target = url::Url::parse(&req.old_link)
        .ok()
        .filter(|u| matches!(u.scheme(), "http" | "https"))
        .ok_or_else(|| (StatusCode::BAD_REQUEST, Json(json!({ "error": "invalid url"}))))?;

    let code = {
        let mut map = store.lock().unwrap();
        loop {
            let code = nanoid::nanoid!(CODE_LENGTH, &nanoid::alphabet::SAFE);
            if let Entry::Vacant(slot) = map.entry(code.clone()) {
                slot.insert(target.to_string());
                println!("generated `{code}` for `{link}`", link = req.old_link);
                break code;
            }
        }
    };
    
    Ok((StatusCode::CREATED,
     Json(models::NewLinkResponse{
        new_link: code,
    })))
}
