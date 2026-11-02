use anyhow::Context;
use tokio::net;
use url_beast::{init, HOST};

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let ln = net::TcpListener::bind(HOST)
        .await
        .with_context(|| format!("failed to bind {HOST}"))?;
    let app = init();
    println!("Up and running on {}", HOST);

    axum::serve(ln, app).await.context("server error")?;
    Ok(())
}
