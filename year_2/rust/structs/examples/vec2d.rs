use std::ops::{Add, Sub};

#[derive(Debug, PartialEq, Copy, Clone)]
struct Vec2D {
    x: f32,
    y: f32,
}

impl Add for Vec2D {
    type Output = Self; // Output describes the resulting type after applying the + operator.
                        // Self describes the current type, here it means Vec2D

    fn add(self, other: Vec2D) -> Self {
        Vec2D {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl Sub for Vec2D {
    type Output = Self;

    fn sub(self, other: Vec2D) -> Self {
        Vec2D {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}

#[derive(Debug)]
struct Spaceship {
    position: Vec2D,
    direction: i32
}

impl Spaceship {
    fn print_position(&self) {
        println!("My position is: {:?}", self.position)
    }

    fn change_position(&mut self, vec: Vec2D) { 
        self.position = self.position + vec
    }

    fn rotate(&mut self, angle: i32) { 
        self.direction += angle
    }
    
    fn distance_from_point(&mut self, point: Vec2D) { 
        todo!() 
    }
    
    fn distance_from_ship(&mut self, other: Spaceship) { 
        todo!() 
    }
}

fn main() {
    let v1 = Vec2D { x: 0.0, y: 0.0 };
    let v2 = Vec2D { x: 1.0, y: 1.0 };
    assert_eq!(Vec2D { x: 1.0, y: 1.0 }, v1 + v2);

    let v1 = Vec2D { x: 0.0, y: 0.0 };
    let v2 = Vec2D { x: 1.0, y: 1.0 };
    assert_eq!(Vec2D { x: -1.0, y: -1.0 }, v1 - v2);
}
