import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Set;

public class Main {
  private static final Path FILE = Path.of("input.txt");
  private static final String EMPTY_HALL = ".".repeat(7); // 7 x "."
  private static final char[] AMPHS = {'A', 'B', 'C', 'D'}; // 4 amphipods
  private static final int[] HALL_POS = {0, 1, 3, 5, 7, 9, 10};
  private static final int[] ROOM_POS = {2, 4, 6, 8};

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);

    State initialOne = getInitial(lines, false);
    int partOne = new Router(initialOne).solve();
    System.out.println("Part One: " + partOne);

    State initialTwo = getInitial(lines, true);
    int partTwo = new Router(initialTwo).solve();
    System.out.println("Part Two: " + partTwo);
  } // main

  record State(String hall, String roomA, String roomB, String roomC, String roomD) {
    String charRoom(char c) {
      return switch (c) {
        case 'A' -> roomA;
        case 'B' -> roomB;
        case 'C' -> roomC;
        case 'D' -> roomD;
        default -> throw new IllegalArgumentException("Invalid room: " + c);
      };
    }

    State roomToHall(char c, int ex, char roomChar, int hallLoc) {
      String oldRoom = charRoom(roomChar);
      String newRoom = oldRoom.substring(0, ex) + '.' + oldRoom.substring(ex + 1);
      String newHall = hall.substring(0, hallLoc) + c + hall.substring(hallLoc + 1);
      return switch (roomChar) {
        case 'A' -> new State(newHall, newRoom, roomB, roomC, roomD);
        case 'B' -> new State(newHall, roomA, newRoom, roomC, roomD);
        case 'C' -> new State(newHall, roomA, roomB, newRoom, roomD);
        case 'D' -> new State(newHall, roomA, roomB, roomC, newRoom);
        default -> throw new IllegalArgumentException("Invalid room: " + roomChar);
      };
    }

    State hallToRoom(char c, int ex, char roomChar, int hallLoc) {
      String oldRoom = charRoom(roomChar);
      String newRoom = oldRoom.substring(0, ex) + c + oldRoom.substring(ex + 1);
      String newHall = hall.substring(0, hallLoc) + '.' + hall.substring(hallLoc + 1);
      return switch (roomChar) {
        case 'A' -> new State(newHall, newRoom, roomB, roomC, roomD);
        case 'B' -> new State(newHall, roomA, newRoom, roomC, roomD);
        case 'C' -> new State(newHall, roomA, roomB, newRoom, roomD);
        case 'D' -> new State(newHall, roomA, roomB, roomC, newRoom);
        default -> throw new IllegalArgumentException("Invalid room: " + roomChar);
      };
    }
  }

  record Node(State state, int cost) {}

  // end State is:
  // State(".......", "AA", "BB", "CC", "DD")

  // #############
  // #...........#
  // ###A#B#C#D###
  //   #A#B#C#D#
  //   #########

  private static State getInitial(List<String> lines, boolean partTwo) {
    String roomA = "", roomB = "", roomC = "", roomD = "";
    int roomCount = 0;
    for (String line : lines) {
      if (line.indexOf('A') >= 0 || line.indexOf('B') >= 0 ||
          line.indexOf('C') >= 0 || line.indexOf('D') >= 0) {
        if (partTwo && roomCount == 1) {
          roomA = roomA + 'D';
          roomB = roomB + 'C';
          roomC = roomC + 'B';
          roomD = roomD + 'A';

          roomA = roomA + 'D';
          roomB = roomB + 'B';
          roomC = roomC + 'A';
          roomD = roomD + 'C';
        }
        roomA = roomA + line.charAt(3);
        roomB = roomB + line.charAt(5);
        roomC = roomC + line.charAt(7);
        roomD = roomD + line.charAt(9);
        roomCount++;
      }
    }
    return new State(EMPTY_HALL, roomA, roomB, roomC, roomD);
  } // getInitial

  private static List<Node> neighbours(Node n) {
    List<Node> possibilities = new ArrayList<>();
    possibilities.addAll(roomToHall(n));
    possibilities.addAll(hallToRoom(n));
    return possibilities;
  } // neighbours

  private static List<Node> hallToRoom(Node n) {
    State s = n.state();
    int cost = n.cost();
    List<Node> possibilities = new ArrayList<>();
    String hall = s.hall();
    for (int i = 0; i < hall.length(); i++) {
      char c = hall.charAt(i);
      if (c == '.') continue;
      String room = s.charRoom(c);
      boolean correct = true;
      for (int j = 0; j < room.length(); j++) {
        char cur = room.charAt(j);
        if (cur != '.' && cur != c) {
          correct = false;
          break;
        }
      }
      if (!correct) continue;
      int roomLoc = room.lastIndexOf('.');
      if (roomLoc == -1) continue;
      int path = walkHome(hall, c, i);
      if (path == -1) continue;
      State newState = s.hallToRoom(c, roomLoc, c, i);
      int newCost = cost + (path + roomLoc) * energy(c);
      possibilities.add(new Node(newState, newCost));
    }
    return possibilities;
  } // hallToRoom

  private static int walkHome(String hall, char c, int loc) {
    int destVal = c - 'A' + 1;

    if (loc == destVal || loc == destVal + 1) {
      return 2;
    }

    int dir = Integer.signum(destVal - loc);
    int curLoc = loc;
    while (curLoc != destVal && curLoc != destVal + 1) {
      curLoc += dir;
      if (hall.charAt(curLoc) != '.') return -1;
    }
    return Math.abs(HALL_POS[loc] - ROOM_POS[c - 'A']) + 1;
  } // walkHome

  private static List<Node> roomToHall(Node n) {
    State s = n.state();
    int cost = n.cost();

    List<Node> possibilities = new ArrayList<>();
    for (char c : AMPHS) {
      String room = s.charRoom(c);
      boolean corrFlag = true;
      int exitCount = 0;
      char top = '.';
      for (int i = room.length() - 1; i >= 0; i--) {
        char cur = room.charAt(i);
        if (cur == '.') {
          exitCount++;
          continue;
        }
        if (cur != c) corrFlag = false;
        top = cur;
      }

      if (corrFlag) continue;

      int[] options = hallOut(s.hall(), c);
      for (int i = 0; i < options.length; i++) {
        if (options[i] == 0) continue;
        State newState = s.roomToHall(top, exitCount, c, i);
        Node newNode = new Node(newState, cost + (options[i] + exitCount) * energy(top));
        possibilities.add(newNode);
      }
    }
    return possibilities;
  } // roomNeighbours

  private static int[] hallOut(String hall, char roomChar) {
    int[] outPoints = new int[7];
    int exitPoint = roomChar - 'A' + 1;
    int roomLoc = ROOM_POS[roomChar - 'A'];

    int loc = exitPoint;
    while (loc >= 0) {
      if (hall.charAt(loc) != '.') break;
      outPoints[loc] = Math.abs(HALL_POS[loc] - roomLoc) + 1; // includes walking out
      loc--;
    }

    loc = exitPoint + 1;
    while (loc < hall.length()) {
      if (hall.charAt(loc) != '.') break;
      outPoints[loc] = Math.abs(HALL_POS[loc] - roomLoc) + 1; // includes walking out
      loc++;
    }

    return outPoints;
  } // hallOut

  private static int energy(char c) {
    return (int) Math.pow(10, c - 'A');
  } // energy

  private static class Router {
    State initial;
    PriorityQueue<Node> queue = new PriorityQueue<>(Comparator.comparingInt(Node::cost));
    Set<State> visited = new HashSet<>();
    final State target;

    Router(State initial) {
      this.initial = initial;
      int rl = initial.roomA().length();
      target = new State(EMPTY_HALL, "A".repeat(rl), "B".repeat(rl), "C".repeat(rl), "D".repeat(rl));
    }

    int solve() {
      queue.add(new Node(initial, 0));

      while (!queue.isEmpty()) {
        Node current = queue.poll();
        State state = current.state();
        int cost = current.cost();

        if (!visited.add(state)) continue;

        if (state.equals(target)) {
          return cost;
        }

        for (Node next : neighbours(current)) {
          queue.add(next);
        }
      }
      return -1;
    } // solve

  } // Router class

} // Main class
