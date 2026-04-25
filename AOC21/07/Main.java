import java.nio.file.Files;
import java.nio.file.Path;
import java.io.IOException;
import java.util.*;
import java.util.stream.Collectors;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> fileLines = Files.readAllLines(FILE);
    List<Integer> nums = Arrays.stream(fileLines.get(0).split(","))
                               .map(Integer::parseInt)
                               .collect(Collectors.toList());
    System.out.println("Part One: " + partOne(nums));
    System.out.println("Part Two: " + partTwo(nums));
  }

  public static int partOne(List<Integer> nums) {
    Collections.sort(nums);
    int med = nums.get(nums.size() / 2);
    return linCost(nums, med);
  }

  public static int partTwo(List<Integer> nums) {
    int count = 0;
    for (int n : nums) count += n; 
    double mean = count / (double) nums.size();
    int a = (int) Math.floor(mean);
    return Math.min(triCost(nums, a), triCost(nums, a + 1));
  }

  public static int linCost(List<Integer> nums, int avgInt) {
    int count = 0;
    for (int n : nums) count += Math.abs(n - avgInt);
    return count;
  }

  public static int triCost(List<Integer> nums, int avgInt) {
    int count = 0;
    for (int n : nums) {
      int a = Math.abs(n - avgInt);
      count += (a * (a + 1)) / 2;
    }
    return count;
  }

}
