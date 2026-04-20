import java.io.File;
import java.io.FileNotFoundException; 
import java.util.Scanner;

public class Main {
    public static void partOne() throws FileNotFoundException {
	File data = new File("input.txt");
	int i = 0;
	int count = 0;
	int prevVal = 0;
	Scanner reader = new Scanner(data);
	while (reader.hasNextLine()) {
	    int val = Integer.parseInt(reader.nextLine().trim());
	    if (i > 0 && val > prevVal) {
		count++;
	    }
	    prevVal = val;
	    i++;
	}
	System.out.println("Part 1: " + count);
    }

    public static void partTwo() throws FileNotFoundException {
	File data = new File("input.txt");
	int i = 0;
	int count = 0;
	int pVal = 0;
	int ppVal = 0;
	int pppVal = 0;
	Scanner reader = new Scanner(data);
	while (reader.hasNextLine()) {
	    int val = Integer.parseInt(reader.nextLine().trim());
	    if (i > 2 && val > pppVal) {
		count++;
	    }
	    pppVal = ppVal;
	    ppVal = pVal;
	    pVal = val;
	    i++;
	}
	System.out.println("Part 2: " + count);
    }

    public static void main(String[] args) throws FileNotFoundException {
	partOne();
	partTwo();
    }
}
