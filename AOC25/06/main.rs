use std::fs::File;
use std::io::{BufRead, BufReader};

fn load_file(filename: &str) -> (Vec<Vec<usize>>, Vec<char>) {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);

    let mut number_table = Vec::new();
    let mut ops = Vec::new();

    for line_result in reader.lines() {
        let line = line_result.unwrap();
        let columns: Vec<&str> = line.trim().split_whitespace().collect();

        if let Ok(_) = columns[0].parse::<usize>() {
            let nums: Vec<usize> = columns
                .iter()
                .map(|e| e.parse::<usize>().unwrap())
                .collect();
            number_table.push(nums);
        } else {
            ops = columns.iter()
                    .map(|s| s.chars().next().unwrap())
                    .collect();
        }
    }

    (number_table, ops)
} // load_file

fn apply_ops(number_table: &Vec<Vec<usize>>, ops: &Vec<char>) -> usize {
    let n = ops.len();
    let mut output = vec![0; n];

    for i in 0..n {
        if ops[i] == '*' {output[i] = 1;}
    }

    for nums in number_table {
        for i in 0..n {
            match ops[i] {
                '*' => {output[i] *= nums[i];},
                '+' => {output[i] += nums[i];},
                 _  => panic!(),
            }
        }
    }
    let result = output.iter().sum();
    result
} // apply_ops

fn process_col(col: Vec<usize>, symb: char) -> usize {
    let lens: Vec<usize> = col.iter().map(|e| e.to_string().len()).collect();
    let &maxlen = lens.iter().max().unwrap();

    let mut output = 0;
    if symb == '*' {output = 1;}

    for ind in (1..=maxlen).rev() {
        let mut place_power = 1;
        let mut current = 0;
        for i in (0..col.len()).rev() {
            if lens[i] < ind {continue;}
            let mut digit = col[i] / 10usize.pow((maxlen - ind) as u32);
            digit %= 10;
            current += digit * place_power;
            place_power *= 10;
        }
        match symb {
            '*' => {output *= current;},
            '+' => {output += current;},
             _  => panic!(),
        }
    }
    output
} // process_col

fn apply_ops_cep(number_table: &Vec<Vec<usize>>, ops: &Vec<char>) -> usize {
    let m = number_table.len();
    let n = ops.len();
    let mut total: usize = 0;

    for j in 0..n {
        let mut col = Vec::new();
        for i in 0..m {
            col.push(number_table[i][j])
        }
        let num = process_col(col, ops[j]);
        total += num;
    }
    total
} // apply_ops


fn main() {
    let (number_table, ops) = load_file("input.txt");
    let result_sum = apply_ops(&number_table, &ops);
    println!("Part 1 overall sum is {result_sum}");
    let result_sum = apply_ops_cep(&number_table, &ops);
    println!("Part 2 overall sum is {result_sum}");
} // main
