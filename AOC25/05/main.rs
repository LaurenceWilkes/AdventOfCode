use std::fs::File;
use std::io::{BufRead, BufReader};
use std::cmp::{min, max};

fn process_file(path: &str, ranges: &mut Vec<(usize, usize)>, ids: &mut Vec<usize>) {
    let file = File::open(path).unwrap();
    let reader = BufReader::new(file);

    let mut blank_switch = true;

    for line_result in reader.lines() {
        let line = line_result.unwrap();   // String
        if line.is_empty() {
            blank_switch = false;
            continue;
        }
        if blank_switch {
            let (a, b) = line.split_once("-").unwrap();
            let l: usize = a.trim().parse().unwrap();
            let r: usize = b.trim().parse().unwrap();
            ranges.push((l, r));
        } else {
            let id: usize = line.trim().parse().unwrap();
            ids.push(id);
        }
    }
}

fn count_fresh(ranges: &Vec<(usize, usize)>, ids: &Vec<usize>) -> u32 {
    let mut count = 0;
    for id in ids {
        for (a, b) in ranges {
            if a <= id && id <= b {
                count += 1;
                break;
            }
        }
    }
    count
} // count_fresh

fn range_union((a, b): (usize, usize), (c, d): (usize, usize)) -> Option<(usize, usize)> {
    if d < a || b < c {return None;}
    Some((min(a, c), max(b, d)))
} // range_union

fn range_lens(ranges: &mut Vec<(usize, usize)>) -> usize {
    let mut total: usize = 0;
    let mut n = ranges.len();
    while n > 0 {
        let cur = ranges.pop().unwrap();
        let mut finished = true;
        for i in 0..n-1 {
            let r = range_union(cur, ranges[i]);
            if let Some(union) = r {
                ranges[i] = union;
                finished = false;
                break;
            } else {
                continue;
            }
        }
        if finished {
            total += cur.1 - cur.0 + 1;
        }
        n -= 1;
    }
    total
} // range_lens

fn main() {
    let mut ranges: Vec<(usize, usize)> = Vec::new();
    let mut ids: Vec<usize> = Vec::new();
    process_file("input.txt", &mut ranges, &mut ids);
    let count = count_fresh(&ranges, &ids);
    println!("Part 1 fresh count is {count}");

    let total_len = range_lens(&mut ranges);
    println!("Part 2 total fresh count is {total_len}");
} // main
