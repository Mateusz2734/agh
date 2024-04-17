use std::fmt::{Display, Formatter};

#[derive(Debug)]
struct Rectangle {
    x: f32,
    y: f32,
}

impl Rectangle {
    fn area(&self) -> f32 {
        self.x * self.y
    }

    fn scale(&mut self, factor: f32) {
        self.x = self.x * factor;
        self.y = self.y * factor;
    }

    fn new_square(x: f32) -> Rectangle {
        Rectangle { x, y: x }
    }
}

trait Shape {
    fn area(&self) -> f32;
    fn perimeter(&self) -> f32;
    fn describe(&self) {
        println!("I'm a general shape.");
    }
}

impl Shape for Rectangle {
    fn area(&self) -> f32 {
        self.x * self.y
    }

    fn perimeter(&self) -> f32 {
        2f32 * (self.x + self.y)
    }

    fn describe(&self) {
        println!("I'm a rectangle.");
    }
}

impl Display for Rectangle {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "Rectangle[x={}, y={}]", self.x, self.y)?;
        Ok(())
    }
}

fn main() {
    let rect = Rectangle { x: 3.0, y: 4.0 };
    println!("{:?}", rect);
    println!("Area: {}", rect.x * rect.y);
    println!("Perimeter: {}", 2.0 * (rect.x + rect.y));

    let r2 = Rectangle { x: 5.0, ..rect };
    println! {"{:?}", rect};
    println!("x: {}, y: {}", r2.x, r2.y);

    let r3 = Rectangle { x: 5.0, y: 9.0 };
    println!("[{}, {}]", r3.x, r3.y);

    let mut r4 = Rectangle { x: 5.0, y: 9.0 };
    println!("[{}, {}]", r4.x, r4.y);
    r4.x = 6.0;
    r4.y = 7.0;
    println!("[{}, {}]", r4.x, r4.y);

    println!("Area of {:?} is {}", r4, r4.area());

    let mut r = Rectangle { x: 5.0, y: 4.0 };
    r.scale(2.0);
    println!("Area of r is {}", r.area());

    let square = Rectangle::new_square(5.0);
    println!("square: {:?}", square);

    square.describe();

    println!("{}", r);
}
