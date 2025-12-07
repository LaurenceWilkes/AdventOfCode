use std::fs::read_to_string;

fn load_problems(filename: &str) -> Vec<(Vec<usize>, char)> {
    let contents = read_to_string(filename).unwrap();
    let lines: Vec<&str> = contents.lines().collect();

    let height = lines.len();
    let width = lines.iter().map(|l| l.len()).max().unwrap();

    let mut grid: Vec<Vec<char>> = Vec::new();
    for line in lines {
        let mut chars: Vec<char> = line.chars().collect();
        grid.push(chars);
    }

    let is_separator_col = |c: usize| -> bool {(0..height).all(|r| grid[r][c] == ' ')};

    let mut problems: Vec<(Vec<usize>, char)> = Vec::new();

    let mut col: isize = (width as isize) - 1;

    while col >= 0 {
        while col >= 0 && is_separator_col(col as usize) {
            col -= 1;
        }
        if col < 0 {break;}

        let mut block_cols: Vec<usize> = Vec::new();

        while col >= 0 && !is_separator_col(col as usize) {
            block_cols.push(col as usize);
            col -= 1;
        }

        block_cols.reverse();

        let mut op = None;
        for &c in &block_cols {
            let ch = grid[height - 1][c]; // operators
            if ch == '+' || ch == '*' {
                op = Some(ch);
                break;
            }
        }

        let mut operands = Vec::new();

        for &c in &block_cols {
            let mut digits = String::new();
            for r in 0..(height - 1) {
                let ch = grid[r][c];
                if ch.is_ascii_digit() {
                    digits.push(ch);
                }
            }
            if !digits.is_empty() {
                operands.push(digits.parse::<usize>().unwrap());
            }
        }

        problems.push((operands, op.expect("No operator")));

        if col < 0 {
            break;
        }
    }

    problems
}


fn eval_cephalopod(problems: &[(Vec<usize>, char)]) -> usize {
    problems.iter().map(|(nums, op)| {
        match op {
            '+' => nums.iter().sum::<usize>(),
            '*' => nums.iter().product::<usize>(),
             _  => unreachable!(),
        }
    }).sum()
}


fn main() {
    let problems = load_problems("input.txt");

    for (nums, op) in &problems {
        println!("{:?} {:?}", nums, op);
    }

    let total = eval_cephalopod(&problems);
    println!("Part 2 answer = {}", total);
}
