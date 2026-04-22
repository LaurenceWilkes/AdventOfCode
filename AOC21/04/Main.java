import java.nio.file.Files;
import java.nio.file.Path;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.HashMap;
import java.util.Map;
import java.util.HashSet;
import java.util.Arrays;
import java.util.stream.Collectors;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  private static class Coord {
    final int x;
    final int y;
    public Coord(int x, int y) {
      this.x = x;
      this.y = y;
    }
  }

  private static class Board {
    final int size;
    final boolean[][] hits;
    final Map<Integer, Coord> vals;

    public Board(int size) {
      this.size = size;
      hits = new boolean[size][size];
      vals = new HashMap<>();
    }

    public boolean isComplete() {
      boolean[] cols = new boolean[size];
      boolean[] rows = new boolean[size];
      Arrays.fill(cols, true);
      Arrays.fill(rows, true);
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          cols[i] &= hits[i][j];
          rows[j] &= hits[i][j];
        }
      }
      for (boolean el : cols) {
        if (el) return true;
      }
      for (boolean el : rows) {
        if (el) return true;
      }
      return false;
    }

    public int sumUnmarked() {
      int sum = 0;
      for (Map.Entry<Integer, Coord> entry : vals.entrySet()) {
        Coord c = entry.getValue();
        if (!hits[c.x][c.y]) sum += entry.getKey();
      }
      return sum;
    }

    public void markOff(int el) {
      if (vals.containsKey(el)) {
        Coord c = vals.get(el);
        hits[c.x][c.y] = true;
      }
    }
  }

  public static class Data {
    List<Integer> calledInts;
    final List<Board> boards;
    public Data() {
      calledInts = new ArrayList<>();
      boards = new ArrayList<>();
    }
  }

  public static Data parseLines(List<String> lines) {
      Data data = new Data();
      data.calledInts = Arrays.stream(lines.get(0).split(","))
                              .map(Integer::parseInt)
                              .collect(Collectors.toList());
      int i = 0;
      int size = 0;
      Board currentBoard = null;
      for (int l = 2; l < lines.size(); l++) {
        if (lines.get(l).isBlank()) continue; 

        String[] stringLine = lines.get(l).trim().split("\\s+");
        if (i == 0) {
          size = stringLine.length;
          currentBoard = new Board(size);
        }

        for (int j = 0; j < stringLine.length; j++) {
          int elInt = Integer.parseInt(stringLine[j]);
          currentBoard.vals.put(elInt, new Coord(i, j));
        }
        i++;
        if (i == size) {
          i = 0;
          data.boards.add(currentBoard);
        }
      }
      return data;
  }

  private static int partOne(Data data) {
    for (int el : data.calledInts) {
      for (Board b : data.boards) {
        b.markOff(el);
        if (b.isComplete()) {
          return el * b.sumUnmarked();
        }
      }
    }
    return -1;
  }

  private static int partTwo(Data data) {
    int lastWinVal = 0;
    Set<Integer> wonValues = new HashSet<>();
    for (int el : data.calledInts) {
      for (int i = 0; i < data.boards.size(); i++) {
        if (wonValues.contains(i)) continue;
        Board b = data.boards.get(i);
        b.markOff(el);
        if (b.isComplete()) {
          wonValues.add(i);
          lastWinVal = el * b.sumUnmarked();
        }
      }
    }
    return lastWinVal;
  }

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);
    Data data = parseLines(lines);

    int oneVal = partOne(data);
    System.out.println("Part 1: " + oneVal);
    int twoVal = partTwo(data);
    System.out.println("Part 2: " + twoVal);
  }

}
