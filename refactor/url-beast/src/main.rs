use anyhow::Context;
use tokio::net;
use url_beast::{HOST, shutdown};

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let ln = net::TcpListener::bind(HOST)
        .await
        .with_context(|| format!("failed to bind {HOST}"))?;
    println!("Up and running on {}", HOST);
    axum::serve(ln, url_beast::init())
        .with_graceful_shutdown(shutdown())
        .await
        .context("server error")?;
    Ok(())
}
