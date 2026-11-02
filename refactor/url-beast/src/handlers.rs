use crate::{BASE, CODE_LENGTH, Store, models};
use axum::{
    Json,
    extract::{Path, State},
    http::StatusCode,
    response::{Html, Redirect},
};
use serde_json::{Value, json};
use std::collections::hash_map::Entry;

pub async fn health_check() -> Json<Value> {
    /* TIL: axum::Json responses default to status code 200 */
    Json(json!({ "status": "ok" }))
}

pub async fn create_record(
    State(store): State<Store>,
    Json(req): Json<models::NewLinkRequest>,
) -> Result<(StatusCode, Json<models::NewLinkResponse>), (StatusCode, Json<Value>)> {
    let target = url::Url::parse(&req.old_link)
        .ok()
        .filter(|u| matches!(u.scheme(), "http" | "https"))
        .ok_or_else(|| {
            (
                StatusCode::BAD_REQUEST,
                Json(json!({ "error": "invalid url"})),
            )
        })?;

    let code = {
        let mut map = store.lock().unwrap();
        loop {
            let code = nanoid::nanoid!(CODE_LENGTH, &nanoid::alphabet::SAFE);
            if let Entry::Vacant(slot) = map.entry(code.clone()) {
                slot.insert(target.to_string());
                break code;
            }
        }
    };

    println!("generated `{code}` for `{target}`");
    Ok((
        StatusCode::CREATED,
        Json(models::NewLinkResponse {
            new_link: format!("{}/{}", BASE, code),
        }),
    ))
}

pub async fn redirect(
    State(store): State<Store>,
    Path(code): Path<String>,
) -> Result<Redirect, (StatusCode, Json<Value>)> {
    let map = store.lock().unwrap();
    map.get(&code)
        .map(|url| Redirect::temporary(url))
        .ok_or_else(|| (StatusCode::NOT_FOUND, Json(json!({ "error": "not found" }))))
}

pub async fn index() -> Html<&'static str> {
    Html(include_str!("../static/index.html"))
}
