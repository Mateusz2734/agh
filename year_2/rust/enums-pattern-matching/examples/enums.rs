#[allow(dead_code)]
#[derive(Debug)]
enum Direction {
    Up,
    Down
}

#[allow(dead_code)]
#[derive(Debug)]
enum UIEvent {
    ButtonClicked,
    Scroll(Direction),
    KeyPressed(char)
}

impl UIEvent {
    fn describe(&self) {
        println!("{:?}", self);
    }
}

#[allow(dead_code)]
#[derive(Debug)]
enum Message {
    Move{ x: i32, y: i32 },
    Echo(String),
    ChangeColor(i32, i32, i32),
    Quit
}

impl Message {
    fn call(&self) {
        println!("{:?}", self);
    }
}

fn main() {
    let key_pressed = UIEvent::KeyPressed('L');
    
    key_pressed.describe();

    let messages = [
        Message::Move { x: 10, y: 30 },
        Message::Echo(String::from("hello world")),
        Message::ChangeColor(200, 255, 255),
        Message::Quit,
    ];

    for message in &messages {
        message.call();
    }
}