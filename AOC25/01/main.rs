use std::fs::read_to_string;

fn first() {
    let mut dial_val: usize = 50;
    let mut zero_count: usize = 0;
    for line in read_to_string("input.txt").unwrap().lines() {
        let line = line.trim();
        let ch = line.chars().nth(0).unwrap();
        let val: usize = line[1..].parse().expect("Couldn\'t parse val");
        dial_val = match ch {
            'L' => (dial_val + 100 - val % 100) % 100,
            'R' => (dial_val + val) % 100,
             _  => panic!("invalid dir '{}'", ch),
        };
        if dial_val == 0 {
            zero_count += 1;
        }
    }
    println!("Part 1 zero count is: {zero_count}");
} // main

fn second() {
    let mut dial_val: usize = 50;
    let mut zero_count: usize = 0;
    for line in read_to_string("input.txt").unwrap().lines() {
        let line = line.trim();
        let ch = line.chars().nth(0).unwrap();
        let val: usize = line[1..].parse().expect("Couldn\'t parse val");
        match ch {
            'L' => {
                zero_count += ((100 - dial_val) % 100 + val) / 100;
                dial_val = (dial_val + 100 - val % 100) % 100;
            },
            'R' => {
                zero_count += (dial_val + val) / 100;
                dial_val = (dial_val + val) % 100;
            },
            _  => panic!("invalid dir '{}'", ch),
        }
    }
    println!("Part 2 zero count is: {zero_count}");
} // main

fn main() {
    first();
    second();
} // main
