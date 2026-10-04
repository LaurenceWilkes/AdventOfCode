import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.ArrayList;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);

    Initialiser init = new Initialiser(lines);
    long val = init.countOn(50);
    System.out.println("Part One: " + val);

    Initialiser secondInit = new Initialiser(lines);
    long valTwo = init.countOn(Long.MAX_VALUE);
    System.out.println("Part Two: " + valTwo);
  } // main

  static class Cube {
    int xs, xl;
    int ys, yl;
    int zs, zl;

    Cube(int xs, int xl, int ys, int yl, int zs, int zl) {
      this.xs = xs; this.xl = xl;
      this.ys = ys; this.yl = yl;
      this.zs = zs; this.zl = zl;
    } // Cube

    public Cube intersect(Cube b) {
      int xsbl = xs - b.xl, xlbs = xl - b.xs;
      int ysbl = ys - b.yl, ylbs = yl - b.ys;
      int zsbl = zs - b.zl, zlbs = zl - b.zs;
      if (xsbl > 0 || ysbl > 0 || zsbl > 0 || xlbs < 0 || ylbs < 0 || zlbs < 0) {
        return null;
      }
      int xBot = Math.max(xs, b.xs); int xTop = Math.min(xl, b.xl);
      int yBot = Math.max(ys, b.ys); int yTop = Math.min(yl, b.yl);
      int zBot = Math.max(zs, b.zs); int zTop = Math.min(zl, b.zl);
      return new Cube(xBot, xTop, yBot, yTop, zBot, zTop);
    } // union

    long size() {
      return ((long) xl - xs + 1) * ((long) yl - ys + 1) * ((long) zl - zs + 1);
    } // size

    boolean lessThan(long val) {
      boolean xBool = Math.max(Math.abs(xs), Math.abs(xl)) <= val;
      boolean yBool = Math.max(Math.abs(ys), Math.abs(yl)) <= val;
      boolean zBool = Math.max(Math.abs(zs), Math.abs(zl)) <= val;
      return xBool && yBool && zBool;
    }

  } // Cube class

  static class Initialiser {
    List<Cube> cubes;
    List<Boolean> onOrOff;

    Initialiser(List<String> lines) {
      cubes = new ArrayList<>();
      onOrOff = new ArrayList<>();
      for (String line : lines) {
        String[] n = line.substring(line.indexOf("x=") + 2).split("[^0-9-]+");
        int xs = Integer.parseInt(n[0]); int xl = Integer.parseInt(n[1]);
        int ys = Integer.parseInt(n[2]); int yl = Integer.parseInt(n[3]);
        int zs = Integer.parseInt(n[4]); int zl = Integer.parseInt(n[5]);
        cubes.add(new Cube(xs, xl, ys, yl, zs, zl));
        onOrOff.add(line.startsWith("on"));
      }
    }

    long countOn(long limit) {
      List<Cube> cubeRecord = new ArrayList<>();
      List<Boolean> varRecord = new ArrayList<>();
      for (int i = 0; i < cubes.size(); i++) {
        Cube c = cubes.get(i);
        if (!c.lessThan(limit)) continue;
        int recordLen = cubeRecord.size();
        if (onOrOff.get(i)) {
          cubeRecord.add(c);
          varRecord.add(true);
        }
        for (int j = 0; j < recordLen; j++) {
          Cube cr = cubeRecord.get(j);
          Cube inter = c.intersect(cr);
          if (inter == null) continue;
          cubeRecord.add(inter);
          varRecord.add(!varRecord.get(j));
        }
      }
      long count = 0;
      for (int i = 0; i < cubeRecord.size(); i++) {
        long cSize = cubeRecord.get(i).size();
        count += varRecord.get(i) ? cSize : -cSize;
      }
      return count;
    } // countOn

  } // Initialiser class

} // Main class
