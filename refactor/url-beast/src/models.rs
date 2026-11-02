use serde::{Serialize, Deserialize};

#[derive(Serialize)]
pub struct NewLinkResponse {
    pub new_link: String,
}

#[derive(Deserialize)]
pub struct NewLinkRequest {
    pub old_link: String,
}
