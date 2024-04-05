use rand::Rng;

fn main() {
    let mut rng = rand::thread_rng();
    let rand_number = rng.gen_range(0..10);

    println!("Random number: {}", rand_number);
}

fn generate_password(n: i32) -> &str {}

fn get_domain() -> &str {}

fn get_many() -> (&str, &str, &str) {}