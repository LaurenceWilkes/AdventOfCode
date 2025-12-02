use std::fs::read_to_string;

fn first() {
    let ranges = read_to_string("input.txt").unwrap();
    let ranges = ranges.trim().split(",");

    let mut idadd = 0;

    for r in ranges {
        let rs: Vec<&str> = r.split("-").collect();
        let a: usize = rs[0].trim().parse().unwrap();
        let b: usize = rs[1].trim().parse().unwrap();

        let lr = rs[0].len();

        let mut hal_s = if lr % 2 == 1 {
            format!("1{}", "0".repeat(lr / 2))
        } else {
            rs[0][..lr/2].to_string()
        };

        let mut haltwo: usize = format!("{}{}", hal_s, hal_s).parse().unwrap();

        while haltwo <= b {
            if haltwo >= a {idadd += haltwo;}
            let hal: usize = hal_s.trim().parse().unwrap();
            hal_s = (hal + 1).to_string();
            haltwo = format!("{}{}", hal_s, hal_s).trim().parse().unwrap();
        }
    }
    println!("Part 1 id sum is: {idadd}");
} // first 

fn second() {
    let ranges: Vec<(usize, usize)> =
        read_to_string("input.txt")
            .unwrap()
            .split(',')
            .map(|s| s.split_once('-').unwrap())
            .map(|(a, b)| (a.trim().parse().unwrap(), b.trim().parse().unwrap()))
            .collect();
    let max_b = ranges.iter().map(|(_, b)| b).max().unwrap();
    let max_len = max_b.to_string().len();

    let mut bad_nos = Vec::new();
    for l in 1..=max_len {
        for p in 1..=l/2 {
            if l % p != 0 {continue;}
            let k = l / p;
            if k < 2 {continue;}

            let start = 10usize.pow((p - 1) as u32);
            let end   = 10usize.pow(p as u32);

            for pat in start..end {
                let mut n = 0;
                for _ in 0..k {
                    n = n * (10usize.pow(p as u32)) + pat;
                }
                bad_nos.push(n);
            }
        }
    }

    bad_nos.sort();
    let mut prev = None::<usize>;
    let mut total: usize = 0;

    for bad in bad_nos {
        if prev == Some(bad) {continue;}
        for (a, b) in &ranges {
            if *a <= bad && bad <= *b {total += bad;}
        }
        prev = Some(bad);
    }
    println!("Part 2 id sum is: {total}");
} // second

fn main() {
    first();
    second();
} // main

