import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Deque;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  private static final Map<Character, Character> PAIRS = Map.of(
    '(', ')',
    '[', ']',
    '{', '}',
    '<', '>'
  );

  private static int score(char c) {
    return switch (c) {
      case ')' -> 3;
      case ']' -> 57;
      case '}' -> 1197;
      case '>' -> 25137;
      case '(' -> 1;
      case '[' -> 2;
      case '{' -> 3;
      case '<' -> 4;
      default -> 0;
    };
  }

  private static int partOne(List<String> lines) {
    int total = 0;
    for (String line : lines) {
      Deque<Character> stk = new ArrayDeque<>();
      for (int i = 0; i < line.length(); i++) {
        char c = line.charAt(i);
        if (PAIRS.containsKey(c)) {
          stk.push(c);
        } else if (stk.isEmpty()) {
          total += score(c);
          break;
        } else {
          char open = stk.pop();
          char alt = PAIRS.get(open);
          if (alt != c) {
            total += score(c);
            break;
          }
        }
      }
    }
    return total;
  }

  private static long partTwo(List<String> lines) {
    List<Long> totals = new ArrayList<>();
    for (String line : lines) {
      boolean contFlag = false;
      Deque<Character> stk = new ArrayDeque<>();
      for (int i = 0; i < line.length(); i++) {
        char c = line.charAt(i);
        if (PAIRS.containsKey(c)) {
          stk.push(c);
        } else if (stk.isEmpty()) {
          contFlag = true;
          break;
        } else {
          char open = stk.pop();
          char alt = PAIRS.get(open);
          if (alt != c) {
            contFlag = true;
            break;
          }
        }
      }
      if (contFlag) continue;
      long total = 0;
      while (!stk.isEmpty()) {
        char c = stk.pop(); 
        total *= 5;
        total += score(c);
      }
      totals.add(total);
    }
    totals.sort(null);
    return totals.get(totals.size() / 2);
  }

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);

    int valOne = partOne(lines);
    System.out.println("Part One: " + valOne);
    long valTwo = partTwo(lines);
    System.out.println("Part Two: " + valTwo);
  }
}
