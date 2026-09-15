import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.List;
import java.util.PriorityQueue;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);

    Cave cave = new Cave(lines);

    System.out.println("Part One: " + cave.lowRiskPath());
    System.out.println("Part Two: " + cave.lowRiskPathLarge());
  } // main

  record DistNode(int dist, int i, int j) {}

  static class Cave {
    final int[][] vals;
    final int[] diri = {1, -1, 0, 0};
    final int[] dirj = {0, 0, 1, -1};
    final int rows;
    final int cols;

    Cave(List<String> lines) {
      rows = lines.size();
      cols = lines.get(0).length();
      vals = new int[rows][cols];
      for (int j = 0; j < rows; j++) {
        String line = lines.get(j);
        for (int i = 0; i < cols; i++) {
          vals[j][i] = line.charAt(i) - '0';
        }
      }
    } // Cave

    int lowRiskPath() {
      int[] distances = new int[rows * cols];
      Arrays.fill(distances, Integer.MAX_VALUE);
      distances[0] = 0;
      PriorityQueue<DistNode> pq = new PriorityQueue<>((a, b) -> a.dist() - b.dist());
      pq.add(new DistNode(0, 0, 0));
      while (!pq.isEmpty()) {
        DistNode current = pq.poll();
        int i = current.i();
        int j = current.j();
        int dist = current.dist();
        int index = j * cols + i;
        if (dist != distances[index]) continue;
        if (i == cols - 1 && j == rows - 1) {
          return dist;
        }
        for (int k = 0; k < 4; k++) {
          int in = i + diri[k];
          int jn = j + dirj[k];
          if (in < 0 || in >= cols || jn < 0 || jn >= rows) continue;
          int next = jn * cols + in;
          int newDist = dist + vals[jn][in];
          if (newDist < distances[next]) {
            distances[next] = newDist;
            pq.add(new DistNode(newDist, in, jn));
          }
        }
      }
      return -1;
    } // lowRiskPath

    int lowRiskPathLarge() {
      int[] distances = new int[25 * rows * cols];
      Arrays.fill(distances, Integer.MAX_VALUE);
      distances[0] = 0;
      PriorityQueue<DistNode> pq = new PriorityQueue<>((a, b) -> a.dist() - b.dist());
      pq.add(new DistNode(0, 0, 0));
      while (!pq.isEmpty()) {
        DistNode current = pq.poll();
        int i = current.i();
        int j = current.j();
        int dist = current.dist();
        int index = j * 5 * cols + i;
        if (dist != distances[index]) continue;
        if (i == 5 * cols - 1 && j == 5 * rows - 1) {
          return dist;
        }
        for (int k = 0; k < 4; k++) {
          int in = i + diri[k];
          int jn = j + dirj[k];
          if (in < 0 || in >= 5 * cols || jn < 0 || jn >= 5 * rows) continue;
          int next = jn * 5 * cols + in;
          int newDist = dist + (vals[jn % rows][in % cols] + jn / rows + in / cols - 1) % 9 + 1;
          if (newDist < distances[next]) {
            distances[next] = newDist;
            pq.add(new DistNode(newDist, in, jn));
          }
        }
      }
      return -1;
    } // lowRiskPathLarge

  } // Cave class

} // Main class
