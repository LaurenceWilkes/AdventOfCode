import java.nio.file.Files;
import java.nio.file.Path;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.HashSet;
import java.util.Queue;
import java.util.LinkedList;
import java.util.Map;
import java.util.HashMap;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  record Coord(int row, int col) {}

  private static class Grid {
    Map<Coord, Integer> map;
    int width;
    int height;
    public Grid(int width, int height) {
      this.width = width;
      this.height = height;
      map = new HashMap<>();
    }
    public boolean isMinima(Coord c) {
      if (!map.containsKey(c)) return false;
      int val = map.get(c);
      Coord cu = new Coord(c.row - 1, c.col);
      Coord cd = new Coord(c.row + 1, c.col);
      Coord cl = new Coord(c.row, c.col - 1);
      Coord cr = new Coord(c.row, c.col + 1);
      if (map.containsKey(cu)) {if (map.get(cu) <= val) return false;}
      if (map.containsKey(cd)) {if (map.get(cd) <= val) return false;}
      if (map.containsKey(cl)) {if (map.get(cl) <= val) return false;}
      if (map.containsKey(cr)) {if (map.get(cr) <= val) return false;}
      return true;
    }
    public int buildBasin(Coord curr) {
      if (!isMinima(curr)) return 0;
      Queue<Coord> toCheck = new LinkedList<>();
      toCheck.add(curr);
      Set<Coord> basin = new HashSet<>();
      basin.add(curr);
      int count = 1;
      while (!toCheck.isEmpty()) {
        Coord c = toCheck.poll();
        int val = map.get(c);
        Coord[] cs = { 
          new Coord(c.row - 1, c.col),
          new Coord(c.row + 1, c.col),
          new Coord(c.row, c.col - 1),
          new Coord(c.row, c.col + 1)
        };
        for (Coord cDir : cs) {
          if (map.containsKey(cDir) && !basin.contains(cDir)) {
            int cDirVal = map.get(cDir);
            if (cDirVal != 9 && cDirVal > val) {
              basin.add(cDir);
              toCheck.add(cDir); 
            }
          }
        }
      }
      return basin.size();
    }
  }

  public static Grid parseLines(List<String> fileLines) {
    int height = fileLines.size();
    int width = fileLines.get(0).length();
    Grid g = new Grid(width, height);
    for (int i = 0; i < fileLines.size(); i++) {
      int j = 0;
      for (char c : fileLines.get(i).toCharArray()) {
        int val = c - '0';
        g.map.put(new Coord(i, j), val);
        j++;
      }
    }
    return g;
  }

  public static int countRisk(Grid g) {
    int riskSum = 0;
    for (int i = 0; i < g.height; i++) {
      for (int j = 0; j < g.width; j++) {
        Coord c = new Coord(i, j);
        if (g.isMinima(c)) {
          riskSum += g.map.get(c) + 1;
        }
      }
    }
    return riskSum;
  }

  public static int basinProd(Grid g) {
    int[] basins = {0, 0, 0};
    for (int i = 0; i < g.height; i++) {
      for (int j = 0; j < g.width; j++) {
        Coord c = new Coord(i, j);
        int bSize = g.buildBasin(c);
        if (bSize >= basins[0]) {
          basins[2] = basins[1];
          basins[1] = basins[0];
          basins[0] = bSize;
        } else if (bSize >= basins[1]) {
          basins[2] = basins[1];
          basins[1] = bSize;
        } else if (bSize >= basins[2]) {
          basins[2] = bSize;
        } 
      }
    }
    return basins[0] * basins[1] * basins[2];
  }

  public static void main(String[] args) throws IOException {
    List<String> fileLines = Files.readAllLines(FILE);
    Grid g = parseLines(fileLines);

    int valOne = countRisk(g);
    System.out.println("Part One: " + valOne);
    int valTwo = basinProd(g);
    System.out.println("Part Two: " + valTwo);
  }

}
