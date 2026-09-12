import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Deque;
import java.util.ArrayDeque;
import java.util.List;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);

    Octopi oct = new Octopi(lines);
    int flashCount = 0;
    int maxFlash = oct.rows * oct.cols;
    int firstSync = 0;
    for (int step = 1; firstSync == 0 || step <= 100; step++) {
      int count = oct.step();
      if (count == maxFlash) {firstSync = step;}
      if (step <= 100) {flashCount += count;}
    }
    System.out.println("Part One: " + flashCount);
    System.out.println("Part Two: " + firstSync);
  }

  static class Octopi {
    private final int[][] values;
    public final int rows, cols;
    private final Deque<Integer> toFlash;

    public Octopi(List<String> lines) {
      rows = lines.size();
      cols = lines.get(0).length();
      values = new int[rows][cols];
      toFlash = new ArrayDeque<>();
      for (int i = 0; i < rows; i++) {
        String line = lines.get(i);
        for (int j = 0; j < cols; j++) {
          values[i][j] = line.charAt(j) - '0';
        }
      }
    }

    private int toIndex(int x, int y) {
      return x * cols + y;
    }

    private void increment() {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          values[i][j]++;
          if (values[i][j] > 9) {
            toFlash.add(toIndex(i, j));
          }
        }
      }
    }

    private int flash() {
      int flashCount = 0;
      boolean[][] flashed = new boolean[rows][cols];
      while (!toFlash.isEmpty()) {
        int index = toFlash.pop();
        int x = index / cols;
        int y = index % cols;
        if (flashed[x][y]) continue;
        flashed[x][y] = true;
        flashCount++;
        values[x][y] = 0;
        for (int dx = -1; dx <= 1; dx++) {
          for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) {continue;}
            if (!flashed[nx][ny]) {
              values[nx][ny]++;
              if (values[nx][ny] > 9) {
                toFlash.add(toIndex(nx, ny));
              }
            }
          }
        }
      }
      return flashCount;
    }

    public int step() {
      increment();
      return flash();
    }

  }

}
