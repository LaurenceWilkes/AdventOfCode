use std::fs::File;
use std::io::{BufRead, BufReader};
use std::collections::{HashSet, HashMap};

fn read_file(filename: &str) -> HashMap<String, HashSet<String>> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);

    let mut back_map: HashMap<String, HashSet<String>> = HashMap::new();

    for line in reader.lines() {
        let line = line.unwrap();

        let (src, outs) = line.split_once(": ").unwrap();
        let src = src.to_string();

        for o in outs.split_whitespace() {
            back_map.entry(o.to_string())
                    .or_insert_with(HashSet::new)
                    .insert(src.clone());
        }
    }

    back_map
}

fn count_paths(start: &str, end: &str, back_map: &HashMap<String, HashSet<String>>) -> usize {
    let mut rf: HashMap<(String, String), usize> = HashMap::new();
    fn reached_from(start: &str, end: &str, back_map: &HashMap<String, HashSet<String>>, rf: &mut HashMap<(String, String), usize>) -> usize {
        if start == end {return 1;}

        let key = (start.to_string(), end.to_string());
        if let Some(&cached) = rf.get(&key) {return cached;}

        let mut sum = 0;
        if let Some(prevs) = back_map.get(end) {
            for el in prevs {
                sum += reached_from(start, el, back_map, rf);
            }
        }

        rf.insert(key, sum);
        sum
    }

    reached_from(start, end, back_map, &mut rf)
}

fn main() {
    let back_map = read_file("input.txt");
    let total_paths = count_paths("you", "out", &back_map);
    println!("Total paths: {}", total_paths);

    let prod_paths = count_paths("svr", "fft", &back_map)
                   * count_paths("fft", "dac", &back_map) // for my input there are no paths from dac to fft
                   * count_paths("dac", "out", &back_map);

    println!("Part 2 paths through fft and dac {}", prod_paths);
    println!();
    println!("Sanity check total is {}", count_paths("svr", "out", &back_map));
}
