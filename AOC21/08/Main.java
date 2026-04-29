import java.nio.file.Files;
import java.nio.file.Path;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  private static class Segment {
    int val;
    int size;
    public Segment(String s) {
      size = s.length();
      val = 0;
      for (char c : s.toCharArray()) {
        val |= (1 << ((int) (c - 'a')));
      }
    }
  }

  private static class Line {
    Segment[] key;
    Segment[] out;
    int one;
    int four;
    int seven;
    int eight;
    
    public Line(String[] key, String[] out) {
      this.key = new Segment[10];
      for (int i = 0; i < 10; i++) {
        Segment s = new Segment(key[i]);
        this.key[i] = s;
        if (s.size == 2) one = s.val;
        else if (s.size == 4) four = s.val;
        else if (s.size == 3) seven = s.val;
        else if (s.size == 7) eight = s.val;
      }
      this.out = new Segment[4];
      for (int i = 0; i < 4; i++) this.out[i] = new Segment(out[i]);
    }

    public int getAssignment(Segment s) {
      if (s.val == one) return 1;
      if (s.val == four) return 4;
      if (s.val == seven) return 7;
      if (s.val == eight) return 8;
      if (s.size == 6) {
        if ((four & s.val) == four) return 9;
        else if ((one | s.val) == eight) return 6;
        else return 0;
      } 
      if (s.size == 5) {
        if ((one & s.val) == one) return 3;
        else if ((four | s.val) == eight) return 2;
        else return 5;
      }
      return -1;
    }
  }

  public static List<Line> parseLines(List<String> fileLines) {
    List<Line> lines = new ArrayList<>();
    for (String el : fileLines) {
      String[] parts = el.split(" \\| ");
      String[] key = parts[0].trim().split(" ");
      String[] out = parts[1].trim().split(" ");
      lines.add(new Line(key, out));
    }
    return lines;
  }

  public static void main(String[] args) throws IOException {
    List<String> fileLines = Files.readAllLines(FILE);
    List<Line> lines = parseLines(fileLines);

    int valOne = countNums(lines);
    System.out.println("Part One: " + valOne);
    int valTwo = solveLines(lines);
    System.out.println("Part Two: " + valTwo);
  }

  private static int countNums(List<Line> lines) {
    int count = 0;
    for (Line l : lines) {
      for (int i = 0; i < 4; i++) {
        int ll = l.out[i].size;
        if (ll == 2 || ll == 4 || ll == 3 || ll == 7) count++; 
      }
    }
    return count;
  }
  
  private static int solveLines(List<Line> lines) {
    int sum = 0;
    for (Line l : lines) {
      int out = 0;
      int mult = 1;
      for (int i = 3; i >= 0; i--) {
        out += mult * l.getAssignment(l.out[i]);
        mult *= 10;
      }
      sum += out;
    }
    return sum;
  }


}
