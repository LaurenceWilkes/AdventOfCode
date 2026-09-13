import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Set;
import java.util.HashSet;
import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Collections;

public class Main { 
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);

    Page page = new Page(lines);

    page.performInstruction(page.instructions.get(0));
    System.out.println("Part One: " + page.countPoints());

    Page newPage = new Page(lines);

    newPage.performAllInstructions();
    System.out.println("Part Two: ");
    newPage.display();

  } // main

  record Instruction(boolean vert, int val) {}
  record Point(int x, int y) {}

  static class Page {
    List<Integer> xs;
    List<Integer> ys;
    public List<Instruction> instructions;

    public Page(List<String> lines) {
      instructions = new ArrayList<>();
      xs = new ArrayList<>();
      ys = new ArrayList<>();
      for (String line : lines) {
        if (line.isEmpty()) continue;
        if (Character.isDigit(line.charAt(0))) {
          String[] p = line.split(",");
          xs.add(Integer.parseInt(p[0]));
          ys.add(Integer.parseInt(p[1]));
        } else {
          boolean vert = line.charAt(11) == 'x';
          int val = Integer.parseInt(line.substring(13));
          instructions.add(new Instruction(vert, val));
        }
      }
    } // Page

    public void performInstruction(Instruction inst) {
      if (inst.vert()) {
        for (int i = 0; i < xs.size(); i++) {
          xs.set(i, inst.val() - Math.abs(xs.get(i) - inst.val()));
        }
      } else {
        for (int j = 0; j < ys.size(); j++) {
          ys.set(j, inst.val() - Math.abs(ys.get(j) - inst.val()));
        }
      }
    } // performInstruction

    public int countPoints() {
      Set<Point> pointSet = new HashSet<>();
      for (int i = 0; i < xs.size(); i++) {
        pointSet.add(new Point(xs.get(i), ys.get(i)));
      }
      return pointSet.size();
    } // countPoints

    public void performAllInstructions() {
      for (Instruction inst : instructions) {
        performInstruction(inst);
      }
    }

    public void display() {
      int xmax = Collections.max(xs);
      int xmin = Collections.min(xs);
      int ymax = Collections.max(ys);
      int ymin = Collections.min(ys);
      List<Point> xys = new ArrayList<>();
      for (int i = 0; i < xs.size(); i++) {
        xys.add(new Point(xs.get(i), ys.get(i)));
      }
      xys.sort(Comparator.comparingInt(Point::y));
      int idx = 0;
      for (int j = ymin; j <= ymax; j++) {
        boolean[] lineVals = new boolean[xmax + 1 - xmin];
        while (idx < xys.size() && xys.get(idx).y() == j) {
          lineVals[xys.get(idx).x() - xmin] = true;
          idx++;
        }
        for (int i = 0; i < lineVals.length; i++) {
          if (lineVals[i]) {
            System.out.print("#");
          } else {
            System.out.print(".");
          }
        }
        System.out.print("\n");
      }
    }

  } // Page class

} // Main class

