use std::fs::read_to_string;

fn dist2(a: (usize, usize, usize), b: (usize, usize, usize)) -> usize {
    let dx = if a.0 > b.0 {a.0 - b.0} else {b.0 - a.0};
    let dy = if a.1 > b.1 {a.1 - b.1} else {b.1 - a.1};
    let dz = if a.2 > b.2 {a.2 - b.2} else {b.2 - a.2};
    dx * dx + dy * dy + dz * dz
}


fn get_graph(filename: &str) -> (Vec<(usize, usize, usize)>, Vec<(usize, (usize, usize))>, usize) {
    let borrow = read_to_string(filename).unwrap();
    let lines = borrow.trim().lines();

    let mut verts: Vec<(usize, usize, usize)> = Vec::new();

    for l in lines {
        let c: Vec<usize> = l.trim().split(",").map(|e| e.parse::<usize>().unwrap()).collect();
        let pos: (usize, usize, usize) = (c[0], c[1], c[2]);
        verts.push(pos);
    }

    let mut edges: Vec<(usize, (usize, usize))> = Vec::new();
    let n = verts.len();

    for i in 0..n {
        for j in 0..i {
            let edge: (usize, usize) = (i, j);
            let dist = dist2(verts[i], verts[j]);
            edges.push((dist, edge));
        }
    }

    (verts, edges, n)

} // get_grid

// Kruskal's 
fn find(parent: &mut Vec<usize>, x: usize) -> usize {
    if parent[x] != x {
        parent[x] = find(parent, parent[x]);
    }
    parent[x]
}

fn union(parent: &mut Vec<usize>, size: &mut Vec<usize>, a: usize, b: usize) {
    let mut ra = find(parent, a);
    let mut rb = find(parent, b);
    if ra == rb { return; }

    if size[ra] < size[rb] {
        std::mem::swap(&mut ra, &mut rb);
    }
    parent[rb] = ra;
    size[ra] += size[rb];
}


fn kruskal_till(edges: &[(usize, (usize, usize))], n: usize, halt_point: usize) -> usize {
    let mut parent: Vec<usize> = (0..n).collect();
    let mut size: Vec<usize> = vec![1; n];
    let mut counter = 0;
    for edge in edges.iter() {
        let (a, b) = edge.1;
        if find(&mut parent, a) != find(&mut parent, b) {
            union(&mut parent, &mut size, a, b);
        }
        counter += 1;
        if counter == halt_point {break;}
    }

    let mut comp_sizes: Vec<usize> = Vec::new();
    for i in 0..n {
        if parent[i] == i { // roots have valid sizes
            comp_sizes.push(size[i]);
        }
    }

    comp_sizes.sort_by(|a, b| b.cmp(&a));

    comp_sizes[0] * comp_sizes[1] * comp_sizes[2]
    
} // kruskal_till

fn kruskal_last(edges: &[(usize, (usize, usize))], n: usize) -> (usize, usize) {
    let mut parent: Vec<usize> = (0..n).collect();
    let mut size: Vec<usize> = vec![1; n];
    let mut last_a = 0;
    let mut last_b = 0;
    for edge in edges.iter() {
        let (a, b) = edge.1;
        if find(&mut parent, a) != find(&mut parent, b) {
            union(&mut parent, &mut size, a, b);
            last_a = a;
            last_b = b;
        }
    }

    (last_a, last_b)
    
} // kruskal_last


fn main() {
    let (verts, mut edges, n) = get_graph("input.txt");
    edges.sort_by_cached_key(|e| e.0);
    let output = kruskal_till(&edges, n, 1000);
    println!("Part 1 prod is {output}");
    let (last_a, last_b) = kruskal_last(&edges, n);
    let result = verts[last_a].0 * verts[last_b].0;
    println!("Part 2 prod is {result}");
} // main
