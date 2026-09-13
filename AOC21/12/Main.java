import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Set;
import java.util.HashSet;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

public class Main { 
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);
    
    Graph g = new Graph(lines);
    System.out.println("Part One: " + g.countAllPaths(false));
    System.out.println("Part Two: " + g.countAllPaths(true));

  } // main

  static class Graph {
    private Map<String, List<String>> adj;

    public Graph(List<String> lines) {
      adj = new HashMap<>();
      for (String line : lines) {
        String[] p = line.split("-");
        adj.computeIfAbsent(p[0], _ -> new ArrayList<>()).add(p[1]);
        adj.computeIfAbsent(p[1], _ -> new ArrayList<>()).add(p[0]);
      }
    } // Graph

    public int countAllPaths(boolean canDoubleVisit) {
      if (canDoubleVisit) {
        return countPathsPlus("start", new HashSet<>(), false);
      } else {
        return countPaths("start", new HashSet<>());
      }
    }

    private int countPaths(String current, Set<String> visited) {
      if (current.equals("end")) return 1;
      int count = 0;

      Set<String> nextVisited = new HashSet<>(visited);
      if (Character.isLowerCase(current.charAt(0))) {
        nextVisited.add(current);
      }

      for (String next : adj.get(current)) {
        if (nextVisited.contains(next)) continue; 
        count += countPaths(next, nextVisited);
      }
      return count;
    }

    public int countPathsPlus(String current, Set<String> visited, boolean extraVisit) {
      if (current.equals("end")) {return 1;}
      int count = 0;

      Set<String> nextVisited = new HashSet<>(visited);
      if (Character.isLowerCase(current.charAt(0))) {
        nextVisited.add(current);
      }

      for (String next : adj.get(current)) {
        if (next.equals("start")) continue;

        boolean nextIsLower = Character.isLowerCase(next.charAt(0));
        boolean beenVisited = nextVisited.contains(next);
        if (nextIsLower && beenVisited) {
          if (extraVisit) continue;
          count += countPathsPlus(next, nextVisited, true);
        } else {
          count += countPathsPlus(next, nextVisited, extraVisit);
        }
      }
      return count;
    } // countPathsPlus

  } // class Graph

} // Main class
