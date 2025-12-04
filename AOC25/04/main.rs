use std::fs::File;
use std::io::{BufRead, BufReader};
use std::collections::HashMap;

const DIRS: [(i32, i32); 8] = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)];

fn get_grid(path: &str) -> HashMap<(i32, i32), char> {
    let file = File::open(path).unwrap();
    let reader = BufReader::new(file);

    let mut i = 0;
    let mut grid = HashMap::new();

    for line_result in reader.lines() {
        let line: String = line_result.unwrap();   
        let mut j = 0;
        for c in line.chars() {
            let coord = (i, j);
            grid.insert(coord, c);
            j += 1;
        }
        i += 1;
    }
    grid
}

fn count_access(grid: &HashMap<(i32, i32), char>) -> i32 {
    let mut total_count = 0;
    for ((k1, k2), &v) in grid.iter() {
        if v != '@' {continue;}
        let mut adj_count = 0;
        for &(d1, d2) in &DIRS {
            let nk = (k1 + d1, k2 + d2);
            if grid.get(&nk).unwrap_or(&'.') == &'@' {
                adj_count += 1;
            }
        }
        if adj_count < 4 {total_count += 1;}
    }
    total_count
} // count_access

fn whittle_rolls(grid: &mut HashMap<(i32, i32), char>) -> i32 {
    let mut removed = 0;
    loop {
        let mut to_flip = Vec::new();
        for ((k1, k2), &v) in grid.iter() {
            if v != '@' {continue;}
            let mut adj_count = 0;
            for &(d1, d2) in &DIRS {
                let nk = (k1 + d1, k2 + d2);
                if grid.get(&nk).unwrap_or(&'.') == &'@' {
                    adj_count += 1;
                }
            }
            if adj_count < 4 {to_flip.push((*k1, *k2));}
        }
        if to_flip.is_empty() {break;}
        for &(x, y) in &to_flip {
            if let Some(v) = grid.get_mut(&(x, y)) {
                *v = '.';
                removed += 1;
            }
        }
    }
    removed
} // whittle_rolls


fn main() {
    let mut grid = get_grid("input.txt");
    let output = count_access(&grid);
    println!("Part 1 output is {output}");
    let removed = whittle_rolls(&mut grid);
    println!("Part 2 removed rolls is {removed}");
} // main
