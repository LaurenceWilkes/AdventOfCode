use std::fs::File;
use std::io::{BufRead, BufReader};

fn process_file(path: &str, count_flag: bool) -> usize {
    let file = File::open(path).unwrap();
    let reader = BufReader::new(file);

    let mut out = 0;

    for line_result in reader.lines() {
        let line = line_result.unwrap();   // String
        if count_flag {
            out += two_count(&line);
        } else {
            out += twelve_count(&line);
        }
    }
    out
}

fn two_count(line: &str) -> usize {
    let len = line.len();

    let mut first: u32 = 0;
    let mut second: u32 = 0;

    for (i, c) in line.chars().enumerate() {
        let cur: u32 = c.to_digit(10).unwrap();
        if i == len-1 {
            if cur > second {second = cur;}
            break;
        }
        if cur > first {
            first = cur;
            second = 0;
        } else if cur > second {second = cur;}
    }

    (10 * first + second) as usize
}

fn twelve_count(line: &str) -> usize {
    let line_len = line.len();
    let mut vals: [u32; 12] = [0; 12];

    for (i, c) in line.chars().enumerate() {
        let cur: u32 = c.to_digit(10).unwrap();
        let cut_off = i.saturating_sub(line_len - 12);
        for j in cut_off..12 {
            if cur > vals[j] {
                vals[j] = cur;
                for k in j+1..12 {
                    vals[k] = 0;
                }
                break;
            }
        }
    }

    let mut out: usize = 0;
    for (i, v) in vals.iter().enumerate() {
        let mult = 10usize.pow(11u32 - i as u32);
        out += mult * (*v as usize);
    }

    out
} // process_line


fn main() {
    let out = process_file("input.txt", true);
    println!("Part 1 output is {out}");

    let out = process_file("input.txt", false);
    println!("Part 2 output is {out}");
} // main
