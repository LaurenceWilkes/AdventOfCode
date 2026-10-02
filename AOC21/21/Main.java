import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Files;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

public class Main {
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);
    String line = lines.get(0);
    int a = Integer.parseInt(line.substring(line.lastIndexOf(' ') + 1));
    line = lines.get(1);
    int b = Integer.parseInt(line.substring(line.lastIndexOf(' ') + 1));
    System.out.println("Player a: " + a + ", Player b: " + b);

    int out = simulateGame(a, b);
    System.out.println("Part One: " + out);

    Solver s = new Solver();
    WinDist wd = s.solve(a, b);
    System.out.println("Part Two: " + Math.max(wd.aWins(), wd.bWins()));
  } // main

  private static int mod10(int a) {
      return ((a - 1) % 10) + 1;
  }

  public static int simulateGame(int a, int b) {
    int t = 0;
    int player = 0;
    int[] pVals = {a, b};
    int[] pScores = {0, 0};
    while (true) {
      pVals[player] += 9 * t + 6;
      pVals[player] = mod10(pVals[player]); // mod 100 unnecessary...
      pScores[player] += pVals[player];
      if (pScores[player] >= 1000) {
        return 3 * (t + 1) * pScores[1 - player];
      }
      player = 1 - player;
      t++;
    }
  }

  private record State(int aVal, int bVal, boolean aTurn, int aPts, int bPts) {}
  private record WinDist(long aWins, long bWins) {}

  private static class Solver {
    Map<State, WinDist> memo = new HashMap<>();
    private static final int WIN_PTS = 21;
    final static int[] mult = {1, 3, 6, 7, 6, 3, 1};

    public WinDist solve(int a, int b) {
      return solvePart(a, b, true, 0, 0);
    }

    private WinDist solvePart(int aVal, int bVal, boolean aTurn, int aPts, int bPts) {
      if (aPts >= WIN_PTS) return new WinDist(1, 0);
      if (bPts >= WIN_PTS) return new WinDist(0, 1);

      State st = new State(aVal, bVal, aTurn, aPts, bPts);
      WinDist cached = memo.get(st);
      if (cached != null) {
        return cached;
      }

      long aWins = 0;
      long bWins = 0;
      WinDist currentWD;
      for (int r = 3; r <= 9; r++) {
        if (aTurn) {
          int naVal = mod10(aVal + r);
          currentWD = solvePart(naVal, bVal, !aTurn, aPts + naVal, bPts);
        } else {
          int nbVal = mod10(bVal + r);
          currentWD = solvePart(aVal, nbVal, !aTurn, aPts, bPts + nbVal);
        }
        int m = mult[r - 3];
        aWins += m * currentWD.aWins();
        bWins += m * currentWD.bWins();
      }

      WinDist wd = new WinDist(aWins, bWins);
      memo.put(st, wd);
      return wd;
    } // solvePart

  } // Solver class

} // Main class
