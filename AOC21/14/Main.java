import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Files;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

public class Main { 
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);

    Polymer poly = new Polymer(lines);
    long tenDiff = poly.mostLeastDiff(poly.startStream, 10);
    System.out.println("Part One: " + tenDiff);
    long fortyDiff = poly.mostLeastDiff(poly.startStream, 40);
    System.out.println("Part Two: " + fortyDiff);

  } // main

  record Pair(char x, char y) {}
  record LevelPair(int l, char x, char y) {}

  static class Polymer {
    public List<Character> startStream;
    Map<Pair, Character> initialMap;
    Map<LevelPair, long[]> memoMap;

    public Polymer(List<String> lines) {
      startStream = new ArrayList<>();
      initialMap = new HashMap<>();
      memoMap = new HashMap<>();
      for (int i = 0; i < lines.get(0).length(); i++) {
        char c = lines.get(0).charAt(i);
        startStream.add(c);
      }
      for (int i = 2; i < lines.size(); i++) {
        String line = lines.get(i);
        char a = line.charAt(0);
        char b = line.charAt(1);
        char val = line.charAt(6);
        initialMap.put(new Pair(a, b), val);
      }
    } // Polymer

    long[] pairDict(LevelPair p) {
      if (memoMap.containsKey(p)) {
        return memoMap.get(p);
      }
      long[] charMap = new long[26];
      if (p.l() == 0) {
        charMap[(int) p.x() - 'A'] += 1;
        charMap[(int) p.y() - 'A'] += 1;
      } else {
        char mid = initialMap.get(new Pair(p.x(), p.y()));
        long[] left = pairDict(new LevelPair(p.l() - 1, p.x(), mid));
        long[] right = pairDict(new LevelPair(p.l() - 1, mid, p.y()));
        for (int i = 0; i < 26; i++) {
          charMap[i] = left[i] + right[i];
        }
        int midIdx = mid - 'A';
        charMap[midIdx] -= 1;
      }
      memoMap.put(p, charMap);
      return charMap;
    } // pairDict

    long[] fullMap(List<Character> stream, int level) {
      char a = stream.get(0);
      char b = stream.get(1);
      long[] charMap = pairDict(new LevelPair(level, a, b)).clone();
      for (int i = 1; i < stream.size() - 1; i++) {
        a = stream.get(i);
        b = stream.get(i + 1);
        long[] nextMap = pairDict(new LevelPair(level, a, b));
        for (int j = 0; j < 26; j++) {
          charMap[j] += nextMap[j];
        }
        charMap[(int) a - 'A'] -= 1;
      }
      return charMap;
    } // fullMap

    public long mostLeastDiff(List<Character> stream, int level) {
      long[] charMap = fullMap(stream, level);
      long most = 0;
      long least = Long.MAX_VALUE;
      for (long count : charMap) {
        if (count > 0) {
          most = Math.max(most, count);
          least = Math.min(least, count);
        }
      }
      return most - least;
    } // mostLeastDiff

  }; // Polymer class

} // Main class
