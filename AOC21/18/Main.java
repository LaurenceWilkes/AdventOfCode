import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;


public class Main {
  private static final Path FILE = Path.of("input.txt");

  public static void main(String[] args) throws IOException {
    List<String> lines = Files.readAllLines(FILE);

    Decoder d = new Decoder(lines.get(0));
    SFN total = d.decode();

    for (int i = 1; i < lines.size(); i++) {
      d = new Decoder(lines.get(i));
      SFN next = new Pair(total, d.decode());
      next.reduce();
      total = next;
    }

    System.out.println("Part One: " + total.magnitude());

    int max = 0;
    for (int i = 0; i < lines.size(); i++) {
      for (int j = 0; j < lines.size(); j++) {
        if (i == j) continue;
        SFN left = new Decoder(lines.get(i)).decode();
        SFN right = new Decoder(lines.get(j)).decode();
        SFN current = new Pair(left, right);

        current.reduce();

        int magnitude = current.magnitude();
        if (magnitude > max) {
          max = magnitude;
        }
      }
    }

    System.out.println("Part Two: " + max);
  } // main

  public static sealed abstract class SFN permits Pair, Number {
    Pair parent;

    Number leftNeighbour() {
      if (parent == null) return null;
      SFN current = this;
      while (current.parent.right != current) {
        current = current.parent;
        if (current.parent == null) return null;
      }
      current = current.parent.left;
      while (current instanceof Pair p) {
        current = p.right;
      }
      return (Number) current;
    } // leftNeighbour

    Number rightNeighbour() {
      if (parent == null) return null;
      SFN current = this;
      while (current.parent.left != current) {
        current = current.parent;
        if (current.parent == null) return null;
      }
      current = current.parent.right;
      while (current instanceof Pair p) {
        current = p.left;
      }
      return (Number) current;
    } // rightNeighbour

    void reduce() {
      while (explode(0) || split());
    } // reduce

    abstract boolean explode(int depth);
    abstract boolean split();
    abstract int magnitude();

  } // SFN class

  public static final class Pair extends SFN {
    SFN left;
    SFN right;

    Pair(SFN left, SFN right) {
      this.left = left;
      this.right = right;
      left.parent = this;
      right.parent = this;
    }

    void replace(SFN oldChild, SFN newChild) {
      if (left == oldChild) {
        left = newChild;
        left.parent = this;
      } else if (right == oldChild) {
        right = newChild;
        right.parent = this;
      }
    } // replace

    @Override
    boolean explode(int depth) {
      if (depth >= 4 && left instanceof Number lNum && right instanceof Number rNum) {
        Number lneigh = leftNeighbour();
        if (lneigh != null) lneigh.value += lNum.value;

        Number rneigh = rightNeighbour();
        if (rneigh != null) rneigh.value += rNum.value;

        parent.replace(this, new Number(0));
        return true;
      }
      if (left.explode(depth + 1)) return true;
      if (right.explode(depth + 1)) return true;
      return false;
    } // explode

    @Override
    boolean split() {
      return left.split() || right.split();
    } // split

    @Override
    int magnitude() {
      return 3 * left.magnitude() + 2 * right.magnitude();
    }
  } // Pair class

  public static final class Number extends SFN {
    int value;

    Number(int value) {
      this.value = value;
    }

    @Override
    boolean explode(int depth) {
      return false;
    } // explode

    @Override
    boolean split() {
      if (value < 10) return false;

      SFN replacement = new Pair(
        new Number(value / 2),
        new Number(value / 2 + value % 2)
      );
      parent.replace(this, replacement);

      return true;
    } // split


    @Override
    int magnitude() {
      return value;
    } // magnitude

  } // Number class

  public static class Decoder {
    private final String s;
    private int pos = 0;

    Decoder(String s) {
      this.s = s;
    }

    SFN decode() {
      char c = s.charAt(pos);
      if (Character.isDigit(c)) {
        pos++;
        return new Number((int) c - '0');
      } else {
        pos++;
        SFN left = decode();
        pos++;
        SFN right = decode();
        pos++;
        return new Pair(left, right);
      }
    } // decode

  } // Decoder class

} // Main class
