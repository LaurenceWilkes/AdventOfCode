def populate(point, matrix, visited):
    """
    Perform a flood-fill to find the connected region starting from a given point and calculate its number of sides.

    Args:
        point (tuple): Starting point (x, y).
        matrix (list of list): 2D grid representation of the region.
        visited (set): Set of already visited points.

    Returns:
        tuple: (area, sides) where `area` is the number of cells in the region, and `sides` is the number of boundary sides.
    """
    # Directions: Up, Right, Down, Left
    directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
    to_visit = [point]
    seen = set(to_visit)
    x_max = len(matrix[0])
    y_max = len(matrix)

    region_points = set()

    while to_visit:
        current = to_visit.pop()
        region_points.add(current)
        for dx, dy in directions:
            neighbor = (current[0] + dx, current[1] + dy)
            if 0 <= neighbor[0] < x_max and 0 <= neighbor[1] < y_max:
                if matrix[neighbor[1]][neighbor[0]] == matrix[current[1]][current[0]]:
                    if neighbor not in seen:
                        to_visit.append(neighbor)
                        seen.add(neighbor)

    visited.update(seen)

    # Count the number of sides from the boundary of the region
    boundary_edges = set()
    for x, y in region_points:
        # Add edges for each unit square in the region
        edges = [
            ((x - 0.5, y), (x + 0.5, y)),   # Top edge
            ((x + 0.5, y), (x + 0.5, y + 1)), # Right edge
            ((x + 0.5, y + 1), (x - 0.5, y + 1)), # Bottom edge
            ((x - 0.5, y + 1), (x - 0.5, y))  # Left edge
        ]
        for edge in edges:
            normalized_edge = tuple(sorted(edge))
            if normalized_edge in boundary_edges:
                boundary_edges.remove(normalized_edge)  # Shared edge; remove it
            else:
                boundary_edges.add(normalized_edge)  # New edge; add it

    # The number of connected sides is the number of unique edges in the boundary
    return len(region_points), len(boundary_edges)

def fenceCost(matrix):
    visited = set()
    total_cost = 0
    for j, row in enumerate(matrix):
        for i, char in enumerate(row):
            if (i, j) not in visited:
                area, sides = populate((i, j), matrix, visited)
                total_cost += area * sides
    return total_cost

def main():
    with open("input.txt", "r") as f:
        matrix = [list(line.strip()) for line in f]

    print("The total cost is:", fenceCost(matrix))

if __name__ == "__main__":
    main()

