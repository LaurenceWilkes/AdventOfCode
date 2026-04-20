import java.nio.file.Files;
import java.nio.file.Path;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);
    int bitLen = lines.get(0).length();

    partOne(lines);
    partTwo(lines);
  }

  public static int binToInt(String str) {
    int bitLen = str.length();
    int cur = 0;
    int powVal = 1;
    for (int i = bitLen - 1; i >= 0; i--) {
      if (str.charAt(i) == '1') cur |= powVal;
      powVal <<= 1;
    }
    return cur;
  }

  private static void partOne(List<String> lines) {
    int len = lines.size();
    int bitLen = lines.get(0).length();
    int[] counts = new int[bitLen];
    for (String s : lines) {
      for (int i = 0; i < bitLen; i++) {
        if (s.charAt(i) == '1') counts[i]++;
      }
    }
    String gammaString = "";
    String epsilonString = "";
    for (int i = 0; i < bitLen; i++) {
      gammaString += counts[i] > len / 2 ? '1' : '0';
      epsilonString += counts[i] > len / 2 ? '0' : '1';
    }
    int gamma = binToInt(gammaString);
    int epsilon = binToInt(epsilonString);
    System.out.println("Part 1: " + (gamma * epsilon));
  } 

  private static int rating(List<String> lines, boolean isOx) {
    int len = lines.size();
    int bitLen = lines.get(0).length();
    ArrayList<String> remaining = new ArrayList<String>(lines);
    ArrayList<String> next = new ArrayList<String>(len);

    for (int i = 0; i < bitLen; i++) {
      int remSize = remaining.size();
      if (remSize <= 1) break;
      int count = 0;
      for (String el : remaining) {
        if (el.charAt(i) == '1') count++;
      }
      char val;
      if (isOx) {
        val = 2 * count >= remSize ? '1' : '0';
      } else {
        val = 2 * count >= remSize ? '0' : '1';
      }

      next.clear();
      for (String el : remaining) {
        if (el.charAt(i) == val) {
          next.add(el);
        }
      }
      ArrayList<String> temp = next;
      next = remaining;
      remaining = temp;
    }
    return binToInt(remaining.get(0));
  }

  private static void partTwo(List<String> lines) {
    int ox = rating(lines, true);
    int co = rating(lines, false);
    System.out.println("Part 2: " + (ox * co));
  } 
}
