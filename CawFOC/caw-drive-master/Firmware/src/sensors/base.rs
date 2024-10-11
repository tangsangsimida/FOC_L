pub trait BaseSensor {
    async fn update(&mut self) -> Result<(), &'static str>;
    async fn get_velocity(&mut self) -> f32;
    async fn get_angle(&mut self) -> f32;
    async fn get_full_rotations(&self) -> i64;
    async fn get_mechanical_angle(&self) -> f32;
}
