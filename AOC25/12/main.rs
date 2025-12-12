use std::fs::File;
use std::io::{BufRead, BufReader};

fn read_file(filename: &str) -> (Vec<Vec<Vec<char>>>, Vec<(usize, usize)>, Vec<Vec<usize>>) {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);

    let mut patterns: Vec<Vec<Vec<char>>> = Vec::new();
    let mut bounds: Vec<(usize, usize)> = Vec::new();
    let mut tile_specs: Vec<Vec<usize>> = Vec::new();

    let mut pattern: Vec<Vec<char>> = Vec::new();
    for line in reader.lines() {
        let line = line.unwrap();
        let bytes = line.as_bytes();
        if line.is_empty() {
            patterns.push(pattern);
            pattern = Vec::new();
        } else if bytes[1] == b'.' || bytes[1] == b'#' {
            pattern.push(line.chars().collect());
        } else if bytes.len() >= 6 && bytes[5] == b':' {
            let max_x: usize = line[0..2].parse().unwrap();
            let max_y: usize = line[3..5].parse().unwrap();
            let tile_nos: Vec<usize> = line[7..].split_whitespace().map(|e| e.parse::<usize>().unwrap()).collect();
            tile_specs.push(tile_nos);
            bounds.push((max_x, max_y));
        }
    }

    (patterns, bounds, tile_specs)
} // read_file

fn check_fit_lazy(pattern_vals: &[usize], bound: (usize, usize), tile_nos: &[usize]) -> Option<bool> {
    let x = bound.0;
    let y = bound.1;
    let pattern_area: usize = pattern_vals.iter().zip(tile_nos).map(|(a, b)| a * b).sum();
    if (x / 3) * (y / 3) >= tile_nos.iter().sum() {
        Some(true)
    } else if pattern_area > x * y {
        Some(false)
    } else {
        None
    }
} // lazy_check

fn main() {
    let (patterns, bounds, tile_specs) = read_file("input.txt");

    let mut pattern_vals: Vec<usize> = Vec::new();
    for p in patterns {
        let mut val = 0;
        for i in 0..3 {
            for j in 0..3 {
                if p[i][j] == '#' {val += 1;}
            }
        }
        pattern_vals.push(val);
    }

    let mut count = 0;
    for i in 0..bounds.len() {
        let Some(out) = check_fit_lazy(&pattern_vals, bounds[i], &tile_specs[i]) else {panic!("This wasn't sufficient...")};
        if out {
            count += 1;
        }
    }

    println!("Part 1 fitting tile sets is {}", count);
} // main
