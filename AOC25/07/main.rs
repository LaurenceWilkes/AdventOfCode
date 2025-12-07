use std::fs::read_to_string;
use std::collections::{HashMap, HashSet};

fn get_grid(filename: &str) -> (HashMap<(usize, usize), char>, usize, usize, usize) {
    let borrow = read_to_string(filename).unwrap();
    let lines = borrow.trim().lines();

    let mut i: usize = 0;
    let mut j: usize = 0;
    let mut s: usize = 0;

    let mut grid = HashMap::new();

    for l in lines {
        j = 0;
        for c in l.chars() {
            let coord = (i, j);
            grid.insert(coord, c);
            if c == 'S' {s = j;}
            j += 1;
        }
        i += 1;
    }
    (grid, s, i, j) // grid, starting_col, number_of_rows, number_of_columns
} // get_grid

fn count_beams(grid: &HashMap<(usize, usize), char>, s: usize, no_rows: usize) -> (usize, usize) {
    let mut beams = HashSet::new();
    beams.insert(s);

    let mut split_count: usize = 0;

    for i in 1..no_rows {
        let mut new_beams: HashSet<usize> = HashSet::new();
        for b in &beams {
            let coord = (i, *b);
            if grid.get(&coord) == Some(&'^') {
                new_beams.insert(b-1);
                new_beams.insert(b+1);
                split_count += 1;
            } else {
                new_beams.insert(*b);
            }
        }
        beams = new_beams;
    }

    (beams.len() as usize, split_count)

} // count_beams

fn count_futures(grid: &HashMap<(usize, usize), char>, s: usize, no_rows: usize, no_cols: usize) -> usize {
    let mut beams: Vec<usize> = vec![0_usize; no_cols];
    beams[s] = 1;

    for i in 1..no_rows {
        for j in 0..no_cols {
            if beams[j] == 0 {continue;}
            let coord = (i, j);
            if grid.get(&coord) == Some(&'^') {
                beams[j - 1] += beams[j];
                beams[j + 1] += beams[j];
                beams[j] = 0;
            } 
        }
    }

    beams.iter().sum::<usize>()

} // count_beams

fn main() {
    let (grid, s, no_rows, no_cols) = get_grid("input.txt");
    let (_amount, split_count) = count_beams(&grid, s, no_rows);
    println!("Part 1 split count is {split_count}");
    let futures = count_futures(&grid, s, no_rows, no_cols);
    println!("Part 2 future count is {futures}");
} // main
