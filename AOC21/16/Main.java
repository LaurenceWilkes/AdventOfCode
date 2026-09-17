import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.ArrayList;

public class Main {

  public static void main(String[] args) throws IOException {
    String hex = Files.readString(Path.of("input.txt")).trim();

    Decoder d = new Decoder(hex);

    Packet packet = d.outerParse();
    System.out.println("Part One: " + packet.versionSum());
    System.out.println("Part Two: " + packet.eval());

  } // main

  record ParsedPacket(Packet packet, int next) {}

  static class Packet {
      int version;
      int typeID;

      Packet(int version, int typeID) {
          this.version = version;
          this.typeID = typeID;
      }

      int versionSum() {
        return version;
      }

      long eval() {
        return -1L; // should never be seen...
      }

  } // Packet class

  static class LiteralPacket extends Packet {
      long value;

      LiteralPacket(int version, int typeID, long value) {
          super(version, typeID);
          this.value = value;
      }

      @Override
      long eval() {
          return value;
      }
  } // LiteralPacket class

 static class OperatorPacket extends Packet {
      boolean lengthType;
      int length;
      List<Packet> interior = new ArrayList<>();

      OperatorPacket(int version, int typeID, boolean lengthType, int length) {
          super(version, typeID);
          this.lengthType = lengthType;
          this.length = length;
      }

      void addPacket(Packet p) {
        interior.add(p);
      }

      @Override
      int versionSum() {
        int sum = version;
        for (Packet p : interior) {
          sum += p.versionSum();
        }
        return sum;
      }

      @Override
      long eval() {
        switch(typeID) {
          case 0:
            long sum = 0L;
            for (Packet p : interior) {
              sum += p.eval();
            }
            return sum;
          case 1:
            long prod = 1L;
            for (Packet p : interior) {
              prod *= p.eval();
            }
            return prod;
          case 2:
            long min = Long.MAX_VALUE;
            for (Packet p : interior) {
              long val = p.eval();
              if (val < min) {
                min = val;
              }
            }
            return min;
          case 3:
            long max = Long.MIN_VALUE;
            for (Packet p : interior) {
              long val = p.eval();
              if (val > max) {
                max = val;
              }
            }
            return max;
          case 5:
            return interior.get(0).eval() > interior.get(1).eval() ? 1L : 0L;
          case 6:
            return interior.get(0).eval() < interior.get(1).eval() ? 1L : 0L;
          case 7:
            return interior.get(0).eval() == interior.get(1).eval() ? 1L : 0L;
        }
        return -1L;
      }
  } // OperatorPacket class

  static class Decoder {
    boolean[] bitString;

    public Decoder(String hex) {
      bitString = new boolean[4 * hex.length()];
      for (int i = 0; i < hex.length(); i++) {
        int value = Character.digit(hex.charAt(i), 16);
        for (int j = 0; j < 4; j++) {
          bitString[4 * i + j] = ((value >> (3 - j)) & 1) == 1;
        }
      }
    } // Decoder

    private int readVal(int start, int len) {
      int value = 0;
      for (int i = 0; i < len; i++) {
        if (bitString[start + i]) {
          value += 1 << (len - 1 - i);
        }
      }
      return value;
    } // readVal


    Packet outerParse() {
      return parsePacket(0).packet();
    }

    ParsedPacket parsePacket(int start) {
      int version = readVal(start, 3);
      int typeID = readVal(start + 3, 3);
      if (typeID == 4) {
        return parseLiteralPacket(start, version, typeID);
      } else {
        return parseOperatorPacket(start, version, typeID);
      }
    } // parsePacket

    ParsedPacket parseLiteralPacket(int start, int version, int typeID) {
        int current = start + 6;
        long value = 0;
        while (true) {
            boolean contFlag = bitString[current];
            for (int i = 1; i <= 4; i++) {
                value = (value << 1) | (bitString[current + i] ? 1 : 0);
            }
            current += 5;
            if (!contFlag) break;
        }

        return new ParsedPacket(new LiteralPacket(version, typeID, value), current);
    }

    ParsedPacket parseOperatorPacket(int start, int version, int typeID) {
        int current = start + 6;
        boolean lengthType = bitString[current];
        current += 1;
        int length;
        if (lengthType) {
          length = readVal(current, 11);
          current += 11;
        } else {
          length = readVal(current, 15);
          current += 15;
        }
        OperatorPacket op = new OperatorPacket(version, typeID, lengthType, length);
        if (lengthType) {
          // Deal with sub-Packets ("length" of them)
          for (int n = 0; n < length; n++) {
            ParsedPacket sp = parsePacket(current);
            op.addPacket(sp.packet());
            current = sp.next();
          }
        } else {
          // Deal with sub-Packets (of length "length")
          int end = current + length;
          while (current < end) {
            ParsedPacket sp = parsePacket(current);
            op.addPacket(sp.packet());
            current = sp.next();
          }
        }
        return new ParsedPacket(op, current);
    } // parseOperatorPacket

  } // Decoder class

} // Main class
