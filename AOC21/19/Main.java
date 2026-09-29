import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Files;
import java.util.Set;
import java.util.HashSet;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;


public class Main {
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);
    List<Scanner> scanners = decode(lines);

    Survey surv = new Survey(scanners);
    Set<Point> allPoints = surv.resolve();

    System.out.println("Part One: " + allPoints.size());
    System.out.println("Part One: " + surv.centreDist());

  }

  record Point(int x, int y, int z) {
    Point orient(int orientation) {
      int face = orientation / 4;  // 6 choices
      int rot = orientation % 4;   // 4 choices
      int nx, ny, nz;
      switch (face) {
        case 0 -> { nx =  x; ny =  y; nz =  z; }
        case 1 -> { nx = -x; ny =  y; nz = -z; }
        case 2 -> { nx =  y; ny =  z; nz =  x; }
        case 3 -> { nx = -y; ny =  z; nz = -x; }
        case 4 -> { nx =  z; ny =  x; nz =  y; }
        case 5 -> { nx = -z; ny =  x; nz = -y; }
        default -> throw new AssertionError();
      }
      for (int i = 0; i < rot; i++) {
        int temp = ny;
        ny = -nz;
        nz = temp;
      }
      return new Point(nx, ny, nz);
    }

    Point minus(Point p) {
      return new Point(x - p.x(), y - p.y(), z - p.z());
    }

    Point plus(Point p) {
      return new Point(x + p.x(), y + p.y(), z + p.z());
    }

  }

  public static class Scanner {
    Set<Point> points;
    int orientation;
    Point centre;

    Scanner(Set<Point> points) {
      this.points = points;
      orientation = 0;
      centre = null;
    }

    boolean testScanner(Scanner s) {
      for (int o = 0; o < 24; o++) {
        Map<Point, Integer> centres = new HashMap<>();
        for (Point a : points) {
          Point aGlobal = centre.plus(a.orient(orientation));
          for (Point b : s.points) {
            Point candidateCentre = aGlobal.minus(b.orient(o));
            int count = centres.merge(candidateCentre, 1, Integer::sum);
            if (count >= 12) {
              s.orientation = o;
              s.centre = candidateCentre;
              return true;
            }
          }
        }
      }
      return false;
    }

  } // Scanner class

  public static class Survey {
    static List<Scanner> scanners;

    Survey(List<Scanner> scanners) {
      this.scanners = scanners;
    }

    public static Set<Point> resolve() {
      scanners.get(0).centre = new Point(0, 0, 0);
      Set<Point> allBeacons = new HashSet<>(scanners.get(0).points);
      boolean[] resolved = new boolean[scanners.size()];
      resolved[0] = true;
      int resolvedCount = 1;
      while (resolvedCount < scanners.size()) {
        for (int i = 0; i < scanners.size(); i++) {
          if (!resolved[i]) continue;
          Scanner a = scanners.get(i);
          for (int j = 0; j < scanners.size(); j++) {
            if (resolved[j]) continue;
            Scanner b = scanners.get(j);
            if (a.testScanner(b)) {
              for (Point p : b.points) {
                allBeacons.add(b.centre.plus(p.orient(b.orientation)));
              }
              resolved[j] = true;
              resolvedCount++;
            }
          }
        }
      }
      return allBeacons;
    }

    public static int centreDist() {
      List<Point> centres = new ArrayList<>();
      for (Scanner s : scanners) {
        centres.add(s.centre);
      }
      int maxDist = 0;
      for (Point a : centres) {
        for (Point b : centres) {
          Point d = a.minus(b);
          int dist = Math.abs(d.x()) + Math.abs(d.y()) + Math.abs(d.z());
          if (dist > maxDist) {
            maxDist = dist;
          }
        }
      }
      return maxDist;
    }

  }; // Survey class

  public static List<Scanner> decode(List<String> lines) {
    List<Scanner> scanners = new ArrayList<>();
    Set<Point> points = new HashSet<>();
    for (String line : lines) {
      if (line.isEmpty()) {
        scanners.add(new Scanner(new HashSet<>(points)));
        points.clear();
      } else if (line.charAt(4) != 's') {
        String[] strs = line.split(",");
        int x = Integer.parseInt(strs[0]);
        int y = Integer.parseInt(strs[1]);
        int z = Integer.parseInt(strs[2]);
        points.add(new Point(x, y, z));
      }
    }
    if (!points.isEmpty()) {
      scanners.add(new Scanner(new HashSet<>(points)));
    }
    return scanners;
  }

} // Main class


