import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;

public class Main {
    public static ArrayList<String> readFile() {
	File input = new File("input.txt");
	ArrayList<String> out = new ArrayList<String>();
	try (Scanner reader = new Scanner(input)) {
	    while (reader.hasNextLine()) {
	        String data = reader.nextLine();
		out.add(data);
	    }
	} catch (FileNotFoundException e) {
	    e.printStackTrace();
	}
	return out;
    }

    public static void partOne() {
	ArrayList<String> fileData = readFile();
	int position = 0;
	int depth = 0;
	for (String line : fileData) {
            char initChar = line.charAt(0);
            String valString = line.replaceAll("\\D", "");
	    int val = Integer.parseInt(valString);
	    switch (initChar) {
		case 'f':
		    position += val;
		    break;
		case 'd':
		    depth += val;
		    break;
		case 'u':
		    depth -= val;
		    break;
	    }
	}
        System.out.println("Part 1: " + (position * depth));
    }

    public static void partTwo() {
	ArrayList<String> fileData = readFile();
	int position = 0;
	int depth = 0;
	int aim = 0;
	for (String line : fileData) {
            char initChar = line.charAt(0);
            String valString = line.replaceAll("\\D", "");
	    int val = Integer.parseInt(valString);
	    switch (initChar) {
		case 'f':
		    position += val;
                    depth += aim * val;
		    break;
		case 'd':
		    aim += val;
		    break;
		case 'u':
		    aim -= val;
		    break;
	    }
	}
        System.out.println("Part 2: " + (position * depth));
    }

    public static void main() {
      partOne();
      partTwo();
    }
}
