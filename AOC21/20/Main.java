import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Files;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);
    Enhancer e = new Enhancer(lines);
    e.enhance();
    e.enhance();
    int trues2 = e.countTrues();
    System.out.println("Part One: " + trues2);
    for (int i = 0; i < 48; i++) {
      e.enhance();
    }
    int trues50 = e.countTrues();
    System.out.println("Part Two: " + trues50);
  } // main

  record Pair(int i, int j) {}

  public static class Enhancer {
    boolean[] alg;
    int xmin = 0;
    int xmax;
    int ymin = 0;
    int ymax;
    Map<Pair, Boolean> lit;  // cannot hold all lit values when the first el of alg is '#'
    Map<Pair, Boolean> ping; // ping pong map to hold the next vals
    boolean ambient = false;

    Enhancer(List<String> lines) {
      alg = new boolean[512];
      for (int i = 0; i < 512; i++) {
        if (lines.get(0).charAt(i) == '#') {
          alg[i] = true;
        }
      }
      int ySize = lines.size() - 2;
      int xSize = 0;
      lit = new HashMap<>();
      ping = new HashMap<>();
      for (int j = 0; j < ySize; j++) {
        String line = lines.get(j + 2);
        xSize = line.length();
        for (int i = 0; i < xSize; i++) {
          Pair key = new Pair(i, j);
          boolean val = false;
          if (line.charAt(i) == '#') val = true;
          lit.put(key, val);
        }
      }
      xmax = xSize - 1;
      ymax = ySize - 1;
    } // Enhancer

    public void display() {
      for (int i = xmin; i <= xmax; i++) {
        for (int j = ymin; j <= ymax; j++) {
          String c = lit.get(new Pair(i, j)) ? "#" : ".";
          System.out.print(c);
        }
        System.out.print("\n");
      }
    } // display

    private void updateAmbient() {
      ambient = ambient ? alg[511] : alg[0];
    }

    private int surroundNum(int i, int j) {
      int num = 0;
      for (int dj = j - 1; dj <= j + 1; dj++) {
        for (int di = i - 1; di <= i + 1; di++) {
          Pair c = new Pair(di, dj);
          num = num << 1;
          if (lit.getOrDefault(c, ambient)) {
            num = num | 1;
          }
        }
      }
      return num;
    }

    public void enhance() {
      ping.clear();
      xmin--; ymin--;
      xmax++; ymax++;
      for (int i = xmin; i <= xmax; i++) {
        for (int j = ymin; j <= ymax; j++) {
          int idx = surroundNum(i, j);
          boolean val = alg[idx];
          Pair key = new Pair(i, j);
          ping.put(key, val);
        }
      }
      Map<Pair, Boolean> temp = lit;
      lit = ping;
      ping = temp;
      updateAmbient();
    } // enhance

    public int countTrues() {
      int count = 0;
      for (int i = xmin; i <= xmax; i++) {
        for (int j = ymin; j <= ymax; j++) {
          if (lit.get(new Pair(i, j))) {
            count++;
          }
        }
      }
      return count;
    } // countTrues

  } // Enhancer class

} // Main class
