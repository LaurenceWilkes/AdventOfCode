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

  record Coord(int x, int y) {}

  private static class Line {
    final boolean isVert;
    final boolean isHori;
    final int xBeg;
    final int yBeg;
    final int xEnd;
    final int yEnd;

    public Line(int xBeg, int yBeg, int xEnd, int yEnd) {
      this.xBeg = xBeg;
      this.yBeg = yBeg;
      this.xEnd = xEnd;
      this.yEnd = yEnd;
      isVert = xBeg == xEnd ? true : false;
      isHori = yBeg == yEnd ? true : false;
    }

    public int length() {
      return Math.max(Math.abs(yEnd - yBeg), Math.abs(xEnd - xBeg));
    }

    public Line intersect(Line b) {
      if (this.isVert && b.isVert && this.xBeg == b.xBeg) {
        int thisMin = Math.min(this.yBeg, this.yEnd);
        int bMin = Math.min(b.yBeg, b.yEnd);
        int thisMax = Math.max(this.yBeg, this.yEnd);
        int bMax = Math.max(b.yBeg, b.yEnd);
        if (thisMin > bMax || thisMax < bMin) {return null;}
        return new Line(b.xBeg, Math.max(thisMin, bMin), b.xBeg, Math.min(thisMax, bMax));
      } else if (this.isHori && b.isHori && this.yBeg == b.yBeg) {
        int thisMin = Math.min(this.xBeg, this.xEnd);
        int bMin = Math.min(b.xBeg, b.xEnd);
        int thisMax = Math.max(this.xBeg, this.xEnd);
        int bMax = Math.max(b.xBeg, b.xEnd);
        if (thisMin > bMax || thisMax < bMin) {return null;}
        return new Line(Math.max(thisMin, bMin), b.yBeg, Math.min(thisMax, bMax), b.yBeg);
      } else if (this.isHori && b.isVert) {
        int byMin = Math.min(b.yBeg, b.yEnd);
        int byMax = Math.max(b.yBeg, b.yEnd);
        int thisxMin = Math.min(this.xBeg, this.xEnd);
        int thisxMax = Math.max(this.xBeg, this.xEnd);
        if (byMin <= this.yBeg && this.yBeg <= byMax && thisxMin <= b.xBeg && b.xBeg <= thisxMax) {
          return new Line(b.xBeg, this.yBeg, b.xBeg, this.yBeg);
        }
      } else if (this.isVert && b.isHori) {
        int bxMin = Math.min(b.xBeg, b.xEnd);
        int bxMax = Math.max(b.xBeg, b.xEnd);
        int thisyMin = Math.min(this.yBeg, this.yEnd);
        int thisyMax = Math.max(this.yBeg, this.yEnd);
        if (bxMin <= this.xBeg && this.xBeg <= bxMax && thisyMin <= b.yBeg && b.yBeg <= thisyMax) {
          return new Line(this.xBeg, b.yBeg, this.xBeg, b.yBeg);
        }
      }
      return null;
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

  private static int partOne(List<Line> lines) {
    Set<Coord> seen = new HashSet<>();
    int count = 0;
    for (int i = 0; i < lines.size() - 1; i++) {
      for (int j = i + 1; j < lines.size(); j++) {
        Line inter = lines.get(i).intersect(lines.get(j));
        if (inter == null) continue;
        int interLen = inter.length();
        int xInc = interLen != 0 ? (inter.xEnd - inter.xBeg) / interLen : 0;
        int yInc = interLen != 0 ? (inter.yEnd - inter.yBeg) / interLen : 0;
        for (int k = 0; k <= interLen; k++) {
          int x = inter.xBeg + k * xInc;
          int y = inter.yBeg + k * yInc;
          Coord c = new Coord(x, y);
          seen.add(c);
        }
      }
    }
    return seen.size();
  }

  private static int partTwo(List<Line> lines) {
    Set<Coord> seenOnce = new HashSet<>();
    Set<Coord> seenTwice = new HashSet<>();
    int count = 0;
    for (Line l : lines) {
      int lLen = l.length();
      int xInc = lLen != 0 ? (l.xEnd - l.xBeg) / lLen : 0;
      int yInc = lLen != 0 ? (l.yEnd - l.yBeg) / lLen : 0;
      for (int k = 0; k <= l.length(); k++) {
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

    int valOne = partOne(lines);
    System.out.println("Part One: " + valOne);
    int valTwo = partTwo(lines);
    System.out.println("Part Two: " + valTwo);
  }

}
