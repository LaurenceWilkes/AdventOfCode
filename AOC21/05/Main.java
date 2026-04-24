import java.nio.file.Files;
import java.nio.file.Path;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.HashSet;
import java.util.Arrays;
import java.util.stream.Collectors;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  record Coord(int x, int y) {}

  private static class Line {
    final boolean axisAdj;
    final int xBeg; final int yBeg;
    final int xEnd; final int yEnd;

    public Line(int xBeg, int yBeg, int xEnd, int yEnd) {
      this.xBeg = xBeg; this.yBeg = yBeg;
      this.xEnd = xEnd; this.yEnd = yEnd;
      axisAdj = yBeg == yEnd || xBeg == xEnd;
    }

    public int length() {
      return Math.max(Math.abs(yEnd - yBeg), Math.abs(xEnd - xBeg));
    }
  }

  public static List<Line> parseLines(List<String> fileLines) {
    List<Line> lines = new ArrayList<>();
    for (String el : fileLines) {
      List<Integer> nums = Arrays.stream(el.split("\\D+"))
                             .map(Integer::parseInt)
                             .collect(Collectors.toList());
      lines.add(new Line(nums.get(0), nums.get(1), nums.get(2), nums.get(3)));
    }
    return lines;
  }

  private static int bothParts(List<Line> lines, boolean partOne) {
    Set<Coord> seenOnce = new HashSet<>();
    Set<Coord> seenTwice = new HashSet<>();
    int count = 0;
    for (Line l : lines) {
      if (partOne && !l.axisAdj) continue;
      int lLen = l.length();
      int xInc = lLen != 0 ? (l.xEnd - l.xBeg) / lLen : 0;
      int yInc = lLen != 0 ? (l.yEnd - l.yBeg) / lLen : 0;
      for (int k = 0; k <= lLen; k++) {
        int x = l.xBeg + k * xInc;
        int y = l.yBeg + k * yInc;
        Coord c = new Coord(x, y);
        if (seenOnce.contains(c)) {
          if (!seenTwice.contains(c)) {
            count++;
            seenTwice.add(c);
          }
        } else {
          seenOnce.add(c);
        }
      }
    }
    return count;
  }

  public static void main(String[] args) throws IOException {
    List<String> fileLines = Files.readAllLines(FILE);
    List<Line> lines = parseLines(fileLines);

    int valOne = bothParts(lines, true);
    System.out.println("Part One: " + valOne);
    int valTwo = bothParts(lines, false);
    System.out.println("Part Two: " + valTwo);
  }

}
