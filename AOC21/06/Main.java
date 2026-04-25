import java.nio.file.Files;
import java.nio.file.Path;
import java.io.IOException;
import java.util.*;
//import java.util.List;
//import java.util.Map;
//import java.util.HashMap;
//import java.util.Arrays;
import java.util.stream.Collectors;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  record Coord(int x, int y) {}

  public static void main(String[] args) throws IOException {
    List<String> fileLines = Files.readAllLines(FILE);
    List<Integer> nums = Arrays.stream(fileLines.get(0).split(","))
                               .map(Integer::parseInt)
                               .collect(Collectors.toList());
    System.out.println("Part One: " + countAll(nums, 80));
    System.out.println("Part Two: " + countAll(nums, 256));
  }

  public static long countAll(List<Integer> nums, int days) {
    long count = 0;
    for (int n : nums) count += countFish(n, days); 
    return count;
  }

  static Map<Coord, Long> memo = new HashMap<>();

  public static long countFish(int fish, int days) {
    Coord c = new Coord(fish, days);
    if (memo.containsKey(c)) return memo.get(c);
    if (days <= fish) return (long) 1;
    int newDays = days - fish - 1;
    long count = countFish(6, newDays) + countFish(8, newDays);
    memo.put(c, count);
    return count;
  }
}
