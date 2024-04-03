use std::io::{self, Write};

fn print_board(cells: &[char; 9]) {
    println!("-------------");
    for (i, cell) in cells.iter().enumerate() {
        print!("| {} ", cell.to_string());
        if i % 3 == 2 {
            println!("|");
            println!("-------------");
        }
    }
}

fn make_move(cells: &mut [char; 9], position: usize, player: char) -> Result<(), String> {
    if position >= 9 {
        return Err(String::from("Invalid position. Position must be between 0 and 8."));
    }
    if cells[position] != ' ' {
        return Err(String::from("Position already taken."));
    }

    cells[position] = player;
    Ok(())
}

fn check_winner(cells: &[char; 9]) -> Option<char> {
    let win_positions = [
        (0, 1, 2),
        (3, 4, 5),
        (6, 7, 8),
        (0, 3, 6),
        (1, 4, 7),
        (2, 5, 8),
        (0, 4, 8),
        (2, 4, 6),
    ];

    for &(a, b, c) in win_positions.iter() {
        let (player, player2, player3) = (cells[a], cells[b], cells[c]);

        if player == player2 && player == player3 && player != ' ' {
            return Some(player);
        }
    }
    None
}

fn is_full(cells: &[char; 9]) -> bool {
    cells.iter().all(|&cell| cell != ' ')
}

fn main() {
    let mut cells = [' '; 9];
    let mut current_player = 0;
    let players = ['X', 'O'];

    loop {
        println!("\nCurrent Board:");
        print_board(&cells);

        println!("\n{}'s turn.", players[current_player]);
        print!("Enter your move (0-8): ");
        io::stdout().flush().unwrap();

        let mut input = String::new();
        io::stdin()
            .read_line(&mut input)
            .expect("Failed to read line");

        let position: usize = match input.trim().parse() {
            Ok(num) => num,
            Err(_) => {
                println!("Invalid input. Please enter a number between 0 and 8.");
                continue;
            }
        };

        match make_move(&mut cells, position, players[current_player]) {
            Ok(_) => {
                if let Some(winner) = check_winner(&cells) {
                    println!("\nCurrent Board:");
                    print_board(&cells);
                    println!("{} wins!", winner);
                    break;
                } else if is_full(&cells) {
                    println!("\nCurrent Board:");
                    print_board(&cells);
                    println!("It's a tie!");
                    break;
                }
                current_player = (current_player + 1) % 2;
            }
            Err(msg) => {
                println!("{}", msg);
                continue;
            }
        }
    }
}