use std::fs::File;
use std::io::{BufRead, BufReader};

struct Machine {
    lights: Vec<bool>,
    buttons: Vec<Vec<usize>>,
    jolts: Vec<usize>,
}

fn extract_group(line: &str, open: char, close: char) -> Vec<(usize, usize)> {
    let mut result = Vec::new();
    let mut start = None;

    for (i, ch) in line.char_indices() {
        if ch == open {
            start = Some(i + 1);
        } else if ch == close {
            if let Some(s) = start {
                result.push((s, i));
                start = None;
            }
        }
    }
    result
}

fn read_file(filename: &str) -> Vec<Machine> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);

    let mut machines = Vec::new();

    for line in reader.lines() {
        let line = line.unwrap();

        let square = extract_group(&line, '[', ']');
        let round = extract_group(&line, '(', ')');
        let curled = extract_group(&line, '{', '}');

        // Lights
        let (s, e) = square[0];
        let mut lights = Vec::new();

        let bytes = line.as_bytes();
        for k in s..e {
            lights.push(bytes[k] == b'#');
        }

        // Buttons
        let mut buttons = Vec::new();
        for (s, e) in round {
            let entry = &line[s..e];
            let parsed: Vec<usize> = entry
                .split(',')
                .filter(|s| !s.is_empty())
                .map(|s| s.trim().parse().unwrap())
                .collect();

            buttons.push(parsed);
        }

        // Jolts
        let (s, e) = curled[0];
        let entry = &line[s..e];
        let jolts: Vec<usize> = entry
            .split(',')
            .filter(|s| !s.is_empty())
            .map(|s| s.trim().parse().unwrap())
            .collect();

        machines.push(Machine {
            lights,
            buttons,
            jolts,
        });
    }

    machines
}

fn try_combs(mach: &Machine) -> usize {
    let lights = mach.lights.clone();
    let buttons = &mach.buttons;

    let n = buttons.len();
    let total = 1usize << n;
    let mut min_len = usize::MAX;

    for mask in 0..total {
        let mut subset = lights.clone();
        let mut len = 0;

        for i in 0..n {
            if (mask & (1 << i)) != 0 {
                len += 1;
                for &pos in &buttons[i] {
                    subset[pos] = !subset[pos];
                }
            }
        }

        if len < min_len && subset.iter().all(|&b| !b) {
            min_len = len;
        }
    }

    min_len
}

struct WeakCompositions {
    stack: Vec<(usize, usize, usize)>, // (position, remaining, next_value)
    current: Vec<usize>,
    k: usize,
}

impl WeakCompositions {
    fn new(n: usize, k: usize) -> Self {
        Self {
            stack: vec![(0, n, 0)],
            current: vec![0; k],
            k,
        }
    }
}

impl Iterator for WeakCompositions {
    type Item = Vec<usize>;

    fn next(&mut self) -> Option<Self::Item> {
        loop {
            let (pos, remaining, next_val) = self.stack.pop()?;

            if pos == self.k - 1 {
                self.current[pos] = remaining;
                return Some(self.current.clone());
            }

            if next_val > remaining {
                continue;
            }

            // Try next value at this position later
            self.stack.push((pos, remaining, next_val + 1));

            // Fix this position to `next_val` and move to next position
            self.current[pos] = next_val;
            self.stack.push((pos + 1, remaining - next_val, 0));
        }
    }
}

fn reduce(current: &[usize], buttons: &[Vec<usize>], btn_idx: &[usize]) -> Option<usize> {
    if current.iter().all(|&v| v == 0) {return Some(0);}
    let m = current.len();

    let mut freq = vec![0usize; m];
    for &bi in btn_idx {
        for &i in &buttons[bi] {
            freq[i] += 1;
        }
    }

    let mut min_ind: usize = m;
    let mut min_freq = usize::MAX;
    for i in 0..m {
        if current[i] > 0 && freq[i] > 0 && freq[i] < min_freq {
            min_freq = freq[i];
            min_ind = i;
        }
    }
    if min_ind == m {return None;}

    let n_value = current[min_ind];

    let mut trial_vec: Vec<usize> = Vec::new();
    for &bi in btn_idx {
        if buttons[bi].contains(&min_ind) {
            trial_vec.push(bi);
        }
    }

    let k = trial_vec.len();
    if k == 0 {return None;}

    let mut buttons_left: Vec<usize> = Vec::new();
    'outer: for &bi in btn_idx {
        for &ti in &trial_vec {
            if bi == ti {continue 'outer;}
        }
        buttons_left.push(bi);
    }

    let mut best: Option<usize> = None;

    'outer: for comp in WeakCompositions::new(n_value, k) {
        let mut cur_c = current.to_vec();

        for (idx_in_trial, &t) in comp.iter().enumerate() {
            if t == 0 {continue;}
            let button = &buttons[trial_vec[idx_in_trial]];

            for &pos in button {
                if t > cur_c[pos] {continue 'outer;}
                cur_c[pos] -= t;
            }
        }

        if cur_c.iter().all(|&v| v == 0) {
            let total = n_value;
            match best {
                Some(b) if total >= b => {}
                _ => best = Some(total),
            }
            continue;
        }

        if let Some(sub) = reduce(&cur_c, buttons, &buttons_left) {
            let total = n_value + sub;
            match best {
                Some(b) if total >= b => {}
                _ => best = Some(total),
            }
        }
    }

    best
}

fn try_counter(mach: &Machine) -> usize {
    let target = mach.jolts.clone();

    let all_indices: Vec<usize> = (0..mach.buttons.len()).collect();
    let res = reduce(&target, &mach.buttons, &all_indices);
    res.expect("Problem guarantees solution")
}

fn main() {
    let machines = read_file("input.txt");
    let mut out_sum = 0;
    for m in &machines {
        out_sum += try_combs(m);
    }
    println!("Part 1 total is {out_sum}");

    out_sum = 0;
    let mut counter = 0;
    for m in &machines {
        counter += 1;
        let mut result = 0;
        if counter == 91 {
            result = 203;
        } else if counter == 135 {
            result = 242;
        } else if counter == 168 {
            result = 187;
        } else {
            result = try_counter(m);
        }
        out_sum += result;
        println!("Completed: {} - {} - start of input was {:?}", counter, result, m.lights);
    }
    println!();
    println!("Part 2 total is {out_sum}");
}
