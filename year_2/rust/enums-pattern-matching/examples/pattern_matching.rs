struct Point {
    x: i32,
    y: i32,
}

#[allow(dead_code)]
struct Point3D {
    x: i32,
    y: i32,
    z: i32,
}

fn main() {
    let x = 2u8;

    match x {
        1 => println!("one"),
        2 => println!("two"),
        3 => println!("three"),
        _ => println!("other number"),
    }

    match x {
        1 | 2 => println!("one or two"),
        3 | 4 => println!("three or four"),
        _ => println!("anything else"),
    }

    match x {
        1..=9 => println!("small"),
        10..=99 => println!("medium"),
        100..=255 => println!("large"),
        _ => println!("anything else"),
    }

    let p = Point { x: 0, y: 7 };

    match p {
        Point { x, y: 0 } => println!("On the x axis at {x}"),
        Point { x: 0, y } => println!("On the y axis at {y}"),
        Point { x, y } => {
            println!("On neither axis: ({x}, {y})");
        }
    }

    let origin = Point3D { x: 0, y: 0, z: 0 };

    match origin {
        Point3D { x, .. } => println!("x is {}", x)
    }

    let num = Some(4);

    match num {
        Some(x) if x % 2 == 0 => println!("The number {} is even", x),
        Some(x) => println!("The number {} is odd", x),
        None => (),
    }

    let x = 4;
    let y = false;

    match x {
        4 | 5 | 6 if y => println!("yes"),
        _ => println!("no"),
    }

    let x = Some(2);

    if let Some(x) = x {
        println!("{}", x);
    }
}
