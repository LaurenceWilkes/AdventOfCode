import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Set;
import java.util.HashSet;

public class Main {

  public static void main(String[] args) throws IOException {
    String line = Files.readString(Path.of("input.txt"));
    int[] numbers = Arrays.stream(line.split("[^-\\d]+"))
                    .filter(s -> !s.isEmpty())
                    .mapToInt(Integer::parseInt)
                    .toArray();
    System.out.println("Part One: " + solve(numbers));
    System.out.println("Part One: " + countAll(numbers));
  } // main

  public static int solve(int[] n) {
    int sx = n[0], ex = n[1];
    int sy = n[2], ey = n[3];
    int TMax = 2 * Math.max(Math.abs(sy), Math.abs(ey));         // If T > TMax, there can be no possible solutions for vx or vy.
    boolean noXcond = sx <= 0 && ex >= 0;                        // In this case vx can be selected as 0 and always be in bounds.
    int mx = Math.min(Math.abs(sx), Math.abs(ex));               // Pretends that the x bounds were positive all along;
    int Mx = Math.max(Math.abs(sx), Math.abs(ex));               // just solve the identical problem for positive vx.
    double sbmx = (-1 + Math.sqrt(1 + 8*mx)) / 2;                // The case where T >= vx
    double sbMx = (-1 + Math.sqrt(1 + 8*Mx)) / 2;
    int mvx = (int) Math.ceil(sbmx);                             // The smallest possible vx so that all T >= vx will always be in the range.
    boolean firstXBound = true;
    if (mvx > sbMx) firstXBound = false;                         // We check that this is even possible.
    int bestObj = 0;                                             // Highest y value
    for (int T = 1; T <= TMax; T++) {
      int lbvx = (int) Math.ceil(((double) mx) / T + ((double) T - 1) / 2);
      int ubvx = (int) Math.floor(((double) Mx) / T + ((double) T - 1) / 2);
      int lbvy = (int) Math.ceil(((double) sy) / T + ((double) T - 1) / 2);
      int ubvy = (int) Math.floor(((double) ey) / T + ((double) T - 1) / 2);
      if (ubvy < lbvy) continue;
      if (noXcond || (lbvx <= ubvx && T <= ubvx) || (firstXBound && T > mvx)) {
        int peak = ubvy <= T ? (ubvy * (ubvy + 1)) / 2 : (ubvy + 1) * T - ((T + 1) * T) / 2;
        if (peak > bestObj) {
          bestObj = peak;
        }
      }
    }
    return bestObj;
  } // solve

  record Pair(int x, int y) {}

  public static int countAll(int[] n) {
    Set<Pair> pairSet = new HashSet<>();
    int sx = n[0], ex = n[1];
    int sy = n[2], ey = n[3];
    int TMax = 2 * Math.max(Math.abs(sy), Math.abs(ey));
    int mx = Math.min(Math.abs(sx), Math.abs(ex));
    int Mx = Math.max(Math.abs(sx), Math.abs(ex));
    int sbmx = (int) Math.ceil((-1 + Math.sqrt(1 + 8*mx)) / 2);
    int sbMx = (int) Math.floor((-1 + Math.sqrt(1 + 8*Mx)) / 2);
    for (int T = 1; T <= TMax; T++) {
      int lbvx = (int) Math.ceil(((double) mx) / T + ((double) T - 1) / 2);
      int ubvx = (int) Math.floor(((double) Mx) / T + ((double) T - 1) / 2);
      int lbvy = (int) Math.ceil(((double) sy) / T + ((double) T - 1) / 2);
      int ubvy = (int) Math.floor(((double) ey) / T + ((double) T - 1) / 2);
      for (int vy = lbvy; vy <= ubvy; vy++) {
        for (int vx = sbmx; vx <= Math.min(T - 1, sbMx); vx++) {
          pairSet.add(new Pair(vx, vy));
        }
        for (int vx = Math.max(lbvx, T - 1); vx <= ubvx; vx++) {
          pairSet.add(new Pair(vx, vy));
        }
      }
    }
    return pairSet.size();
  }

} // Main class
